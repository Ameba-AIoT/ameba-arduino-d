/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_uart_api.h
 * Introduction	: this is the uart module data  core head file
 *
 * Current Version	: v0.1
 * Author			: william <fuxin.liu@midea.com.cn>
 * Create Time	: 2015/9/12
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

#ifndef _MS_UART_API_H__
#define _MS_UART_API_H__

typedef enum MS_NETWORK_SERVICE_TYPE_T ms_network_service_type_t;

typedef enum MS_UART_MSG_TYPE_T
{
	MS_UART_MSG_COMMON_COMMAND = 0,
	MS_UART_EVENT_START_SUCCESS,
	MS_UART_EVENT_NETWORK_ON,
	MS_UART_EVENT_NETWORK_OFF,
	MS_UART_EVENT_REBOOT_REQUEST,
	MS_UART_EVENT_FACTORY_RESET_REQUEST,
	MS_UART_EVENT_ENTER_SELF_DETECT_MODE,
	MS_UART_EVENT_AUTH_SUCCESS,
}ms_uart_msg_type_t;

typedef enum MS_UART_SERVICE_TYPE_T
{
	MS_UART_SERVICE_UART_RETRANSM_FLAG		= 0x10,
	MS_UART_SERVICE_SELF_DETECT_FLAG		= 0x12,
	MS_UART_SERVICE_DEVICE_SN				= 0x14,
	MS_UART_SERVICE_FRAME_TYPE				= 0x16,
	MS_UART_SERVICE_DEV_TYPE				= 0x18,
	MS_UART_SERVICE_AUTHENTICATION_FLAG		= 0x20,
}ms_uart_service_type_t;

typedef struct MS_UART_MSG_T
{
	ms_uart_msg_type_t	type;
	ms_core_data_t		message;
	uint32_t     		destination;
}ms_uart_msg_t;

int ms_uart_api_init(void);
int ms_uart_api_uninit(void);

int ms_uart_api_read(ms_uart_msg_t * const msg);
int ms_uart_api_write(const ms_uart_msg_t * const msg);

void ms_uart_api_notify_dev_service(ms_network_service_type_t service_type, uint8_t *param);
void ms_uart_api_obtain_dev_service(ms_uart_service_type_t service_type, uint8_t *param);

#endif

