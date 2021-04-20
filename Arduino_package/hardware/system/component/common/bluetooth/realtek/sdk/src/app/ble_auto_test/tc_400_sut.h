
#ifndef _BLE_AUTO_TEST_TC_400_SUT_H_
#define _BLE_AUTO_TEST_TC_400_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif



typedef struct _T_TC_400_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint8_t remote_bd[6];
} T_TC_400_SUT_IN_PARAM_DATA;


void tc_400_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_400_sut_link_connected(uint8_t conn_id);
void tc_400_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_400_sut_conn_param_update_evt(uint8_t conn_id);
void tc_400_sut_dump_result(void);
void tc_400_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);

typedef struct _T_TC_401_SUT_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
    uint8_t remote_bd[6];
} T_TC_401_SUT_IN_PARAM_DATA;

void tc_401_sut_start(uint32_t count, uint8_t remote_bd[6]);
void tc_401_sut_link_connected(uint8_t conn_id);
void tc_401_sut_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_401_sut_conn_param_update_evt(uint8_t conn_id);
void tc_401_sut_dump_result(void);
void tc_401_sut_add_case(uint32_t max_count, uint8_t remote_bd[6]);



#ifdef __cplusplus
}
#endif

#endif

