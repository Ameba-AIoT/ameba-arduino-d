/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     vendor_tp_client.h
  * @brief
  * @details  Simple BLE data structs and external functions declaration.
  * @author   jane
  * @date     2016-02-18
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _VD_PXP_CLIENT_H_
#define _VD_PXP_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_client.h"

typedef enum
{
    PXP_CLIENT_CB_TYPE_DISC_RESULT,          //!< Discovery procedure state, done or pending.
    PXP_CLIENT_CB_TYPE_WRITE_RESULT,         //!< Read request's result data, responsed from server.
    PXP_CLIENT_CB_TYPE_READ_RESULT,
    PXP_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_PXP_CB_TYPE;

typedef struct
{
    bool is_found;
    T_GATT_CHARACT_ELEM128 pxp_char;
} T_PXP_DISC_RESULT;

typedef struct
{
    uint16_t cause;
    uint16_t value_size;
    uint8_t *pValue;
} T_PXP_READ_RESULT;

typedef struct
{
    uint16_t cause;
} T_PXP_WRITE_RESULT;

typedef union
{
    T_PXP_DISC_RESULT disc_result;
    T_PXP_WRITE_RESULT write_result;
    T_PXP_READ_RESULT read_result;
} T_PXP_DATA;

typedef struct
{
    T_PXP_CB_TYPE     cb_type;
    T_PXP_DATA        cb_content;
} T_PXP_CB_DATA;


T_CLIENT_ID vendor_pxpext_client_add(P_FUN_GENERAL_APP_CB app_cb);
bool vendor_pxpext_start_discovery(uint8_t conn_id);
bool vendor_pxpext_write_value(uint8_t conn_id, uint16_t length, uint8_t *p_value);
bool vendor_pxpext_read_value(uint8_t conn_id);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _VD_PXP_CLIENT_H_ */
