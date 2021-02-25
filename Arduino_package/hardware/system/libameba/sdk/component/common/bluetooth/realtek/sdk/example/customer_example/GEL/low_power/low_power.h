/**
*****************************************************************************************
*     Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     low_power.h
  * @brief    Source file of idle check function specific for scenario together with wifi.
  * @details  User command interfaces.
  * @author   sherman_sun
  * @date     2020_11_06
  * @version  v1.0
  * *************************************************************************************
  */
#ifndef _LOWER_POWER_H_
#define _LOWER_POWER_H_

#include "device_idle_check.h"
#include "platform_os.h"

/**
  * @for init resources of power saving task.
  *
  * @param[in] none
  * @return result None
  */
void bt_mesh_power_saving_init(void);

/**
  * @for deinit power saving task.
  *
  * @param[in] none
  * @return result None
  */
void bt_mesh_power_saving_deinit(void);

#endif


