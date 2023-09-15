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

/* AMB21/AMB22 (RTL8722DM/CSM) */

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
    {PB_2,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC          | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D0
    {PB_1,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC          | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D1
    {PB_3,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC                                        , MODE_NOT_INITIAL}, // AMB_D2
    {PB_31, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     | PIO_IR                               , MODE_NOT_INITIAL}, // AMB_D3
    {PB_30, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                                                            , MODE_NOT_INITIAL}, // AMB_D4
    {PB_28, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                                                            , MODE_NOT_INITIAL}, // AMB_D5
    {PB_29, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     | PIO_IR                               , MODE_NOT_INITIAL}, // AMB_D6
    {NC,    NOT_INITIAL,                NOT_INITIAL                                                                        , MODE_NOT_INITIAL}, // AMB_D7
    {PB_22, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           | PIO_IR                               , MODE_NOT_INITIAL}, // AMB_D8
    {PB_23, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           | PIO_IR                               , MODE_NOT_INITIAL}, // AMB_D9
    {PB_21, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART           | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D10
    {PB_18, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART           | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D11
    {PB_19, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART           | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D12
    {PB_20, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART           | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D13
    {PA_7,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                              | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D14
    {PA_8,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                              | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D15
    {PA_25, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           | PIO_IR | PIO_UART | PIO_I2C          , MODE_NOT_INITIAL}, // AMB_D16
    {PA_26, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           | PIO_IR | PIO_UART | PIO_I2C          , MODE_NOT_INITIAL}, // AMB_D17
    {PB_7,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM | PIO_ADC                               | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D18
    {PB_6,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC                     | PIO_I2C | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D19
    {PB_5,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM | PIO_ADC                     | PIO_I2C | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D20
    {PB_4,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM | PIO_ADC                               | PIO_SPI, MODE_NOT_INITIAL}, // AMB_D21
    {PA_28, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                                                            , MODE_NOT_INITIAL}, // AMB_D22
    {PA_24, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART | PIO_I2C          , MODE_NOT_INITIAL}, // AMB_D23
    {PA_23, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM                    | PIO_UART | PIO_I2C          , MODE_NOT_INITIAL}, // AMB_D24
    {PA_22, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                              | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D25
    {PA_21, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                              | PIO_UART                    , MODE_NOT_INITIAL}, // AMB_D26
    {PA_20, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                                                            , MODE_NOT_INITIAL}, // AMB_D27
    {PA_19, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                                                            , MODE_NOT_INITIAL}  // AMB_D28
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
