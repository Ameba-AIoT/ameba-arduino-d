/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      app_flags.h
   * @brief     This file is used to config app functions.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _BT_MESH_DEVICE_MULTIPLE_PROFILE_APP_FLAGS_H_
#define _BT_MESH_DEVICE_MULTIPLE_PROFILE_APP_FLAGS_H_

#include "mesh_config.h"
#include <app_common_flags.h>

/**
 * @addtogroup MESH_APP_CONFIG
 * @{
 */

/**
 * @defgroup  Mesh_App_Config Mesh App Configuration
 * @brief This file is used to config app functions.
 * @{
 */
/*============================================================================*
 *                              Constants
 *============================================================================*/

#if defined(CONFIG_BT_MESH_SCATTERNET) && CONFIG_BT_MESH_SCATTERNET
/** @brief  Config APP LE link number */
#define BLE_SCATTERNET_APP_MAX_LINKS  2
//for amebaD/amebz2 support the max number of master or slave is 1
#define  BLE_SCATTERNET_PERIPHERAL_APP_MAX_LINKS   1 //for max link slave num
#define  BLE_SCATTERNET_CENTRAL_APP_MAX_LINKS      1 //for max link master num
/** @brief  Config the discovery table number of gcs_client */
#define BLE_SCATTERNET_APP_MAX_DISCOV_TABLE_NUM 40
#define F_BT_GAPS_CHAR_WRITEABLE            1
/** @brief  Config set physical: 0-Not built in, 1-built in, use user command to set*/
#if defined(CONFIG_PLATFORM_8721D)
#define F_BT_LE_5_0_SET_PHY_SUPPORT         1
#elif defined(CONFIG_PLATFORM_8710C)
#define F_BT_LE_5_0_SET_PHY_SUPPORT         0
#endif

#else
/** @brief  Config APP LE link number */
#define APP_MAX_LINKS  1
/** @brief  Config airplane mode support: 0-Not built in, 1-built in, use user command to set*/
#define F_BT_AIRPLANE_MODE_SUPPORT          0
/** @brief  Config device name characteristic and appearance characteristic property: 0-Not writeable, 1-writeable, save to flash*/
#define F_BT_GAPS_CHAR_WRITEABLE            0
/** @brief  Config set physical: 0-Not built in, 1-built in, use user command to set*/
#define F_BT_LE_5_0_SET_PHY_SUPPORT         0

/** @} */
/** @} */
#endif
#endif