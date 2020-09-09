/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: sys_wlanl.h
 * Introduction	: wlan control
 *
 * Current Version	: v0.1
 * Author			: zouwei <wei.zou@midea.com.cn>
 * Create Time	: 2015/8/28
 * Change Log	:
 *
 * All software, firmware and related documentation here in ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 */
#ifndef __FW_SYS_WLAN_H__
#define __FW_SYS_WLAN_H__


#define MS_FW_WLAN_SSID_MAX_LEN 32
#define MS_FW_WLAN_PASSWD_MAX_LEN 32


typedef enum
{
	MS_WLAN_RESULT_SUCCESS 	= 0x0,
	MS_WLAN_RESULT_ERROR	= 0x1,
}ms_wlan_reslut_t;

typedef enum
{
	/*	wlan event	  */
	MS_WLAN_EVT_LINK_LOST				= 0x10,
	MS_WLAN_EVT_CHANNEL_SWITCH			= 0x12,
	MS_WLAN_EVT_NETWORK_RESET			= 0x13,
	MS_WLAN_EVT_DHCP_RENEW				= 0x14,
	/*	sta event.	   */
	MS_WLAN_EVT_STA_CONNECT_SUCCESS		= 0x82,
	MS_WLAN_EVT_STA_CONNECT_FAILED		= 0x84, 

	/*  msc event.     */
	MS_WLAN_EVT_MSC_SUCCESS				= 0x90,
	MS_WLAN_EVT_MSC_ERROR				= 0x91,
	/*  airkiss event. */
	MS_WLAN_EVT_AIRKISS_SUCCESS			= 0x92,
	MS_WLAN_EVT_AIRKISS_ERROR			= 0x93,
	
	/*	ap event.	   */
	MS_WLAN_EVT_AP_START_SUCCESS 		= 0xC2,
	MS_WLAN_EVT_AP_START_FAILED			= 0xC4,
}ms_wlan_event_t;

typedef enum
{
	MS_WLAN_MODE_AP  	= 0x00,
	MS_WLAN_MODE_STA    = 0x01,
	MS_WLAN_MODE_IDEL 	= 0x02,
	MS_WLAN_MODE_STA_CONNECTING = 0x03,
	MS_WLAN_MODE_SNIFFER = 0x04,
	MS_WLAN_MODE_UNKNOWN= 0x0F,
}ms_wlan_mode_t;

typedef int (*ms_sys_wlan_msc_handler_t)(uint8_t state);

typedef struct
{
	int (*wlan_cb)(ms_hal_wlan_event_t event, void *data);
	int (*get_radio_status)(void);
	int (*start_scan)(void *scan_param);
	int (*stop_scan)(void);
	int (*connect_ap)(void *connect_param);
	int (*reconnect_ap)(void *connect_param);
	int (*join_ap)(void	*join_param);
	int (*disconnect_ap)(void);
	ms_hal_wlan_sta_status_t (*get_ap_connecting_status)(void);
	int (*start_ap)(void *ap_start_param);
	int (*stop_ap)(void);
	ms_hal_wlan_ap_status_t (*get_ap_status)(void);
	int (*get_mac_address)(ms_hal_wlan_mac_t *mac);
	int (*set_mac_address)(ms_hal_wlan_mac_t *mac);
	int (*get_ip_address)(ms_hal_wlan_mode_t mode, ms_hal_wlan_ip_t *ip);
	int (*set_ip_address)(ms_hal_wlan_ip_t *ip);
	int (*enable_powersaving)(ms_hal_wlan_powersaving_mode_t mode);
	int (*disable_powersaving)(void);
	int (*get_tx_power)( uint8_t *dbm );
	int (*set_tx_power)( uint8_t dbm );
	int (*get_rssi)( int32_t *rssi );
	int (*set_listen_interval)(uint8_t listen_interval);
	int (*get_listen_interval)( uint8_t *interval );
	int (*get_channel)(ms_hal_wlan_802_11_band_t band, uint32_t *channel);
	int (*set_channel)( uint32_t channel);
	int (*start_sniffer)(ms_hal_wlan_sniffer_param_t *sniffer_param);
	int (*stop_sniffer)(void);
	void (*start_msc)(void);
	int (*start_airkiss)(void);
	int (*get_wlan_mode)(ms_wlan_mode_t *pcMode);
}ms_wlan_ctrl_t;

#endif

