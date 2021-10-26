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
#if 1
PinDescription g_APinDescription[TOTAL_GPIO_PIN_NUM]=
{
    {PA_7,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 0, Serial(0) TX
    {PA_8,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 1, Serial(0) RX
    {PA_27,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 2
    {PA_30,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 3
    {PB_1,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 4, A0, Recommended Serial2 TX
    {PB_2,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 5, A1, Recommended Serial2 RX
    {PB_3,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 6, A2,
    {PA_25,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 7, PWM, Default Serial2 TX, I2C0 SCL, IR TX
    {PA_26,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 8, PWM, Default Serial2 RX, I2C0 SDA, IR RX
    {PA_15,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 9, SPI1 CS
    {PA_14,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 10, SPI1 CLK, GREEN LED
    {PA_13,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 11, SPI1 MISO, PWM, BLUE LED
    {PA_12,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}  //Arduino pin 12, SPI1 MOSI, PWM, RED LED
};
#else
    PinDescription g_APinDescription[TOTAL_GPIO_PIN_NUM]=
    {
    {PB_1,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 0, A0, Recommended Serial2 TX
    {PB_2,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 1, A1, Recommended Serial2 RX
    {PA_7,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 2, Serial(0) TX
    {PA_25,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 3, PWM, Default Serial2 TX, I2C0 SCL, IR TX
    {PB_3,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 4, A2,
    {PA_12,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 5, SPI1 MOSI, PWM, RED LED
    {PA_30,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 6
    {PA_14,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 7, SPI1 CLK, GREEN LED
    {PA_13,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 8, SPI1 MISO, PWM, BLUE LED
    {PA_8,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 9, Serial(0) RX
    {PA_26,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL}, //Arduino pin 10, PWM, Default Serial2 RX, I2C0 SDA, IR RX
    {PA_27,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}, //Arduino pin 11
    {PA_15,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}  //Arduino pin 12, SPI1 CS
    };
#endif

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

// zzw
//    uint8_t *regionAddr;
//    size_t regionSize;

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
