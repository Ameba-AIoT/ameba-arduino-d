/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ir-sonydata/
 */

#include "IRDevice.h"

// User defined txPin, rxPin and carrier frequency
#if defined(BOARD_RTL8722DM)
#define IR_RX_PIN       8
#define IR_TX_PIN       9
#elif defined(BOARD_RTL8722DM_MINI)
#error Sorry, RTL8722DM_MINI do not support.
#elif defined(BOARD_RTL8720DN_BW16)
#define IR_RX_PIN       PA26
#define IR_TX_PIN       PA25
#elif defined(BOARD_RTL8721DM)
#define IR_RX_PIN       5
#define IR_TX_PIN       6
#elif defined(BOARD_RTL8720DF)
#define IR_RX_PIN       6
#define IR_TX_PIN       7
#endif

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
