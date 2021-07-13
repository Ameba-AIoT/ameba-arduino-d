
#ifndef _BLE_AUTO_TEST_TC_COMMON_H_
#define _BLE_AUTO_TEST_TC_COMMON_H_
#include <app_msg.h>
#include <os_queue.h>
#ifdef __cplusplus
extern "C" {
#endif

//#include "board.h"

extern uint8_t gSimpleProfileServiceId;

extern void *g_test_timer_handle;
extern uint32_t  g_test_begin_time;
extern uint32_t  g_test_end_time;

extern uint16_t conn_interval;
extern uint16_t conn_latency;
extern uint16_t conn_supervision_timeout;

typedef void(*P_FUN_TC_RESULT_CB)(uint16_t case_id, uint16_t result, void *p_cb_data);


extern P_FUN_TC_RESULT_CB p_tc_result_cb;

typedef struct _BLE_DISCONN_REASON
{
    uint16_t reason_08;
    uint16_t reason_13;
    uint16_t reason_15;
    uint16_t reason_16;
    uint16_t reason_22;
    uint16_t reason_28;
    uint16_t reason_3d;
    uint16_t reason_3e;
    uint16_t reason_others;
} BLE_DISCONN_REASON;

extern BLE_DISCONN_REASON g_ble_disconn_reason;

#define VENDOR_BASE_ADDRESS 0x40058000
#define VENDOR_READ(Vendor_offset) \
    ((uint32_t)*((volatile uint32_t*)(VENDOR_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_WRITE(Vendor_offset, Value) \
    ((*((volatile uint32_t*)(VENDOR_BASE_ADDRESS + (Vendor_offset)))) = (Value))

#define TEST_CPU_CLOCK  20// MHz

static inline void reset_vendor_counter(void)
{
#if F_BT_UPPER_STACK_USE_VIRTUAL_HCI
    /* reser the counter */
    VENDOR_WRITE(0x17C, (1U << 31));

    /* set the counter */
    VENDOR_WRITE(0x17C, 0);
#else

#endif
}
static inline uint32_t read_vendor_counter_no_display(void)
{
#if F_BT_UPPER_STACK_USE_VIRTUAL_HCI
    return (VENDOR_READ(0x17C) & 0x3FFFFFF);
#else
    return 0;
#endif
}
void tc_check_remote_disc_reason(uint16_t case_id, uint16_t reason);
void tc_check_local_disc_reason(uint16_t case_id, uint16_t reason);


void tc_reg_result_callback(P_FUN_TC_RESULT_CB tc_result_cb);
void tc_update_disc_reason(uint16_t reason);
void tc_dump_disc_reason(void);



typedef struct _T_TC_PARAM_DATA
{
    uint16_t id;
} T_TC_PARAM_DATA;


typedef struct _T_TC_PARAM
{
    struct _T_TC_PARAM *p_next;
    T_TC_PARAM_DATA *p_data;
} T_TC_PARAM;

extern T_OS_QUEUE tc_q;


#ifdef __cplusplus
}
#endif

#endif

