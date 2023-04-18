/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-usb-gamepad/
 */

#include "USBHIDDevice.h"
#include "USBHIDGamepad.h"

USBHIDGamepad padDev;

#define ENABLE_PIN 8

// We need to define a HID gamepad report descriptor to replace the default keyboard and mouse descriptor
uint8_t hid_gamepad_report_descriptor[] = {
  TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(1) )
};

void setup() {
  Serial.begin(115200);
  
  // Replace the default HID report descriptor, and provide the HID gamepad report descriptor
  USBHIDDev.setReportDescriptor(hid_gamepad_report_descriptor, sizeof(hid_gamepad_report_descriptor));
  // Configure USB HID device for 16 byte gamepad reports
  USBHIDDev.setUSBEndpointMPS(16);
  USBHIDDev.begin();

  pinMode(ENABLE_PIN, INPUT);
}

void loop() {
  if (USBHIDDev.USBconnected() && digitalRead(ENABLE_PIN)) {
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
