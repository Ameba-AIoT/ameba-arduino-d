/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_level.h
* @brief    Head file for generic level models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_LEVEL_H
#define _GENERIC_LEVEL_H

#include "mesh_api.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_LEVEL
 * @{
 */

/**
 * @defgroup GENERIC_LEVEL_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_LEVEL_GET                      0x8205
#define MESH_MSG_GENERIC_LEVEL_SET                      0x8206
#define MESH_MSG_GENERIC_LEVEL_SET_UNACK                0x8207
#define MESH_MSG_GENERIC_LEVEL_STAT                     0x8208
#define MESH_MSG_GENERIC_DELTA_SET                      0x8209
#define MESH_MSG_GENERIC_DELTA_SET_UNACK                0x820A
#define MESH_MSG_GENERIC_MOVE_SET                       0x820B
#define MESH_MSG_GENERIC_MOVE_SET_UNACK                 0x820C
/** @} */

/**
 * @defgroup GENERIC_LEVEL_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_LEVEL_SERVER                 0x1002FFFF
#define MESH_MODEL_GENERIC_LEVEL_CLIENT                 0x1003FFFF
/** @} */

/**
 * @defgroup GENERIC_LEVEL_MESH_MSG Mesh Msg
 * @brief Mesh mesage types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LEVEL_GET)];
} _PACKED_ generic_level_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LEVEL_SET)];
    int16_t level;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ generic_level_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LEVEL_STAT)];
    int16_t present_level;
    int16_t target_level; //!< optional
    generic_transition_time_t remaining_time;
} _PACKED_ generic_level_stat_t, *generic_level_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_DELTA_SET)];
    int32_t delta_level;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ generic_delta_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MOVE_SET)];
    int16_t delta_level;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ generic_move_set_t;
/** @} */

/**
 * @defgroup GENERIC_LEVEL_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LEVEL           0
#define DELAY_EXECUTION_TYPE_LEVEL              0
#define DELAY_EXECUTION_TYPE_LEVEL_MOVE         1
/** @} */

/**
 * @defgroup GENERIC_LEVEL_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_LEVEL_SERVER_GET                                0 //!< @ref generic_level_server_get_t
#define GENERIC_LEVEL_SERVER_GET_DEFAULT_TRANSITION_TIME        1 //!< @ref generic_level_server_get_default_transition_time_t
#define GENERIC_LEVEL_SERVER_SET                                2 //!< @ref generic_level_server_set_t
#define GENERIC_LEVEL_SERVER_SET_MOVE                           3 //!< @ref generic_level_server_set_move_t

typedef struct
{
    int16_t level;
} generic_level_server_get_t;

typedef struct
{
    generic_transition_time_t trans_time;
} generic_level_server_get_default_transition_time_t;

typedef struct
{
    int16_t level;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} generic_level_server_set_t;

typedef struct
{
    int16_t move_delta;
    int16_t target_level;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} generic_level_server_set_move_t;
/** @} */

/**
 * @defgroup GENERIC_LEVEL_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_LEVEL_CLIENT_STATUS   0 //!< @ref generic_level_client_status_t

typedef struct
{
    uint16_t src;
    int16_t present_level;
    bool optional;
    int16_t target_level;
    generic_transition_time_t remaining_time;
} generic_level_client_status_t;
/** @} */

/**
 * @defgroup GENERIC_LEVEL_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic level server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic level server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_level_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic level status
 * @param[in] pmodel_info: pointer to generic level server model context
 * @param[in] level: level status need to publish
 * @return publish status
 */
mesh_msg_send_cause_t generic_level_publish(const mesh_model_info_p pmodel_info, int16_t level);

/** @} */

/** @defgroup GENERIC_LEVEL_CLIENT_API Client API
  * @brief Functions declaration
  * @{
  */

/**
 * @brief register generic level client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic level client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_level_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic level status
 * @param[in] pmodel_info: pointer to generic level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_level_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index);

/**
 * @brief set generic level
 * @param[in] pmodel_info: pointer to generic level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] level: desired level value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_level_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index,
                                        int16_t level, uint8_t tid, bool optional,
                                        generic_transition_time_t trans_time, uint8_t delay, bool ack);
/**
 * @brief set generic level delta parameter
 * @param[in] pmodel_info: pointer to generic level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] delta_level: desired delta level value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_delta_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index,
                                        int32_t delta_level, uint8_t tid, bool optional,
                                        generic_transition_time_t trans_time, uint8_t delay, bool ack);
/**
 * @brief set generic level move parameter
 * @param[in] pmodel_info: pointer to generic default on off client context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] delta_level: desired delta level value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_move_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                       uint16_t app_key_index,
                                       int16_t delta_level, uint8_t tid, bool optional,
                                       generic_transition_time_t trans_time, uint8_t delay, bool ack);

/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_LEVEL_H */
