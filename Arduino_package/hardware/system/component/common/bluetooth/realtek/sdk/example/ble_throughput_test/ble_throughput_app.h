/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      ble_auto_test_application.h
* @brief     gap roles test app implementation
* @details   gap roles test app implementation
* @author    jane
* @date      2016-02-18
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _GAP_ROLES_TEST_APP_H_
#define _GAP_ROLES_TEST_APP_H_
#include "app_msg.h"

#ifdef __cplusplus
extern "C" {
#endif


#if F_BT_LE_GATT_CLIENT_SUPPORT
#include "profile_client.h"
#endif
#if F_BT_LE_GATT_SERVER_SUPPORT
#include "profile_server.h"
#endif


extern void ble_throughput_app_handle_io_msg(T_IO_MSG io_msg);
extern T_APP_RESULT ble_throughput_app_gap_callback(uint8_t cb_type,
                                     void *p_cb_data);

#if F_BT_LE_GATT_SERVER_SUPPORT
extern T_APP_RESULT ble_throughput_app_profile_callback(T_SERVER_ID serviceID, void *p_data);
#endif
#if F_BT_LE_4_1_CBC_SUPPORT
T_APP_RESULT ble_throughput_app_credit_based_conn_callback(uint8_t cbc_type, void *p_cbc_data);
#endif
#if F_BT_LE_GATT_CLIENT_SUPPORT
T_APP_RESULT ble_throughput_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
#endif



#ifdef __cplusplus
}
#endif

#endif

