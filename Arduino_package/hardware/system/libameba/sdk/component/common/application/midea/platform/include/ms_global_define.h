/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_global_define.h
 * Introduction	: this is the global header file for error trace
 *
 * Current Version	: v0.1
 * Author			: william <fuxin.liu@midea.com.cn>
 * Create Time	: 2015/8/28
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

#ifndef __MS_GLOBAL_DEFINE_H__
#define __MS_GLOBAL_DEFINE_H__

#include "ms_hal_chip.h"

#ifndef bool
#define bool unsigned char
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MS_ARRAY_SIZE
#define MS_ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

#define TRUE    					1
#define FALSE   					0 
#define SETB(m,n)   				(m |=(0x01<<n))
#define TESTB(m,n)   				((m & (0x01<<n)) >> n)
#define RESETB(m,n)     			(m &= ~((0x01)<<n))
#define MS_MIN(a,b) 				(((a) <= (b)) ? (a) : (b))
#define MS_MAX(a,b) 				(((a) >= (b)) ? (a) : (b))
#define MS_NETWORK_BUFFER_LENGTH 	600
#define MS_OS_THREAD_PRIO_0 		4 /* high */
#define MS_OS_THREAD_PRIO_1 		3 
#define MS_OS_THREAD_PRIO_2 		2 
#define MS_OS_THREAD_PRIO_3 		1 
#define MS_OS_THREAD_PRIO_4	 		0 /* low */
 
typedef enum MS_REPORT_LEVEL_T
{
	MS_ERROR = 0,
	MS_INFO,
	MS_DEBUG
}ms_report_level_t;

#define MS_PRINT_LEVEL(report_levl) \
	do     \
	{ \
		if(report_levl == MS_ERROR)\
		{ \
			printf("\033[1;40;31m ");\
			printf("ERROR: ");\
		}\
		else if(report_levl == MS_INFO)\
		{ \
			printf("INFO: ");\
		}\
		else if(report_levl == MS_DEBUG)\
		{ \
			printf("DEBUG: ");\
		} \
		else \
		{ \
		} \
    }while(0)

#define MS_TRACE(report_levl,error_message) \
	do   \
	{ \
		MS_PRINT_LEVEL(report_levl);\
		printf("[%s %d]",__FUNCTION__,__LINE__);\
		printf error_message;\
		if(report_levl == MS_ERROR){printf("\033[0m \n");}\
		else{printf("\r\n");}\
	}while(0)

#define MS_ASSERT(e) \
			do{ \
				((e) ? (void)0 : (void)(printf("\033[1;40;31m [MS_ASSERT]Assertion failed: %s ,file %s, line %d \033[0m \r\n", #e, __FUNCTION__, __LINE__))); \
			}while(0)
			
typedef enum MS_STATUS_CODE_T
{
	MS_SUCCESS = 0,
	MS_DATA_SUCCESS,
	MS_ERROR_BAD_PARAMETER,
	MS_ERROR_NO_MEMORY,
	MS_ERROR_ALREADY_INITIALIZED,
	MS_ERROR_INVALID_HANDLE,
	MS_ERROR_TIMEOUT,
	MS_ERROR_INIT_FAIL,
	MS_ERROR_TASK_CREATE_FAIL,
	MS_ERROR_NO_INIT,
	MS_ERROR_DATA_ERR,
	MS_FAIL
}ms_status_code_t;

typedef struct MS_CORE_DATA_T
{
	uint8_t  addr[MS_NETWORK_BUFFER_LENGTH];
	uint16_t len;	
}ms_core_data_t;

#endif	

