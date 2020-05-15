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

#include "Arduino.h"
#include "Wire.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "PinNames.h"
#include "i2c_api.h"

i2c_t i2cwire0;
i2c_t i2cwire1;

#ifdef __cplusplus
}
#endif

TwoWire::TwoWire (uint32_t dwSDAPin, uint32_t dwSCLPin) {
    this->SDA_pin = dwSDAPin;
    this->SCL_pin = dwSCLPin;

    if ((dwSDAPin == PA_26) && (dwSCLPin == PA_25)) {
        this->pI2C = (void *)&i2cwire0;
    } else if ((dwSDAPin == PB_6) && (dwSCLPin == PB_5)) {
        this->pI2C = (void *)&i2cwire1;
    } else {
        printf("Invalid I2C pin\r\n");
    }
}

void TwoWire::begin () {
    this->rxBufferIndex = 0;
    this->rxBufferLength = 0;
    this->txAddress = 0;
    this->txBufferLength = 0;
    this->srvBufferIndex = 0;
    this->srvBufferLength = 0;
    this->status = UNINITIALIZED;
    this->twiClock = this->TWI_CLOCK;

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);

    status = MASTER_IDLE;
}

void TwoWire::begin (uint8_t address = 0) {
    this->rxBufferIndex = 0;
    this->rxBufferLength = 0;
    this->txAddress = 0;
    this->txBufferLength = 0;
    this->srvBufferIndex = 0;
    this->srvBufferLength = 0;
    this->status = UNINITIALIZED;
    this->twiClock = this->TWI_CLOCK;

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
    i2c_slave_address(((i2c_t *)this->pI2C), 0, address, 0xFF);
    i2c_slave_mode(((i2c_t *)this->pI2C), 1);

    status = SLAVE_IDLE;
}

void TwoWire::begin (int address) {
    begin((uint8_t)address);
}

void TwoWire::setClock(uint32_t frequency) {
    twiClock = frequency;
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

uint8_t TwoWire::requestFrom (uint8_t address, uint8_t quantity, uint8_t sendStop) {
    int readed = 0;

    if (quantity > BUFFER_LENGTH) {
        quantity = BUFFER_LENGTH;
    }

    // perform blocking read into buffer
    readed = i2c_read(((i2c_t *)this->pI2C), ((int)address), ((char*)&this->rxBuffer[0]), ((int)quantity), ((int)sendStop));

    // i2c_read error;
    if (readed != quantity) {
        printf("requestFrom: readed=%d, quantity=%d : ERROR\n", readed, quantity);
        return readed;
    }

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = readed;

    return readed;
}

uint8_t TwoWire::requestFrom (uint8_t address, uint8_t quantity) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)true));
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)true));
}

uint8_t TwoWire::requestFrom (int address, int quantity, int sendStop) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)sendStop));
}

void TwoWire::beginTransmission (uint8_t address) {
    status = MASTER_SEND;

    // save address of target and empty buffer
    txAddress = address;
    txBufferLength = 0;
}

void TwoWire::beginTransmission (int address) {
    beginTransmission(((uint8_t)address));
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission (uint8_t sendStop) {
    int length;
    uint8_t error = 0;

    length = i2c_write(((i2c_t *)this->pI2C), ((int)this->txAddress), ((const char*)&this->txBuffer[0]), ((int)this->txBufferLength), ((int)sendStop));
    if ((txBufferLength > 0) && (length <= 0)) {
        error = 1;
    }

    txBufferLength = 0;     // empty buffer
    status = MASTER_IDLE;
    return error;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission (void) {
    return endTransmission(true);
}

size_t TwoWire::write (uint8_t data) {
    if (status == MASTER_SEND) {
        if (txBufferLength >= BUFFER_LENGTH) {
            return 0;
        }
        txBuffer[txBufferLength++] = data;
        return 1;
    } else {
        if (srvBufferLength >= BUFFER_LENGTH) {
            return 0;
        }
        srvBuffer[srvBufferLength++] = data;
        return 1;
    }
}

size_t TwoWire::write (const uint8_t *data, size_t quantity) {
    if (status == MASTER_SEND) {
        for (size_t i = 0; i < quantity; ++i) {
            if (txBufferLength >= BUFFER_LENGTH) {
                return i;
            }
            txBuffer[txBufferLength++] = data[i];
        }
    } else {
        for (size_t i = 0; i < quantity; ++i) {
            if (srvBufferLength >= BUFFER_LENGTH) {
                return i;
            }
            srvBuffer[srvBufferLength++] = data[i];
        }
    }
    return quantity;
}

int TwoWire::available (void) {
    return (rxBufferLength - rxBufferIndex);
}

int TwoWire::read (void) {
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex++];
    }
    return -1;
}

int TwoWire::peek (void) {
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex];
    }
    return -1;
}

void TwoWire::flush (void) {
    // Do nothing, use endTransmission(..) to force
    // data transfer.
}

void TwoWire::onReceive (void(*function)(int)) {
    onReceiveCallback = function;
}

void TwoWire::onRequest (void(*function)(void)) {
    onRequestCallback = function;
}

//#if defined(BOARD_RTL8195A)
//#if defined(BOARD_RTL8711AM)
//TwoWire Wire = TwoWire(PD_4, PD_5);
//#else
//// HW: I2C1
//TwoWire Wire  = TwoWire(PD_7, PD_6);
////TwoWire Wire  = TwoWire(PC_4, PC_5);
//// HW: I2C0
//TwoWire Wire1 = TwoWire(PD_4, PD_5);
// HW: I2C3
//TwoWire Wire2 = TwoWire(PB_3, PB_2);
//#endif
//#elif defined(BOARD_RTL8710)
//TwoWire Wire  = TwoWire(PC_4, PC_5);
//#else
//#endif

#if defined(BOARD_RTL8721D)

// SDA SCL
// HW: I2C0
TwoWire Wire  = TwoWire(PA_26, PA_25);

// HW: I2C1
TwoWire Wire1 = TwoWire(PB_6, PB_5);

#endif
