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
    {PB_2,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D0
    {PB_1,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D1
    {PB_3,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D2
    {PB_31,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D3
    {PB_30,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D4
    {PB_28,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D5
    {PB_29,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D6
    {NC,       NOT_INITIAL, NOT_INITIAL           , NOT_INITIAL}, //D7
    {PB_22,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D8
    {PB_23,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D9
    {PB_21,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D10
    {PB_18,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D11
    {PB_19,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D12
    {PB_20,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D13
    {PA_7,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D14
    {PA_8,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D15
    {PA_25,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D16
    {PA_26,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D17
    {PB_7,     NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D18
    {PB_6,     NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D19
    {PB_5,     NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D20
    {PB_4,     NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D21
    {PA_28,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D22
    {PA_24,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D23
    {PA_23,    NOT_INITIAL, PIO_GPIO | PIO_PWM    , NOT_INITIAL}, //D24
    {PA_22,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D25
    {PA_21,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D26
    {PA_20,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}, //D27
    {PA_19,    NOT_INITIAL, PIO_GPIO              , NOT_INITIAL}  //D28
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
