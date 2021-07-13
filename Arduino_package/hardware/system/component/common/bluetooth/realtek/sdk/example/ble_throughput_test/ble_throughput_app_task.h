/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    application.h
  * @brief   Application task related interfaces.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Add Includes here **/

/** Add all public functions here **/
extern void ble_throughput_app_task_init(void);
extern void ble_throughput_app_task_deinit(void);

#ifdef __cplusplus
}
#endif

#endif

