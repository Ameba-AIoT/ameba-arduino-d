/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_server_app.h
* @brief     Smart mesh light demo application
* @details
* @author    
* @date      2020-3-25
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_SERVER_APP_H
#define _LIGHT_SERVER_APP_H

#include "light_lightness.h"
#include "delay_execution.h"
#if MODEL_ENABLE_DELAY_MSG_RSP
#include "delay_msg_rsp.h"
#endif
#include "light_ctl.h"
#include "light_hsl.h"
#include "light_lightness.h"
#include "light_xyl.h"

BEGIN_DECLS
/**
 * @defgroup Light_Clients_Exported_Functions Light Clients Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize light server models
 */
void light_server_models_init(void);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CLIENT_APP_H */


