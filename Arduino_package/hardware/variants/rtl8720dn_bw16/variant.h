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

/* BW16/BW16-TypeC (RTL8720DN) */

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

#define TOTAL_GPIO_PIN_NUM                      (13)
#define TOTAL_PWM_PIN_NUM                       (4)

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                                  0  // PA_7
#define AMB_D1                                  1  // PA_8
#define AMB_D2                                  2  // PA_27
#define AMB_D3                                  3  // PA_30
#define AMB_D4                                  4  // PB_1
#define AMB_D5                                  5  // PB_2
#define AMB_D6                                  6  // PB_3
#define AMB_D7                                  7  // PA_25
#define AMB_D8                                  8  // PA_26
#define AMB_D9                                  9  // PA_15
#define AMB_D10                                 10 // PA_14
#define AMB_D11                                 11 // PA_13
#define AMB_D12                                 12 // PA_12

#define PA7                                     AMB_D0
#define PA8                                     AMB_D1
#define PA27                                    AMB_D2
#define PA30                                    AMB_D3
#define PB1                                     AMB_D4
#define PB2                                     AMB_D5
#define PB3                                     AMB_D6
#define PA25                                    AMB_D7
#define PA26                                    AMB_D8
#define PA15                                    AMB_D9
#define PA14                                    AMB_D10
#define PA13                                    AMB_D11
#define PA12                                    AMB_D12

/* Analog pin mapping */
//#define A0                                    AMB_D4 // PB_1 3.3V    CH4 //Not available connect with USB chip
//#define A1                                    AMB_D5 // PB_2 3.3V    CH5 //Not available connect with USB chip
#define A2                                      AMB_D6 // PB_3 3.3V    CH6

/* LED pin mapping */
#define LED_G                                   AMB_D10 // AMB_D10  Green
#define LED_R                                   AMB_D12 // AMB_D12  Red
#define LED_B                                   AMB_D11 // AMB_D11  Blue
#define LED_BUILTIN                             LED_G   // AMB_D11  Green

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                                AMB_D12 // AMB_D12
#define SPI_MISO                                AMB_D11 // AMB_D11
#define SPI_SCLK                                AMB_D10 // AMB_D10
#define SPI_SS                                  AMB_D9  // AMB_D9

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 AMB_D8 // AMB_D8
#define I2C_SCL                                 AMB_D7 // AMB_D7

/* IR Device pin mapping */
#define IR_TX                                   AMB_D7 // AMB_D7
#define IR_RX                                   AMB_D8 // AMB_D8

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  AMB_D0 // UART_LOG_TX  AMB_D0
#define LOG_RX                                  AMB_D1 // UART_LOG_RX  AMB_D1
//#define SERIAL1_TX                                   // HS_UART0_TX  not available use LP_UART as SERIAL1
//#define SERIAL1_RX                                   // HS_UART0_RX  not available use LP_UART as SERIAL1
#define SERIAL1_TX                              AMB_D4 // LP_UART_TX   AMB_D4 / AMB_D8 / AMB_D12
#define SERIAL1_RX                              AMB_D5 // LP_UART_RX   AMB_D5 / AMB_D7 / AMB_D11

/* SWD pin mapping */
#define SWD_DATA                                AMB_D2
#define SWD_CLK                                 AMB_D6

#endif /* _VARIANT_ARDUINO_AMEBA_ */
