/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_ctl.h
* @brief    Head file for light ctl models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-27
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _LIGHT_CTL_H
#define _LIGHT_CTL_H

#include "mesh_api.h"
#include "generic_transition_time.h"
#include "generic_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CTL
 * @{
 */

/**
 * @defgroup LIGHT_CTL_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_CTL_GET                          0x825D
#define MESH_MSG_LIGHT_CTL_SET                          0x825E
#define MESH_MSG_LIGHT_CTL_SET_UNACK                    0x825F
#define MESH_MSG_LIGHT_CTL_STAT                         0x8260
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_GET              0x8261
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_GET        0x8262
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_STAT       0x8263
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_SET              0x8264
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_SET_UNACK        0x8265
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_STAT             0x8266
#define MESH_MSG_LIGHT_CTL_DEFAULT_GET                  0x8267
#define MESH_MSG_LIGHT_CTL_DEFAULT_STAT                 0x8268
#define MESH_MSG_LIGHT_CTL_DEFAULT_SET                  0x8269
#define MESH_MSG_LIGHT_CTL_DEFAULT_SET_UNACK            0x826A
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET        0x826B
#define MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK  0x826C
/** @} */

/**
 * @defgroup LIGHT_CTL_MODEL_DI Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_CTL_SERVER                     0x1303FFFF
#define MESH_MODEL_LIGHT_CTL_SETUP_SERVER               0x1304FFFF
#define MESH_MODEL_LIGHT_CTL_CLIENT                     0x1305FFFF
#define MESH_MODEL_LIGHT_CTL_TEMPERATURE_SERVER         0x1306FFFF
/** @} */

/**
 * @defgroup LIGHT_CTL_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define LIGHT_CTL_TEMPERATURE_LOWER_LIMIT               0x0320
#define LIGHT_CTL_TEMPERATURE_UPPER_LIMIT               0x4E20
#define LIGHT_CTL_TEMPERATURE_DELTA                     (0x4E20 - 0x0320)
#define LIGHT_CTL_TEMPERATURE_UNKNOWN                   0xFFFF
#define IS_LIGHT_CTL_TEMPERATURE_VALID(VAL)  \
    (((VAL) >= LIGHT_CTL_TEMPERATURE_LOWER_LIMIT) && ((VAL) <= LIGHT_CTL_TEMPERATURE_UPPER_LIMIT))
#define IS_LIGHT_CTL_TEMPERATURE_RANGE_VALID(VAL) \
    (IS_LIGHT_CTL_TEMPERATURE_VALID(VAL) || ((VAL) == LIGHT_CTL_TEMPERATURE_UNKNOWN))

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_GET)];
} _PACKED_ light_ctl_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_SET)];
    uint16_t lightness;
    uint16_t temperature; //!< The color temperature of white light in Kelvin
    int16_t delta_uv;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_ctl_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_STAT)];
    uint16_t present_lightness;
    uint16_t present_temperature;
    uint16_t target_lightness; //!< optional
    /* If the Target CTL Lightness field is present, the Target CTL Temperature and the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    uint16_t target_temperature;
    generic_transition_time_t remaining_time;
} _PACKED_ light_ctl_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_GET)];
} _PACKED_ light_ctl_temperature_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_SET)];
    uint16_t temperature; //!< The color temperature of white light in Kelvin
    int16_t delta_uv;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ light_ctl_temperature_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_STAT)];
    uint16_t present_temperature;
    uint16_t present_delta_uv;
    uint16_t target_temperature; //!< optional
    /* If the Target CTL Lightness field is present, the Target CTL Temperature and the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    uint16_t target_delta_uv;
    generic_transition_time_t remaining_time;
} _PACKED_ light_ctl_temperature_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_GET)];
} _PACKED_ light_ctl_temperature_range_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET)];
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ light_ctl_temperature_range_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_STAT)];
    generic_stat_t stat;
    uint16_t range_min;
    uint16_t range_max;
} _PACKED_ light_ctl_temperature_range_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_DEFAULT_GET)];
} _PACKED_ light_ctl_default_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_DEFAULT_SET)];
    uint16_t lightness;
    uint16_t temperature; //!< The color temperature of white light in Kelvin
    int16_t delta_uv;
} _PACKED_ light_ctl_default_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_CTL_DEFAULT_STAT)];
    uint16_t lightness;
    uint16_t temperature; //!< The color temperature of white light in Kelvin
    int16_t delta_uv;
} _PACKED_ light_ctl_default_stat_t;
/** @} */

/**
 * @defgroup LIGHT_CTL_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LIGHT_CTL                0
#define GENERIC_TRANSITION_TYPE_LIGHT_CTL_TEMPERATURE    1
#define DELAY_EXECUTION_TYPE_CTL                         0
#define DELAY_EXECUTION_TYPE_CTL_TEMPERATURE             1
/** @} */

/**
 * @defgroup LIGHT_CTL_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_CTL_SERVER_GET                            0 //!< @ref light_ctl_server_get_t
#define LIGHT_CTL_SERVER_GET_DEFAULT                    1 //!< @ref light_ctl_server_get_default_t
#define LIGHT_CTL_SERVER_GET_TEMPERATURE                2 //!< @ref light_ctl_server_get_temperature_t
#define LIGHT_CTL_SERVER_GET_TEMPERATURE_RANGE          3 //!< @ref light_ctl_server_get_temperature_range_t
#define LIGHT_CTL_SERVER_GET_DEFAULT_TRANSITION_TIME    4 //!< @ref light_ctl_server_get_default_transition_time_t
#define LIGHT_CTL_SERVER_SET                            5 //!< @ref light_ctl_server_set_t
#define LIGHT_CTL_SERVER_SET_TEMPERATURE                6 //!< @ref light_ctl_server_set_temperature_t
#define LIGHT_CTL_SERVER_SET_DEFAULT                    7 //!< @ref light_ctl_server_set_default_t
#define LIGHT_CTL_SERVER_SET_TEMPERATURE_RANGE          8 //!< @ref light_ctl_server_set_temperature_range_t

typedef struct
{
    uint16_t lightness;
    uint16_t temperature;
} light_ctl_server_get_t;

typedef struct
{
    uint16_t temperature;
    int16_t delta_uv;
} light_ctl_server_get_temperature_t;

typedef struct
{
    generic_transition_time_t trans_time;
} light_ctl_server_get_default_transition_time_t;

typedef struct
{
    uint16_t lightness;
    uint16_t temperature;
    int16_t delta_uv;
} light_ctl_server_get_default_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} light_ctl_server_get_temperature_range_t;

typedef struct
{
    uint16_t lightness;
    uint16_t temperature;
    int16_t delta_uv;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_ctl_server_set_t;

typedef struct
{
    uint16_t temperature;
    int16_t delta_uv;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_ctl_server_set_temperature_t;

typedef struct
{
    uint16_t lightness;
    uint16_t temperature;
    int16_t delta_uv;
} light_ctl_server_set_default_t;

typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} light_ctl_server_set_temperature_range_t;

/** @} */

/**
 * @defgroup LIGHT_CTL_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_CTL_CLIENT_STATUS                        0 //!< @ref light_ctl_client_status_t
#define LIGHT_CTL_CLIENT_STATUS_TEMPERATURE            1 //!< @ref light_ctl_client_status_temperature_t
#define LIGHT_CTL_CLIENT_STATUS_TEMPERATURE_RANGE      2 //!< @ref light_ctl_client_status_temperature_range_t
#define LIGHT_CTL_CLIENT_STATUS_DEFAULT                3 //!< @ref light_ctl_client_status_default_t

typedef struct
{
    uint16_t src;
    uint16_t present_lightness;
    uint16_t present_temperature;
    bool optional;
    uint16_t target_lightness;
    uint16_t target_temperature;
    generic_transition_time_t remaining_time;
} light_ctl_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t present_temperature;
    uint16_t present_delta_uv;
    bool optional;
    uint16_t target_temperature;
    uint16_t target_delta_uv;
    generic_transition_time_t remaining_time;
} light_ctl_client_status_temperature_t;

typedef struct
{
    uint16_t src;
    generic_stat_t status;
    uint16_t range_min;
    uint16_t range_max;
} light_ctl_client_status_temperature_range_t;

typedef struct
{
    uint16_t src;
    uint16_t lightness;
    uint16_t temperature;
    int16_t delta_uv;
} light_ctl_client_status_default_t;
/** @} */


/**
 * @defgroup LIGHT_CTL_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light ctl server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light ctl server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_ctl_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light ctl setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light ctl server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_ctl_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light ctl temperature server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light ctl temperature server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_ctl_temperature_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish light ctl status
 * @param[in] pmodel_info: pointer to light ctl server model context
 * @param[in] lightness: lightness need to publish
 * @param[in] temperature: temperature need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_ctl_publish(const mesh_model_info_p pmodel_info, uint16_t lightness,
                                        uint16_t temperature);

/**
 * @brief publish light temperature status
 * @param[in] pmodel_info: pointer to light ctl temperature server model context
 * @param[in] temperature: temperature need to publish
 * @param[in] delta_uv: delta_uv need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_ctl_temperature_publish(const mesh_model_info_p pmodel_info,
                                                    uint16_t temperature, int16_t delta_uv);

/**
 * @brief convert light ctl temperature to generic level
 * @param[in] temperature: light ctl temperature
 * @return generic level value
 */
int16_t light_ctl_temperature_to_generic_level(uint16_t temperature);

/**
 * @brief convert generic level to light ctl temperature
 * @param[in] level: generic level value
 * @return light ctl temperature value
 */
uint16_t generic_level_to_light_ctl_temperature(int16_t level);

/**
 * @brief convert light ctl delta uv to represented delta uv
 * @param[in] delta_uv: light ctl delta uv
 * @return light represented delta uv
 */
float light_ctl_delta_uv_to_represented_delta_uv(int16_t delta_uv);

/**
 * @brief convert light represented delta uv to light ctl delta uv
 * @param[in] delta_uv: light represented delta uv
 * @return light ctl delta uv
 */
int16_t light_represented_delta_uv_to_ctl_delta_uv(float delta_uv);

/** @} */

/**
 * @defgroup LIGHT_CTL_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light ctl client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_ctl_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get light ctl
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);
/**
 * @brief set light ctl
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired lightness
 * @param[in] temperature: desired temperature
 * @param[in] delta_uv: desired delta_uv
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index, uint16_t lightness, uint16_t temperature, int16_t delta_uv, uint8_t tid,
                                    bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light ctl temperature
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_temperature_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index);

/**
 * @brief set light ctl temperature
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] temperature: desired temperature
 * @param[in] delta_uv: desired delta_uv
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_temperature_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t temperature, int16_t delta_uv, uint8_t tid, bool optional,
                                                generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light ctl temperature range
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_temperature_range_get(const mesh_model_info_p pmodel_info,
                                                      uint16_t dst,
                                                      uint16_t app_key_index);

/**
 * @brief set light ctl temperature range
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] range_min: desired light lightness minimum value
 * @param[in] range_max: desired light lightness maximum value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_temperature_range_set(const mesh_model_info_p pmodel_info,
                                                      uint16_t dst,
                                                      uint16_t app_key_index, uint16_t range_min, uint16_t range_max, bool ack);

/**
 * @brief get default light ctl
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_default_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index);

/**
 * @brief set default light ctl
 * @param[in] pmodel_info: pointer to light ctl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] lightness: desired default lightness value
 * @param[in] temperature : desired default temperature value
 * @param[in] delta_uv: desired default delta_uv value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_ctl_default_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, uint16_t lightness, uint16_t temperature, int16_t delta_uv, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CTL_H */
