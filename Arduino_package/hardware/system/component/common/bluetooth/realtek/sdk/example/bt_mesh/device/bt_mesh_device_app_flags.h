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
#ifndef _BT_MESH_DEVICE_APP_FLAGS_H_
#define _BT_MESH_DEVICE_APP_FLAGS_H_

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

/** @brief  Config APP LE link number */
#define APP_MAX_LINKS  1
/** @brief  Config airplane mode support: 0-Not built in, 1-built in, use user command to set*/
#define F_BT_AIRPLANE_MODE_SUPPORT          0
/** @brief  Config set physical: 0-Not built in, 1-built in, use user command to set*/
#define F_BT_LE_5_0_SET_PHY_SUPPORT         0

/** @} */
/** @} */
#endif
