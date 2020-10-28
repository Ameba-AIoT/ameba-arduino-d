/**************************************************************************//**
 * @file     PinNames.h
 * @brief    Mbed HAL API pin naming definition.
 * @version  V1.00
 * @date     2017-05-03
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef _PINNAMES_H_
#define _PINNAMES_H_

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTL_PIN_PERI(FUN, IDX, SEL)     ((int)(((FUN) << 8) | ((IDX)<<4) | (SEL)))
#define RTL_PIN_FUNC(FUN, SEL)          ((int)(((FUN) << 7) | (SEL)))
#define RTL_GET_PERI_SEL(peri)          ((int)((peri)&0x0F))
#define RTL_GET_PERI_IDX(peri)          ((int)(((peri) >> 4)&0x0F))

typedef enum {
    PIN_INPUT=0,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PA_0    = PIN_A0,
    PA_1    = PIN_A1,
    PA_2    = PIN_A2,
    PA_3    = PIN_A3,
    PA_4    = PIN_A4,
    PA_5    = PIN_A5,
    PA_6    = PIN_A6,
    PA_7    = PIN_A7,
    PA_8    = PIN_A8,
    PA_9    = PIN_A9,
    PA_10   = PIN_A10,
    PA_11   = PIN_A11,
    PA_12   = PIN_A12,
    PA_13   = PIN_A13,
    PA_14   = PIN_A14,
    PA_15   = PIN_A15,
    PA_16   = PIN_A16,
    PA_17   = PIN_A17,
    PA_18   = PIN_A18,
    PA_19   = PIN_A19,
    PA_20   = PIN_A20,
    PA_21   = PIN_A21,
    PA_22   = PIN_A22,
    PA_23   = PIN_A23,

    PB_0    = PIN_B0,
    PB_1    = PIN_B1,
    PB_2    = PIN_B2,
    PB_3    = PIN_B3,
    PB_4    = PIN_B4,
    PB_5    = PIN_B5,
    PB_6    = PIN_B6,
    PB_7    = PIN_B7,
    PB_8    = PIN_B8,
    PB_9    = PIN_B9,
    PB_10   = PIN_B10,
    PB_11   = PIN_B11,
    PB_12   = PIN_B12,

    // Arduino connector namings
/*
    A0          = PA_0,
    A1          = PA_1,
    A2          = PA_4,
    A3          = PB_0,
    A4          = PC_1,
    A5          = PC_0,
    D0          = PA_3,
    D1          = PA_2,
    D2          = PA_10,
    D3          = PB_3,
    D4          = PB_5,
    D5          = PB_4,
    D6          = PB_10,
    D7          = PA_8,
    D8          = PA_9,
    D9          = PC_7,
    D10         = PB_6,
    D11         = PA_7,
    D12         = PA_6,
    D13         = PA_5,
    D14         = PB_9,
    D15         = PB_8,
*/

    // Generic signals namings
/*
    LED1        = PB_4,
    LED2        = PB_5,
    LED3        = PB_6,
    LED4        = PB_7,
    USER_BUTTON = PA_3,
    SERIAL_TX   = PA_7,
    SERIAL_RX   = PA_6,
    USBTX       = PA_7,
    USBRX       = PA_6,
    I2C_SCL     = PC_5,
    I2C_SDA     = PC_4,
    SPI_MOSI    = PC_2,
    SPI_MISO    = PC_3,
    SPI_SCK     = PC_1,
    SPI_CS      = PC_0,
    PWM_OUT     = PD_4,
*/
    // Not connected
    NC = (uint32_t)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone  = 0,
    PullUp    = 1,
    PullDown  = 2,
    OpenDrain = 3,
    PullDefault = PullNone
} PinMode;

#define PORT_NUM(pin)       (((uint8_t)(pin) >> 5) & 0x7)
#define PIN_NUM(pin)        ((uint8_t)(pin) & 0x1F)

#ifdef __cplusplus
}
#endif

#endif
