
#ifndef _BLE_AUTO_TEST_TC_400_H_
#define _BLE_AUTO_TEST_TC_400_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
   dut is slave, dut init connection param update
*/

typedef struct _T_TC_400_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_400_IN_PARAM_DATA;


void tc_400_start(uint32_t count);
void tc_400_link_connected(uint8_t conn_id);
void tc_400_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_400_conn_param_update_evt(uint8_t conn_id);
void tc_400_dump_result(void);
void tc_400_add_case(uint32_t max_count);


/**
   dut is slave, sut init connection param update
*/

typedef struct _T_TC_401_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_401_IN_PARAM_DATA;


void tc_401_start(uint32_t count);
void tc_401_link_connected(uint8_t conn_id);
void tc_401_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_401_conn_param_update_evt(uint8_t conn_id);
void tc_401_dump_result(void);
void tc_401_add_case(uint32_t max_count);

#ifdef __cplusplus
}
#endif

#endif

