/*
 Software serial test

 Receives from serial RX, and then sends to serial TX
 baud rate: 38400, data bits: 7, parity: even, 1 stop bit, with RTS/CTS

 The circuit: (BOARD_AMB21_AMB22)
 * RX is digital pin 0 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
 * RTS is pin PA_14 (connect to the other device CTS)
 * CTS is pin PA_15 (connect to the other device RTS)

 The circuit: (BOARD_AMB23)
 * RX is digital pin 2 (connect to the other device TX)
 * TX is digital pin 1 (connect to the other device RX)
 * RTS is pin PA_14 (connect to the other device CTS)
 * CTS is pin PA_15 (connect to the other device RTS)

 The circuit: (BOARD_AITHINKER_BW16)
 * RX is digital pin PB2 (connect to the other device TX)
 * TX is digital pin PB1 (connect to the other device RX)
 * RTS is pin PA_14 (connect to the other device CTS)
 * CTS is pin PA_15 (connect to the other device RTS)

The circuit: (BOARD_SPARKFUN_AWCU488)
* RX is digital pin 3 (connect to the other device TX)
* TX is digital pin 4 (connect to the other device RX)
* RTS is pin PA_14 (connect to the other device CTS)
* CTS is pin PA_15 (connect to the other device RTS)

The circuit: (BOARD_AMB25, BOARD_AMB26)
* RX is digital pin 17 (connect to the other device TX)
* TX is digital pin 16 (connect to the other device RX)
* RTS is pin PB_21 (connect to the other device CTS)
* CTS is pin PB_20 (connect to the other device RTS)
*/


#include <SoftwareSerial.h>

//RTS and CTS pins are fixed, it should not be changed by default.
#define RTS_PIN PA_14
#define CTS_PIN PA_15

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
    mySerial.begin(38400, 7, PARITY_EVEN, 1, FLOW_CONTROL_RTSCTS, RTS_PIN, CTS_PIN);

    mySerial.println("Hello, world?");

}

void loop() { // run over and over
    // send data on Serial Monitor and receive data on Tera Term with above settings
    if (Serial.available()) {
        mySerial.write(Serial.read());
    }
}
