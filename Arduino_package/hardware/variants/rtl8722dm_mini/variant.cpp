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
    {PB_0,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D0
    {PB_1,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D1
    {PB_2,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D2
    {PB_3,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D3
    {PB_4,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D4
    {PB_5,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D5
    {PB_6,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D6
    {PB_7,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D7
    {PA_2,      NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D8
    {PA_12,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D9
    {PA_13,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D10
    {PA_14,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D11
    {PA_15,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D12
    {PA_16,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D13
    {PA_28,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D14
    {PA_18,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D15
    {PA_19,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D16
    {PA_30,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D17
    {PA_21,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D18
    {PA_22,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D19
    {PA_23,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D20
    {PA_24,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM    , NOT_INITIAL},   //D21
    {PA_31,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL},   //D22
    {NC,        NOT_INITIAL, NOT_INITIAL                          , NOT_INITIAL},   //NC/Reserved   D23
    {PA_9,      NOT_INITIAL, PIO_GPIO                             , NOT_INITIAL},   //LED_B         D24
    {PA_10,     NOT_INITIAL, PIO_GPIO                             , NOT_INITIAL},   //LED_G         D25
    {PA_17,     NOT_INITIAL, PIO_GPIO | PIO_GPIO_IRQ              , NOT_INITIAL}    //PUSH_BTN      D26
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
