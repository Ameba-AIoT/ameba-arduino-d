/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ble-gamepad/
 */

#include "BLEHIDDevice.h"
#include "BLEHIDGamepad.h"
#include "BLEDevice.h"

BLEHIDGamepad padDev;
BLEAdvertData advdata;

#define ENABLE_PIN 8

// We need to define a HID gamepad report descriptor to replace the default keyboard and mouse descriptor
uint8_t hid_gamepad_report_descriptor[] = {
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(1))
};

void setup() {
    Serial.begin(115200);
    advdata.addFlags();
    advdata.addCompleteName("AMEBA_BLE_HID");
    advdata.addAppearance(GAP_GATT_APPEARANCE_GAMEPAD);
    advdata.addCompleteServices(BLEUUID(HID_SERVICE_UUID));

    BLEHIDDev.init();
    // Replace the default HID report descriptor, and provide the HID gamepad report descriptor with the corresponding report ID
    BLEHIDDev.setReportMap(hid_gamepad_report_descriptor, sizeof(hid_gamepad_report_descriptor));
    padDev.setReportID(1);

    BLE.init();
    BLE.configAdvert()->setAdvData(advdata);
    BLE.setDeviceName("AMEBA_BLE_HID");
    BLE.setDeviceAppearance(GAP_GATT_APPEARANCE_GAMEPAD);
    BLE.configSecurity()->setPairable(true);
    BLE.configSecurity()->setAuthFlags(GAP_AUTHEN_BIT_BONDING_FLAG);
    BLE.configServer(3);
    BLE.addService(BLEHIDDev.hidService());
    BLE.addService(BLEHIDDev.battService());
    BLE.addService(BLEHIDDev.devInfoService());

    pinMode(ENABLE_PIN, INPUT);

    BLE.beginPeripheral();
}

void loop() {
    if (BLE.connected() && digitalRead(ENABLE_PIN)) {
        Serial.println("Moving joysticks and pressing buttons");
        // Press buttons 5 and 16. Move all axes to max. Set DPAD (hat 1) to down right
        padDev.gamepadReport( (GAMEPAD_BUTTON_4 | GAMEPAD_BUTTON_15) , GAMEPAD_HAT_DOWN_RIGHT, 32767, 32767, 32767, 32767, 32767, 32767);
        delay(1000);
        // Release button 5. Move all axes to min. Set DPAD (hat 1) to centred
        padDev.gamepadReport( (GAMEPAD_BUTTON_15), GAMEPAD_HAT_CENTERED, -32767, -32767, -32767, -32767, -32767, -32767);
        delay(1000);
        // Release all buttons and set all axes to centered
        padDev.buttonReleaseAll();
        padDev.setAxes(0, 0, 0, 0, 0, 0);
        delay(1000);
    } else {
        delay(500);
    }
}
