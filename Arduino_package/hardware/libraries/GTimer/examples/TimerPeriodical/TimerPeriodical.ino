/*
  This sketch shows how to use hardware timer and invoke interrupt handler periodically
*/

#include <GTimer.h>

int counter = 0;

void myhandler(uint32_t data) {
    counter++;
    Serial.print("counter: ");
    Serial.println(counter);
    if (counter >= 10) {
        Serial.println("stop timer");
        GTimer.stop(0);
    }
}

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // timerid 0, period 1s, invoke myhander
    GTimer.begin(0, (1 * 1000 * 1000), myhandler);
}

void loop() {
    delay(1000);
}
