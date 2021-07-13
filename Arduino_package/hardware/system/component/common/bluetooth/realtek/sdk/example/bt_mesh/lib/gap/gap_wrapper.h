/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gap_wrapper.h
  * @brief    Head file for GAP roles used in mesh.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-10-28
  * @version  v1.0
  * *************************************************************************************
  */

#ifndef _GAP_WRAPPER_H
#define _GAP_WRAPPER_H

/* Add Includes here */
#include "platform_types.h"
#include "gap.h"
#include "gap_conn_le.h"

BEGIN_DECLS

/** @addtogroup Gap_Wrapper
  * @{
  */

/** @defgroup Gap_Wrapper_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief Get the MTU size of the link
  * @param[in] conn_id: the link handle
  * @param[out] pvalue: the pointer of output mtu size
  * @return operation result
  */
#define gap_get_att_mtu_size(conn_id, pvalue)          le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, pvalue, conn_id)

/** @defgroup BT_Addr_Type BT Addr Type
  * @{
  */
#define GAP_BT_ADDR_TYPE_LE_PUBLIC                      GAP_LOCAL_ADDR_LE_PUBLIC
#define GAP_BT_ADDR_TYPE_LE_RANDOM                      GAP_LOCAL_ADDR_LE_RANDOM
/** @} */

/**
  * @brief Get the MTU size of the link
  * @param[in] random_addr: the random addr with the length of 6 Bytes
  * @return operation result
  */
#define gap_set_random_addr(random_addr)                le_set_gap_param(GAP_PARAM_RANDOM_ADDR, 6, random_addr)

/**
  * @brief Set the adv addr type
  * @param[in] addr_type: @ref BT_Addr_Type
  * @return operation result
  */
#define gap_adv_set_addr_type(addr_type)                le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, 1, &addr_type);

/**
  * @brief disconnect the le link
  * @param[in] conn_id: the link handle
  * @return operation result
  */
#define gap_disc(conn_id)                               le_disconnect(conn_id) //GAP_CAUSE_SUCCESS == 

/** @} */
/** @} */

END_DECLS

#endif /* _GAP_WRAPPER_H */
