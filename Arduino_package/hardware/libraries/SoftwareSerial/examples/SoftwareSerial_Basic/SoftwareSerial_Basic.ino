/*
 Software serial test

 Receives from serial RX, and then sends to serial TX

 The circuit: (BOARD_AMB21_AMB22)
 * RX is digital pin 0 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
              (BOARD_AMB23)
 * RX is digital pin 2 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
              (BOARD_AITHINKER_BW16)
 * RX is digital pin PB2 (connect to the other device TX)
 * TX is digital pin PB1 (connect to the other device RX)
               (BOARD_SPARKFUN_AWCU488)
 * RX is digital pin 3 (connect to the other device TX)
 * TX is digital pin 4 (connect to the other device RX)
              (BOARD_AMB25, BOARD_AMB26)
 * RX is digital pin 17 (connect to the other device TX)
 * TX is digital pin 16 (connect to the other device RX)

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-uart/
 */

#include <SoftwareSerial.h>

#if defined(BOARD_AMB21_AMB22)
    SoftwareSerial mySerial(0, 1); // RX, TX
#elif defined(BOARD_AMB23)
    SoftwareSerial mySerial(2, 1); // RX, TX
#elif defined(BOARD_AITHINKER_BW16)
    SoftwareSerial mySerial(PB2, PB1); // RX, TX
#elif defined(BOARD_SPARKFUN_AWCU488)
    SoftwareSerial mySerial(3, 4); // RX, TX
#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306) || defined(BOARD_DATALOGGER_AMB26)
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
