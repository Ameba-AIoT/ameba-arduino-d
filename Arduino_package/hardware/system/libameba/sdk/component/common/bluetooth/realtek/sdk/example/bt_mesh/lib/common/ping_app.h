/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ping_app.h
  * @brief    Head file for ping app.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2018-4-2
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PING_APP_H
#define _PING_APP_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "ping.h"

/**
 * @addtogroup PING_SERVER_APP
 * @{
 */

/**
 * @defgroup Ping_Exported_Macros Ping Exported Macros
 * @brief
 * @{
 */
#define PING_APP_TIMEOUT_MSG           100
/** @} */

/**
 * @defgroup Ping_Exported_Functions Ping Exported Functions
 * @brief
 * @{
 */
void ping_app_ping_cb(uint16_t src, uint16_t dst, uint8_t hops_forward, ping_pong_type_t type,
                      uint8_t init_ttl, uint8_t key_index, uint16_t pong_max_delay);
void ping_app_handle_timeout(void);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _PING_APP_H */
