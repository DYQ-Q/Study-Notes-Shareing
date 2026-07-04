# HAL库常用函数

---

## 一、系统核心函数

| 函数                       | 功能        | 示例                                         |
| ------------------------ | --------- | ------------------------------------------ |
| `HAL_Init()`             | 初始化HAL库   | `HAL_Init();`                              |
| `HAL_Delay()`            | 毫秒级延时     | `HAL_Delay(1000);`                         |
| `HAL_GetTick()`          | 获取系统滴答时间  | `tick = HAL_GetTick();`                    |
| `HAL_IncTick()`          | 滴答定时器中断回调 | 在SysTick_Handler中调用                        |
| `HAL_SuspendTick()`      | 暂停滴答定时器   | `HAL_SuspendTick();`                       |
| `HAL_ResumeTick()`       | 恢复滴答定时器   | `HAL_ResumeTick();`                        |
| `HAL_NVIC_SetPriority()` | 设置中断优先级   | `HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);` |
| `HAL_NVIC_EnableIRQ()`   | 使能中断      | `HAL_NVIC_EnableIRQ(USART1_IRQn);`         |

---

## 二、GPIO常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_GPIO_WritePin()` | 写GPIO电平 | `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);` |
| `HAL_GPIO_ReadPin()` | 读GPIO电平 | `state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);` |
| `HAL_GPIO_TogglePin()` | 翻转GPIO电平 | `HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);` |
| `HAL_GPIO_Init()` | 初始化GPIO | `HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);` |
| `HAL_GPIO_DeInit()` | 反初始化GPIO | `HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);` |
| `HAL_GPIO_LockPin()` | 锁定GPIO配置 | `HAL_GPIO_LockPin(GPIOA, GPIO_PIN_5);` |

**GPIO电平宏定义：**
```c
GPIO_PIN_SET    // 高电平
GPIO_PIN_RESET  // 低电平
```

---

## 三、UART/USART常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_UART_Transmit()` | 阻塞发送 | `HAL_UART_Transmit(&huart1, buf, len, 100);` |
| `HAL_UART_Receive()` | 阻塞接收 | `HAL_UART_Receive(&huart1, buf, len, 100);` |
| `HAL_UART_Transmit_IT()` | 中断发送 | `HAL_UART_Transmit_IT(&huart1, buf, len);` |
| `HAL_UART_Receive_IT()` | 中断接收 | `HAL_UART_Receive_IT(&huart1, buf, len);` |
| `HAL_UART_Transmit_DMA()` | DMA发送 | `HAL_UART_Transmit_DMA(&huart1, buf, len);` |
| `HAL_UART_Receive_DMA()` | DMA接收 | `HAL_UART_Receive_DMA(&huart1, buf, len);` |
| `HAL_UART_Init()` | 初始化UART | `HAL_UART_Init(&huart1);` |
| `HAL_UART_DeInit()` | 反初始化UART | `HAL_UART_DeInit(&huart1);` |
| `HAL_UART_IRQHandler()` | 中断处理函数 | 在USART1_IRQHandler中调用 |

**常用回调函数：**
```c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);    // 发送完成
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);    // 接收完成
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);     // 错误处理
```

---

## 四、定时器常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_TIM_Base_Start()` | 启动基本定时器 | `HAL_TIM_Base_Start(&htim2);` |
| `HAL_TIM_Base_Stop()` | 停止基本定时器 | `HAL_TIM_Base_Stop(&htim2);` |
| `HAL_TIM_Base_Start_IT()` | 启动定时器+中断 | `HAL_TIM_Base_Start_IT(&htim2);` |
| `HAL_TIM_PWM_Start()` | 启动PWM输出 | `HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);` |
| `HAL_TIM_PWM_Stop()` | 停止PWM输出 | `HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);` |
| `HAL_TIM_Encoder_Start()` | 启动编码器模式 | `HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);` |
| `HAL_TIM_IC_Start()` | 启动输入捕获 | `HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_1);` |
| `HAL_TIM_OC_Start()` | 启动输出比较 | `HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_1);` |
| `__HAL_TIM_SET_COMPARE()` | 设置比较值(PWM占空比) | `__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500);` |
| `__HAL_TIM_GET_COUNTER()` | 获取计数器值 | `cnt = __HAL_TIM_GET_COUNTER(&htim2);` |

**常用回调函数：**
```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);  // 更新事件
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);     // 输入捕获
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim); // PWM脉冲完成
```

---

## 五、ADC常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_ADC_Start()` | 启动ADC转换 | `HAL_ADC_Start(&hadc1);` |
| `HAL_ADC_Stop()` | 停止ADC转换 | `HAL_ADC_Stop(&hadc1);` |
| `HAL_ADC_Start_IT()` | 启动ADC+中断 | `HAL_ADC_Start_IT(&hadc1);` |
| `HAL_ADC_Start_DMA()` | 启动ADC+DMA | `HAL_ADC_Start_DMA(&hadc1, &value, 1);` |
| `HAL_ADC_PollForConversion()` | 等待转换完成 | `HAL_ADC_PollForConversion(&hadc1, 100);` |
| `HAL_ADC_GetValue()` | 获取转换结果 | `value = HAL_ADC_GetValue(&hadc1);` |
| `HAL_ADC_ConfigChannel()` | 配置ADC通道 | `HAL_ADC_ConfigChannel(&hadc1, &sConfig);` |
| `HAL_ADC_Init()` | 初始化ADC | `HAL_ADC_Init(&hadc1);` |

**常用回调函数：**
```c
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);      // 转换完成
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);  // 转换完成一半
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc); // 超出阈值
```

---

## 六、SPI常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_SPI_Transmit()` | 阻塞发送 | `HAL_SPI_Transmit(&hspi1, txBuf, len, 100);` |
| `HAL_SPI_Receive()` | 阻塞接收 | `HAL_SPI_Receive(&hspi1, rxBuf, len, 100);` |
| `HAL_SPI_TransmitReceive()` | 阻塞收发 | `HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, len, 100);` |
| `HAL_SPI_Transmit_IT()` | 中断发送 | `HAL_SPI_Transmit_IT(&hspi1, txBuf, len);` |
| `HAL_SPI_Receive_IT()` | 中断接收 | `HAL_SPI_Receive_IT(&hspi1, rxBuf, len);` |
| `HAL_SPI_Transmit_DMA()` | DMA发送 | `HAL_SPI_Transmit_DMA(&hspi1, txBuf, len);` |
| `HAL_SPI_Receive_DMA()` | DMA接收 | `HAL_SPI_Receive_DMA(&hspi1, rxBuf, len);` |
| `HAL_SPI_Init()` | 初始化SPI | `HAL_SPI_Init(&hspi1);` |

**常用回调函数：**
```c
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);    // 发送完成
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);    // 接收完成
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);  // 收发完成
```

---

## 七、I2C常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_I2C_Master_Transmit()` | 主机发送 | `HAL_I2C_Master_Transmit(&hi2c1, devAddr, buf, len, 100);` |
| `HAL_I2C_Master_Receive()` | 主机接收 | `HAL_I2C_Master_Receive(&hi2c1, devAddr, buf, len, 100);` |
| `HAL_I2C_Slave_Transmit()` | 从机发送 | `HAL_I2C_Slave_Transmit(&hi2c1, buf, len, 100);` |
| `HAL_I2C_Slave_Receive()` | 从机接收 | `HAL_I2C_Slave_Receive(&hi2c1, buf, len, 100);` |
| `HAL_I2C_Mem_Write()` | 写内存(如EEPROM) | `HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, 16, buf, len, 100);` |
| `HAL_I2C_Mem_Read()` | 读内存 | `HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, 16, buf, len, 100);` |
| `HAL_I2C_IsDeviceReady()` | 检测设备是否就绪 | `HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 10, 100);` |

---

## 八、DMA常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_DMA_Start()` | 启动DMA传输 | `HAL_DMA_Start(&hdma1, src, dst, len);` |
| `HAL_DMA_Start_IT()` | 启动DMA+中断 | `HAL_DMA_Start_IT(&hdma1, src, dst, len);` |
| `HAL_DMA_Abort()` | 中止DMA传输 | `HAL_DMA_Abort(&hdma1);` |
| `HAL_DMA_GetState()` | 获取DMA状态 | `state = HAL_DMA_GetState(&hdma1);` |
| `HAL_DMA_PollForTransfer()` | 轮询DMA传输完成 | `HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, 100);` |

---

## 九、FLASH常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_FLASH_Unlock()` | 解锁FLASH | `HAL_FLASH_Unlock();` |
| `HAL_FLASH_Lock()` | 锁定FLASH | `HAL_FLASH_Lock();` |
| `HAL_FLASH_Program()` | 编程FLASH | `HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);` |
| `HAL_FLASH_Erase()` | 擦除FLASH页 | `HAL_FLASH_Erase(&eraseInit, &pageError);` |
| `HAL_FLASH_OB_Unlock()` | 解锁选项字节 | `HAL_FLASH_OB_Unlock();` |
| `HAL_FLASH_OB_Launch()` | 加载选项字节 | `HAL_FLASH_OB_Launch();` |

---

## 十、RCC时钟常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_RCC_GetHCLKFreq()` | 获取HCLK频率 | `freq = HAL_RCC_GetHCLKFreq();` |
| `HAL_RCC_GetPCLK1Freq()` | 获取PCLK1频率 | `freq = HAL_RCC_GetPCLK1Freq();` |
| `HAL_RCC_GetPCLK2Freq()` | 获取PCLK2频率 | `freq = HAL_RCC_GetPCLK2Freq();` |
| `HAL_RCC_OscConfig()` | 配置振荡器 | `HAL_RCC_OscConfig(&RCC_OscInitStruct);` |
| `HAL_RCC_ClockConfig()` | 配置时钟树 | `HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);` |
| `__HAL_RCC_GPIOA_CLK_ENABLE()` | 使能GPIOA时钟 | `__HAL_RCC_GPIOA_CLK_ENABLE();` |
| `__HAL_RCC_USART1_CLK_ENABLE()` | 使能USART1时钟 | `__HAL_RCC_USART1_CLK_ENABLE();` |

---

## 十一、WDT看门狗常用函数

| 函数 | 功能 | 示例 |
|------|------|------|
| `HAL_IWDG_Init()` | 初始化独立看门狗 | `HAL_IWDG_Init(&hiwdg);` |
| `HAL_IWDG_Start()` | 启动独立看门狗 | `HAL_IWDG_Start(&hiwdg);` |
| `HAL_IWDG_Reload()` | 喂狗 | `HAL_IWDG_Reload(&hiwdg);` |
| `HAL_WWDG_Init()` | 初始化窗口看门狗 | `HAL_WWDG_Init(&hwwdg);` |
| `HAL_WWDG_Start()` | 启动窗口看门狗 | `HAL_WWDG_Start(&hwwdg);` |
| `HAL_WWDG_Refresh()` | 喂窗口看门狗 | `HAL_WWDG_Refresh(&hwwdg);` |

---

## 十二、常用宏定义

```c
// GPIO
GPIO_PIN_SET      // 高电平
GPIO_PIN_RESET    // 低电平
GPIO_MODE_INPUT   // 输入模式
GPIO_MODE_OUTPUT_PP  // 推挽输出
GPIO_MODE_AF_PP      // 复用推挽
GPIO_MODE_ANALOG     // 模拟模式
GPIO_NOPULL          // 无上拉下拉
GPIO_PULLUP          // 上拉
GPIO_PULLDOWN        // 下拉

// 中断
ENABLE    // 使能
DISABLE   // 禁用

// 状态
HAL_OK       // 成功
HAL_ERROR    // 错误
HAL_BUSY     // 忙
HAL_TIMEOUT  // 超时
```

---

## 十三、快速参考表

| 外设 | 初始化 | 启动 | 发送/转换 | 接收/读取 | 中断方式 | DMA方式 |
|------|--------|------|----------|----------|---------|---------|
| UART | `HAL_UART_Init()` | - | `HAL_UART_Transmit()` | `HAL_UART_Receive()` | `_IT` 后缀 | `_DMA` 后缀 |
| SPI | `HAL_SPI_Init()` | - | `HAL_SPI_Transmit()` | `HAL_SPI_Receive()` | `_IT` 后缀 | `_DMA` 后缀 |
| I2C | `HAL_I2C_Init()` | - | `HAL_I2C_Master_Transmit()` | `HAL_I2C_Master_Receive()` | `_IT` 后缀 | `_DMA` 后缀 |
| ADC | `HAL_ADC_Init()` | `HAL_ADC_Start()` | - | `HAL_ADC_GetValue()` | `_IT` 后缀 | `_DMA` 后缀 |
| TIM | `HAL_TIM_Base_Init()` | `HAL_TIM_Base_Start()` | - | - | `_IT` 后缀 | - |

---

> 💡 **提示**：大多数HAL函数都有三种模式：
> - **阻塞模式**：函数名无后缀，等待操作完成
> - **中断模式**：函数名带 `_IT` 后缀，完成后调用回调函数
> - **DMA模式**：函数名带 `_DMA` 后缀，使用DMA传输

# HAL库编程实例（main函数部分）

---

## 实例1：LED闪烁（GPIO输出）

```c
int main(void)
{
    HAL_Init();                    // 初始化HAL库
    SystemClock_Config();          // 配置系统时钟
    MX_GPIO_Init();                // 初始化GPIO（CubeMX生成）
    
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // 翻转LED引脚
        HAL_Delay(500);                         // 延时500ms
    }
}
```

---

## 实例2：串口发送数据（UART轮询）

```c
int main(void)
{
    uint8_t txData[] = "Hello STM32!\r\n";
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();         // 初始化串口
    
    while(1)
    {
        HAL_UART_Transmit(&huart1, txData, sizeof(txData)-1, 100);  // 发送数据
        HAL_Delay(1000);                                            // 每秒发送一次
    }
}
```

---

## 实例3：串口接收中断（UART中断）

```c
uint8_t rxData;
uint8_t rxBuffer[10];
uint8_t rxIndex = 0;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    
    // 启动串口接收中断
    HAL_UART_Receive_IT(&huart1, &rxData, 1);
    
    while(1)
    {
        // 主循环处理其他任务
        HAL_Delay(10);
    }
}

// 回调函数（放在main.c中）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        rxBuffer[rxIndex++] = rxData;
        
        if(rxData == '\n' || rxIndex >= 10)  // 收到换行或缓冲区满
        {
            HAL_UART_Transmit(&huart1, rxBuffer, rxIndex, 100);  // 回显
            rxIndex = 0;
        }
        
        HAL_UART_Receive_IT(&huart1, &rxData, 1);  // 重新启用接收中断
    }
}
```

---

## 实例4：定时器中断（1秒定时）

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();                // 初始化定时器2
    
    // 启动定时器中断
    HAL_TIM_Base_Start_IT(&htim2);
    
    while(1)
    {
        // 主循环处理其他任务
        HAL_Delay(10);
    }
}

// 定时器更新回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // 翻转LED
    }
}
```

---

## 实例5：ADC读取电压值

```c
int main(void)
{
    uint32_t adcValue;
    float voltage;
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();                // 初始化ADC
    
    while(1)
    {
        HAL_ADC_Start(&hadc1);                          // 启动ADC转换
        HAL_ADC_PollForConversion(&hadc1, 100);         // 等待转换完成
        adcValue = HAL_ADC_GetValue(&hadc1);            // 获取转换值
        HAL_ADC_Stop(&hadc1);                           // 停止ADC
        
        // 计算电压值（假设3.3V参考，12位ADC）
        voltage = (adcValue * 3.3f) / 4095.0f;
        
        // 可通过串口发送电压值
        // printf("Voltage: %.2fV\r\n", voltage);
        
        HAL_Delay(500);
    }
}
```

---

## 实例6：按键检测（GPIO输入）

```c
int main(void)
{
    uint8_t keyState;
    uint8_t lastState = 1;
    uint32_t pressTime = 0;
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();                // 初始化GPIO（包含按键引脚）
    
    while(1)
    {
        keyState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);  // 读取按键状态
        
        // 检测按键按下（低电平有效）
        if(keyState == 0 && lastState == 1)
        {
            pressTime = HAL_GetTick();
            
            // 消抖处理
            HAL_Delay(20);
            if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
            {
                // 按键确认按下
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);   // 切换LED
            }
        }
        
        lastState = keyState;
        HAL_Delay(10);
    }
}
```

---

## 实例7：PWM输出（控制LED亮度/电机速度）

```c
int main(void)
{
    uint32_t dutyCycle = 0;
    uint8_t direction = 1;
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM3_Init();                // 初始化定时器3（PWM模式）
    
    // 启动PWM输出
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    
    while(1)
    {
        // 渐变效果
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dutyCycle);
        
        if(direction)
        {
            dutyCycle += 10;
            if(dutyCycle >= 1000) direction = 0;
        }
        else
        {
            dutyCycle -= 10;
            if(dutyCycle <= 0) direction = 1;
        }
        
        HAL_Delay(50);
    }
}
```

---

## 实例8：多任务综合示例（LED+串口+按键+ADC）

```c
uint32_t adcValue;
uint8_t keyFlag = 0;

int main(void)
{
    char buffer[50];
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    
    // 启动定时器中断（用于按键扫描）
    HAL_TIM_Base_Start_IT(&htim2);
    
    // 发送启动信息
    HAL_UART_Transmit(&huart1, (uint8_t*)"System Started\r\n", 16, 100);
    
    while(1)
    {
        // 读取ADC
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 100);
        adcValue = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        
        // 发送数据到串口
        sprintf(buffer, "ADC: %d  Key: %d\r\n", adcValue, keyFlag);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
        
        // 按键处理
        if(keyFlag)
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            keyFlag = 0;
        }
        
        HAL_Delay(500);
    }
}

// 定时器中断回调（10ms）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t lastPress = 0;
    static uint8_t keyLast = 1;
    uint8_t keyNow = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    
    if(keyNow == 0 && keyLast == 1)
    {
        if(HAL_GetTick() - lastPress > 300)  // 防抖
        {
            keyFlag = 1;
            lastPress = HAL_GetTick();
        }
    }
    keyLast = keyNow;
}
```

---

## 实例9：低功耗模式（停机模式）

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    
    // 配置唤醒引脚（如PA0）
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    
    HAL_UART_Transmit(&huart1, (uint8_t*)"Enter Stop Mode\r\n", 17, 100);
    
    while(1)
    {
        // 进入停机模式，唤醒后继续执行
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        
        // 唤醒后重新配置系统时钟
        SystemClock_Config();
        
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_UART_Transmit(&huart1, (uint8_t*)"Wakeup!\r\n", 9, 100);
        
        HAL_Delay(1000);
    }
}
```

---

## 实例10：看门狗保护

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_IWDG_Init();                // 初始化独立看门狗
    
    // 启动看门狗
    HAL_IWDG_Start(&hiwdg);
    
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(500);
        
        // 喂狗（必须在超时前执行）
        HAL_IWDG_Reload(&hiwdg);
    }
}
```

---
