/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _BT_MESH_CMD_TYPES_H_
#define _BT_MESH_CMD_TYPES_H_

#define GEN_MESH_HANDLER(cmd)	{user_cmd ## cmd},

struct bt_mesh_api_hdl {
	user_cmd_parse_result_t (*mesh_func)(user_cmd_parse_value_t *pparse_value);	
};

#endif

