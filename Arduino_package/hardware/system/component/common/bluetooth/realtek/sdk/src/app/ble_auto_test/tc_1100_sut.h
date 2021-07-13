
#ifndef _BLE_AUTO_TEST_TC_1100_SUT_H_
#define _BLE_AUTO_TEST_TC_1100_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

void tc_1100_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_1100_sut_dump_result(void);
void tc_1100_sut_link_connected(uint8_t conn_id);
void tc_1100_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1100_sut_chann_connected(uint16_t cid);
void tc_1100_sut_chann_data_send_cmpl(uint16_t cause);
void tc_1100_sut_chann_disconnected(uint16_t cid, uint16_t cause);

void tc_1101_sut_start(uint32_t count, uint8_t remote_bd[6], uint16_t data_len);
void tc_1101_sut_dump_result(void);
void tc_1101_sut_link_connected(uint8_t conn_id);
void tc_1101_sut_receive_data(uint16_t cid, uint16_t length);
void tc_1101_sut_chann_data_send_cmpl(uint16_t cause, uint8_t credit);
void tc_1101_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1101_sut_chann_connected(uint16_t cid, uint16_t mtu, uint8_t credit);
void tc_1101_sut_chann_disconnected(uint16_t cid, uint16_t cause);

void tc_1102_sut_start(uint8_t remote_bd[6]);
void tc_1102_sut_conn_param_update_event(uint8_t conn_id);
void tc_1102_sut_dump_result(void);
void tc_1102_sut_link_connected(uint8_t conn_id);
void tc_1102_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1102_sut_chann_connected(uint16_t cid, uint16_t mtu, uint8_t credit);
void tc_1102_sut_receive_data(uint16_t cid, uint16_t length);
void tc_1102_sut_chann_disconnected(uint16_t cid, uint16_t cause);
#if 0
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

void tc_200_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_200_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_200_sut_link_connected(uint8_t conn_id);
void tc_200_sut_conn_param_update_event(uint8_t conn_id);
void tc_200_sut_dump_result(void);
void tc_200_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);
#endif


#ifdef __cplusplus
}
#endif

#endif

