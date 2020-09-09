/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     datatrans_server.h
* @brief    Head file for data transmission server
* @details
* @author   hector_huang
* @date     2018-10-31
* @version  v1.0
* *************************************************************************************
*/
#ifndef _DATATRANS_SERVER_H_
#define _DATATRANS_SERVER_H_

#include "platform_types.h"
#include "datatrans_service.h"

BEGIN_DECLS

/** @addtogroup Data_Trans_Server
  * @{
  */

/**
 * @defgroup Data_Trans_Server_Exported_Types Exported Types
 * @{
 */

/** data transmission service id */
extern uint8_t datatrans_server_id;

/** callback data structure */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE type;
    uint16_t len;
    uint8_t *data;
} datatrans_server_data_t;
/** @} */

/**
 * @defgroup Data_Trans_Server_Exported_Functions Exported Functions
 * @{
 */

/**
 * @brief add data transmission server service to gatt database
 * @param[in] pcb: application callback function
 * @return service id
 */
uint8_t datatrans_server_add(void *pcb);

/**
 * @brief notify data change to remote device
 * @param[in] conn_id: connection link id
 * @param[in] pvalue: value to notify
 * @param[in] len: value length
 * @return status
 */
bool datatrans_server_notify(uint8_t conn_id, uint8_t *pvalue, uint16_t len);

/** @} */
/** @} */

END_DECLS

#endif /** DATATRANS_SERVER_H_ */

