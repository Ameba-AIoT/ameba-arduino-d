/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     airsync_ble_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author   ethan
  * @date     2015-5-10
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _AIRSYNC_BLE_SERVICE_H_
#define _AIRSYNC_BLE_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"

/** @addtogroup RTK_Profile_Module RTK Profile Module
  * @{
  */

///@cond
/** @brief  Demo Profile service related UUIDs. */
#define GATT_UUID_AIRSYNC_SERVICE                                0xFEE7
#define GATT_UUID_AIRSYNC_CHAR_WRITE                             0xFEC7
#define GATT_UUID_AIRSYNC_CHAR_INDICATION                        0xFEC8
#define GATT_UUID_AIRSYNC_CHAR_READ                              0xFEC9



/** @brief  Index of each characteristic in Demo Profile service database. */
#define GATT_UUID_AIRSYNC_CHAR_WRITE_INDEX               0x02
#define GATT_UUID_AIRSYNC_CHAR_INDICATION_INDEX          0x04
#define GATT_UUID_AIRSYNC_CHAR_INDICATION_CCCD_INDEX          (GATT_UUID_AIRSYNC_CHAR_INDICATION_INDEX + 1)
#define GATT_UUID_AIRSYNC_CHAR_READ_INDEX                0x07

#define GATT_UUID_AIRSYNC_CHAR_INDICATION_ENABLE 0x01
#define GATT_UUID_AIRSYNC_CHAR_INDICATION_DISABLE 0x02
///@endcond

/** @} */

/** Write content */
typedef struct _TWAS_WRITE_MSG
{
    uint8_t opcode;
    uint16_t length;
    uint8_t *pValue;
} T_WAS_WRITE_MSG;

/** Message content: @ref SIMP_Upstream_Message */
typedef union _TWAS_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index;
    T_WAS_WRITE_MSG write;
} T_WAS_UPSTREAM_MSG_DATA;

/** Simple profile service data to inform application */
typedef struct _TWAS_CALLBACK_DATA
{
    uint8_t                     conn_id;
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_WAS_UPSTREAM_MSG_DATA     msg_data;
} T_WAS_CALLBACK_DATA;
/** @} */

/** @} End of SIMP_Exported_Types */

/** @defgroup SIMP_Exported_Functions SIMP Exported Functions
  * @brief functions that other .c files may use all defined here
  * @{
  */
/* add Simple Profile to application */
T_SERVER_ID airsync_add_service(void *p_func);

/** @} End of SIMP_Exported_Functions */

/** @} End of SIMP */

/** @} End of RTK_Profile_Module */

#ifdef __cplusplus
}
#endif

#endif /* _SIMPLE_BLE_SERVICE_H_ */
