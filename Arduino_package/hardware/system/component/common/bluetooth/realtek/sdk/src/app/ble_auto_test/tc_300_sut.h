
#ifndef _BLE_AUTO_TEST_TC_300_SUT_H_
#define _BLE_AUTO_TEST_TC_300_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

#if TC_300_SUT_SUPPORT
/**
    TC_0300_ADV_ONLY
*/
typedef struct _T_TC_300_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint8_t remote_bd[6];
} T_TC_300_SUT_IN_PARAM_DATA;
void tc_300_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_300_sut_link_connected(uint8_t conn_id);
void tc_300_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_300_sut_dump_result(void);
void tc_300_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);



/**
    TC_0301_ADV_DISC
*/
typedef struct _T_TC_301_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint8_t remote_bd[6];
} T_TC_301_SUT_IN_PARAM_DATA;
void tc_301_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_301_sut_link_connected(uint8_t conn_id);
void tc_301_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_301_sut_dump_result(void);
void tc_301_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);
#endif

#ifdef __cplusplus
}
#endif

#endif

