
#ifndef _BLE_AUTO_TEST_TC_200_SUT_H_
#define _BLE_AUTO_TEST_TC_200_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

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

void tc_20x_sut_client_result_callback(uint8_t conn_id, void *p_cb_data);


/**

*/
typedef struct _T_TC_200_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint8_t remote_bd[6];
    T_TP_CONFIG_STATE cur_config_state;
    T_TP_CONFIG_OP cur_config_op;
    uint32_t total_notify_rx_count;
} T_TC_200_SUT_IN_PARAM_DATA;

void tc_200_sut_start(uint32_t count, uint8_t mode, uint8_t mode_end, uint8_t remote_bd[6]);
void tc_200_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_200_sut_link_connected(uint8_t conn_id);
void tc_200_sut_conn_param_update_event(uint8_t conn_id);
void tc_200_sut_dump_result(void);
void tc_200_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);

void tc_206_sut_start(uint8_t remote_bd[6]);
void tc_206_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_206_sut_link_connected(uint8_t conn_id);
void tc_206_sut_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void tc_206_sut_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                              T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
void tc_206_sut_phy_update(uint8_t conn_id);
#endif
void tc_206_sut_update_conn_param(uint8_t conn_id);
void tc_206_sut_dump_result(void);

void tc_207_sut_start(uint8_t remote_bd[6]);
void tc_207_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_207_sut_link_connected(uint8_t conn_id);
void tc_207_sut_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void tc_207_sut_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                              T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
void tc_207_sut_phy_update(uint8_t conn_id);
#endif
void tc_207_sut_update_conn_param(uint8_t conn_id);
void tc_207_sut_dump_result(void);
void tc_207_sut_tx_data_complete(uint8_t credits);

void tc_208_sut_start(uint8_t remote_bd[6]);
void tc_208_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_208_sut_link_connected(uint8_t conn_id);
void tc_208_sut_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void tc_208_sut_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                              T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
void tc_208_sut_phy_update(uint8_t conn_id);
#endif
void tc_208_sut_update_conn_param(uint8_t conn_id);
void tc_208_sut_dump_rx_result(void);
void tc_208_sut_dump_tx_result(void);
void tc_208_sut_tx_data_complete(uint8_t credits);

#ifdef __cplusplus
}
#endif

#endif

