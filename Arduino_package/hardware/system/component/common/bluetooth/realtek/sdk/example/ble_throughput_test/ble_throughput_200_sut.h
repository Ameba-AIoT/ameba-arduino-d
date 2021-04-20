
#ifndef _BLE_AUTO_TEST_TC_200_SUT_H_
#define _BLE_AUTO_TEST_TC_200_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t initial_value;
    uint32_t total_test_count;
    uint8_t remote_bd[6];
    uint32_t total_notify_rx_count;
    uint32_t begin_time;
    uint32_t end_time;
    uint32_t elapsed_time;
    uint32_t data_rate;
} TC_206_SUT_MGR;

typedef struct
{
    uint8_t initial_value;
    uint32_t total_test_count;
    uint8_t remote_bd[6];
    uint32_t count_remain;
    uint32_t begin_time;
    uint32_t end_time;
    uint32_t elapsed_time;
    uint32_t data_rate;
} TC_207_SUT_MGR;

typedef void (*P_FUN_PROFILE_CLIENT_RESULT_CB)(uint8_t conn_id, void *p_data);

typedef enum
{
    TP_CONFIG_OP_SET_LL_DATA_LEN_27    = 1,
    TP_CONFIG_OP_SET_LL_DATA_LEN_251   = 2,
    TP_CONFIG_OP_START                 = 3,

    TP_CONFIG_OP_RFU = 0xFF
} T_TP_CONFIG_OP;


typedef enum
{
    TP_CONFIG_STATE_CONFIGING = 1,
    TP_CONFIG_STATE_STARTED = 2,

    TP_CONFIG_STATE_RFU = 0xFF
} T_TP_CONFIG_STATE;

void ble_throughput_20x_sut_client_result_callback(uint8_t conn_id, void *p_cb_data);


/**

*/
void ble_throughput_206_sut_start(uint8_t remote_bd[6]);
void ble_throughput_206_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void ble_throughput_206_sut_link_connected(uint8_t conn_id);
void ble_throughput_206_sut_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void ble_throughput_206_sut_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                              T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
void ble_throughput_206_sut_phy_update(uint8_t conn_id);
#endif
void ble_throughput_206_sut_update_conn_param(uint8_t conn_id);
void ble_throughput_206_sut_dump_result(void);

void ble_throughput_207_sut_start(uint8_t remote_bd[6]);
void ble_throughput_207_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void ble_throughput_207_sut_link_connected(uint8_t conn_id);
void ble_throughput_207_sut_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void ble_throughput_207_sut_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                              T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
void ble_throughput_207_sut_phy_update(uint8_t conn_id);
#endif
void ble_throughput_207_sut_update_conn_param(uint8_t conn_id);
void ble_throughput_207_sut_dump_result(void);
void ble_throughput_207_sut_tx_data_complete(uint8_t credits);

#ifdef __cplusplus
}
#endif

#endif

