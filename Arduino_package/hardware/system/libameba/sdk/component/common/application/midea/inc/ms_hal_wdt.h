/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_hal_wdt.h
 * Introduction	: wdt
 *
 * Current Version	: v0.2
 * Author			: zouwei
 * Create Time	: 2015/10/09
 * Change Log	     	:(2015/10/15) 
 *
 * All software, firmware and related documentation herein ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 *
 */
#ifndef __MS_HAL_WDT_H__
#define __MS_HAL_WDT_H__

#include "FreeRTOS.h"

typedef enum
{
	MS_HAL_WDT_RESULT_SUCCESS 	= 0x00,
	MS_HAL_WDT_RESULT_ERROR 	= 0x01,
}ms_hal_wdt_result_t;

//wdt 
/**
* @Function	: set timeout of wdt driver
* @Parameter  msecs
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: set timeout of wdt success
*                  MS_HAL_SYS_RESULT_ERROR: set timeout of wdt failed
**/
ms_hal_wdt_result_t ms_hal_wdt_set_timeout(uint32_t msecs);

/**
* @Function	: start wdt driver
* @Parameter  null
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: start wdt success
*                  MS_HAL_SYS_RESULT_ERROR: start wdt failed
**/
ms_hal_wdt_result_t ms_hal_wdt_start(void);

/**
* @Function	: feed wdt driver
* @Parameter  null
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: feed wdt success
*                  MS_HAL_SYS_RESULT_ERROR: feed wdt failed
**/
ms_hal_wdt_result_t ms_hal_wdt_feed(void);

/**
* @Function	: stop wdt driver
* @Parameter  null
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: stop wdt success
*                  MS_HAL_SYS_RESULT_ERROR: stop wdt failed
**/
ms_hal_wdt_result_t ms_hal_wdt_stop(void);

#endif
