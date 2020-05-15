/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      gap_test_app.h
   * @brief     GAP roles test application implementation
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _GAP_TEST_APP_H_
#define _GAP_TEST_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#if F_BT_LE_GATT_CLIENT_SUPPORT
#include "profile_client.h"
#endif
#include <profile_server.h>
#include <app_msg.h>
#if F_BT_LE_PRIVACY_SUPPORT
#include <privacy_mgnt.h>
#endif
#include <gap_msg.h>

typedef enum
{
    GAP_TC_00_NORMAL            = 0,
    GAP_TC_01_ANCS              = 1,
    GAP_TC_02_GAPS_WRITEABLE    = 2,
    GAP_TC_03_PRIVACY           = 3,
    GAP_TC_04_PIN_CODE          = 4,
    GAP_TC_05_BT5               = 5,
    GAP_TC_07_CCCD              = 7,
    GAP_TC_08_LOCAL_IRK         = 8,
    GAP_TC_09_SLAVE_LATENCY     = 9,
    GAP_TC_10_BT41              = 10,
    GAP_TC_11_MIN_REM_SCA       = 11,
    GAP_TC_12_GAP_MSG_CALLBACK  = 12,
    GAP_TC_13_V3_V8_TX          = 13,
    GAP_TC_14_SRV_CHANGE        = 14,
    GAP_TC_15_HID_PROFILE       = 15,
} T_GAP_TEST_CASE;

typedef struct
{
    uint8_t       v3_tx_idx;
    uint8_t       v3_tx_conn_id;
    uint8_t       v3_tx_len;
    uint16_t      v3_tx_num;
    uint16_t      v3_tx_cmp_num;
    uint16_t      v3_rx_num;
    uint16_t      v3_tx_cnt;
    uint16_t      v8_tx_cnt;
    uint16_t      v8_rx_num;
} T_GAP_V3_NOTIF_TEST;

/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_GAP_DEV_STATE gap_dev_state;
extern T_GAP_TEST_CASE       gap_test_case;
extern T_GAP_V3_NOTIF_TEST gap_v3_notif_test;
#if F_BT_LE_GATT_CLIENT_SUPPORT
extern T_CLIENT_ID   gaps_client_id;        /**< Simple ble service client id*/
extern T_CLIENT_ID   simple_ble_client_id;  /**< gap service client id*/
#endif
extern T_SERVER_ID   simp_srv_id;           /**< Simple ble service id*/
extern T_SERVER_ID   hid_srv_id;

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void app_handle_io_msg(T_IO_MSG io_msg);
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
#if F_BT_LE_GATT_CLIENT_SUPPORT
T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
#endif
T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data);

void app_gap_common_callback(uint8_t cb_type, void *p_cb_data);
void app_gap_vendor_callback(uint8_t cb_type, void *p_cb_data);

#if F_BT_GAPS_CHAR_WRITEABLE
T_APP_RESULT gap_service_callback(T_SERVER_ID service_id, void *p_para);
#endif

#if F_BT_LE_PRIVACY_SUPPORT
void app_privacy_callback(T_PRI_CB_TYPE type, uint8_t status);
#endif

#if F_BT_LE_4_1_CBC_SUPPORT
T_APP_RESULT app_credit_based_conn_callback(uint8_t cbc_type, void *p_cbc_data);
#endif

#ifdef __cplusplus
}
#endif

#endif

