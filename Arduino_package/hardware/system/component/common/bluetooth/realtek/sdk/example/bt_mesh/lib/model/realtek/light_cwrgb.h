/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_cwrgb.h
* @brief    Head file for light cwrgb(cold/warn/red/green/blue) module.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-1-3
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _LIGHT_CWRGB_H
#define _LIGHT_CWRGB_H

/* Add Includes here */
#include "platform_misc.h"
#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CWRGB
 * @{
 */

/**
 * @defgroup LIGHT_CWRGB_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_CWRGB_GET                        0xC45D00
#define MESH_MSG_LIGHT_CWRGB_SET                        0xC55D00
#define MESH_MSG_LIGHT_CWRGB_SET_UNACK                  0xC65D00
#define MESH_MSG_LIGHT_CWRGB_STAT                       0xC75D00
/** @} */

/**
 * @defgroup LIGHT_CWRGB_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_CWRGB_SERVER                   0x0001005D
#define MESH_MODEL_LIGHT_CWRGB_CLIENT                   0x0002005D
/** @} */

/**
 * @defgroup LIGHT_CWRGB_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CWRGB_GET)];
} _PACKED_ light_cwrgb_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CWRGB_SET)];
    uint8_t cwrgb[5]; //!< up to 8 bytes to fit into only one adv packet
} _PACKED_ light_cwrgb_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CWRGB_STAT)];
    uint8_t cwrgb[5]; //!< up to 8 bytes to fit into only one adv packet
} _PACKED_ light_cwrgb_stat_t;
/** @} */

/**
 * @defgroup LIGHT_CWRGB_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
void light_cwrgb_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info,
                            model_receive_pf pf_model_receive);
mesh_msg_send_cause_t light_cwrgb_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                       uint16_t app_key_index, uint8_t cwrgb[5]);
/** @} */

/**
 * @defgroup LIGHT_CWRGB_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
bool light_cwrgb_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);
mesh_msg_send_cause_t light_cwrgb_get(mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index);
mesh_msg_send_cause_t light_cwrgb_set(mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index, uint8_t cwrgb[5], bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CWRGB_H */
