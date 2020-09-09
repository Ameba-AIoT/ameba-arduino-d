/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: nvm.h
 * Introduction	: this is the header file about the api of midea_wifi_nvm
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

#ifndef __MS_HAL_NVRAM_H__
#define	__MS_HAL_NVRAM_H__

typedef int32_t ms_hal_nvm_handle_t;

typedef enum
{
	MS_HAL_NVM_RESULT_ERROR = -1,
	MS_HAL_NVM_RESULT_SUCCESS = 0
}ms_hal_nvm_result_t;


/**
* @Function	: open the registered module, if not found, register it.
* @Parameter handle: if open success will return a new handle
*                  module_name:the name of the module to open
* @return	MS_HAL_NVM_RESULT_SUCCESS :open success
*                 MS_HAL_NVM_RESULT_ERROR     :open fail
**/
ms_hal_nvm_result_t  ms_hal_nvm_open_module (ms_hal_nvm_handle_t *handle, const char *module_name);

/**
* @Function	: close the opened module
* @Parameter handle: the handle pointed to the opened module 
* @return	MS_HAL_NVM_RESULT_SUCCESS :close success
*                 MS_HAL_NVM_RESULT_ERROR     :close fail
**/
ms_hal_nvm_result_t  ms_hal_nvm_close_module (ms_hal_nvm_handle_t *handle);

/**
* @Function	: backup the specify variable`s value when power off
* @Parameter handle:ponit the the save storage blocks  descriptor
*                  variable_name:the variable name which want to save
*                  value :the value want to save
* @return	MS_HAL_NVM_RESULT_SUCCESS :save success
*                 MS_HAL_NVM_RESULT_ERROR     :save fail
**/
ms_hal_nvm_result_t  ms_hal_nvm_save_data (ms_hal_nvm_handle_t *handle, const char *variable_name, const char *value);


/**
* @Function	: restore specify variable`s value
* @Parameter handle:ponit the the save storage blocks  descriptor
*                  variable_name:the variable name which want to read
*                  buffer :the value want to backup
*                  buffer_size:buffer size
* @return	MS_HAL_NVM_RESULT_SUCCESS :backup success
*                 MS_HAL_NVM_RESULT_ERROR     :backup fail
**/
ms_hal_nvm_result_t  ms_hal_nvm_get_data (ms_hal_nvm_handle_t *handle, const char *variable_name, char *buffer, uint16_t buffer_size);


/**
* @Function	: erase a storage blocks
* @Parameter module_name:the name of the modules which will be erase
* @return	MS_HAL_NVM_RESULT_SUCCESS :register success
*                 MS_HAL_NVM_RESULT_ERROR     :register fail
**/
ms_hal_nvm_result_t  ms_hal_nvm_erase_module (const char *module_name);


#endif 

