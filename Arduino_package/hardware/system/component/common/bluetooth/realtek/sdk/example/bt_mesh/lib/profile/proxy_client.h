/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     proxy_client.h
  * @brief    Head file for using proxy service client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2016-4-1
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROXY_CLIENT_H_
#define _PROXY_CLIENT_H_

/* Add Includes here */
#include "mesh_config.h"
#include "profile_client.h"
#include "proxy_service.h"

BEGIN_DECLS

/** @addtogroup Proxy_Client
  * @{
  */

/** @defgroup Proxy_Client_Exported_Types Exported Types
  * @{
  */

/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_PROXY_SRV_START,
    HDL_PROXY_SRV_END,
    HDL_PROXY_DATA_IN,
    HDL_PROXY_DATA_OUT,
    HDL_PROXY_DATA_OUT_END,
    HDL_PROXY_DATA_OUT_CCCD,
    HDL_PROXY_CACHE_LEN
} proxy_handle_type_t;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    PROXY_DISC_DONE,
    PROXY_DISC_FAIL,
    PROXY_DISC_NOT_FOUND
} proxy_disc_state_t;

/** @brief handle information found in discovery procedure */
typedef struct
{
    proxy_handle_type_t handle_type;
    uint16_t handle_value;
} proxy_disc_hdl_info_t;

/** @brief read request type, used by app to send read request */
typedef enum
{
    PROXY_READ_DATA_OUT_CCCD
} proxy_read_type_t;

/** @brief read content, used to inform app read response data content */
typedef union
{
    bool proxy_data_out_cccd;
} proxy_read_data_t;

/** @brief read data, used to inform app read response data */
typedef struct
{
    proxy_read_type_t type;
    proxy_read_data_t data;
    uint16_t cause;
} proxy_read_result_t;

/** @brief write request result
  */
typedef enum
{
    PROXY_WRITE_DATA_IN
} proxy_write_type_t;

/** @brief write request result */
typedef struct
{
    proxy_write_type_t type;
    uint16_t cause;
} proxy_write_result_t;

/** @brief notif/ind receive type */
typedef enum
{
    PROXY_DATA_OUT_NOTIFY
} proxy_notify_indicate_type_t;

/** @brief notif/ind receive content */
typedef union
{
    uint8_t proxy_data_out[31];
} proxy_notify_indicate_value_t;

/** @brief The notif/ind data received by client */
typedef struct
{
    proxy_notify_indicate_type_t type;
    proxy_notify_indicate_value_t value;
} proxy_notify_indicate_data_t;

/** @brief Event type to inform app */
typedef enum
{
    PROXY_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    PROXY_CLIENT_CB_TYPE_DISC_HDL,            //!< Discovered handle information.
    PROXY_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    PROXY_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    PROXY_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    PROXY_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} proxy_client_cb_type_t;

/** @brief Callback content sent to application */
typedef union
{
    proxy_disc_state_t disc_state;
    proxy_disc_hdl_info_t disc_hdl_info;
    proxy_read_result_t read_result;
    proxy_write_result_t write_result;
    proxy_notify_indicate_data_t notify_indicate_data;
} proxy_client_cb_content_t;

/** @brief Callback data sent to application from client */
typedef struct
{
    proxy_client_cb_type_t cb_type;
    proxy_client_cb_content_t cb_content;
} proxy_client_cb_data_t;

extern T_CLIENT_ID proxy_client_id;
/** @} */

/** @defgroup Proxy_Client_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief register the client
  * @param[in] app_cb: the callback function
  * @return client id
  */
T_CLIENT_ID proxy_client_add(P_FUN_GENERAL_APP_CB appCB);

/**
  * @brief set the handle of the client
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @param[in] value: the handle value
  * @return the operatino result
  */
bool proxy_client_handle_set(uint8_t conn_id, proxy_handle_type_t type, uint16_t value);

/**
  * @brief get the handle of the client
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @return the handle value
  */
uint16_t proxy_client_handle_get(uint8_t conn_id, proxy_handle_type_t handle_type);

/**
  * @brief start the service discovery
  *
  * contains the service/characteristic/cccd declaration
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
bool proxy_client_start_discovery(uint8_t conn_id);

/**
  * @brief read the server by handle
  * @param[in] conn_id: the connection id
  * @param[in] type: the read type
  * @return the operatino result
  */
bool proxy_client_read_by_handle(uint8_t conn_id, proxy_read_type_t readCharType);

/**
  * @brief read the server by uuid
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @return the operatino result
  */
bool proxy_client_read_by_uuid(uint8_t conn_id, proxy_read_type_t readCharType);

/**
  * @brief write the cccd of the characteristic
  * @param[in] conn_id: the connection id
  * @param[in] command: the cccd value
  * @return the operatino result
  */
bool proxy_client_data_out_cccd_set(uint8_t conn_id, bool command);

/**
  * @brief write the characteristic
  * @param[in] conn_id: the connection id
  * @param[in] pdata: the data pointer
  * @param[in] length: the data length
  * @return the operatino result
  */
bool proxy_client_data_in_write(uint8_t conn_id, uint8_t *pdata, uint16_t length);

/** @} */
/** @} */

END_DECLS

#endif  /* _PROXY_CLIENT_H_ */
