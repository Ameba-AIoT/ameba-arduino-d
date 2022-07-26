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
    {PA_7,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D0,  LOG_TX
    {PA_8,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D1,  LOG_RX
    {PA_27, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D2                SWD_DATA
    {PA_30, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           , MODE_NOT_INITIAL}, // AMB_D3,               PWM
    {PB_1,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D4,  SERIAL1_TX
    {PB_2,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D5,  SERIAL1_RX
    {PB_3,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC , MODE_NOT_INITIAL}, // AMB_D6,  A2,          SWD_CLK
    {PA_25, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           , MODE_NOT_INITIAL}, // AMB_D7,  *SERIAL1_RX, PWM, I2C_SCL,   IR_TX
    {PA_26, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           , MODE_NOT_INITIAL}, // AMB_D8,  *SERIAL1_TX, PWM, I2C_SDA,   IR_RX
    {PA_15, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D9,                    SPI_SS
    {PA_14, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                     , MODE_NOT_INITIAL}, // AMB_D10,                   SPI_SCLK, LED_G
    {PA_13, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           , MODE_NOT_INITIAL}, // AMB_D11, *SERIAL1_RX, PWM, SPI_MISO, LED_B
    {PA_12, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM           , MODE_NOT_INITIAL}  // AMB_D12, *SERIAL1_TX, PWM, SPI_MOSI, LED_R
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
