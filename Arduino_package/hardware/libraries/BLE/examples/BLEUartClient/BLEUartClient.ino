
#include "BLEDevice.h"

#define UART_SERVICE_UUID      "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define STRING_BUF_SIZE 100

BLEAdvertData foundDevice;
BLEAdvertData targetDevice;
BLEClient* client;
BLERemoteService* UartService;
BLERemoteCharacteristic* Rx;
BLERemoteCharacteristic* Tx;

void scanCB(T_LE_CB_DATA* p_data) {
    foundDevice.parseScanInfo(p_data);
    if (foundDevice.hasName()) {
        if (foundDevice.getName() == String("AMEBA_BLE_DEV")) {
            Serial.print("Found Ameba BLE Device at address ");
            Serial.println(foundDevice.getAddr().str());
            targetDevice = foundDevice;
        }
    }
}

void notificationCB (BLERemoteCharacteristic* chr, uint8_t* data, uint16_t len) {
    char msg[len+1] = {0};
    memcpy(msg, data, len);
    Serial.print("Notification received for chr UUID: ");
    Serial.println(chr->getUUID().str());
    Serial.print("Received string: ");
    Serial.println(String(msg));
}

void setup() {
    Serial.begin(115200);

    BLE.init();
    BLE.setScanCallback(scanCB);
    BLE.beginCentral(1);

    BLE.configScan()->startScan(2000);
    BLE.configConnection()->connect(targetDevice, 2000);
    delay(2000);
    int8_t connID = BLE.configConnection()->getConnId(targetDevice);
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

        UartService = client->getService(UART_SERVICE_UUID);
        if (UartService != nullptr) {
            Tx = UartService->getCharacteristic(CHARACTERISTIC_UUID_TX);
            if (Tx != nullptr) {
                Serial.println("TX characteristic found");
                Tx->setBufferLen(STRING_BUF_SIZE);
                Tx->setNotifyCallback(notificationCB);
                Tx->enableNotifyIndicate();
            }
            Rx = UartService->getCharacteristic(CHARACTERISTIC_UUID_RX);
            if (Rx != nullptr) {
                Serial.println("RX characteristic found");
                Rx->setBufferLen(STRING_BUF_SIZE);
            }
        }
    }
}

void loop() {
    if (Serial.available()) {
        Rx->writeString(Serial.readString());
    }
    delay(100);
}
