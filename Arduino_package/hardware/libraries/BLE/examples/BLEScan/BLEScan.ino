
#include "BLEDevice.h"

int dataCount = 0;
BLEAdvertData foundDevice;

void scanFunction(T_LE_CB_DATA* p_data) {
    Serial.print("Scan Data ");
    Serial.println(++dataCount);
    BLE.configScan()->printScanInfo(p_data);

    foundDevice.parseScanInfo(p_data);
    if (foundDevice.hasName()) {
        if (foundDevice.getName() == String("AMEBA_BLE_DEV")) {
            Serial.print("Found Ameba BLE Device at address ");
            Serial.println(foundDevice.getAddr().str());
        }
    }
    uint8_t serviceCount = foundDevice.getServiceCount();
    if (serviceCount > 0) {
    BLEUUID* serviceList = foundDevice.getServiceList();
        for (uint8_t i = 0; i < serviceCount; i++) {
            if (serviceList[i] == BLEUUID("180F")) {
            Serial.print("Found Battery Service at address ");
            Serial.println(foundDevice.getAddr().str());
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    BLE.init();
    BLE.configScan()->setScanMode(GAP_SCAN_MODE_ACTIVE);    // Active mode requests for scan response packets
    BLE.configScan()->setScanInterval(500);   // Start a scan every 500ms
    BLE.configScan()->setScanWindow(250);     // Each scan lasts for 250ms
    BLE.configScan()->updateScanParams();
    // Provide a callback function to process scan data.
    // If no function is provided, default BLEScan::printScanInfo is used
    BLE.setScanCallback(scanFunction);
    BLE.beginCentral(0);

    BLE.configScan()->startScan(5000);    // Repeat scans for 5 seconds, then stop
}

void loop() {
    delay(1000);
}
