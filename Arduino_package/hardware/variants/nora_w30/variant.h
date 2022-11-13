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

/* u-blox NORA-W30 (RTL8720DF) */

#ifndef _VARIANT_ARDUINO_AMEBA_
#define _VARIANT_ARDUINO_AMEBA_

#include "Arduino.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#include "PinNames.h"

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
}
#endif

#define MAIN_THREAD_STACK_SIZE                  (4096 * 4)

#define TOTAL_GPIO_PIN_NUM                      (20)
#define TOTAL_PWM_PIN_NUM                       (12)

/* Digital pin mapping refer to g_APinDescription */
/*               index into g_APinDescription --v */
#define D0                                      0 // PB_18
#define D1                                      1 // PB_19
#define D2                                      2 // PB_20
#define D3                                      3 // PB_21

#define D10                                     4 // PA_28
#define D11                                     5 // PA_25
#define D12                                     6 // PA_26
#define D13                                     7 // PA_30
#define D14                                     8 // PA_7
#define D15                                     9 // PA_8
#define D16                                     10 // PA_14
#define D17                                     11 // PA_15
#define D18                                     12 // PA_12
#define D19                                     13 // PA_13

/* Analog pin mapping */
#define A0                                      14 // PB_3
#define A1                                      15 // PB_1
#define A2                                      16 // PA_27
#define A3                                      17 // PB_23
#define A4                                      18 // PB_22
#define A5                                      19 // PB_2

/* LED pin mapping */
#define LED_BUILTIN                             10 // PA_14
#define LED_RED                                 13 // PA_13
#define LED_GREEN                               11 // PA_15
#define LED_BLUE                                10 // PA_14

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_SS                                  4 // PA_28
#define SPI_MOSI                                5 // PA_25
#define SPI_MISO                                6 // PA_26
#define SPI_SCLK                                7 // PA_30

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 3 // PB_21
#define I2C_SCL                                 2 // PB_20

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  8 // PA_7
#define LOG_RX                                  9 // PA_8
#define SERIAL1_TX                              1 // PB_19
#define SERIAL1_RX                              0 // PB_18
#define SERIAL2_TX                              12 // PA_12
#define SERIAL2_RX                              13 // PA_13

#endif /* _VARIANT_ARDUINO_AMEBA_ */
