/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_lightness.h
* @brief    Head file for light lightness models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-27
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _LIGHT_LIGHTNESS_H
#define _LIGHT_LIGHTNESS_H

#include "mesh_api.h"
#include "generic_types.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_LIGHTNESS
 * @{
 */

/**
 * @defgroup LIGHT_LIGHTNESS_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_LIGHTNESS_GET                    0x824B
#define MESH_MSG_LIGHT_LIGHTNESS_SET                    0x824C
#define MESH_MSG_LIGHT_LIGHTNESS_SET_UNACK              0x824D
#define MESH_MSG_LIGHT_LIGHTNESS_STAT                   0x824E
#define MESH_MSG_LIGHT_LIGHTNESS_LINEAR_GET             0x824F
#define MESH_MSG_LIGHT_LIGHTNESS_LINEAR_SET             0x8250
#define MESH_MSG_LIGHT_LIGHTNESS_LINEAR_SET_UNACK       0x8251
#define MESH_MSG_LIGHT_LIGHTNESS_LINEAR_STAT            0x8252
#define MESH_MSG_LIGHT_LIGHTNESS_LAST_GET               0x8253
#define MESH_MSG_LIGHT_LIGHTNESS_LAST_STAT              0x8254
#define MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_GET            0x8255
#define MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_STAT           0x8256
#define MESH_MSG_LIGHT_LIGHTNESS_RANGE_GET              0x8257
#define MESH_MSG_LIGHT_LIGHTNESS_RANGE_STAT             0x8258
#define MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_SET            0x8259
#define MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK      0x825A
#define MESH_MSG_LIGHT_LIGHTNESS_RANGE_SET              0x825B
#define MESH_MSG_LIGHT_LIGHTNESS_RANGE_SET_UNACK        0x825C
/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_LIGHTNESS_SERVER               0x1300FFFF
#define MESH_MODEL_LIGHT_LIGHTNESS_SETUP_SERVER         0x1301FFFF
#define MESH_MODEL_LIGHT_LIGHTNESS_CLIENT               0x1302FFFF
/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_GET)];
} _PACKED_ light_lightness_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_SET)];
    uint16_t lightness; //!< The target value of the Light Lightness Actual state.
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_lightness_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_STAT)];
    uint16_t present_lightness;
    uint16_t target_lightness; //!< optional
    /* If the Target Lightness field is present, the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ light_lightness_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_LINEAR_GET)];
} _PACKED_ light_lightness_linear_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_LINEAR_SET)];
    uint16_t lightness; //!< The target value of the Light Lightness Linear state.
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_lightness_linear_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_LINEAR_STAT)];
    uint16_t present_lightness;
    uint16_t target_lightness; //!< optional
    /* If the Target Lightness field is present, the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ light_lightness_linear_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_LAST_GET)];
} _PACKED_ light_lightness_last_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_LAST_STAT)];
    uint16_t lightness; //!< The target value of the Light Lightness Actual state.
} _PACKED_ light_lightness_last_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_GET)];
} _PACKED_ light_lightness_default_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_SET)];
    uint16_t lightness; //!< The target value of the Light Lightness Actual state.
} _PACKED_ light_lightness_default_set_t, *light_lightness_default_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_DEFAULT_STAT)];
    uint16_t lightness; //!< The target value of the Light Lightness Actual state.
} _PACKED_ light_lightness_default_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_RANGE_GET)];
} _PACKED_ light_lightness_range_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_RANGE_SET)];
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ light_lightness_range_set_t, *light_lightness_range_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LIGHTNESS_RANGE_STAT)];
    generic_stat_t stat;
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ light_lightness_range_stat_t;
/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LIGHT_LIGHTNESS                   0
#define GENERIC_TRANSITION_TYPE_LIGHT_LIGHTNESS_LINEAR            1
#define DELAY_EXECUTION_TYPE_LIGHT_LIGHTNESS                      0
#define DELAY_EXECUTION_TYPE_LIGHT_LIGHTNESS_LINEAR               1
/** @} */


/**
 * @defgroup LIGHT_LIGHTNESS_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_LIGHTNESS_SERVER_GET                               0 //!< @ref light_lightness_server_get_t
#define LIGHT_LIGHTNESS_SERVER_GET_LINEAR                        1 //!< @ref light_lightness_server_get_t
#define LIGHT_LIGHTNESS_SERVER_GET_DEFAULT                       2 //!< @ref light_lightness_server_get_t
#define LIGHT_LIGHTNESS_SERVER_GET_LAST                          3 //!< @ref light_lightness_server_get_t
#define LIGHT_LIGHTNESS_SERVER_GET_RANGE                         4 //!< @ref light_lightness_server_get_range_t
#define LIGHT_LIGHTNESS_SERVER_GET_DEFAULT_TRANSITION_TIME       5 //!< @ref light_lightness_server_get_default_transition_time_t
#define LIGHT_LIGHTNESS_SERVER_SET                               6 //!< @ref light_lightness_server_set_t
#define LIGHT_LIGHTNESS_SERVER_SET_LINEAR                        7 //!< @ref light_lightness_server_set_t
#define LIGHT_LIGHTNESS_SERVER_SET_LAST                          8 //!< @ref light_lightness_server_set_t
#define LIGHT_LIGHTNESS_SERVER_SET_DEFAULT                       9 //!< @ref light_lightness_server_set_t
#define LIGHT_LIGHTNESS_SERVER_SET_RANGE                         10 //!< @ref light_lightness_server_set_range_t

typedef struct
{
    uint16_t lightness;
} light_lightness_server_get_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} light_lightness_server_get_range_t;

typedef struct
{
    generic_transition_time_t trans_time;
} light_lightness_server_get_default_transition_time_t;

typedef struct
{
    uint16_t lightness;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_lightness_server_set_t;

typedef struct
{
    uint16_t lightness;
} light_lightness_server_set_last_t;

typedef struct
{
    uint16_t lightness;
} light_lightness_server_set_default_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} light_lightness_server_set_range_t;

/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_LIGHTNESS_CLIENT_STATUS                        0 //!< @ref light_lightness_client_status_t
#define LIGHT_LIGHTNESS_CLIENT_STATUS_LINEAR                 1 //!< @ref light_lightness_client_status_t
#define LIGHT_LIGHTNESS_CLIENT_STATUS_LAST                   2 //!< @ref light_lightness_client_status_last_t
#define LIGHT_LIGHTNESS_CLIENT_STATUS_DEFAULT                3 //!< @ref light_lightness_client_status_default_t
#define LIGHT_LIGHTNESS_CLIENT_STATUS_RANGE                  4 //!< @ref light_lightness_client_status_range_t

typedef struct
{
    uint16_t src;
    uint16_t present_lightness;
    bool optional;
    uint16_t target_lightness;
    generic_transition_time_t remaining_time;
} light_lightness_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t lightness;
} light_lightness_client_status_last_t;

typedef struct
{
    uint16_t src;
    uint16_t lightness;
} light_lightness_client_status_default_t;

typedef struct
{
    uint16_t src;
    generic_stat_t status;
    uint16_t range_min;
    uint16_t range_max;
} light_lightness_client_status_range_t;
/** @} */


/**
 * @defgroup LIGHT_LIGHTNESS_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light lightness server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lightness server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_lightness_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light lightness setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lightness setup server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_lightness_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief enable or disable lightness period publish
 * @param[in] lightness: lightness publish enable status
 * @param[in] lightness_linear: lightness linear publish enable status
 * @note default is lightness enable, lightness linear enabled
 */
void light_lightness_period_pub_enable(bool lightness, bool lightness_linear);

/**
 * @brief publish light lightness
 * @param[in] pmodel_info: pointer to light lightness server model context
 * @param[in] lightness: lightness need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_lightness_publish(const mesh_model_info_p pmodel_info,
                                              uint16_t lightness);

/**
 * @brief publish light lightness linear
 * @param[in] pmodel_info: pointer to light lightness server model context
 * @param[in] lightness: linear lightness need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_lightness_linear_publish(const mesh_model_info_p pmodel_info,
                                                     uint16_t lightness);

/**
 * @brief convert lightness linear to lightness actual
 * @param[in] lightness_linear: lightness linear
 * @return lightness value
 */
uint16_t light_lightness_linear_to_actual(uint16_t lightness_linear);

/**
 * @brief convert lightness actual to lightness linear
 * @param[in] lightness_actual: actual lightness
 * @return lightness linear value
 */
uint16_t light_lightness_actual_to_linear(uint16_t lightness_actual);

/**
 * @brief convert light lightness value to generic level value
 * @param[in] lightness: light lightness value
 * @return generic level value
 */
int16_t light_lightness_to_generic_level(uint16_t lightness);

/**
 * @brief convert generic level value to light lightness value
 * @param[in] level: generic level value
 * @return light lightness value
 */
uint16_t generic_level_to_light_lightness(int16_t level);

/**
 * @brief convert light lightness linear value to generic level value
 * @param[in] lightness: light lightness linear value
 * @return generic level value
 */
int16_t light_lightness_linear_to_generic_level(uint16_t lightness);

/**
 * @brief convert generic level value to light lightness linear value
 * @param[in] level: generic level value
 * @return light lightness lienar value
 */
uint16_t generic_level_to_light_lightness_linear(int16_t level);

/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light lightness client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_lightness_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get light lightness
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index);

/**
 * @brief set light lightness
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired lightness
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index, uint16_t lightness, uint8_t tid, bool optional,
                                          generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light lightness linear
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_linear_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index);

/**
 * @brief set light lightness linear
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired lightness linear
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_linear_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index, uint16_t lightness, uint8_t tid, bool optional,
                                                 generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get last light lightness
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_last_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                               uint16_t app_key_index);

/**
 * @brief get default light lightness
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_default_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                  uint16_t app_key_index);

/**
 * @brief set default light lightness
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired default light lightness value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_default_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                  uint16_t app_key_index, uint16_t lightness, bool ack);

/**
 * @brief get light lightness range
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_range_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index);

/**
 * @brief set light lightness range
 * @param[in] pmodel_info: pointer to light lightness client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] range_min: desired light lightness minimum value
 * @param[in] range_max: desired light lightness maximum value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lightness_range_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t range_min, uint16_t range_max, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_LIGHTNESS_H */
