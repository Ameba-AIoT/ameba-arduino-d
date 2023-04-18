// Wire Peripheral Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI Peripheral device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-i2c-4/
 */

#include <Wire.h>

void setup() {	
    Wire.begin(8);                // join i2c bus with address #8
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(115200);         // start serial for output
}

void loop() {
    delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    howMany = howMany;              // clear warning msg
    while(1 < Wire.available()) {   // loop through all but the last
        char c = Wire.read();         // receive byte as a character
        Serial.print(c);              // print the character
    }
    int x = Wire.read();            // receive byte as an integer
    Serial.println(x);              // print the integer
}

