#include "epdif.h"
#include "image.h"

EpdIf EPD;

void setup() {
    pinMode(BUSY_Pin, INPUT);   // BUSY_Pin    8
    pinMode(RES_Pin, OUTPUT);   // RES_Pin     9
    pinMode(DC_Pin, OUTPUT);    // DC_Pin     10
    pinMode(CS_Pin, OUTPUT);    // CS_Pin     11
    pinMode(SCK_Pin, OUTPUT);   // SCK_Pin    12
    pinMode(SDI_Pin, OUTPUT);   // SDI_Pin    13
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
