
#include "BLEDevice.h"
#include "BLEBatteryService.h"

BLEBatteryService battery;
uint8_t battlevel = 0;

void readCB () {
    Serial.println("Battery value read");
    battery.setLevel(90);
}

void notifCB (uint8_t state) {
    if (state == 1) {
        Serial.println("Notification Enabled");
    } else if (state == 2) {
        Serial.println("Notification Disabled");
    }
}

void setup() {
    Serial.begin(115200);

    BLE.init();
    BLE.configAdvert()->setAdvType(GAP_ADTYPE_ADV_IND);
    BLE.configAdvert()->setAdvData();
    BLE.configAdvert()->setScanRspData();

    BLE.configServer(1);
    battery.setReadCallback(readCB);
    battery.setNotificationCallback(notifCB);
    battery.addService();

    BLE.beginPeripheral();
}

void loop() {
    battlevel = (battlevel + 1)%100;
    if (battery.notifyEnabled()) {
        battery.sendNotification(battlevel);
    } else {
        battery.setLevel(battlevel);
    }
    Serial.println(battlevel);
    delay(1000);
}
