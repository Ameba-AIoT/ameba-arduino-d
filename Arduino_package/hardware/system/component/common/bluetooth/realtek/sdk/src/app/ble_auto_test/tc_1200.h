
#ifndef _BLE_AUTO_TEST_TC_1200_H_
#define _BLE_AUTO_TEST_TC_1200_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
   dut is slave, dut init connection param update
*/

typedef struct _T_TC_1200_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_1200_IN_PARAM_DATA;


void tc_1200_start(uint32_t count);
void tc_1200_scan_state_change_to_idle(void);
void tc_1200_link_connected(uint8_t conn_id);
void tc_1200_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1200_conn_param_update_evt(uint8_t conn_id);
void tc_1200_add_case(uint32_t max_count);


void tc_1201_start(uint32_t count);
void tc_1201_scan_state_change_to_idle(void);
void tc_1201_link_connected(uint8_t conn_id);
void tc_1201_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1201_conn_param_update_evt(uint8_t conn_id);
void tc_1201_add_case(uint32_t max_count);


void tc_1202_start(uint32_t count);
void tc_1202_scan_state_change_to_idle(void);
void tc_1202_link_connected(uint8_t conn_id);
void tc_1202_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1202_conn_param_update_evt(uint8_t conn_id);
void tc_1202_add_case(uint32_t max_count);

void tc_1203_start(uint32_t count);
void tc_1203_scan_state_change_to_idle(void);
void tc_1203_link_connected(uint8_t conn_id);
void tc_1203_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1203_conn_param_update_evt(uint8_t conn_id);
void tc_1203_add_case(uint32_t max_count);


void tc_1204_start(uint32_t count);
void tc_1204_scan_state_change_to_idle(void);
void tc_1204_link_connected(uint8_t conn_id);
void tc_1204_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1204_conn_param_update_evt(uint8_t conn_id);
void tc_1204_add_case(uint32_t max_count);

void tc_1205_start(uint32_t count);
void tc_1205_scan_state_change_to_idle(void);
void tc_1205_link_connected(uint8_t conn_id);
void tc_1205_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1205_conn_param_update_evt(uint8_t conn_id);
void tc_1205_add_case(uint32_t max_count);

void tc_1206_start(uint32_t count);
void tc_1206_scan_state_change_to_idle(void);
void tc_1206_link_connected(uint8_t conn_id);
void tc_1206_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1206_conn_param_update_evt(uint8_t conn_id);
void tc_1206_add_case(uint32_t max_count);

void tc_1207_start(uint32_t count);
void tc_1207_scan_state_change_to_idle(void);
void tc_1207_link_connected(uint8_t conn_id);
void tc_1207_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_1207_conn_param_update_evt(uint8_t conn_id);
void tc_1207_add_case(uint32_t max_count);



#ifdef __cplusplus
}
#endif

#endif

