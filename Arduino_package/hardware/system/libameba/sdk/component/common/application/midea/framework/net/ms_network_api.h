/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.
 *
 * File Name 		: 
 * Introduction	: 
 *
 * Current Version: 
 * Author			: George.yang <jun7.yang@midea.com.cn>
 * Create Time	: 2015/09/11
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

#ifndef __MS_NETWORK_API_H__
#define __MS_NETWORK_API_H__

typedef enum MS_NETWORK_RESULT_T
{
	MS_NETWORK_RESULT_SUCCESS 	= 0x00,
	MS_NETWORK_RESULT_ERROR		= 0x01,
}ms_network_result_t;

typedef enum MS_MCLOUD_STATUS_T
{
	MS_CLOUD_CONNECTED 		= 0x00,
	MS_CLOUD_UNCONNECTED 	= 0x01,
	MS_CLOUD_UNSTABLE		= 0x02,
	MS_CLOUD_DNS_ERROR		= 0x03,
	MS_CLOUD_REJECT_CONNECT	= 0x04,
	MS_CLOUD_MAINTAIN		= 0x05,
	MS_CLOUD_FAULT			= 0xFF,
}ms_mcloud_status_t;

typedef enum MS_NETWORK_MODE_T
{
	MS_NETWORK_AP_MODE = 0x00,
	MS_NETWORK_STA_MODE = 0x01,
}ms_network_mode_t;

typedef enum MS_NETWORK_SERVICE_TYPE_T
{
	MS_NETWORK_SERVICE_NETWORK_MODE 		= 0x02,
	MS_NETWORK_SERVICE_CLOUD_CONNECT_STATUS	= 0x03,
	MS_NETWORK_SERVICE_CLIENT_NUMBER		= 0x04,
	MS_NETWORK_SERVICE_UPDATE_NETWORK_STATUS = 0x05,
	MS_NETWORK_SERVICE_SYSTEM_TIME			= 0x06,
	MS_NETWORK_SERVICE_AUTHENTICATION_FLAG	= 0x07,
}ms_network_service_type_t;

typedef enum MS_NETWORK_MSG_TYPE_T
{
	MS_NETWORK_MSG_COMMON_COMMAND	 = 0x00,
	MS_NETWORK_EVENT_LOGIN_CLOUD_FAILED,
	MS_NETWORK_EVENT_LOGIN_CLOUD_SUCCESS,
	MS_NETWORK_EVENT_REBOOT_REQUEST,
	MS_NETWORK_EVENT_FACTORY_RESET_REQUEST,
	MS_NETWORK_EVENT_NETWORK_STATUS_UPDATE,
	MS_NETWORK_EVENT_CLIENT_SOCKET_NUM_UPDATE,
	MS_NETWORK_EVENT_NEWORK_AUTH_START,
	MS_NETWORK_EVENT_NETWORK_AUTH_SUCCESS,
}ms_network_msg_type_t;

typedef enum MS_NETWORK_SOURCE_T
{
	MS_SOURCE_MCLOUD		= 0x00,
	MS_SOURCE_MDEVICE1		= 0x01,
	MS_SOURCE_MDEVICE2		= 0x02,
	MS_SOURCE_MDEVICE3		= 0x03,
	MS_SOURCE_MDEVICE4		= 0x04,
	MS_SOURCE_MDEVICE5		= 0x05,
	MS_SOURCE_MDEVICE_BROADCAST = 0x0e,
	MS_SOURCE_MIDEA_ALL		= 0x0f,
	MS_SOURCE_UNKNOWN		= 0x50,
	MS_SOURCE_ALL    		= 0xFF,
}ms_network_source_t;

typedef struct MS_NETWORK_MSG_T
{	
	ms_network_msg_type_t		type;
	ms_core_data_t				data;
	uint32_t        			source;
	uint16_t                    socket;
	struct sockaddr_in          ip_addr;
}ms_network_msg_t;

typedef struct MS_NETWORK_TIME_T
{
	uint8_t time_zone;
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t second;
}ms_network_time_t;

int ms_network_api_init(void);
int ms_network_api_unint(void);

int ms_network_api_read(ms_network_msg_t *msg);
int ms_network_api_write(ms_network_msg_t *msg);

int ms_network_api_start_network(ms_network_mode_t network_mode);
int ms_network_api_stop_network(void);

void ms_network_api_notify_dev_service(ms_uart_service_type_t service_type, uint8_t *param);
void ms_network_api_obtain_dev_service(ms_network_service_type_t service_type, uint8_t *param);

int ms_network_api_uart_cmd_handle(ms_uart_msg_t *msg);
int ms_network_api_network_cmd_handle(ms_network_msg_t *msg);

#endif

