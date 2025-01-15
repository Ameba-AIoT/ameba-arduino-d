/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-usb-mouse/
 */

// USB functionality is not supported on core RTL8720DN
#ifndef CORE_RTL8720DN

#include "USBHIDDevice.h"
#include "USBHIDMouse.h"

USBHIDMouse mouseDev;

#define ENABLE_PIN 8

void setup() {
    Serial.begin(115200);

    USBHIDDev.begin();

    pinMode(ENABLE_PIN, INPUT);
}

void loop() {
    if (USBHIDDev.USBconnected() && digitalRead(ENABLE_PIN)) {
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

#endif
