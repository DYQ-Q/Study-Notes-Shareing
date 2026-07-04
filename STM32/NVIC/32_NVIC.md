

---
## 第一部分：NVIC 基础概念

### 1.1 NVIC 是什么？

NVIC（Nested Vectored Interrupt Controller）是Cortex-M内核的一部分，负责管理所有中断的优先级和响应。

### 1.2 NVIC 的主要特性

- **嵌套中断**：高优先级中断可以打断低优先级中断
- **动态优先级**：运行时可以改变中断优先级
- **向量中断**：自动跳转到中断服务函数
- **低延迟**：专门为中断处理优化
- **尾链**：高效的中断处理机制

---

## 第二部分：中断优先级分组详解

### 2.1 优先级分组概念

STM32使用**4位**来表示中断优先级，这4位可以划分为**抢占优先级**和**子优先级**。

```c
// 优先级分组配置函数
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
// 优先级分组枚举
#define NVIC_PriorityGroup_0          ((uint32_t)0x700) // 0位抢占，4位子优先级
#define NVIC_PriorityGroup_1          ((uint32_t)0x600) // 1位抢占，3位子优先级  
#define NVIC_PriorityGroup_2          ((uint32_t)0x500) // 2位抢占，2位子优先级
#define NVIC_PriorityGroup_3          ((uint32_t)0x400) // 3位抢占，1位子优先级
#define NVIC_PriorityGroup_4          ((uint32_t)0x300) // 4位抢占，0位子优先级
```

### 2.2 五种优先级分组详解

#### 分组0：NVIC_PriorityGroup_0
```
抢占优先级位数：0位（2^0）
子优先级位数：4位（2^4）
抢占优先级范围：0（只有1级）
子优先级范围：0-15（16级）
```
**特点**：所有中断都不能相互打断，只能按子优先级顺序执行
#### 分组1：NVIC_PriorityGroup_1
```
抢占优先级位数：1位（2^1）
子优先级位数：3位（2^3）
抢占优先级范围：0-1（2级）
子优先级范围：0-7（8级）
```
#### 分组2：NVIC_PriorityGroup_2（最常用）
```
抢占优先级位数：2位（2^2）
子优先级位数：2位（2^2）
抢占优先级范围：0-3（4级）
子优先级范围：0-3（4级）
```
#### 分组3：NVIC_PriorityGroup_3
```
抢占优先级位数：3位（2^3）
子优先级位数：1位（2^1）
抢占优先级范围：0-7（8级）
子优先级范围：0-1（2级）
```
#### 分组4：NVIC_PriorityGroup_4
```
抢占优先级位数：4位（2^4）
子优先级位数：0位（2^0）
抢占优先级范围：0-15（16级）
子优先级范围：0（只有1级）
```
**特点**：所有中断都可以相互打断，没有子优先级概念
### 2.3 优先级分组配置示例

```c
void NVIC_PriorityGroup_Example(void)
{
    // 在程序开始时设置优先级分组（通常只设置一次）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*
    解释：
    - 使用2位抢占优先级，2位子优先级
    - 抢占优先级范围：0-3（0最高，3最低）
    - 子优先级范围：0-3（0最高，3最低）
    - 总共可以配置 4 × 4 = 16 个不同的优先级组合
    */
}
```

---
## 第三部分：NVIC 配置结构体详解

### 3.1 NVIC_InitTypeDef 结构体

```c
typedef struct
{
    uint8_t NVIC_IRQChannel;                    // 中断通道
    uint8_t NVIC_IRQChannelPreemptionPriority;  // 抢占优先级
    uint8_t NVIC_IRQChannelSubPriority;         // 子优先级
    FunctionalState NVIC_IRQChannelCmd;         // 使能或禁用
} NVIC_InitTypeDef;
```

### 3.2 中断通道定义

STM32F103的常见中断通道：

```c
// 外部中断
#define EXTI0_IRQn                ((uint8_t)6)   // EXTI Line0 中断
#define EXTI1_IRQn                ((uint8_t)7)   // EXTI Line1 中断
#define EXTI2_IRQn                ((uint8_t)8)   // EXTI Line2 中断
#define EXTI3_IRQn                ((uint8_t)9)   // EXTI Line3 中断
#define EXTI4_IRQn                ((uint8_t)10)  // EXTI Line4 中断
#define EXTI9_5_IRQn              ((uint8_t)23)  // EXTI Line[9:5] 中断
#define EXTI15_10_IRQn            ((uint8_t)40)  // EXTI Line[15:10] 中断

// 定时器中断
#define TIM1_UP_IRQn              ((uint8_t)25)  // TIM1 更新中断
#define TIM1_TRG_COM_IRQn         ((uint8_t)26)  // TIM1 触发和通信中断
#define TIM2_IRQn                 ((uint8_t)28)  // TIM2 全局中断
#define TIM3_IRQn                 ((uint8_t)29)  // TIM3 全局中断
#define TIM4_IRQn                 ((uint8_t)30)  // TIM4 全局中断

// 串口中断
#define USART1_IRQn               ((uint8_t)37)  // USART1 全局中断
#define USART2_IRQn               ((uint8_t)38)  // USART2 全局中断
#define USART3_IRQn               ((uint8_t)39)  // USART3 全局中断

// DMA中断
#define DMA1_Channel1_IRQn        ((uint8_t)11)  // DMA1 通道1 中断
#define DMA1_Channel2_IRQn        ((uint8_t)12)  // DMA1 通道2 中断
#define DMA1_Channel3_IRQn        ((uint8_t)13)  // DMA1 通道3 中断
```

---
## 第四部分：完整配置示例

### 4.1 基本NVIC配置流程

```c
#include "stm32f10x.h"

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：设置优先级分组（通常在程序开始处设置一次） */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* 步骤2：配置具体的中断通道 */
    
    // 配置EXTI0中断（高优先级）
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级0（最高）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0（最高）
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置TIM2中断（中等优先级）
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置USART1中断（低优先级）
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
```
### 4.2 多中断优先级配置示例
```c
void Complex_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 设置优先级分组：2位抢占，2位子优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* 紧急中断 - 最高优先级 */
    
    // 外部紧急信号（最高优先级）
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 重要外设中断 - 中高优先级 */
    
    // 定时器1（重要定时任务）
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0
    NVIC_Init(&NVIC_InitStructure);
    
    // DMA传输完成（重要数据传输）
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级1
    NVIC_Init(&NVIC_InitStructure);
    
    /* 普通外设中断 - 中等优先级 */
    
    // 串口1接收（通信）
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0
    NVIC_Init(&NVIC_InitStructure);
    
    // 定时器2（普通定时）
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级1
    NVIC_Init(&NVIC_InitStructure);
    
    /* 低优先级中断 */
    
    // 普通按键检测
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  // 抢占3（最低）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // 子优先级0
    NVIC_Init(&NVIC_InitStructure);
}
```
---
## 第五部分：中断行为分析

### 5.1 中断响应规则
```c
void Interrupt_Behavior_Analysis(void)
{
    /*
    基于上面的配置，分析中断行为：
    
    中断优先级排序（从高到低）：
    1. EXTI0      - 抢占0, 子0  (最高)
    2. TIM1_UP    - 抢占1, 子0
    3. DMA1_Ch1   - 抢占1, 子1
    4. USART1     - 抢占2, 子0  
    5. TIM2       - 抢占2, 子1
    6. EXTI15_10  - 抢占3, 子0  (最低)
    
    中断嵌套规则：
    - EXTI0可以打断任何其他中断
    - TIM1_UP可以打断USART1、TIM2、EXTI15_10
    - DMA1_Ch1可以打断USART1、TIM2、EXTI15_10
    - USART1和TIM2不能相互打断（相同抢占优先级）
    - 所有中断都可以打断主程序
    */
}
```
### 5.2 相同抢占优先级的中断行为
```c
void Same_Preemption_Priority_Behavior(void)
{
    /*
    当多个中断具有相同的抢占优先级时：
    
    示例配置：
    - USART1: 抢占2, 子0
    - TIM2:   抢占2, 子1  
    - TIM3:   抢占2, 子2
    
    行为分析：
    1. 这些中断不能相互打断
    2. 如果同时发生，按子优先级顺序响应：USART1 → TIM2 → TIM3
    3. 如果正在执行USART1中断时TIM2发生，TIM2要等待USART1执行完
    4. 自然优先级（中断号）只在完全相同的优先级时起作用
    */
}
```
### 5.3 中断响应时序示例
```c
// 模拟中断响应时序
void Interrupt_Timing_Example(void)
{
    /*
    场景模拟：
    
    时间轴：
    t0: 主程序运行
    t1: TIM2中断发生（抢占2）→ 响应TIM2中断
    t2: USART1中断发生（抢占2）→ 等待，因为相同抢占优先级
    t3: EXTI0中断发生（抢占0）→ 打断TIM2，响应EXTI0
    t4: EXTI0执行完毕 → 返回TIM2
    t5: TIM2执行完毕 → 响应USART1
    t6: USART1执行完毕 → 返回主程序
    */
}
```
---
## 第六部分：实际应用案例

### 6.1 实时控制系统
```c
void RealTime_Control_System(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 设置优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* 安全关键中断 - 最高优先级 */
    
    // 急停信号（必须立即响应）
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 过流保护
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 运动控制中断 - 高优先级 */
    
    // 编码器接口（精确位置检测）
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // PWM更新（电机控制）
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 通信中断 - 中等优先级 */
    
    // CAN总线通信
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // 调试串口
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 用户接口中断 - 低优先级 */
    
    // 按键输入
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
```
### 6.2 通信数据处理系统
```c
void Communication_Data_System(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* 数据传输中断 - 最高优先级 */
    
    // DMA传输完成（大数据块传输）
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 实时通信中断 - 高优先级 */
    
    // 以太网接收
    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // USB数据传输
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 协议处理中断 - 中等优先级 */
    
    // 串口数据接收
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // SPI数据传输
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 系统维护中断 - 低优先级 */
    
    // 系统滴答定时器（用于操作系统）
    NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
```
---
## 第七部分：高级配置技巧

### 7.1 运行时动态修改优先级
```c
void Dynamic_Priority_Change(void)
{
    // 在运行时动态修改中断优先级
    
    /* 方法1：使用NVIC_SetPriority()函数 */
    NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    
    /* 方法2：直接操作寄存器 */
    NVIC->IP[EXTI0_IRQn] = (0 << 4);  // 设置优先级为0
    
    /* 方法3：临时提升优先级 */
    void Temporary_Priority_Boost(void)
    {
        // 保存原优先级
        uint32_t original_priority = NVIC->IP[USART1_IRQn];
        
        // 临时提升优先级
        NVIC->IP[USART1_IRQn] = (1 << 4);  // 提升到抢占优先级1
        
        // 执行关键操作
        // ...
        
        // 恢复原优先级
        NVIC->IP[USART1_IRQn] = original_priority;
    }
}
```
### 7.2 优先级编码和解码
```c
void Priority_Encode_Decode_Example(void)
{
    uint32_t priority_group = NVIC_GetPriorityGrouping();
    
    // 编码优先级：将抢占和子优先级合并为一个值
    uint32_t encoded_priority = NVIC_EncodePriority(priority_group, 1, 2);
    
    // 解码优先级：从一个值中提取抢占和子优先级
    uint32_t preempt_priority, sub_priority;
    NVIC_DecodePriority(encoded_priority, priority_group, &preempt_priority, &sub_priority);
    
    // 设置编码后的优先级
    NVIC_SetPriority(USART1_IRQn, encoded_priority);
}
```
### 7.3 中断使能与禁用
```c
void Interrupt_Enable_Disable(void)
{
    // 使能中断
    NVIC_EnableIRQ(EXTI0_IRQn);
    
    // 禁用中断
    NVIC_DisableIRQ(EXTI0_IRQn);
    
    // 获取中断使能状态
    FunctionalState state = NVIC_GetEnableIRQ(EXTI0_IRQn);
    
    // 设置挂起中断（软件触发）
    NVIC_SetPendingIRQ(EXTI0_IRQn);
    
    // 清除挂起中断
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    
    // 获取挂起状态
    uint32_t pending = NVIC_GetPendingIRQ(EXTI0_IRQn);
}
```
---
## 第八部分：调试与问题排查
### 8.1 常见配置错误
```c
void Common_Configuration_Errors(void)
{
    /*
    常见错误1：忘记设置优先级分组
    - 症状：中断行为不符合预期
    - 解决：在程序开始调用 NVIC_PriorityGroupConfig()
    
    常见错误2：优先级数值超出范围
    - 症状：实际优先级与设置不符
    - 解决：根据分组确定有效的优先级范围
    
    常见错误3：共用中断未检查具体线路
    - 症状：EXTI9_5或EXTI15_10中断服务函数中未检查具体线路
    - 解决：在共用ISR中检查所有可能的线路
    */
}
```
### 8.2 调试技巧
```c
void NVIC_Debugging_Tips(void)
{
    // 调试方法1：读取当前优先级设置
    uint32_t exti0_priority = NVIC_GetPriority(EXTI0_IRQn);
    
    // 调试方法2：检查中断是否使能
    if(NVIC_GetEnableIRQ(EXTI0_IRQn) == ENABLE)
    {
        // 中断已使能
    }
    
    // 调试方法3：检查挂起状态
    if(NVIC_GetPendingIRQ(EXTI0_IRQn) != RESET)
    {
        // 有中断挂起
    }
    
    // 调试方法4：获取优先级分组
    uint32_t priority_group = NVIC_GetPriorityGrouping();
}
```
## 总结

**NVIC中断优先级配置关键要点：**

1. **优先级分组**：程序开始时设置一次，决定抢占和子优先级的位数分配
2. **抢占优先级**：决定中断是否可以相互打断
3. **子优先级**：相同抢占优先级时决定响应顺序
4. **数值越小优先级越高**：0是最高优先级
5. **合理规划**：根据中断的重要性和实时性要求分配优先级
6. **分组2最常用**：提供良好的灵活性和足够的优先级级别

**推荐实践：**
- 使用 `NVIC_PriorityGroup_2`（2位抢占，2位子优先级）
- 紧急中断设置抢占优先级0
- 重要外设设置抢占优先级1
- 普通外设设置抢占优先级2
- 非关键任务设置抢占优先级3
