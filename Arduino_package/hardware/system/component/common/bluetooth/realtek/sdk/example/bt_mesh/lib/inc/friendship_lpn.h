/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     friendship_lpn.h
  * @brief    Head file for lpn.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2016-4-29
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _FRIENDSHIP_LPN_H
#define _FRIENDSHIP_LPN_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup LPN LPN
  * @{
  */

/** @defgroup LPN_Exported_Types Exported Types
  * @{
  */
/* lpn node offer choose algorithm function */
typedef bool (*pf_lpn_offer_choose_t)(uint16_t frnd_addr, uint8_t frnd_index,
                                      frnd_offer_t *pfrnd_offer, int8_t rssi);
typedef struct
{
    uint32_t poll_interval; //!< in units of 100 milliseconds, little than poll_timeout
    uint32_t poll_timeout; //!< in units of 100 milliseconds, range 0x00000A - C0x34BBFF
    frnd_criteria_t criteria;
    pf_lpn_offer_choose_t
    offer_choose; /* user can define offer choose algorithm or set to NULL to use stack's default offer choose algorithm */
} lpn_req_params_t;

typedef enum
{
    LPN_REQ_REASON_SUCCESS,
    LPN_REQ_REASON_REQ_PARAMS_INVALID,
    LPN_REQ_REASON_NET_KEY_INDEX_INVALID,
    LPN_REQ_REASON_FRND_ON_SAME_NET_KEY_INDEX,
    LPN_REQ_REASON_FRND_INDEX_INVALID,
    LPN_REQ_REASON_FRND_ALREADY_EXIST_OR_ESTABLISING,
    LPN_REQ_REASON_RESOURCE_INSUFFICIENT
} lpn_req_reason_t;

typedef enum
{
    LPN_CB_TYPE_ESTABLISH_SUCCESS,
    LPN_CB_TYPE_ESTABLISH_FAIL_NO_OFFER,
    LPN_CB_TYPE_ESTABLISH_FAIL_NO_UPDATE,
    LPN_CB_TYPE_FRIENDSHIP_LOST //!< LPN_CB_TYPE_FRIENDSHIP_CLEAR
} lpn_cb_type_t;

typedef void (*pf_lpn_cb_t)(uint8_t frnd_index, lpn_cb_type_t type, uint16_t fn_addr);
/** @} */

/** @defgroup LPN_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief initialize the lpn
  *
  * A lpn may establish multiple friendships on different NetKeys.
  * @param[in] fn_num: the maxium supported fn number
  * @param[in] pf_lpn_cb: lpn friendship info callback
  * @return operation result
  * @retval true: operation success
  * @retval false: operation failure
  */
bool lpn_init(uint8_t fn_num, pf_lpn_cb_t pf_lpn_cb);

/**
  * @brief deinitialize the lpn to release resources
  * @return none
  */
void lpn_deinit(void);

/**
  * @brief start to establish the friendship on the desired Netkey
  *
  * The app shall manage the fn index. At most one friendship can be established on
  * a NetKey by the lpn.
  *
  * @param[in] frnd_index: the fn index
  * @param[in] net_key_index: the net key index to establish
  * @param[in] p_req_params: the friendship requirements
  * @return operation result
  */
lpn_req_reason_t lpn_req(uint8_t frnd_index, uint16_t net_key_index,
                         lpn_req_params_t *p_req_params);

/**
  * @brief clear the friendship
  * @param[in] fn_index: the fn index
  * @return operation result
  * @retval true: operation success when the friendship exists
  * @retval false: operation failure
  */
bool lpn_clear(uint8_t fn_index);

/**
 * @brief get friend offer information
 * @param[in] fn_index: the fn index
 * @return friend offer information
 */
frnd_offer_t lpn_get_frnd_offer(uint8_t fn_index);

/**
 * @brief set friend offer information
 * @param[in] fn_index: the fn index
 * @param[in] friend offer information
 * @return TRUE: local set friend offer success
 *         FALSE: local set friend offer failed
 */
bool lpn_set_frnd_offer(uint8_t fn_index, uint16_t net_key_index, uint16_t fn_addr,
                        frnd_offer_t frnd_offer);

/**
 * @brief get lpn counter
 * @param[in] fn_index: the fn index
 * @return lpn counter
 */
uint16_t lpn_get_counter(uint8_t fn_index);

/**
 * @brief set lpn counter
 * @param[in] fn_index: the fn index
 * @param[in] lpn_counter: lpn counter
 * @return TRUE: local set lpn counter success
 *         FALSE: local set lpn counter failed
 */
bool lpn_set_counter(uint8_t fn_index, uint16_t lpn_counter);

/**
 * @brief start friend poll
 * @param[in] poll_interval: friend poll interval, in unit of 100 milliseconds
 */
bool lpn_poll(uint8_t fn_index, uint32_t poll_interval);

/**
  * @brief add or remove the subscribe list when the friendship exists
  * @param[in] fn_index: the fn index
  * @param[in] addr: the subscribe address list
  * @param[in] addr_num: the number of addresses in the list
  * @param[in] add_rm: add or remove flag
  * @return none
  */
void frnd_sub_list_add_rm(uint8_t fn_index, uint16_t addr[], uint8_t addr_num, bool add_rm);

/** @} */
/** @} */

END_DECLS

#endif /* _FRIENDSHIP_LPN_H */
