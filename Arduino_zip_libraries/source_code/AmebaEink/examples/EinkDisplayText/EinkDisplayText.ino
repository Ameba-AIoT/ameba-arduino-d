#include "image.h"
#include "epdif.h"
#include "epdpaint.h"

// SPI Pin definition
#if defined(BOARD_RTL8722D)
#define EPD_BUSY_Pin        8
#define EPD_RES_Pin         9
#define EPD_SPI_CS_Pin      10
#define EPD_SPI_MOSI_Pin    11
#define EPD_SPI_MISO_Pin    12
#define EPD_SPI_CLK_Pin     13

#elif defined(BOARD_RTL8722DM_MINI)
#define EPD_BUSY_Pin        21
#define EPD_RES_Pin         20
#define EPD_SPI_CS_Pin      12
#define EPD_SPI_MOSI_Pin    9
#define EPD_SPI_MISO_Pin    10
#define EPD_SPI_CLK_Pin     11

#else
#error chack the borad supported
#endif

#define COLOR_BLACK        0
#define COLOR_WHITE        1

EpdIf EPD(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);
unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

void setup() {
    pinMode(EPD_BUSY_Pin, INPUT);       // EPD_BUSY_Pin
    pinMode(EPD_RES_Pin, OUTPUT);       // EPD_RES_Pin
    pinMode(EPD_SPI_CS_Pin, OUTPUT);    // EPD_SPI_CS_Pin
    pinMode(EPD_SPI_MOSI_Pin, OUTPUT);  // EPD_SPI_MOSI_Pin
    pinMode(EPD_SPI_MISO_Pin, OUTPUT);  // EPD_SPI_MISO_Pin
    pinMode(EPD_SPI_CLK_Pin, OUTPUT);   // EPD_SPI_CLK_Pin
}

void loop() {
    DisplayText();
    EPD.EPD_Sleep();
}

void DisplayText(void) {
    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapNA);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth(128);
    paint.SetHeight(24);

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "Hello Ameba", &Font16, COLOR_WHITE);
    EPD.EPD_SetFrame(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 4, "1234567890", &Font16, COLOR_BLACK);
    EPD.EPD_SetFrame(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "+_*/><:*&^%", &Font16, COLOR_WHITE);
    EPD.EPD_SetFrame(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());

    EPD.EPD_UpdateDisplay();
}
