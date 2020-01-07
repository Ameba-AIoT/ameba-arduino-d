/**
  ******************************************************************************
  * @file    dct.h
  * @author
  * @version
  * @brief   Device Configuration Table API for storing information in NVRAM(flash).
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */
#ifndef __RTK_DCT_H__
#define __RTK_DCT_H__

#include <platform_stdlib.h>
#include <osdep_service.h>

/**
 * @defgroup dct dct
 * @brief device configuration table API.
 *
 * DCT module have below functions 
 *  - dct init, deinit
 *  - dct module register, unregister, open, close
 *	- dct variable set, get ,delete
 * @{
 */ 

/**
  * @brief  DCT error number.
  */
enum{
	DCT_SUCCESS = 0,			/*!< success */
	DCT_ERROR = -1,				/*!< error */
	DCT_ERR_CRC = -2,			/*!< crc error */
	DCT_ERR_NO_SPACE = -3,		/*!< no space error */
	DCT_ERR_NO_MEMORY = -4,		/*!< alloc memory error */
	DCT_ERR_FLASH_RW = -5,		/*!< flash r/w error */
	DCT_ERR_NOT_FIND = -6,		/*!< not find error */
	DCT_ERR_INVALID = -7,		/*!< invalid operation error */
	DCT_ERR_SIZE_OVER = -8,		/*!< varialbe length over max size error */
	DCT_ERR_MODULE_BUSY = -9,	/*!< module mutex time out */
};

/**
  * @brief  The structure is the module handler for DCT operation.
  */
#define MODULE_NAME_SIZE		32			/*!< max size of the module name */
typedef struct _dct_handle_t{
	uint32_t	module_state;						/*!< the module state */
	uint8_t		module_name[MODULE_NAME_SIZE+1];	/*!< the module name */
	uint16_t	module_idx;							/*!< the module index */
	uint16_t	block_idx;							/*!< the block index */
	uint16_t	used_variable_num;					/*!< the number of variable had used */
	uint32_t	variable_crc;						/*!< the crc of all variables */
	uint8_t		cache_dirty;						/*!< the dirty flag of cache */
	uint8_t		*variable_cache;					/*!< the buffer point of variable cache*/
}dct_handle_t;

/**
 * @brief      Format device configuration table.
 * @param[in]  begin_address : DCT begin address of flash
 * @param[in]  module_number : total module number
 * @param[in]  variable_name_size : size of variable name
 * @param[in]  variable_value_size : size of variable size
 * @param[in]  enable_backup : enable backup function to backup module, it need double module size
 * @param[in]  enable_wear_leveling : enable wear leveling function, it need sextuple module size
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_format(uint32_t begin_address, uint16_t module_number, uint16_t variable_name_size, uint16_t variable_value_size, uint8_t enable_backup, uint8_t enable_wear_leveling);

/**
 * @brief      Initialize device configuration table.
 * @param[in]  begin_address : DCT begin address of flash
 * @param[in]  module_number : total module number
 * @param[in]  variable_name_size : size of variable name
 * @param[in]  variable_value_size : size of variable size
 * @param[in]  enable_backup : enable backup function to backup module, it need double module size
 * @param[in]  enable_wear_leveling : enable wear leveling function, it need sextuple module size
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_init(uint32_t begin_address, uint16_t module_number, uint16_t variable_name_size, uint16_t variable_value_size, uint8_t enable_backup, uint8_t enable_wear_leveling);

/**
 * @brief      Deinitialize device configuration table.
 */
void dct_deinit(void);

/**
 * @brief      Register module in DCT.
 * @param[in]  module_name : module name
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_register_module(char *module_name);

/**
 * @brief      Unregister and delete module in DCT.
 * @param[in]  module_name : module name
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_unregister_module(char *module_name);

/**
 * @brief      Open module in DCT.
 * @param[out] dct_handle : setup module informations in dct handler
 * @param[in]  module_name : module name 
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_open_module(dct_handle_t *dct_handle, char *module_name);

/**
 * @brief      Close module in DCT.
 * @param[in]  dct_handle : dct handler
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_close_module(dct_handle_t *dct_handle);

/**
 * @brief      Write variable name and value in opened module.
 * @param[in]  dct_handle : dct handler
 * @param[in]  variable_name : variable name which you want to store in module
 * @param[in]  variable_value : variable value which you want to store in module
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_set_variable(dct_handle_t *dct_handle, char *variable_name, char *variable_value);

/**
 * @brief      read value of variable name in opened module.
 * @param[in]  dct_handle : dct handler
 * @param[in]  variable_name : variable name which you want to get from module
 * @param[out] buffer : read variable value
 * @param[in]  buffer_size : the buffer size
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_get_variable(dct_handle_t *dct_handle, char *variable_name, char *buffer, uint16_t buffer_size);

/**
 * @brief      delete variable name and value in opened module.
 * @param[in]  dct_handle : dct handler
 * @param[in]  variable_name : variable name which you want to delete in module
 * @return     0  : SUCCESS
 * @return     <0 : ERROR
 */
int32_t dct_delete_variable(dct_handle_t *dct_handle, char *variable_name);

/**
 * @brief      Remaining variable amount in opened module.
 * @param[in]  dct_handle : dct handler
 * @return     integer  : Remaining variable amount
 * @return     <0 : ERROR
 */
int32_t dct_remain_variable(dct_handle_t *dct_handle);

/*\@}*/

#endif // #ifndef __RTK_DCT_H__

