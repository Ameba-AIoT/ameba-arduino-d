#include "IRDevice.h"

// User defined txPin, rxPin and carrier frequency
#define IR_RX_PIN           8
#define IR_TX_PIN           9
#define CARRIER_FREQ        38000

unsigned int irRawSignal[] = {
    9000, 4500,                                                                            // starting bit
    560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560,       // address   00100000 ： 4
    560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, // ~address  11011111
    560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560,       // data      00010000 ： 8
    560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, //~data      11101111
    560                                                                                    // stoping bit
};

int DataLen = sizeof(irRawSignal) / sizeof(irRawSignal[0]); // 284/ 4 = 71

void setup() {
    Serial.begin(115200);
    IR.begin(IR_RX_PIN, IR_TX_PIN, IR_MODE_TX, CARRIER_FREQ);
}

void loop() {
    IR.send(irRawSignal, DataLen);
    Serial.println("Finished Sending NEC Raw Data");
    delay(2000);
}
