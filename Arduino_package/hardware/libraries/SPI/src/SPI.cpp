/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "spi_api.h"
#include "spi_ex_api.h"
#include "PinNames.h"
#include "cmsis_os.h"

//extern void log_uart_enable_printf(void);
//extern void log_uart_disable_printf(void);

#ifdef __cplusplus
}
#endif

spi_t spi_obj0;
spi_t spi_obj1;

SPIClass::SPIClass(void *pSpiObj, int mosi, int miso, int clk, int ss)
{
    pSpiMaster = pSpiObj;

    /* These 4 pins should belong same spi pinmux*/
    pinMOSI = (PinName)g_APinDescription[mosi].pinname;
    pinMISO = (PinName)g_APinDescription[miso].pinname;
    pinCLK = (PinName)g_APinDescription[clk].pinname;
    pinSS = (PinName)g_APinDescription[ss].pinname;

    pinUserSS = -1;
    initStatus = false;
    dataBits = 8;           // default databits is 8 bits
    dataMode = SPI_MODE0;   // default datamode is mode 0

#if defined(BOARD_RTL8721DM)
    defaultFrequency = 2000000;
#else
    defaultFrequency = 20000000;
#endif

    SPI_Mode = 'master';
}

void SPIClass::beginTransaction(uint8_t pin, SPISettings settings) {
    (void)pin;
    bitOrder = settings._bitOrder;
    spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 0);
    spi_frequency((spi_t *)pSpiMaster, settings._clock);

    //log_uart_disable_printf();
    // comment out to avoid error log while using tft_eSPI library
    // pinUserSS = pin;
    // pinMode(pinUserSS, OUTPUT);
    // digitalWrite(pinUserSS, 0);

    //log_uart_enable_printf();
}

void SPIClass::beginTransaction(SPISettings settings) {
    beginTransaction(pinSS, settings);
}

void SPIClass::endTransaction(void) {
    // TBD: comment out to avoid error log while using tft_eSPI library
    // if (pinUserSS >= 0) {
    //     digitalWrite(pinUserSS, 1);
    //     pinUserSS = -1;
    // }
}

void SPIClass::begin(void) {
    if (pinMOSI == PA_16 || pinMOSI == PB_18) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
    } else if (pinMOSI == PA_12 || pinMOSI == PB_4) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
    } else {
        printf("SPI begin: error. wrong spi_idx \r\n");
        return;
    }

    spi_init(
        (spi_t *)pSpiMaster, 
        (PinName)pinMOSI, 
        (PinName)pinMISO, 
        (PinName)pinCLK, 
        (PinName)pinSS
    );
    spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 0);
    spi_frequency((spi_t *)pSpiMaster, defaultFrequency);

    // Mark SPI init status
    initStatus = true;
}

void SPIClass::begin(int ss) {
    pinSS = (PinName)g_APinDescription[ss].pinname;

    if (pinMOSI == PA_16 || pinMOSI == PB_18) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
    } else if (pinMOSI == PA_12 || pinMOSI == PB_4) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
    } else {
        printf("SPI begin: error. wrong spi_idx \r\n");
        return;
    }

    spi_init(
        (spi_t *)pSpiMaster, 
        (PinName)pinMOSI, 
        (PinName)pinMISO, 
        (PinName)pinCLK, 
        (PinName)pinSS
    );
    spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 0);
    spi_frequency((spi_t *)pSpiMaster, defaultFrequency);

    // Mark SPI init status
    initStatus = true;
}

void begin(char mode) {
    SPI_Mode = mode;
    if (SPI_Mode == 'master') {
        begin();
    } else if (SPI_Mode == 'slave') {
        if (pinMOSI == PA_16 || pinMOSI == PB_18) {
            ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
        } else if (pinMOSI == PA_12 || pinMOSI == PB_4) {
            //((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
            printf("SPI begin: error. SPI1 does not support slave mode \r\n");
            return;
        } else {
            printf("SPI begin: error. wrong spi_idx \r\n");
            return;
        }

        spi_init(
            (spi_t *)pSpiMaster, 
            (PinName)pinMOSI, 
            (PinName)pinMISO, 
            (PinName)pinCLK, 
            (PinName)pinSS
        );
        spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 1);
        //spi_frequency((spi_t *)pSpiMaster, defaultFrequency);

        // Mark SPI init status
        initStatus = true;
    } else {
        printf("SPI begin: error. SPI mode \r\n");
        return;
    }
}

void begin(int ss, char mode) {
    SPI_Mode = mode;
    if (SPI_Mode == 'master') {
        begin(ss);
    } else if (SPI_Mode == 'slave') {
        pinSS = (PinName)g_APinDescription[ss].pinname;

        if (pinMOSI == PA_16 || pinMOSI == PB_18) {
            ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
        } else if (pinMOSI == PA_12 || pinMOSI == PB_4) {
            //((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
            printf("SPI begin: error. SPI1 does not support slave mode \r\n");
            return;
        } else {
            printf("SPI begin: error. wrong spi_idx \r\n");
            return;
        }

        spi_init(
            (spi_t *)pSpiMaster, 
            (PinName)pinMOSI, 
            (PinName)pinMISO, 
            (PinName)pinCLK, 
            (PinName)pinSS
        );
        spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 1);
        //spi_frequency((spi_t *)pSpiMaster, defaultFrequency);

        // Mark SPI init status
        initStatus = true;
    } else {
        printf("SPI begin: error. SPI mode \r\n");
        return;
    }
}

void SPIClass::end(void)
{
    spi_free((spi_t *)pSpiMaster);
    
    // Mark SPI init status
    initStatus = false;
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {
    byte d;
    u8 spi_Index;
    u32 spi_addr;

    spi_Index = ((spi_t *)pSpiMaster)->spi_idx;
    //spi_addr = 0x40042000 + (spi_Index * SSI_REG_OFF);
    if (spi_Index == MBED_SPI0) {
        spi_addr = SPI0_REG_BASE;
    } else if (spi_Index == MBED_SPI1) {
        spi_addr = SPI1_REG_BASE;
    } else {
        printf("error: wrong spi_idx \r\n");
        return 0;
    }

    if (_pin != pinSS) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, 0);
    }

    while (!(HAL_READ32(spi_addr, 0x28) & 0x0000002));
    HAL_WRITE32(spi_addr, 0x60, (_data & 0xFFFF));
    while (!(HAL_READ32(spi_addr, 0x28) & 0x0000008));
    d = HAL_READ32(spi_addr, 0x60);

    if ((_pin != pinSS) && (_mode == SPI_LAST)) {
        digitalWrite(_pin, 1);
    }

    return d;
}

byte SPIClass::transfer(uint8_t _data, SPITransferMode _mode) {
    (void)_mode;

    byte d;
    u8 spi_Index;
    u32 spi_addr;

    spi_Index = ((spi_t *)pSpiMaster)->spi_idx;
    //spi_addr = 0x40042000 + (spi_Index * SSI_REG_OFF);
    if (spi_Index == MBED_SPI0) {
        spi_addr = SPI0_REG_BASE;
    } else if (spi_Index == MBED_SPI1) {
        spi_addr = SPI1_REG_BASE;
    } else {
        printf("error: wrong spi_idx \r\n");
        return 0;
    }

    while (!(HAL_READ32(spi_addr, 0x28) & 0x0000002));
    HAL_WRITE32(spi_addr, 0x60, _data & 0xFFFF);
    while (!(HAL_READ32(spi_addr, 0x28) & 0x0000008));
    d = HAL_READ32(spi_addr, 0x60);

    return d;
}

void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode) {
    if (_pin != pinSS) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, 0);
    }

    spi_master_write_read_stream((spi_t *)pSpiMaster, (char *)_buf, (char *)_buf, (uint32_t)_count);

    if ((_pin != pinSS) && (_mode == SPI_LAST)) {
        digitalWrite(_pin, 1);
    }
}

void SPIClass::transfer(void *_buf, size_t _count, SPITransferMode _mode) {
    transfer(pinSS, _buf, _count, _mode);
}

uint16_t SPIClass::transfer16(byte _pin, uint16_t _data, SPITransferMode _mode) {
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;
    t.val = _data;

    if (bitOrder == LSBFIRST) {
        t.lsb = transfer(_pin, t.lsb, SPI_CONTINUE);
        t.msb = transfer(_pin, t.msb, _mode);
    } else {
        t.msb = transfer(_pin, t.msb, SPI_CONTINUE);
        t.lsb = transfer(_pin, t.lsb, _mode);
    }
    _data = t.val;
    return _data;
}

uint16_t SPIClass::transfer16(uint16_t _data, SPITransferMode _mode) {
    return transfer16(pinSS, _data, _mode);
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {
    (void)_pin;

    bitOrder = _bitOrder;
}

void SPIClass::setBitOrder(BitOrder _order) {
    setBitOrder(pinSS, _order);
}

// bits: data frame size, 4-16 supported.
void SPIClass::setDataMode(uint8_t _bits, uint8_t _mode) {
    dataBits = _bits;
    dataMode = _mode;
    if(initStatus) {
        spi_format((spi_t *)pSpiMaster, dataBits, dataMode, 0);
    }
}

void SPIClass::setDataMode(uint8_t _mode) {
    setDataMode(dataBits, _mode);
}


void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {
    (void)_pin;
    (void)_divider;

    // no effect on Ameba
}


void SPIClass::setClockDivider(uint8_t _div) {
    (void)_div;

    // no effect on Ameba
}

void SPIClass::setDefaultFrequency(int _frequency) {
    defaultFrequency = _frequency;
    if(initStatus) {
        spi_frequency((spi_t *)pSpiMaster, defaultFrequency);
    }
}


#if defined(BOARD_RTL8722DM)
SPIClass SPI((void *)(&spi_obj0), SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_SS);        // 11, 12, 13, 10
SPIClass SPI1((void *)(&spi_obj1), SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SS);   // 21, 20, 19, 18

#elif defined(BOARD_RTL8722DM_MINI)
SPIClass SPI((void *)(&spi_obj0), SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_SS);        // 9, 10, 11, 12 or 4, 5, 6, 7

#elif defined(BOARD_RTL8720DN_BW16)
SPIClass SPI((void *)(&spi_obj0), SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_SS);        // PA12, PA13, PA14, PA15

#elif defined(BOARD_RTL8721DM)
SPIClass SPI((void *)(&spi_obj0), SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_SS);        // 1, 2, 0, 8
SPIClass SPI1((void *)(&spi_obj1), SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SS);   // 14, 15, 16, 17 or 4, 3, 29, 28 

#elif defined(BOARD_RTL8720DF)
SPIClass SPI((void *)(&spi_obj0), SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_SS);        // 17, 16, 19, 18
SPIClass SPI1((void *)(&spi_obj1), SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SS);   // 3, 2, 1, 0

#else
#error check the board supported SPI
#endif
