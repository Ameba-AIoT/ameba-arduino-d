/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_hal_sys.h
 * Introduction	: system control
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
 
#ifndef __MS_HAL_SYS_H__
#define __MS_HAL_SYS_H__

//system control
/**
* @Function	: Init the chip, including hardware init & system init
* @Parameter  null
*                  
* @return	MS_HAL_SYS_RESULT_SUCCESS: init success
*           MS_HAL_SYS_RESULT_ERROR: init error
**/
void ms_hal_sys_init(void);

/**
* @Function	: reboot the system
* @Parameter  null
*                  
* @return	null
**/
void ms_hal_sys_reboot(void);


/**
 * @Function	: start ota upgrade
 * @Parameter  addr: image start addr in spi flash
 *             len£º image size
 */
void ms_hal_sys_ota_upgrade(uint32_t addr, uint32_t len);


/** 
 * @Function: read license from otp.
 * @Parameter  license: buffer to store the license
 *			   len: buffer length
 * @return 0: read license successfully
 *         !0: read license failed
 */
int ms_hal_sys_read_license(uint8_t *license, uint8_t len);


/** 
 * @Function: write license to otp.(write once, just for test)
 * @Parameter  license: license buffer
 *			   len: buffer length
 * @return 0: write license successfully
 *         !0: write license failed
 */
void ms_hal_sys_write_license(uint8_t *license, uint8_t len);
#endif
