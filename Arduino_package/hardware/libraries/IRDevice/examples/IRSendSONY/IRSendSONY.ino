#include "IRDevice.h"

// User defined txPin, rxPin and carrier frequency
#define IR_RX_PIN         8
#define IR_TX_PIN         9
#define CARRIER_FREQ  40000

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  IR.begin(IR_RX_PIN, IR_TX_PIN, IR_MODE_TX, CARRIER_FREQ);
}

void loop() {
  unsigned long dataSend = 0xa90;
  for (int i = 0; i < 3; i++) {
    IR.sendSONY(dataSend, 12);
    Serial.print("SentSony: 0x");
    Serial.println(dataSend, HEX);
  }
  delay(3000);
}
