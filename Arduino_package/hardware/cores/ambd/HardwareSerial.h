/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"

// Define config for Serial.begin(baud, config);
//#define SERIAL_5N1 0x00
//#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
//#define SERIAL_5N2 0x08
//#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
//#define SERIAL_5E1 0x20
//#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
//#define SERIAL_5E2 0x28
//#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
//#define SERIAL_5O1 0x30
//#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
//#define SERIAL_5O2 0x38
//#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

//#define SERIAL_511 0x40
//#define SERIAL_611 0x42
#define SERIAL_711 0x44
#define SERIAL_811 0x46
//#define SERIAL_512 0x48
//#define SERIAL_612 0x4A
#define SERIAL_712 0x4C
#define SERIAL_812 0x4E
//#define SERIAL_501 0x50
//#define SERIAL_601 0x52
#define SERIAL_701 0x54
#define SERIAL_801 0x56
//#define SERIAL_502 0x58
//#define SERIAL_602 0x5A
#define SERIAL_702 0x5C
#define SERIAL_802 0x5E

class HardwareSerial : public Stream
{
    public:
        virtual void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
        virtual void begin(unsigned long, uint8_t) = 0;
        virtual void end() = 0;
        virtual int available(void) = 0;
        virtual int peek(void) = 0;
        virtual int read(void) = 0;
        virtual void flush(void) = 0;
        virtual size_t write(uint8_t) = 0;
        using Print::write; // pull in write(str) and write(buf, size) from Print
        virtual operator bool() = 0;
};

extern void serialEventRun(void) __attribute__((weak));

#endif
