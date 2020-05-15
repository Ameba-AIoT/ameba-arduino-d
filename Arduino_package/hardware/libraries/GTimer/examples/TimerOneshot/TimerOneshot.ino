/*
   This sketch shows how to use several hardware timers in invoke handler only once for each timer.
*/

#include <GTimer.h>

void myhandler(uint32_t data) {
    Serial.print("I am timer!");
    Serial.println(data);
}

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // timerid 0, period 1s, invoke myhandler, invoke only once, user data is 0
    GTimer.begin(0, 1 * 1000 * 1000, myhandler, false, 0);

    // timerid 1, period 2s, invoke myhandler, invoke only once, user data is 1
    GTimer.begin(1, 2 * 1000 * 1000, myhandler, false, 1);

    GTimer.begin(2, 3 * 1000 * 1000, myhandler, false, 2);
    GTimer.begin(3, 4 * 1000 * 1000, myhandler, false, 3);
}

void loop() {
    delay(1000);
}
