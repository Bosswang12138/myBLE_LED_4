#include "ledBLE.h"

const uint8_t *characteristicValue;
std::string ledCommand = "Voltage Cycle Duration Lastingtime";
bool deviceConnected = false;    //本次连接状态
bool oldDeviceConnected = false; //上次连接状态
bool newReceived = false;
uint8_t receivedMessage[50];

void ledBLE_Init()
{
    // 创建service和characteristic
    BLEService bleLedService(serviceUUID); // Create Bluetooth® Low Energy LED Service
    BLECharacteristic setLedCharacteristic(characteristicUUID, BLERead | BLEWrite, 200);

    if (!BLE.begin())
    {
        Serial.println("starting Bluetooth® Low Energy module failed!");

        while (1)
            ;
    }

    BLE.setLocalName("MICE_LED");

    BLE.setAdvertisedService(bleLedService);
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    bleLedService.addCharacteristic(setLedCharacteristic);
    setLedCharacteristic.setEventHandler(BLEWritten, setLedWrittenHandler);
    BLE.addService(bleLedService); //设置好了characteristic再addService，否则Service里无Characteristic
    setLedCharacteristic.writeValue(ledCommand.data());

    Serial.print("BLECharacteristic.value: ");
    characteristicValue = setLedCharacteristic.value();
    int length = setLedCharacteristic.valueLength();
    char *value = (char *)characteristicValue;
    value[length] = '\0';
    Serial.println(value);
    Serial.print("value length: ");
    Serial.println(length);

    BLE.advertise();
}

void blePeripheralConnectHandler(BLEDevice central)
{
  // central connected event handler
  Serial.print("连接成功, central地址: ");
  Serial.println(central.address());
  deviceConnected = true;
}

void blePeripheralDisconnectHandler(BLEDevice central)
{
  // central disconnected event handler
  Serial.print("连接断开, central地址: ");
  Serial.println(central.address());
  deviceConnected = false;
}

void setLedWrittenHandler(BLEDevice central, BLECharacteristic setLedCharacteristic)
{
  Serial.print("Characteristic event, written: ");
  extern uint8_t receivedMessage[50];
  int num_read; // number of bytes read
  num_read = setLedCharacteristic.readValue(receivedMessage, 50);
  receivedMessage[num_read] = '\0'; //截断字符数组，否组可能输出乱码
  Serial.println((const char *)receivedMessage);
  Serial.print("received bytes number: ");
  Serial.println(num_read);
  extern bool newReceived;
  newReceived = true;

  
}