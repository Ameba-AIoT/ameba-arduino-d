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

/* AMB21/AMB22 (RTL8722DM/CSM) */

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

#define TOTAL_GPIO_PIN_NUM                      (29)
#define TOTAL_PWM_PIN_NUM                       (13)

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                                  0  // PB_2
#define AMB_D1                                  1  // PB_1
#define AMB_D2                                  2  // PB_3
#define AMB_D3                                  3  // PB_31
#define AMB_D4                                  4  // PB_30
#define AMB_D5                                  5  // PB_28
#define AMB_D6                                  6  // PB_29
#define AMB_D7                                  7  // NC
#define AMB_D8                                  8  // PB_22
#define AMB_D9                                  9  // PB_23
#define AMB_D10                                 10 // PB_21
#define AMB_D11                                 11 // PB_18
#define AMB_D12                                 12 // PB_19
#define AMB_D13                                 13 // PB_20
#define AMB_D14                                 14 // PA_7
#define AMB_D15                                 15 // PA_8
#define AMB_D16                                 16 // PA_25
#define AMB_D17                                 17 // PA_26
#define AMB_D18                                 18 // PB_7
#define AMB_D19                                 19 // PB_6
#define AMB_D20                                 20 // PB_5
#define AMB_D21                                 21 // PB_4
#define AMB_D22                                 22 // PA_28
#define AMB_D23                                 23 // PA_24
#define AMB_D24                                 24 // PA_23
#define AMB_D25                                 25 // PA_22
#define AMB_D26                                 26 // PA_21
#define AMB_D27                                 27 // PA_20
#define AMB_D28                                 28 // PA_19

/* Analog pin mapping */
#define A0                                      AMB_D21 // PB_4    3.3V    CH0
#define A1                                      AMB_D20 // PB_5    3.3V    CH1
#define A2                                      AMB_D19 // PB_6    3.3V    CH2
#define A3                                      AMB_D18 // PB_7    3.3V    CH3
#define A4                                      AMB_D1  // PB_1    3.3V    CH4
#define A5                                      AMB_D0  // PB_2    3.3V    CH5
#define A6                                      AMB_D2  // PB_3    3.3V    CH6

/* LED pin mapping */
#define LED_BUILTIN                             AMB_D8

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                                AMB_D11 // AMB_D11
#define SPI_MISO                                AMB_D12 // AMB_D12
#define SPI_SCLK                                AMB_D13 // AMB_D13
#define SPI_SS                                  AMB_D10 // AMB_D10
#define SPI1_MOSI                               AMB_D21 // AMB_D21
#define SPI1_MISO                               AMB_D20 // AMB_D20
#define SPI1_SCLK                               AMB_D19 // AMB_D19
#define SPI1_SS                                 AMB_D18 // AMB_D18

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 AMB_D17 // AMB_D17 / AMB_D19
#define I2C_SCL                                 AMB_D16 // AMB_D16 / AMB_D20
#define I2C1_SDA                                AMB_D23 // AMB_D23
#define I2C1_SCL                                AMB_D24 // AMB_D24

/* IR Device pin mapping */
#define IR_TX                                   AMB_D9 // AMB_D9 / AMB_D16 / AMB_D3
#define IR_RX                                   AMB_D8 // AMB_D8 / AMB_D17 / AMB_D6

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  AMB_D14 // UART_LOG_TX   AMB_D14
#define LOG_RX                                  AMB_D15 // UART_LOG_RX   AMB_D15
#define SERIAL1_TX                              AMB_D26 // HS_UART0_TX   AMB_D26 / AMB_D12
#define SERIAL1_RX                              AMB_D25 // HS_UART0_RX   AMB_D25 / AMB_D11
#define SERIAL1_RTS                             AMB_D24 // HS_UART0_RTS  AMB_D24 / AMB_D10
#define SERIAL1_CTS                             AMB_D23 // HS_UART0_CTS  AMB_D23 / AMB_D13
#define SERIAL2_TX                              AMB_D17 // LP_UART_TX    AMB_D17 / AMB_D1
#define SERIAL2_RX                              AMB_D16 // LP_UART_RX    AMB_D16 / AMB_D0

/* SWD pin mapping */
#define SWD_DATA                                AMB_D12
#define SWD_CLK                                 AMB_D11 // AMB_D2

#endif /* _VARIANT_ARDUINO_AMEBA_ */
