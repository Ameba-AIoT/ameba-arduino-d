/*
 This example demonstrate how to read pm2.5 value on PMS 3003 air condition sensor
 And display it on ILI9341 TFT LCD

 In this example, we only use Serial to get PM 2.5 value.

 The PMS 3003 circuit:
 * RX is digital pin 0 (connect to TX of PMS 3003)
 * TX is digital pin 1 (connect to RX of PMS 3003)
 * Vcc connect to 5V
 * GND connect to GND


 For RTL8722DM/CSM, ILI9341 TFT LCD with SPI interface has these pins:
    RST  : connect to pin 8
    D/C  : connect to pin 9
    CS   : connect to pin 10
    MOSI : connect to pin 11
    MISO : connect to pin 12
    CLK  : connect to pin 13
    VCC  : connect to 3V3
    GND  : connect to GND

 */

#include <SoftwareSerial.h>
#include <SPI.h>
#include <AmebaILI9341.h>

#if defined(BOARD_RTL8195A)

SoftwareSerial mySerial(0, 1); // RX, TX

#define TFT_RESET       8
#define TFT_DC          9
#define TFT_CS          10

#elif defined(BOARD_RTL8710)

SoftwareSerial mySerial(17, 5); // RX, TX

// IMPORTANT: Due to limit pin, we do not connect TFT_RESET pin.
#define TFT_RESET       0xFFFFFFFF
#define TFT_DC          2
#define TFT_CS          10

#elif defined(BOARD_RTL8721D)
// RTL8722DM -- Do NOT change this pin setting, as softserial is currently only supported on selected pins
SoftwareSerial mySerial(0, 1); // RX, TX

// IMPORTANT: Due to limit pin, we do not connect TFT_RESET pin.
#define TFT_RESET       8
#define TFT_DC          9
#define TFT_CS          10

#endif


AmebaILI9341 tft = AmebaILI9341(TFT_CS, TFT_DC, TFT_RESET);

#define ILI9341_SPI_FREQUENCY 20000000

#define pmsDataLen 32
uint8_t buf[pmsDataLen];
int idx = 0;
int pm10 = 0;
int last_pm25 = 0;
int pm25 = 0;
int pm100 = 0;

uint16_t pm25color[] = {
    0x9FF3,
    0x37E0,
    0x3660,
    0xFFE0,
    0xFE60,
    0xFCC0,
    0xFB2C,
    0xF800,
    0x9800,
    0xC99F
};

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600); // PMS 3003 UART has baud rate 9600

    SPI.setDefaultFrequency(ILI9341_SPI_FREQUENCY);

    tft.begin();
    drawPictureFrames();
}

void loop() { // run over and over
    uint8_t c;
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
    last_pm25 = pm25;
    pm25 = (buf[12] << 8) | buf[13];
    pm100 = (buf[14] << 8) | buf[15];

    updateValueToTftScreen();
}

void drawPictureFrames() {
    tft.setRotation(1);
    tft.clr();

    tft.setFontSize(1);

    // Upper title
    tft.setFontSize(1);
    tft.setCursor(20, 20);
    tft.print("PM2.5 DETECTOR");

    // PM 2.5 Circle Frame
    tft.drawCircle(100, 130, 60, ILI9341_BLUE);
    tft.drawCircle(100, 130, 61, ILI9341_BLUE);

    tft.setFontSize(1);
    tft.setCursor(90, 85);
    tft.print("PM2.5");

    tft.setFontSize(1);
    tft.setCursor(90, 170);
    tft.print("um/m3");

    // PM 10 Circle Frame
    tft.drawCircle(220, 70, 40, ILI9341_BLUE);

    tft.setFontSize(1);
    tft.setCursor(210, 40);
    tft.print("PM10");

    tft.setFontSize(1);
    tft.setCursor(205, 95);
    tft.print("um/m3");

    // PM 1.0 Circle Frame
    tft.drawCircle(220, 170, 40, ILI9341_BLUE);

    tft.setFontSize(1);
    tft.setCursor(205, 140);
    tft.print("PM1.0");

    tft.setFontSize(1);
    tft.setCursor(205, 195);
    tft.print("um/m3");

    // right side bar, referenced from: http://taqm.epa.gov.tw/taqm/tw/
    tft.fillRectangle(290, (30 +  0 * 2), 10, (12 * 2), pm25color[0]); // 0~11
    tft.fillRectangle(290, (30 + 12 * 2), 10, (12 * 2), pm25color[1]); // 12-23
    tft.fillRectangle(290, (30 + 24 * 2), 10, (12 * 2), pm25color[2]); // 24-35
    tft.fillRectangle(290, (30 + 36 * 2), 10,  (6 * 2), pm25color[3]); // 36-41
    tft.fillRectangle(290, (30 + 42 * 2), 10,  (6 * 2), pm25color[4]); // 42-47
    tft.fillRectangle(290, (30 + 48 * 2), 10,  (6 * 2), pm25color[5]); // 48-53
    tft.fillRectangle(290, (30 + 54 * 2), 10,  (6 * 2), pm25color[6]); // 54-58
    tft.fillRectangle(290, (30 + 59 * 2), 10,  (6 * 2), pm25color[7]); // 59-64
    tft.fillRectangle(290, (30 + 65 * 2), 10,  (6 * 2), pm25color[8]); // 65-70
    tft.fillRectangle(290, (30 + 71 * 2), 10, (10 * 2), pm25color[9]); // >=71

    tft.setCursor(302, 30);
    tft.setFontSize(1);
    tft.print("0");
    tft.setCursor(302, (30 + 36 * 2));
    tft.print("36");
    tft.setCursor(302, (30 + 54 * 2));
    tft.print("54");
    tft.setCursor(302, (30 + 71 * 2));
    tft.print("71");

    // bottom right text
    tft.setCursor(210, 230);
    tft.setFontSize(1);
    tft.print("Powered by Realtek");

    updateValueToTftScreen();
}

void updateValueToTftScreen() {
    tft.setCursor(60, 111);
    tft.setFontSize(5);
    tft.setForeground(getPm25Color(pm25));
    if (pm25 < 10) {
        tft.print("  ");
    } else if (pm25 < 100) {
        tft.print(" ");
    }
    tft.print(pm25);

    tft.setCursor(195, 60);
    tft.setFontSize(3);
    if (pm100 < 10) {
        tft.print("  ");
    } else if (pm100 < 100) {
        tft.print(" ");
    }
    tft.print(pm100);

    tft.setCursor(198,160);
    if (pm10 < 10) {
        tft.print("  ");
    } else if (pm10 < 100) {
        tft.print(" ");
    }
    tft.print(pm10);

    tft.setFontSize(1);
    tft.setForeground(ILI9341_WHITE);
    if (last_pm25 > 80) {
        tft.fillRectangle(275, (80 * 2 + 30 - 3), 12, 8, ILI9341_BLACK);
    } else {
        tft.fillRectangle(275, (last_pm25 * 2 + 30 - 3), 12, 8, ILI9341_BLACK);
    }
    if (pm25 > 80) {
        tft.setCursor(275, (80 * 2 + 30 - 3));
    } else {
        tft.setCursor(275, (pm25 * 2 + 30 - 3));
    }
    tft.print("=>");
}

uint16_t getPm25Color(int v) {
    if (v < 12) {
        return pm25color[0];
    } else if (v < 24) {
        return pm25color[1];
    } else if (v < 36) {
        return pm25color[2];
    } else if (v < 42) {
        return pm25color[3];
    } else if (v < 48) {
        return pm25color[4];
    } else if (v < 54) {
        return pm25color[5];
    } else if (v < 59) {
        return pm25color[6];
    } else if (v < 65) {
        return pm25color[7];
    } else if (v < 71) {
        return pm25color[8];
    } else {
        return pm25color[9];
    }
}
