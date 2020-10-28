/*
  This file is part of the ArduinoBLE library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

// Modified by Realtek for use with Ameba development boards with the Arduino IDE

#ifndef _BLE_UUID_H_
#define _BLE_UUID_H_

#include <Arduino.h>

#define BLE_UUID_MAX_LENGTH 16

class BLEUUID {
    public: 
        BLEUUID();
        BLEUUID(const char* str);      // Build a BLEUUID object from an UUID string. Use of hyphens (-) to seperate UUID sections in the string is acceptable.
        BLEUUID(uint8_t* data, uint8_t length);
        const char* str();
        const uint8_t* data();
        const uint8_t* dataNative();
        uint8_t length();
        bool operator ==(const BLEUUID &uuid);

    private:
        char _str[BLE_UUID_MAX_LENGTH * 2 + 5] = {0};
        uint8_t _data[BLE_UUID_MAX_LENGTH] = {0};           // stores the UUID MSB in position 0, useful for printing as characters/strings
        uint8_t _dataNative[BLE_UUID_MAX_LENGTH] = {0};     // stores the UUID LSB in position 0
        uint8_t _length = 0;    // number of bytes(characters) of UUID
};

#endif
