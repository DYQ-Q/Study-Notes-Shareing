

---

### **1. 外部中断概述**
- **EXTI（External Interrupt）**：STM32的外部中断控制器，支持多达19个中断线（EXTI0~EXTI18）。
- **中断线映射**：
  - `EXTI0`：对应所有端口的`Pin 0`（如PA0、PB0、PC0等）。
  - `EXTI1`：对应所有端口的`Pin 1`（如PA1、PB1、PC1等）。
  - 以此类推，直到`EXTI15`对应`Pin 15`。
  - `EXTI16~18`：用于PVD输出、RTC闹钟、USB唤醒等特殊事件。

---

### **2. 外部中断配置步骤**
#### **步骤1：开启GPIO时钟**
```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
```

#### **步骤2：配置GPIO引脚为输入模式**
```c
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;       // 选择PA0
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;   // 上拉输入（避免浮空）
GPIO_Init(GPIOA, &GPIO_InitStruct);
```

#### **步骤3：开启AFIO时钟（复用功能）**
```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 必须开启AFIO时钟
```

#### **步骤4：配置EXTI中断线**
使用`GPIO_EXTILineConfig`将GPIO引脚映射到EXTI线：
```c
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); // PA0 -> EXTI0
```

#### **步骤5：初始化EXTI**
```c
EXTI_InitTypeDef EXTI_InitStruct;
EXTI_InitStruct.EXTI_Line = EXTI_Line0;       // 选择EXTI0
EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
EXTI_InitStruct.EXTI_LineCmd = ENABLE;        // 使能EXTI线
EXTI_Init(&EXTI_InitStruct);
```

#### **步骤6：配置NVIC（嵌套向量中断控制器）**
```c
NVIC_InitTypeDef NVIC_InitStruct;
NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; // EXTI0中断通道
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;        // 子优先级
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
NVIC_Init(&NVIC_InitStruct);
```

#### **步骤7：编写中断服务函数（ISR）**
```c
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) { // 检查是否是EXTI0中断
        // 执行用户代码（如翻转LED）
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)));

        EXTI_ClearITPendingBit(EXTI_Line0); // 清除中断标志位
    }
}
```

---

### **3. 完整示例（PA0下降沿触发中断，翻转PB5）**
```c
#include "stm32f10x.h"

void EXTI0_Config(void) {
    // 1. 开启GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 2. 配置PA0为上拉输入
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. 配置PB5为推挽输出（LED）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 4. 开启AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 5. 将PA0映射到EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    // 6. 初始化EXTI0
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    // 7. 配置NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

// 中断服务函数
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // 翻转PB5（LED）
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)));

        // 清除中断标志位
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

int main(void) {
    EXTI0_Config();
    while (1);
}
```

---

### **4. 注意事项**
1. **中断线冲突**：
   - 同一时刻只能有一个引脚映射到`EXTI0`（如PA0和PB0不能同时使用）。

2. **中断优先级**：
   - 如果有多个中断，需合理设置抢占优先级和子优先级。

3. **清除中断标志位**：
   - 必须在ISR中调用`EXTI_ClearITPendingBit`，否则会无限进入中断。

4. **浮空输入问题**：
   - 避免使用浮空输入，建议配置上拉/下拉。

5. **HAL库版本**：
   - 如果使用HAL库，代码略有不同（如`HAL_GPIO_EXTI_IRQHandler`）。

---

通过以上步骤和示例，你可以灵活配置STM32的外部中断！