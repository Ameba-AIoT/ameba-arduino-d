
#include "BLEDevice.h"
#include "BLEWifiConfigService.h"

BLEWifiConfigService configService;

void setup() {
    Serial.begin(115200);

    BLE.init();
    BLE.configServer(1);
    configService.addService();
    configService.begin();

    // Wifi config service requires a specific advertisement format to be recognised by the app
    // The advertisement needs the local BT address, which can only be obtained after starting peripheral mode
    // Thus, we stop advertising to update the advert data, wait for advertising to stop, then restart advertising with new data
    BLE.beginPeripheral();
    BLE.configAdvert()->stopAdv();
    BLE.configAdvert()->setAdvData(configService.advData());
    BLE.configAdvert()->updateAdvertParams();
    delay(100);
    BLE.configAdvert()->startAdv();

}

void loop() {
    delay(1000);
}
