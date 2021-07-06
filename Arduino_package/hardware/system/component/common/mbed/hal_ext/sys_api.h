/** mbed Microcontroller Library
  ******************************************************************************
  * @file    sys_api.h
  * @author 
  * @version V1.0.0
  * @brief   This file provides following mbed system API:
  *				-JTAG OFF
  *				-LOGUART ON/OFF
  *				-OTA image switch
  *				-System Reset
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015, Realtek Semiconductor Corp.
  * All rights reserved.
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  ****************************************************************************** 
  */
#ifndef MBED_SYS_API_H
#define MBED_SYS_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup sys SYSTEM
 *  @ingroup    hal
 *  @brief      system functions
 *  @{
 */

///@name Ameba Common
///@{

/**
  * @brief  Turn off the JTAG function
  * @retval none  
  */
void sys_jtag_off(void);

/**
  * @brief  switch OTA image if the othe OTA image is valid
  * @retval none
  * @note for AmebaZ, sys_clear_ota_signature is the same with sys_recover_ota_signature
  */
void sys_clear_ota_signature(void);

/**
  * @brief  switch OTA image if the othe OTA image is valid
  * @retval none
  * @note for AmebaZ, sys_clear_ota_signature is the same with sys_recover_ota_signature
  */
void sys_recover_ota_signature(void);

/**
  * @brief  open log uart
  * @retval none
  */
void sys_log_uart_on(void);

/**
  * @brief  close log uart
  * @retval none
  */
void sys_log_uart_off(void);

/**
  * @brief  store or load adc calibration parameter 
  * @param  write:  this parameter can be one of the following values:
  *		@arg 0: load adc calibration parameter offset & gain from flash system data region
  *		@arg 1: store adc calibration parameter offset & gain to flash system data region
  * @param  offset: pointer to adc parameter offset
  * @param  gain: pointer to adc parameter gain
  * @retval none
  */
void sys_adc_calibration(u8 write, u16 *offset, u16 *gain);

/**
  * @brief  system software reset
  * @retval none
  */
void sys_reset(void);

///@}

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only 
///@{
/**
  * @brief check whether is sdram power on
  * @retval 1: power on
  *         0: power off
  */
u8   sys_is_sdram_power_on(void);

/**
  * @brief sdram power off
  * @retval none
  */
void sys_sdram_off(void);
///@}
#endif //CONFIG_PLATFORM_8195A

#if defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)
///@name AmebaZ Only 
///@{
/**
  * @brief vector reset 
  * @retval none
  */
void sys_cpu_reset(void);
///@}
#endif //CONFIG_PLATFORM_8711B

/**
  * @brief read chip package type
  * @retval 0: efuse not program
            1: RTL8720
            2: RTL8721
            3: RTL8722
            -1: UNKNOWN
  */
int sys_chip_package_read(void);

/**
  * @brief check chip MCM PSRAM existance
  * @retval 0: efuse not program
            1: not exist
            2: exist
            -1: unknown
  */
int sys_chip_psram_check(void);

/**
  * @brief check chip MCM FLASH existance
  * @retval 0: efuse not program
            1: not exist
            2: exist
            -1: unknown
  */
int sys_chip_flash_check(void);

/**
  * @brief check chip band type
  * @retval 0: efuse not program
            1: single band
            2: dual band
            -1: unknown
  */
int sys_chip_band_type_check(void);

enum amebad_chip_model_number {
	CHIP_NUMBER_UNKNOWN = -1,
	CHIP_RTL8720CS = 0,
	CHIP_RTL8720CSM,
	CHIP_RTL8720CSF,
	CHIP_RTL8721CS,
	CHIP_RTL8721CSM,
	CHIP_RTL8721CSF,
	CHIP_RTL8722CS,
	CHIP_RTL8722CSM,
	CHIP_RTL8722CSF,
	CHIP_RTL8720DN,
	CHIP_RTL8720DM,
	CHIP_RTL8720DF,
	CHIP_RTL8721DN,
	CHIP_RTL8721DM,
	CHIP_RTL8721DF,
	CHIP_RTL8722DN,
	CHIP_RTL8722DM,
	CHIP_RTL8722DF,
};

/**
  * @brief get chip model number
  * @retval CHIP_NUMBER_UNKNOWN: efuse not program or incorrect value
  *         other: chip model number
  */
enum amebad_chip_model_number sys_get_chip_model_number(void);

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif
