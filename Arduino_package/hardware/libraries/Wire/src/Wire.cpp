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
#include "i2c_slave.h"

i2c_t i2cwire0;
i2c_t i2cwire1;

#ifdef __cplusplus
}
#endif

TwoWire::TwoWire (uint32_t dwSDAPin, uint32_t dwSCLPin) {
    this->SDA_pin = dwSDAPin;
    this->SCL_pin = dwSCLPin;
    this->user_onReceive = NULL;
    this->user_onRequest = NULL;
    this->is_slave = false;

    this->rxBufferIndex = 0;
    this->rxBufferLength = 0;
    this->txAddress = 0;
    this->txBufferLength = 0;
    this->twiClock = this->TWI_CLOCK;
}

void TwoWire::begin() {
    amb_ard_pin_check_fun(SDA_pin, PIO_I2C);
    amb_ard_pin_check_fun(SCL_pin, PIO_I2C);

    SDA_pin = (PinName)g_APinDescription[SDA_pin].pinname;
    SCL_pin = (PinName)g_APinDescription[SCL_pin].pinname;

    if ((SDA_pin == PA_26) || (SDA_pin == PB_0) || (SDA_pin == PB_6)) {
        if ((SCL_pin == PA_25) || (SCL_pin == PA_31) || (SCL_pin == PB_5)) {
            this->pI2C = (void *)&i2cwire0;
        } else {
            printf("Invalid I2C pin, SDA and SCL not in same group. \r\n");
        }
    } else if (SDA_pin == PA_24) {
        if (SCL_pin == PA_23) {
            this->pI2C = (void *)&i2cwire1;
        } else {
            printf("Invalid I2C pin, SDA and SCL not in same group. \r\n");
        }
    } else {
        printf("Invalid I2C pin\r\n");
    }

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

void TwoWire::begin(uint8_t address = 0) {
    amb_ard_pin_check_fun(SDA_pin, PIO_I2C);
    amb_ard_pin_check_fun(SCL_pin, PIO_I2C);

    SDA_pin = (PinName)g_APinDescription[SDA_pin].pinname;
    SCL_pin = (PinName)g_APinDescription[SCL_pin].pinname;

    if ((SDA_pin == PA_26) || (SDA_pin == PB_0) || (SDA_pin == PB_6)) {
        if ((SCL_pin == PA_25) || (SCL_pin == PA_31) || (SCL_pin == PB_5)) {
            this->pI2C = (void *)&i2cwire0;
        } else {
            printf("Invalid I2C pin, SDA and SCL not in same group. \r\n");
        }
    } else if (SDA_pin == PA_24) {
        if (SCL_pin == PA_23) {
            this->pI2C = (void *)&i2cwire1;
        } else {
            printf("Invalid I2C pin, SDA and SCL not in same group. \r\n");
        }
    } else {
        printf("Invalid I2C pin\r\n");
    }

    // Attach user callbacks 
    i2c_slave_attach_callbacks(onRequestService, onReceiveService, this);

    // Init I2C as slave and enable I2C interrupt
    i2c_slave_init((i2c_t *)this->pI2C, (PinName)this->SDA_pin, (PinName)this->SCL_pin, address, BUFFER_LENGTH);

    //status = SLAVE_IDLE;
    is_slave = true;
}

void TwoWire::begin(int address) {
    begin((uint8_t)address);
}

void TwoWire::end() {
    i2c_reset((i2c_t *)this->pI2C);
}

void TwoWire::setClock(uint32_t frequency) {
    twiClock = frequency;
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
    int read = 0;

    if (quantity > BUFFER_LENGTH) {
        quantity = BUFFER_LENGTH;
    }

    // perform blocking read into buffer
    read = i2c_read(((i2c_t *)this->pI2C), ((int)address), ((char*)&this->rxBuffer[0]), ((int)quantity), ((int)sendStop));

    // i2c_read error;
    if (read != quantity) {
        printf("requestFrom: read=%d, quantity=%d : ERROR\n", read, quantity);
        return read;
    }

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = read;

    return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)true));
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)true));
}

uint8_t TwoWire::requestFrom (int address, int quantity, int sendStop) {
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)sendStop));
}

void TwoWire::beginTransmission(uint8_t address) {
    // save address of target and empty buffer
    if (txAddress != address) {
        txAddress = address;
        // If target address changes, wait for 50us to avoid losing next data packet, tested ok down to 10us
        DelayUs(50);
    }
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
    beginTransmission(((uint8_t)address));
}

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
uint8_t TwoWire::endTransmission(uint8_t sendStop) {
    int length;
    uint8_t error = 0;

    length = i2c_write(((i2c_t *)this->pI2C), ((int)this->txAddress), ((const char*)&this->txBuffer[0]), ((int)this->txBufferLength), ((int)sendStop));
    if ((txBufferLength > 0) && (length <= 0)) {
        error = 1;
    }

    txBufferLength = 0; // empty buffer
    return error;
}

// This provides backwards compatibility with the original
// definition, and expected behaviour, of endTransmission
uint8_t TwoWire::endTransmission(void) {
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t data) {
    if (txBufferLength >= BUFFER_LENGTH) {
        return 0;
    }
    txBuffer[txBufferLength++] = data;
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
    for (size_t i = 0; i < quantity; ++i) {
        if (txBufferLength >= BUFFER_LENGTH) {
            return i;
        }
        txBuffer[txBufferLength++] = data[i];
    }
    return quantity;
}

int TwoWire::available(void) {
    return (rxBufferLength - rxBufferIndex);
}

int TwoWire::read(void) {
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex++];
    }
    return -1;
}

int TwoWire::peek(void) {
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex];
    }
    return -1;
}

void TwoWire::flush(void) {
    // Do nothing, use endTransmission(..) to force
    // data transfer.
}

size_t TwoWire::slaveWrite(int buffer) {
    return slaveWrite((uint8_t *)&buffer, 1);
}

size_t TwoWire::slaveWrite(char *buffer) {
    return slaveWrite((uint8_t *)buffer, (size_t)sizeof(buffer));
}

size_t TwoWire::slaveWrite(uint8_t *buffer, size_t len) {
    return i2cSlaveWrite(buffer, len, RECV_TIMEOUT);
}
void TwoWire::onReceiveService(uint8_t *inBytes, size_t numBytes, bool stop, void *arg) {
    stop = stop;

    TwoWire *wire = (TwoWire*)arg;
    if(!wire->user_onReceive){
        return;
    }
    for (uint8_t i = 0; i < numBytes; ++i) {
        wire->rxBuffer[i] = inBytes[i];
    }
    wire->rxBufferIndex = 0;
    wire->rxBufferLength = numBytes;
    wire->user_onReceive(numBytes);
}

void TwoWire::onRequestService(void * arg) {
    TwoWire *wire = (TwoWire*)arg;
    if(!wire->user_onRequest){
        return;
    }
    wire->txBufferLength = 0;
    wire->user_onRequest(); // user callback normally write data into txbuffer
    if (wire->txBufferLength){
        wire->slaveWrite((uint8_t*)wire->txBuffer, wire->txBufferLength);
        //wire->slaveWrite((uint8_t*)txBuffer, txBufferLength);
    }
}

void TwoWire::onReceive(void(*function)(int)) {
    user_onReceive = function;
}

void TwoWire::onRequest(void(*function)(void)) {
    user_onRequest = function;
}

#if defined(BOARD_AMB21_AMB22)
// SDA SCL
// HW: I2C0
TwoWire Wire = TwoWire(I2C_SDA, I2C_SCL); // PA_26, PA_25 / PB_6, PB_5

// HW: I2C1
TwoWire Wire1 = TwoWire(I2C1_SDA, I2C1_SCL); // PA_24, PA_23

#elif defined(BOARD_AMB23)
// SDA SCL
// HW: I2C0
TwoWire Wire = TwoWire(I2C_SDA, I2C_SCL); // PB_0, PA_31 / PB_6, PB_5

// HW: I2C1
TwoWire Wire1 = TwoWire(I2C1_SDA, I2C1_SCL); // PA_24, PA_23

#elif defined(BOARD_AITHINKER_BW16)
// SDA SCL
// HW: I2C0
TwoWire Wire = TwoWire(I2C_SDA, I2C_SCL); // PA_26, PA_25

#elif defined(BOARD_SPARKFUN_AWCU488)
// SDA SCL
// HW: I2C0
TwoWire Wire = TwoWire(I2C_SDA, I2C_SCL); // PA_26, PA_25 / PB_6, PB_5

#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30)
// SDA SCL
// HW: I2C0
TwoWire Wire = TwoWire(I2C_SDA, I2C_SCL); // PA_26, PA_25

#else
#error check the board supported
#endif
