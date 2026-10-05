# 单片机外设笔记
> 说明：以STM32F103为例，包含原理、公式、标准库代码、HAL库代码，Markdown格式，直接复制可用。

## 目录
- [1. GPIO 通用输入输出](#1-gpio-通用输入输出)
- [2. 外部中断 EXTI](#2-外部中断-exti)
- [3. 定时器 TIM（重点，含全套计算公式 + 输入捕获）](#3-定时器-tim重点含全套计算公式--输入捕获)
- [4. USART 串口](#4-usart-串口)
- [5. ADC 模数转换](#5-adc-模数转换)
- [6. I2C](#6-i2c)
- [7. SPI](#7-spi)
- [8. DMA 直接存储器访问（新增）](#8-dma-直接存储器访问新增)
- [9. 经典项目案例（关键代码+核心逻辑，不写完整工程）](#9-经典项目案例关键代码核心逻辑不写完整工程)
- [10. 考点总结 & 外设易错点](#10-考点总结--外设易错点)

---

## 1. GPIO 通用输入输出
### 原理
GPIO是通用IO引脚，可配置为推挽输出、开漏输出、浮空输入、上拉输入、下拉输入。
- 推挽输出：高低电平驱动能力强，常用LED
- 开漏输出：只能拉低，需要外部上拉，适合I2C
- 上拉输入：默认高电平，按键常用

### 标准库（StdPeriph）
```c
#include "stm32f10x.h"
void GPIO_Init_LED(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOC时钟

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
```

### HAL库
```c
#include "stm32f1xx_hal.h"
GPIO_InitTypeDef GPIO_InitStruct = {0};
void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
// 引脚置位复位
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
```

### 公式
GPIO无复杂运算，只有电气参数，无定时相关公式。

---

## 2. 外部中断 EXTI
### 原理
引脚电平变化触发中断，可配置上升沿、下降沿触发。EXTI线映射到GPIO，NVIC负责中断优先级。

### 标准库
```c
void EXTI_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
//中断服务函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        //用户代码
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

### HAL库
```c
void MX_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn,1,1);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
//中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        //用户代码
    }
}
```

### 公式
无计算公式，仅中断优先级配置。

---

## 3. 定时器 TIM（重点，含全套计算公式 + 输入捕获）
### 原理
STM32定时器分为基本定时器、通用定时器、高级定时器。
- 基本定时器：仅更新中断，无PWM/输入捕获
- 通用定时器：定时中断、PWM输出、输入捕获
- 高级定时器：增加死区、刹车，用于电机驱动

> 核心参数：**定时器时钟`Tclk`，预分频PSC，自动重装载ARR**

### ✅ 定时器基础公式
1. **定时器计数频率**
$$
F_{cnt} = \frac{T_{clk}}{PSC+1}
$$
$T_{clk}$：定时器输入时钟；PSC：预分频寄存器
> F103 APB1预分频≠1时，定时器时钟 = APB1时钟 ×2

2. **定时器溢出周期（中断周期）**
$$
T_{update} = \frac{(PSC+1)\times(ARR+1)}{T_{clk}}
$$
单位：秒

3. **溢出频率**
$$
F_{update} = \frac{T_{clk}}{(PSC+1)\times(ARR+1)}
$$

> 举例：Tclk=72MHz，PSC=7199，ARR=9999
> $F_{update}=72000000/(7200\times10000)=1\mathrm{Hz}$，1秒一次中断

### PWM补充公式
PWM周期和上面定时器溢出周期一致
$$
Duty = \frac{CCR}{ARR+1}\times100\%
$$
CCR：捕获比较寄存器，占空比。

### ✅ 输入捕获 原理 + 公式
输入捕获：检测引脚电平跳变，锁存当前定时器CNT值，用于测量**脉冲周期/脉冲宽度**。
> 常用：测量方波频率、高电平时间。

设：
- $F_{cnt}$：定时器计数频率
- $CNT_1$：第一次捕获值
- $CNT_2$：第二次捕获值
- $Cnt\_delta$：两次捕获计数值之差
- 发生$N$次定时器溢出

$$
Cnt\_total = Cnt\_delta + N\times(ARR+1)
$$
脉冲周期：
$$
T_{pulse}=\frac{Cnt\_total}{F_{cnt}}
$$
信号频率：
$$
F_{signal}=\frac{1}{T_{pulse}}
$$

> 例：$F_{cnt}=1\mathrm{MHz}$，捕获差值5000，则脉冲周期 = 5000 / 1000000 = 5ms，频率200Hz

### 标准库 定时中断
```c
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_InitStruct.TIM_Prescaler = 7199;  //PSC
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 9999;     //ARR
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
        //定时中断任务
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}
```

### HAL库 定时中断
```c
TIM_HandleTypeDef htim2;
void MX_TIM2_Init(void)
{
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 7199;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 9999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
}
//开启中断
HAL_TIM_Base_Start_IT(&htim2);
//定时器更新中断回调
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        //定时任务
    }
}
```

### 标准库 TIM输入捕获（TIM_CH1 PA0，测量脉冲）
```c
uint16_t IC_Value1 = 0,IC_Value2=0;
uint32_t Cnt_Delta = 0;
uint8_t Cap_State = 0; //0等待上升沿，1等待下降沿

void TIM5_IC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_ICInitTypeDef TIM_ICStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    //PA0 TIM5_CH1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //时基：72M/(71+1)=1MHz计数频率
    TIM_BaseStruct.TIM_Prescaler = 71;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 0xFFFF;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM_BaseStruct);

    TIM_ICStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
    TIM_ICStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICStruct.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM5,&TIM_ICStruct);

    TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM5,ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)
    {
        if(Cap_State == 0)
        {
            IC_Value1 = TIM_GetCapture1(TIM5);
            Cap_State = 1;
            TIM_SetIC1Polarity(TIM5,TIM_ICPolarity_Falling);
        }
        else if(Cap_State ==1)
        {
            IC_Value2 = TIM_GetCapture1(TIM5);
            if(IC_Value2>IC_Value1) Cnt_Delta = IC_Value2 - IC_Value1;
            else Cnt_Delta = (0xFFFF - IC_Value1)+IC_Value2;
            Cap_State = 0;
            TIM_SetIC1Polarity(TIM5,TIM_ICPolarity_Rising);
        }
        TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
    }
    //溢出中断，处理溢出计数
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
    }
}
```

### HAL库 TIM输入捕获（TIM5 CH1 PA0）
```c
TIM_HandleTypeDef htim5;
TIM_IC_InitTypeDef sConfigIC;
uint16_t IC1_Val1,IC1_Val2;
uint32_t CntDelta;
uint8_t cap_state=0;

void MX_TIM5_Init(void)
{
    htim5.Instance = TIM5;
    htim5.Init.Prescaler =71;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 0xFFFF;
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
}
//开启输入捕获
HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);

//捕获中断回调
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {
        if(cap_state ==0)
        {
            IC1_Val1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
            cap_state =1;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if(cap_state ==1)
        {
            IC1_Val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
            if(IC1_Val2>IC1_Val1)
                CntDelta = IC1_Val2 - IC1_Val1;
            else
                CntDelta = (0xFFFF - IC1_Val1)+IC1_Val2;
            cap_state =0;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}
```

---

## 4. USART 串口
### 原理
异步串行通信，TX发送，RX接收，波特率决定通信速度。
### 波特率公式（STM32）
$$
USART\_DIV = \frac{f_{APBx}}{16\times Baudrate}
$$
USART_DIV存到BRR寄存器。

> fAPBx：串口外设时钟；Baudrate目标波特率。

### 标准库
```c
void USART1_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);

    //TX PA9 推挽复用输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //RX PA10 浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_Init(USART1,&USART_InitStruct);
    USART_Cmd(USART1,ENABLE);
}
void USART_SendData(uint8_t data)
{
    USART_SendData(USART1,data);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}
```

### HAL库
```c
UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}
//发送
HAL_UART_Transmit(&huart1, (uint8_t*)"hello",5,100);
```

---

## 5. ADC 模数转换
### 原理
将模拟电压转为数字量。12位ADC，F103分辨率12bit。
### ADC公式
$$
V_{in} = \frac{D_{out}}{2^N-1}\times V_{ref}
$$
- $D_{out}$：ADC采样结果
- $\(N=12\)$，$2^{12}=4096$
- $V_{ref}$参考电压，常用3.3V

$$
V_{in} = \frac{D_{out}}{4095}\times3.3
$$

### 标准库
```c
void ADC1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef ADC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; //模拟输入
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&ADC_InitStruct);
    ADC_Cmd(ADC1,ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}
uint16_t ADC_Read(void)
{
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}
```

### HAL库
```c
ADC_HandleTypeDef hadc1;
void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
uint16_t ADC_Read(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,100);
    return HAL_ADC_GetValue(&hadc1);
}
```

---

## 6. I2C
### 原理
半双工串行总线，SDA数据线，SCL时钟线，多设备挂载，开漏输出。
通信速率标准100kHz，快速400kHz。

> I2C无复杂计算，仅时钟配置。

### 标准库（软件I2C，硬件I2C容易坑）
```c
void I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
}
```

### HAL库硬件I2C
```c
I2C_HandleTypeDef hi2c1;
void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}
//读写函数
HAL_I2C_Mem_Read(&hi2c1,DevAddr,MemAddr,I2C_MEMADD_SIZE_8BIT,buff,1,1000);
```

---

## 7. SPI
### 原理
高速同步串行总线，CLK时钟，MOSI主机发，MISO从机发，NSS片选。
时钟极性CPOL、相位CPHA决定4种SPI模式。

### SPI波特率公式
$$
F_{spi} = \frac{f_{APBx}}{Prescaler}
$$
Prescaler：2,4,8,16,32,64,128,256分频

### 标准库
```c
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);
    //PA5 SCK PA7 MOSI
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //PA6 MISO
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1,&SPI_InitStruct);
    SPI_Cmd(SPI1,ENABLE);
}
```

### HAL库
```c
SPI_HandleTypeDef hspi1;
void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}
uint8_t SPI_ReadWrite(uint8_t data)
{
    uint8_t ret;
    HAL_SPI_TransmitReceive(&hspi1,&data,&ret,1,100);
    return ret;
}
```

---

## 8. DMA 直接存储器访问（新增）
### 原理
DMA：Direct Memory Access，直接存储器访问。**不需要CPU参与，在外设与内存、内存与内存之间搬运数据**，CPU可以并行执行其他任务，大幅提升传输效率。
- DMA1：外设到内存，内存到外设；支持USART、ADC、TIM、SPI、I2C
- DMA2：仅大容量F1才有，支持内存到内存
传输方向：
1. 外设 → 内存（ADC连续采样）
2. 内存 → 外设（串口批量发送、PWM波形输出）
3. 内存 → 内存（仅DMA2）

关键概念：
- 外设地址：PeripheralBaseAddr
- 存储器地址：MemoryBaseAddr
- 传输数据量：BufferSize
- 数据宽度：8bit/16bit/32bit
- 循环模式Circular：循环传输，常用于ADC连续采样；正常模式Normal，传完一次停止

### DMA公式
DMA传输时间估算：
$$
T_{dma} = \frac{BufferSize\times DataWidth}{Clock_{DMA}}
$$
> DMA时钟来自AHB，F103 AHB=72MHz。
> 工程上一般不用精确计算，重点关注传输完成中断。

### 标准库：DMA ADC循环采样（外设→内存）
```c
uint16_t adc_buf[10];
void DMA_ADC_Init(void)
{
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adc_buf;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; //外设作为源
    DMA_InitStruct.DMA_BufferSize = 10;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; //循环模式
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1,&DMA_InitStruct);
    DMA_Cmd(DMA1_Channel1,ENABLE);
}
```

### HAL库 DMA 串口发送（内存→外设）
```c
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
uint8_t tx_buf[] = "DMA send test\r\n";

void MX_DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);
}
//启动DMA发送
HAL_UART_Transmit_DMA(&huart1,tx_buf,sizeof(tx_buf));
//DMA传输完成回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        //DMA发送完成，执行后续任务
    }
}
```

### DMA易错点
1. 必须开启AHB的DMA时钟（不是APB）
2. 内存地址必须对齐，半字/字传输时，数组地址要求2/4字节对齐
3. Circular循环模式：传输完成自动重装计数器，连续采集ADC最常用
4. 多DMA通道要配置优先级，防止抢占冲突
5. HAL库必须调用`__HAL_LINKDMA`把外设句柄和DMA句柄绑定

---

## 9. 经典项目案例（关键代码+核心逻辑，不写完整工程）
> 项目1：基于ADC+DMA的电压采集 + 串口上报
> 项目2：TIM PWM + 输入捕获 直流电机调速+测速
> 项目3：SPI+DMA驱动W25Q64，Flash读写
> 项目4：I2C读取MPU6050姿态传感器

### 项目1：ADC+DMA多通道电压采集，串口上报
**核心逻辑**
ADC多通道扫描模式 + DMA循环搬运采样值，CPU不参与搬运；定时读取缓冲区，通过串口上传电压数据。
- 外设：ADC1，DMA1，USART1，TIM2定时触发ADC
- 流程：TIM触发ADC采样 → DMA自动把ADC结果搬运到数组 → 定时读取数组计算电压 → 串口打印

**关键HAL代码**
```c
#define ADC_BUF_LEN 4
uint16_t adc_buf[ADC_BUF_LEN]; //4路ADC采样缓存

//DMA+ADC初始化（前面DMA代码+ADC扫描模式）
//定时器中断回调，读取并计算电压
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        float v0 = adc_buf[0] * 3.3f / 4095.0f;
        float v1 = adc_buf[1] * 3.3f / 4095.0f;
        //串口打印
        HAL_UART_Transmit(&huart1, (uint8_t*)"V0=",3,100);
    }
}
```

### 项目2：TIM PWM + 输入捕获，直流电机调速+测速
**核心逻辑**
TIM1输出PWM控制电机占空比调速；TIM5输入捕获测量电机编码器脉冲，计算转速。
转速公式：
$$
n = \frac{F_{signal}\times60}{Encoder\_Line}
$$
$F_{signal}$：编码器脉冲频率；Encoder_Line：编码器线数。

**关键HAL代码**
```c
//PWM设置占空比，控制电机
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, CCR_val);

//输入捕获回调，算出脉冲频率，计算转速
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {
        uint32_t total_cnt = CntDelta;
        float pulse_period = total_cnt / 1000000.0f;
        float freq = 1.0f / pulse_period;
        float rpm = freq * 60 / 11; //11线编码器
    }
}
```

### 项目3：SPI+DMA W25Q64 读写Flash
**核心逻辑**
SPI外设配合DMA批量读写，提升大数据读写速度；W25Q64操作命令：写使能、页写、读数据、扇区擦除。
> 重点：Flash写前必须擦除，擦除后字节为0xFF；一页最大256字节。

**关键HAL代码**
```c
//W25Q读取函数，DMA传输
uint8_t w25q_read_buf[256];
void W25Q_Read(uint32_t addr,uint8_t *buf,uint16_t len)
{
    W25Q_CS_L();
    uint8_t cmd[4]={0x03,(addr>>16)&0xFF,(addr>>8)&0xFF,addr&0xFF};
    HAL_SPI_Transmit(&hspi1,cmd,4,1000);
    //DMA读取
    HAL_SPI_Receive_DMA(&hspi1,buf,len);
    W25Q_CS_H();
}
```

### 项目4：I2C读取MPU6050（姿态传感器）
**核心逻辑**
I2C读取MPU6050寄存器，获取加速度、陀螺仪原始值；再进行单位换算。
加速度：±2g模式，分辨率 = 2*9.8 / 32768
陀螺仪：±250deg/s模式，分辨率 = 250 / 32768

**关键HAL代码**
```c
int16_t ax,ay,az,gx,gy,gz;
void MPU6050_ReadData(void)
{
    uint8_t rec_buf[14];
    //读取0x3B开始14字节数据
    HAL_I2C_Mem_Read(&hi2c1,0xD0,0x3B,I2C_MEMADD_SIZE_8BIT,rec_buf,14,1000);
    ax = (rec_buf[0]<<8)|rec_buf[1];
    ay = (rec_buf[2]<<8)|rec_buf[3];
    az = (rec_buf[4]<<8)|rec_buf[5];
    gx = (rec_buf[8]<<8)|rec_buf[9];
}
```

---

## 10. 考点总结 & 外设易错点
### 10.1 考点总结
1. **GPIO**
- 输出模式：推挽/开漏；输入模式：浮空、上拉、下拉、模拟输入
- 复用功能：串口TX、SPI、定时器PWM通道都需要配置复用推挽输出
2. **EXTI外部中断**
- EXTI线最多16根，PAx/PBx共用一条EXTI线，同一编号引脚不能同时做EXTI
- 中断服务函数标准库写`IRQHandler`；HAL库推荐写回调函数`HAL_GPIO_EXTI_Callback`
3. **TIM定时器（最高频考点）**
- APB1分频不为1时，定时器时钟×2；APB2不分频，直接等于APB2时钟
- 三个核心公式：计数频率、溢出周期、PWM占空比；输入捕获用于测周期/脉宽
- 三种模式：基础定时、PWM输出、输入捕获；高级定时器额外有刹车和死区
4. **USART串口**
- 异步通信，不需要时钟线；TX复用推挽输出，RX输入
- TXE：发送寄存器空；TC：发送完成；RXNE：接收寄存器非空
5. **ADC**
- 12bit，结果范围0~4095；参考电压决定量程
- 独立模式、扫描模式；软件触发/外部触发；采样时间影响精度
6. **I2C**
- 开漏输出，必须外部上拉；7bit/10bit从机地址；硬件I2C容易卡死，工程常用软件I2C
7. **SPI**
- 同步通信，自带SCK时钟；4种模式由CPOL+CPHA组合；MSB先行；NSS软件/硬件片选
8. **DMA（新增）**
- DMA不需要CPU搬运数据，AHB时钟；Normal单次传输，Circular循环传输；外设和内存双向搬运
- 典型场景：ADC连续采集、串口大批量收发、SPI高速读写Flash

### 10.2 易错点汇总
1. **时钟忘记开启**：所有外设第一步必须开启RCC时钟，代码最常见bug。DMA时钟属于AHB，不是APB。
2. **中断标志不清除**：标准库中断结束不清除PendingBit，会重复进中断；HAL库底层自动清标志，但捕获/更新中断仍要留意。
3. **定时器PSC、ARR理解错误**：PSC是预分频系数，实际分频为PSC+1，很多人直接写PSC=72而不是71。
4. **引脚模式配错**
    - PWM、SPI、USART TX必须配置复用推挽输出，不能普通推挽。
    - ADC引脚必须配置模拟输入，不能带上拉/下拉。
    - I2C SDA/SCL必须开漏输出。
5. **输入捕获溢出问题**：高频信号无溢出；低频信号定时器多次溢出，必须增加溢出计数变量，否则测量值偏小。
6. **串口波特率误差**：系统时钟配置错误，会带来波特率偏差，出现乱码。
7. **SPI模式不匹配**：主机从机CPOL、CPHA必须完全一致，否则读数据全错。
8. **NVIC优先级**：抢占优先级 > 子优先级；抢占优先级高可以打断低抢占优先级中断；子优先级不能打断。
9. **DMA易错**：内存地址对齐；循环模式自动重装；`__HAL_LINKDMA`绑定句柄；多通道DMA优先级冲突。

### 10.3 面试/笔试简答清单
- Q：推挽输出和开漏输出区别？
> 推挽：高低电平均可强驱动；开漏只能拉低，高电平靠外部上拉，支持线与。
- Q：定时器APB倍频规则？
> APB1预分频 >1，定时器时钟 = APB1时钟×2；APB1预分频=1，定时器时钟=APB1时钟。APB2定时器直接等于APB2时钟。
- Q：ADC分辨率怎么计算？
> $Resolution = V_{ref}/(2^N-1)$，\(N=12\)时，分辨率=3.3V/4095。
- Q：DMA的作用是什么？
> DMA直接在内存和外设间搬运数据，不占用CPU，提升传输效率，CPU可以并行处理其他任务。
- Q：DMA Normal模式与Circular循环模式区别？
> Normal：传输完成一次，计数器清零，停止；Circular：传输完成自动重装传输长度，持续循环传输。

---

这份笔记已经覆盖常用外设+DMA+4个经典工程案例。
你是否想要继续补充**RCC时钟系统**章节（全公式+标准库/HAL代码，时钟树讲解，单片机所有外设的根基）？