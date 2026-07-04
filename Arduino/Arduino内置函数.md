
---

### 一、核心结构函数

这是每个 Arduino 程序必须包含的部分：

1. **`setup()`**: 程序启动时运行一次。用于初始化变量、设置引脚模式、启动库等。
2. **`loop()`**: `setup()` 结束后无限循环运行。这是程序的主体逻辑所在。

---

### 二、数字 I/O 函数 (Digital I/O)

用于控制引脚输出高/低电平或读取引脚状态。

| 函数                         | 描述                                                  | 示例                                 |
| :------------------------- | :-------------------------------------------------- | :--------------------------------- |
| `pinMode(pin, mode)`       | 设置引脚模式。`mode` 可为 `INPUT`, `OUTPUT`, `INPUT_PULLUP`。 | `pinMode(13, OUTPUT);`             |
| `digitalWrite(pin, value)` | 向数字引脚写入高(`HIGH`)或低(`LOW`)电平。                        | `digitalWrite(LED_BUILTIN, HIGH);` |
| `digitalRead(pin)`         | 读取数字引脚的状态，返回 `HIGH` 或 `LOW`。                        | `int val = digitalRead(2);`        |

---

### 三、模拟 I/O 函数 (Analog I/O)

用于读取模拟传感器数值或输出模拟信号（PWM）。

|函数|描述|示例|
|:--|:--|:--|
|`analogRead(pin)`|读取模拟引脚电压，返回 0-1023 (10位分辨率)。|`int sensor = analogRead(A0);`|
|`analogWrite(pin, value)`|向支持 PWM 的引脚写入模拟值，范围 0-255。|`analogWrite(9, 128);`|
|`analogReference(type)`|配置参考电压 (如 `DEFAULT`, `INTERNAL`, `EXTERNAL`)。|`analogReference(INTERNAL);`|
|`analogReadResolution(bits)`|(仅部分板卡如 Due, Zero) 设置读取分辨率位数。|`analogReadResolution(12);`|

---

### 四、时间函数 (Time)

用于处理延时和获取运行时间。

|函数|描述|示例|
|:--|:--|:--|
|`delay(ms)`|暂停程序执行指定的毫秒数。|`delay(1000); // 暂停1秒`|
|`delayMicroseconds(us)`|暂停程序执行指定的微秒数。|`delayMicroseconds(10);`|
|`millis()`|返回自程序启动以来经过的毫秒数 (unsigned long)。常用于非阻塞延时。|`if (millis() - lastTime > 1000) {...}`|
|`micros()`|返回自程序启动以来经过的微秒数。|`unsigned long start = micros();`|

---

### 五、数学与三角函数 (Math & Trigonometry)

Arduino 封装了标准的 C 数学库函数。

- **基础运算**: `min(a, b)`, `max(a, b)`, `abs(x)`, `constrain(x, a, b)` (限制范围), `map(value, fromLow, fromHigh, toLow, toHigh)` (数值映射)。
- **三角函数**: `sin(rad)`, `cos(rad)`, `tan(rad)` (参数为弧度)。
- **反三角**: `asin(x)`, `acos(x)`, `atan(x)`, `atan2(y, x)`。
- **其他**: `sqrt(x)` (平方根), `pow(base, exponent)` (幂运算), `exp(x)` (e的x次方), `log(x)` (自然对数), `log10(x)`。
- **随机数**: `randomSeed(seed)` (设置种子), `random(max)` 或 `random(min, max)`。

---

### 六、位运算函数 (Bitwise Operations)

用于直接操作二进制位，常用于底层硬件控制。

- `lowByte(w)`: 提取一个字的低8位。
- `highByte(w)`: 提取一个字的高8位。
- `bitRead(value, bit)`: 读取特定位。
- `bitWrite(value, bit, bitvalue)`: 写入特定位。
- `bitSet(value, bit)`: 将特定位设为1。
- `bitClear(value, bit)`: 将特定位清零。
- `bit(bitNum)`: 生成一个只有第 `bitNum` 位为1的数 (即 $2^{bitNum}$)。

---

### 七、常用标准库及其函数（详解与实例）
以下是针对 **Wire (I2C)**、**SPI**、**UART (Serial)** 和 **Servo** 四个核心库的深度解析，严格按照 **用途**、**函数详解**、**实例** 的结构整理。

---

#### 1. `Wire` 库 (I2C 通信)

 **用途**

用于 **I2C (Inter-Integrated Circuit)** 总线通信。

- **特点**：只需两根线（SDA 数据，SCL 时钟），支持多主多从，适合连接大量低速传感器（如 MPU6050、OLED 屏幕、RTC 时钟、EEPROM）。
- **速度**：标准模式 100kHz，快速模式 400kHz。
- **接线**：Arduino SDA ↔ 设备 SDA，Arduino SCL ↔ 设备 SCL（通常需外接 4.7kΩ 上拉电阻，开发板通常内置）。

 **函数详解**

|函数|描述|参数/返回值|
|:--|:--|:--|
|`Wire.begin()`|初始化 I2C 总线（作为主机）。|无参数；若作为从机需传入地址 `Wire.begin(addr)`。|
|`Wire.beginTransmission(addr)`|开始向指定地址的从机传输数据。|`addr`: 从机 7 位地址 (0-127)。|
|`Wire.write(val)`|将数据写入传输缓冲区。|`val`: 字节或字符串；返回写入字节数。可连续调用。|
|`Wire.endTransmission()`|结束传输并发送数据。|返回 0 (成功), 1 (数据太长), 2 (地址无响应) 等。|
|`Wire.requestFrom(addr, qty)`|向从机请求读取指定数量的字节。|`qty`: 请求字节数；返回实际读取到的字节数。|
|`Wire.available()`|检查接收缓冲区中是否有可读数据。|返回可用字节数。|
|`Wire.read()`|从缓冲区读取一个字节。|返回读取的字节，若无数据返回 -1。|
|`Wire.setClock(freq)`|设置 I2C 时钟频率。|`freq`: 如 100000L (100kHz), 400000L (400kHz)。|

 **实例：读取 MPU6050 加速度计数据**

MPU6050 是典型的 I2C 设备，地址通常为 `0x68`。

```cpp
#include <Wire.h>

const int MPU_ADDR = 0x68; // I2C 地址
int16_t ax, ay, az;        // 原始数据变量

void setup() {
  Serial.begin(9600);
  Wire.begin();            // 初始化 I2C
  
  // 唤醒 MPU6050 (默认睡眠)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);        // 电源管理寄存器
  Wire.write(0);           // 写入 0 唤醒
  Wire.endTransmission();
}

void loop() {
  // 1. 告诉设备我们要从 0x3B (加速度 X 高位) 开始读
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false); // false: 不释放总线，紧接着读取

  // 2. 请求读取 6 个字节 (X,Y,Z 各 2 字节)
  Wire.requestFrom(MPU_ADDR, 6, true);

  if (Wire.available() == 6) {
    // 读取并组合高低位 (高位 << 8 | 低位)
    ax = Wire.read() << 8 | Wire.read();
    ay = Wire.read() << 8 | Wire.read();
    az = Wire.read() << 8 | Wire.read();

    Serial.print("Accel X: "); Serial.print(ax);
    Serial.print(" | Y: "); Serial.print(ay);
    Serial.print(" | Z: "); Serial.println(az);
  } else {
    Serial.println("I2C Error: No data received");
  }

  delay(500);
}
```

---

#### 2. `SPI` 库 (SPI 通信)

 **用途**

用于 **SPI (Serial Peripheral Interface)** 总线通信。

- **特点**：四线制（MOSI, MISO, SCK, SS/CS），全双工，高速同步通信。适合大数据量或高速设备（如 SD 卡、TFT 屏幕、RFID、NRF24L01）。
- **速度**：可达几 MHz 甚至更高。
- **接线**：
    - MOSI (Master Out Slave In)
    - MISO (Master In Slave Out)
    - SCK (Clock)
    - SS/CS (片选，每个设备一根，低电平有效)

 **函数详解**

|函数|描述|参数/返回值|
|:--|:--|:--|
|`SPI.begin()`|初始化 SPI 总线（配置 SCK, MOSI, MISO 为输出/输入）。|无参数。|
|`SPI.beginTransaction(settings)`|开始事务，锁定 SPI 配置以防冲突。|`settings`: `SPISettings(speed, bitOrder, mode)`。|
|`SPI.transfer(val)`|发送一个字节并同时接收一个字节。|`val`: 发送的数据；返回接收到的数据。|
|`SPI.endTransaction()`|结束事务，恢复之前的 SPI 设置。|无参数。|
|`SPI.end()`|禁用 SPI 接口，释放引脚。|无参数。|
|`SPISettings(...)`|配置对象：速度(Hz), 位序(MSBFIRST/LSBFIRST), 模式(SPI_MODE0-3)。|用于 `beginTransaction`。|

 **实例：模拟读取 SPI 设备 ID**

假设有一个 SPI 闪存芯片，发送 `0x9F` 命令可读取 ID。

```cpp
#include <SPI.h>

const int CS_PIN = 10; // 片选引脚 (根据硬件连接修改)

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // 初始状态：取消选中 (高电平)
  SPI.begin();                // 初始化 SPI
}

void loop() {
  // 配置：4MHz, MSB 优先, Mode 0 (最常见)
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  digitalWrite(CS_PIN, LOW); // 1. 选中设备 (拉低片选)

  // 2. 发送读取 ID 命令 (0x9F)
  byte response = SPI.transfer(0x9F); 
  // 注意：SPI 是全双工，发送命令的同时也会读回一个无用字节，通常忽略

  // 3. 继续读取 3 个字节的制造商/设备 ID
  byte id1 = SPI.transfer(0x00); // 发送 0x00 占位以获取时钟从而读出数据
  byte id2 = SPI.transfer(0x00);
  byte id3 = SPI.transfer(0x00);

  digitalWrite(CS_PIN, HIGH); // 4. 取消选中 (拉高片选)
  SPI.endTransaction();       // 5. 结束事务

  Serial.print("Device ID: ");
  Serial.print(id1, HEX); Serial.print("-");
  Serial.print(id2, HEX); Serial.print("-");
  Serial.println(id3, HEX);

  delay(2000);
}
```

---

#### 3. `Serial` 库 (UART 通信)

 **用途**

用于 **UART (Universal Asynchronous Receiver/Transmitter)** 串口通信。

- **特点**：异步通信，仅需 TX (发送) 和 RX (接收) 两根线。主要用于与电脑调试、蓝牙模块、GPS 模块或其他单片机通信。
- **注意**：Arduino Uno 的硬件串口是引脚 0(RX) 和 1(TX)，烧录程序时需断开外接设备。其他引脚可使用 `SoftwareSerial` 库模拟。

 **函数详解**

|函数|描述|参数/返回值|
|:--|:--|:--|
|`Serial.begin(baud)`|设置波特率 (如 9600, 115200)。|`baud`: 波特率。|
|`Serial.print(val)`|打印数据，不换行。|自动处理 int, float, String 等。|
|`Serial.println(val)`|打印数据并换行。|同上。|
|`Serial.read()`|读取接收缓冲区的一个字节。|返回字节，若无数据返回 -1。|
|`Serial.available()`|检查接收缓冲区有多少字节可读。|返回字节数 (>0 表示有数据)。|
|`Serial.write(val)`|以原始字节形式发送数据。|常用于发送二进制数据。|
|`Serial.flush()`|等待发送缓冲区清空 (旧版本含义不同，新版指等待发送完成)。|无参数。|

 **实例：串口回声与指令控制 LED**

接收电脑发送的字符，如果是 '1' 开灯，'0' 关灯，其他字符原样返回。

```cpp
const int LED_PIN = 13;
char incomingByte = 0;

void setup() {
  Serial.begin(9600); // 必须与串口监视器波特率一致
  pinMode(LED_PIN, OUTPUT);
  Serial.println("System Ready. Send '1' for ON, '0' for OFF.");
}

void loop() {
  // 检查是否有数据到来
  if (Serial.available() > 0) {
    // 读取一个字节
    incomingByte = Serial.read();
    
    Serial.print("Received: ");
    Serial.println(incomingByte); // 打印收到的字符

    if (incomingByte == '1') {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED turned ON");
    } 
    else if (incomingByte == '0') {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED turned OFF");
    } 
    else {
      Serial.println("Unknown command");
    }
  }
}
```

---

#### 4. `Servo` 库 (舵机控制)

 **用途**

专门用于控制 **舵机 (Servo Motor)**。

- **特点**：舵机通过 PWM 脉冲宽度控制角度（通常 544us=0°, 2400us=180°）。该库自动处理复杂的时序，用户只需调用角度函数。
- **限制**：不同板卡支持的舵机数量不同（Uno 通常支持 12 个），且会占用定时器资源（可能影响 `analogWrite` 在某些引脚的使用）。
- **供电**：舵机电流较大，建议单独供电，共地即可。

 **函数详解**

|函数|描述|参数/返回值|
|:--|:--|:--|
|`Servo.attach(pin)`|将舵机对象绑定到引脚。|`pin`: 数字引脚号。|
|`Servo.write(angle)`|设置舵机角度。|`angle`: 0-180 (度)。|
|`Servo.writeMicroseconds(us)`|直接设置脉冲宽度。|`us`: 微秒数 (通常 544-2400)。精度更高。|
|`Servo.read()`|读取当前设定的角度值。|返回上次 `write` 的角度 (非实际物理位置)。|
|`Servo.attached()`|检查是否已绑定引脚。|返回 true/false。|
|`Servo.detach()`|解除绑定，释放引脚供他用。|无参数。|

 **实例：电位器控制舵机角度**

使用模拟引脚读取电位器值 (0-1023)，映射到舵机角度 (0-180)。

```cpp
#include <Servo.h>

Servo myServo;       // 创建舵机对象
const int potPin = A0; // 电位器连接 A0
const int servoPin = 9; // 舵机信号线连接 D9

void setup() {
  myServo.attach(servoPin); // 绑定引脚
  Serial.begin(9600);
}

void loop() {
  // 1. 读取电位器值 (0 - 1023)
  int sensorValue = analogRead(potPin);
  
  // 2. 映射到舵机角度 (0 - 180)
  int angle = map(sensorValue, 0, 1023, 0, 180);
  
  // 3. 控制舵机转动
  myServo.write(angle);
  
  // 4. 调试输出
  Serial.print("Potentiometer: ");
  Serial.print(sensorValue);
  Serial.print(" -> Angle: ");
  Serial.println(angle);
  
  delay(15); // 给舵机一点反应时间，避免抖动
}
```
---
#### 5. `DHT` 库 (温湿度传感器)

**用途**：用于读取 DHT 系列温湿度传感器（如 DHT11、DHT22）的数据。

**核心函数详解**：

- `DHT.readTemperature()`: 读取温度（摄氏度）。
- `DHT.readHumidity()`: 读取湿度。
- `DHT.temperature()`: 获取上次读取的温度。
- `DHT.humidity()`: 获取上次读取的湿度。

**实例：显示温湿度数据**

```cpp
#include <DHT.h>

#define DHTPIN 2          // 连接 DHT 传感器的引脚
#define DHTTYPE DHT11     // 传感器类型

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // 读取温湿度
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // 检查读取是否成功
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // 打印结果
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  
  delay(2000); // 每2秒读取一次
}
```
---
#### 6. `WiFi` 库 (Wi-Fi 通信)

**用途**：用于支持 Wi-Fi 连接的 Arduino 板（如 ESP8266、ESP32）连接 Wi-Fi 网络，实现远程控制、数据传输等物联网功能。

**核心函数详解**：

- `WiFi.begin(ssid, password)`: 连接指定 Wi-Fi 网络。
- `WiFi.status()`: 返回连接状态，`WL_CONNECTED` 表示连接成功。
- `WiFi.localIP()`: 获取设备的本地 IP 地址。
- `WiFi.SSID()`: 获取连接的 Wi-Fi 网络名称。
- `WiFi.RSSI()`: 获取 Wi-Fi 信号强度（RSSI）。
- `WiFi.disconnect()`: 断开 Wi-Fi 连接。

**实例：连接 Wi-Fi 并通过 HTTP 获取天气数据**

```cpp
#include <WiFi.h>

const char* ssid = "Your_SSID";
const char* password = "Your_Password";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 连接到 OpenWeatherMap API 获取天气
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect("api.openweathermap.org", 80)) {
      client.println("GET /data/2.5/weather?q=Beijing&appid=YOUR_API_KEY HTTP/1.1");
      client.println("Host: api.openweathermap.org");
      client.println("Connection: close");
      client.println();
      
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      
      // 读取响应数据
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      
      client.stop();
      Serial.println("Connection closed");
    }
  }
  
  delay(10000); // 每10秒获取一次天气
}
```

#### 7. `Ethernet` 库 (以太网通信)

**用途**：用于支持以太网的 Arduino 板（如 Arduino Ethernet Shield）连接到局域网，实现网络通信功能。

**核心函数详解**：

- `Ethernet.begin(mac)`: 初始化以太网连接，`mac` 是 MAC 地址。
- `Ethernet.begin(mac, ip)`: 指定 IP 地址。
- `Ethernet.localIP()`: 获取设备的本地 IP 地址。
- `Ethernet.status()`: 返回以太网连接状态。
- `EthernetServer server(port)`: 创建服务器对象。

**实例：创建简单 HTTP 服务器**

```cpp
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

EthernetServer server(80);

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client");
    
    // 读取 HTTP 请求
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '\n') {
          // HTTP 请求结束，发送响应
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>Arduino Ethernet Server</title></head>");
          client.println("<body><h1>Hello from Arduino!</h1></body>");
          client.println("</html>");
          break;
        }
      }
    }
    
    // 关闭连接
    client.stop();
    Serial.println("Client disconnected");
  }
}
```
---
#### 8. `Blynk` 库 (物联网平台)

**用途**：用于与 Blynk IoT 平台通信，实现手机 APP 控制 Arduino 设备。

**核心函数详解**：

- `Blynk.begin(authToken, ssid, password)`: 连接 Blynk 服务器。
- `Blynk.run()`: 处理 Blynk 通信。
- `Blynk.virtualWrite(vPin, value)`: 发送虚拟引脚数据。
- `Blynk.connected()`: 检查是否连接到 Blynk。

**实例：通过 Blynk APP 控制 LED**

```cpp
#include <BlynkSimpleEsp8266.h>

// Blynk 服务器认证令牌
char auth[] = "YourAuthToken";

// Wi-Fi 信息
char ssid[] = "Your_SSID";
char pass[] = "Your_Password";

// 定义虚拟引脚
#define VIRTUAL_PIN V0

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  // 检查虚拟引脚状态
  if (Blynk.connected() && Blynk.virtualRead(VIRTUAL_PIN) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
```
---

#### 9. `PubSubClient` 库 (MQTT 协议)

**用途**：用于实现 MQTT 协议，适用于物联网项目，实现设备间通信。

**核心函数详解**：

- `client.connect("client_id")`: 连接到 MQTT 服务器。
- `client.publish(topic, payload)`: 发布消息。
- `client.subscribe(topic)`: 订阅主题。
- `client.loop()`: 处理 MQTT 通信。

**实例：MQTT 发布/订阅温度数据**

```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Your_SSID";
const char* password = "Your_Password";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // 发布温度数据
  float temperature = 25.5; // 模拟温度
  char tempStr[10];
  sprintf(tempStr, "%.2f", temperature);
  
  client.publish("sensor/temperature", tempStr);
  Serial.print("Published temperature: ");
  Serial.println(tempStr);
  
  delay(5000); // 每5秒发布一次
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");
      client.subscribe("sensor/temperature");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
```
---
#### 10. `FastLED` 库 (LED 灯带控制)

**用途**：用于控制各种 LED 灯带（如 WS2812、APA102），支持复杂灯光效果。

**核心函数详解**：

- `FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)`: 添加 LED 灯带。
- `FastLED.show()`: 更新 LED 显示。
- `leds[i].setRGB(r, g, b)`: 设置指定 LED 的颜色。
- `leds[i].setHue(hue)`: 设置指定 LED 的色相。

**实例：彩虹流动效果**

```cpp
#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    30
#define BRIGHTNESS  64
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint8_t hue = 0;
  
  // 每个 LED 有不同颜色
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + i * 2, 255, 255);
  }
  
  FastLED.show();
  hue++;
  delay(1000 / FRAMES_PER_SECOND);
}
```
---
#### 11. `RTClib` 库 (实时时钟)

**用途**：用于与 RTC（实时时钟）模块（如 DS1307、DS3231）通信，获取准确时间。

**核心函数详解**：

- `RTC.now()`: 获取当前时间。
- `RTC.adjust(DateTime)`: 设置时间。
- `DateTime.now()`: 获取当前时间。
- `DateTime.year()`: 获取年份。
- `DateTime.month()`: 获取月份。
- `DateTime.day()`: 获取日期。
- `DateTime.hour()`: 获取小时。
- `DateTime.minute()`: 获取分钟。
- `DateTime.second()`: 获取秒。

**实例：显示实时时间**

```cpp
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // 设置时间（年、月、日、时、分、秒）
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();
  
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(now.dayOfTheWeek(), DEC);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  delay(1000);
}
```
---

#### 12. `PS2X_lib` 库 (PS2 手柄控制)

**用途**：用于控制 PS2 手柄，适用于机器人、游戏控制器等项目。

**核心函数详解**：

- `config_gamepad(clk, cmd, att, dat, pressures, rumble)`: 配置手柄连接。
- `read_gamepad(motor1, motor2)`: 读取手柄输入并设置震动。
- `Button(button)`: 检查指定按键是否按下。
- `NewButtonState(button)`: 检查指定按键状态是否变化。

**实例：PS2 手柄控制 LED 亮度**

```cpp
#include <PS2X_lib.h>

PS2X ps2x; // 创建 PS2X 对象

// 手柄连接引脚
#define PS2_CLK  2
#define PS2_CMD  3
#define PS2_SEL  4
#define PS2_DAT  5

void setup() {
  Serial.begin(9600);
  
  // 初始化 PS2 手柄
  if (ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true) == 0) {
    Serial.println("Found Controller, configured successfully");
  } else {
    Serial.println("No controller found, check wiring");
    while (1);
  }
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // 读取手柄状态
  ps2x.read_gamepad(false, 0);
  
  // 检查按键状态
  if (ps2x.Button(PS2_SELECT)) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  // 读取左摇杆 Y 轴，控制 LED 亮度
  int leftY = ps2x.Analog(PS2_LY);
  int ledBrightness = map(leftY, 0, 255, 0, 255);
  analogWrite(LED_BUILTIN, ledBrightness);
  
  delay(20);
}
```

---

### 八、非阻塞编程原理深度解析

在 Arduino 编程中，**"阻塞"**是导致程序响应迟钝、无法多任务并行的罪魁祸首。

#### 1. 什么是阻塞？

当你调用 `delay(1000)` 时，CPU 会停下手头所有工作，傻等 1000 毫秒。在这 1 秒内：

- 按钮按了没反应。
- 传感器数据没更新。
- 串口数据发不出去。
- LED 灯不会闪。 这就是**阻塞**。程序像一条单行道，必须走完这一步才能走下一步。

#### 2. 非阻塞的核心思想：**"查表法" + "状态机"**

非阻塞编程不等待时间流逝，而是**记录时间点**，并在每次循环中**检查时间差**。

- **比喻**：
    - **阻塞 (`delay`)**：你要煮泡面，设定闹钟 3 分钟。这 3 分钟你盯着闹钟看，什么都不干，直到闹钟响。
    - **非阻塞 (`millis`)**：你看一眼手表记下开始时间 (比如 12:00:00)。然后你去洗菜、切肉、看书。每隔一会儿你看一眼手表，如果当前时间 - 开始时间 > 3 分钟，你就去吃面。如果没到，就继续干别的事。

#### 3. 实现步骤

1. **定义变量**：需要一个 `unsigned long` 类型的变量来存储上一次动作发生的时间戳。
2. **获取当前时间**：使用 `millis()` 获取自开机以来的毫秒数。
3. **计算时间差**：`当前时间 - 上次时间 >= 间隔时间`。
4. **执行动作并更新时间**：如果条件满足，执行任务，并将 `上次时间` 更新为 `当前时间`。

#### 4. 经典代码模板 (Blink without Delay)

这是 Arduino 官方示例中最经典的非阻塞写法：

```cpp
const int ledPin = LED_BUILTIN; 
int ledState = LOW;             // LED 当前状态

unsigned long previousMillis = 0; // 上次闪烁的时间点
const long interval = 1000;       // 闪烁间隔 1 秒

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 1. 获取当前时间 (关键：不要用 int，要用 unsigned long，防止溢出)
  unsigned long currentMillis = millis();

  // 2. 检查时间差
  // 注意：即使 millis() 溢出 (约 50 天后归零)，减法运算在 unsigned 类型下依然正确
  if (currentMillis - previousMillis >= interval) {
    
    // 3. 保存当前时间为"上次时间"
    previousMillis = currentMillis;

    // 4. 执行任务 (切换 LED 状态)
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }

  // 5. 这里可以放其他代码，它们不会被上面的逻辑阻塞
  // 例如：读取传感器、响应按键、发送串口数据
  readSensors(); 
  checkButtons();
}

void readSensors() {
  // 模拟耗时操作，但在非阻塞架构下，它不会耽误 LED 闪烁
  // ...
}

void checkButtons() {
  // 模拟按键检测
  // ...
}
```

#### 5. 进阶：有限状态机 (FSM)

对于更复杂的逻辑（如：按一下亮灯，再按一下灭灯，长按呼吸灯），单纯的时间判断不够用，需要结合**状态机**。

- **状态 (State)**：程序当前所处的模式 (如 `STATE_IDLE`, `STATE_BLINKING`, `STATE_ERROR`)。
- **转换 (Transition)**：在什么条件下从一个状态跳到另一个状态 (如：时间到了、按钮按下)。

**简单状态机结构**：

```cpp
enum State { IDLE, RUNNING, STOPPED };
State currentState = IDLE;

void loop() {
  switch (currentState) {
    case IDLE:
      if (buttonPressed()) {
        currentState = RUNNING;
        startTime = millis();
      }
      break;
      
    case RUNNING:
      if (millis() - startTime > 5000) {
        currentState = STOPPED;
      }
      // 执行运行中的任务
      break;
      
    case STOPPED:
      if (resetButton()) {
        currentState = IDLE;
      }
      break;
  }
}
```

**总结**：非阻塞编程是 Arduino 进阶的必经之路。它让单片机从"单线程死等"变成了"多线程并发"（虽然是伪并发），极大地提升了系统的实时性和交互体验。