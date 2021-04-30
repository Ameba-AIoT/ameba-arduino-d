
#ifndef _BLE_AUTO_TEST_TC_600_SUT_H_
#define _BLE_AUTO_TEST_TC_600_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
    TC_0600_IOP_PAIR_LEGACL
*/
typedef struct _T_TC_600_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint32_t total_pair_start_count;
    uint32_t total_pair_success_count;
    uint32_t total_pair_fail_count;
    uint8_t remote_bd[6];
} T_TC_600_SUT_IN_PARAM_DATA;

void tc_600_sut_iop_android_legacl_pair_start(uint32_t count, uint8_t remote_bd[6]);
void tc_600_sut_iop_android_legacl_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_600_sut_iop_android_legacl_pair_link_connected(uint8_t conn_id);
void tc_600_sut_iop_android_legacl_pair_state_to_start(uint8_t conn_id);
void tc_600_sut_iop_android_legacl_pair_state_to_success(uint8_t conn_id);
void tc_600_sut_iop_android_legacl_pair_state_to_fail(uint8_t conn_id, uint16_t reason);
void tc_600_sut_dump_result(void);
void tc_600_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);





/**
    TC_0601_IOP_PAIR_SC
*/
typedef struct _T_TC_601_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint32_t total_pair_start_count;
    uint32_t total_pair_success_count;
    uint32_t total_pair_fail_count;
    uint8_t remote_bd[6];
} T_TC_601_SUT_IN_PARAM_DATA;
void tc_601_sut_iop_android_sc_pair_start(uint32_t count, uint8_t remote_bd[6]);
void tc_601_sut_iop_android_sc_pair_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_601_sut_iop_android_sc_pair_link_connected(uint8_t conn_id);
void tc_601_sut_iop_android_sc_pair_state_to_start(uint8_t conn_id);
void tc_601_sut_iop_android_sc_pair_state_to_success(uint8_t conn_id);
void tc_601_sut_iop_android_sc_pair_state_to_fail(uint8_t conn_id);
void tc_601_sut_dump_result(void);
void tc_601_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);


#ifdef __cplusplus
}
#endif

#endif

