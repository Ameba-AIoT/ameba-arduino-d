/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-irdevice/
 */

#include "IRDevice.h"

// Set IR TX and RX pins
// check the board pin mapping for available IR pins
#define IR_RX_PIN       8
#define IR_TX_PIN       9

// User defined carrier frequency
#define CARRIER_FREQ    38000

unsigned int irRawSignal[] = {
// starting bit
    9000, 4500,
// address   00100000 ： 4
    560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560,
// ~address  11011111
    560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690,
// data      00010000 ： 8
    560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560,
//~data      11101111
    560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690,
// stoping bit
    560
};

// 284/ 4 = 71
int DataLen = sizeof(irRawSignal) / sizeof(irRawSignal[0]);

void setup() {
    Serial.begin(115200);
    IR.begin(IR_RX_PIN, IR_TX_PIN, IR_MODE_TX, CARRIER_FREQ);
}

void loop() {
    IR.send(irRawSignal, DataLen);
    Serial.println("Finished Sending NEC Raw Data");
    delay(2000);
}
