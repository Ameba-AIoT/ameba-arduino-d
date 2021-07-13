
#ifndef _BLE_AUTO_TEST_TC_300_H_
#define _BLE_AUTO_TEST_TC_300_H_


#ifdef __cplusplus
extern "C" {
#endif




typedef struct _T_TC_300_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_300_IN_PARAM_DATA;


void tc_300_adv_only_start(uint32_t count);
void tc_300_adv_only_link_connected(uint8_t conn_id);
void tc_300_adv_only_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_300_dump_result(void);
void tc_300_add_case(uint32_t max_count);


/**
    TC_0301_ADV_DISC
*/

typedef struct _T_TC_301_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_301_IN_PARAM_DATA;

void tc_301_adv_disc_start(uint32_t count);
void tc_301_adv_disc_link_connected(uint8_t conn_id);
void tc_301_adv_disc_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_301_dump_result(void);
void tc_301_add_case(uint32_t max_count);


#ifdef __cplusplus
}
#endif

#endif

