// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-i2c-1/
 */

#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin(); // join i2c bus (address optional for master)
    Serial.println("Master writer sending data to slave 8");
}

byte x = 0;

void loop() {
    Wire.beginTransmission(8);  // transmit to device #8
    Wire.write("x is ");        // sends five bytes
    Wire.write(x);              // sends one byte
    Wire.endTransmission();     // stop transmitting

    x++;
    delay(500);
}
