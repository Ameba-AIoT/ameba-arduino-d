
#include "BLEDevice.h"
#include "BLEBatteryService.h"
#include "BLEAdvert.h"
#include "BLEUUID.h"

BLEBatteryService battery;
BLEAdvertData advdata;
BLEAdvertData scandata;
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

    advdata.addFlags(GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);
    advdata.addCompleteName("AMEBA_BLE_DEV");
    scandata.addCompleteServices(BLEUUID("180F"));

    BLE.init();
    BLE.configAdvert()->setAdvData(advdata);
    BLE.configAdvert()->setScanRspData(scandata);

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
