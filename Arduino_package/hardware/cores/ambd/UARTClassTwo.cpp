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

#include "Arduino.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UARTClassTwo.h"

//#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE 1

#ifdef __cplusplus
extern "C" {
#endif

#include "serial_api.h"
#include "rtl8721d_vector.h"

static serial_t uart_obj;

#ifdef __cplusplus
}
#endif

RingBuffer rx_buffer2;

static void arduino_uart_irq_handler(uint32_t id, SerialIrq event) {
    char c;
    RingBuffer *pRxBuffer = (RingBuffer *)id;

    if (event == RxIrq) {
        c = char(serial_getc(&uart_obj));
        pRxBuffer->store_char(c);
    }
}

UARTClassTwo::UARTClassTwo(RingBuffer* pRx_buffer) {
//UARTClassTwo::UARTClassTwo(int dwIrq, RingBuffer* pRx_buffer) {
    _rx_buffer = pRx_buffer;
    //_dwIrq = dwIrq;
}

// Protected Methods //////////////////////////////////////////////////////////////




// Public Methods //////////////////////////////////////////////////////////////
void UARTClassTwo::IrqHandler(void) {
    uint8_t     data = 0;
    BOOL        PullMode = _FALSE;

    //UART_TypeDef * pLOG_UART = UART2_DEV;
    uint32_t    IrqEn;
    IrqEn = UART_IntStatus((UART_TypeDef*)UART2_DEV);

    //DiagSetIsrEnReg(0);

    serial_irq_set(&uart_obj, RxIrq, 0);
    //serial_irq_set(&uart_obj, TxIrq, 0);

    data = DiagGetChar(PullMode);
    if (data > 0) {
        _rx_buffer->store_char(data);
    }

    //DiagSetIsrEnReg(IrqEn);
    serial_irq_set(&uart_obj, RxIrq, IrqEn);
}

void UARTClassTwo::begin(const uint32_t dwBaudRate, uint8_t serial_config_value) {
#if defined(BOARD_RTL8720DN_BW16)
    // Log, UART_LOG
    //serial_init(&log_uart_obj, PA_7, PA_8);
    //serial_init(&log_uart_obj, PinName(g_APinDescription[LOG_TX].pinname), PinName(g_APinDescription[LOG_RX].pinname));

    // HS_UART0_TX  not available use LP_UART as SERIAL1
    // serial1, LP_UART
    //serial_init(&uart_obj, PB_1, PB_2);
    //serial_init(&uart_obj, PA_26, PA_25);
    //serial_init(&uart_obj, PA_12, PA_13);
    serial_init(&uart_obj, PinName(g_APinDescription[SERIAL1_TX].pinname), PinName(g_APinDescription[SERIAL1_RX].pinname));
#else
    // Log, UART_LOG
    //serial_init(&log_uart_obj, PA_7, PA_8);
    //serial_init(&log_uart_obj, PinName(g_APinDescription[LOG_TX].pinname), PinName(g_APinDescription[LOG_RX].pinname));

    // serial1, HS_UART0
    //serial_init(&uart_obj, PB_19, PB_18);
    //serial_init(&uart_obj, PA_21, PA_22);
    //serial_init(&uart_obj, PA_18, PA_19);
    //serial_init(&uart_obj, PinName(g_APinDescription[SERIAL1_TX].pinname), PinName(g_APinDescription[SERIAL1_RX].pinname));

    // serial2, LP_UART
    //serial_init(&uart_obj, PB_1, PB_2);
    //serial_init(&uart_obj, PA_26, PA_25);
    //serial_init(&uart_obj, PA_12, PA_13);
    serial_init(&uart_obj, PinName(g_APinDescription[SERIAL2_TX].pinname), PinName(g_APinDescription[SERIAL2_RX].pinname));
#endif

    switch(serial_config_value) {
        case SERIAL_7N1:
            serial_format(&uart_obj, 7, ParityNone, 1);
            break;
        case SERIAL_8N1:
            serial_format(&uart_obj, 8, ParityNone, 1);
            break;
        case SERIAL_7N2:
            serial_format(&uart_obj, 7, ParityNone, 2);
            break;
        case SERIAL_8N2:
            serial_format(&uart_obj, 8, ParityNone, 2);
            break;
        case SERIAL_7E1:
            serial_format(&uart_obj, 7, ParityEven, 1);
            break;
        case SERIAL_8E1:
            serial_format(&uart_obj, 8, ParityEven, 1);
            break;
        case SERIAL_7E2:
            serial_format(&uart_obj, 7, ParityEven, 2);
            break;
        case SERIAL_8E2:
            serial_format(&uart_obj, 8, ParityEven, 2);
            break;
        case SERIAL_7O1:
            serial_format(&uart_obj, 7, ParityOdd, 1);
            break;
        case SERIAL_8O1:
            serial_format(&uart_obj, 8, ParityOdd, 1);
            break;
        case SERIAL_7O2:
            serial_format(&uart_obj, 7, ParityOdd, 2);
            break;
        case SERIAL_8O2:
            serial_format(&uart_obj, 8, ParityOdd, 2);
            break;
        case SERIAL_711:
            serial_format(&uart_obj, 7, ParityForced1, 1);
            break;
        case SERIAL_811:
            serial_format(&uart_obj, 8, ParityForced1, 1);
            break;
        case SERIAL_712:
            serial_format(&uart_obj, 7, ParityForced1, 2);
            break;
        case SERIAL_812:
            serial_format(&uart_obj, 8, ParityForced1, 2);
            break;
        case SERIAL_701:
            serial_format(&uart_obj, 7, ParityForced0, 1);
            break;
        case SERIAL_801:
            serial_format(&uart_obj, 8, ParityForced0, 1);
            break;
        case SERIAL_702:
            serial_format(&uart_obj, 7, ParityForced0, 2);
            break;
        case SERIAL_802:
            serial_format(&uart_obj, 8, ParityForced0, 2);
            break;
      default:
        serial_format(&uart_obj, 8, ParityNone, 1);
    }

    uint32_t UART_BaudRate = dwBaudRate;

#if SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE
    /* log uart initialize in 115200 baud rate.
     * If we change baud rate here, Serail Monitor would not detect this change and show nothing on screen.
     */
    //serial_baud(&uart_obj, dwBaudRate);
    serial_baud(&uart_obj, UART_BaudRate);
#else
    UART_BaudRate = 115200;
    serial_baud(&uart_obj, UART_BaudRate);
#endif

    serial_irq_set(&uart_obj, RxIrq, 1);
    serial_irq_handler(&uart_obj, arduino_uart_irq_handler, (uint32_t)_rx_buffer);
}

void UARTClassTwo::end(void) {
    // clear any received data
    _rx_buffer->_iHead = _rx_buffer->_iTail;

    serial_free(&uart_obj);
}

int UARTClassTwo::available(void) {
    return (uint32_t)(SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE;
}

int UARTClassTwo::peek(void) {
    if (_rx_buffer->_iHead == _rx_buffer->_iTail)
        return -1;

    return _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
}

int UARTClassTwo::read(void) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer->_iHead == _rx_buffer->_iTail) {
        return -1;
    }

    uint8_t uc = _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
    _rx_buffer->_iTail = (unsigned int)(_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE;
    return uc;
}

void UARTClassTwo::flush(void) {
// TODO: 
// while ( serial_writable(&(this->sobj)) != 1 );
/*
  // Wait for transmission to complete
  while ((_pUart->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY)
    ;
*/
}

size_t UARTClassTwo::write(const uint8_t uc_data) {
    serial_putc(&uart_obj, (int)(uc_data));
    return 1;
}

#if defined(BOARD_RTL8720DN_BW16)
UARTClassTwo Serial1(&rx_buffer2);
bool Serial1_available() {
    return Serial1.available() > 0;
}
#else
UARTClassTwo Serial2(&rx_buffer2);
bool Serial2_available() {
    return Serial2.available() > 0;
}
#endif
