/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     provision_client.h
  * @brief    Head file for using provision service Client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2015-11-09
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROV_CLIENT_H_
#define _PROV_CLIENT_H_

/* Add Includes here */
#include "mesh_config.h"
#include "profile_client.h"
#include "provision_service.h"

BEGIN_DECLS

/** @addtogroup Prov_Client
  * @{
  */

/** @defgroup Prov_Client_Exported_Types Exported Types
  * @brief  types that other.c files may use all defined here
  * @{
  */

/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_PROV_SRV_START,           // start handle of simple ble service
    HDL_PROV_SRV_END,             // end handle of simple ble service
    HDL_PROV_DATA_IN,
    HDL_PROV_DATA_OUT,
    HDL_PROV_DATA_OUT_END,
    HDL_PROV_DATA_OUT_CCCD,
    HDL_PROV_CACHE_LEN
} prov_handle_type_t;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    PROV_DISC_DONE,
    PROV_DISC_FAIL,
    PROV_DISC_NOT_FOUND
} prov_disc_state_t;

/** @brief handle information found in discovery procedure */
typedef struct
{
    prov_handle_type_t handle_type;
    uint16_t handle_value;
} prov_disc_hdl_info_t;

/** @brief read request type, used by app to send read request */
typedef enum
{
    PROV_READ_DATA_OUT_CCCD
} prov_read_type_t;

/** @brief read content, used to inform app read response data content */
typedef union
{
    bool prov_data_out_cccd;
} prov_read_data_t;

/** @brief read data, used to inform app read response data */
typedef struct
{
    prov_read_type_t type;
    prov_read_data_t data;
    uint16_t cause;
} prov_read_result_t;

/** @brief write request result */
typedef enum
{
    PROV_WRITE_DATA_IN
} prov_write_type_t;

/** @brief write request result */
typedef struct
{
    prov_write_type_t type;
    uint16_t cause;
} prov_write_result_t;

/** @brief notif/ind receive type */
typedef enum
{
    PROV_DATA_OUT_NOTIFY
} prov_notify_indicate_type_t;

/** @brief notif/ind receive content */
typedef union
{
    uint8_t prov_data_out[65];
} _PACKED_ prov_notify_indicate_value_t;

/** @brief The notif/ind data received by client */
typedef struct
{
    prov_notify_indicate_type_t type;
    prov_notify_indicate_value_t value;
} prov_notify_indicate_data_t;

/** @brief Event type to inform app */
typedef enum
{
    PROV_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    PROV_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    PROV_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    PROV_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    PROV_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} prov_client_cb_type_t;

/** @brief Callback content sent to application */
typedef union
{
    prov_disc_state_t disc_state;
    prov_disc_hdl_info_t disc_hdl_info;
    prov_read_result_t read_result;
    prov_write_result_t write_result;
    prov_notify_indicate_data_t notify_indicate_data;
} prov_client_cb_content_t;

/** @brief data sent to application from client, include type and content */
typedef struct
{
    prov_client_cb_type_t cb_type;
    prov_client_cb_content_t cb_content;
} prov_client_cb_data_t;

extern uint8_t prov_client_conn_id;
extern T_CLIENT_ID prov_client_id;
/** @} */

/** @defgroup Prov_Client_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief register the client
  * @param[in] app_cb: the callback function
  * @return client id
  */
T_CLIENT_ID prov_client_add(P_FUN_GENERAL_APP_CB app_cb);

/**
  * @brief set the handle of the client
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @param[in] value: the handle value
  * @return the operatino result
  */
bool prov_client_handle_set(uint8_t conn_id, prov_handle_type_t type, uint16_t value);

/**
  * @brief get the handle of the client
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @return the handle value
  */
uint16_t prov_client_handle_get(uint8_t conn_id, prov_handle_type_t type);

/**
  * @brief start the service discovery
  *
  * contains the service/characteristic/cccd declaration
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
bool prov_client_start_discovery(uint8_t conn_id);

/**
  * @brief read the server by handle
  * @param[in] conn_id: the connection id
  * @param[in] type: the read type
  * @return the operatino result
  */
bool prov_client_read_by_handle(uint8_t conn_id, prov_read_type_t type);

/**
  * @brief read the server by uuid
  * @param[in] conn_id: the connection id
  * @param[in] type: the handle type
  * @return the operatino result
  */
bool prov_client_read_by_uuid(uint8_t conn_id, prov_read_type_t type);

/**
  * @brief write the cccd of the characteristic
  * @param[in] conn_id: the connection id
  * @param[in] command: the cccd value
  * @return the operatino result
  */
bool prov_client_data_out_cccd_set(uint8_t conn_id, bool command);

/**
  * @brief write the characteristic
  * @param[in] conn_id: the connection id
  * @param[in] pdata: the data pointer
  * @param[in] length: the data length
  * @return the operatino result
  */
bool prov_client_data_in_write(uint8_t conn_id, uint8_t *pdata, uint16_t length);

/** @} */
/** @} */

END_DECLS

#endif  /* _PROV_CLIENT_H_ */
