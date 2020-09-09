/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     datatrans_client.h
* @brief    Head file for data transmission client
* @details
* @author   hector_huang
* @date     2018-10-31
* @version  v1.0
* *************************************************************************************
*/

#ifndef _DATATRANS_CLIENT_H_
#define _DATATRANS_CLIENT_H_

#include "platform_types.h"
#include "profile_client.h"

BEGIN_DECLS

/** @addtogroup Data_Trans_Client
  * @{
  */

/**
 * @defgroup Data_Trans_Client_Exported_Macros Exported Macros
 * @{
 */
#define DATATRANS_CLIENT_MAX_DATA_OUT_LEN       32
/** @} */

/**
 * @defgroup Data_Trans_Client_Exported_types Exported Types
 * @{
 */
/**  datatrans client ID. */
extern T_CLIENT_ID datatrans_client_id;

typedef enum
{
    HDL_DATATRANS_SRV_START,
    HDL_DATATRANS_SRV_END,
    HDL_DATATRANS_DATA_IN,
    HDL_DATATRANS_DATA_OUT,
    HDL_DATATRANS_DATA_OUT_END,
    HDL_DATATRANS_DATA_OUT_CCCD,
    HDL_DATATRANS_CACHE_LEN
} datatrans_handle_type_t;

typedef enum
{
    DATATRANS_DISC_DONE,
    DATATRANS_DISC_FAIL,
    DATATRANS_DISC_NOT_FOUND
} datatrans_disc_state_t;

typedef struct
{
    datatrans_handle_type_t handle_type;
    uint16_t handle_value;
} datatrans_disc_hdl_info_t;

typedef enum
{
    DATATRANS_READ_DATA_OUT_CCCD,
    DATATRANS_READ_DATA_OUT
} datatrans_read_type_t;

typedef struct
{
    uint16_t len;
    uint8_t datatrans_data_out[DATATRANS_CLIENT_MAX_DATA_OUT_LEN];
} _PACKED_ datatrans_data_out_value_t;

typedef union
{
    bool datatrans_data_out_cccd;
    datatrans_data_out_value_t value;
} datatrans_read_data_t;

typedef struct
{
    datatrans_read_type_t type;
    datatrans_read_data_t data;
    uint16_t cause;
} datatrans_read_result_t;

typedef enum
{
    DATATRANS_WRITE_DATA_IN
} datatrans_write_type_t;

typedef struct
{
    datatrans_write_type_t type;
    uint16_t cause;
} datatrans_write_result_t;

typedef enum
{
    DATATRANS_DATA_OUT_NOTIFY
} datatrans_notify_indicate_type_t;

typedef struct
{
    datatrans_notify_indicate_type_t type;
    datatrans_data_out_value_t value;
} datatrans_notify_indicate_data_t;

typedef enum
{
    DATATRANS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    DATATRANS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    DATATRANS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    DATATRANS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    DATATRANS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} datatrans_client_cb_type_t;

typedef union
{
    datatrans_disc_state_t disc_state;
    datatrans_disc_hdl_info_t disc_hdl_info;
    datatrans_read_result_t read_result;
    datatrans_write_result_t write_result;
    datatrans_notify_indicate_data_t notify_indicate_data;
} datatrans_client_cb_content_t;

typedef struct
{
    datatrans_client_cb_type_t cb_type;
    datatrans_client_cb_content_t cb_content;
} datatrans_client_cb_data_t;
/** @} */

/**
 * @defgroup Data_Trans_Client_Exported_Functions Exported Functions
 * @{
 */

/**
 * @brief add data transmission client to gatt stack
 * @param[in] app_cb: application callback function
 * @return service id
 */
T_CLIENT_ID datatrans_client_add(P_FUN_GENERAL_APP_CB app_cb);

/**
 * @brief start discovery data transmission service
 * @param[in] conn_id: connection link id
 * @retval TRUE: start discovery success
 * @retval FALSE: start discovery fail
 */
bool datatrans_client_start_discovery(uint8_t conn_id);

/**
 * @brief read data from server by using handles.
 * @param[in] conn_id: connection link id
 * @param[in] type: one of characteristic that has the readable property.
 * @retval TRUE: send request to upper stack success.
 * @retval FALSE: send request to upper stack failed.
 */
bool datatrans_client_read_by_handle(uint8_t conn_id, datatrans_read_type_t type);

/**
 * @brief read data from server by using uuids.
 * @param[in] conn_id: connection link id
 * @param[in] type: one of characteristic that has the readable property.
 * @retval TRUE: send request to upper stack success.
 *         FALSE: send request to upper stack failed.
 */
bool datatrans_client_read_by_uuid(uint8_t conn_id, datatrans_read_type_t type);

/**
 * @brief enable or disable the notification of peer server's notify characteristic.
 * @param[in] conn_id: connection link id
 * @param[in] command: 0-disable the notification, 1-enable the notification.
 * @retval TRUE: send request to upper stack success.
 * @retval FALSE: send request to upper stack failed.
 */
bool datatrans_client_data_out_cccd_set(uint8_t conn_id, bool command);

/**
 * @brief send write request to peer server's control point characteristic.
 * @param[in] conn_id: connection link id
 * @param[in] pdata: data to write
 * @param[in] length: data length
 * @retval TRUE: send request to upper stack success.
 * @retval FALSE: send request to upper stack failed.
 */
bool datatrans_client_data_in_write(uint8_t conn_id, uint8_t *pdata, uint16_t length);

/**
 * @brief  used by application, to set the handles in datatrans handle cache.
 * @param[in] type: handle types of this specific profile.
 * @param[in] value: handle value to set.
 * @retval TRUE: success.
 * @retval FALSE: failed.
 */
bool datatrans_client_handle_set(datatrans_handle_type_t type, uint16_t value);

/**
 * @brief  used by application, to get the handles in datatrans handle cache.
 * @param[in] type: handle types of this specific profile.
 * @retval handle value.
 */
uint16_t datatrans_client_handle_get(datatrans_handle_type_t type);

/** @} */
/** @} */

END_DECLS

#endif /** _DATATRANS_CLIENT_H_ */

