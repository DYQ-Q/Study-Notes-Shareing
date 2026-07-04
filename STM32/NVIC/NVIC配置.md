
好的，我来为你详细讲解STM32的NVIC（Nested Vectored Interrupt Controller，嵌套向量中断控制器）配置过程。NVIC是ARM Cortex-M内核的核心组件，用于管理中断的优先级、使能和屏蔽。

---

### **1. NVIC概述**
- **作用**：管理中断的优先级、使能和屏蔽。
- **特性**：
  - 支持中断嵌套（高优先级中断可打断低优先级中断）。
  - 优先级分为**抢占优先级（Preemption Priority）**和**子优先级（Subpriority）**。
  - 优先级分组（Priority Grouping）决定了抢占优先级和子优先级的位数分配。

---

### **2. 优先级分组**
STM32的优先级分组通过`NVIC_PriorityGroupConfig`设置，共有5种分组方式：

| 分组方式                | 抢占优先级位数 | 子优先级位数 | 取值范围       |
|-------------------------|----------------|--------------|----------------|
| `NVIC_PriorityGroup_0`  | 0位            | 4位          | 0~15           |
| `NVIC_PriorityGroup_1`  | 1位            | 3位          | 抢占：0~1，子：0~7 |
| `NVIC_PriorityGroup_2`  | 2位            | 2位          | 抢占：0~3，子：0~3 |
| `NVIC_PriorityGroup_3`  | 3位            | 1位          | 抢占：0~7，子：0~1 |
| `NVIC_PriorityGroup_4`  | 4位            | 0位          | 0~15           |

**推荐分组**：通常使用`NVIC_PriorityGroup_2`（2位抢占，2位子优先级）。

---

### **3. NVIC配置步骤**
#### **步骤1：设置优先级分组**
```c
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2位抢占，2位子优先级
```

#### **步骤2：初始化NVIC**
使用`NVIC_InitTypeDef`结构体配置中断通道、优先级和使能状态：
```c
NVIC_InitTypeDef NVIC_InitStruct;
NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;       // 中断通道（如EXTI0）
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;      // 子优先级
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;         // 使能中断
NVIC_Init(&NVIC_InitStruct);
```

#### **步骤3：编写中断服务函数（ISR）**
```c
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // 用户代码
        EXTI_ClearITPendingBit(EXTI_Line0); // 清除中断标志
    }
}
```

---

### **4. 完整示例（EXTI0中断配置）**
```c
#include "stm32f10x.h"

void NVIC_Config(void) {
    // 1. 设置优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 2. 初始化NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级1
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        // 子优先级1
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_Config(void) {
    // 配置EXTI0（略，见外部中断配置）
    // ...
}

int main(void) {
    NVIC_Config();
    EXTI0_Config();
    while (1);
}

// 中断服务函数
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // 用户代码
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

---

### **5. 常见中断通道**
| 中断通道               | 说明                     |
|------------------------|--------------------------|
| `EXTI0_IRQn`           | EXTI0中断                |
| `EXTI1_IRQn`           | EXTI1中断                |
| `TIM2_IRQn`            | TIM2全局中断             |
| `USART1_IRQn`          | USART1全局中断           |
| `RTC_IRQn`             | RTC全局中断              |
| `USB_HP_CAN1_TX_IRQn`  | USB高优先级/CAN1发送中断 |

---

### **6. 注意事项**
1. **优先级数值越小，优先级越高**：
   - 抢占优先级：`0` > `1` > `2` > `3`。
   - 子优先级：`0` > `1` > `2` > `3`。

2. **抢占优先级和子优先级的区别**：
   - **抢占优先级**：决定是否可以嵌套（高抢占可打断低抢占）。
   - **子优先级**：当抢占优先级相同时，子优先级高的先执行。

3. **中断服务函数命名**：
   - 必须与启动文件（`startup_stm32f10x_xx.s`）中的弱定义一致。

4. **HAL库版本**：
   - 如果使用HAL库，代码略有不同（如`HAL_NVIC_SetPriority`）。

---

### **7. HAL库版本（可选）**
```c
// 设置优先级分组
HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

// 配置NVIC
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 1); // 抢占优先级1，子优先级1
HAL_NVIC_EnableIRQ(EXTI0_IRQn);         // 使能中断
```

---

通过以上步骤和示例，你可以灵活配置STM32的NVIC中断优先级！