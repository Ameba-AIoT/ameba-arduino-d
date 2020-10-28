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
    PA_0  = PIN_A0,
    PA_1  = PIN_A1,
    PA_2  = PIN_A2,
    PA_3  = PIN_A3,
    PA_4  = PIN_A4,
    PA_5  = PIN_A5,
    PA_6  = PIN_A6,
    PA_7  = PIN_A7,
    PA_8  = PIN_A8,
    PA_9  = PIN_A9,
    PA_10 = PIN_A10,
    PA_11 = PIN_A11,
    PA_12 = PIN_A12,
    PA_13 = PIN_A13,

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
