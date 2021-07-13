#ifndef  _HRP_BTIF_SYSTEM_API_H_
#define  _HRP_BTIF_SYSTEM_API_H_

#include <hrp.h>
#include <hrp_utils.h>

typedef enum
{
    HRP_SYSTEM_RESET_REQ = 0,
    HRP_SYSTEM_RESET_RSP = 1,

    HRP_SYSTEM_INIT_RSP = 3,
    HRP_SYSTEM_L2C_DATA_TEST_CMPL_INFO = 5,
    HRP_SYSTEM_LE_CFG_PASSKEY_VALUE_RSP = 7,
    HRP_SYSTEM_EVENT_ACK = 8,
    HRP_SYSTEM_CMD_ACK = 9,

    HRP_SYSTEM_ENABLE_DLPS_RSP = 0x0B,
    HRP_SYSTEM_READ_DLPS_COUNT_REQ = 0x0C,
    HRP_SYSTEM_READ_DLPS_COUNT_RSP = 0x0D,

} HRP_SYSTEM_CMD_ENTRY;

typedef enum
{
    HRP_SYSTEM_COMMAND_COMPLETE = 0,
    HRP_SYSTEM_COMMAND_DISALLOW = 1,
    HRP_SYSTEM_UNKNOWN_COMMAND = 2,
} HRP_SYSTEM_CMD_STATUS;

typedef struct
{
    uint16_t tx_cid;
    uint16_t tx_data_offset;
    uint16_t tx_length;
    uint16_t tx_total_count;    /*if tx_total_count > 0, send data*/
    uint8_t  tx_next_byte;
    uint16_t tx_max_credits;

    uint16_t tx_available_credits;
    uint16_t tx_sent_count;

    uint16_t rx_cid;
    uint16_t rx_length;
    uint16_t rx_total_count;    /*if rx_total_count > 0, receive data*/
    uint8_t  rx_next_byte;

    uint16_t rx_rcved_count;

    bool is_testing;

    //uint32_t tx_start_timestamp;
    //uint32_t tx_stop_timestamp;
    //uint32_t rx_start_timestamp;
    //uint32_t rx_stop_timestamp;
} T_L2C_DATA_TRANS;

typedef struct
{
    uint16_t tx_cid;
    uint16_t tx_data_offset;
    uint16_t tx_length;
    uint16_t tx_total_count;    /*if tx_total_count > 0, send data*/
    uint16_t tx_max_credits;

    uint16_t rx_cid;
    uint16_t rx_length;
    uint16_t rx_total_count;    /*if rx_total_count > 0, receive data*/

    uint8_t  tx_next_byte;
    uint8_t  rx_next_byte;

} T_L2C_DATA_TEST_REQ;

typedef struct
{
    uint32_t leFixedDisplayValue;

} T_LE_CFG_PASSKEY_VALUE;



void hrp_system_send_cmd_ack(uint8_t cmd_group, uint16_t cmd_index, HRP_SYSTEM_CMD_STATUS status);

void hrp_btif_system_reset(uint16_t len, uint8_t *p_param_list);

void hrp_btif_system_init(uint16_t len, uint8_t *p_param_list);


void hrp_btif_system_le_cfg_passkey_value(uint16_t len, uint8_t *p_param_list);

void hrp_btif_system_event_ack(uint16_t len, uint8_t *p_param_list);

void hrp_btif_system_enable_dlps_req(uint16_t len, uint8_t *p_param_list);

void  hrp_btif_system_read_dlps_count_req(uint16_t len, uint8_t *p_param_list);

#endif
