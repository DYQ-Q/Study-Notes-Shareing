

---

## 第一部分：TIM 定时器概述

### 1.1 STM32定时器分类

STM32F103系列有3种类型的定时器：

#### 基本定时器（TIM6, TIM7）
- **功能**：只有最基本的定时功能
- **特性**：16位自动重装载，16位可编程预分频器
- **应用**：通用定时、DAC触发

#### 通用定时器（TIM2-TIM5）
- **功能**：完整定时器功能
- **特性**：16位/32位自动重装载，16位预分频器，4个独立通道
- **应用**：定时、输入捕获、输出比较、PWM生成、编码器接口

#### 高级定时器（TIM1, TIM8）
- **功能**：通用定时器所有功能+高级特性
- **特性**：互补输出、死区生成、紧急制动、定时器同步
- **应用**：电机控制、电源管理、高级PWM应用

### 1.2 定时器基本工作原理

```
时钟源 → 预分频器 → 计数器 → 自动重载寄存器
                        ↓
                 比较/捕获电路
```

---

## 第二部分：TIM 寄存器详解

### 2.1 主要寄存器结构

```c
typedef struct
{
    __IO uint16_t CR1;        // 控制寄存器1
    __IO uint16_t RESERVED0;
    __IO uint16_t CR2;        // 控制寄存器2
    __IO uint16_t RESERVED1;
    __IO uint16_t SMCR;       // 从模式控制寄存器
    __IO uint16_t RESERVED2;
    __IO uint16_t DIER;       // DMA/中断使能寄存器
    __IO uint16_t RESERVED3;
    __IO uint16_t SR;         // 状态寄存器
    __IO uint16_t RESERVED4;
    __IO uint16_t EGR;        // 事件产生寄存器
    __IO uint16_t RESERVED5;
    __IO uint16_t CCMR1;      // 捕获/比较模式寄存器1
    __IO uint16_t RESERVED6;
    __IO uint16_t CCMR2;      // 捕获/比较模式寄存器2
    __ERO uint16_t RESERVED7;
    __IO uint16_t CCER;       // 捕获/比较使能寄存器
    __IO uint16_t RESERVED8;
    __IO uint16_t CNT;        // 计数器
    __IO uint16_t RESERVED9;
    __IO uint16_t PSC;        // 预分频器
    __IO uint16_t RESERVED10;
    __IO uint16_t ARR;        // 自动重装载寄存器
    __IO uint16_t RESERVED11;
    __IO uint16_t RCR;        // 重复计数寄存器（高级定时器）
    __IO uint16_t RESERVED12;
    __IO uint16_t CCR1;       // 捕获/比较寄存器1
    __IO uint16_t RESERVED13;
    __IO uint16_t CCR2;       // 捕获/比较寄存器2
    __IO uint16_t RESERVED14;
    __IO uint16_t CCR3;       // 捕获/比较寄存器3
    __IO uint16_t RESERVED15;
    __IO uint16_t CCR4;       // 捕获/比较寄存器4
} TIM_TypeDef;
```

### 2.2 关键寄存器详解

#### CR1 - 控制寄存器1
```c
// CR1位定义：
#define TIM_CR1_CEN          (0x0001)  // 计数器使能
#define TIM_CR1_UDIS         (0x0002)  // 更新禁止
#define TIM_CR1_URS          (0x0004)  // 更新请求源
#define TIM_CR1_OPM          (0x0008)  // 单脉冲模式
#define TIM_CR1_DIR          (0x0010)  // 计数方向（0=向上，1=向下）
#define TIM_CR1_CMS          (0x0060)  // 中央对齐模式选择
#define TIM_CR1_ARPE         (0x0080)  // 自动重装载预装载使能
```

#### DIER - DMA/中断使能寄存器
```c
// DIER位定义：
#define TIM_DIER_UIE         (0x0001)  // 更新中断使能
#define TIM_DIER_CC1IE       (0x0002)  // 捕获/比较1中断使能
#define TIM_DIER_CC2IE       (0x0004)  // 捕获/比较2中断使能
#define TIM_DIER_CC3IE       (0x0008)  // 捕获/比较3中断使能
#define TIM_DIER_CC4IE       (0x0010)  // 捕获/比较4中断使能
#define TIM_DIER_TIE         (0x0040)  // 触发中断使能
#define TIM_DIER_UDE         (0x0100)  // 更新DMA请求使能
```

#### SR - 状态寄存器
```c
// SR位定义：
#define TIM_SR_UIF           (0x0001)  // 更新中断标志
#define TIM_SR_CC1IF         (0x0002)  // 捕获/比较1中断标志
#define TIM_SR_CC2IF         (0x0004)  // 捕获/比较2中断标志
#define TIM_SR_CC3IF         (0x0008)  // 捕获/比较3中断标志
#define TIM_SR_CC4IF         (0x0010)  // 捕获/比较4中断标志
#define TIM_SR_TIF           (0x0040)  // 触发中断标志
#define TIM_SR_CC1OF         (0x0200)  // 捕获/比较1重复捕获标志
```

---

## 第三部分：定时器工作模式

### 3.1 计数模式

#### 向上计数模式
```c
void TIM_UpCounting_Mode(void)
{
    /*
    向上计数模式：
    - 计数器从0计数到ARR值
    - 达到ARR时产生更新事件，计数器归零
    - 应用：通用定时、PWM生成
    */
}
```

#### 向下计数模式
```c
void TIM_DownCounting_Mode(void)
{
    /*
    向下计数模式：
    - 计数器从ARR值向下计数到0
    - 达到0时产生更新事件，计数器重载ARR值
    - 应用：特定定时需求
    */
}
```

#### 中央对齐模式
```c
void TIM_CenterAligned_Mode(void)
{
    /*
    中央对齐模式：
    - 计数器从0向上计数到ARR-1，然后向下计数到1
    - 在ARR和0时都产生更新事件
    - 应用：对称PWM生成
    */
}
```

### 3.2 时钟源选择

```c
void TIM_Clock_Source_Selection(void)
{
    /*
    定时器时钟源：
    1. 内部时钟（CK_INT）：最常用，来自APB总线
    2. 外部时钟模式1：外部输入引脚
    3. 外部时钟模式2：外部触发输入
    4. 内部触发输入：其他定时器触发
    */
}
```

---

## 第四部分：定时器基本配置

### 4.1 时基初始化结构体

```c
// 时基初始化结构体
typedef struct
{
    uint16_t TIM_Prescaler;          // 预分频器
    uint16_t TIM_CounterMode;        // 计数模式
    uint16_t TIM_Period;             // 自动重装载值
    uint16_t TIM_ClockDivision;      // 时钟分频
    uint8_t TIM_RepetitionCounter;   // 重复计数器（高级定时器）
} TIM_TimeBaseInitTypeDef;

// 计数模式枚举
#define TIM_CounterMode_Up           ((uint16_t)0x0000)  // 向上计数
#define TIM_CounterMode_Down         ((uint16_t)0x0010)  // 向下计数
#define TIM_CounterMode_CenterAligned1 ((uint16_t)0x0020) // 中央对齐模式1
#define TIM_CounterMode_CenterAligned2 ((uint16_t)0x0040) // 中央对齐模式2
#define TIM_CounterMode_CenterAligned3 ((uint16_t)0x0060) // 中央对齐模式3

// 时钟分频枚举
#define TIM_CKD_DIV1                 ((uint16_t)0x0000)  // tDTS = tCK_INT
#define TIM_CKD_DIV2                 ((uint16_t)0x0100)  // tDTS = 2 * tCK_INT
#define TIM_CKD_DIV4                 ((uint16_t)0x0200)  // tDTS = 4 * tCK_INT
```

### 4.2 基本定时器配置

```c
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

void TIM_Basic_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：开启定时器时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 步骤2：配置时基参数 */
    // 定时时间计算：Time = (ARR + 1) * (PSC + 1) / TIMxCLK
    // 假设系统时钟72MHz，配置1ms定时：
    // Time = (1000) * (72) / 72MHz = 1ms
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;         // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;        // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 步骤3：使能定时器中断 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* 步骤4：配置NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 步骤5：启动定时器 */
    TIM_Cmd(TIM2, ENABLE);
}

// 定时器中断服务函数
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 定时器更新中断处理
        // 这里可以执行周期性任务
        
        // 清除中断标志
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```

### 4.3 精确延时函数实现

```c
// 基于SysTick的精确延时函数
void Delay_Init(void)
{
    // 配置SysTick为1ms中断
    if(SysTick_Config(SystemCoreClock / 1000))
    {
        while(1);  // 初始化失败
    }
}

void Delay_ms(uint32_t nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

// SysTick中断服务函数
void SysTick_Handler(void)
{
    if(TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

// 基于定时器的微秒级延时
void TIM_Delay_us(uint16_t us)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    // 使用TIM7（基本定时器）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    // 配置为1us计数
    TIM_TimeBaseStructure.TIM_Period = us - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;  // 72MHz/72 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    // 启动定时器
    TIM_Cmd(TIM7, ENABLE);
    
    // 等待定时结束
    while(TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == RESET);
    
    // 清除标志并关闭定时器
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    TIM_Cmd(TIM7, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);
}
```

---

## 第五部分：PWM 输出配置

### 5.1 PWM 输出初始化结构体

```c
// 输出比较初始化结构体
typedef struct
{
    uint16_t TIM_OCMode;        // PWM模式
    uint16_t TIM_OutputState;   // 输出使能
    uint16_t TIM_OutputNState;  // 互补输出使能（高级定时器）
    uint16_t TIM_Pulse;         // 脉冲宽度（比较值）
    uint16_t TIM_OCPolarity;    // 输出极性
    uint16_t TIM_OCNPolarity;   // 互补输出极性
    uint16_t TIM_OCIdleState;   // 空闲状态
    uint16_t TIM_OCNIdleState;  // 互补输出空闲状态
} TIM_OCInitTypeDef;

// PWM模式枚举
#define TIM_OCMode_Timing          ((uint16_t)0x0000)  // 定时模式
#define TIM_OCMode_Active          ((uint16_t)0x0010)  // 主动模式
#define TIM_OCMode_Inactive        ((uint16_t)0x0020)  // 非主动模式
#define TIM_OCMode_Toggle          ((uint16_t)0x0030)  // 翻转模式
#define TIM_OCMode_PWM1            ((uint16_t)0x0060)  // PWM模式1
#define TIM_OCMode_PWM2            ((uint16_t)0x0070)  // PWM模式2

// 输出状态枚举
#define TIM_OutputState_Disable    ((uint16_t)0x0000)  // 输出禁用
#define TIM_OutputState_Enable     ((uint16_t)0x0001)  // 输出使能

// 输出极性枚举
#define TIM_OCPolarity_High        ((uint16_t)0x0000)  // 高电平有效
#define TIM_OCPolarity_Low         ((uint16_t)0x0002)  // 低电平有效
```

$$T_{\text{高电平}} = \frac{CCR}{TIM_{CLK}} \times (PSC + 1)  $$
### 5.2 PWM 输出完整配置

```c
void TIM_PWM_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 步骤2：配置GPIO为复用推挽输出 */
    // PA0 - TIM2_CH1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA1 - TIM2_CH2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置时基 */
    // 配置PWM频率为1kHz
    // PWM频率 = 72MHz / ((ARR+1) * (PSC+1))
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;         // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;        // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 步骤4：配置PWM模式 - 通道1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;      // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 500;                   // 初始占空比50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);               // 初始化通道1
    
    /* 步骤5：配置PWM模式 - 通道2 */
    TIM_OCInitStructure.TIM_Pulse = 250;                   // 初始占空比25%
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);               // 初始化通道2
    
    /* 步骤6：启动定时器 */
    TIM_Cmd(TIM2, ENABLE);
    
    // 对于高级定时器，还需要使能主输出
    // TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// 动态改变PWM占空比
void PWM_SetDutyCycle(TIM_TypeDef* TIMx, uint16_t Channel, float duty_cycle)
{
    uint16_t pulse = (uint16_t)((TIMx->ARR + 1) * duty_cycle);
    
    switch(Channel)
    {
        case TIM_Channel_1:
            TIM_SetCompare1(TIMx, pulse);
            break;
        case TIM_Channel_2:
            TIM_SetCompare2(TIMx, pulse);
            break;
        case TIM_Channel_3:
            TIM_SetCompare3(TIMx, pulse);
            break;
        case TIM_Channel_4:
            TIM_SetCompare4(TIMx, pulse);
            break;
    }
}

// 使用示例
void PWM_Example(void)
{
    // 设置通道1占空比为75%
    PWM_SetDutyCycle(TIM2, TIM_Channel_1, 0.75f);
    
    // 设置通道2占空比为10%
    PWM_SetDutyCycle(TIM2, TIM_Channel_2, 0.10f);
}
```

### 5.3 呼吸灯效果

```c
void PWM_Breathing_LED(void)
{
    static uint16_t pwm_val = 0;
    static int8_t dir = 1;
    
    // 逐渐改变PWM值实现呼吸效果
    pwm_val += dir;
    
    if(pwm_val > 1000)  // ARR = 1000
    {
        pwm_val = 1000;
        dir = -1;  // 改为递减
    }
    else if(pwm_val == 0)
    {
        dir = 1;   // 改为递增
    }
    
    TIM_SetCompare1(TIM2, pwm_val);
}
```

---

## 第六部分：输入捕获配置

### 6.1 输入捕获初始化结构体

```c
// 输入捕获初始化结构体
typedef struct
{
    uint16_t TIM_Channel;       // 输入捕获通道
    uint16_t TIM_ICPolarity;    // 输入捕获极性
    uint16_t TIM_ICSelection;   // 输入捕获选择
    uint16_t TIM_ICPrescaler;   // 输入捕获预分频
    uint16_t TIM_ICFilter;      // 输入捕获滤波器
} TIM_ICInitTypeDef;

// 输入捕获极性枚举
#define TIM_ICPolarity_Rising        ((uint16_t)0x0000)  // 上升沿捕获
#define TIM_ICPolarity_Falling       ((uint16_t)0x0002)  // 下降沿捕获
#define TIM_ICPolarity_BothEdge      ((uint16_t)0x000A)  // 双边沿捕获

// 输入捕获选择枚举
#define TIM_ICSelection_DirectTI     ((uint16_t)0x0001)  // 直接模式
#define TIM_ICSelection_IndirectTI   ((uint16_t)0x0002)  // 间接模式
#define TIM_ICSelection_TRC          ((uint16_t)0x0003)  // TRC模式

// 输入捕获预分频枚举
#define TIM_ICPSC_DIV1               ((uint16_t)0x0000)  // 无分频
#define TIM_ICPSC_DIV2               ((uint16_t)0x0004)  // 2分频
#define TIM_ICPSC_DIV4               ((uint16_t)0x0008)  // 4分频
#define TIM_ICPSC_DIV8               ((uint16_t)0x000C)  // 8分频
```

### 6.2 输入捕获完整配置

```c
void TIM_InputCapture_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 步骤2：配置GPIO为输入 */
    // PA0 - TIM2_CH1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;        // 下拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置时基 */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;           // 最大计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;        // 1MHz计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 步骤4：配置输入捕获 */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;          // 通道1
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 直接模式
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     // 无分频
    TIM_ICInitStructure.TIM_ICFilter = 0x0;                   // 无滤波
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    /* 步骤5：使能捕获中断 */
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    
    /* 步骤6：配置NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 步骤7：启动定时器 */
    TIM_Cmd(TIM2, ENABLE);
}

// 输入捕获中断服务函数 - 测量脉冲宽度
void TIM2_IRQHandler_InputCapture(void)
{
    static uint16_t capture1 = 0, capture2 = 0;
    static uint8_t capture_count = 0;
    uint32_t pulse_width;
    
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        switch(capture_count)
        {
            case 0:
                // 第一次捕获 - 上升沿
                capture1 = TIM_GetCapture1(TIM2);
                // 改为下降沿捕获
                TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);
                capture_count = 1;
                break;
                
            case 1:
                // 第二次捕获 - 下降沿
                capture2 = TIM_GetCapture1(TIM2);
                
                // 计算脉冲宽度（考虑计数器溢出）
                if(capture2 > capture1)
                {
                    pulse_width = capture2 - capture1;
                }
                else
                {
                    pulse_width = (0xFFFF - capture1) + capture2 + 1;
                }
                
                // 转换为时间（微秒）
                // pulse_width_us = pulse_width; // 因为预分频配置为1MHz
                
                // 改回上升沿捕获，准备下一次测量
                TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
                capture_count = 0;
                break;
        }
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
}
```

### 6.3 频率测量实现

```c
// 使用输入捕获测量频率
void TIM_Frequency_Measurement(void)
{
    static uint32_t last_capture = 0;
    static uint32_t period = 0;
    uint32_t current_capture;
    float frequency;
    
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        current_capture = TIM_GetCapture1(TIM2);
        
        if(last_capture != 0)
        {
            // 计算周期（考虑计数器溢出）
            if(current_capture > last_capture)
            {
                period = current_capture - last_capture;
            }
            else
            {
                period = (0xFFFF - last_capture) + current_capture + 1;
            }
            
            // 计算频率（Hz）
            // 定时器计数频率 = 72MHz / 72 = 1MHz
            frequency = 1000000.0f / period;
        }
        
        last_capture = current_capture;
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
}
```

---

## 第七部分：高级定时器应用

### 7.1 互补PWM输出（电机控制）

```c
void TIM_Advanced_PWM_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                          RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    /* 步骤2：配置GPIO */
    // PA8 - TIM1_CH1 (PWM输出)
    // PB13 - TIM1_CH1N (互补输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* 步骤3：配置时基 */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;         // PWM周期
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;        // 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    /* 步骤4：配置PWM输出 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable; // 互补输出使能
    TIM_OCInitStructure.TIM_Pulse = 500;                  // 占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    /* 步骤5：配置死区时间 */
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x10;           // 死区时间
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
    
    /* 步骤6：启动定时器 */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  // 必须使能主输出
}
```

### 7.2 编码器接口模式

```c
void TIM_Encoder_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 步骤2：配置编码器输入引脚 */
    // PA0 - TIM2_CH1 (编码器A相)
    // PA1 - TIM2_CH2 (编码器B相)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置时基 */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;           // 最大计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 步骤4：配置编码器接口模式 */
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    // 配置通道1为输入捕获
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    // 配置通道2为输入捕获
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    /* 步骤5：启动编码器接口 */
    TIM_Cmd(TIM2, ENABLE);
}

// 读取编码器位置
int16_t Read_Encoder_Value(void)
{
    int16_t encoder_value;
    encoder_value = (int16_t)TIM_GetCounter(TIM2);
    TIM_SetCounter(TIM2, 0);  // 清零计数器
    return encoder_value;
}
```

---

## 第八部分：定时器同步与触发

### 8.1 主从定时器配置

```c
void TIM_Master_Slave_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    /* 主定时器 TIM2 配置 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;  // 10ms
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 配置TIM2为主模式，更新事件作为触发输出
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    
    /* 从定时器 TIM3 配置 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;       // 从定时器周期
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;     // 1us
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 配置TIM3为从模式，使用ITR1（TIM2）作为触发
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);
    TIM_SelectSlaveMode