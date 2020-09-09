/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_hal_flash.h
 * Introduction	: flash operation
 *
 * Current Version	: v0.1
 * Author			: zouwei
 * Create Time	: 2015/10/09
 * Change Log	     	:(2015/10/09) 
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
 
#ifndef __MS_HAL_FLASH_H__
#define __MS_HAL_FLASH_H__
	 
#include "FreeRTOS.h"
#include "flash_api.h"

typedef int32_t ms_hal_flash_handle_t;

typedef enum
{
	MS_HAL_FLASH_RESULT_SUCCESS 	= 0x00,
	MS_HAL_FLASH_RESULT_ERROR 		= 0x01,
}ms_hal_flash_result_t;

//spi flash operation
/**  
* @Function : open the Flash Driver
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: open flash driver success
*         	MS_HAL_SYS_RESULT_ERROR: open flash driver failed
**/
ms_hal_flash_result_t ms_hal_flash_open(void);

/**  
* @Function : read data from flash
*           flash_addr: read address in flash
*			data: Pointer to store the read data			
*			data_len: length of data to be read
*
* @return	MS_HAL_SYS_RESULT_SUCCESS: read data from flash success
*          	MS_HAL_SYS_RESULT_ERROR: read data from flash failed
**/
ms_hal_flash_result_t ms_hal_flash_read(uint32_t flash_addr, uint8_t *data, uint32_t data_len);

/**  
* @Function : write data to flash
*           flash_addr: write address in flash
*			data: Pointer to store the write data			
*			data_len: length of data to be write
*
* @return	MS_HAL_SYS_RESULT_SUCCESS: write data to flash success
*         	MS_HAL_SYS_RESULT_ERROR: write data to flash failed
**/
ms_hal_flash_result_t ms_hal_flash_write(uint32_t flash_addr, uint8_t *data, uint32_t data_len);

/**  
* @Parameter  handle:  flash operation handle
*                   flash_addr: start address in flash
*			size: length of data to be erased
*
* @return	MS_HAL_SYS_RESULT_SUCCESS: erase data success
*         	MS_HAL_SYS_RESULT_ERROR: erase data failed
**/
ms_hal_flash_result_t ms_hal_flash_erase(uint32_t flash_addr, uint32_t size);


/**  
* @Function : close the flash driver
*
* @return	MS_HAL_SYS_RESULT_SUCCESS: close flash driver success
*         	MS_HAL_SYS_RESULT_ERROR: close flash driver failed
**/
ms_hal_flash_result_t ms_hal_flash_close(void);

#endif