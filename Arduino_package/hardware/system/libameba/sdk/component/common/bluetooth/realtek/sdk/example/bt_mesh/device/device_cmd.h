/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _DEV_CMD_H_
#define _DEV_CMD_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "mesh_data_uart.h"
#include "mesh_user_cmd_parse.h"
#include "mesh_config.h"
#if defined(CONFIG_BT_MESH_USER_API) && CONFIG_BT_MESH_USER_API
#include "bt_mesh_cmd_types.h"
#endif

/****************************************************************************************************************
* exported variables other .c files may use all defined here.
****************************************************************************************************************/
extern const user_cmd_table_entry_t device_cmd_table[];
#if defined(CONFIG_BT_MESH_USER_API) && CONFIG_BT_MESH_USER_API
extern const struct bt_mesh_api_hdl devicecmds[];
#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

