
#ifndef _BLE_AUTO_TEST_TC_100_H_
#define _BLE_AUTO_TEST_TC_100_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct t_scan_test_param
{
    uint16_t scan_interval;
    uint16_t scan_window;

    uint32_t start_scan_begin_time;
    uint32_t start_scan_end_time;
    uint32_t stop_scan_begin_time;
    uint32_t stop_scan_end_time;
} T_SCAN_TEST_PARAM;



/**
    for case   TC_0100_SCAN_PERFORMANCE
*/
void tc_0100_scan_perf_start(uint16_t scan_interval, uint16_t scan_window);
void tc_0100_scan_state_change_to_scaning(void);
void tc_0100_scan_state_change_to_idle(void);


/**
    for case   TC_0101_SCAN_STRESS_ENABLE_DISABLE
*/
typedef struct
{
    T_GAP_SCAN_MODE     mode;
    uint16_t                interval;
    uint16_t                window;
    T_GAP_SCAN_FILTER_POLICY filter_policy;
    uint8_t                 filter_duplicates;

    void *scan_enable_disable_timer_handle;

    uint32_t scan_enable_disable_begin_time;
    uint32_t scan_enable_disable_max_enable_scan_ellapsed_time;
    uint32_t scan_enable_disable_max_disable_scan_ellapsed_time;
    uint32_t scan_enable_disable_max_count;
    uint32_t scan_enable_disable_cur_count;
} T_TC_0101_PARAM;
typedef T_TC_0101_PARAM *P_TC_0101_PARAM;


typedef struct _T_TC_0101_IN_PARAM_DATA
{
    uint16_t                id;
    T_GAP_SCAN_MODE     mode;
    uint16_t                interval;
    uint16_t                window;
    T_GAP_SCAN_FILTER_POLICY filter_policy;
    uint8_t                 filter_duplicates;
    uint32_t                max_count;
} T_TC_0101_IN_PARAM_DATA;

void tc_0101_scan_stress_enable_disable_start(T_GAP_SCAN_MODE mode,
                                              uint16_t                interval,
                                              uint16_t                window,
                                              T_GAP_SCAN_FILTER_POLICY filter_policy,
                                              uint8_t                 filter_duplicates,
                                              uint32_t max_count);
void tc_0101_scan_stress_enable_disable_state_change_to_scaning(void);
void tc_0101_scan_stress_enable_disable_state_change_to_idle(void);


void tc_0101_add_case(T_GAP_SCAN_MODE mode,
                      uint16_t                interval,
                      uint16_t                window,
                      T_GAP_SCAN_FILTER_POLICY filter_policy,
                      uint8_t                 filter_duplicates,
                      uint32_t max_count
                     );

#ifdef __cplusplus
}
#endif

#endif

