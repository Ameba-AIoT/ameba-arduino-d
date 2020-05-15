
#ifndef _BLE_AUTO_TEST_TC_700_H_
#define _BLE_AUTO_TEST_TC_700_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
    TC_0700_STABLE_NOTIFICATION_TX_01
*/
void tc_700_stable_notification_tx_01_start(void);
void tc_700_stable_notification_tx_01_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_700_stable_notification_tx_01_link_connected(uint8_t conn_id);
void tc_700_stable_notification_tx_01_tx_data_complete(uint8_t credits);
void tc_700_stable_notification_tx_01_cccd_enable(uint8_t conn_id, bool bEnable);
void tc_700_stable_notification_tx_conn_param_update_event(uint8_t conn_id);


#ifdef __cplusplus
}
#endif

#endif

