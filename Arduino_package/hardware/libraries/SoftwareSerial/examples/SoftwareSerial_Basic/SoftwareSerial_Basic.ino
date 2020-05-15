/*
 Software serial test

 Receives from serial RX, and then sends to serial TX

 The circuit: (BOARD RTL8722CSM)
 * RX is digital pin 0 (connect to TX of other device)
 * TX is digital pin 1 (connect to RX of other device)

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
    mySerial.begin(4800);
    mySerial.println("Hello, world?");
}

void loop() { // run over and over
    if (mySerial.available()) {
        mySerial.write(mySerial.read());
    }
}
