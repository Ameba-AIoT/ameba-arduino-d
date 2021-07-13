
#ifndef _BLE_AUTO_TEST_TC_500_SUT_H_
#define _BLE_AUTO_TEST_TC_500_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*P_FUN_PROFILE_CLIENT_RESULT_CB)(uint8_t conn_id, void *p_data);


void tc_50x_sut_client_result_callback(uint8_t conn_id, void *p_cb_data);


/**
    TC_0500_SLAVE_AUTO_ADV
*/
typedef struct _T_TC_500_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint8_t remote_bd[6];
} T_TC_500_SUT_IN_PARAM_DATA;
void tc_500_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_500_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_500_sut_link_connected(uint8_t conn_id);
void tc_500_sut_dump_result(void);
void tc_500_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);



/**
    TC_0501_SLAVE_AUTO_ADV_WITH_SEC_REQ
*/
typedef struct _T_TC_501_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t conn_supervision_timeout;
    uint8_t remote_bd[6];
} T_TC_501_SUT_IN_PARAM_DATA;
void tc_501_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_501_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_501_sut_link_connected(uint8_t conn_id);
void tc_501_sut_dump_result(void);
void tc_501_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);



#ifdef __cplusplus
}
#endif

#endif

