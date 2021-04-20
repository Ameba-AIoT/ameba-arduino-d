/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      time_server_app.h
* @brief     Smart mesh time demo application
* @details
* @author    
* @date      2020-3-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _TIME_SERVER_APP_H
#define _TIME_SERVER_APP_H

#include "delay_execution.h"
#if MODEL_ENABLE_DELAY_MSG_RSP
#include "delay_msg_rsp.h"
#endif
#include "time_model.h"


BEGIN_DECLS
/**
 * @defgroup TIME_Clients_Exported_Functions Time Clients Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize time server models
 */
void time_server_models_init(void);
/** @} */
/** @} */

END_DECLS

#endif /* _TIME_CLIENT_APP_H */


