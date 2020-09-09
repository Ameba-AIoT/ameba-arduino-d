
#ifndef _BLE_AUTO_TEST_TC_200_H_
#define _BLE_AUTO_TEST_TC_200_H_


#ifdef __cplusplus
extern "C" {
#endif



typedef struct t_tp_test_config
{
    uint16_t con_interval;
    uint16_t length;
    uint8_t  tx_octets;
    bool test_drop_acl_data;

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
    uint32_t elapsed_time;
    uint32_t data_rate;
} T_TP_TEST_PARAM;

void ble_throughput_206_tp_notification_tx_start(void);
void ble_throughput_206_tp_notification_tx_init_config(uint16_t con_interval, uint16_t slave_latency,
                                           uint16_t length, uint8_t mode, uint32_t max_count, uint8_t data_check);
void ble_throughput_206_tp_notification_tx_tx_data_complete(uint8_t credits);
void ble_throughput_206_tp_notification_tx_tx_config(void *pdata);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void ble_throughput_206_tp_notification_phy_update_event(uint8_t conn_id, uint16_t cause,
                                             T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
#endif
void ble_throughput_206_tp_notification_tx_conn_param_update_event(uint8_t conn_id);
void ble_throughput_206_link_disconnected(uint8_t conn_id, uint16_t reason);
void ble_throughput_206_dump_result(void);

void ble_throughput_207_tp_rx_start(void);
void ble_throughput_207_tp_rx_init_config(uint16_t con_interval, uint16_t slave_latency,
                              uint16_t length, uint8_t mode, uint32_t max_count, uint8_t data_check);
void ble_throughput_207_tp_rx_conn_param_update_event(uint8_t conn_id);
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
void ble_throughput_207_tp_phy_update_event(uint8_t conn_id, uint16_t cause,
                                T_GAP_PHYS_TYPE tx_phy, T_GAP_PHYS_TYPE rx_phy);
#endif
void ble_throughput_207_tp_handle_write_data(void *pdata);
void ble_throughput_207_link_disconnected(uint8_t conn_id, uint16_t reason);
void ble_throughput_207_dump_result(void);

#ifdef __cplusplus
}
#endif

#endif

