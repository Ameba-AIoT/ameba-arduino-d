/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    simple_ble_config.h
  * @brief   This file includes common constants or types for Simple BLE service/client.
  *          And some optional feature may be defined in this file.
  * @details
  * @author  Ethan
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef _VENDOR_TP_CONFIG_H_
#define _VENDOR_TP_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t  data_check;
    uint8_t  initial_value;
    uint8_t  mode;
    /** 0 : 1M + w/o data length extension
        1 : 1M + w data length extension
        2 : 2M + w/o data length extension
        3 : 2M + w data length extension
        4 : Coded(s=2) + w/o data length extension
        5 : Coded(s=2) + w data length extension
        6 : Coded(s=8) + w/o data length extension
        7 : Coded(s=8) + w data length extension
        8 : 1M + w/o data length extension(BT4.0)
       */
    uint16_t con_interval;
    uint16_t conn_slave_latency;
    uint16_t conn_supervision_timeout;
    uint16_t length;
    uint32_t count;
} TTP_PERFER_PARAM;

#define GATT_UUID_VENDOR_TP_SERVICE                            0xA00D
#define GATT_UUID_VENDOR_TP_NOTIFY_INDICATE               0xB001
#define GATT_UUID_VENDOR_TP_PREFER_PARAM                  0xB002

#ifdef __cplusplus
}
#endif

#endif
