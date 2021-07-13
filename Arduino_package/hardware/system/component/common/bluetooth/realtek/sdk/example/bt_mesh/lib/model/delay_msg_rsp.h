/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     delay_msg_rsp.h
* @brief    Head file for delay message response
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2019-05-20
* @version  v1.0
* *************************************************************************************
*/
#ifndef _DELAY_MSG_RSP_H_
#define _DELAY_MSG_RSP_H_

#include "platform_types.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @brief get message response delay time
 * @param[in] dst_addr: destination address
 * @return delay time, unit is ms
 */
uint32_t delay_msg_get_rsp_delay(uint16_t dst_addr);

/**
 * @brief get transition delay time
 * @param[in] delay_time: delay execution time
 * @param[in] trans_time: generic transition time
 * @param[in] delay_rsp_time: message response delay time
 * @param[in] send_immediately: whether message shall send immediately
 * @param[in] ack: whether message is acknowledge message
 * @return transition delay time, unit is ms
 */
uint32_t delay_msg_get_trans_delay(uint32_t delay_time, generic_transition_time_t trans_time,
                                   uint32_t delay_rsp_time, bool send_immediately, bool ack);

END_DECLS

#endif /* _DELAY_MSG_RSP_H_ */

