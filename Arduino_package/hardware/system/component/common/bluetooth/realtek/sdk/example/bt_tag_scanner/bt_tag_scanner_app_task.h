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
#ifndef _BT_TAG_SCANNER_APP_TASK_H_
#define _BT_TAG_SCANNER_APP_TASK_H_

#include <app_msg.h>
#include <bt_tag_scanner_keep_message.h>

bool bt_tag_scanner_app_send_msg_to_apptask(T_IO_MSG *p_msg);
bool bt_tag_scanner_send_msg_to_transfertask(tag_info_t *p_msg);


/**
 * @brief  Initialize App task
 * @return void
 */
void bt_tag_scanner_app_task_init(void);
void bt_tag_scanner_app_task_deinit(void);

void bt_tag_scanner_transfer_task_init(void);
void bt_tag_scanner_transfer_task_deinit(void);

#endif

