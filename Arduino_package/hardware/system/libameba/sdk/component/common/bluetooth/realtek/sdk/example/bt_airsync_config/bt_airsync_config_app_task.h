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
#ifndef _BT_AIRSYNC_CONFIG_APP_TASK_H_
#define _BT_AIRSYNC_CONFIG_APP_TASK_H_

typedef struct
{
    uint8_t *buf;
    uint16_t length;
} T_BT_AIRSYNC_CONFIG_TX_DATA;

void bt_airsync_config_send_msg(uint16_t sub_type);

/**
 * @brief  Initialize App task
 * @return void
 */
void bt_airsync_config_app_task_init(void);

/**
 * @brief  Deinitialize App task
 * @return void
 */
void bt_airsync_config_app_task_deinit(void);

void bt_airsync_config_send_data_to_apptask(uint8_t *buf, uint16_t length);

#endif

