/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _BT_MESH_PROVISIONER_MULTIPLE_PROFILE_APP_TASK_H_
#define _BT_MESH_PROVISIONER_MULTIPLE_PROFILE_APP_TASK_H_

#include "mesh_config.h"
void bt_mesh_scatternet_send_msg(uint16_t sub_type);
/**
 * @brief  Initialize App task
 * @return void
 */
void bt_mesh_provisioner_multiple_profile_app_task_init(void);

/**
 * @brief  Deinitialize App task
 * @return void
 */
void bt_mesh_provisioner_multiple_profile_app_task_deinit(void);

#endif

