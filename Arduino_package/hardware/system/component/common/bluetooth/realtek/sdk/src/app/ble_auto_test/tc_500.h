
#ifndef _BLE_AUTO_TEST_TC_500_H_
#define _BLE_AUTO_TEST_TC_500_H_


#ifdef __cplusplus
extern "C" {
#endif



/**
    TC_0500_SLAVE_AUTO_ADV
*/

typedef struct _T_TC_500_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_500_IN_PARAM_DATA;
void tc_500_salve_auto_adv_start(uint32_t count);
void tc_500_salve_auto_adv_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_500_salve_auto_adv_link_connected(uint8_t conn_id);
void tc_500_dump_result(void);
void tc_500_add_case(uint32_t max_count);


/**
    TC_0501_SLAVE_AUTO_ADV_WITH_SEC_REQ
*/
typedef struct _T_TC_501_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_501_IN_PARAM_DATA;
void tc_501_salve_auto_adv_with_sec_req_start(uint32_t count);
void tc_501_salve_auto_adv_with_sec_req_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_501_salve_auto_adv_with_sec_req_link_connected(uint8_t conn_id);
void tc_501_dump_result(void);
void tc_501_add_case(uint32_t max_count);


typedef struct _T_TC_502_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_502_IN_PARAM_DATA;
void tc_502_salve_auto_adv_start(uint32_t count);
void tc_502_salve_auto_adv_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_502_salve_auto_adv_link_connected(uint8_t conn_id);
void tc_502_dump_result(void);
void tc_502_add_case(uint32_t max_count);



#ifdef __cplusplus
}
#endif

#endif

