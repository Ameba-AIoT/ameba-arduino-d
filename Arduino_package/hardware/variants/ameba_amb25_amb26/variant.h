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

/* AMB25/AMB26 (RTL8720DF) */

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

#define MAIN_THREAD_STACK_SIZE      (4096 * 4)
#define TOTAL_GPIO_PIN_NUM          (20)
#define TOTAL_PWM_PIN_NUM           (12)
#define digitalPinToInterrupt(p)    (((p)<TOTAL_GPIO_PIN_NUM)?(p):-1)

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                                  0  // PA_15
#define AMB_D1                                  1  // PA_14
#define AMB_D2                                  2  // PA_13
#define AMB_D3                                  3  // PA_12
#define AMB_D4                                  4  // PB_30
#define AMB_D5                                  5  // PA_28
#define AMB_D6                                  6  // PA_26
#define AMB_D7                                  7  // PA_25
#define AMB_D8                                  8  // PA_27
#define AMB_D9                                  9  // PB_3
#define AMB_D10                                 10 // PB_2
#define AMB_D11                                 11 // PB_1
#define AMB_D12                                 12 // PA_7
#define AMB_D13                                 13 // PA_8
#define AMB_D14                                 14 // PB_23
#define AMB_D15                                 15 // PB_22
#define AMB_D16                                 16 // PB_19
#define AMB_D17                                 17 // PB_18
#define AMB_D18                                 18 // PB_21
#define AMB_D19                                 19 // PB_20

#define PB23                                    AMB_D14
#define PB22                                    AMB_D15
#define PB21                                    AMB_D18
#define PB20                                    AMB_D19
#define PB19                                    AMB_D16
#define PB18                                    AMB_D17
#define PA27                                    AMB_D8
#define PA28                                    AMB_D5
#define PA25                                    AMB_D7
#define PA26                                    AMB_D6
#define PB1                                     AMB_D11
#define PB2                                     AMB_D10
#define PB3                                     AMB_D9
#define PA12                                    AMB_D3
#define PA13                                    AMB_D2
#define PA14                                    AMB_D1
#define PA15                                    AMB_D0
#define PA8                                     AMB_D13
#define PA7                                     AMB_D12
#define PB30                                    AMB_D4

/* Analog pin mapping */
#define A4                                      AMB_D11 // PB_1 3.3V    CH4
#define A5                                      AMB_D10 // PB_2 3.3V    CH5
#define A6                                      AMB_D9  // PB_3 3.3V    CH6

/* LED pin mapping */
#define LED_G                                   AMB_D3  // Green
#define LED_R                                   AMB_D2  // Red
#define LED_B                                   AMB_D1  // Blue
#define LED_BUILTIN                             LED_G   // Green

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                                AMB_D17 // PB_18
#define SPI_MISO                                AMB_D16 // PB_19
#define SPI_SCLK                                AMB_D19 // PB_20
#define SPI_SS                                  AMB_D18 // PB_21
#define SPI1_MOSI                               AMB_D3  // PA_12
#define SPI1_MISO                               AMB_D2  // PA_13
#define SPI1_SCLK                               AMB_D1  // PA_14
#define SPI1_SS                                 AMB_D0  // PA_15

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 AMB_D6  // PA_26
#define I2C_SCL                                 AMB_D7  // PA_25

/* IR Device pin mapping */
#define IR_TX                                   AMB_D7  // PA_25 PB_23
#define IR_RX                                   AMB_D6  // PA_26 PB_22

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  AMB_D12 // UART_LOG_TX   PA_7
#define LOG_RX                                  AMB_D13 // UART_LOG_RX   PA_8
#define SERIAL1_TX                              AMB_D16 // HS_UART0_TX   PB_19
#define SERIAL1_RX                              AMB_D17 // HS_UART0_RX   PB_18
#define SERIAL1_RTS                             AMB_D18 // HS_UART0_RTS  PB_21
#define SERIAL1_CTS                             AMB_D19 // HS_UART0_CTS  PB_20
#define SERIAL2_TX                              AMB_D3  // LP_UART_TX    PA_12
#define SERIAL2_RX                              AMB_D2  // LP_UART_RX    PA_13

/* SWD pin mapping */
#define SWD_DATA                                AMB_D8  // PA_27
#define SWD_CLK                                 AMB_D9  // PB_3

#endif /* _VARIANT_ARDUINO_AMEBA_ */
