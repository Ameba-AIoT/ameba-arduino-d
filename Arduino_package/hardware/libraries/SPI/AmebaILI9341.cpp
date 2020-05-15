#include "AmebaILI9341.h"
#include "SPI.h"

#include "font5x7.h"

#include <inttypes.h>

AmebaILI9341::AmebaILI9341(int csPin, int dcPin, int resetPin)
{
    _csPin = csPin; // TODO: no effect now, use pin 10 as default
    _dcPin = dcPin;
    _resetPin = resetPin;

    _dcPort = _dcMask = 0;

    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;

    cursor_x = 0;
    cursor_y = 0;
    foreground = ILI9341_WHITE;
    background = ILI9341_BLACK;
    fontsize = 1;
    rotation = 0;
}

void AmebaILI9341::begin(void)
{
    pinMode(_resetPin, OUTPUT);
    digitalWrite(_resetPin, LOW);

    pinMode(_dcPin, OUTPUT);
    _dcPort = digitalPinToPort(_dcPin);
    _dcMask = digitalPinToBitMask(_dcPin);

    SPI.begin();

    reset();

    writecommand(0xEF);
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);
    writedata(0x00);
    writedata(0XC1);
    writedata(0X30);

    writecommand(0xED);
    writedata(0x64);
    writedata(0x03);
    writedata(0X12);
    writedata(0X81);

    writecommand(0xE8);
    writedata(0x85);
    writedata(0x00);
    writedata(0x78);

    writecommand(0xCB);
    writedata(0x39);
    writedata(0x2C);
    writedata(0x00);
    writedata(0x34);
    writedata(0x02);

    writecommand(0xF7);
    writedata(0x20);

    writecommand(0xEA);
    writedata(0x00);
    writedata(0x00);

    writecommand(ILI9341_PWCTR1);    //Power control
    writedata(0x23);

    writecommand(ILI9341_PWCTR2);    //Power control
    writedata(0x10);

    writecommand(ILI9341_VMCTR1);    //VCM control
    writedata(0x3e);
    writedata(0x28); 

    writecommand(ILI9341_VMCTR2);    //VCM control2
    writedata(0x86);

    writecommand(ILI9341_MADCTL);    //Memory Access Control
    writedata(0x48);

    writecommand(ILI9341_PIXFMT);
    writedata(0x55);

    writecommand(ILI9341_FRMCTR1);
    writedata(0x00);
    writedata(0x18);

    writecommand(ILI9341_DFUNCTR);     //Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(0xF2);                //3Gamma Function Disable
    writedata(0x00);

    writecommand(ILI9341_GAMMASET);    //Gamma curve selected
    writedata(0x01);

    writecommand(ILI9341_GMCTRP1);    //Set Gamma
    writedata(0x0F);
    writedata(0x31);
    writedata(0x2B);
    writedata(0x0C);
    writedata(0x0E);
    writedata(0x08);
    writedata(0x4E);
    writedata(0xF1);
    writedata(0x37);
    writedata(0x07);
    writedata(0x10);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x09);
    writedata(0x00);

    writecommand(ILI9341_GMCTRN1);    //Set Gamma 
    writedata(0x00);
    writedata(0x0E);
    writedata(0x14);
    writedata(0x03);
    writedata(0x11);
    writedata(0x07);
    writedata(0x31);
    writedata(0xC1);
    writedata(0x48);
    writedata(0x08);
    writedata(0x0F);
    writedata(0x0C);
    writedata(0x31);
    writedata(0x36);
    writedata(0x0F);

    writecommand(ILI9341_SLPOUT);    //Exit Sleep 

    delay(120);

    writecommand(ILI9341_DISPON);    //Display on 
}

void AmebaILI9341::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t x, y, w, h;

    if (x1 > x0) {
        x = x0;
        w = x1 - x0;
    } else {
        x = x1;
        w = x0 - x1;
    }

    if (y1 > y0) {
        y = y0;
        h = y1 - y0;
    } else {
        y = y1;
        h = y0 - y1;
    }

    *portOutputRegister(_dcPort) &= ~(_dcMask);
    SPI.transfer(ILI9341_CASET);
    *portOutputRegister(_dcPort) |=  (_dcMask);
    SPI.transfer(x >> 8);
    SPI.transfer(x & 0xFF);
    SPI.transfer((x+w) >> 8);
    SPI.transfer((x+w) & 0xFF);

    *portOutputRegister(_dcPort) &= ~(_dcMask);
    SPI.transfer(ILI9341_PASET);
    *portOutputRegister(_dcPort) |=  (_dcMask);
    SPI.transfer(y >> 8);
    SPI.transfer(y & 0xFF);
    SPI.transfer((y+h) >> 8);
    SPI.transfer((y+h) & 0xFF);

    *portOutputRegister(_dcPort) &= ~(_dcMask);
    SPI.transfer(ILI9341_RAMWR);
}

void AmebaILI9341::writecommand(uint8_t command)
{
    *portOutputRegister(_dcPort) &= ~(_dcMask);
    SPI.transfer(command);
}

void AmebaILI9341::writedata(uint8_t data)
{
    *portOutputRegister(_dcPort) |=  (_dcMask);
    SPI.transfer(data);
}

void AmebaILI9341::setRotation(uint8_t m)
{
    writecommand(ILI9341_MADCTL);
    rotation = m % 4;
    switch (rotation) {
        case 0:
            writedata(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            _width = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 1:
            writedata(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            _width = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
        case 2:
            writedata(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
            _width = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 3:
            writedata(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            _width = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
    }
}

void AmebaILI9341::fillScreen(uint16_t color)
{
    fillRectangle(0, 0, _width, _height, color);
}

void AmebaILI9341::clr(void)
{
    fillScreen(background);
}

void AmebaILI9341::fillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    uint8_t color_hi, color_lo;

    if((x >= _width) || (y >= _height)) {
        return;
    }

    if ((x + w - 1) >= _width) {
        w = _width - x;
    }

    if ((y + h - 1) >= _height) {
        h = _height - y;
    }

    setAddress(x, y, (x + w - 1), (y + h - 1));

    uint32_t pixelCount = h * w;
    uint32_t i;
    color_hi = color >> 8;
    color_lo = color & 0xFF;

    *portOutputRegister(_dcPort) |=  (_dcMask);
    for (i = 0; i < pixelCount; i++) {
        SPI.transfer(color_hi);
        SPI.transfer(color_lo);
    }
}

void AmebaILI9341::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
        return;
    }

    setAddress(x, y, (x + 1), (y + 1));
    *portOutputRegister(_dcPort) |=  (_dcMask);
    SPI.transfer(color >> 8);
    SPI.transfer(color & 0xFF);
}

void AmebaILI9341::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t temp;
    uint8_t color_hi;
    uint8_t color_lo;
    bool exchange_xy;
    int16_t idx, dx, dy, linelen, err, ystep;

    if (x0 > x1) {
        temp = x0; x0 = x1; x1 = temp;
        temp = y0; y0 = y1; y1 = temp;
    }

    color_hi = color >> 8;
    color_lo = color & 0xFF;

    if (x0 == x1) {
        // draw vertical line
        if (y0 < 0) {
            y0 = 0;
        }
        if (y1 < 0) {
            y1 = 0;
        }
        if (y0 >= _height) {
            y0 = _height;
        }
        if (y1 >= _height) {
            y1 = _height;
        }

        setAddress(x0, y0, x1, y1);
        *portOutputRegister(_dcPort) |=  (_dcMask);
        linelen = abs(y1-y0);
        for (idx = 0; idx < linelen; idx++) {
            SPI.transfer(color_hi);
            SPI.transfer(color_lo);
        }
    } else if (y0 == y1) {
        // draw horizontal line
        if (x0 < 0) {
            x0 = 0;
        }
        if (x1 < 0) {
            x1 = 0;
        }
        if (x0 >= _width) {
            x0 = _width-1;
        }
        if (x1 >= _width) {
            x1 = _width-1;
        }

        setAddress(x0, y0, x1, y1);
        *portOutputRegister(_dcPort) |=  (_dcMask);
        linelen = abs(x1 - x0);
        for (idx = 0; idx < linelen; idx++) {
            SPI.transfer(color_hi);
            SPI.transfer(color_lo);
        }
    } else {
        // Bresenham's line algorithm
        exchange_xy = (abs(y1 - y0) > (x1-x0)) ? true : false;
        if (exchange_xy) {
            temp = x0; x0 = y0; y0 = temp;
            temp = x1; x1 = y1; y1 = temp;
        }

        if (x0 > x1) {
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }
        
        dx = x1 - x0;
        dy = abs(y1 - y0);
        err = dx / 2;
        ystep = (y0 < y1) ? 1 : -1;

        for (; x0 <= x1; x0++) {
            if (exchange_xy) {
                drawPixel(y0, x0, color);
            } else {
                drawPixel(x0, y0, color);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
    }
}

void AmebaILI9341::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    drawLine(x0, y0, x1, y1, foreground);
}

void AmebaILI9341::drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    drawLine(x      , y      , x      , (y + h), color);
    drawLine(x      , y      , (x + w), y      , color);
    drawLine((x + w), y      , (x + w), (y + h), color);
    drawLine(x      , (y + h), (x + w), (y + h), color);
}

void AmebaILI9341::drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h)
{
    drawRectangle(x, y, w, h, foreground);
}

void AmebaILI9341::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0        , (y0 + r), color);
    drawPixel(x0        , (y0 - r), color);
    drawPixel((x0 + r)  , y0      , color);
    drawPixel((x0 - r)  , y0      , color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel((x0 + x), (y0 + y), color);
        drawPixel((x0 - x), (y0 + y), color);
        drawPixel((x0 + x), (y0 - y), color);
        drawPixel((x0 - x), (y0 - y), color);
        drawPixel((x0 + y), (y0 + x), color);
        drawPixel((x0 - y), (y0 + x), color);
        drawPixel((x0 + y), (y0 - x), color);
        drawPixel((x0 - y), (y0 - x), color);
    }
}

void AmebaILI9341::drawCircle(int16_t x0, int16_t y0, int16_t r)
{
    drawCircle(x0, y0, r, foreground);
}

size_t AmebaILI9341::write(uint8_t c)
{
    if (c == '\n') {
        cursor_y += fontsize * 8;
        cursor_x = 0;
    } else if (c == '\r') {

    } else {
        if ((cursor_x + fontsize * 6) >= _width) {
            // new line
            cursor_x = 0;
            cursor_y += fontsize * 8;
        }
        drawChar(c);
    }

    return 1;
}

int16_t AmebaILI9341::getWidth()
{
    return _width;
}

int16_t AmebaILI9341::getHeight()
{
    return _height;
}

void AmebaILI9341::setCursor(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

void AmebaILI9341::setForeground(uint16_t color)
{
    foreground = color;
}

void AmebaILI9341::setBackground(uint16_t _background)
{
    background = _background;
}

void AmebaILI9341::setFontSize(uint8_t size)
{
    fontsize = size;
}

void AmebaILI9341::drawChar(unsigned char c)
{
    drawChar(cursor_x, cursor_y, c, foreground, background, fontsize);
}

void AmebaILI9341::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t _fontcolor, uint16_t _background, uint8_t _fontsize)
{
    int i, j;
    uint8_t line;

    foreground = _fontcolor;
    background = _background;
    fontsize = _fontsize;

    if ((x >= _width) || (y >= _height) || (x + 6 * fontsize - 1) < 0 || (y + 8 * fontsize - 1) < 0) {
        return;
    }

    for (i = 0; i < 6; i++) {
        if (i < 5) {
            line = font5x7[(c * 5 + i)];
        } else {
            line = 0x00;
        }
        for (j = 0; j < 8; j++, line >>= 1) {
            if (line & 0x01) {
                if (fontsize == 1) {
                    drawPixel((x + i), (y + j), foreground);
                } else {
                    fillRectangle((x + i * fontsize), (y + j * fontsize), fontsize, fontsize, foreground);
                }
            } else if (background != foreground) {
                if (fontsize == 1) {
                    drawPixel((x + i), (y + j), background);
                } else {
                    fillRectangle((x + i * fontsize), (y + j * fontsize), fontsize, fontsize, background);
                }
            }
        }
    }

    // update cursor
    cursor_x += fontsize * 6;
    cursor_y = y;
}

void AmebaILI9341::reset(void)
{
    if (_resetPin > 0) {
        digitalWrite(_resetPin, HIGH);
        delay(5);
        digitalWrite(_resetPin, LOW);
        delay(20);
        digitalWrite(_resetPin, HIGH);
        delay(150);
    }
}
