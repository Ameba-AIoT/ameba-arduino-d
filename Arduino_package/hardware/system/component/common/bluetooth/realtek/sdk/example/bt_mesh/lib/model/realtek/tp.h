/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     tp.h
* @brief    Head file for tp models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-3-7
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _TP_H
#define _TP_H

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup TP
 * @{
 */

/**
 * @defgroup TP_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_TP_MSG                                 0xC85D00
/** @} */

/**
 * @defgroup TP_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_TP_CONTROL                           0x0003005D
/** @} */

/**
 * @defgroup TP_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TP_MSG)];
    uint32_t tid;
    uint8_t padding[ACCESS_PAYLOAD_MAX_SIZE - sizeof(uint32_t) - ACCESS_OPCODE_SIZE(MESH_MSG_TP_MSG)];
} _PACKED_ tp_msg_t;
/** @} */

/**
 * @defgroup TP_CONTROL_API Control API
 * @brief Functions declaration
 * @{
 */
void tp_control_reg(model_data_cb_pf data_cb);
mesh_msg_send_cause_t tp_msg(uint16_t dst, uint8_t ttl, uint16_t app_key_index, uint8_t *pdata,
                             uint16_t data_len);
mesh_msg_send_cause_t tp_start(uint16_t dst, uint8_t ttl, uint16_t app_key_index,
                               uint32_t count);
/** @} */
/** @} */

END_DECLS

#endif /* _TP_H */
