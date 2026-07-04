好的，我们来详细讲解STM32的USART（通用同步/异步收发器）。USART是STM32中最常用、最重要的通信接口之一。

---

## 第一部分：USART 概述

### 1.1 USART vs UART

```c
void USART_UART_Comparison(void)
{
    /*
    USART (Universal Synchronous/Asynchronous Receiver/Transmitter)
    UART (Universal Asynchronous Receiver/Transmitter)
    
    主要区别：
    - UART：仅支持异步通信
    - USART：支持同步和异步通信
    
    STM32的USART特性：
    - 全双工异步通信
    - 单线半双工通信
    - 同步通信（需要时钟线）
    - LIN总线支持
    - 智能卡模式
    - IrDA红外模式
    - DMA支持
    */
}
```

### 1.2 USART 主要特性

- **全双工异步通信**：可同时收发
- **可编程波特率**：最高4.5Mbps
- **数据帧格式**：可配置数据位、停止位、校验位
- **硬件流控制**：支持RTS/CTS
- **多处理器通信**：地址位识别
- **多种中断源**：发送完成、接收、错误等
- **DMA支持**：高效数据传输

### 1.3 USART 内部结构简图

```
TX引脚 → 发送移位寄存器 → 发送数据寄存器(TDR)
RX引脚 ← 接收移位寄存器 ← 接收数据寄存器(RDR)

控制逻辑：波特率生成、帧格式、中断/DMA控制
时钟源：APB总线时钟
```

---

## 第二部分：USART 寄存器详解

### 2.1 USART 主要寄存器

```c
typedef struct
{
    __IO uint16_t SR;         // 状态寄存器
    __IO uint16_t RESERVED0;
    __IO uint16_t DR;         // 数据寄存器
    __IO uint16_t RESERVED1;
    __IO uint16_t BRR;        // 波特率寄存器
    __IO uint16_t RESERVED2;
    __IO uint16_t CR1;        // 控制寄存器1
    __IO uint16_t RESERVED3;
    __IO uint16_t CR2;        // 控制寄存器2
    __IO uint16_t RESERVED4;
    __IO uint16_t CR3;        // 控制寄存器3
    __IO uint16_t RESERVED5;
    __IO uint16_t GTPR;       // 保护时间和预分频寄存器
} USART_TypeDef;
```

### 2.2 关键寄存器详解

#### SR - 状态寄存器
```c
// SR重要位：
#define USART_SR_TXE          ((uint16_t)0x0080)  // 发送数据寄存器空
#define USART_SR_TC           ((uint16_t)0x0040)  // 发送完成
#define USART_SR_RXNE         ((uint16_t)0x0020)  // 接收数据寄存器非空
#define USART_SR_IDLE         ((uint16_t)0x0010)  // 检测到空闲线路
#define USART_SR_ORE          ((uint16_t)0x0008)  // 溢出错误
#define USART_SR_NE           ((uint16_t)0x0004)  // 噪声错误
#define USART_SR_FE           ((uint16_t)0x0002)  // 帧错误
#define USART_SR_PE           ((uint16_t)0x0001)  // 奇偶校验错误
```

#### DR - 数据寄存器
```c
// DR寄存器特点：
// 低9位：实际数据（8位或9位数据）
// 读写不同物理寄存器：
// - 写DR：写入TDR（发送数据寄存器）
// - 读DR：读取RDR（接收数据寄存器）
```

#### BRR - 波特率寄存器
```c
// 波特率计算公式：
// 波特率 = fCK / (16 × USARTDIV)
// 其中USARTDIV = DIV_Mantissa + (DIV_Fraction / 16)

// BRR寄存器组成：
// [15:4] DIV_Mantissa 整数部分
// [3:0] DIV_Fraction 小数部分（×16）
```

#### CR1 - 控制寄存器1
```c
// CR1重要位：
#define USART_CR1_UE          ((uint16_t)0x2000)  // USART使能
#define USART_CR1_M           ((uint16_t)0x1000)  // 字长（0=8位，1=9位）
#define USART_CR1_PCE         ((uint16_t)0x0400)  // 奇偶校验控制使能
#define USART_CR1_PS          ((uint16_t)0x0200)  // 奇偶校验选择（0=偶，1=奇）
#define USART_CR1_TXEIE       ((uint16_t)0x0080)  // 发送缓冲区空中断使能
#define USART_CR1_TCIE        ((uint16_t)0x0040)  // 发送完成中断使能
#define USART_CR1_RXNEIE      ((uint16_t)0x0020)  // 接收中断使能
#define USART_CR1_IDLEIE      ((uint16_t)0x0010)  // 空闲中断使能
#define USART_CR1_TE          ((uint16_t)0x0008)  // 发送使能
#define USART_CR1_RE          ((uint16_t)0x0004)  // 接收使能
```

#### CR2 - 控制寄存器2
```c
// CR2重要位：
#define USART_CR2_LINEN       ((uint16_t)0x4000)  // LIN模式使能
#define USART_CR2_STOP        ((uint16_t)0x3000)  // 停止位（2位）
#define USART_CR2_CLKEN       ((uint16_t)0x0400)  // 时钟使能（同步模式）
#define USART_CR2_CPOL        ((uint16_t)0x0200)  // 时钟极性
#define USART_CR2_CPHA        ((uint16_t)0x0100)  // 时钟相位
#define USART_CR2_LBCL        ((uint16_t)0x0080)  // 最后一位时钟脉冲
```

#### CR3 - 控制寄存器3
```c
// CR3重要位：
#define USART_CR3_CTSIE       ((uint16_t)0x0400)  // CTS中断使能
#define USART_CR3_CTSE        ((uint16_t)0x0200)  // CTS硬件流控制使能
#define USART_CR3_RTSE        ((uint16_t)0x0100)  // RTS硬件流控制使能
#define USART_CR3_DMAT        ((uint16_t)0x0080)  // 发送DMA使能
#define USART_CR3_DMAR        ((uint16_t)0x0040)  // 接收DMA使能
#define USART_CR3_SCEN        ((uint16_t)0x0020)  // 智能卡模式使能
#define USART_CR3_NACK        ((uint16_t)0x0010)  // 智能卡NACK使能
#define USART_CR3_HDSEL       ((uint16_t)0x0008)  // 半双工选择
#define USART_CR3_IRLP        ((uint16_t)0x0004)  // 红外低功耗
#define USART_CR3_IREN        ((uint16_t)0x0002)  // 红外模式使能
```

---

## 第三部分：USART 配置与使用

### 3.1 USART 初始化结构体

```c
// USART初始化结构体
typedef struct
{
    uint32_t USART_BaudRate;            // 波特率
    uint16_t USART_WordLength;          // 字长
    uint16_t USART_StopBits;            // 停止位
    uint16_t USART_Parity;              // 校验位
    uint16_t USART_Mode;                // 收发模式
    uint16_t USART_HardwareFlowControl; // 硬件流控制
} USART_InitTypeDef;

// 字长定义
#define USART_WordLength_8b          ((uint16_t)0x0000)  // 8位数据
#define USART_WordLength_9b          ((uint16_t)0x1000)  // 9位数据

// 停止位定义
#define USART_StopBits_1             ((uint16_t)0x0000)  // 1个停止位
#define USART_StopBits_0_5           ((uint16_t)0x1000)  // 0.5个停止位
#define USART_StopBits_2             ((uint16_t)0x2000)  // 2个停止位
#define USART_StopBits_1_5           ((uint16_t)0x3000)  // 1.5个停止位

// 校验位定义
#define USART_Parity_No              ((uint16_t)0x0000)  // 无校验
#define USART_Parity_Even            ((uint16_t)0x0400)  // 偶校验
#define USART_Parity_Odd             ((uint16_t)0x0600)  // 奇校验

// 模式定义
#define USART_Mode_Rx                ((uint16_t)0x0004)  // 接收模式
#define USART_Mode_Tx                ((uint16_t)0x0008)  // 发送模式

// 硬件流控制定义
#define USART_HardwareFlowControl_None   ((uint16_t)0x0000)  // 无流控
#define USART_HardwareFlowControl_RTS    ((uint16_t)0x0100)  // RTS流控
#define USART_HardwareFlowControl_CTS    ((uint16_t)0x0200)  // CTS流控
#define USART_HardwareFlowControl_RTS_CTS ((uint16_t)0x0300) // RTS+CTS流控
```

### 3.2 基本USART配置

```c
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | 
                          RCC_APB2Periph_AFIO, ENABLE);
    
    /* 步骤2：配置GPIO */
    // PA9 - USART1_TX (复用推挽输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       // 复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA10 - USART1_RX (浮空输入)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置USART参数 */
    USART_InitStructure.USART_BaudRate = 115200;          // 波特率115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       // 1位停止
    USART_InitStructure.USART_Parity = USART_Parity_No;          // 无校验
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;     // 无流控
    USART_Init(USART1, &USART_InitStructure);
    
    /* 步骤4：配置USART中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        // 使能接收中断
    
    /* 步骤5：配置NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 步骤6：使能USART */
    USART_Cmd(USART1, ENABLE);
}
```

### 3.3 波特率计算

```c
// 波特率计算函数
void USART_BaudRate_Calculation(void)
{
    /*
    波特率计算公式：
    
    对于STM32F1系列：
    波特率 = fCK / (16 × USARTDIV)
    
    其中：
    fCK = USART时钟频率（APB2或APB1）
    USARTDIV = 整数部分 + 小数部分/16
    
    例：72MHz时钟，115200波特率
    USARTDIV = 72000000 / (16 × 115200) = 39.0625
    整数部分 = 39
    小数部分 = 0.0625 × 16 = 1
    
    BRR寄存器值 = (39 << 4) | 1 = 0x0271
    */
    
    // 计算函数
    uint16_t USART_BRR_Calculation(uint32_t clock_freq, uint32_t baudrate)
    {
        uint32_t integer_divider, fractional_divider;
        uint16_t brr_value;
        
        // 计算整数部分
        integer_divider = (25 * clock_freq) / (4 * baudrate);
        
        // 计算BRR值
        brr_value = (integer_divider / 100) << 4;
        brr_value |= (integer_divider - (100 * (integer_divider / 100))) << 4;
        brr_value /= 100;
        
        return brr_value;
    }
}
```

### 3.4 USART 发送函数

```c
// USART发送单个字符
void USART_SendChar(USART_TypeDef* USARTx, uint8_t ch)
{
    // 等待发送缓冲区空
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    
    // 发送数据
    USART_SendData(USARTx, ch);
}

// USART发送字符串
void USART_SendString(USART_TypeDef* USARTx, char *str)
{
    while(*str)
    {
        USART_SendChar(USARTx, *str++);
    }
}

// USART发送数据块
void USART_SendBuffer(USART_TypeDef* USARTx, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    
    for(i = 0; i < length; i++)
    {
        USART_SendChar(USARTx, buffer[i]);
    }
}

// 格式化输出（类似printf）
#include <stdarg.h>
#include <stdio.h>

void USART_Printf(USART_TypeDef* USARTx, const char *format, ...)
{
    char buffer[256];
    va_list args;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    USART_SendString(USARTx, buffer);
}
```

### 3.5 USART 接收函数

```c
// USART接收单个字符（阻塞式）
uint8_t USART_ReceiveChar(USART_TypeDef* USARTx)
{
    // 等待接收到数据
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    
    // 读取数据
    return (uint8_t)USART_ReceiveData(USARTx);
}

// USART接收字符串（带超时）
uint8_t USART_ReceiveString(USART_TypeDef* USARTx, char *buffer, uint16_t max_len, uint32_t timeout)
{
    uint16_t index = 0;
    uint32_t start_time = GetTickCount();
    
    while(index < max_len - 1)
    {
        // 检查超时
        if(GetTickCount() - start_time > timeout)
        {
            buffer[index] = '\0';
            return 0;  // 超时
        }
        
        // 检查是否有数据
        if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) != RESET)
        {
            buffer[index] = USART_ReceiveData(USARTx);
            
            // 检查结束符（回车或换行）
            if(buffer[index] == '\r' || buffer[index] == '\n')
            {
                buffer[index] = '\0';
                return 1;  // 成功接收
            }
            
            index++;
            start_time = GetTickCount();  // 重置超时计时器
        }
    }
    
    buffer[index] = '\0';
    return 0;  // 缓冲区满
}
```

### 3.6 USART 中断服务函数

```c
// USART1中断服务函数
volatile uint8_t usart_rx_buffer[256];
volatile uint16_t usart_rx_index = 0;
volatile uint8_t usart_rx_complete = 0;

void USART1_IRQHandler(void)
{
    uint8_t received_data;
    
    /* 接收中断 */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        received_data = USART_ReceiveData(USART1);
        
        // 简单的命令解析（以回车结束）
        if(received_data == '\r' || received_data == '\n')
        {
            if(usart_rx_index > 0)
            {
                usart_rx_buffer[usart_rx_index] = '\0';  // 字符串结束符
                usart_rx_complete = 1;
                usart_rx_index = 0;
            }
        }
        else if(usart_rx_index < sizeof(usart_rx_buffer) - 1)
        {
            usart_rx_buffer[usart_rx_index++] = received_data;
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    
    /* 发送完成中断 */
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        // 发送完成处理
        // 例如：可以启动下一次发送
        
        USART_ClearITPendingBit(USART1, USART_IT_TC);
    }
    
    /* 错误中断处理 */
    if(USART_GetITStatus(USART1, USART_IT_PE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_PE);
    }
    if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_FE);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART1, USART_IT_NE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_NE);
    }
}
```

---

## 第四部分：高级功能配置

### 4.1 硬件流控制配置

```c
void USART_HardwareFlowControl_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    /* 步骤1：开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | 
                          RCC_APB2Periph_AFIO, ENABLE);
    
    /* 步骤2：配置GPIO */
    // PA9 - TX, PA10 - RX
    // PA11 - CTS, PA12 - RTS
    
    // TX - 复用推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // RX - 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // CTS - 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // RTS - 复用推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 步骤3：配置USART带硬件流控 */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS; // RTS+CTS流控
    USART_Init(USART1, &USART_InitStructure);
    
    /* 步骤4：使能USART */
    USART_Cmd(USART1, ENABLE);
}
```

### 4.2 多处理器通信模式

```c
void USART_Multiprocessor_Config(void)
{
    /*
    多处理器通信特点：
    - 多个设备共享一条总线
    - 通过地址识别目标设备
    - 减少总线竞争
    */
    
    USART_InitTypeDef USART_InitStructure;
    
    // 配置USART为多处理器模式
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;  // 9位数据，MSB为地址标志
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    
    // 使能多处理器模式
    USART_SetAddress(USART1, 0x01);  // 设置本机地址
    USART_WakeUpConfig(USART1, USART_WakeUp_AddressMark);  // 地址唤醒
    USART_ReceiverWakeUpCmd(USART1, ENABLE);  // 使能接收唤醒
    
    USART_Cmd(USART1, ENABLE);
}

// 多处理器模式发送
void USART_Multiprocessor_Send(USART_TypeDef* USARTx, uint8_t address, uint8_t *data, uint16_t length)
{
    // 发送地址帧（第9位=1）
    USARTx->CR1 |= USART_CR1_M;  // 设置为9位数据
    USARTx->CR1 |= USART_CR1_TXE;  // 发送地址标志
    USART_SendData(USARTx, address);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    
    // 发送数据帧（第9位=0）
    USARTx->CR1 &= ~USART_CR1_TXE;  // 清除地址标志
    for(uint16_t i = 0; i < length; i++)
    {
        USART_SendData(USARTx, data[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}
```

### 4.3 LIN总线模式

```c
void USART_LIN_Mode_Config(void)
{
    /*
    LIN总线特点：
    - 单线通信
    - 最高20kbps
    - 主从结构
    - 低成本汽车网络
    */
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    // 配置GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  // USART2 TX/RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 开漏输出用于LIN
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置USART为LIN模式
    USART_InitStructure.USART_BaudRate = 19200;  // LIN标准速率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);
    
    // 使能LIN模式
    USART_LINCmd(USART2, ENABLE);
    USART_LINBreakDetectLengthConfig(USART2, USART_LINBreakDetectLength_11b); // 11位Break检测
    
    USART_Cmd(USART2, ENABLE);
}

// 发送LIN Break同步场
void USART_LIN_SendBreak(USART_TypeDef* USARTx)
{
    USART_SendBreak(USARTx);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_LBD) == RESET);  // 等待Break发送完成
    USART_ClearFlag(USARTx, USART_FLAG_LBD);
}
```

### 4.4 红外模式

```c
void USART_IrDA_Mode_Config(void)
{
    /*
    红外模式特点：
    - 支持IrDA物理层
    - 可配置脉冲宽度
    - 支持低功耗模式
    */
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    // 配置GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  // USART2 TX/RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置USART
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);
    
    // 配置红外模式
    USART_IrDAConfig(USART2, USART_IrDAMode_LowPower);  // 低功耗模式
    USART_SetPrescaler(USART2, 1);  // 预分频器
    
    // 使能红外模式
    USART_IrDACmd(USART2, ENABLE);
    USART_Cmd(USART2, ENABLE);
}
```

---

## 第五部分：DMA配置

### 5.1 USART DMA发送配置

```c
#include "stm32f10x_dma.h"

void USART_DMA_TX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    /* 步骤1：开启DMA时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* 步骤2：配置DMA通道4（USART1_TX） */
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  // 外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;               // 稍后设置
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                // 内存到外设
    DMA_InitStructure.DMA_BufferSize = 0;                             // 稍后设置
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;           // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  // 8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          // 8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                     // 普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;               // 高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                      // 非内存到内存
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    
    /* 步骤3：使能USART DMA发送 */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

// 使用DMA发送数据
void USART_DMA_Send(uint8_t *data, uint16_t length)
{
    // 等待上一次DMA传输完成
    while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC4);
    
    // 配置DMA
    DMA1_Channel4->CMAR = (uint32_t)data;      // 内存地址
    DMA1_Channel4->CNDTR = length;             // 传输数量
    
    // 使能DMA
    DMA_Cmd(DMA1_Channel4, ENABLE);
}
```

### 5.2 USART DMA接收配置

```c
#define RX_BUFFER_SIZE 256
uint8_t usart_rx_dma_buffer[RX_BUFFER_SIZE];

void USART_DMA_RX_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 步骤1：开启DMA时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* 步骤2：配置DMA通道5（USART1_RX） */
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  // 外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart_rx_dma_buffer; // 内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                // 外设到内存
    DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;                // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;           // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  // 8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          // 8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                   // 循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;           // 最高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                      // 非内存到内存
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    
    /* 步骤3：配置DMA中断 */
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);  // 传输完成中断
    DMA_ITConfig(DMA1_Channel5, DMA_IT_HT, ENABLE);  // 半传输中断
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 步骤4：使能USART DMA接收 */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    
    /* 步骤5：使能DMA */
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

// DMA接收中断服务函数
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        // 缓冲区全满处理
        // ...
        
        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }
    
    if(DMA_GetITStatus