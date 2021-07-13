/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     ping.h
* @brief    Head file for ping models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2016-3-24
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _PING_H
#define _PING_H

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup PING
 * @{
 */

/**
 * @defgroup PING_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_PING                                   0xC05D00
#define MESH_MSG_PONG                                   0xC15D00
#define MESH_MSG_BIG_PING                               0xC25D00
#define MESH_MSG_BIG_PONG                               0xC35D00
/** @} */

/**
 * @defgroup PING_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_PING_CONTROL                         0x0000005D
/** @} */

/**
 * @defgroup PING_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PING)];
    uint8_t init_ttl; //!< Initial TTL value used when sending this message.
    uint16_t pong_max_delay; //!< unit: 10ms
} _PACKED_ ping_t, *ping_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PONG)];
    uint8_t init_ttl; //!< Initial TTL value used when sending this message.
    uint8_t hops_forward; //!< Hops in the forward direction.
    uint16_t pong_delay; //!< unit: 10ms
} _PACKED_ pong_t, *pong_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BIG_PING)];
    uint8_t init_ttl;
    uint16_t pong_max_delay; //!< unit: 10ms
    uint8_t padding[ACCESS_PAYLOAD_MAX_SIZE - 3 - ACCESS_OPCODE_SIZE(MESH_MSG_BIG_PING)];
} _PACKED_ big_ping_t, *big_ping_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BIG_PONG)];
    uint8_t init_ttl;
    uint8_t hops_forward;
    uint16_t pong_delay; //!< unit: 10ms
    uint8_t padding[ACCESS_PAYLOAD_MAX_SIZE - 4 - ACCESS_OPCODE_SIZE(MESH_MSG_BIG_PONG)];
} _PACKED_ big_pong_t, *big_pong_p;
/** @} */

/**
 * @defgroup PING_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
void ping_control_reg(pf_ping_cb_t pf_ping_cb, pf_pong_cb_t pf_pong_cb);
mesh_msg_send_cause_t ping(uint16_t dst, uint8_t ttl, uint16_t app_key_index,
                           uint16_t pong_max_delay);
mesh_msg_send_cause_t pong(uint16_t dst, uint8_t ttl, uint16_t app_key_index, uint8_t hops_forward,
                           uint16_t pong_delay);
mesh_msg_send_cause_t big_ping(uint16_t dst, uint8_t ttl, uint16_t app_key_index,
                               uint16_t pong_max_delay);
mesh_msg_send_cause_t big_pong(uint16_t dst, uint8_t ttl, uint16_t app_key_index,
                               uint8_t hops_forward, uint16_t pong_delay);
/** @} */
/** @} */

END_DECLS

#endif /* _PING_H */
