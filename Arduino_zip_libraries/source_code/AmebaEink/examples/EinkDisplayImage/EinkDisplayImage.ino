#include "epdif.h"
#include "image.h"

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

EpdIf EPD(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);

void setup() {
    pinMode(EPD_BUSY_Pin, INPUT);       // EPD_BUSY_Pin
    pinMode(EPD_RES_Pin, OUTPUT);       // EPD_RES_Pin
    pinMode(EPD_SPI_CS_Pin, OUTPUT);    // EPD_SPI_CS_Pin
    pinMode(EPD_SPI_MOSI_Pin, OUTPUT);  // EPD_SPI_MOSI_Pin
    pinMode(EPD_SPI_MISO_Pin, OUTPUT);  // EPD_SPI_MISO_Pin
    pinMode(EPD_SPI_CLK_Pin, OUTPUT);   // EPD_SPI_CLK_Pin
}

void loop() {
    DisplayImage();   // Display an image
    DisplayQRCode();  // Displays QR codes every 4-5 sec
    DisplayGIF();     // Display a 3 frame GIF
}

void DisplayImage(void) {
    /* User predefined image files "gImage_basemapIMG"  (296 x 128)
     can be modified in "image.h"
    */
    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapIMG);   
    delay(4000);
}

void DisplayQRCode(void) {
    EPD.EPD_SetRAMValue_BaseMap(gImage_QR0);          // fb website 296 x 128
    delay(4000);
    EPD.EPD_SetRAMValue_BaseMap(gImage_QR1);          // official website 296 x 128
    delay(4000);
    EPD.EPD_SetRAMValue_BaseMap(gImage_QR2);          // communicty website 296 x 128
    delay(4000);
}

void DisplayGIF(void) {
    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapNA);
    delay(100);
    for (int i = 0; i < 5; i++) {
        EPD.EPD_Dis_Part(20, 200, gImage_f1, 128, 64);  // x,y,DATA,resolution 128 x 64
        EPD.EPD_UpdateDisplay();
        EPD.EPD_Dis_Part(20, 200, gImage_f2, 128, 64);  // x,y,DATA,resolution 128 x 64
        EPD.EPD_UpdateDisplay();
        EPD.EPD_Dis_Part(20, 200, gImage_f3, 128, 64);  // x,y,DATA,resolution 128 x 64
        EPD.EPD_UpdateDisplay();
    }
}
