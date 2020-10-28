
#include "BLEDevice.h"

#define BATT_SERVICE_UUID   "180F"
#define BATT_LEVEL_UUID     "2A19"

BLEAdvertData foundDevice;
BLEAdvertData targetDevice;
BLEClient* client;
BLERemoteService* battService;
BLERemoteCharacteristic* battChar;
bool notifyState = false;
int8_t connID;

void scanCB(T_LE_CB_DATA* p_data) {
    foundDevice.parseScanInfo(p_data);
    if (foundDevice.hasName()) {
        if (foundDevice.getName() == String("AMEBA_BLE_DEV")) {
            Serial.print("Found Ameba BLE Device at address ");
            Serial.println(foundDevice.getAddr().str());
            targetDevice = foundDevice;
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

void notificationCB (BLERemoteCharacteristic* chr, uint8_t* data, uint16_t len) {
    Serial.print("Notification received for chr UUID: ");
    Serial.print(chr->getUUID().str());
    if (len == 1) {
        Serial.print(" Battery level: ");
        Serial.println(data[0]);
    }
}

void setup() {
    Serial.begin(115200);

    BLE.init();
    BLE.configScan()->setScanMode(GAP_SCAN_MODE_ACTIVE);
    BLE.configScan()->setScanInterval(500);   // Start a scan every 500ms
    BLE.configScan()->setScanWindow(250);     // Each scan lasts for 250ms
    BLE.setScanCallback(scanCB);
    BLE.beginCentral(1);

    BLE.configScan()->startScan(2000);    // Scan for 2 seconds, then stop
    BLE.configConnection()->connect(targetDevice, 2000);
    delay(2000);
    connID = BLE.configConnection()->getConnId(targetDevice);

    if (!BLE.connected(connID)) {
        Serial.println("BLE not connected");
    } else {
        BLE.configClient();
        client = BLE.addClient(connID);
        client->discoverServices();
        Serial.print("Discovering services of connected device");
        do {
            Serial.print(".");
            delay(1000);
        } while (!(client->discoveryDone()));
        Serial.println();

        battService = client->getService(BATT_SERVICE_UUID);
        if (battService != nullptr) {
            battChar = battService->getCharacteristic(BATT_LEVEL_UUID);
            if (battChar != nullptr) {
                Serial.println("Battery level characteristic found");
                battChar->setNotifyCallback(notificationCB);
            }
        }
    }
}

void loop() {
    if (BLE.connected(connID)) {
        delay(5000);
        Serial.print("Battery level read: ");
        Serial.println(battChar->readData8());

        delay(5000);
        notifyState = !notifyState;
        if (notifyState) {
            Serial.println("Notifications Enabled");
            battChar->enableNotifyIndicate();
        } else {
            Serial.println("Notifications Disabled");
            battChar->disableNotifyIndicate();
        }
    } else {
        Serial.println("BLE not connected");
        delay(5000);
    }
}
