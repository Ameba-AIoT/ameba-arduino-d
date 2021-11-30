#include "BLEHIDDevice.h"
#include "BLEHIDMouse.h"
#include "BLEDevice.h"

BLEHIDMouse mouseDev;
BLEAdvertData advdata;

#define ENABLE_PIN 8

void setup() {
  Serial.begin(115200);
  advdata.addFlags();
  advdata.addCompleteName("AMEBA_BLE_HID");
  advdata.addAppearance(GAP_GATT_APPEARANCE_HUMAN_INTERFACE_DEVICE);
  advdata.addCompleteServices(BLEUUID(HID_SERVICE_UUID));

  BLEHIDDev.init();

  BLE.init();
  BLE.configAdvert()->setAdvData(advdata);
  BLE.setDeviceName("AMEBA_BLE_HID");
  BLE.setDeviceAppearance(GAP_GATT_APPEARANCE_HUMAN_INTERFACE_DEVICE);
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
    Serial.println("Moving mouse");
    // Move cursor in a square
    mouseDev.mouseMove(0, 50);
    delay(500);
    // Right click
    mouseDev.mousePress(MOUSE_BUTTON_RIGHT);
    delay(100);
    mouseDev.mouseRelease(MOUSE_BUTTON_RIGHT);
    delay(100);
    mouseDev.mouseMove(-50, 0);
    delay(500);
    mouseDev.mouseMove(0, -50);
    delay(500);
    // Left click
    mouseDev.mousePress(MOUSE_BUTTON_LEFT);
    delay(100);
    mouseDev.mouseRelease(MOUSE_BUTTON_LEFT);
    delay(100);
    mouseDev.mouseMove(50, 0);
    delay(500);
    // Scroll up and down
    mouseDev.mouseScroll(5);
    delay(500);
    mouseDev.mouseScroll(-5);
    delay(500);
  } else {
    delay(500);
  }
}