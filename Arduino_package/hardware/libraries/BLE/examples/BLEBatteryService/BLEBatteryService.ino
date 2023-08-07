/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ble-battery/
 */

#include "BLEDevice.h"

#define BATT_SERVICE_UUID   "180F"
#define BATT_LEVEL_UUID     "2A19"

BLEService battService(BATT_SERVICE_UUID);
BLECharacteristic battChar(BATT_LEVEL_UUID);
BLEAdvertData advdata;
BLEAdvertData scandata;
uint8_t battlevel = 0;
bool notify = false;

void readCB (BLECharacteristic* chr, uint8_t connID) {
    //printf("Characteristic %s read by connection %d \n", chr->getUUID().str(), connID);
    Serial.print("Characteristic ");
    Serial.print(chr->getUUID().str());
    Serial.print(" read by connection ");
    Serial.println(connID);
    chr->writeData8(90);
}

void notifCB (BLECharacteristic* chr, uint8_t connID, uint16_t cccd) {
    if (cccd & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
        //printf("Notifications enabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        Serial.print("Notifications enabled on Characteristic");
        notify = true;
    } else {
        //printf("Notifications disabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        Serial.print("Notifications disabled on Characteristic");
        notify = false;
    }
    Serial.print(chr->getUUID().str());
    Serial.print(" for connection");
    Serial.println(connID);
}

void setup() {
    Serial.begin(115200);

    advdata.addFlags();
    advdata.addCompleteName("AMEBA_BLE_DEV");
    scandata.addCompleteServices(BLEUUID("180F"));

    battChar.setReadProperty(true);
    battChar.setReadPermissions(GATT_PERM_READ);
    battChar.setReadCallback(readCB);
    battChar.setNotifyProperty(true);
    battChar.setCCCDCallback(notifCB);

    battService.addCharacteristic(battChar);

    BLE.init();
    BLE.configAdvert()->setAdvData(advdata);
    BLE.configAdvert()->setScanRspData(scandata);
    BLE.configServer(1);
    BLE.addService(battService);

    BLE.beginPeripheral();
}

void loop() {
    battlevel = (battlevel + 1) % 100;
    battChar.writeData8(battlevel);
    if (BLE.connected(0) && notify) {
        battChar.notify(0);
    }
    Serial.println(battlevel);
    delay(1000);
}
