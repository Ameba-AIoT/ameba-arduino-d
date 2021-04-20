/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     friendship_fn.h
  * @brief    Head file for friendship friend node.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2016-4-29
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _FRIENDSHIP_FN_H
#define _FRIENDSHIP_FN_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup FN
  * @{
  */

/** @defgroup FN_Exported_Types Exported Types
  * @brief
  * @{
  */
typedef struct
{
    uint8_t queue_size;
} fn_params_t;

typedef enum
{
    FN_CB_TYPE_ESTABLISHING,
    FN_CB_TYPE_ESTABLISH_FAIL_NO_POLL,
    FN_CB_TYPE_ESTABLISH_SUCCESS,
    FN_CB_TYPE_FRND_LOST //!< FN_CB_TYPE_FRND_CLEAR, FN_CB_TYPE_CFG_DISABLE
} fn_cb_type_t;

typedef void (*pf_fn_cb_t)(uint8_t frnd_index, fn_cb_type_t type, uint16_t lpn_addr);
/** @} */

/** @defgroup FN_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief initialize the friend node
  * @param[in] lpn_num: the maxium supported lpn number
  * @param[in] fn_params_t: fn parameters
  * @param[in] pf_fn_cb: fn friendship info callback
  * @return operation result
  * @retval true: operation success
  * @retval false: operation failure
  */
bool fn_init(uint8_t lpn_num, fn_params_t *pfn_params, pf_fn_cb_t pf_fn_cb);

/**
  * @brief deinitialize the friend node to release resources
  * @return none
  */
void fn_deinit(void);

/**
  * @brief clear all the existing lpn
  * @return none
  */
void fn_clear(void);

/** @} */
/** @} */

END_DECLS

#endif /* _FRIENDSHIP_FN_H */
