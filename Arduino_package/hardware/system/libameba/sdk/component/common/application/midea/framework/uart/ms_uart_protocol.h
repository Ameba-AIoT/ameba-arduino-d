/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.
 *
 * File Name 		: ms_network_msmart_pro_wifi.h
 * Introduction	: 
 *
 * Current Version: 
 * Author			: George.yang <jun7.yang@midea.com.cn>
 * Create Time	: 2015/09/03
 * Change Log		: create this file
 *
 * All software, firmware and related documentation here in ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 */

#ifndef __MS_UART_PROTOCOL_H__
#define __MS_UART_PROTOCOL_H__

typedef enum
{
	MS_UART_CTL_CMD                  = 0x02,
	MS_UART_QUERY_CMD                = 0x03,
	MS_UART_REPORT_RUNNING_PARA_NOACK= 0x04,
	MS_UART_REPORT_RUNNING_PARA_ACK  = 0x05,
	MS_UART_REPORT_ERROR_NOACK       = 0x06,
	MS_UART_SN_ACQUIRE               = 0x07,
	MS_UART_ADD_NOTIFICATION         = 0x08,/*Unimplemented*/
	MS_UART_QUERY_STATE              = 0x09,/*Unimplemented*/
	MS_UART_REPORT_ERROR_ACK         = 0x0a,
	MS_UART_ONLINE_NOTIFY            = 0x0d,
	MS_UART_ASSIGN_DEV_ID            = 0x0e,
	MS_UART_SET_DEV_ID               = 0x0f,
	MS_UART_WRITE_SN                 = 0x11,
	MS_UART_RENAME_SSID              = 0x12,
	MS_UART_GET_MAC                  = 0x13,
	MS_UART_ENTER_SELF_DETECT_MODE   = 0x14,
	MS_UART_ENTER_AFTER_SALE_MODE    = 0x15,
	MS_UART_LEAVE_AFTER_SALE_MODE    = 0x16,
	MS_UART_SYNCHRONIZE_TIME         = 0x61,
	MS_UART_GET_TEMPERATURE          = 0x62,/*Unimplemented*/
    MS_UART_DEV_QUERY_NETWORK_STATE  = 0x63,
	MS_UART_AIRCONDITION_RESTORE     = 0x64,
	MS_UART_AIRCONDITION_GET_INFO    = 0x65,
	MS_UART_CFG_UART                 = 0x66,/*Unimplemented*/
	MS_UART_CFG_UART_TIMER_SEND      = 0x67,/*Unimplemented*/
	MS_UART_AIRCONDITION_CONTROL_WIFI= 0x68,
	MS_UART_CFG_NET                  = 0x6a, 
	MS_UART_SCAN_WIFI_AP_LIST        = 0x6b,
	MS_UART_CFG_WIFI_PARA            = 0x70,/*Unimplemented*/	
	MS_UART_ROUTE_INFO				 = 0x71,
	MS_UART_NOTIFY_MOBILE_OFFLINE    = 0x7d,/*Unimplemented*/	
	MS_UART_MOBILE_CONNECT_WIFI_AUTH = 0x7e,/*Unimplemented*/
	MS_UART_SWITCH_WLAN_MODE         = 0x81,
	MS_UART_REBOOT_WIFI              = 0x82,
	MS_UART_RESTORE_WIFI_CFG         = 0x83,
	MS_UART_ENTER_CONFIG_MODE	     = 0x84,
	MS_UART_IDENTIFY        	     = 0x85,
	MS_UART_WIFI_VERSION_QUERY     	 = 0X87,
	MS_UART_QUERY_DEV_FW_VER         = 0x90,/*Unimplemented*/
	MS_UART_APPLIANCE_UPGRADE_CONFIRM = 0x91,
	MS_UART_APPLIANCE_FW_UPGRADE_SUCCESS = 0x92,
	MS_UART_REQUEST_UPDATE_FW        = 0x93,/*Unimplemented*/
	MS_UART_QUERY_DEV_INFO           = 0xa0,
	MS_UART_APPLIANCE_FW_DOWNLOAD	 = 0xa2,
	MS_UART_QUERY_APPLIANCE_UPGRADE  = 0xb3,
	MS_UART_DEV_SERVICE_QUERY        = 0xe1,
	MS_UART_ABNORMAL_RESPONSE        = 0xfe
}ms_pro_uart_type_t;

typedef struct MS_UART_MSG_HEAD_T
{
	uint8_t head;
    uint8_t msg_length;
	uint8_t dev_type;
	uint8_t frame_crc;
	uint8_t reserve[2];
	uint8_t msg_id;
	uint8_t frame_version;
	uint8_t dev_version;
	uint8_t cmd_type;
} ms_uart_msg_head_t;

#endif

