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
#ifndef _BT_FUZZ_TEST_APP_FLAGS_H_
#define _BT_FUZZ_TEST_APP_FLAGS_H_

//#include "upperstack_config.h"

/** @defgroup  SCATTERNET_Config Scatternet App Configuration
    * @brief This file is used to config app functions.
    * @{
    */
/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @brief  Config APP LE link number */
#define BT_FUZZ_TEST_APP_MAX_LINKS  2
/** @brief  Config DLPS: 0-Disable DLPS, 1-Enable DLPS */
#define BT_FUZZ_TEST_F_BT_DLPS_EN                        0
/** @brief  Use general client: 0-Disable general client, 1-Enable general client */
#define BT_FUZZ_TEST_APP_GENERAL_CLIENT_TEST                  1
/** @brief  Config the discovery table number */
#define BT_FUZZ_TEST_APP_MAX_DISCOV_TABLE_NUM 50

/** @} */ /* End of group SCATTERNET_Config */
#endif
