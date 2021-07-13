#ifndef _BT_MESH_APP_LIST_INTF_H_
#define _BT_MESH_APP_LIST_INTF_H_

#include "osdep_service.h"

struct BT_MESH_DEVICE_INFO {
	struct list_head list;
	uint8_t dev_uuid[16];
	uint16_t mesh_addr;
	uint16_t group_addr;
	uint32_t last_scan_time;
	uint32_t last_active_time;
	uint8_t connect_status;		//0: Fail, 1: Success
	uint8_t delete_status;		//0: Fail, 1: Success
	uint8_t active_state;		//0: Inactive, 1: Active
	uint8_t light_state;		//0: Off, 1: On
	uint16_t target_group_addr;
	uint8_t target_light_state;	//0: Off, 1: On
	uint8_t get_group_addr_flag;//0: No need to get group address, 1: Need to get group address
	uint8_t set_node_state_flag;//0: Use mesh address, 1: Use group address
};

struct BT_MESH_PRIV {
	_list unproed_dev_list;
	_list proed_active_dev_list;
	_list proed_inactive_dev_list;
	_list processing_dev_list;

	_lock dev_list_lock;

	_timer list_flushing_timer;

	bool provisioner_started_flag;
};

typedef struct meshFlushInfo
{
    struct list_head          meshFlushList;            //!< List contains flush contants.
    uint8_t                   meshFlushEvent;           //!< Indicate currently flush event.
    uint8_t                   eventListNum;             //!< user command num included in .meshCmdList.  
    _mutex                    eventMutex;               //!< mesh flush private struct mutex.
} MESH_FLUSH_INFO_S;

typedef struct meshFlushItem
{
    struct list_head          list;                     //!< list of .meshFlushItem.
    struct list_head          *plist;                   //!< pointer to list.
    uint8_t                   meshFlushEvent;           //!< Indicate currently flush event.
} MESH_FLUSH_ITEM_S;

enum
{
    CONNECTION_CHECK_EVENT ,
    GET_NODE_GROUP ,
    COMBO_CHECK
};

void init_bt_mesh_priv(void);

void add_unproed_dev(uint8_t *dev_uuid);

void update_proed_dev(uint16_t mesh_addr, uint8_t light_state);

void update_node_group(uint16_t mesh_addr, uint16_t group_addr);

/* RTW_BT_CMD_START_PROVISIONER */
void bt_mesh_cmd_start_provisioner_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_GET_UNPROVISIONED_DEVICE */
void bt_mesh_cmd_get_unprovisioned_device_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_CONNECT_DEVICE */
void bt_mesh_cmd_connect_device_pre_handler(uint8_t *dev_uuid);
void bt_mesh_cmd_connect_device_handler(void);
void bt_mesh_cmd_connect_device_post_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_DELETE_NODE */
void bt_mesh_cmd_delete_node_pre_handler(uint16_t mesh_addr);
void bt_mesh_cmd_delete_node_handler(void);
void bt_mesh_cmd_delete_node_post_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_GET_NODE_STATUS */
void bt_mesh_cmd_get_node_status_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_SET_NODE_STATE */
void bt_mesh_cmd_set_node_state_pre_handler(uint16_t mesh_addr, uint8_t light_state);
void bt_mesh_cmd_set_node_state_handler(void);
void bt_mesh_cmd_set_node_state_post_handler(uint8_t *bt_mesh_event_value);

/* RTW_BT_CMD_SET_NODE_GROUP */
void bt_mesh_cmd_set_node_group_pre_handler(uint16_t mesh_addr, uint16_t group_addr);
void bt_mesh_cmd_set_node_group_handler(void);
void bt_mesh_cmd_set_node_group_post_handler(uint8_t *bt_mesh_event_value);

uint8_t bt_mesh_enqueue_flush_event(struct list_head *queue);
struct list_head *bt_mesh_dequeue_flush_event(void);
uint8_t bt_mesh_demo_start_drv_thread(void);
void bt_mesh_demo_stop_drv_thread(void);

#endif

