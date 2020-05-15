/*
 Software serial test

 Receives from serial RX, and then sends to serial TX
 baud rate: 38400, data bits: 7, parity: even, 1 stop bit, with RTS/CTS

 The circuit: (BOARD RTL8722CSM)
 * RX is digital pin 0 (connect to TX of other device)
 * TX is digital pin 1 (connect to RX of other device)
 * RTS is pin A14 (connect to CTS of other device)
 * CTS is pin A15 (connect to RTS of other device)

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }


    Serial.println("Goodnight moon!");
    // set the data rate for the SoftwareSerial port
    mySerial.begin(38400, 7, PARITY_EVEN, 1, FLOW_CONTROL_RTSCTS, 14, 15);// Note that 14 and 15 are pin A14 and A15 

    mySerial.println("Hello, world?");

}

void loop() { // run over and over
    // send data on Serial Monitor and receive data on Tera Term with above settings
    if (Serial.available()) {
        mySerial.write(Serial.read());
    }
}
