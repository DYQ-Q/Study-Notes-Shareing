# 定时器详解（附STM32标准库代码示例）

## 第一部分：时基单元

### 1.1 核心概念详解

时基单元是定时器的**心脏**，控制着定时器的基本计数功能。

#### 核心寄存器：
- **CNT**：计数器寄存器（16位或32位）
- **PSC**：预分频器寄存器（16位）
- **ARR**：自动重装载寄存器（16位或32位）
- **CR1**：控制寄存器1

#### 工作流程：
```
定时器时钟源 → 预分频器 → 计数器 → 比较ARR → 产生更新事件
```

#### 计算公式：
```c
定时周期 = (ARR + 1) * (PSC + 1) / F_定时器时钟
```

### 1.2 标准库配置代码

#### 基础时基配置示例：
```c
#include "stm32f10x.h"

/**
  * @brief  配置TIM2基本定时功能，产生1ms中断
  * @param  无
  * @retval 无
  */
void TIM2_TimeBase_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 2. 配置时基参数
    // 假设系统时钟72MHz，APB1分频系数为2，TIM2时钟为72MHz
    // 目标：1ms定时
    // 计算：PSC=71, ARR=999
    // 定时时间 = (999+1) * (71+1) / 72MHz = 1000 * 72 / 72MHz = 1ms
    TIM_TimeBaseStructure.TIM_Period = 999;         // ARR值
    TIM_TimeBaseStructure.TIM_Prescaler = 71;       // PSC值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频，用于滤波器
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 3. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 4. 配置NVIC（嵌套向量中断控制器）
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 5. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  TIM2中断服务函数
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 用户代码：定时任务
        // 例如：翻转LED
        // GPIO_WriteBit(GPIOB, GPIO_Pin_5, 
        //     (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)));
        
        // 清除中断标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```

#### 高级时基配置（中央对齐模式）：
```c
void TIM3_CenterAligned_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 2. 配置中央对齐模式
    // 常用于对称PWM生成
    TIM_TimeBaseStructure.TIM_Period = 999;          // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 71;        // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    // CenterAligned1: 向上/向下计数，中断/DMA在向下计数时产生
    // CenterAligned2: 向上/向下计数，中断/DMA在向上计数时产生
    // CenterAligned3: 向上/向下计数，中断/DMA在向上和向下计数时都产生
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_Cmd(TIM3, ENABLE);
}
```

## 第二部分：输出比较（OC）

### 2.1 工作原理详解

#### OC模式分类：
1. **PWM模式**（最常用）
   - PWM模式1：CNT<CCR时有效，CNT≥CCR时无效
   - PWM模式2：CNT<CCR时无效，CNT≥CCR时有效
   
2. **强制输出模式**
   - 强制为高/低电平
   
3. **翻转模式**
   - 匹配时翻转电平

4. **单脉冲模式**
   - 从门控信号触发，产生单个脉冲

#### PWM参数计算：
```c
PWM频率 = F_TIM_CLK / [(ARR + 1) * (PSC + 1)]
占空比 = CCR / (ARR + 1)
```

### 2.2 标准库配置代码

#### PWM输出配置（TIM2通道1）：
```c
/**
  * @brief  配置TIM2通道1输出PWM
  * @param  频率：1kHz，占空比：50%
  * @retval 无
  */
void TIM2_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 2. 配置GPIO为复用推挽输出（TIM2_CH1在PA0）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置时基（1kHz PWM）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 999;      // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 71;    // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 4. 配置PWM模式（通道1）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 500;                     // 占空比50%（CCR值）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 有效电平为高
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    // 5. 使能预装载寄存器
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // 6. 使能ARR预装载寄存器
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    
    // 7. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  动态改变PWM占空比
  * @param  duty: 占空比（0-1000对应0%-100%）
  * @retval 无
  */
void PWM_SetDuty(uint16_t duty)
{
    if (duty > 1000) duty = 1000;  // 限制范围
    
    // 方法1：直接设置CCR寄存器
    TIM_SetCompare1(TIM2, duty);
    
    // 方法2：通过库函数设置（效果相同）
    // TIM2->CCR1 = duty;
}

/**
  * @brief  配置互补输出（高级定时器TIM1）
  * @note   带死区时间的互补PWM，用于电机驱动
  */
void TIM1_ComplementaryPWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // 2. 配置主输出GPIO（TIM1_CH1: PA8）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置互补输出GPIO（TIM1_CH1N: PB13）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 4. 配置时基
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // 5. 配置PWM输出
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      // 主输出
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;    // 互补输出
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    // 6. 配置死区时间（防止上下桥臂直通）
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x5F;  // 死区时间值，根据实际需要调整
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;         // 刹车使能
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
    
    // 7. 启动定时器
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  // 高级定时器需要此函数
    TIM_Cmd(TIM1, ENABLE);
}
```

#### 输出比较翻转模式示例：
```c
void TIM4_OC_Toggle_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 2. 配置GPIO（TIM4_CH1: PD12）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    // 3. 配置时基（1kHz频率）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    // 4. 配置输出比较翻转模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  // 翻转模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;  // 计数到500时翻转
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    
    // 5. 启动定时器
    TIM_Cmd(TIM4, ENABLE);
}
```

## 第三部分：输入捕获（IC）

### 3.1 工作原理详解

#### 输入捕获模式：
1. **直接模式**：捕获信号直接进入通道
2. **间接模式**：捕获信号交叉进入通道（用于PWM输入模式）
3. **PWM输入模式**：专门用于测量PWM频率和占空比

#### 测量原理：
```
测量脉宽：
上升沿捕获 → 记录CNT值T1 → 下降沿捕获 → 记录CNT值T2
脉宽 = (T2 - T1) * 计数周期

测量频率：
第一次上升沿捕获 → 记录CNT值T1
第二次上升沿捕获 → 记录CNT值T2
频率 = 1 / [(T2 - T1) * 计数周期]
```

### 3.2 标准库配置代码

#### 输入捕获测量高电平脉宽：
```c
#include "stm32f10x.h"

// 全局变量
volatile uint32_t IC1Value = 0, IC2Value = 0;
volatile uint32_t Capture = 0;
volatile uint8_t Capture_Flag = 0;

/**
  * @brief  配置TIM5通道1输入捕获
  * @param  无
  * @retval 无
  */
void TIM5_IC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO为输入（TIM5_CH1在PA0）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // 下拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置时基
    // 72MHz/72 = 1MHz计数频率，1us计数一次
    // ARR设为最大值0xFFFFFFFF（32位定时器）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    
    // 4. 配置输入捕获参数
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   // 初始上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 直接模式
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;         // 不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x0;                       // 无滤波
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    
    // 5. 使能捕获中断
    TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
    
    // 6. 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 7. 启动定时器
    TIM_Cmd(TIM5, ENABLE);
}

/**
  * @brief  TIM5中断服务函数
  * @param  无
  * @retval 无
  */
void TIM5_IRQHandler(void)
{
    static uint8_t capture_stage = 0;  // 0:等待上升沿, 1:等待下降沿
    
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
    {
        switch (capture_stage)
        {
            case 0:  // 第一次捕获：上升沿
                IC1Value = TIM_GetCapture1(TIM5);  // 读取CCR1寄存器
                
                // 改变触发边沿为下降沿
                TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);
                
                // 清除捕获标志，准备下一次捕获
                TIM_SetCounter(TIM5, 0);
                
                capture_stage = 1;
                break;
                
            case 1:  // 第二次捕获：下降沿
                IC2Value = TIM_GetCapture1(TIM5);
                
                // 计算高电平脉宽（微秒）
                if (IC2Value > IC1Value)
                {
                    Capture = IC2Value - IC1Value;  // 单位为微秒
                }
                else
                {
                    // 处理计数器溢出情况（32位定时器很少溢出）
                    Capture = (0xFFFFFFFF - IC1Value) + IC2Value;
                }
                
                // 恢复为上升沿触发，准备下一次测量
                TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);
                
                capture_stage = 0;
                Capture_Flag = 1;  // 设置捕获完成标志
                break;
        }
        
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
    }
}

/**
  * @brief  获取测量的脉宽
  * @param  无
  * @retval 脉宽（微秒）
  */
uint32_t Get_PulseWidth(void)
{
    uint32_t width;
    
    // 等待测量完成
    while (Capture_Flag == 0);
    
    // 读取结果并清除标志
    width = Capture;
    Capture_Flag = 0;
    
    return width;  // 返回微秒数
}
```

#### PWM输入模式（测量频率和占空比）：
```c
/**
  * @brief  PWM输入模式配置（TIM3通道1和2）
  * @note   通道1捕获周期，通道2捕获脉宽
  */
void TIM3_PWMInput_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO（TIM3_CH1: PA6, TIM3_CH2: PA7）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置时基
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 1MHz计数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 4. 配置PWM输入模式
    // 通道1配置为上升沿捕获（用于测量周期）
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;  // 间接输入
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    // 通道2配置为下降沿捕获（用于测量脉宽）
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    // 5. 选择触发输入为通道1
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    
    // 6. 从模式配置为复位模式
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
    
    // 7. 使能中断
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
    
    // 8. 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 9. 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}

// 全局变量
volatile uint16_t IC1Value = 0, IC2Value = 0;
volatile float Frequency = 0, DutyCycle = 0;

/**
  * @brief  TIM3中断服务函数（PWM输入模式）
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        // 捕获到周期值（通道1上升沿）
        IC1Value = TIM_GetCapture1(TIM3);
        
        // 计算频率：F = 1 / (T * 1us)
        if (IC1Value != 0)
            Frequency = 1000000.0 / IC1Value;  // 单位：Hz
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    }
    
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        // 捕获到脉宽值（通道2下降沿）
        IC2Value = TIM_GetCapture2(TIM3);
        
        // 计算占空比
        if (IC1Value != 0)
            DutyCycle = (float)IC2Value / IC1Value * 100.0;  // 单位：%
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }
}
```

## 第四部分：综合应用实例

### 4.1 编码器接口模式（测速）

```c
/**
  * @brief  配置TIM4为编码器接口模式
  * @note   用于正交编码器测量
  */
void TIM4_Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 2. 配置编码器输入引脚（TIM4_CH1: PB6, TIM4_CH2: PB7）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 3. 配置时基
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;      // 最大计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;        // 不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    // 4. 配置编码器接口模式
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    // 5. 配置输入捕获通道
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0xF;  // 添加滤波器抗干扰
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    
    // 6. 清除计数器
    TIM_SetCounter(TIM4, 0);
    
    // 7. 启动定时器
    TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  获取编码器计数值
  * @param  无
  * @retval 编码器计数值（带方向）
  */
int16_t Encoder_GetCount(void)
{
    int16_t count;
    count = (int16_t)TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4, 0);  // 读取后清零
    return count;
}
```

### 4.2 定时器级联（主从模式）

```c
/**
  * @brief  配置TIM1为主定时器，TIM2为从定时器
  * @note   TIM1更新事件触发TIM2
  */
void TIM_MasterSlave_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // 1. 配置TIM1（主定时器）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = 719;  // 100kHz计数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // 设置TIM1为主模式，更新事件作为触发输出
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    
    // 2. 配置TIM2（从定时器）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 9;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;  // 使用外部时钟
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 设置TIM2为从模式，使用ITR0（TIM1）作为触发源
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);
    
    // 3. 启动定时器
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
```

### 4.3 DMA与定时器配合（用于ADC规则转换）

```c
/**
  * @brief  配置TIM2触发ADC采样（通过DMA传输）
  */
void TIM2_ADC_DMA_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // 1. 配置TIM2（10kHz触发频率）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 7200 - 1;    // 72MHz/7200 = 10kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 2. 配置TIM2触发输出
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    
    // 3. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}
```

## 第五部分：调试与技巧

### 5.1 常见问题及解决方法

#### 1. **定时不准**
```c
// 检查点：
// 1. 时钟源配置是否正确
SystemCoreClockUpdate();  // 更新系统时钟变量
printf("System Clock: %lu Hz\n", SystemCoreClock);

// 2. 预分频器和ARR值计算
// 3. 中断响应延迟影响
```

#### 2. **PWM输出异常**
```c
// 检查点：
// 1. GPIO是否配置为复用功能
// 2. PWM极性配置是否正确
// 3. 预装载寄存器是否使能
// 4. 对于高级定时器，需要使能主输出
// TIM_CtrlPWMOutputs(TIM1, ENABLE);
```

#### 3. **输入捕获误差大**
```c
// 解决方法：
// 1. 添加数字滤波器
TIM_ICInitStructure.TIM_ICFilter = 0xF;  // 最大滤波

// 2. 提高计数器频率（减小预分频）
// 3. 使用32位定时器提高测量范围
// 4. 多次测量取平均
```

### 5.2 优化建议

1. **使用DMA减少CPU负担**
   - PWM更新通过DMA
   - ADC采样定时触发+DMA传输

2. **合理使用中断优先级**
   - 时间关键的用高优先级
   - 非关键的用低优先级

3. **利用定时器的硬件特性**
   - 编码器接口硬件解码
   - 主从模式硬件同步
   - 互补输出带死区控制

4. **功耗考虑**
   - 不用时关闭定时器时钟
   - 需要时再使能

```c
// 示例：低功耗模式下的定时器使用
void Enter_LowPower_Mode(void)
{
    // 关闭不需要的定时器
    TIM_Cmd(TIM2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
    
    // 保留一个低功耗定时器（如TIM7）
    // 配置唤醒间隔
}
```

## 总结

定时器的三大核心功能形成了一个完整的生态系统：

1. **时基单元**：提供时间基准，是定时器的发动机
2. **输出比较**：主动控制输出，是定时器的执行机构
3. **输入捕获**：被动测量输入，是定时器的感知器官

在实际应用中，通常组合使用这些功能：
- PWM输出 + 输入捕获：形成闭环控制
- 编码器接口 + 定时中断：实现位置速度测量
- 多个定时器级联：扩展定时范围或实现复杂时序

掌握定时器的关键在于理解其硬件架构和工作原理，然后根据具体需求灵活配置各个寄存器。标准库提供了良好的封装，但了解底层寄存器操作对于优化性能和解决问题至关重要。