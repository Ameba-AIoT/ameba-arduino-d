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

/* SparkFun Thing Plus NORA-W306 (RTL8720DF) */

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

#define TOTAL_GPIO_PIN_NUM                      (21)
#define TOTAL_PWM_PIN_NUM                       (12)

/* Digital pin mapping refer to g_APinDescription */
//               index into g_APinDescription --v     AMEBA   NORA      Function

// Analog pin mapping
#define A0                                      19 // PB_1
#define A1                                      8  // PB_3
#define A2                                      9  // PB_2

// NORA-W30 module pad names
#define NORA_A5                                 1  // PA_7    NORA_A5   DBG_TX 
#define NORA_A6                                 0  // PA_8    NORA_A6   DBG_RX
#define NORA_B1                                 11 // PA_30   NORA_B1   SPI_SCLK
#define NORA_B3                                 12 // PA_28   NORA_B3   SPI_CS
#define NORA_C1                                 7  // PA_26   NORA_C1   SPI_MISO
#define NORA_C2                                 6  // PA_25   NORA_C2   SPI_MOSI
#define NORA_C8                                 16 // PB_23   NORA_C8   SW2
#define NORA_D8                                 17 // PB_1    NORA_D8   SW1
#define NORA_E8                                 19 // PB_2    NORA_E8   UART_DTR
#define NORA_E9                                 15 // PB_22   NORA_E9   UART_DSR
#define NORA_F8                                 9  // PB_21   NORA_F8   I2C_SDA / UART_RTS
#define NORA_F9                                 10 // PB_20   NORA_F9   I2C_SCL / UART_CTS
#define NORA_G8                                 13 // PB_19   NORA_G8   UART_TXD
#define NORA_G9                                 14 // PB_18   NORA_G9   UART_RXD
#define NORA_H2                                 8  // PA_27   NORA_H2
#define NORA_H8                                 2  // PA_15   NORA_H8   LED_GREEN
#define NORA_H9                                 3  // PA_14   NORA_H9   LED_BLUE
#define NORA_J2                                 18 // PB_3    NORA_J2
#define NORA_J8                                 5  // PA_13   NORA_J8   LED_RED
#define NORA_J9                                 4  // PA_12   NORA_J9

/* LED pin mapping */
#define LED_BUILTIN                             18 // PB_22

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_PICO                               6  // PB_18
#define SPI_POCI                               7  // PB_19
#define SPI_SCLK                               5  // PB_20
#define SPI_SS                                 4  // PB_21

/* SPI1 pin mapping */
#define SPI1_PICO                              14  // PA_12
#define SPI1_POCI                              13  // PA_13
#define SPI1_SCLK                              16  // PA_14
#define SPI1_SS                                15  // PA_15

/* Naming modernization/backwards compatibility*/
#define SPI_MOSI SPI_PICO
#define SPI_MISO SPI_POCI
#define SPI1_MOSI SPI1_PICO
#define SPI1_MISO SPI1_POCI

/* "SS" is used in some libraries, e.g. SdFat. */
#define SS SPI_SS

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 11  // PA_26
#define I2C_SCL                                 12  // PA_25

/* IR Device pin mapping */
#define IR_TX                                   NC
#define IR_RX                                   NC

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  1  // PA_7
#define LOG_RX                                  0  // PA_8
#define SERIAL1_TX                              9  // PB_1
#define SERIAL1_RX                              8  // PB_2
#define SERIAL2_TX                              4  // PA_12
#define SERIAL2_RX                              5  // PA_13

#endif /* _VARIANT_ARDUINO_AMEBA_ */
