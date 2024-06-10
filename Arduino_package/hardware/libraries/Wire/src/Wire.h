/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@arduino.cc>
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TwoWire_h
#define TwoWire_h

#include "Stream.h"

#define BUFFER_LENGTH 128

// Ameba D support 1 set of I2C in both master and slave mode

typedef void(*user_onRequest)(void);
typedef void(*user_onReceive)(int);

class TwoWire : public Stream {
    public:
        TwoWire(uint32_t dwSDAPin, uint32_t dwSCLPin);
        void begin();
        void begin(uint8_t);
        void begin(int);
        void end();
        void setClock(uint32_t);
        void beginTransmission(uint8_t);
        void beginTransmission(int);
        uint8_t endTransmission(void);
        uint8_t endTransmission(uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
        uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
        uint8_t requestFrom(int, int);
        uint8_t requestFrom(int, int, int);
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *, size_t);
        virtual int available(void);
        virtual int read(void);
        virtual int peek(void);
        virtual void flush(void);
        void onReceive(void(*)(int));
        void onRequest(void(*)(void));

        inline size_t write(unsigned long n) { return write((uint8_t)n); }
        inline size_t write(long n) { return write((uint8_t)n); }
        inline size_t write(unsigned int n) { return write((uint8_t)n); }
        inline size_t write(int n) { return write((uint8_t)n); }
        using Print::write;
        size_t slaveWrite(int);
        size_t slaveWrite(char *);
        size_t slaveWrite(uint8_t *, size_t);

    private:
        bool is_slave;

        // RX Buffer
        uint8_t rxBuffer[BUFFER_LENGTH];
        uint8_t rxBufferIndex;
        uint8_t rxBufferLength;

        // TX Buffer
        uint8_t txAddress;
        uint8_t txBuffer[BUFFER_LENGTH];
        uint8_t txBufferLength;

        // Callback user functions
        void (*user_onRequest)(void);
        void (*user_onReceive)(int);
        static void onRequestService(void *);
        static void onReceiveService(uint8_t*, size_t, bool, void *);

        uint32_t SDA_pin;
        uint32_t SCL_pin;

        void*    pI2C;

        // TWI clock frequency
        static const uint32_t TWI_CLOCK = 100000;
        uint32_t twiClock;

        // Timeouts
        static const uint32_t RECV_TIMEOUT = 50;
        static const uint32_t XMIT_TIMEOUT = 100000;
};

#if defined(BOARD_AMB21_AMB22)
extern TwoWire Wire;
extern TwoWire Wire1;
#elif defined(BOARD_AMB23)
extern TwoWire Wire;
extern TwoWire Wire1;
#elif defined(BOARD_AITHINKER_BW16)
extern TwoWire Wire;
#elif defined(BOARD_SPARKFUN_AWCU488)
extern TwoWire Wire;
#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306)
extern TwoWire Wire;
#endif

#endif
