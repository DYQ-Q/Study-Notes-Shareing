
---

## 第一部分：GPIO 基本概念与特性

### 1.1 GPIO 是什么？

GPIO（General Purpose Input/Output）即通用输入输出端口，是MCU与外部世界交互的接口。

### 1.2 STM32 GPIO 主要特性

- **8种工作模式**：输入/输出/模拟/复用等
- **可配置输出速度**：2MHz, 25MHz, 50MHz, 100MHz
- **位操作支持**：单独的位设置和清除
- **复用功能**：引脚可映射到其他外设
- **大电流驱动**：部分引脚可驱动LED等负载

### 1.3 GPIO 内部结构简图

```
引脚 → 保护二极管 → 施密特触发器 → 输入数据寄存器
                    ↓
                输出驱动器 ← 输出数据寄存器
                    ↓
            复用功能选择器
```

---

## 第二部分：GPIO 工作模式详解

### 2.1 8种工作模式对比

| 模式 | 说明 | 应用场景 | 输出状态 | 输入状态 |
|------|------|----------|----------|----------|
| **输入浮空** | 浮空输入，电平不确定 | 外部按键、通信线路 | 高阻抗 | 电平不确定 |
| **输入上拉** | 内部上拉电阻使能 | 按键输入，默认高电平 | 高阻抗 | 内部上拉到VDD |
| **输入下拉** | 内部下拉电阻使能 | 按键输入，默认低电平 | 高阻抗 | 内部下拉到GND |
| **模拟输入** | 关闭施密特触发器 | ADC采样，模拟信号 | 高阻抗 | 直接连接到ADC |
| **开漏输出** | 只能输出低电平或高阻态 | I2C总线，电平转换 | 开漏输出 | - |
| **推挽输出** | 可输出高/低电平 | LED控制，数字输出 | 推挽输出 | - |
| **复用开漏** | 开漏输出，信号来自外设 | I2C的SDA/SCL | 开漏输出 | - |
| **复用推挽** | 推挽输出，信号来自外设 | SPI, USART等 | 推挽输出 | - |

### 2.2 各种模式详细说明

#### 输入浮空模式（GPIO_Mode_IN_FLOATING）

特点：
- 引脚处于高阻抗状态
- 电平完全由外部电路决定
- 内部既不上拉也不下拉
应用：
- 外部按键（需外部上拉/下拉）
- 通信线路（如UART）
- 数字传感器接口



#### 输入上拉模式（GPIO_Mode_IPU）
 特点：
 - 内部上拉电阻连接到VDD
 - 默认输入为高电平
 - 外部拉低时读到的低电平
 应用：
  - 按键（按键另一端接地）
 - 默认需要高电平的输入

#### 输入下拉模式（GPIO_Mode_IPD）
 特点：
- 内部下拉电阻连接到GND
- 默认输入为低电平
- 外部拉高时读到的高电平 
 应用：
 - 按键（按键另一端接VDD）
 - 默认需要低电平的输入
#### 模拟输入模式（GPIO_Mode_AIN）
 特点：
 - 施密特触发器被禁用
 - 输出驱动器被禁用
 - 引脚直接连接到ADC/DAC 
 应用：
 - ADC模拟信号采样
 - DAC模拟信号输出
 - 高阻抗模拟电路


#### 推挽输出模式（GPIO_Mode_Out_PP）
特点：
 - 可以输出高电平（接近VDD）和低电平（接近GND）
 - 有较强的驱动能力
 - 输出高时：P-MOS导通，N-MOS截止
- 输出低时：P-MOS截止，N-MOS导通 
应用：
 - LED控制
 - 驱动数字器件
 - 普通数字输出


#### 开漏输出模式（GPIO_Mode_Out_OD）
 特点：
- 只能输出低电平或高阻态
- 输出高时：高阻态（需要外部上拉）
- 输出低时：N-MOS导通
- 支持"线与"连接
应用：
- I2C总线
- 电平转换
- 多设备共享总线


#### 复用功能模式
特点：
 - 输出信号来自片上外设而非CPU   
 - 复用推挽：用于SPI、USART等
 - 复用开漏：用于I2C等 
应用：
 - 所有使用外设通信的场景
 - USART、SPI、I2C、TIM等


---

## 第三部分：GPIO 寄存器详解

### 3.1 GPIO 寄存器结构

STM32的每个GPIO端口都有以下寄存器：

```c
typedef struct
{
    __IO uint32_t CRL;    // 端口配置低寄存器（PIN0-7）
    __IO uint32_t CRH;    // 端口配置高寄存器（PIN8-15）
    __IO uint32_t IDR;    // 端口输入数据寄存器
    __IO uint32_t ODR;    // 端口输出数据寄存器
    __IO uint32_t BSRR;   // 端口位设置/清除寄存器
    __IO uint32_t BRR;    // 端口位清除寄存器
    __IO uint32_t LCKR;   // 端口配置锁定寄存器
} GPIO_TypeDef;
```

### 3.2 CRL 和 CRH 寄存器（配置寄存器）

每个引脚由4个配置位控制：

```
CRL/CRH寄存器格式：
位31:30 - CNFy[1:0] (配置模式)
位29:28 - MODEy[1:0] (模式选择)
位27:24 - 保留
位23:20 - 同上，用于下一个引脚...
```

#### 配置模式（CNFy[1:0]）

| MODEy[1:0] | CNFy[1:0] | 模式 | 说明 |
|------------|-----------|------|------|
| 00 (输入) | 00 | 模拟输入 | 用于ADC/DAC |
| 00 (输入) | 01 | 浮空输入 | 复位后的状态 |
| 00 (输入) | 10 | 上拉/下拉输入 | 由ODR寄存器决定 |
| 01 (输出10MHz) | 00 | 通用推挽输出 | |
| 01 (输出10MHz) | 01 | 通用开漏输出 | |
| 01 (输出10MHz) | 10 | 复用功能推挽输出 | |
| 01 (输出10MHz) | 11 | 复用功能开漏输出 | |
| 10 (输出2MHz) | 同上 | 同上 | 速度2MHz |
| 11 (输出50MHz) | 同上 | 同上 | 速度50MHz |

### 3.3 其他重要寄存器

#### IDR - 输入数据寄存器
- 只读寄存器
- 位0-15对应引脚0-15的输入状态
- 读取时获得引脚的当前电平

#### ODR - 输出数据寄存器
- 读写寄存器
- 位0-15对应引脚0-15的输出状态
- 写1输出高电平，写0输出低电平

#### BSRR - 位设置/清除寄存器
- 写1有效的寄存器
- 低16位：设置对应位（输出1）
- 高16位：清除对应位（输出0）
- 原子操作，不会产生读-修改-写问题

#### BRR - 位清除寄存器
- 写1有效的寄存器
- 低16位：清除对应位（输出0）
- 高16位：保留

---

## 第四部分：标准库 GPIO 配置

### 4.1 GPIO 初始化结构体

```c
typedef struct
{
    uint16_t GPIO_Pin;              // 选择要配置的引脚
    GPIOSpeed_TypeDef GPIO_Speed;   // 输出速度
    GPIMode_TypeDef GPIO_Mode;      // 工作模式
} GPIO_InitTypeDef;
```

### 4.2 引脚定义

```c
// 单个引脚定义
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /* All pins selected */

// 多个引脚组合
#define GPIO_PIN_MASK              ((uint32_t)0x0000FFFF) /* PIN mask for assert test */
```

### 4.3 输出速度枚举

```c
typedef enum
{ 
    GPIO_Speed_10MHz = 1,  // 低速10MHz
    GPIO_Speed_2MHz,       // 中速2MHz  
    GPIO_Speed_50MHz       // 高速50MHz
} GPIOSpeed_TypeDef;
```

### 4.4 工作模式枚举

```c
typedef enum
{ 
    GPIO_Mode_AIN = 0x0,           // 模拟输入
    GPIO_Mode_IN_FLOATING = 0x04,  // 浮空输入
    GPIO_Mode_IPD = 0x28,          // 下拉输入
    GPIO_Mode_IPU = 0x48,          // 上拉输入
    GPIO_Mode_Out_OD = 0x14,       // 开漏输出
    GPIO_Mode_Out_PP = 0x10,       // 推挽输出
    GPIO_Mode_AF_OD = 0x1C,        // 复用开漏
    GPIO_Mode_AF_PP = 0x18         // 复用推挽
} GPIOMode_TypeDef;
```

---

## 第五部分：GPIO 配置实例

### 5.1 基本GPIO配置流程

```c
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 步骤1：开启GPIO时钟（必须！） */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                          RCC_APB2Periph_GPIOC, ENABLE);
    
    /* 步骤2：配置LED引脚 - 推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;        // PB5 - LED
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// 50MHz速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* 步骤3：配置按键引脚 - 上拉输入 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;        // PA0 - 按键
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // 输入模式速度不重要
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤4：配置USART引脚 - 复用推挽 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        // PA9 - USART1_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       // PA10 - USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤5：配置ADC引脚 - 模拟输入 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;        // PC0 - ADC
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    // 模拟输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
```

### 5.2 GPIO 读写操作

```c
void GPIO_ReadWrite_Operations(void)
{
    /* 写入操作 */
    
    // 方法1：使用ODR寄存器（会改变整个端口）
    GPIOB->ODR = 0x0020;  // 设置PB5=1，其他=0
    
    // 方法2：使用BSRR寄存器（推荐 - 原子操作）
    GPIOB->BSRR = GPIO_Pin_5;        // 设置PB5（输出1）
    GPIOB->BSRR = GPIO_Pin_5 << 16;  // 清除PB5（输出0）
    
    // 方法3：使用标准库函数
    GPIO_SetBits(GPIOB, GPIO_Pin_5);    // 设置引脚
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);  // 清除引脚
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);   // 写1
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); // 写0
    
    // 方法4：写整个端口
    GPIO_Write(GPIOB, 0x0020);  // PB5=1，其他=0
    
    /* 读取操作 */
    
    // 方法1：读取输入状态
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
    {
        // PA0为低电平（按键按下）
        GPIO_SetBits(GPIOB, GPIO_Pin_5);  // 点亮LED
    }
    
    // 方法2：读取输出状态
    if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == Bit_SET)
    {
        // PB5输出高电平
    }
    
    // 方法3：读取整个端口输入
    uint16_t port_input = GPIO_ReadInputData(GPIOA);
    
    // 方法4：读取整个端口输出
    uint16_t port_output = GPIO_ReadOutputData(GPIOB);
}
```

### 5.3 位带操作

STM32支持位带操作，可以实现类似51单片机的sbit功能：

```c
// 位带操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND((volatile unsigned long)addr, bitnum))

// GPIO位带别名区
#define GPIOA_ODR_Addr (GPIOA_BASE + 0x0C)
#define GPIOB_ODR_Addr (GPIOB_BASE + 0x0C)
#define GPIOC_ODR_Addr (GPIOC_BASE + 0x0C)

#define GPIOA_IDR_Addr (GPIOA_BASE + 0x08)
#define GPIOB_IDR_Addr (GPIOB_BASE + 0x08)
#define GPIOC_IDR_Addr (GPIOC_BASE + 0x08)

// 定义LED和按键的位带别名
#define LED PBout(5)    // PB5输出
#define KEY PAin(0)     // PA0输入

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n)   // GPIOA输出
#define PAin(n)  BIT_ADDR(GPIOA_IDR_Addr, n)   // GPIOA输入
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)   // GPIOB输出
#define PBin(n)  BIT_ADDR(GPIOB_IDR_Addr, n)   // GPIOB输入
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n)   // GPIOC输出
#define PCin(n)  BIT_ADDR(GPIOC_IDR_Addr, n)   // GPIOC输入

void BitBand_Example(void)
{
    // 使用位带操作 - 像51单片机一样直观！
    LED = 1;        // 点亮LED
    LED = 0;        // 熄灭LED
    
    if(KEY == 0)    // 检测按键
    {
        LED = 1;    // 按键按下，点亮LED
    }
    
    // 位带操作是原子操作，不会被中断打断
}
```

---

## 第六部分：高级应用

### 6.1 矩阵键盘扫描

```c
void Matrix_Keyboard_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // 行线（输出）：PA0-PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 列线（输入）：PB0-PB3，上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Matrix_Keyboard_Scan(void)
{
    uint8_t row, col, key_value = 0;
    
    for(row = 0; row < 4; row++)
    {
        // 设置当前行为低电平，其他行为高电平
        GPIO_Write(GPIOA, ~(1 << row) & 0x000F);
        
        // 延时消抖
        Delay_us(10);
        
        // 读取列线状态
        uint16_t col_state = GPIO_ReadInputData(GPIOB) & 0x000F;
        
        // 检查哪一列为低电平
        for(col = 0; col < 4; col++)
        {
            if((col_state & (1 << col)) == 0)
            {
                key_value = row * 4 + col + 1;  // 计算键值
                break;
            }
        }
        
        if(key_value != 0) break;
    }
    
    // 所有行恢复高电平
    GPIO_Write(GPIOA, 0x000F);
    
    return key_value;
}
```

### 6.2 LED点阵显示

```c
void LED_Matrix_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // 行驱动（输出）：PA0-PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 列驱动（输出）：PB0-PB7
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED_Matrix_Display(uint8_t *pattern)
{
    uint8_t row;
    
    for(row = 0; row < 8; row++)
    {
        // 设置行（共阳极，低电平有效）
        GPIO_Write(GPIOA, ~(1 << row));
        
        // 设置列数据（共阴极，高电平有效）
        GPIO_Write(GPIOB, pattern[row]);
        
        // 延时保持显示
        Delay_us(1000);
        
        // 消隐
        GPIO_Write(GPIOB, 0x00);
    }
}
```

### 6.3 软件模拟I2C

```c
// 软件I2C GPIO定义
#define I2C_SCL_PIN    GPIO_Pin_6
#define I2C_SDA_PIN    GPIO_Pin_7
#define I2C_GPIO       GPIOB

#define SCL_HIGH()     GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN)
#define SCL_LOW()      GPIO_ResetBits(I2C_GPIO, I2C_SCL_PIN)
#define SDA_HIGH()     GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN)
#define SDA_LOW()      GPIO_ResetBits(I2C_GPIO, I2C_SDA_PIN)
#define SDA_READ()     GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA_PIN)

void I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // SCL和SDA初始化为开漏输出
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
    
    // 初始状态：SCL和SDA都为高电平
    SCL_HIGH();
    SDA_HIGH();
}

void I2C_Start(void)
{
    SDA_HIGH();
    SCL_HIGH();
    Delay_us(5);
    SDA_LOW();
    Delay_us(5);
    SCL_LOW();
}

void I2C_Stop(void)
{
    SDA_LOW();
    SCL_HIGH();
    Delay_us(5);
    SDA_HIGH();
    Delay_us(5);
}
```

---

## 第七部分：调试技巧与最佳实践

### 7.1 未使用引脚的处理

```c
void Unused_Pin_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* 未使用引脚的处理方法 */
    
    // 方法1：配置为模拟输入（推荐）
    // 功耗最低，不会干扰外部电路
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 方法2：配置为输出低电平
    // 固定电平，不会浮动
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
    
    // 避免：浮空输入模式
    // 可能产生功耗问题或受干扰
}
```

### 7.2 GPIO配置检查清单

```c
void GPIO_Checklist(void)
{
    /*
    GPIO配置检查清单：
    
    ✅ 1. 开启GPIO时钟了吗？
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    ✅ 2. 模式选择正确吗？
        - 普通输出：GPIO_Mode_Out_PP 或 GPIO_Mode_Out_OD
        - 普通输入：GPIO_Mode_IPU 或 GPIO_Mode_IPD 或 GPIO_Mode_IN_FLOATING
        - 外设功能：GPIO_Mode_AF_PP 或 GPIO_Mode_AF_OD
        - 模拟功能：GPIO_Mode_AIN
    
    ✅ 3. 速度设置合适吗？
        - 普通IO：GPIO_Speed_2MHz 或 GPIO_Speed_50MHz
        - 输入模式：速度不重要
        - 高速通信：GPIO_Speed_50MHz
    
    ✅ 4. 上拉/下拉配置正确吗？
        - 按键（接地）：GPIO_Mode_IPU
        - 按键（接VDD）：GPIO_Mode_IPD
        - 通信线路：GPIO_Mode_IN_FLOATING（外加合适的上拉）
    
    ✅ 5. 未使用引脚处理了吗？
        - 推荐：GPIO_Mode_AIN
    
    ✅ 6. 复用功能需要开启AFIO时钟吗？
        - 引脚重映射时需要：RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    */
}
```

### 7.3 常见问题与解决方案

```c
void GPIO_Common_Issues(void)
{
    /*
    常见问题1：GPIO无法正常工作
    - 原因：忘记开启GPIO时钟
    - 解决：调用 RCC_APB2PeriphClockCmd()
    
    常见问题2：输出电平不正确
    - 原因：模式配置错误（如输入模式尝试输出）
    - 解决：检查GPIO_Mode配置
    
    常见问题3：输入读取值不稳定
    - 原因：浮空输入且外部无上拉/下拉
    - 解决：配置为上拉或下拉输入，或外部加上拉电阻
    
    常见问题4：输出驱动能力不足
    - 原因：驱动大负载（如电机、多个LED）
    - 解决：外加驱动电路（三极管、MOS管、驱动芯片）
    
    常见问题5：通信波形失真
    - 原因：输出速度设置过低
    - 解决：提高GPIO速度（GPIO_Speed_50MHz）
    */
}
```

## 总结

**STM32 GPIO 核心要点：**

1. **8种工作模式**：根据应用场景选择合适的模式
2. **时钟使能**：使用GPIO前必须开启对应时钟
3. **速度配置**：根据需求选择合适的速度等级
4. **输入配置**：合理使用上拉/下拉电阻
5. **输出配置**：推挽输出用于普通驱动，开漏输出用于总线
6. **复用功能**：外设通信使用复用模式
7. **位带操作**：实现类似51的位操作，提高效率

**推荐配置：**
- LED控制：推挽输出，50MHz
- 按键输入：上拉输入，2MHz  
- USART：复用推挽（TX），浮空输入（RX），50MHz
- I2C：复用开漏，50MHz
- ADC：模拟输入
- 未使用引脚：模拟输入
