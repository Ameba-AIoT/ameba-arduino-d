#ifndef _SPI_WS2812B_H_
#define _SPI_WS2812B_H_

#include <Arduino.h>
#include "SPI.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel;

class WS2812B {
    public:
        WS2812B(uint8_t input_pin, uint16_t num_leds);
        void begin(void);
        void show(void);
        void clear(void);
        void setLEDCount(uint16_t num_leds);
        void setPixelColor(uint16_t led_Number, uint8_t rColour, uint8_t bColour, uint8_t gColour);
        void fill(uint8_t rColor, uint8_t gColor, uint8_t bColor, uint16_t first = 0, uint16_t count = 0);
        uint32_t colorHSV(uint16_t hue, uint8_t sat, uint8_t val);
        void rainbow(uint16_t first_hue = 0, int8_t reps = 1, uint8_t saturation = 255, uint8_t brightness = 60);

    private:
        void sendPixel(uint8_t red ,uint8_t green ,uint8_t blue);
        void *pSpiMaster;
        uint32_t spi_addr = 0;
        uint8_t reset_count = 12;
        uint16_t _num_leds = 0;
        uint16_t led_Number = 0;
        pixel* _led_array = NULL;
        uint8_t _input_pin = 0;
};

#if defined(BOARD_AMB21_AMB22)
extern SPIClass SPI;
extern SPIClass SPI1;
#elif defined(BOARD_AMB23)
extern SPIClass SPI;
#elif defined(BOARD_AITHINKER_BW16)
extern SPIClass SPI;
#elif defined(BOARD_SPARKFUN_AWCU488)
extern SPIClass SPI;
extern SPIClass SPI1;
#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306)
extern SPIClass SPI;
extern SPIClass SPI1;
#else
#error check the board supported
#endif

#endif
