/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-usb-keyboard/
 */

#include "USBHIDDevice.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard keyboardDev;

#define ENABLE_PIN 8

void setup() {
    Serial.begin(115200);

    USBHIDDev.begin();

    pinMode(ENABLE_PIN, INPUT);
}

void loop() {
    if (USBHIDDev.USBconnected() && digitalRead(ENABLE_PIN)) {
        Serial.println("Sending keystrokes");
        keyboardDev.keyReleaseAll();
        delay(500);
        keyboardDev.keySequence("Hello World !");
        delay(1000);
        keyboardDev.keyPress(HID_KEY_BACKSPACE);
        delay(2000);
        keyboardDev.keyReleaseAll();
        delay(500);
        keyboardDev.consumerPress(HID_USAGE_CONSUMER_MUTE);
        delay(500);
        keyboardDev.consumerRelease();
    } else {
        delay(500);
    }
}
