/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-usb-cdc/
 */

#include "USBCDCDevice.h"

void setup() {
    Serial.begin(115200);
    SerialUSB.begin();
}

void loop() {
    if (SerialUSB.available()) {
        int inByte = SerialUSB.read();
        Serial.write(inByte);
    }
    if (Serial.available()) {
        int inByte = Serial.read();
        SerialUSB.write(inByte);
    }
    delay(1);
}
