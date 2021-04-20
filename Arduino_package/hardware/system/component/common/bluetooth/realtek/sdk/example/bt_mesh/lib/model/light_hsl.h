/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_hsl.h
* @brief    Head file for light hsl models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-27
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _LIGHT_HSL_H
#define _LIGHT_HSL_H

#include "mesh_api.h"
#include "generic_transition_time.h"
#include "generic_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_HSL
 * @{
 */

/**
 * @defgroup LIGHT_HSL_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_HSL_GET                          0x826D
#define MESH_MSG_LIGHT_HSL_HUE_GET                      0x826E
#define MESH_MSG_LIGHT_HSL_HUE_SET                      0x826F
#define MESH_MSG_LIGHT_HSL_HUE_SET_UNACK                0x8270
#define MESH_MSG_LIGHT_HSL_HUE_STAT                     0x8271
#define MESH_MSG_LIGHT_HSL_SATURATION_GET               0x8272
#define MESH_MSG_LIGHT_HSL_SATURATION_SET               0x8273
#define MESH_MSG_LIGHT_HSL_SATURATION_SET_UNACK         0x8274
#define MESH_MSG_LIGHT_HSL_SATURATION_STAT              0x8275
#define MESH_MSG_LIGHT_HSL_SET                          0x8276
#define MESH_MSG_LIGHT_HSL_SET_UNACK                    0x8277
#define MESH_MSG_LIGHT_HSL_STAT                         0x8278
#define MESH_MSG_LIGHT_HSL_TARGET_GET                   0x8279
#define MESH_MSG_LIGHT_HSL_TARGET_STAT                  0x827A
#define MESH_MSG_LIGHT_HSL_DEFAULT_GET                  0x827B
#define MESH_MSG_LIGHT_HSL_DEFAULT_STAT                 0x827C
#define MESH_MSG_LIGHT_HSL_RANGE_GET                    0x827D
#define MESH_MSG_LIGHT_HSL_RANGE_STAT                   0x827E
#define MESH_MSG_LIGHT_HSL_DEFAULT_SET                  0x827F
#define MESH_MSG_LIGHT_HSL_DEFAULT_SET_UNACK            0x8280
#define MESH_MSG_LIGHT_HSL_RANGE_SET                    0x8281
#define MESH_MSG_LIGHT_HSL_RANGE_SET_UNACK              0x8282
/** @} */

/**
 * @defgroup LIGHT_HSL_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_HSL_SERVER                     0x1307FFFF
#define MESH_MODEL_LIGHT_HSL_SETUP_SERVER               0x1308FFFF
#define MESH_MODEL_LIGHT_HSL_CLIENT                     0x1309FFFF
#define MESH_MODEL_LIGHT_HSL_HUE_SERVER                 0x130AFFFF
#define MESH_MODEL_LIGHT_HSL_SATURATION_SERVER          0x130BFFFF
/** @} */

/**
 * @defgroup LIGHT_HSL_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define LIGHT_HSL_TEMPERATURE_LOWER_LIMIT               0x0320
#define LIGHT_HSL_TEMPERATURE_UPPER_LIMIT               0x4E20

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_GET)];
} _PACKED_ light_hsl_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_SET)];
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_hsl_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_STAT)];
    uint16_t lightness; //!< actual
    uint16_t hue;
    uint16_t saturation;
    generic_transition_time_t remaining_time; //!< optional
} _PACKED_ light_hsl_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_TARGET_GET)];
} _PACKED_ light_hsl_target_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_TARGET_STAT)];
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
    generic_transition_time_t remaining_time; //!< optional
} _PACKED_ light_hsl_target_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_HUE_GET)];
} _PACKED_ light_hsl_hue_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_HUE_SET)];
    uint16_t hue;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_hsl_hue_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_HUE_STAT)];
    uint16_t present_hue;
    uint16_t target_hue; //!< optional
    /* If the target_hue field is present, the remaining_time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ light_hsl_hue_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_SATURATION_GET)];
} _PACKED_ light_hsl_saturation_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_SATURATION_SET)];
    uint16_t saturation;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_hsl_saturation_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_SATURATION_STAT)];
    uint16_t present_saturation;
    uint16_t target_saturation; //!< optional
    /* If the target_saturation field is present, the remaining_time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ light_hsl_saturation_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_DEFAULT_GET)];
} _PACKED_ light_hsl_default_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_DEFAULT_SET)];
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} _PACKED_ light_hsl_default_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_DEFAULT_STAT)];
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} _PACKED_ light_hsl_default_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_RANGE_GET)];
} _PACKED_ light_hsl_range_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_RANGE_SET)];
    uint16_t hue_range_min;
    uint16_t hue_range_max;
    uint16_t saturation_range_min;
    uint16_t saturation_range_max;
} _PACKED_ light_hsl_range_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_HSL_RANGE_STAT)];
    generic_stat_t stat;
    uint16_t hue_range_min;
    uint16_t hue_range_max;
    uint16_t saturation_range_min;
    uint16_t saturation_range_max;
} _PACKED_ light_hsl_range_stat_t;
/** @} */

/**
 * @defgroup LIGHT_HSL_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LIGHT_HSL                      0
#define GENERIC_TRANSITION_TYPE_LIGHT_HSL_HUE                  1
#define GENERIC_TRANSITION_TYPE_LIGHT_HSL_SATURATION           2
#define DELAY_EXECUTION_TYPE_LIGHT_HSL                         0
#define DELAY_EXECUTION_TYPE_LIGHT_HSL_HUE                     1
#define DELAY_EXECUTION_TYPE_LIGHT_HSL_SATURATION              2
/** @} */

/**
 * @defgroup LIGHT_HSL_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_HSL_SERVER_GET                            0 //!< @ref light_hsl_server_get_t
#define LIGHT_HSL_SERVER_GET_HUE                        1 //!< @ref light_hsl_server_get_hue_t
#define LIGHT_HSL_SERVER_GET_SATURATION                 2 //!< @ref light_hsl_server_get_saturation_t
#define LIGHT_HSL_SERVER_GET_DEFAULT                    3 //!< @ref light_hsl_server_get_default_t
#define LIGHT_HSL_SERVER_GET_RANGE                      4 //!< @ref light_hsl_server_get_range_t
#define LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME    5 //!< @ref light_hsl_server_get_default_transition_time_t
#define LIGHT_HSL_SERVER_SET                            6 //!< @ref light_hsl_server_set_t
#define LIGHT_HSL_SERVER_SET_HUE                        8 //!< @ref light_hsl_server_set_hue_t
#define LIGHT_HSL_SERVER_SET_SATURATION                 9 //!< @ref light_hsl_server_set_saturation_t
#define LIGHT_HSL_SERVER_SET_DEFAULT                    10 //!< @ref light_hsl_server_set_default_t
#define LIGHT_HSL_SERVER_SET_RANGE                      11 //!< @ref light_hsl_server_set_range_t

typedef struct
{
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} light_hsl_server_get_t;

typedef struct
{
    uint16_t hue;
} light_hsl_server_get_hue_t;

typedef struct
{
    uint16_t saturation;
} light_hsl_server_get_saturation_t;

typedef struct
{
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} light_hsl_server_get_default_t;

typedef struct
{
    uint16_t hue_range_min;
    uint16_t hue_range_max;
    uint16_t saturation_range_min;
    uint16_t saturation_range_max;
} light_hsl_server_get_range_t;

typedef struct
{
    generic_transition_time_t trans_time;
} light_hsl_server_get_default_transition_time_t;

typedef struct
{
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_hsl_server_set_t;

typedef struct
{
    uint16_t hue;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_hsl_server_set_hue_t;

typedef struct
{
    uint16_t saturation;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_hsl_server_set_saturation_t;

typedef struct
{
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} light_hsl_server_set_default_t;

typedef struct
{
    uint16_t hue_range_min;
    uint16_t hue_range_max;
    uint16_t saturation_range_min;
    uint16_t saturation_range_max;
} light_hsl_server_set_range_t;

/** @} */

/**
 * @defgroup LIGHT_HSL_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_HSL_CLIENT_STATUS                        0 //!< @ref light_hsl_client_status_t
#define LIGHT_HSL_CLIENT_STATUS_TARGET                 1 //!< @ref light_hsl_client_status_t
#define LIGHT_HSL_CLIENT_STATUS_HUE                    2 //!< @ref light_hsl_client_status_hue_t
#define LIGHT_HSL_CLIENT_STATUS_SATURATION             3 //!< @ref light_hsl_client_status_saturation_t
#define LIGHT_HSL_CLIENT_STATUS_DEFAULT                4 //!< @ref light_hsl_client_status_default_t
#define LIGHT_HSL_CLIENT_STATUS_RANGE                  5 //!< @ref light_hsl_client_status_range_t

typedef struct
{
    uint16_t src;
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
    bool optional;
    generic_transition_time_t remaining_time;
} light_hsl_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t present_hue;
    bool optional;
    uint16_t target_hue;
    generic_transition_time_t remaining_time;
} light_hsl_client_status_hue_t;

typedef struct
{
    uint16_t src;
    uint16_t present_saturation;
    bool optional;
    uint16_t target_saturation;
    generic_transition_time_t remaining_time;
} light_hsl_client_status_saturation_t;

typedef struct
{
    uint16_t src;
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} light_hsl_client_status_default_t;

typedef struct
{
    uint16_t src;
    generic_stat_t status;
    uint16_t hue_range_min;
    uint16_t hue_range_max;
    uint16_t saturation_range_min;
    uint16_t saturation_range_max;
} light_hsl_client_status_range_t;
/** @} */

/**
 * @defgroup LIGHT_HSL_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief register light hsl server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light hsl server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_hsl_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light hsl setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light hsl setup server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_hsl_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light hsl hue server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light hsl hue server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_hsl_hue_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light hsl saturation server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light hsl saturation server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_hsl_saturation_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish light hsl status
 * @param[in] pmodel_info: pointer to light hsl server model context
 * @param[in] lightness: light lightness need to publish
 * @param[in] hue: light hue need to publish
 * @param[in] saturation: light saturation need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_hsl_publish(const mesh_model_info_p pmodel_info, uint16_t lightness,
                                        uint16_t hue, uint16_t saturation);

/**
 * @brief publish light hsl hue status
 * @param[in] pmodel_info: pointer to light hsl hue server model context
 * @param[in] hue: light hue need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_hsl_hue_publish(const mesh_model_info_p pmodel_info, uint16_t hue);

/**
 * @brief publish light hsl saturation status
 * @param[in] pmodel_info: pointer to light hsl saturation server model context
 * @param[in] saturation: light saturation need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_hsl_saturation_publish(const mesh_model_info_p pmodel_info,
                                                   uint16_t saturation);

/**
 * @brief convert light hue value to generic level value
 * @param[in] hue: light hue value
 * @return generic level value
 */
int16_t light_hue_to_generic_level(uint16_t hue);

/**
 * @brief convert generic level value to light hue value
 * @param[in] level: generic level value
 * @return light hue value
 */
uint16_t generic_level_to_light_hue(int16_t level);

/**
 * @brief convert light saturation value to generic level value
 * @param[in] saturation: light saturation value
 * @return generic level value
 */
int16_t light_saturation_to_generic_level(uint16_t saturation);

/**
 * @brief convert generic level value to light saturation value
 * @param[in] level: generic level value
 * @return light saturation value
 */
uint16_t generic_level_to_light_saturation(int16_t level);

/** @} */

/**
 * @defgroup LIGHT_HSL_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light hsl client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_hsl_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get light hsl value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);

/**
 * @brief set light hsl value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired lightness
 * @param[in] hue: desired hue
 * @param[in] saturation: desired saturation
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index, uint16_t lightness, uint16_t hue, uint16_t saturation, uint8_t tid,
                                    bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light hsl target value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_target_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                           uint16_t app_key_index);

/**
 * @brief get light hsl hue value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_hue_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index);

/**
 * @brief set light hsl hue value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] hue: desired hue
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_hue_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index, uint16_t hue, uint8_t tid, bool optional,
                                        generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light hsl saturation value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_saturation_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                               uint16_t app_key_index);

/**
 * @brief set light hsl saturation value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] saturation: desired saturation
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_saturation_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                               uint16_t app_key_index, uint16_t saturation, uint8_t tid, bool optional,
                                               generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light hsl default value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_default_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index);

/**
 * @brief set default light hsl value
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired default lightness value
 * @param[in] hue: desired default hue value
 * @param[in] saturation: desired default saturation value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_default_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, uint16_t lightness, uint16_t hue, uint16_t saturation, bool ack);

/**
 * @brief get light hsl range
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_range_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index);

/**
 * @brief set light hsl range
 * @param[in] pmodel_info: pointer to light hsl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] hue_range_min: desired light hue minimum value
 * @param[in] hue_range_max: desired light hue maximum value
 * @param[in] saturation_range_min: desired light saturation minimum value
 * @param[in] saturation_range_max: desired light saturation maximum value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_hsl_range_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index, uint16_t hue_range_min, uint16_t hue_range_max,
                                          uint16_t saturation_range_min, uint16_t saturation_range_max, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_HSL_H */
