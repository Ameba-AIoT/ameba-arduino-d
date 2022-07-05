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

#define MAIN_THREAD_STACK_SIZE        (4096 * 4)

#define TOTAL_GPIO_PIN_NUM            (30)
#define TOTAL_PWM_PIN_NUM             (13)

#define LED_BUILTIN                   (13)

#define FEATURE_ADC
//#define FEATURE_DAC

/* Analog pin mapping */
#define A0                            0 //PB_4    3.3V    CH0
#define A1                            1 //PB_5    3.3V    CH1
#define A2                            2 //PB_6    3.3V    CH2
#define A3                            3 //PB_7    3.3V    CH3
#define A4                            4 //PB_1    3.3V    CH4
#define A5                            5 //PB_2    3.3V    CH5
#define A6                            6 //PB_3    3.3V    CH6

/* DAC pin mapping */
//#define DAC0    AD_3

#define SS                            10

#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define UART2_TX    PA_7
#define UART2_RX    PA_8
#define UART0_TX    PA_12    // Serial1
#define UART0_RX    PA_13    // Serial1
#define UART3_TX    PA_26    
#define UART3_RX    PA_25    

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
