#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "ledBLE.h"

extern bool deviceConnected, oldDeviceConnected, newReceived;
extern uint8_t receivedMessage[50];
std::string receivedCommand;
Adafruit_MCP4725 dac;
void setup()
{
  // put your setup code here, to run once:
  // LED初始化
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, HIGH);

  // 串口初始化
  Serial.begin(9600);
  while(!Serial);
  Serial.println("串口打开");

  dac.begin(0x60);
  dac.setVoltage(0, false);

  ledBLE_Init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  BLE.poll();
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500); // 留时间给蓝牙缓冲
    BLE.advertise();
    Serial.println(" 开始广播 ");
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
  if (newReceived)
  {
    receivedCommand = (char *)receivedMessage;
    std::string amp = receivedCommand.substr(0, 2);
    std::string cycle_str = receivedCommand.substr(3, 2);
    std::string dura_str = receivedCommand.substr(6, 2);
    std::string totalCycle_str = receivedCommand.substr(9, 2);

    int ledAmplitude = stoi(amp, 0, 16);
    int cycle = stoi(cycle_str, 0, 16);
    int duration = stoi(dura_str, 0, 16);
    int totalCycle = stoi(totalCycle_str, 0, 16);
    int offTime = cycle - duration;

    Serial.print("ledAmplitude");
    Serial.println(ledAmplitude);
    Serial.print("cycle");
    Serial.println(cycle);
    Serial.print("duration");
    Serial.println(duration);
    Serial.print("totalCycle");
    Serial.println(totalCycle);
    Serial.print("offTime");
    Serial.println(offTime);

    while (totalCycle)
    {
      // Serial.print("on line 129...the totalCycle is: ");
      // Serial.println(totalCycle);
      // analogWrite(LEDB, ledAmplitude);
      dac.setVoltage(3000, false);
      // Serial.print("剩余几次闪烁：");
      // Serial.println(totalCycle);
      delay(duration);
      // Serial.println("on line 135...delay successed ");
      // analogWrite(LEDB, 255);
      dac.setVoltage(0, false);
      // Serial.println("on line 137...led set off ");
      delay(offTime);
      // Serial.println("on line 139...second delay finished");
      totalCycle--;
      // Serial.print("on line 140...the totalCycle is: ");
      // Serial.println(totalCycle);
    }
    newReceived = false;
  }
}