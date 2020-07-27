
#include "BLEDevice.h"
#include "BLEBatteryClient.h"

int8_t connID;
BLEAdvertData foundDevice;
BLEAdvertData targetDevice;
BLEBatteryClient battClient;
uint8_t battlevel = 0;
bool notifyState = 0;

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

void discoveryCB (uint8_t conn_id, bool service_found) {
    if (service_found) {
        Serial.print("Battery service found on connection id: ");
        Serial.println(conn_id);
    } else {
        Serial.print("Battery service not found on connection id: ");
        Serial.println(conn_id);
    }
}

void notificationCB (uint8_t conn_id, uint8_t batt_level) {
    Serial.print("Notification received for connection id: ");
    Serial.print(conn_id);
    Serial.print(" Battery level: ");
    Serial.println(batt_level);
}

void readCB (uint8_t conn_id, uint8_t batt_level) {
    Serial.print("Battery level read callback for connection id: ");
    Serial.print(conn_id);
    Serial.print(" Battery level: ");
    Serial.println(batt_level);
}

void setup() {
    Serial.begin(115200);

    BLE.init();
    BLE.configScan()->setScanMode(GAP_SCAN_MODE_ACTIVE);
    BLE.configScan()->setScanInterval(500);   // Start a scan every 500ms
    BLE.configScan()->setScanWindow(250);     // Each scan lasts for 250ms
    BLE.setScanCallback(scanCB);
    BLE.beginCentral(1);

    BLE.configClient(1);
    battClient.addClient(1);
    battClient.setDiscoveryCallback(discoveryCB);
    battClient.setNotificationCallback(notificationCB);
    battClient.setReadCallback(readCB);

    BLE.configScan()->startScan(2000);    // Scan for 2 seconds, then stop

    BLE.configConnection()->connect(targetDevice, 2000);
    connID = BLE.configConnection()->getConnId(targetDevice);
    vTaskDelay(2000 / portTICK_RATE_MS);

    if (BLE.connected(connID)) {
        Serial.print("BLE connected to device at ");
        Serial.println(connID);
        battClient.discoverServices(connID);
    } else {
        Serial.println("BLE not connected");
    }
}

void loop() {
    if (BLE.connected(connID)) {
        vTaskDelay(5000 / portTICK_RATE_MS);
        battClient.readLevel(connID);

        vTaskDelay(5000 / portTICK_RATE_MS);
        notifyState = battClient.readNotificationState(connID);
        notifyState = !notifyState;
        battClient.setNotificationState(connID, notifyState);
        if (notifyState)
            Serial.println("Notifications Enabled");
        else
            Serial.println("Notifications Disabled");
    } else {
        Serial.println("BLE not connected");
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

