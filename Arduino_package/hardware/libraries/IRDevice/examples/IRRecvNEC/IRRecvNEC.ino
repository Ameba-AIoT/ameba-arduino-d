#include "IRDevice.h"

uint8_t adr = 0;
uint8_t cmd = 0;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    IR.beginNEC(8, 9, IR_MODE_RX); // configure for NEC IR protocol
}

void loop() {
    if (IR.recvNEC(adr, cmd, 1000)) {
        Serial.print("Received ");
        Serial.print(adr);
        Serial.print(cmd);
        Serial.println();
    } else {
        Serial.println("Received nothing, timed out");
    }
    //IR.end();   // Call this method to stop IR device and free up the pins for other uses
}
