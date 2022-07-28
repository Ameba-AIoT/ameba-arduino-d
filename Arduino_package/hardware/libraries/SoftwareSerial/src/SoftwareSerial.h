/*
SoftwareSerial.h (formerly NewSoftSerial.h) - 
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

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size

#define PARITY_NONE         (0)
#define PARITY_ODD          (1)
#define PARITY_EVEN         (2)
#define PARITY_FORCED1      (3)
#define PARITY_FORCED0      (4)

#define FLOW_CONTROL_NONE   (0)
#define FLOW_CONTROL_RTS    (1)
#define FLOW_CONTROL_CTS    (2)
#define FLOW_CONTROL_RTSCTS (3)

class SoftwareSerial : public Stream
{
    private:
        void* pUART;
        uint8_t receivePin;
        uint8_t transmitPin;
        long speed;
        int data_bits;
        int parity;
        int stop_bits;
        int flowctrl;

        // Expressed as 4-cycle delays (must never be 0!)
        bool _buffer_overflow;

        uint32_t _receive_buffer_size;
        char *_receive_buffer; 
        volatile uint8_t _receive_buffer_tail;
        volatile uint8_t _receive_buffer_head;

        friend void handle_interrupt(uint32_t id, uint32_t event);

        void (*availableCallback)(char c);

    public:
        // public methods
        SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
        ~SoftwareSerial();
        void begin(long speed);
        bool listen();
        void end();
        bool isListening() { return (pUART != NULL); }
        bool stopListening();

        bool overflow() {
            bool ret = _buffer_overflow;
            if (ret) {
                _buffer_overflow = false;
            }
            return ret;
        }

        int peek();
        virtual size_t write(uint8_t byte);
        virtual int read();
        virtual int available();
        virtual void flush();
        operator bool() { return true; }

        using Print::write;

    /* Extend API provide by RTK */
    public:
        void setBufferSize(uint32_t buffer_size);
        void setAvailableCallback(void (*callback)(char c));
        void begin(long speed, int data_bits, int parity, int stop_bits);
        void begin(long speed, int data_bits, int parity, int stop_bits, int flowctrl, int rtsPin, int ctsPin);
};

#endif
