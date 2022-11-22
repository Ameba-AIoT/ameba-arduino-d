#include "IRDevice.h"

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

uint8_t adr = 0;
uint8_t cmd = 0;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    IR.beginNEC(IR_RX_PIN, IR_TX_PIN, IR_MODE_TX); // configure for NEC IR protocol
}

void loop() {
    if (cmd++ >=255) {
        adr++;
    }

    IR.sendNEC(adr, cmd);
    Serial.print("Sent ");
    Serial.print(adr);
    Serial.print(cmd);
    Serial.println();
    //IR.end();   // Call this method to stop IR device and free up the pins for other uses
}
