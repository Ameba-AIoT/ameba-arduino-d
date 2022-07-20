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

#define MAIN_THREAD_STACK_SIZE          (4096 * 4)

#define TOTAL_GPIO_PIN_NUM              (30)
#define TOTAL_PWM_PIN_NUM               (11)

/* Analog pin mapping */
#define A0                              0       // PB_4    3.3V    CH0
#define A1                              1       // PB_5    3.3V    CH1
#define A2                              2       // PB_6    3.3V    CH2
#define A3                              3       // PB_7    3.3V    CH3
#define A4                              4       // PB_1    3.3V    CH4
#define A5                              5       // PB_2    3.3V    CH5
#define A6                              6       // PB_3    3.3V    CH6

/* Digital pin mapping refer to g_APinDescription */
#define D0                              0       // PA_18
#define D1                              1       // PA_16
#define D2                              2       // PA_17
#define D3                              3       // PA_13
#define D4                              4       // PA_12
#define D5                              5       // PA_26
#define D6                              6       // PA_25
#define D7                              7       // PA_30
#define D8                              8       // PA_19
#define D9                              9       // PB_3
#define D10                             10      // PB_23
#define D11                             11      // PB_22
#define D12                             12      // PB_26
#define D13                             13      // PB_29
#define D14                             14      // PB_4
#define D15                             15      // PB_5
#define D16                             16      // PB_6
#define D17                             17      // PB_7
#define D18                             18      // PB_1
#define D19                             19      // PB_2
#define D20                             20      // PB_31
#define D21                             21      // PA_0
#define D22                             22      // PA_4
#define D23                             23      // PA_2
#define D24                             24      // PA_7
#define D25                             25      // PA_8
#define D26                             26      // PA_28
#define D27                             27      // PA_27
#define D28                             28      // PA_15
#define D29                             29      // PA_14

/* LED pin mapping */
#define LED_BUILTIN                     D13     // D13 Blue

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                        D1
#define SPI_MISO                        D2
#define SPI_SCLK                        D0
#define SPI_SS                          D8
#define SPI1_MOSI                       D14     // D14/D4
#define SPI1_MISO                       D15     // D15/D3
#define SPI1_SCLK                       D16     // D16/D29
#define SPI1_SS                         D17     // D17/D28

/* TwoWire/I2C pin mapping */
#define TwoWire_SDA                     D5      // D5 qwiic/D16
#define TwoWire_SCL                     D6      // D6 qwiic/D15

/* IR Device pin mapping */
#define IR_TX                           D6      // D6/D10/D20
#define IR_RX                           D5      // D5/D11/D13

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                          D24     // UART_LOG_TX  D24
#define LOG_RX                          D25     // UART_LOG_RX  D25
#define SERIAL1_TX                      D0      // HS_UART0_TX  D0
#define SERIAL1_RX                      D8      // HS_UART0_RX  D8
#define SERIAL2_TX                      D4      // LP_UART_TX   D4/D5/D18
#define SERIAL2_RX                      D3      // LP_UART_RX   D3/D6/D19

#if 0
static const uint32_t LOG_TX        =   D24;    // UART_LOG_TX  D24
static const uint32_t LOG_RX        =   D25;    // UART_LOG_RX  D25
#endif

#endif /* _VARIANT_ARDUINO_AMEBA_ */
