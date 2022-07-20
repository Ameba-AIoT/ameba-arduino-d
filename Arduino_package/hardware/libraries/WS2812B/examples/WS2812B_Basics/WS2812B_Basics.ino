#include "WS2812B.h"

#define NUM_OF_LEDS 8
// There are mutiple choice of SPI_MOSI pins depends on different boards. The default is SPI_MOSI/SPI1_MOSI
// AMB21/AMB22              D11/D21
// AMB23                    D9/D4
// BW16/BW16 Type C         D12
// AW-CU488 ThingPlus       D1/D14
WS2812B led(SPI_MOSI, NUM_OF_LEDS);

void setup() {
    Serial.begin(115200);
    Serial.println("WS2812B test");
    led.begin();
    //Set a specific LED with a certain color
    led.setPixelColor(0, 50, 0, 0);
    led.setPixelColor(1, 0, 50, 0);
    led.setPixelColor(2, 0, 0, 50);
    led.show();
    delay(1000);
    //Fill the entire LED strip with the same color
    led.fill(60, 0, 25, 0, 8);
    led.show();
}

void loop() {
    delay(500);
}
