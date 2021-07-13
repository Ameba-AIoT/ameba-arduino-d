

#ifndef _BLE_AUTO_TEST_TC_0001_H_
#define _BLE_AUTO_TEST_TC_0001_H_


#if F_BT_LE_GAP_PERIPHERAL_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif




typedef struct t_adv_test_param
{
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;

    uint32_t start_adv_begin_time;
    uint32_t start_adv_end_time;
    uint32_t stop_adv_begin_time;
    uint32_t stop_adv_end_time;

} T_ADV_TEST_PARAM;




typedef struct _T_TC_0001_PARAM_DATA
{
    uint16_t id;
    uint16_t result;
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;
} T_TC_0001_PARAM_DATA;

typedef struct _T_TC_0002_PARAM_DATA
{
    uint16_t id;
    uint16_t result;
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;
    uint32_t max_count;
} T_TC_0002_PARAM_DATA;


/**
    for case   TC_0001_ADV_PERFORMANCE
*/
void tc_0001_adv_start(uint16_t advIntMin, uint16_t advIntMax);
void tc_0001_adv_timeout_handler(void *pxTimer);
void tc_0001_adv_adv_state_change_to_idle(void);
void tc_0001_adv_adv_state_change_to_advertising(void);
void tc_0001_add_case(uint16_t adv_interval_min, uint16_t adv_interval_max);


/**
    for case   TC_0002_ADV_PERFORMANCE
*/
void tc_0002_adv_start_stop_start(uint16_t advIntMin, uint16_t advIntMax, uint32_t max_count);
void tc_0002_adv_start_stop_adv_state_change_to_advertising(void);
void tc_0002_adv_start_stop_adv_state_change_to_idle(void);
void tc_0002_add_case(uint16_t adv_interval_min, uint16_t adv_interval_max, uint32_t max_count);




#ifdef __cplusplus
}
#endif

#endif

#endif

