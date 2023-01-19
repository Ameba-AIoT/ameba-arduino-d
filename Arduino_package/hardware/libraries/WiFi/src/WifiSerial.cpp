#include "WifiSerial.h"
#include <stdlib.h>
#include <Arduino.h>

WifiSerial::WifiSerial() {}//declaration

//Function: Reads Serial monitor input
const char* WifiSerial::readInput() {
    currentInput = "";    //clear string output in preparation
    String name;
    while (Serial.available()) {
        delay(2);    //add a delay to buy time for bytes to enter buffer
        char c = Serial.read();
        name += c;
    }
    if (name.length() > 0) {          //loop triggers when Serial data is received.
        name.trim();                  //removes trailing newline
        currentInput = name.c_str();  //convert name from String to string
    }
    return currentInput;
}
