/*
SoftwareSerial.cpp (formerly NewSoftSerial.cpp) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

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
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

// 
// Includes
// 
#include <Arduino.h>
#include <SoftwareSerial.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "serial_api.h"
#include "serial_ex_api.h"

serial_t sobj;
extern PinDescription g_APinDescription[];

#ifdef __cplusplus
}
#endif

//
// Private methods
//

void handle_interrupt(uint32_t id, uint32_t event)
{
    volatile char d = 0;
    uint8_t next;
    SoftwareSerial *pSwSerial = (SoftwareSerial *)id;

    if (((SerialIrq)event) == RxIrq) {
        d = serial_getc((serial_t *)(pSwSerial->pUART));
        next = ((pSwSerial->_receive_buffer_tail) + 1) % (pSwSerial->_receive_buffer_size);
        if (next != (pSwSerial->_receive_buffer_head)) {
            pSwSerial->_receive_buffer[(pSwSerial->_receive_buffer_tail)] = d;
            pSwSerial->_receive_buffer_tail = next;
        } else {
            pSwSerial->_buffer_overflow = true;
        }
        if ((pSwSerial->availableCallback) != NULL) {
            pSwSerial->availableCallback(d);
        }
    }
}

// This function sets the current object as the "listening"
// one and returns true if it replaces another 
bool SoftwareSerial::listen()
{
    bool ret = false;

    serial_init(((serial_t *)pUART), ((PinName)g_APinDescription[transmitPin].pinname), ((PinName)g_APinDescription[receivePin].pinname));
    serial_baud(((serial_t *)pUART), speed);
    serial_format(((serial_t *)pUART), data_bits, ((SerialParity)parity), stop_bits);
    serial_irq_handler(((serial_t *)pUART), ((uart_irq_handler)handle_interrupt), ((uint32_t)this));
    serial_irq_set(((serial_t *)pUART), RxIrq, 1);
    serial_irq_set(((serial_t *)pUART), TxIrq, 1);
    if (flowctrl) {
        serial_set_flow_control(((serial_t *)pUART), FlowControlRTSCTS, ((PinName)PA_14), ((PinName)0));     // PA_14 = RTS,  PA_15 = CTS
    }
    return ret;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
    serial_free((serial_t *)pUART);
    free((serial_t *)pUART);
    pUART = NULL;
    return true;
}

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */) : 
    _buffer_overflow(false)
{
    inverse_logic = inverse_logic;
    this->receivePin = receivePin;
    this->transmitPin = transmitPin;
    pUART = NULL;
    _receive_buffer_size = _SS_MAX_RX_BUFF;
    _receive_buffer = ((char *)malloc(_receive_buffer_size));
    availableCallback = NULL;

    data_bits = 8;
    parity = PARITY_NONE;
    flowctrl = FLOW_CONTROL_NONE;
    stop_bits = 1;
}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
    end();
}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
    pUART = malloc(sizeof(serial_t));
    if (pUART == NULL) {
        printf("fail to malloc\r\n");
    }
    this->speed = speed;
    listen();
}

void SoftwareSerial::end()
{
    if (_receive_buffer != NULL) {
        free(_receive_buffer);
        _receive_buffer = NULL;
    }
    stopListening();
}


// Read data from buffer
int SoftwareSerial::read()
{
    if (!isListening()) {
        return -1;
    }

    // Empty buffer?
    if (_receive_buffer_head == _receive_buffer_tail) {
        return -1;
    }

    // Read from "head"
    uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
    _receive_buffer_head = (_receive_buffer_head + 1) % _receive_buffer_size;
    _buffer_overflow = false;
    return d;
}

int SoftwareSerial::available()
{
    if (!isListening()) {
        return 0;
    }

    return (_receive_buffer_tail + _receive_buffer_size - _receive_buffer_head) % _receive_buffer_size;
}

size_t SoftwareSerial::write(uint8_t b)
{
    int ret = 0;

    ret = serial_send_blocked(((serial_t *)pUART), ((char *)&b), 1, 10);

    return ret;
}

void SoftwareSerial::flush()
{
    if (!isListening()) {
        return;
    }

    _receive_buffer_head = _receive_buffer_tail = 0;
    _buffer_overflow = false;
}

int SoftwareSerial::peek()
{
    if (!isListening()) {
        return -1;
    }

    // Empty buffer?
    if (_receive_buffer_head == _receive_buffer_tail) {
        return -1;
    }

    // Read from "head"
    return _receive_buffer[_receive_buffer_head];
}

/* Extend API provide by RTK */
void SoftwareSerial::setBufferSize(uint32_t buffer_size)
{
    if (_receive_buffer != NULL) {
        if (realloc(_receive_buffer, buffer_size) != NULL) {
            _receive_buffer_size = buffer_size;
        }
    }
}

void SoftwareSerial::setAvailableCallback(void (*callback)(char c))
{
    availableCallback = callback;
}

void SoftwareSerial::begin(long speed, int data_bits, int parity, int stop_bits)
{
    pUART = malloc(sizeof(serial_t));
    if (pUART == NULL) {
        printf("fail to malloc\r\n"); 
    }
    this->speed     = speed;
    this->data_bits = data_bits;
    this->parity    = parity;
    this->stop_bits = stop_bits;
    listen();
}

void SoftwareSerial::begin(long speed, int data_bits, int parity, int stop_bits, int flowctrl, int rtsPin, int ctsPin)
{
    rtsPin = rtsPin;
    ctsPin = ctsPin;
    pUART = malloc(sizeof(serial_t));
    if (pUART == NULL) {
        printf("fail to malloc\r\n");
    }
    this->speed     = speed;
    this->data_bits = data_bits;
    this->parity    = parity;
    this->stop_bits = stop_bits;
    this->flowctrl  = flowctrl;
    listen();
}
