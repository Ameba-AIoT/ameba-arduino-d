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

#ifndef _SPI_H_
#define _SPI_H_

#include "variant.h"
#include <stdio.h>
#include <inttypes.h>

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_EXTENDED_CS_PIN_HANDLING means SPI has automatic 
// CS pin handling and provides the following methods:
//   - begin(pin)
//   - end(pin)
//   - setBitOrder(pin, bitorder)
//   - setDataMode(pin, datamode)
//   - setClockDivider(pin, clockdiv)
//   - transfer(pin, data, SPI_LAST/SPI_CONTINUE)
//   - beginTransaction(pin, SPISettings settings) (if transactions are available)
#define SPI_HAS_EXTENDED_CS_PIN_HANDLING 1

#define SPI_DATA_MODE0      0x00
#define SPI_DATA_MODE1      0x01
#define SPI_DATA_MODE2      0x02
#define SPI_DATA_MODE3      0x03

#define SPI_MODE_MASTER     'M'
#define SPI_MODE_SLAVE      'S'

enum SPITransferMode {
    SPI_CONTINUE,
    SPI_LAST
};

class SPISettings {
    public:
        SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
            _clock = clock;
            _bitOrder = bitOrder;
            _dataMode = dataMode;
        }
        SPISettings() {
            SPISettings(4000000, MSBFIRST, SPI_DATA_MODE0);
        }

    private:
        uint32_t _clock;
        BitOrder _bitOrder;
        uint8_t _dataMode;

        friend class SPIClass;
};

class SPIClass {
    public:
        SPIClass(void *pSpiObj, int mosi, int miso, int clk, int ss);

        byte transfer(byte _pin, uint8_t _data, SPITransferMode _mode = SPI_LAST);
        byte transfer(uint8_t _data, SPITransferMode _mode = SPI_LAST);

        void transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode = SPI_LAST);
        void transfer(void *_buf, size_t _count, SPITransferMode _mode = SPI_LAST);

        uint16_t transfer16(byte _pin, uint16_t _data, SPITransferMode _mode = SPI_LAST);
        uint16_t transfer16(uint16_t _data, SPITransferMode _mode = SPI_LAST);

        void beginTransaction(uint8_t pin, SPISettings settings);
        void beginTransaction(SPISettings settings);
        void endTransaction(void);

        void begin(void);
        void begin(int ss);
        void begin(char mode);
        void begin(int ss, char mode);
        void end(void);

        void setBitOrder(uint8_t _pin, BitOrder _bitOrder);
        void setBitOrder(BitOrder _order);

        void setDataMode(uint8_t _bits, uint8_t _mode);
        void setDataMode(uint8_t _mode);

        void setClockDivider(uint8_t _pin, uint8_t _divider);
        void setClockDivider(uint8_t _div);

        /* extend api added by RTK */
        void setDefaultFrequency(int _frequency);

    private:
        void *pSpiMaster;
        int pinMOSI;
        int pinMISO;
        int pinCLK;
        int pinSS;
        int pinUserSS;
        BitOrder bitOrder;
        bool initStatus;   // flag to mark SPI init status
        u8 dataBits;
        u8 dataMode;
        int defaultFrequency;
        char SPI_Mode;
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
#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30)
extern SPIClass SPI;
extern SPIClass SPI1;
#else
#error check the board supported
#endif

#endif
