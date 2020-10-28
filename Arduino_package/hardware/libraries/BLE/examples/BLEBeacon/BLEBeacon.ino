
#include "BLEDevice.h"
#include "BLEBeacon.h"

iBeacon beacon;
//altBeacon beacon;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define UUID "00112233-4455-6677-8899-AABBCCDDEEFF"


void setup() {
    // For all possible BLE manufacturer IDs, refer to:
    // www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
    beacon.setManufacturerId(0x004C); // MfgId (0x004C: Apple Inc)
    beacon.setRSSI(0xBF);             // rssi: (0xBF: -65 dBm)
    beacon.setMajor(0x007B);          // 123
    beacon.setMinor(0x01C8);          // 456
    beacon.setUUID(UUID);

    BLE.init();
    BLE.configAdvert()->setAdvType(GAP_ADTYPE_ADV_NONCONN_IND);
    BLE.configAdvert()->setAdvData(beacon.getAdvData(), beacon.advDataSize);
    BLE.configAdvert()->setScanRspData(beacon.getScanRsp(), beacon.scanRspSize);
    BLE.beginPeripheral();
}

void loop() {
    delay(1000);
}
