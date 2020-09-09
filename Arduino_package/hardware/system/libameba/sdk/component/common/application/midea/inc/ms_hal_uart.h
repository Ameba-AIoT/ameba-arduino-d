/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_hal_uart.h
 * Introduction	: this is the header file about the api of midea_wifi_uart
 *
 * Current Version	: v2.1
 * Author			: william <weimin.luo@midea.com.cn>
 * Create Time	: 2015/8/21
 * Change Log		: (2015/08/26) 1.rename this file  2.added the MS_HAL_ to each enum and struct
 *                                3.add comment to each function
 *
 * All software, firmware and related documentation here in ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 */

#ifndef __MS_HAL_UART_H__
#define	__MS_HAL_UART_H__

typedef   int32_t  ms_hal_hw_uart_handle_t;

typedef enum
{
	MS_HAL_BAUDRATE_300,
	MS_HAL_BAUDRATE_600,
	MS_HAL_BAUDRATE_1200,
	MS_HAL_BAUDRATE_2400,
	MS_HAL_BAUDRATE_4800,
	MS_HAL_BAUDRATE_9600,
	MS_HAL_BAUDRATE_19200,
	MS_HAL_BAUDRATE_38400,
	MS_HAL_BAUDRATE_43000,
	MS_HAL_BAUDRATE_56000,
	MS_HAL_BAUDRATE_57600,
	MS_HAL_BAUDRATE_115200,
}ms_hal_uart_baudrate_t;

typedef enum
{
    MS_HAL_DATA_WIDTH_5BIT,
    MS_HAL_DATA_WIDTH_6BIT,
    MS_HAL_DATA_WIDTH_7BIT,
    MS_HAL_DATA_WIDTH_8BIT,
    MS_HAL_DATA_WIDTH_9BIT,
} ms_hal_uart_data_width_t;

typedef enum
{
    MS_HAL_STOP_BITS_1,
    MS_HAL_STOP_BITS_2,
} ms_hal_uart_stop_bits_t;

typedef enum
{
    MS_HAL_FLOW_CONTROL_DISABLED,
    MS_HAL_FLOW_CONTROL_CTS,
    MS_HAL_FLOW_CONTROL_RTS,
    MS_HAL_FLOW_CONTROL_CTS_RTS,
} ms_hal_uart_flow_control_t;

typedef enum
{
    MS_HAL_NO_PARITY,
    MS_HAL_ODD_PARITY,
    MS_HAL_EVEN_PARITY,
} ms_hal_uart_parity_t;

typedef enum
{
	MS_HAL_UART_RESULT_SUCCESS = 0,
	MS_HAL_UART_RESULT_ERROR   = 1,
}ms_hal_uart_result_t;

typedef struct
{
	ms_hal_uart_baudrate_t baudrate;
	ms_hal_uart_data_width_t data_width;
	ms_hal_uart_parity_t parity;
	ms_hal_uart_stop_bits_t stop_bits;
	ms_hal_uart_flow_control_t flow_control;
}ms_hal_uart_config_t;

typedef enum
{
	MS_HAL_UART_DEV_1, //for home appliance communication
	MS_HAL_UART_DEV_2, //for uart debug
	MS_HAL_UART_DEV_MAX,
}ms_hal_uart_device_t;


/**
* @Function	: open a uart port and set params
* @Parameter uart_dev:uart device name
*                  uart_cfg:uart paramas
*                  handle: return a handle to the serial port
* @return	MS_HAL_UART_RESULT_SUCCESS :open success
*                 MS_HAL_UART_RESULT_ERROR     :open fail
**/
ms_hal_uart_result_t  ms_hal_hw_uart_open(const ms_hal_uart_device_t uart_dev, ms_hal_uart_config_t *uart_cfg, ms_hal_hw_uart_handle_t *handle);


/**
* @Function	: close a uart device
* @Parameter  handle: which uart device will to be close,get by open
* @return	MS_HAL_UART_RESULT_SUCCESS :close success
*                 MS_HAL_UART_RESULT_ERROR     :close fail
**/
ms_hal_uart_result_t  ms_hal_hw_uart_close( ms_hal_hw_uart_handle_t handle );


/**
* @Function	: write data to uart
* @Parameter  handle: which uart device will to writen
*                    buffer:store the write data
*                    len:write data length
* @return	Actually written data bytes
**/
uint32_t  ms_hal_hw_uart_write( ms_hal_hw_uart_handle_t handle, uint8_t *buffer, uint32_t len );


/**
* @Function	: read data from uart
* @Parameter  handle: will read data from which uart
*                    buffer:the data will store in this buffer
*                    len:read data length
* @return	Actually read data bytes
**/
uint32_t  ms_hal_hw_uart_read ( ms_hal_hw_uart_handle_t handle, uint8_t *buffer, uint32_t  len );

#endif

