/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.
 *
 * File Name 		: chip.h
 * Introduction	: this is the header file about chip related.
 *
 * Current Version	: v1.0
 * Author			: zouwei <wei.zou@midea.com.cn>
 * Create Time	: 2015/09/18
 * Change Log		: (2015/09/18) 1. Initial Verison
 *
 * All software, firmware and related documentation here in ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 */

#ifndef __MS_HAL_CHIP_H__
#define __MS_HAL_CHIP_H__

#include "ms_hal_wlan.h"
#include "ms_hal_os.h"
#include "ms_hal_socket.h"
#include "ms_hal_uart.h"
#include "ms_hal_nvram.h"
#include "ms_hal_sys.h"
#include "ms_hal_flash.h"
#include "ms_hal_wdt.h"

//#define SUPERNOVA_PLATFORM_BROADCOM_CHIP_BCM4336
//#define SUPERNOVA_PLATFORM_REALTEK_CHIP_8711AM
#define SUPERNOVA_PLATFORM_REALTEK_CHIP_8711AF
//#define SUPERNOVA_PLATFORM_MTK_CHIP_MT7687
//#define SUPERNOVA_PLATFORM_NUFRONT_CHIP_NL6621
//#define SUPERNOVA_PLATFORM_ATMEL_CHIP_WINC1500

#ifdef SUPERNOVA_PLATFORM_BROADCOM_CHIP_BCM4336
#include "wiced.h"
#include "stdlib.h"
#define MS_OS_NETWORK_THREAD_STACK_SIZE		12288  	//12k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192   	//8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		5120	//5k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OTA_BASE_ADDR					0x00000 //OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0x64000 //appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x06	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY			

#elif defined SUPERNOVA_PLATFORM_REALTEK_CHIP_8711AM
#define MS_OS_NETWORK_THREAD_STACK_SIZE		11264  	//11k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192    //8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		8192	//8k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OTA_BASE_ADDR					0x80000	//OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0x100000 //appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x06	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY			{\
												ms_hal_os_thread_sleep(1);\
											}

#elif defined SUPERNOVA_PLATFORM_REALTEK_CHIP_8711AF
#define MS_OS_NETWORK_THREAD_STACK_SIZE		11264  	//11k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192    //8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		8192	//8k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OTA_BASE_ADDR					0x64000	//OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0xBB800 //appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x06	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY			{\
												ms_hal_os_thread_sleep(1);\
											}

#elif defined SUPERNOVA_PLATFORM_MTK_CHIP_MT7687
#define MS_OS_NETWORK_THREAD_STACK_SIZE		12288  	//12k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192   	//8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		6144	//6k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OTA_BASE_ADDR					0xFC000	//OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0x00000	//appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x04	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY

#elif defined SUPERNOVA_PLATFORM_NUFRONT_CHIP_NL6621
#define MS_OS_NETWORK_THREAD_STACK_SIZE		17508  	//17k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192   	//8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		6144	//6k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_2
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_1
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_4
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_0
#define MS_OTA_BASE_ADDR					0x34000	//OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0x7D000	//appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x04	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY			{\
												ms_hal_os_thread_sleep(10);\
											}

#elif defined SUPERNOVA_PLATFORM_ATMEL_CHIP_WINC1500
#define MS_OS_NETWORK_THREAD_STACK_SIZE		17508  	//17k for network thread.
#define MS_OS_LOGIC_THREAD_STACK_SIZE		8192   	//8k for logic thread. 
#define MS_OS_UART_THREAD_STACK_SIZE 		6144	//6k for uart thread.
#define MS_OS_OTA_THREAD_STACK_SIZE			4096	//4k for ota thread.
#define MS_OS_MSC_THREAD_STACK_SIZE			6144	//6k for msc thread.
#define MS_OS_NETWORK_THREAD_PRIO			MS_OS_THREAD_PRIO_3
#define MS_OS_LOGIC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_UART_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_OTA_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OS_MSC_THREAD_PRIO				MS_OS_THREAD_PRIO_3
#define MS_OTA_BASE_ADDR					0xFC000	//OTA upgrade address
#define MS_APPLIANCE_OTA_BASE_ADDR			0x00000 //appliance upgrade address
#define MS_UART_BUFFER_DEPTH			   	0x04	//uart buffer size
#define MS_OS_THREAD_DEFUALT_DELAY
#endif

#endif

