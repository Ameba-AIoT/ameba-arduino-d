#ifndef _BT8761_H_
#define _BT8761_H_

#include <inttypes.h>

#define BT_EV_LE_AUTHEN_COMPLETE (1)
#define BT_EV_LINK_KEY_REQ       (2)
#define BT_EV_APP_DATA           (3)
#define BT_EV_APP_READ_IND       (4)

typedef struct _bt_status_t {
    uint8_t gap_init_state;
    uint8_t gap_adv_sub_state;
    uint8_t gap_adv_state;
    uint8_t gap_conn_state;
    uint8_t gap_scan_state;
    uint8_t radio_state;
    uint8_t spp_state;
} bt_status_t;

typedef struct _bt_addr_t {
    uint8_t addr_type;
    uint8_t bd_addr[6];
} bt_addr_t;

typedef struct _bt_link_key_req_t {
    uint8_t bd_addr[6];
    uint8_t remote_addr_type;
    uint8_t key_type;
} bt_link_key_req_t;

typedef struct _bt_link_key_t {
	uint8_t key_type;
    uint8_t link_key[28];
} bt_link_key_t;

typedef struct _bt_link_info_t {
    uint8_t valid;
	uint8_t bd_addr[6];
	uint8_t remote_addr_type;
    bt_link_key_t ltk;
    bt_link_key_t irk;
} bt_link_info_t;

#define BT8761_MAX_LINK_INFO_SIZE (4)
typedef struct _bt_link_table_t {
    uint8_t tblname[10];
    bt_link_info_t link_info[BT8761_MAX_LINK_INFO_SIZE];
} bt_link_table_t;

int bt8761_init(bt_link_table_t *tbl);
int bt8761_dl_patchcfg(uint8_t *patch, uint32_t patch_len);
int bt8761_read_bt_status(bt_status_t *bs);
int bt8761_ble_read_addr(bt_addr_t *addr);
int bt8761_set_adv_mode(uint8_t mode);
int bt8761_ble_adv_enable(uint8_t enable);
int bt8761_ble_set_name(char *name);
int bt8761_set_adv_data(uint8_t *data, uint8_t len);
int bt8761_hci_le_cmd_all_paired_device_info(bt_link_key_req_t *req);
int bt8761_hci_send_raw(uint16_t cmd, uint8_t *data, uint32_t len);

void bt8761_reg_event_listener( void (*callback)(uint32_t type, void *data, uint32_t len) );

#endif