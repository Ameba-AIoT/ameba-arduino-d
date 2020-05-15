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

/** set this value to 1 if enable delay execution module */
#define MODEL_ENABLE_DELAY_EXECUTION                       0

/** set this value to 1 if need to notify application when user stop transition */
#define MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION     0

/** set this value to 1 if need to notify model when user stop delay execution */
/** note: this value must stay in 0 if use model deinit module */
#define MODEL_ENABLE_USER_STOP_DELAY_NOTIFICATION          0

/** do not modify this field unless you really want to use model functions in different threads */
#define MODEL_ENABLE_MULTI_THREAD                          0

#endif /** _MODEL_CONFIG_H_ */

