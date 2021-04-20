/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     datatrans_client_app.h
* @brief    Source file for data transmission client model.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-10-30
* @version  v1.0
* *************************************************************************************
*/
#ifndef _DATATRANS_APP_H_
#define _DATATRANS_APP_H_

#include "mesh_api.h"
#include "platform_types.h"

BEGIN_DECLS
/**
 * @addtogroup DATATRANS_CLIENT_APP
 * @{
 */

/**
 * @defgroup Datatrans_Client_Exported_Types Datatrans Client Exported Types
 * @brief
 * @{
 */
extern mesh_model_info_t datatrans;
/** @} */

/**
 * @defgroup Datatrans_Client_Exported_Functions Datatrans Client Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize data transmission client model
 */
void datatrans_model_init(void);
/** @} */
/** @} */

END_DECLS

#endif /* _DATATRANS_CLIENT_APP_H_ */
