#include "WS2812B.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "spi_api.h"

#ifdef __cplusplus
}
#endif

spi_t spi_obj;

WS2812B::WS2812B(uint8_t input_pin, uint16_t num_leds) {
    pSpiMaster = (void *)(&spi_obj);
    _input_pin = input_pin;
    setLEDCount(num_leds);
}

void WS2812B::begin(void) {
    if ((PinName)g_APinDescription[_input_pin].pinname == PA_16 || (PinName)g_APinDescription[_input_pin].pinname == PB_18) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
        spi_addr = SPI0_REG_BASE;
    } else if ((PinName)g_APinDescription[_input_pin].pinname == PA_12 || (PinName)g_APinDescription[_input_pin].pinname == PB_4) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
        spi_addr = SPI1_REG_BASE;
    } else {
        printf("spi_init: error. wrong spi_idx \r\n");
        return;
    }
}

void WS2812B::sendPixel(uint8_t red ,uint8_t green ,uint8_t blue) {
    uint8_t grb_in_array[3] = {green, red, blue};
    //Set a bit pattern corresponding to RGB = 0,0,0 for WS2812B LED timings
    uint16_t grb_out_array[6] = {0x0924, 0x0924, 0x0924, 0x0924, 0x0924, 0x924};
    //Check the input colours bits and output it as a '1' or '0'.
    for (int i = 0; i < 6; i++) {
        //check the first four bits of each colours
        if ((i % 2) == 0) {
            grb_out_array[i] |= (grb_in_array[i/2] & 0x80) ? (0x0400) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x40) ? (0x0080) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x20) ? (0x0010) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x10) ? (0x0002) : (0);
        //check the last four bits of each colours
        } else {
            grb_out_array[i] |= (grb_in_array[i/2] & 0x08) ? (0x0400) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x04) ? (0x0080) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x02) ? (0x0010) : (0);
            grb_out_array[i] |= (grb_in_array[i/2] & 0x01) ? (0x0002) : (0);
        }
        spi_slave_write((spi_t *)pSpiMaster, grb_out_array[i]);
    }
}

void WS2812B::show(void) {
#if 0
    if (_input_pin == SPI_MOSI) {
    //Initialise SPI
        spi_init((spi_t *)pSpiMaster, (PinName)g_APinDescription[_input_pin].pinname, (PinName)g_APinDescription[SPI_MISO].pinname, (PinName)g_APinDescription[SPI_SCLK].pinname, (PinName)g_APinDescription[SPI_SS].pinname);
        //Revert the unneccesary SPI pins to GPIO functions
        Pinmux_Config((PinName)g_APinDescription[SPI_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI_SS].pinname, PINMUX_FUNCTION_GPIO);
    } else if (_input_pin == SPI1_MOSI) {
        spi_init((spi_t *)pSpiMaster, (PinName)g_APinDescription[_input_pin].pinname, (PinName)g_APinDescription[SPI1_MISO].pinname, (PinName)g_APinDescription[SPI1_SCLK].pinname, (PinName)g_APinDescription[SPI1_SS].pinname);
        //Revert the unneccesary SPI pins to GPIO functions
        Pinmux_Config((PinName)g_APinDescription[SPI1_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI1_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI1_SS].pinname, PINMUX_FUNCTION_GPIO);
    } else {
        printf("spi_init: error. wrong spi_idx \r\n");
        return;
    }
    spi_format((spi_t *)pSpiMaster, 12, 0, 0);
    spi_frequency((spi_t *)pSpiMaster, 2500000);
#else
    if (_input_pin == SPI_MOSI) {
        //Initialise SPI
        SPI.begin();
        //Revert the unneccesary SPI pins to GPIO functions
        Pinmux_Config((PinName)g_APinDescription[SPI_MISO].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI_SS].pinname, PINMUX_FUNCTION_GPIO);
        SPI.setDefaultFrequency(2500000);
        SPI.setDataMode(12, 0);
#if !defined(BOARD_RTL8722DM_MINI) && !defined(BOARD_RTL8720DN_BW16)
    } else if (_input_pin == SPI1_MOSI) {
        //Initialise SPI
        SPI1.begin();
        //Revert the unneccesary SPI pins to GPIO functions
        Pinmux_Config((PinName)g_APinDescription[SPI1_MISO].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI1_SCLK].pinname, PINMUX_FUNCTION_GPIO);
        Pinmux_Config((PinName)g_APinDescription[SPI1_SS].pinname, PINMUX_FUNCTION_GPIO);
        SPI1.setDefaultFrequency(2500000);
        SPI1.setDataMode(12, 0);
#endif
    } else {
        printf("spi_init: error. wrong spi_idx \r\n");
        return;
    }
#endif
    //Send Reset pulse of at least 50us duration
    for (uint8_t i = 0; i < reset_count; i++) {
        spi_slave_write((spi_t *)pSpiMaster, 0);
    }
    //Send RGB data
    for (uint16_t i = 0; i < _num_leds; i++) {
        sendPixel(_led_array[i].red, _led_array[i].green, _led_array[i].blue);
    }
    //Add a dummy bytes to ensure that the last bit of the data is sent out
    spi_slave_write((spi_t *)pSpiMaster, 0);
    //Ensure that Tx FIFO is empty before disable SPI
    while (!(HAL_READ32(spi_addr,0x28) & 0x04));
    spi_free((spi_t *)pSpiMaster);
}

void WS2812B::clear(void) {
    memset(_led_array, 0, _num_leds*sizeof(pixel));
}

void WS2812B::setLEDCount (uint16_t num_leds) {
    //Allocate memory for the total numbers of LEDs 
    free(_led_array);
    _led_array = (pixel*)realloc(_led_array, num_leds*sizeof(pixel));
    //Check if memory allocation is successful
    if (_led_array == NULL) {
        printf("Insufficient memory avaliable \r\n");
        _num_leds = 0;
    } else {
        memset(_led_array, 0, num_leds*sizeof(pixel));
        _num_leds = num_leds;
    }
}

void WS2812B::setPixelColor(uint16_t led_Number, uint8_t rColor, uint8_t gColor, uint8_t bColor) {
    //Verify that memory was successfully allocated
    if (_led_array == NULL) {
        printf("set LED count first \r\n");
        return;
    }
    if (led_Number < _num_leds) {
        _led_array[led_Number].red = rColor;
        _led_array[led_Number].green = gColor;
        _led_array[led_Number].blue = bColor;
    } else {
        printf("This LED does not exist \r\n");
    }
}

void WS2812B::fill(uint8_t rColor, uint8_t gColor, uint8_t bColor, uint16_t first, uint16_t count) {
    uint16_t i, end;
    
    //If first led is past end of strip, do nothing
    if (first >= _num_leds) {
        return;
    }
    if (count == 0 ) {
        //Fill up till the end of the strip
        end = _num_leds;
    } else {
        //Ensure that the last Pixels would not light up more than the LED that are present
        end = first + count ;
        if (end > _num_leds) {
            end = _num_leds;
        }
    }
    for (i = first; i < end ; i++) {
        setPixelColor(i, rColor,gColor,bColor);
        }
}

uint32_t WS2812B::colorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
    uint8_t r, g, b;

    hue = (hue * 1530L + 32768) /  65536;

    if (hue < 510) { // Red to Green-1
      b = 0;
      if (hue < 255) { //   Red to Yellow-1
        r = 255;
        g = hue;       //     g = 0 to 254
      } else {         //   Yellow to Green-1
        r = 510 - hue; //     r = 255 to 1
        g = 255;
      }
    } else if (hue < 1020) { // Green to Blue-1
      r = 0;
      if (hue < 765) { //   Green to Cyan-1
        g = 255;
        b = hue - 510;  //     b = 0 to 254
      } else {          //   Cyan to Blue-1
        g = 1020 - hue; //     g = 255 to 1
        b = 255;
      }
    } else if (hue < 1530) { // Blue to Red-1
      g = 0;
      if (hue < 1275) { //   Blue to Magenta-1
        r = hue - 1020; //     r = 0 to 254
        b = 255;
      } else { //   Magenta to Red-1
        r = 255;
        b = 1530 - hue; //     b = 255 to 1
      }
    } else { // Last 0.5 Red (quicker than % operator)
      r = 255;
      g = b = 0;
    }

    uint32_t v1 = 1 + val;
    uint16_t s1 = 1 + sat;
    uint8_t s2 = 255 - sat;

    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
        (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
        (((((b * s1) >> 8) + s2) * v1) >> 8);
}

void WS2812B::rainbow(uint16_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness) {
    for (uint16_t i=0; i<_num_leds; i++) {
        uint16_t hue = first_hue + (i * reps * 65536) / _num_leds;
        uint32_t color = colorHSV(hue, saturation, brightness);
        uint8_t r = (color & 0x00FF0000) >> 16;
        uint8_t g = (color & 0x0000FF00) >> 8;
        uint8_t b = (color & 0x000000FF);
        setPixelColor(i, r,g,b);
    }
}
