/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_sys_api.h
 * Introduction	: system control
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


#ifndef __MS_SYS_API_H__
#define __MS_SYS_API_H__

#include "ms_fw_sys_wlan.h"

typedef enum
{
	MS_SYS_SERVICE_UART_RETRANSM 	= 0x00,
	MS_SYS_SERVICE_FRAME_TYPE 		= 0x01,
	MS_SYS_SERVICE_DEV_UPGRADE		= 0x02,
	MS_SYS_SERVICE_BOOT_MODE		= 0x03,
	MS_SYS_SERVICE_LOW_POWER		= 0x04,
	MS_SYS_SERVICE_AUTH_REQUIRE		= 0x05,
	MS_SYS_SERVICE_AUTH_TIME		= 0x06,
	MS_SYS_SERVICE_AP_SNIFFER_TIME	= 0x07,
}ms_sys_api_service_type_t;

typedef enum
{
	MS_SYS_SERVICE_TIME_FRAME 		= 0x00,
	MS_SYS_SERVICE_SEQUENTIAL_FRAME = 0x01,
}ms_sys_api_service_frame_type_t;

typedef enum
{
	MS_SYS_SERVICE_BOOT_AP_MODE		= 0x00,
	MS_SYS_SERVICE_BOOT_SNIFFER_MODE = 0x01,
	MS_SYS_SERVICE_BOOT_IDLE_MODE	= 0x02,
}ms_sys_api_service_boot_type_t;

typedef enum
{
	MS_SYS_SERVICE_UART_RETRANSM_UNSUPPORT 	= 0x00,
	MS_SYS_SERVICE_UART_RETRANSM_SUPPORT	= 0x01,
}ms_sys_api_service_uart_retransm_t;

typedef enum
{
	MS_SYS_SERVICE_DEV_UPGRADE_UNSUPPORT = 0x00,
	MS_SYS_SERVICE_DEV_UPGRADE_SUPPORT	= 0x01,
}ms_sys_api_service_dev_upgrade_t;

typedef enum
{
	MS_SYS_SERVICE_LOW_POWER_UNSUPPORT 	= 0x00,
	MS_SYS_SERVICE_LOW_POWER_SUPPORT	= 0x01,
}ms_sys_api_service_low_power_t;

typedef enum
{
	MS_SYS_SERVICE_AUTH_NOT_REQUIRED	= 0x00,
	MS_SYS_SERVICE_AUTH_REQUIRED		= 0x01,
}ms_sys_api_service_auth_required_t;

typedef enum 
{
	MS_BOOT_AP_MODE  		 	= '0',
	MS_BOOT_STA_MODE 			= '1',
	MS_BOOT_AFTER_SALE_MODE	 	= '2',
	MS_BOOT_SELF_TEST_MODE    	= '3',
	MS_BOOT_MIDEA_MSC_MODE		= '4',
	MS_BOOT_ALI_CFG_MODE		= '5',
	MS_BOOT_JD_CFG_MODE			= '6',
	MS_BOOT_XM_CFG_MODE			= '7',
	MS_BOOT_AIRKISS_MODE		= '8',
	MS_BOOT_UNKNOWN      		= '9',
	MS_BOOT_IDLE_MODE			= 'A',
}ms_sys_api_boot_mode_t;

typedef struct ms_dev_service_s
{
	uint8_t service_version[2];
	ms_sys_api_service_uart_retransm_t uart_retransm;
	ms_sys_api_service_dev_upgrade_t dev_update;
	ms_sys_api_service_frame_type_t frame_type;
	ms_sys_api_service_boot_type_t boot_type;
	ms_sys_api_service_low_power_t low_power;
	ms_sys_api_service_auth_required_t auth_required;
	uint8_t auth_time;
	uint8_t ap_sniffer_time;
}ms_dev_service_t;

void ms_sys_api_init(void);
void ms_sys_api_reboot(void);
int ms_sys_api_factoryReset(void);
int ms_sys_api_wlan_init(ms_hal_wlan_event_handler_t event_handler);
int ms_sys_api_wlan_uninit(void);
int ms_sys_api_start_scan(uint8_t number, ms_hal_wlan_scan_result_callback_t callback);
int ms_sys_api_stop_scan(void);
int ms_sys_api_wlan_radio_on(void);
int ms_sys_api_wlan_radio_off(void);
int ms_sys_api_wlan_get_radio_status(void);
int ms_sys_api_start_sta_with_param(ms_hal_wlan_ap_connect_param_t *connect_param);
int ms_sys_api_start_sta(void);
int ms_sys_api_disconnect_ap(void);
int ms_sys_api_get_sta_status(uint32_t *status);
int ms_sys_api_start_ap_with_param(ms_hal_wlan_ap_start_param_t *ap_start_param);
int ms_sys_api_start_ap(void);
int ms_sys_api_stop_ap(void);
int ms_sys_api_get_ap_status(uint32_t *status);
int ms_sys_api_get_mac(uint8_t* mac);
int ms_sys_api_get_ip(uint32_t mode, uint32_t *ip);
int ms_sys_api_get_rssi( int32_t *rssi );
int ms_sys_api_set_listen_interval(uint8_t listen_interval);
int ms_sys_api_get_listen_interval( uint8_t *interval );
int ms_sys_api_get_channel(uint32_t band, uint32_t *channel);
int ms_sys_api_set_channel( uint32_t channel);
int ms_sys_api_start_sniffer(ms_hal_wlan_sniffer_param_t *sniffer_param);
int ms_sys_api_stop_sniffer(void);
int ms_sys_api_start_msc(void);
int ms_sys_api_start_airkiss(void);
int ms_sys_api_get_wlan_mode(ms_wlan_mode_t *wlan_mode);

int ms_sys_nvram_set_ap_ssid(uint8_t *ssid);
int ms_sys_nvram_get_ap_ssid(uint8_t *ssid, uint32_t len);
int ms_sys_nvram_set_sta_ssid(uint8_t *ssid);
int ms_sys_nvram_get_sta_ssid(uint8_t *ssid, uint32_t len);
int ms_sys_nvram_set_sta_passwd(uint8_t *password);
int ms_sys_nvram_get_sta_passwd(uint8_t *password, uint32_t len);
int ms_sys_nvram_set_sta_authType(ms_hal_wlan_security_t auth_type);
int ms_sys_nvram_get_sta_authType(ms_hal_wlan_security_t *auth_type);
int ms_sys_nvram_get_boot_mode(ms_sys_api_boot_mode_t *boot_mode);
int ms_sys_nvram_set_boot_mode(ms_sys_api_boot_mode_t boot_mode);
int ms_sys_nvram_get_device_type(uint8_t *dev_type);
int ms_sys_nvram_set_device_type(uint8_t dev_type);
int ms_sys_nvram_get_serverIP(uint32_t* server_ip);
int ms_sys_nvram_set_sn(uint8_t *sn);
int ms_sys_nvram_get_sn(uint8_t *sn, uint32_t len);
int ms_sys_nvram_set_serverIP(uint32_t server_ip);
int ms_sys_nvram_get_serverIP(uint32_t* server_ip);
int ms_sys_nvram_set_dev_service(ms_dev_service_t *dev_service);
bool ms_sys_nvram_get_dev_service_exist(void);
int ms_sys_nvram_get_dev_service(ms_sys_api_service_type_t service_type, uint8_t* param);
int ms_sys_nvram_set_self_detect(bool b_self_detect_mode);
int ms_sys_nvram_get_self_detect_state(bool *b_self_detect_mode);

#endif

