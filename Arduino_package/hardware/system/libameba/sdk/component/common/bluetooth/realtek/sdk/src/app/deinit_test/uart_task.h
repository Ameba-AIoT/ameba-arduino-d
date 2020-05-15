/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      app_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author    jane
   * @date      2017-06-02
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _UART_TASK_H_
#define _UART_TASK_H_


typedef enum
{
    TC_IDLE = 0x0000,
    TC_0001_IDLE = 1,
    TC_0002_ADV,
    TC_0003_SCAN,
    TC_0004_CON_TX,
    TC_0005_CON_RX,

    TC_MAX = 65535
} T_CUR_TEST_CASE;

typedef enum
{
    TC_START = 0x0000,
    TC_DEL_STACK,
    TC_ADD_STACK,
    TC_STACK_STARTED,
    TC_START_ADV,
    TC_START_SCAN,

    TC_END
} T_TEST_PROC_SUBTYPE;

extern T_CUR_TEST_CASE test_case_id;
extern uint32_t test_cur_count;
extern uint32_t test_max_count;
extern void *auto_test_timer;
void app_send_msg_to_uart_app(T_TEST_PROC_SUBTYPE sub_type, uint16_t cause);
/**
 * @brief  Initialize App task
 * @return void
 */
void uart_task_init(void);

#endif

