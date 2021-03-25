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

#ifndef _VARIANT_ARDUINO_AMEBA_
#define _VARIANT_ARDUINO_AMEBA_

#include "Arduino.h"
#include <stdarg.h>

#ifdef __cplusplus
#include "LOGUARTClass.h"
#include "UARTClassOne.h"
#include "UARTClassTwo.h"
extern "C"{
#include "PinNames.h"
}
#endif

#define MAIN_THREAD_STACK_SIZE          (4096 * 4)

#define TOTAL_GPIO_PIN_NUM              (27)
#define TOTAL_PWM_PIN_NUM               (9)

#define LED_B                           (24)
#define LED_G                           (25)
#define LED_BUILTIN                     (LED_B)

#define PUSH_BTN                        (26)

#define FEATURE_ADC
//#define FEATURE_DAC

/* Analog pin mapping */
#define A0                            4 //D1    PB_1    3.3V    CH4
#define A1                            5 //D2    PB_2    3.3V    CH5
#define A2                            6 //D3    PB_3    3.3V    CH6
#define A3                            0 //D4    PB_4    3.3V    CH0
#define A4                            1 //D5    PB_5    3.3V    CH1
#define A5                            2 //D6    PB_6    3.3V    CH2
#define A6                            3 //D7    PB_7    3.3V    CH3

#define SS                            10

#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define UART2_TX    PA_7
#define UART2_RX    PA_8
#define UART0_TX    PA_21    //PA_18
#define UART0_RX    PA_22    //PA_19
#define UART3_TX    PB_1     //PA_12
#define UART3_RX    PB_2     //PA_13

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define portOutputRegister(P) ((volatile uint32_t *)(0x48014000 + (P) * 0x400))
#define portInputRegister(P)  ((volatile uint32_t *)(0x48014050 + (P) * 0x400))
#define portModeRegister(P)   ((volatile uint32_t *)(0x48014004 + (P) * 0x400))

/*
 * Wait until enter debug mode
 *
 * Check DHCSR(0xE000EDF0) register and hold until bit C_DEBUGEN is set.
 * Use this function along with J-LINK or other debug tool
 **/
extern void wait_for_debug(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern LOGUARTClass Serial;
extern UARTClassOne Serial1;
extern UARTClassTwo Serial2;
#endif

#endif /* _VARIANT_ARDUINO_AMEBA_ */
