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

#define TOTAL_GPIO_PIN_NUM              (29)
#define TOTAL_PWM_PIN_NUM               (9)

/* Analog pin mapping */
#define A0                                  0       //PB_4    3.3V    CH0
#define A1                                  1       //PB_5    3.3V    CH1
#define A2                                  2       //PB_6    3.3V    CH2
#define A3                                  3       //PB_7    3.3V    CH3
#define A4                                  4       //PB_1    3.3V    CH4
#define A5                                  5       //PB_2    3.3V    CH5
#define A6                                  6       //PB_3    3.3V    CH6

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                              0       //PB_0
#define AMB_D1                              1       //PB_1
#define AMB_D2                              2       //PB_2
#define AMB_D3                              3       //PB_3
#define AMB_D4                              4       //PB_4
#define AMB_D5                              5       //PB_5
#define AMB_D6                              6       //PB_6
#define AMB_D7                              7       //PB_7
#define AMB_D8                              8       //PA_2
#define AMB_D9                              9       //PA_12
#define AMB_D10                             10      //PA_13
#define AMB_D11                             11      //PA_14
#define AMB_D12                             12      //PA_15
#define AMB_D13                             13      //PA_16
#define AMB_D14                             14      //PA_28
#define AMB_D15                             15      //PA_18
#define AMB_D16                             16      //PA_19
#define AMB_D17                             17      //PA_30
#define AMB_D18                             18      //PA_21
#define AMB_D19                             19      //PA_22
#define AMB_D20                             20      //PA_23
#define AMB_D21                             21      //PA_24
#define AMB_D22                             22      //PA_31
#define AMB_D23                             23      //PA_27
#define AMB_D24                             24      //PA_9
#define AMB_D25                             25      //PA_10
#define AMB_D26                             26      //PA_17
#define AMB_D27                             27      //PA_7
#define AMB_D28                             28      //PA_8

/* LED pin mapping */
#define LED_B                           AMB_D24     //PA_9  Blue
#define LED_G                           AMB_D25     //PA_10 Green
#define LED_BUILTIN                     LED_B       //PA_9  Blue

/* Push button */
#define PUSH_BTN                        AMB_D26

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                       AMB_D9      //D9/D4
#define SPI_MISO                       AMB_D10     //D10/D5
#define SPI_SCLK                       AMB_D11     //D11/D6
#define SPI_SS                         AMB_D12     //D12/D7

/* TwoWire/I2C pin mapping */
#define I2C_SDA                        AMB_D0      //D0/D6
#define I2C_SCL                        AMB_D22     //D22/D5

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                          AMB_D27
#define LOG_RX                          AMB_D28
#define SERIAL1_TX                      AMB_D18     // HS_UART0_TX 18/15
#define SERIAL1_RX                      AMB_D19     // HS_UART0_RX 16/19
#define SERIAL2_TX                      AMB_D1      // LP_UART_TX 1/9
#define SERIAL2_RX                      AMB_D2      // LP_UART_RX 2/10

/* SWD pin mapping */
#define SWD_DATA                        AMB_D23
#define SWD_CLK                         AMB_D3


#endif /* _VARIANT_ARDUINO_AMEBA_ */
