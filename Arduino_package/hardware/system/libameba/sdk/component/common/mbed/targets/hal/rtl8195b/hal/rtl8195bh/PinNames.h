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
    PB_0  = PIN_B0,
    PB_1  = PIN_B1,
    PB_2  = PIN_B2,
    PB_3  = PIN_B3,
    PB_4  = PIN_B4,
    PB_5  = PIN_B5,
    PB_6  = PIN_B6,
    PB_7  = PIN_B7,

    PC_0  = PIN_C0,
    PC_1  = PIN_C1,
    PC_2  = PIN_C2,
    PC_3  = PIN_C3,
    PC_4  = PIN_C4,
    PC_5  = PIN_C5,
    PC_6  = PIN_C6,
    PC_7  = PIN_C7,
    PC_8  = PIN_C8,
    PC_9  = PIN_C9,
    PC_10 = PIN_C10,
    PC_11 = PIN_C11,

    PD_0  = PIN_D0,
    PD_1  = PIN_D1,
    PD_2  = PIN_D2,
    PD_3  = PIN_D3,
    PD_4  = PIN_D4,
    PD_5  = PIN_D5,
    PD_6  = PIN_D6,
    PD_7  = PIN_D7,
    PD_8  = PIN_D8,
    PD_9  = PIN_D9,
    PD_10 = PIN_D10,
    PD_11 = PIN_D11,
    PD_12 = PIN_D12,
    PD_13 = PIN_D13,
    PD_14 = PIN_D14,
    PD_15 = PIN_D15,

    PE_0  = PIN_E0,
    PE_1  = PIN_E1,
    PE_2  = PIN_E2,
    PE_3  = PIN_E3,
    PE_4  = PIN_E4,
    PE_5  = PIN_E5,
    PE_6  = PIN_E6,
    PE_7  = PIN_E7,
    PE_8  = PIN_E8,
    PE_9  = PIN_E9,
    PE_10 = PIN_E10,
    PE_11 = PIN_E11,
    PE_12 = PIN_E12,
    PE_13 = PIN_E13,
    PE_14 = PIN_E14,
    PE_15 = PIN_E15,

    PF_0  = PIN_F0,
    PF_1  = PIN_F1,
    PF_2  = PIN_F2,
    PF_3  = PIN_F3,
    PF_4  = PIN_F4,
    PF_5  = PIN_F5,
    PF_6  = PIN_F6,
    PF_7  = PIN_F7,
    PF_8  = PIN_F8,
    PF_9  = PIN_F9,
    PF_10 = PIN_F10,

    PG_0  = PIN_G0,
    PG_1  = PIN_G1,
    PG_2  = PIN_G2,
    PG_3  = PIN_G3,
    PG_4  = PIN_G4,
    PG_5  = PIN_G5,
    PG_6  = PIN_G6,
    PG_7  = PIN_G7,
    PG_8  = PIN_G8,
    PG_9  = PIN_G9,

    PH_0  = PIN_H0,
    PH_1  = PIN_H1,
    PH_2  = PIN_H2,
    PH_3  = PIN_H3,
    PH_4  = PIN_H4,
    PH_5  = PIN_H5,
    PH_6  = PIN_H6,
    PH_7  = PIN_H7,
    PH_8  = PIN_H8,
    PH_9  = PIN_H9,
    PH_10 = PIN_H10,
    PH_11 = PIN_H11,
    PH_12 = PIN_H12,
    PH_13 = PIN_H13,
    PH_14 = PIN_H14,

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
    NC = (int32_t)0xFFFFFFFF
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
