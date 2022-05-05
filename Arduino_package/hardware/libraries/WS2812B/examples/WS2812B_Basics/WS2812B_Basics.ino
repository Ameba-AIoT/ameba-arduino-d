#include "WS2812B.h"

#if defined(BOARD_RTL8722DM)
#define SPI_MOSI_PIN        11
#elif defined(BOARD_RTL8722DM_MINI)
#define SPI_MOSI_PIN       9
#elif defined(BOARD_RTL8720DN_BW16)
#define SPI_MOSI_PIN       PA12
#endif

#define NUM_OF_LEDS  8

WS2812B led(SPI_MOSI_PIN,NUM_OF_LEDS);

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

