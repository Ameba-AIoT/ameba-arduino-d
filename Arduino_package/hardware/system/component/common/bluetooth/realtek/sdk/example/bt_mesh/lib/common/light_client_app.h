/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_client_app.h
* @brief     Smart mesh light demo application
* @details
* @author    bill
* @date      2018-1-4
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_CLIENT_APP_H
#define _LIGHT_CLIENT_APP_H

#include "mesh_api.h"
#include "light_cwrgb.h"
#include "light_lightness.h"
#include "light_ctl.h"
#include "light_hsl.h"
#include "light_xyl.h"
#include "light_lc.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CLIENTS_APP
 * @{
 */

/**
 * @defgroup Light_Clients_Exported_Types Light Clients Exported Types
 * @brief
 * @{
 */
extern mesh_model_info_t light_cwrgb_client;
extern mesh_model_info_t light_lightness_client;
extern mesh_model_info_t light_ctl_client;
extern mesh_model_info_t light_hsl_client;
extern mesh_model_info_t light_xyl_client;
extern mesh_model_info_t light_lc_client;
/** @} */

/**
 * @defgroup Light_Clients_Exported_Functions Light Clients Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize light client models
 */
void light_client_models_init(void);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CLIENT_APP_H */


