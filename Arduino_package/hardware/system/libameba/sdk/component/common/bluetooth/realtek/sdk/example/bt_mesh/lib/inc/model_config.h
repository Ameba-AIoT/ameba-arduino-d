/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     model_config.h
* @brief    Head file for model configuration.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-12-4
* @version  v1.0
* *************************************************************************************
*/

#ifndef _MODEL_CONFIG_H_
#define _MODEL_CONFIG_H_

#include "mesh_config.h"

/* always publish regardless of state change */
#define MODEL_ENABLE_PUBLISH_ALL_TIME                      1

/* enable delay message response */
#define MODEL_ENABLE_DELAY_MSG_RSP                         1

/* enable  parallel adv */
#define MODEL_ENABLE_PARALLEL_ADV                          1

/* set this value to 1 if need to notify application when user stop transition */
#define MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION     0

/* set this value to 1 if need to notify model when user stop delay execution */
#define MODEL_ENABLE_USER_STOP_DELAY_NOTIFICATION          0
#if MODEL_ENABLE_USER_STOP_DELAY_NOTIFICATION && MESH_MODEL_ENABLE_DEINIT
#error must NOT enable both of MODEL_ENABLE_USER_STOP_DELAY_NOTIFICATION and MESH_MODEL_ENABLE_DEINIT
#endif

/* do not modify this field */
#define MODEL_ENABLE_MULTI_THREAD                          0

#endif /* _MODEL_CONFIG_H_ */

