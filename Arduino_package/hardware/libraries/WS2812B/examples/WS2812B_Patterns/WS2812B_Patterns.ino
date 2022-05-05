#include "WS2812B.h"

#if defined(BOARD_RTL8722DM)
#define SPI_MOSI_PIN        11
#elif defined(BOARD_RTL8722DM_MINI)
#define SPI_MOSI_PIN       9
#elif defined(BOARD_RTL8720DN_BW16)
#define SPI_MOSI_PIN       PA12
#endif

#define NUM_OF_LEDS  8

uint32_t rgbhue;

WS2812B led(SPI_MOSI_PIN,NUM_OF_LEDS);

void setup() {
    Serial.begin(115200);
    Serial.println("WS2812B test");
    led.begin();
}

void loop() {
    colorWipe(50, 0, 0, 50);
    colorWipe(0, 50, 0, 50);
    colorWipe(0, 0, 50, 50);

    theaterChase(0, 50, 50, 50);
    theaterChase(50, 0, 50, 50);
    theaterChase(50, 50, 0, 50);

    rainbow(20);

    theaterChaseRainbow(50);
}

void colorWipe(uint8_t rColor, uint8_t gColor, uint8_t bColor, uint8_t wait) {
    for (uint16_t i = 0; i < NUM_OF_LEDS; i++) {
        led.setPixelColor(i, rColor, gColor, bColor);
        led.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    led.rainbow(firstPixelHue);
    led.show();
    delay(wait);
  }
}

void theaterChase(uint8_t rColor, uint8_t gColor, uint8_t bColor, uint8_t wait) {
    for (uint8_t a = 0; a < 10; a++) {
        for(uint8_t b = 0; b < 3 ; b++) {
            led.clear();
            for(uint8_t c = b;c < NUM_OF_LEDS; c+=3) {
                led.setPixelColor(c,rColor,gColor,bColor);
            }
            led.show();
            delay(wait);
        }
    }
}

void theaterChaseRainbow(uint8_t wait) {
    //First pixel starts at red (hue 0)
    int firstPixelHue = 0;
    //Repeat 30 times...
    for(int a=0; a<30; a++) {
        //'b' counts from 0 to 2...
        for(int b=0; b<3; b++) {
            led.clear();
            //'c' counts up from 'b' to end of strip in increments of 3...
            for(int c=b; c < NUM_OF_LEDS; c += 3) {
                // hue of pixel 'c' is offset by an amount to make one full
                // revolution of the color wheel (range 65536) along the length
                // of the strip (strip.numPixels() steps):
                int      hue   = firstPixelHue + c * 65536L / NUM_OF_LEDS;
                uint32_t color = led.colorHSV(hue,255,255); // hue -> RGB
                uint8_t r = (color & 0x00FF0000) >> 16;
                uint8_t g = (color & 0x0000FF00) >> 8;
                uint8_t b = (color & 0x000000FF);
                led.setPixelColor(c, r,g,b);
            }
            led.show();
            delay(wait);
            // One cycle of color wheel over 90 frames
            firstPixelHue += 65536 / 90; 
        }
    }
}

