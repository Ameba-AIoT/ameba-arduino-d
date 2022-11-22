/*
 Software serial test

 Receives from serial RX, and then sends to serial TX

 The circuit: (BOARD RTL8722CSM / DM)
 * RX is digital pin 0 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
              (BOARD RTL8722DM_MINI)
 * RX is digital pin 2 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
              (BOARD RTL8720DN / BW16)
 * RX is digital pin PB2 (connect to the other device TX)
 * TX is digital pin PB1 (connect to the other device RX)
               (BOARD RTL8721DM)
 * RX is digital pin 3 (connect to the other device TX)
 * TX is digital pin 4 (connect to the other device RX)
 */

#include <SoftwareSerial.h>

#if defined(BOARD_RTL8722DM)
    SoftwareSerial mySerial(0, 1); // RX, TX
#elif defined(BOARD_RTL8722DM_MINI)
    SoftwareSerial mySerial(2, 1); // RX,TX
#elif defined(BOARD_RTL8720DN_BW16)
    SoftwareSerial mySerial(PB2, PB1); // RX, TX
#elif defined(BOARD_RTL8721DM)
    SoftwareSerial mySerial(3, 4); // RX, TX
#elif defined(BOARD_RTL8720DF)
    SoftwareSerial mySerial(17, 16); // RX, TX
#else
    SoftwareSerial mySerial(0, 1); // RX, TX
#endif


void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Goodnight moon!");

    // set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    mySerial.println("Hello, world?");
}

void loop() { // run over and over
    if (mySerial.available()) {
        mySerial.write(mySerial.read());
    }
}
