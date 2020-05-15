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
    pinMOSI = mosi;
    pinMISO = miso;
    pinCLK = clk;
    pinSS = ss;

    pinUserSS = -1;

    defaultFrequency = 20000000;
}

void SPIClass::beginTransaction(uint8_t pin, SPISettings settings)
{
    bitOrder = settings._bitOrder;
    spi_format((spi_t *)pSpiMaster, 8, settings._dataMode, 0);
    spi_frequency((spi_t *)pSpiMaster, settings._clock);

    //log_uart_disable_printf();

    pinUserSS = pin;
    pinMode(pinUserSS, OUTPUT);
    digitalWrite(pinUserSS, 0);

    //log_uart_enable_printf();
}

void SPIClass::beginTransaction(SPISettings settings)
{
    beginTransaction(pinSS, settings);
}

void SPIClass::endTransaction(void)
{
    if (pinUserSS >= 0) {
        digitalWrite(pinUserSS, 1);
        pinUserSS = -1;
    }
}

void SPIClass::begin(void)
{
    if (pinMOSI == 11) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
    } else if (pinMOSI == 21) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
    } else {
        printf("spi_init: error, wrong spi_idx \r\n");
        return;
    }
    
    spi_init(
        (spi_t *)pSpiMaster, 
        (PinName)g_APinDescription[pinMOSI].pinname, 
        (PinName)g_APinDescription[pinMISO].pinname, 
        (PinName)g_APinDescription[pinCLK].pinname, 
        (PinName)g_APinDescription[pinSS].pinname
    );
    spi_format((spi_t *)pSpiMaster, 8, 0, 0);
    spi_frequency((spi_t *)pSpiMaster, defaultFrequency);
}

void SPIClass::begin(int ss)
{
    if (pinMOSI == 11) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI0;
    } else if (pinMOSI == 21) {
        ((spi_t *)pSpiMaster)->spi_idx = MBED_SPI1;
    } else {
        printf("spi_init: error. wrong spi_idx \r\n");
        return;
    }

    spi_init(
        (spi_t *)pSpiMaster, 
        (PinName)g_APinDescription[pinMOSI].pinname, 
        (PinName)g_APinDescription[pinMISO].pinname, 
        (PinName)g_APinDescription[pinCLK].pinname, 
        (PinName)g_APinDescription[ss].pinname
    );
    spi_format((spi_t *)pSpiMaster, 8, 0, 0);
    spi_frequency((spi_t *)pSpiMaster, defaultFrequency);
}

void SPIClass::end(void)
{
    spi_free((spi_t *)pSpiMaster);
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode)
{
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

byte SPIClass::transfer(uint8_t _data, SPITransferMode _mode)
{
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

void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode)
{
    if (_pin != pinSS) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, 0);
    }

    spi_master_write_stream((spi_t *)pSpiMaster , (char *)_buf, (uint32_t)_count);

    if ((_pin != pinSS) && (_mode == SPI_LAST)) {
        digitalWrite(_pin, 1);
    }
}

void SPIClass::transfer(void *_buf, size_t _count, SPITransferMode _mode)
{
    transfer(pinSS, _buf, _count, _mode);
}

uint16_t SPIClass::transfer16(byte _pin, uint16_t _data, SPITransferMode _mode)
{
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;
    t.val = _data;

    if (bitOrder == LSBFIRST) {
        t.lsb = transfer(_pin, t.lsb, SPI_CONTINUE);
        t.msb = transfer(_pin, t.msb, _mode);
    } else {
        t.msb = transfer(_pin, t.msb, SPI_CONTINUE);
        t.lsb = transfer(_pin, t.lsb, _mode);
    }

    return _data;
}

uint16_t SPIClass::transfer16(uint16_t _data, SPITransferMode _mode)
{
    return transfer16(pinSS, _data, _mode);
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder)
{
    (void)_pin;

    bitOrder = _bitOrder;
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode)
{
    (void)_pin;

    spi_format((spi_t *)pSpiMaster, 8, _mode, 0);
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider)
{
    (void)_pin;
    (void)_divider;

    // no affact in Ameba
}

void SPIClass::setBitOrder(BitOrder _order)
{
    setBitOrder(pinSS, _order);
}

void SPIClass::setDataMode(uint8_t _mode)
{
    setDataMode(pinSS, _mode);
}

void SPIClass::setClockDivider(uint8_t _div)
{
    (void)_div;

    // no affact in Ameba
}

void SPIClass::setDefaultFrequency(int _frequency)
{
    defaultFrequency = _frequency;
}

SPIClass SPI((void *)(&spi_obj0), 11, 12, 13, 10);
SPIClass SPI1((void *)(&spi_obj1), 21, 20, 19, 18);
