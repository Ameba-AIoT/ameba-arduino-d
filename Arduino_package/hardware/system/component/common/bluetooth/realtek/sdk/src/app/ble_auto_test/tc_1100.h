
#ifndef _BLE_AUTO_TEST_TC_1100_H_
#define _BLE_AUTO_TEST_TC_1100_H_


#ifdef __cplusplus
extern "C" {
#endif

#if F_BT_LE_4_1_CBC_SUPPORT
void tc_1100_start(uint32_t count);
void tc_1100_link_connected(uint8_t conn_id);
void tc_1100_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1100_chann_connected(uint16_t cid);
void tc_1100_chann_disconnected(uint16_t cid, uint16_t cause);
void tc_1100_dump_result(void);
void tc_1100_receive_data(uint16_t cid);

void tc_1101_start(uint32_t count, uint16_t data_len);
void tc_1101_dump_result(void);
void tc_1101_link_connected(uint8_t conn_id);
void tc_1101_receive_data(uint16_t cid, uint16_t length);
void tx_1101_send_data_cmpl(uint16_t cause, uint16_t credit);
void tc_1101_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1101_chann_connected(uint16_t cid, uint16_t mtu, uint16_t credit);
void tc_1101_chann_disconnected(uint16_t cid, uint16_t cause);

void tc_1102_start(uint16_t data_len);
void tc_1102_conn_param_update_event(uint8_t conn_id);
void tc_1102_mtu_size_info(uint8_t conn_id);
void tc_1102_send_data_cmpl(uint16_t cause, uint16_t credit);
void tc_1102_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1102_chann_connected(uint16_t cid, uint16_t mtu, uint16_t credit);
void tc_1102_chann_disconnected(uint16_t cid, uint16_t cause);
#endif
#if 0
typedef struct t_tp_test_config
{
    uint16_t con_interval;
    uint16_t length;
    uint8_t  tx_octets;

} T_TP_TEST_CONFIG;


typedef struct t_tp_test_param
{
    uint16_t con_interval;
    uint16_t conn_slave_latency;
    uint16_t conn_supervision_timeout;
    uint16_t length;
    uint32_t count;
    uint32_t count_remain;
    uint8_t initial_value;

    uint8_t conn_id;
    uint32_t begin_time;
    uint32_t end_time;
    uint32_t data_rate;
} T_TP_TEST_PARAM;


/**
    for case   TC_0200_TP_NOTIFICATION_TX_01
*/
void tc_200_tp_notification_tx_start(void);
void tc_200_tp_notification_tx_init_config(uint16_t con_interval, uint16_t length,
                                           uint8_t tx_octets);
void tc_200_tp_notification_tx_init_default_param(uint8_t conn_id);
void tc_200_tp_notification_tx_change_tx_octets(uint8_t conn_id, uint16_t tx_octets);
void tc_200_tp_notification_tx_config_with_fixed_interval(uint16_t con_interval);
void tc_200_tp_notification_tx_config_with_fixed_length(uint16_t length);
void tc_200_tp_notification_tx_conn_param_update_event(uint8_t conn_id);
void tc_200_tp_notification_tx_tx_data_complete(uint8_t credits);
void tc_200_tp_notification_tx_tx_config(void *pdata);
#endif
#ifdef __cplusplus
}
#endif

#endif

