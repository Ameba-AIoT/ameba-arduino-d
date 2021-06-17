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

#define TOTAL_GPIO_PIN_NUM            (13)
#define TOTAL_PWM_PIN_NUM             (4)

#define LED_BUILTIN                   (10) //PA_14, Green
#define LED_BUILTIN_G                 (10)
#define LED_BUILTIN_R                 (12)
#define LED_BUILTIN_B                 (11)

#define FEATURE_ADC
//#define FEATURE_DAC

/* Analog pin mapping */
#define A0                            4 //PB_1    3.3V    CH0
#define A1                            5 //PB_2    3.3V    CH1
#define A2                            6 //PB_3    3.3V    CH2

/* DAC pin mapping */
//#define DAC0    AD_3

//#define SS                            10

#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define UART2_TX    PA_7
#define UART2_RX    PA_8
#define UART0_TX    PA_21    //PB_19  //Not available
#define UART0_RX    PA_22    //PB_18  //Not available
#define UART3_TX    PB_1    //PB_1
#define UART3_RX    PB_2    //PB_2

//All gpios. This is actrually necessary...
#define PA7 0 
#define PA8 1 
#define PA27 2
#define PA30 3
#define PB1 4
#define PB2 5 
#define PB3 6 
#define PA25 7 
#define PA26 8 
#define PA15 9 
#define PA14 10 
#define PA13 11 
#define PA12 12 


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
//extern void* rtw_calloc(size_t cnt,char fill);
extern char* ltoa(long l, char * buffer, int radix);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern LOGUARTClass Serial1;  //This should be Serial1 for this board
//extern UARTClassOne Serial; //Not available
extern UARTClassTwo Serial;
#endif

#endif /* _VARIANT_ARDUINO_AMEBA_ */
