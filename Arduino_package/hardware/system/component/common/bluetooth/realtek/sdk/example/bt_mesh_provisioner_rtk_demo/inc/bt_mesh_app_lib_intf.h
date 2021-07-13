#ifndef _BT_MESH_APP_LIB_INTF_H_
#define _BT_MESH_APP_LIB_INTF_H_

#include "osdep_service.h"
#include "app_msg.h"

typedef enum
{
	API_PB_ADV_LINK_OPEN = 0x0,
	API_PROV_INVITE = 0x1,
	API_CFG_APP_KEY_ADD = 0x2,
	API_CFG_MODEL_APP_BIND = 0x3,
	API_CFG_MODEL_PUB_SET = 0x4,
	API_CFG_MODEL_SUB_ADD = 0x5,
	API_CFG_MODEL_SUB_GET = 0x6,
	API_CFG_MODEL_SUB_DELETE_ALL = 0x7,
	API_CFG_NODE_RESET = 0x8,
	API_GENERIC_ON_OFF_GET = 0x9,
	API_GENERIC_ON_OFF_SET = 0xA,
	API_GENERIC_ON_OFF_PUBLISH = 0xB,
} T_MESH_API_TYPE;

struct BT_MESH_LIB_PRIV {
	_sema connect_device_sema;
	_sema connect_device_goog_sema;
	_sema connect_device_nr_sema;
	_sema delete_node_sema;
	_sema set_node_state_sema;
	_sema set_node_group_sema;

	uint16_t connect_device_mesh_addr;
	uint16_t connect_device_goog_mesh_addr;
	uint16_t connect_device_nr_mesh_addr;
	uint16_t delete_node_mesh_addr;
	uint16_t set_node_state_mesh_addr;
	uint16_t set_node_group_mesh_addr;

	uint8_t connect_device_flag;
	uint8_t set_node_group_flag;

	uint8_t connect_device_goog_light_state;
	uint8_t set_node_state_light_state;
};

uint8_t bt_mesh_cmd_start_provisioner_api(void);

uint8_t bt_mesh_cmd_connect_device_api(uint8_t *dev_uuid, uint16_t *mesh_addr, uint8_t *light_state);

uint8_t bt_mesh_cmd_delete_node_api(uint16_t mesh_addr);

uint8_t bt_mesh_cmd_set_node_state_api(uint16_t mesh_addr, uint16_t group_addr, uint8_t light_state, uint8_t flag);

uint8_t bt_mesh_cmd_get_node_state_api(uint16_t mesh_addr);

uint8_t bt_mesh_cmd_set_node_group_api(uint16_t mesh_addr, uint16_t group_addr);

uint8_t bt_mesh_cmd_get_node_group_api(uint16_t mesh_addr);

#endif

