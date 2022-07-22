#ifndef __LEDBLE_H__
#define __LEDBLE_H__

#include "Arduino.h"
#include "ArduinoBLE.h"

#define serviceUUID "d2b3b3dd-9dce-4af7-a01a-036bd9956beb"
#define characteristicUUID "b4ae9c49-d400-456d-89fb-1af8cb6ab15b"

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);
void setLedWrittenHandler(BLEDevice central, BLECharacteristic setLedCharacteristic);

void ledBLE_Init();








#endif
