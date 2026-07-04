
---

## 第一部分：EXTI 系统架构与寄存器详解

### 1.1 EXTI 系统架构

```
GPIO引脚 → 边沿检测电路 → EXTI控制器 → NVIC → CPU
      ↑           ↑           ↑
   AFIO选择   触发选择    中断掩码
```

### 1.2 EXTI 线路映射

EXTI有20条中断线（EXTI0-EXTI19），但GPIO引脚远多于这个数量，所以需要**复用**：

| EXTI线  | 可映射的GPIO                                 | 中断服务函数               |
| ------ | ---------------------------------------- | -------------------- |
| EXTI0  | PA0, PB0, PC0, PD0, PE0, PF0, PG0        | EXTI0_IRQHandler     |
| EXTI1  | PA1, PB1, PC1, PD1, PE1, PF1, PG1        | EXTI1_IRQHandler     |
| EXTI2  | PA2, PB2, PC2, PD2, PE2, PF2, PG2        | EXTI2_IRQHandler     |
| EXTI3  | PA3, PB3, PC3, PD3, PE3, PF3, PG3        | EXTI3_IRQHandler     |
| EXTI4  | PA4, PB4, PC4, PD4, PE4, PF4, PG4        | EXTI4_IRQHandler     |
| EXTI5  | PA5, PB5, PC5, PD5, PE5, PF5, PG5        | EXTI9_5_IRQHandler   |
| EXTI6  | PA6, PB6, PC6, PD6, PE6, PF6, PG6        | EXTI9_5_IRQHandler   |
| EXTI7  | PA7, PB7, PC7, PD7, PE7, PF7, PG7        | EXTI9_5_IRQHandler   |
| EXTI8  | PA8, PB8, PC8, PD8, PE8, PF8, PG8        | EXTI9_5_IRQHandler   |
| EXTI9  | PA9, PB9, PC9, PD9, PE9, PF9, PG9        | EXTI9_5_IRQHandler   |
| EXTI10 | PA10, PB10, PC10, PD10, PE10, PF10, PG10 | EXTI15_10_IRQHandler |
| EXTI11 | PA11, PB11, PC11, PD11, PE11, PF11, PG11 | EXTI15_10_IRQHandler |
| EXTI12 | PA12, PB12, PC12, PD12, PE12, PF12, PG12 | EXTI15_10_IRQHandler |
| EXTI13 | PA13, PB13, PC13, PD13, PE13, PF13, PG13 | EXTI15_10_IRQHandler |
| EXTI14 | PA14, PB14, PC14, PD14, PE14, PF14, PG14 | EXTI15_10_IRQHandler |
| EXTI15 | PA15, PB15, PC15, PD15, PE15, PF15, PG15 | EXTI15_10_IRQHandler |

**注**：同一时刻，每条EXTI线只能连接到一个GPIO端口。

### 1.3 EXTI 相关寄存器详解
```c
// EXTI寄存器结构体（在stm32f10x.h中定义）
typedef struct
{
    __IO uint32_t IMR;    // 中断掩码寄存器
    __IO uint32_t EMR;    // 事件掩码寄存器  
    __IO uint32_t RTSR;   // 上升沿触发选择寄存器
    __IO uint32_t FTSR;   // 下降沿触发选择寄存器
    __IO uint32_t SWIER;  // 软件中断事件寄存器
    __IO uint32_t PR;     // 挂起寄存器
} EXTI_TypeDef;

// 寄存器位定义详解：
```
#### IMR - 中断掩码寄存器 (Interrupt Mask Register)
- **位0-18**：对应EXTI0-EXTI18的中断使能
- `0` = 屏蔽中断请求
- `1` = 使能中断请求
#### EMR - 事件掩码寄存器 (Event Mask Register)  
- **位0-18**：对应EXTI0-EXTI18的事件使能
- `0` = 屏蔽事件请求
- `1` = 使能事件请求
#### RTSR - 上升沿触发选择寄存器 (Rising Trigger Selection Register)
- **位0-18**：对应EXTI0-EXTI18的上升沿触发使能
- `0` = 禁止上升沿触发
- `1` = 使能上升沿触发
#### FTSR - 下降沿触发选择寄存器 (Falling Trigger Selection Register)
- **位0-18**：对应EXTI0-EXTI18的下降沿触发使能
- `0` = 禁止下降沿触发
- `1` = 使能下降沿触发
#### PR - 挂起寄存器 (Pending Register)
- **位0-18**：对应EXTI0-EXTI18的中断挂起标志
- 读取：`1` = 发生了中断请求
- 写入：`1` = 清除挂起标志（写入0无效）
---

## 第二部分：EXTI 配置完整步骤

### 2.1 标准库EXTI配置步骤
```c
#include "stm32f10x.h"

void EXTI_Configuration(void)
{
	//定义GPIO,EXTI,NVIC结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：开启相关时钟 */
    // 开启GPIO A端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 开启AFIO时钟（必须开启才能配置EXTI）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    /* 步骤2：配置GPIO A0为输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;        // PA0引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    // 上拉输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置GPIO连接到EXTI线（关键步骤！） */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//连到A0
    
    /* 步骤4：配置EXTI线参数 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;                // 使用EXTI线0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;       // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                 // 使能EXTI线
    EXTI_Init(&EXTI_InitStructure);
    
    /* 步骤5：配置NVIC（嵌套向量中断控制器，配置中断优先级） */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          // EXTI0中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断通道
    NVIC_Init(&NVIC_InitStructure);
}
```

### 2.2 GPIO_EXTILineConfig 函数详解

这个函数用于选择GPIO引脚连接到哪条EXTI线：

```c
// 函数原型
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);

// 参数说明：
// GPIO_PortSource: GPIO端口源
#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE       ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF       ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG       ((uint8_t)0x06)

// GPIO_PinSource: GPIO引脚源
#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
// ... 一直到 GPIO_PinSource15
```
### 2.3 EXTI_InitTypeDef 结构体详解

```c
typedef struct
{
    uint32_t EXTI_Line;               // 指定要配置的EXTI线
    EXTIMode_TypeDef EXTI_Mode;       // EXTI模式：中断或事件
    EXTITrigger_TypeDef EXTI_Trigger; // 触发方式：上升沿、下降沿或双边沿
    FunctionalState EXTI_LineCmd;     // EXTI线使能或禁用
} EXTI_InitTypeDef;

// EXTI模式枚举
typedef enum
{
    EXTI_Mode_Interrupt = 0x00,  // 中断模式
    EXTI_Mode_Event = 0x04       // 事件模式
} EXTIMode_TypeDef;

// 触发方式枚举  
typedef enum
{
    EXTI_Trigger_Rising = 0x08,         // 上升沿触发
    EXTI_Trigger_Falling = 0x0C,        // 下降沿触发
    EXTI_Trigger_Rising_Falling = 0x10  // 双边沿触发
} EXTITrigger_TypeDef;
```

---

## 第三部分：EXTI 中断服务函数

### 3.1 中断服务函数编写规范

```c
// EXTI0中断服务函数
void EXTI0_IRQHandler(void)
{
    /* 步骤1：检查中断标志 */
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* 步骤2：中断处理代码 */
        // 这里写你的中断处理逻辑
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, 
                     (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)));
        
        /* 步骤3：清除中断标志（必须！） */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

// EXTI1中断服务函数
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        // 处理EXTI1中断
        // ...
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

// EXTI9_5中断服务函数（EXTI5-EXTI9共用）
void EXTI9_5_IRQHandler(void)
{
    /* 需要检查具体是哪条线触发的中断 */
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        // 处理EXTI5中断
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        // 处理EXTI6中断
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        // 处理EXTI7中断
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        // 处理EXTI8中断
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        // 处理EXTI9中断
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

// EXTI15_10中断服务函数（EXTI10-EXTI15共用）
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        // 处理EXTI10中断
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        // 处理EXTI11中断
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    // ... 类似的检查EXTI12-EXTI15
}
```

### 3.2 中断标志管理函数

```c
// 检查EXTI线中断标志
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);
// 清除EXTI线中断标志
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);
// 检查EXTI线事件标志
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);
// 清除EXTI线事件标志  
void EXTI_ClearFlag(uint32_t EXTI_Line);
```

---
## 第四部分：完整配置示例

### 4.1 单按键控制LED

```c
#include "stm32f10x.h"

// 硬件定义
#define LED_GPIO_PORT    GPIOB
#define LED_GPIO_PIN     GPIO_Pin_5

#define KEY_GPIO_PORT    GPIOA  
#define KEY_GPIO_PIN     GPIO_Pin_0

void GPIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);

int main(void)
{
    // 系统初始化
    SystemInit();
    // 配置GPIO
    GPIO_Config();
    // 配置EXTI
    EXTI_Config();
    // 配置NVIC
    NVIC_Config();
    while(1)
    {
        // 主循环 - 可以执行其他任务
        // 中断会异步处理按键事件
    }
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    // 配置LED引脚（PB5）为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
    // 初始状态：LED熄灭
    GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
    // 配置按键引脚（PA0）为上拉输入
    GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
}

void EXTI_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    // 开启AFIO时钟（必须！）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 连接PA0到EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0)
    // 配置EXTI0
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发（按键按下）
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 配置EXTI0中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
// EXTI0中断服务函数
void EXTI0_IRQHandler(void)
{
    // 简单的防抖动延时
    volatile uint32_t i;
    for(i = 0; i < 0x1000; i++);
    
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 切换LED状态
        if(GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_GPIO_PIN) == Bit_SET)
        {
            GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);  // 熄灭LED
        }
        else
        {
            GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);    // 点亮LED
        }
        
        // 等待按键释放（简单的防抖动）
        while(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == Bit_RESET);
        for(i = 0; i < 0x1000; i++);
        
        // 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

### 4.2 多按键应用

```c
#include "stm32f10x.h"

// 多按键定义
#define KEY1_GPIO_PORT   GPIOA
#define KEY1_GPIO_PIN    GPIO_Pin_0  // EXTI0

#define KEY2_GPIO_PORT   GPIOA  
#define KEY2_GPIO_PIN    GPIO_Pin_1  // EXTI1

#define KEY3_GPIO_PORT   GPIOB
#define KEY3_GPIO_PIN    GPIO_Pin_5  // EXTI9_5

void Multi_KEY_EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                          RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置按键GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 按键1 (PA0)
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
    
    // 按键2 (PA1)  
    GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
    
    // 按键3 (PB5)
    GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
    
    // 连接GPIO到EXTI线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  // KEY1 -> EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);  // KEY2 -> EXTI1  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);  // KEY3 -> EXTI5
    
    // 配置EXTI0 (KEY1)
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置EXTI1 (KEY2)
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置EXTI5 (KEY3)
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // EXTI0中断 (高优先级)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // EXTI1中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // EXTI9_5中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

// 中断服务函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 处理KEY1按键
        // ...
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        // 处理KEY2按键
        // ...
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        // 处理KEY3按键
        // ...
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}
```

---

## 第五部分：高级应用与调试技巧

### 5.1 软件触发EXTI中断

```c
void EXTI_Software_Trigger(void)
{
    // 通过软件产生EXTI中断（用于测试）
    
    // 方法1：使用SWIER寄存器
    EXTI->SWIER |= EXTI_Line0;  // 产生EXTI0软件中断
    
    // 方法2：使用标准库函数
    EXTI_GenerateSWInterrupt(EXTI_Line0);
}

// 软件中断事件函数
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);
```

### 5.2 事件模式配置

```c
void EXTI_Event_Mode_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    // 配置EXTI为事件模式（不产生中断，用于唤醒CPU或触发DMA）
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;      // 事件模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // 事件模式不需要配置NVIC！
}
```

### 5.3 防抖动处理

```c
// 高级防抖动处理
void EXTI0_IRQHandler_With_Debounce(void)
{
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time;
    
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 获取当前时间（需要配置一个定时器来提供时间基准）
        current_time = Get_System_Tick();
        
        // 防抖动检查（至少间隔50ms）
        if((current_time - last_interrupt_time) > 50)
        {
            // 有效的按键事件
            // 执行处理逻辑...
            
            last_interrupt_time = current_time;
        }
        
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

### 5.4 调试技巧

```c
// 在中断服务函数中添加调试信息
void EXTI0_IRQHandler_Debug(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 调试方法1：切换调试引脚
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));
        
        // 调试方法2：设置变量标志（在调试器中观察）
        static uint32_t interrupt_count = 0;
        interrupt_count++;
        
        // 调试方法3：如果串口已初始化，可以发送调试信息
        // printf("EXTI0 Interrupt Count: %lu\n", interrupt_count);
        
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

## 总结

**EXTI标准库配置关键要点：**

1. **必须开启AFIO时钟**：`RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE)`

2. **GPIO到EXTI的映射**：使用`GPIO_EXTILineConfig()`函数

3. **EXTI配置结构体**：正确设置线路、模式、触发方式和使能

4. **NVIC配置**：设置中断优先级和使能中断通道

5. **中断服务函数**：
   - 检查中断标志
   - 处理中断逻辑
   - **必须清除中断标志**

6. **防抖动处理**：在中断服务函数中添加延时或时间检查

7. **共用中断**：EXTI5-9共用`EXTI9_5_IRQHandler`，EXTI10-15共用`EXTI15_10_IRQHandler`

