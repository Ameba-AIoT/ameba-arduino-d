/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_default_transition_time.h
* @brief    Head file for generic default transition time models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_DEFAULT_TRANSITION_TIME_H
#define _GENERIC_DEFAULT_TRANSITION_TIME_H

/* Add Includes here */
#include "mesh_api.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_DEFAULT_TRANSITION_TIME
 * @{
 */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_GET        0x820D
#define MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_SET        0x820E
#define MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK  0x820F
#define MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_STAT       0x8210
/** @} */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_DEFAULT_TRANSITION_TIME_SERVER   0x1004FFFF
#define MESH_MODEL_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT   0x1005FFFF
/** @} */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_GET)];
} _PACKED_ generic_default_transition_time_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_SET)];
    generic_transition_time_t trans_time;
} _PACKED_ generic_default_transition_time_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_DEFAULT_TRANSITION_TIME_STAT)];
    generic_transition_time_t trans_time;
} _PACKED_ generic_default_transition_time_stat_t;
/** @} */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_DEFAULT_TRANSITION_TIME_SERVER_GET    0 //!< @ref generic_default_transition_time_server_set_t
#define GENERIC_DEFAULT_TRANSITION_TIME_SERVER_SET    1 //!< @ref generic_default_transition_time_server_get_t

typedef struct
{
    generic_transition_time_t trans_time;
} generic_default_transition_time_server_set_t;

typedef struct
{
    generic_transition_time_t trans_time;
} generic_default_transition_time_server_get_t;
/** @} */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_STATUS   0 //!< @ref generic_default_transition_time_client_status_t

typedef struct
{
    uint16_t src;
    generic_transition_time_t trans_time;
} generic_default_transition_time_client_status_t;
/** @} */

/**
 * @defgroup GENERIC_DEFAULT_TRANSITION_TIME_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic default transition time server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic default transition server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_default_transition_time_server_reg(uint8_t element_index,
                                                mesh_model_info_p pmodel_info);
/**
 * @brief publish generic default transition time
 * @param[in] pmodel_info: pointer to generic default transition server model context
 * @param[in] trans_time: transition time need to publish
 * @return publish status
 */
mesh_msg_send_cause_t generic_default_transition_time_publish(const mesh_model_info_p pmodel_info,
                                                              generic_transition_time_t trans_time);
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic default transition time client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic default transition client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_default_transition_time_client_reg(uint8_t element_index,
                                                mesh_model_info_p pmodel_info);

/**
 * @brief get generic default tarnsition time
 * @param[in] pmodel_info: pointer to generic default transition client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_default_transition_time_get(const mesh_model_info_p
                                                          pmodel_info,
                                                          uint16_t dst, uint16_t app_key_index);

/**
 * @brief set generic default tarnsition time
 * @param[in] pmodel_info: pointer to generic default transition client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] trans_time: new transition time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_default_transition_time_set(const mesh_model_info_p
                                                          pmodel_info,
                                                          uint16_t dst, uint16_t app_key_index,
                                                          generic_transition_time_t trans_time, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_DEFAULT_TRANSITION_TIME_H */
