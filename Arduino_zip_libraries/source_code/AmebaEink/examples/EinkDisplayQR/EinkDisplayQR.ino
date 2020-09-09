#include <avr/pgmspace.h>

#include "epdif.h"
#include "epdpaint.h"
#include "image.h"
#include "qrcode.h"

#define COLORED           0
#define UNCOLORED         1
#define QRCODE_VERSION    5

EpdIf EPD;
unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

void setup() {
    pinMode(BUSY_Pin, INPUT);   // BUSY_Pin    8
    pinMode(RES_Pin, OUTPUT);   // RES_Pin     9
    pinMode(DC_Pin, OUTPUT);    // DC_Pin     10
    pinMode(CS_Pin, OUTPUT);    // CS_Pin     11
    pinMode(SCK_Pin, OUTPUT);   // SCK_Pin    12
    pinMode(SDI_Pin, OUTPUT);   // SDI_Pin    13
}

void loop() {
    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapNA);
    PrintQRCode("https://www.amebaiot.com/en/ameba-arduino-summary/");
    EPD.EPD_Sleep();
}

void PrintQRCode(const char * url){
    QRCode qrcode;
    const int ps = 2;               // pixels / square : used to define the size of QR
    uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
    qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth (80);
    paint.SetHeight(80);
    paint.Clear(UNCOLORED);

    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y)) {
                for (int xi = (x * ps + 2); xi < (x * ps + ps + 2); xi++) { // If pixel is on, we draw a "ps x ps" black square
                    for (int yi = (y * ps + 2); yi < (y * ps + ps + 2); yi++) {
                        paint.DrawPixel(xi, yi, COLORED);
                    }
                }
            }
        }
    }
    EPD.EPD_ClearScreen_White();  // Display whole screen in white
    EPD.EPD_Dis_Part(30, 200, paint.GetImage(), paint.GetWidth(), paint.GetHeight());
    EPD.EPD_UpdateDisplay();
}
