/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      generic_client_app.h
* @brief     Smart mesh generic models demo application
* @details
* @author    bill
* @date      2018-1-10
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _GENERIC_CLIENT_APP_H
#define _GENERIC_CLIENT_APP_H

#include "generic_default_transition_time.h"
#include "generic_on_off.h"
#include "generic_level.h"
#include "generic_power_level.h"
#include "generic_power_on_off.h"
#include "generic_battery.h"
#include "health.h"
#include "time_model.h"
#include "scene.h"
#include "sensor.h"
#include "scheduler.h"
#include "generic_location.h"
#include "generic_property.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_CLIENTS_APP
 * @{
 */

/**
 * @defgroup Generic_Clients_Exported_Types Generic Clients Exported Types
 * @brief
 * @{
 */
extern mesh_model_info_t model_gdtt_client;
extern mesh_model_info_t model_goo_client;
extern mesh_model_info_t model_gl_client;
extern mesh_model_info_t model_gpoo_client;
extern mesh_model_info_t model_gpl_client;
extern mesh_model_info_t model_gb_client;
extern mesh_model_info_t model_health_client;
extern mesh_model_info_t model_time_client;
extern mesh_model_info_t model_scene_client;
extern mesh_model_info_t model_sensor_client;
extern mesh_model_info_t model_scheduler_client;
extern mesh_model_info_t model_generic_location_client;
extern mesh_model_info_t model_gp_client;
/** @} */

/**
 * @defgroup Generic_Clients_Exported_Functions Generic Clients Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize generic client models
 */
void generic_client_models_init(void);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_CLIENT_APP_H */


