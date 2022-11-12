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

/* u-blox NORA-W3 (RTL8720DF) */

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

//#include "PinNames.h"
void __libc_init_array(void);

/*
 * Pins descriptions
 */
PinDescription g_APinDescription[TOTAL_GPIO_PIN_NUM]=
{
 // index  D0 - D3                                                                                               NORA     EVK  Function
 /*  0 */ {PB_18, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_G9  D0   UART_RXD
 /*  1 */ {PB_19, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_G8  D1   UART_TXD
 /*  2 */ {PB_20, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_F9  D2   UART_CTS
 /*  3 */ {PB_21, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_F8  D3   UART_RTS
          // D9 - D12
 /*  4 */ {PA_28, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_B3  D9   SPI_CS
 /*  5 */ {PA_25, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_C2  D10  SPI_MOSI
 /*  6 */ {PA_26, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_C1  D11  SPI_MISO
 /*  7 */ {PA_30, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_B1  D12  SPI_CLK
          // D14 - D19
 /*  8 */ {PA_7,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}, // NORA_A5  D14  DBG_TX
 /*  9 */ {PA_8,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}, // NORA_A6  D15  DBG_RX
 /* 10 */ {PA_14, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}, // NORA_H9  D16  LED_BLUE
 /* 11 */ {PA_15, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}, // NORA_H8  D17  LED_GREEN
 /* 12 */ {PA_12, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_J9  D18
 /* 13 */ {PA_13, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_J8  D19  LED_RED
           // A0 - A6
 /* 14 */ {PB_3,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC, MODE_NOT_INITIAL}, // NORA_J2  A0
 /* 15 */ {PB_1,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC, MODE_NOT_INITIAL}, // NORA_D8  A1   SW1
 /* 16 */ {PA_27, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}, // NORA_H2  A2
 /* 17 */ {PB_23, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_C8  A3   SW2
 /* 18 */ {PB_22, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          , MODE_NOT_INITIAL}, // NORA_E9  A4   UART_DSR
 /* 19 */ {PB_2,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC, MODE_NOT_INITIAL}  // NORA_E8  A5   UART_DTR
};

void *gpio_pin_struct[TOTAL_GPIO_PIN_NUM] = {NULL};
void *gpio_irq_handler_list[TOTAL_GPIO_PIN_NUM] = {NULL};
#ifdef __cplusplus
} // extern C
#endif

void serialEvent() __attribute__((weak));
bool Serial_available() __attribute__((weak));

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
    if (Serial_available && serialEvent && Serial_available()) serialEvent();
}

void init(void)
{
    // Initialize C library
    __libc_init_array();
}

// ----------------------------------------------------------------------------

void wait_for_debug(void) {
    while (((CoreDebug->DHCSR) & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0) {
        asm("nop");
    }
    delay(1000);
}
