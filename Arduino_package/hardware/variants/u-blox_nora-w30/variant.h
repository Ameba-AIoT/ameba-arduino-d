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
//               index into g_APinDescription --v     AMEBA   NORA      Function
#define EVK_D0                                  17 // PB_18   NORA_G9   HS_UART_RXD
#define EVK_D1                                  16 // PB_19   NORA_G8   HS_UART_TXD
#define EVK_D2                                  19 // PB_20   NORA_F9   HS_I2C_SCL / HS_UART_CTS
#define EVK_D3                                  18 // PB_21   NORA_F8   HS_I2C_SDA / HS_UART_RTS
//      EVK_D4 .. D9                            NC
#define EVK_D10                                 0  // PA_15   NORA_H8   SPI_CS
#define EVK_D11                                 3  // PA_12   NORA_J9   SPI_MOSI
#define EVK_D12                                 2  // PA_13   NORA_J8   SPI_MISO
#define EVK_D13                                 1  // PA_14   NORA_H9   SPI_SCLK

#define EVK_D14                                 11 // PB_1    NORA_D8   BTN2 / LP_UART_RXD
#define EVK_D15                                 10 // PB_2    NORA_E8   BTN1 / LP_UART_TXD
#define EVK_D16                                 14 // PB_23   NORA_C8   LED_BLUE / LP_TIM5_TRIG
#define EVK_D17                                 15 // PB_22   NORA_E9   LED_GREEN / LP_TIM4_TRIG
#define EVK_D18                                 12 // PA_7    NORA_A5   UART_LOG_TX
#define EVK_D19                                 13 // PA_8    NORA_A6   UART_LOG_RX 
// Analog pin mapping
#define A0                                      9  // PB_3    NORA_J2   SWDCLK
#define A1                                      4  // PA_30   NORA_B1   LED_RED
#define A2                                      8  // PA_27   NORA_H2   SWDIO
#define A3                                      5  // PA_28   NORA_B3   USB_RREF
#define A4                                      6  // PA_26   NORA_C1   LP_I2C_SDA
#define A5                                      7  // PA_25   NORA_C2   LP_I2C_SCL

// NORA-W30 module pad names
#define NORA_A5                                 12 // PA_7    NORA_A5   UART_LOG_TX 
#define NORA_A6                                 13 // PA_8    NORA_A6   UART_LOG_RX
#define NORA_B1                                 4  // PA_30   NORA_B1   LED_RED
#define NORA_B3                                 5  // PA_28   NORA_B3   USB_RREF
#define NORA_C1                                 6  // PA_26   NORA_C1   LP_I2C_SDA
#define NORA_C2                                 7  // PA_25   NORA_C2   LP_I2C_SCL
#define NORA_C8                                 14 // PB_23   NORA_C8   LED_BLUE / LP_TIM5_TRIG
#define NORA_D8                                 11 // PB_1    NORA_D8   BTN2 / LP_UART_RXD
#define NORA_E8                                 10 // PB_2    NORA_E8   BTN1 / LP_UART_TXD
#define NORA_E9                                 15 // PB_22   NORA_E9   LED_GREEN / LP_TIM4_TRIG
#define NORA_F8                                 18 // PB_21   NORA_F8   HS_I2C_SDA / HS_UART_RTS
#define NORA_F9                                 19 // PB_20   NORA_F9   HS_I2C_SCL / HS_UART_CTS
#define NORA_G8                                 16 // PB_19   NORA_G8   HS_UART_TXD
#define NORA_G9                                 17 // PB_18   NORA_G9   HS_UART_RXD
#define NORA_H2                                 8  // PA_27   NORA_H2   SWDIO
#define NORA_H8                                 0  // PA_15   NORA_H8   SPI_CS
#define NORA_H9                                 1  // PA_14   NORA_H9   SPI_SCLK
#define NORA_J2                                 9  // PB_3    NORA_J2   SWDCLK
#define NORA_J8                                 2  // PA_13   NORA_J8   SPI_MISO
#define NORA_J9                                 3  // PA_12   NORA_J9   SPI_MOSI

/* LED pin mapping */
#define LED_BUILTIN                             14 // PB_23   NORA_C8   LED_BLUE / LP_TIM5_TRIG
#define LED_RED                                 4  // PA_30   NORA_B1   LED_RED
#define LED_GREEN                               15 // PB_22   NORA_E9   LED_GREEN / LP_TIM4_TRIG
#define LED_BLUE                                14 // PB_23   NORA_C8   LED_BLUE / LP_TIM5_TRIG

/* Button pin mapping */
#define BTN1                                    10 // PB_2    NORA_E8   BTN1 / LP_UART_TXD
#define BTN2                                    11 // PB_1    NORA_D8   BTN2 / LP_UART_RXD

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */
#define SPI_MOSI                                3  // PA_12   NORA_J9   SPI_MOSI
#define SPI_MISO                                2  // PA_13   NORA_J8   SPI_MISO
#define SPI_SCLK                                1  // PA_14   NORA_H9   SPI_SCLK
#define SPI_SS                                  0  // PA_15   NORA_H8   SPI_CS

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 18 // PB_21   NORA_F8   HS_I2C_SDA / HS_UART_RTS
#define I2C_SCL                                 19 // PB_20   NORA_F9   HS_I2C_SCL / HS_UART_CTS
#define I2C1_SDA                                6  // PA_26   NORA_C1   LP_I2C_SDA
#define I2C1_SCL                                7  // PA_25   NORA_C2   LP_I2C_SCL

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  12 // PA_7    NORA_A5   UART_LOG_TX 
#define LOG_RX                                  13 // PA_8    NORA_A6   UART_LOG_RX
#define SERIAL1_TX                              16 // PB_19   NORA_G8   HS_UART_TXD
#define SERIAL1_RX                              17 // PB_18   NORA_G9   HS_UART_RXD
#define SERIAL2_TX                              11 // PB_1    NORA_D8   BTN2 / LP_UART_RXD
#define SERIAL2_RX                              10 // PB_2    NORA_E8   BTN1 / LP_UART_TXD

#endif /* _VARIANT_ARDUINO_AMEBA_ */
