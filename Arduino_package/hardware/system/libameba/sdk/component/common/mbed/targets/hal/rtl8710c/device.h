 /**************************************************************************//**
  * @file     device.h
  * @brief    Mbed HAL peripheral devices enable configuration
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

/** @addtogroup hal Mbed HAL API
 *  @ingroup    hal
 *  @brief      ARM defined Mbed HAL API and Realtek defined Mbed style extension API.
 *  @{
 */
#if defined(CONFIG_BUILD_SECURE) && (CONFIG_BUILD_SECURE == 1)
// Secure Region
#define DEVICE_PORTIN           0
#define DEVICE_PORTOUT          0
#define DEVICE_PORTINOUT        0
#define DEVICE_INTERRUPTIN      0
#define DEVICE_ANALOGIN         0
#define DEVICE_ANALOGOUT        0
#define DEVICE_SERIAL           1
#define DEVICE_I2C              0
#define DEVICE_I2CSLAVE         0
#define DEVICE_SPI              0
#define DEVICE_SPISLAVE         0
#define DEVICE_CAN              0
#define DEVICE_RTC              0
#define DEVICE_ETHERNET         0
#define DEVICE_PWMOUT           0
#define DEVICE_SLEEP            1
#else
// Non-Secure or Ignore Secure Region
#define DEVICE_PORTIN           1
#define DEVICE_PORTOUT          1
#define DEVICE_PORTINOUT        1
#define DEVICE_INTERRUPTIN      1
#define DEVICE_ANALOGIN         0
#define DEVICE_ANALOGOUT        0
#define DEVICE_SERIAL           1
#define DEVICE_I2C              1
#define DEVICE_I2CSLAVE         1
#define DEVICE_SPI              1
#define DEVICE_SPISLAVE         1
#define DEVICE_CAN              0
#define DEVICE_RTC              1
#define DEVICE_ETHERNET         0
#define DEVICE_PWMOUT           1
#define DEVICE_SLEEP            1
#endif

#include "objects.h"

/** @} */ /* End of group hal */

#endif
