
#ifndef _BLE_AUTO_TEST_TC_800_H_
#define _BLE_AUTO_TEST_TC_800_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
    TC_0600_IOP_PAIR_LEGACL
*/
typedef struct _T_TC_800_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_800_IN_PARAM_DATA;
void tc_800_iop_android_legacl_pair_start(uint32_t count);
void tc_800_iop_android_legacl_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_800_iop_android_legacl_pair_link_connected(uint8_t conn_id);
void tc_800_iop_android_legacl_pair_state_to_start(uint8_t conn_id);
void tc_800_iop_android_legacl_pair_state_to_success(uint8_t conn_id);
void tc_800_iop_android_legacl_pair_state_to_fail(uint8_t conn_id, uint16_t reason);
void tc_800_dump_result(void);
void tc_800_add_case(uint32_t max_count);






/**
    TC_0601_IOP_PAIR_SC
*/
typedef struct _T_TC_801_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_801_IN_PARAM_DATA;
void tc_801_iop_android_sc_pair_start(uint32_t count);
void tc_801_iop_android_sc_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_801_iop_android_sc_pair_link_connected(uint8_t conn_id);
void tc_801_iop_android_sc_pair_state_to_start(uint8_t conn_id);
void tc_801_iop_android_sc_pair_state_to_success(uint8_t conn_id);
void tc_801_iop_android_sc_pair_state_to_fail(uint8_t conn_id);
void tc_801_dump_result(void);
void tc_801_add_case(uint32_t max_count);


#ifdef __cplusplus
}
#endif

#endif

