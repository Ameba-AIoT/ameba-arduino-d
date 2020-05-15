
#include "BLEDevice.h"
#include "BLEScan.h"

int dataCount = 0;

void scanFunction(T_LE_CB_DATA* p_data) {
    printf("\r\nScan Data %d\r\n", ++dataCount);
    BLE.configScan()->printScanInfo(p_data);
}

void setup() {
    BLE.init();
    BLE.configScan()->setScanMode(GAP_SCAN_MODE_ACTIVE);
    BLE.configScan()->setScanInterval(500);   // Start a scan every 500ms
    BLE.configScan()->setScanWindow(250);     // Each scan lasts for 250ms
    // Provide a callback function to process scan data.
    // If no function is provided, default BLEScan::printScanInfo is used
    BLE.setScanCallback(scanFunction);
    BLE.beginCentral(0);

    BLE.configScan()->startScan(5000);    // Repeat scans for 5 seconds, then stop
}

void loop() {

}
