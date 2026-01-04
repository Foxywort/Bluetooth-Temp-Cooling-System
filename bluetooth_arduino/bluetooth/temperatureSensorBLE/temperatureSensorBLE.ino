#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// 定义 DS18B20 数据引脚
#define ONE_WIRE_BUS 2


// D10 连接 HC05 的TXD
// D11 连接 HC05 的RXD
SoftwareSerial BTSerial(10, 11);

// 初始化 OneWire 和 Sensors 对象
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {

  Serial.begin(9600);
  Serial.println("System Initializing...");
  BTSerial.begin(9600);
  sensors.begin();
  Serial.println("DS18B20 & HC-05 Ready.");
}

void loop() {
  sensors.requestTemperatures();

  // 获取摄氏度
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {
    // --- 通过 USB 串口打印 (调试用) ---
    Serial.print("Current Temp: ");
    Serial.print(tempC);
    Serial.println(" ℃");

    // --- 通过 蓝牙 串口发送 (上位机用) ---
    // 格式例如: "T:25.50\n"
    BTSerial.print("T:");
    BTSerial.print(tempC);
    BTSerial.print(" ℃\n");  // 发送换行符作为结束标志
  } else {
    Serial.println("Error: Could not read temperature data");
    BTSerial.println("Error: Sensor Error");
  }

  // 延时 1 秒
  delay(1000);
}
