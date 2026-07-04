
好的，我来为你详细讲解STM32的定时器（Timer）配置过程。定时器是STM32中功能最强大的外设之一，可用于定时、PWM输出、输入捕获、输出比较等多种场景。

---

### **1. 定时器分类**
STM32的定时器分为以下几类：

| 类型       | 特点                          | 示例型号      |
|------------|-------------------------------|---------------|
| 通用定时器 | 基本定时、PWM、输入捕获等     | TIM2~TIM5     |
| 高级定时器 | 增加死区控制、刹车功能等      | TIM1、TIM8    |
| 基本定时器 | 只能定时，无外部IO            | TIM6、TIM7    |

---

### **2. 定时器配置步骤**
#### **步骤1：开启定时器时钟**
```c
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // TIM2挂载在APB1
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // TIM1挂载在APB2
```

#### **步骤2：配置定时器时基单元**
使用`TIM_TimeBaseInitTypeDef`结构体配置预分频器（PSC）、自动重装载值（ARR）、计数模式等：
```c
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_InitStruct.TIM_Prescaler = 7199;          // PSC=7199（分频7200倍）
TIM_InitStruct.TIM_Period = 9999;              // ARR=9999
TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟分频
TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
```

#### **步骤3：使能定时器更新中断**
```c
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能更新中断
```

#### **步骤4：配置NVIC**
```c
NVIC_InitTypeDef NVIC_InitStruct;
NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn; // TIM2中断通道
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStruct);
```

#### **步骤5：启动定时器**
```c
TIM_Cmd(TIM2, ENABLE); // 启动TIM2
```

#### **步骤6：编写中断服务函数**
```c
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        // 用户代码（如翻转LED）
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
    }
}
```

---

### **3. 完整示例（TIM2定时1ms中断）**
```c
#include "stm32f10x.h"

void TIM2_Config(void) {
    // 1. 开启TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. 配置时基单元（1ms定时）
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_Prescaler = 7199;    // 72MHz / (7199+1) = 10kHz
    TIM_InitStruct.TIM_Period = 9;          // 10kHz / (9+1) = 1kHz（1ms）
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    // 3. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 4. 配置NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 5. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

// 中断服务函数
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        // 用户代码（如翻转LED）
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

int main(void) {
    TIM2_Config();
    while (1);
}
```

---

### **4. PWM输出配置（TIM3 CH1）**
```c
void TIM3_PWM_Config(void) {
    // 1. 开启TIM3和GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置PA6为复用推挽输出（TIM3 CH1）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. 配置TIM3时基单元
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_Prescaler = 7199;    // 10kHz
    TIM_InitStruct.TIM_Period = 999;        // 10Hz（100ms）
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);

    // 4. 配置PWM模式
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 500;               // 占空比50%
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);

    // 5. 启动TIM3
    TIM_Cmd(TIM3, ENABLE);
}
```

---

### **5. 注意事项**
1. **时钟频率**：
   - 定时器时钟频率取决于总线时钟（APB1/APB2）。
   - 如果APB1预分频系数>1，定时器时钟=2×APB1_CLK。

2. **PSC和ARR范围**：
   - 16位定时器：`0~65535`。
   - 32位定时器（如TIM2/TIM5）：`0~4294967295`。

3. **中断优先级**：
   - 定时器中断需设置合适的NVIC优先级。

4. **PWM输出**：
   - 需配置GPIO为复用推挽模式（`GPIO_Mode_AF_PP`）。

5. **HAL库版本**：
   - 如果使用HAL库，代码略有不同（如`HAL_TIM_Base_Start_IT`）。

---

通过以上步骤和示例，你可以灵活配置STM32的定时器！