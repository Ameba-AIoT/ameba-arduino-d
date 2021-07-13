/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_power_level.h
* @brief    Head file for generic power level models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_POWER_LEVEL_H
#define _GENERIC_POWER_LEVEL_H

#include "mesh_api.h"
#include "generic_transition_time.h"
#include "generic_types.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_POWER_LEVEL
 * @{
 */

/**
 * @defgroup GENERIC_POWER_LEVEL_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_POWER_LEVEL_GET                0x8215
#define MESH_MSG_GENERIC_POWER_LEVEL_SET                0x8216
#define MESH_MSG_GENERIC_POWER_LEVEL_SET_UNACK          0x8217
#define MESH_MSG_GENERIC_POWER_LEVEL_STAT               0x8218
#define MESH_MSG_GENERIC_POWER_LAST_GET                 0x8219
#define MESH_MSG_GENERIC_POWER_LAST_STAT                0x821A
#define MESH_MSG_GENERIC_POWER_DEFAULT_GET              0x821B
#define MESH_MSG_GENERIC_POWER_DEFAULT_STAT             0x821C
#define MESH_MSG_GENERIC_POWER_RANGE_GET                0x821D
#define MESH_MSG_GENERIC_POWER_RANGE_STAT               0x821E
#define MESH_MSG_GENERIC_POWER_DEFAULT_SET              0x821F
#define MESH_MSG_GENERIC_POWER_DEFAULT_SET_UNACK        0x8220
#define MESH_MSG_GENERIC_POWER_RANGE_SET                0x8221
#define MESH_MSG_GENERIC_POWER_RANGE_SET_UNACK          0x8222
/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_POWER_LEVEL_SERVER           0x1009FFFF
#define MESH_MODEL_GENERIC_POWER_LEVEL_SETUP_SERVER     0x100AFFFF
#define MESH_MODEL_GENERIC_POWER_LEVEL_CLIENT           0x100BFFFF
/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_MESH_MSG Mesh Msg
 * @brief Message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_LEVEL_GET)];
} _PACKED_ generic_power_level_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_LEVEL_SET)];
    uint16_t power;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ generic_power_level_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_LEVEL_STAT)];
    uint16_t present_power;
    uint16_t target_power; //!< optional
    /* If the Target Power field is present, the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ generic_power_level_stat_t, *generic_power_level_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_LAST_GET)];
} _PACKED_ generic_power_last_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_LAST_STAT)];
    uint16_t power;
} _PACKED_ generic_power_last_stat_t, *generic_power_last_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_DEFAULT_GET)];
} _PACKED_ generic_power_default_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_DEFAULT_SET)];
    uint16_t power;
} _PACKED_ generic_power_default_set_t, *generic_power_default_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_DEFAULT_STAT)];
    uint16_t power;
} _PACKED_ generic_power_default_stat_t, *generic_power_default_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_RANGE_GET)];
} _PACKED_ generic_power_range_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_RANGE_SET)];
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ generic_power_range_set_t, *generic_power_range_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_POWER_RANGE_STAT)];
    generic_stat_t stat;
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ generic_power_range_stat_t, *generic_power_range_stat_p;
/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_POWER_LEVEL             0
#define DELAY_EXECUTION_TYPE_POWER_LEVEL                0
/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_POWER_LEVEL_SERVER_GET                              0 //!< @ref generic_power_level_server_get_t
#define GENERIC_POWER_LEVEL_SERVER_GET_DEFAULT                      1 //!< @ref generic_power_level_server_get_t 
#define GENERIC_POWER_LEVEL_SERVER_GET_LAST                         2 //!< @ref generic_power_level_server_get_t
#define GENERIC_POWER_LEVEL_SERVER_GET_RANGE                        3 //!< @ref generic_power_level_server_get_range_t
#define GENERIC_POWER_LEVEL_SERVER_GET_DEFAULT_TRANSITION_TIME      4 //!< @ref generic_power_level_server_get_default_transition_time_t
#define GENERIC_POWER_LEVEL_SERVER_SET                              5 //!< @ref generic_power_level_server_set_t
#define GENERIC_POWER_LEVEL_SERVER_SET_LAST                         6 //!< @ref generic_power_level_server_set_last_t
#define GENERIC_POWER_LEVEL_SERVER_SET_DEFAULT                      7 //!< @ref generic_power_level_server_set_default_t
#define GENERIC_POWER_LEVEL_SERVER_SET_RANGE                        8 //!< @ref generic_power_level_server_set_range_t

typedef struct
{
    uint16_t power;
} generic_power_level_server_get_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} generic_power_level_server_get_range_t;

typedef struct
{
    generic_transition_time_t trans_time;
} generic_power_level_server_get_default_transition_time_t;

typedef struct
{
    uint16_t power;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} generic_power_level_server_set_t;

typedef struct
{
    uint16_t power;
} generic_power_level_server_set_last_t;

typedef struct
{
    uint16_t power;
} generic_power_level_server_set_default_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} generic_power_level_server_set_range_t;

/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_POWER_LEVEL_CLIENT_STATUS                        0 //!< @ref generic_power_level_client_status_t
#define GENERIC_POWER_LEVEL_CLIENT_STATUS_LAST                   1 //!< @ref generic_power_level_client_status_simple_t
#define GENERIC_POWER_LEVEL_CLIENT_STATUS_DEFAULT                2 //!< @ref generic_power_level_client_status_simple_t
#define GENERIC_POWER_LEVEL_CLIENT_STATUS_RANGE                  3 //!< @ref generic_power_level_client_status_range_t

typedef struct
{
    uint16_t src;
    uint16_t present_power;
    bool optional;
    uint16_t target_power;
    generic_transition_time_t remaining_time;
} generic_power_level_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t power;
} generic_power_level_client_status_simple_t;

typedef struct
{
    uint16_t src;
    generic_stat_t stat;
    uint16_t range_min;
    uint16_t range_max;
} generic_power_level_client_status_range_t;
/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic power level server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power level server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_level_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register generic power level setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power level setup server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_level_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic power level status
 * @param[in] pmodel_info: pointer to generic power level server model context
 * @param[in] power: power status need to publish
 * @return publish status
 */
mesh_msg_send_cause_t generic_power_level_publish(const mesh_model_info_p pmodel_info,
                                                  uint16_t power);

/**
 * @brief convert generic power level value to level value
 * @param[in] power: genric power level value
 * @return generic level value
 */
int16_t generic_power_level_to_generic_level(uint16_t power);

/**
 * @brief convert generic level value to generic power level value
 * @param[in] level: generic level value
 * @return generic power level value
 */
uint16_t generic_level_to_power_level(int16_t level);

/** @} */

/**
 * @defgroup GENERIC_POWER_LEVEL_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic power level client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_level_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic power level value
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_power_level_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                              uint16_t app_key_index);

/**
 * @brief set generic power level
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] power: desired power level value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_power_level_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                              uint16_t app_key_index,
                                              uint16_t power, uint8_t tid, bool optional,
                                              generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get generic power level last value
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_power_last_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                             uint16_t app_key_index);

/**
 * @brief get generic power level default value
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_power_default_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index);

/**
 * @brief set generic power level default value
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] power: default power level
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_power_default_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t power, bool ack);

/**
 * @brief get generic power level range
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_power_range_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                              uint16_t app_key_index);

/**
 * @brief set generic power level range
 * @param[in] pmodel_info: pointer to generic power level client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] range_min: power level minimum value
 * @param[in] range_max: power level maximum value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_power_range_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                              uint16_t app_key_index, uint16_t range_min, uint16_t range_max, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_POWER_LEVEL_H */
