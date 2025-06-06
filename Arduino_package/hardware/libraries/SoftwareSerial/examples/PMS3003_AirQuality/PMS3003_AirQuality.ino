/*
 This example demonstrate how to read pm2.5 value on PMS 3003 air condition sensor

 PMS 3003 pin map is as follow:
    PIN1  :VCC, connect to 5V
    PIN2  :GND
    PIN3  :SET, 0:Standby mode, 1:operating mode
    PIN4  :RXD :Serial RX
    PIN5  :TXD :Serial TX
    PIN6  :RESET
    PIN7  :NC
    PIN8  :NC

 In this example, we only use Serial to get PM 2.5 value.

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
 https://www.amebaiot.com/en/amebad-arduino-pm25/
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


#define pmsDataLen 32
uint8_t buf[pmsDataLen];
int idx = 0;
int pm10 = 0;
int pm25 = 0;
int pm100 = 0;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
}

void loop() { // run over and over
    uint8_t c = 0;
    idx = 0;
    memset(buf, 0, pmsDataLen);

    while (true) {
        while (c != 0x42) {
            while (!mySerial.available());
            c = mySerial.read();
        }
        while (!mySerial.available());
        c = mySerial.read();
        if (c == 0x4d) {
            // now we got a correct header)
            buf[idx++] = 0x42;
            buf[idx++] = 0x4d;
            break;
        }
    }

    while (idx != pmsDataLen) {
        while(!mySerial.available());
        buf[idx++] = mySerial.read();
    }

    pm10 = (buf[10] << 8) | buf[11];
    pm25 = (buf[12] << 8) | buf[13];
    pm100 = (buf[14] << 8) | buf[15];

    Serial.print("pm2.5: ");
    Serial.print(pm25);
    Serial.println(" ug/m3");
}
