
#ifndef _BLE_AUTO_TEST_TC_600_H_
#define _BLE_AUTO_TEST_TC_600_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
    TC_0600_IOP_PAIR_LEGACL
*/
typedef struct _T_TC_600_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_600_IN_PARAM_DATA;
void tc_600_iop_android_legacl_pair_start(uint32_t count);
void tc_600_iop_android_legacl_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_600_iop_android_legacl_pair_link_connected(uint8_t conn_id);
void tc_600_iop_android_legacl_pair_state_to_start(uint8_t conn_id);
void tc_600_iop_android_legacl_pair_state_to_success(uint8_t conn_id);
void tc_600_iop_android_legacl_pair_state_to_fail(uint8_t conn_id, uint16_t reason);
void tc_600_dump_result(void);
void tc_600_add_case(uint32_t max_count);






/**
    TC_0601_IOP_PAIR_SC
*/
typedef struct _T_TC_601_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_601_IN_PARAM_DATA;
void tc_601_iop_android_sc_pair_start(uint32_t count);
void tc_601_iop_android_sc_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_601_iop_android_sc_pair_link_connected(uint8_t conn_id);
void tc_601_iop_android_sc_pair_state_to_start(uint8_t conn_id);
void tc_601_iop_android_sc_pair_state_to_success(uint8_t conn_id);
void tc_601_iop_android_sc_pair_state_to_fail(uint8_t conn_id);
void tc_601_dump_result(void);
void tc_601_add_case(uint32_t max_count);


#ifdef __cplusplus
}
#endif

#endif

