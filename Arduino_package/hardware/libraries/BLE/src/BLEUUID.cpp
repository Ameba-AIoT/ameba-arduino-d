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

#include <stdlib.h>
#include <string.h>

#include "BLEUUID.h"

BLEUUID::BLEUUID() {
}

BLEUUID::BLEUUID(const char* str) {
    char temp[] = {0, 0, 0};

    memset(_data, 0x00, sizeof(_data));

    _length = 0;
    uint8_t i;
    for (i = 0; (i <= (strlen(str) - 1)) && (_length < BLE_UUID_MAX_LENGTH); i += 2) {
        if (str[i] == '-') {
            i++;
            //continue;
        }

        temp[0] = str[i];
        temp[1] = str[i+1];

        _data[_length] = strtoul(temp, NULL, 16);

        _length++;
    }

    if (_length <= 2) {
        _length = 2;
    } else if (_length <= 4) {
        _length = 4;
    } else {
        _length = 16;
    }
    
    for (i = 0; i < _length; i++) {
        _dataNative[i] = _data[(_length - 1 - i)];
    }
}

BLEUUID::BLEUUID(uint8_t* data, uint8_t length) {
    if ((length == 2) || (length == 4) || (length == 16)) {
        _length = length;
        uint8_t i = 0;
        for (i = 0; i < _length; i++) {
            _dataNative[i] = data[i];
            _data[i] = data[(_length - 1 - i)];
        }
    }
}

const char* BLEUUID::str() {
    if (_length == 2) {
        sprintf(_str, "%2x%2x", 
                _dataNative[1], _dataNative[0]);
    }

    if (_length == 4) {
        sprintf(_str, "%2x%2x%2x%2x", 
                _dataNative[3], _dataNative[2],
                _dataNative[1], _dataNative[0]);
    }

    if (_length == 16) {
        sprintf(_str, "%2x%2x%2x%2x-%2x%2x-%2x%2x-%2x%2x-%2x%2x%2x%2x%2x%2x", 
                _dataNative[15], _dataNative[14],
                _dataNative[13], _dataNative[12],
                _dataNative[11], _dataNative[10],
                _dataNative[9], _dataNative[8],
                _dataNative[7], _dataNative[6],
                _dataNative[5], _dataNative[4],
                _dataNative[3], _dataNative[2],
                _dataNative[1], _dataNative[0]);
    }
    return _str;
}

const uint8_t* BLEUUID::data() {
    return _data;
}

const uint8_t* BLEUUID::dataNative() {
    return _dataNative;
}

uint8_t BLEUUID::length() {
    return _length;
}

bool BLEUUID::operator ==(const BLEUUID &uuid) {
    return(memcmp(_dataNative, uuid._dataNative, BLE_UUID_MAX_LENGTH) == 0);
}

