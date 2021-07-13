/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     datatrans_service.h
* @brief    Head file for data transmission service
* @details
* @author   hector_huang
* @date     2018-11-05
* @version  v1.0
* *************************************************************************************
*/

#ifndef _DATATRANS_SERVICE_H_
#define _DATATRANS_SERVICE_H_

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup Data_Trans
 * @{
 */

/**
 * @defgroup Data_Trans_Exported_Macros Exported Macros
 * @{
 */
/** characteristic uuid */
#define DATATRANS_DATA_IN_UUID             0xFFE1
#define DATATRANS_DATA_OUT_UUID            0xFFE2
/** @} */

/**
 * @defgroup Data_Trans_Exported_Types Exported Types
 * @{
 */

/** 128bit UUID of data transmission service */
extern const uint8_t GATT_UUID_DATATRANS_SERVICE[16];

/** @} */
/** @} */

END_DECLS

#endif /** _DATATRANS_SERVICE_H_ */

