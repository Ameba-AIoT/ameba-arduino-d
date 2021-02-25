/**
*****************************************************************************************
*     Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     device_idle_check.h
  * @brief    Source file of idle check function specific for scenario together with wifi.
  * @details  User command interfaces.
  * @author   sherman_sun
  * @date     2020_11_06
  * @version  v1.0
  * *************************************************************************************
  */
#ifndef _DEVICE_IDLE_CHECK_H_
#define _DEVICE_IDLE_CHECK_H_

#include "mesh_api.h"
#include "platform_os.h"

/** @brief Indicate the result of power save enable */
typedef enum
{
    DEVICE_IDLE_CHECK_FALSE,
    DEVICE_IDLE_CHECK_TRUE
} device_idle_check_t;

/**
  * @for init resources of bt mesh idle check.
  *
  * @param[in] none
  * @return result 1: success 0: fail
  */
uint8_t bt_mesh_idle_check_init(void);

/**
  * @for deinit resources of bt mesh idle check.
  *
  * @param[in] none
  * @return result 1: success 0: fail
  */
uint8_t bt_mesh_idle_check_deinit(void);

/**
  * @get idle state of bt mesh.
  *
  * @param[in] none
  * @return result of device_enter_ps_enable_t
  */
device_idle_check_t bt_mesh_idle_check(void);

#endif


