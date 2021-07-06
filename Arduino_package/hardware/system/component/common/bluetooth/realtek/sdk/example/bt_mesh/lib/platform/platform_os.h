/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     platform_os.h
  * @brief    Head file for platform os related.
  * @details  OS related
  * @author   bill
  * @date     2017-9-30
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PLATFORM_OS_H
#define _PLATFORM_OS_H

/* Add Includes here */
#include "platform_macros.h"
#include "os_timer.h"
#include "os_msg.h"
#include "os_sync.h"
#include "os_mem.h"
#include "os_sched.h"

BEGIN_DECLS

/** @addtogroup Platform_OS
  * @{
  */

/** @defgroup Critical_Section Critical Section
  * @{
  */
#define plt_critical_enter()                                        os_lock()
#define plt_critical_exit(flags)                                    os_unlock(flags)
/** @} */

/** @defgroup Mutex Mutex
  * @{
  */
typedef void *plt_mutex_t;
plt_mutex_t plt_mutex_create(void);
#define plt_mutex_delete(mutex)                                     os_mutex_delete(mutex)
#define plt_mutex_take(mutex, wait_ms)                              os_mutex_take(mutex, wait_ms)
#define plt_mutex_give(mutex)                                       os_mutex_give(mutex)
/** @} */

/** @defgroup Dynamic_Memory Dynamic Memory
  * @brief heap memory management
  * @{
  */
#define RAM_TYPE_DATA_OFF                                           RAM_TYPE_DATA_ON
#define plt_malloc(size, ram_type)                                  os_mem_alloc(ram_type, size)
#define plt_zalloc(size, ram_type)                                  os_mem_zalloc(ram_type, size)
#define plt_free(p, ram_type)                                       os_mem_free(p)
/** @} */

/** @defgroup Sync_Queue Sync Queue
  * @brief sync os queue, the task maybe blocked when invoked
  * @{
  */
typedef void *plt_os_queue_handle_t;
plt_os_queue_handle_t plt_os_queue_create(uint32_t item_num, uint32_t item_size);
#define plt_os_queue_delete(queue_handle)                           os_msg_queue_delete(queue_handle)
#define plt_os_queue_send(queue_handle, pmsg, wait_ms)              os_msg_send(queue_handle, pmsg, wait_ms)
#define plt_os_queue_receive(queue_handle, pmsg, wait_ms)           os_msg_recv(queue_handle, pmsg, wait_ms)
/** @} */

/** @defgroup SW Timer
  * @{
  */
typedef void *plt_timer_t;
plt_timer_t plt_timer_create(const char *name, uint32_t period_ms, bool reload, uint32_t timer_id,
                             void (*pf_cb)(void *));
#define plt_timer_delete(timer, wait_ms)                            os_timer_delete(&timer)
#define plt_timer_start(timer, wait_ms)                             os_timer_start(&timer)
#define plt_timer_stop(timer, wait_ms)                              os_timer_stop(&timer)
#define plt_timer_reset(timer, wait_ms)                             os_timer_start(&timer)
#define plt_timer_change_period(timer, period_ms, wait_ms)          os_timer_restart(&timer, period_ms)
bool plt_timer_is_active(plt_timer_t timer);
uint32_t plt_timer_get_id(plt_timer_t timer);
/** @} */

/** @defgroup Time
  * @{
  */
/** cpu occupied delay */
void plt_delay_us(uint32_t t);

/** precision: 10ms, cpu unoccupied delay since task is switched out */
#define plt_delay_ms(t)                                             os_delay(t)

/** @brief system time value increase progressively */
#define VENDOR_BASE_ADDRESS                                         0x40058000
#define VENDOR_READ(Vendor_offset)                                  ((uint32_t)*((volatile uint32_t*)(VENDOR_BASE_ADDRESS+(Vendor_offset))))
#define PLT_TIME_READ_US_MAX_VALUE                                  (0x3FFFFFF/40)
/* precision: 1us, range: 0 ~ 1.677 second */
#define plt_time_read_us()                                          0//((VENDOR_READ(0x17C) & 0x3FFFFFF)/40)

/* precision: 10ms, range: 0 ~ 49 day*/
#define plt_time_read_ms()                                          os_sys_time_get()

/**
  * @brief diff the time with high precision as much as possible
  *
  * The precision is 1us in the range of 0 ~ 1.6 second, otherwise 10ms with the range of 0 ~ 24 day.
  * @param[in] begin_ms: the ms-level begin time
  * @param[in] begin_us: the us-level begin time
  * @param[in] end_ms: the ms-level end time
  * @param[in] end_us: the us-level end time
  * @return time elapsed (bit31 indicates the unit of return value: 1 - us, 0 - ms.)
  */
uint32_t plt_time_diff(uint32_t begin_ms, uint32_t begin_us, uint32_t end_ms, uint32_t end_us);
/** @} */
/** @} */

END_DECLS

#endif /* _PLATFORM_OS_H */
