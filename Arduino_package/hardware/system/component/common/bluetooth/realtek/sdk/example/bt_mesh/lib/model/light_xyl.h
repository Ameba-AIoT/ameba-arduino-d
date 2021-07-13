/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_xyl.h
* @brief    Head file for light xyl models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2019-07-09
* @version  v1.0
* *************************************************************************************
*/
#ifndef _LIGHT_XYL_H
#define _LIGHT_XYL_H

#include "mesh_api.h"
#include "generic_types.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_XYL
 * @{
 */

/**
 * @defgroup LIGHT_XYL_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_XYL_GET                          0x8283
#define MESH_MSG_LIGHT_XYL_SET                          0x8284
#define MESH_MSG_LIGHT_XYL_SET_UNACK                    0x8285
#define MESH_MSG_LIGHT_XYL_STATUS                       0x8286
#define MESH_MSG_LIGHT_XYL_TARGET_GET                   0x8287
#define MESH_MSG_LIGHT_XYL_TARGET_STATUS                0x8288
#define MESH_MSG_LIGHT_XYL_DEFAULT_GET                  0x8289
#define MESH_MSG_LIGHT_XYL_DEFAULT_STATUS               0x828A
#define MESH_MSG_LIGHT_XYL_RANGE_GET                    0x828B
#define MESH_MSG_LIGHT_XYL_RANGE_STATUS                 0x828C
#define MESH_MSG_LIGHT_XYL_DEFAULT_SET                  0x828D
#define MESH_MSG_LIGHT_XYL_DEFAULT_SET_UNACK            0x828E
#define MESH_MSG_LIGHT_XYL_RANGE_SET                    0x828F
#define MESH_MSG_LIGHT_XYL_RANGE_SET_UNACK              0x8290
/** @} */

/**
 * @defgroup LIGHT_XYL_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_XYL_SERVER                     0x130CFFFF
#define MESH_MODEL_LIGHT_XYL_SETUP_SERVER               0x130DFFFF
#define MESH_MODEL_LIGHT_XYL_CLIENT                     0x130EFFFF
/** @} */

/**
 * @defgroup LIGHT_XYL_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint16_t xyl_lightness;
    uint16_t xyl_x;
    uint16_t xyl_y;
} _PACKED_ light_xyl_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_GET)];
} _PACKED_ light_xyl_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_SET)];
    light_xyl_t xyl;
    uint8_t tid;
    generic_transition_time_t trans_time;
    uint8_t delay;
} _PACKED_ light_xyl_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_STATUS)];
    light_xyl_t xyl;
    generic_transition_time_t remaining_time;
} _PACKED_ light_xyl_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_TARGET_GET)];
} _PACKED_ light_xyl_target_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_TARGET_STATUS)];
    light_xyl_t xyl;
    generic_transition_time_t remaining_time;
} _PACKED_ light_xyl_target_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_DEFAULT_GET)];
} _PACKED_ light_xyl_default_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_DEFAULT_SET)];
    light_xyl_t xyl;
} _PACKED_ light_xyl_default_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_DEFAULT_STATUS)];
    light_xyl_t xyl;
} _PACKED_ light_xyl_default_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_RANGE_GET)];
} _PACKED_ light_xyl_range_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_RANGE_SET)];
    uint16_t xyl_x_range_min;
    uint16_t xyl_x_range_max;
    uint16_t xyl_y_range_min;
    uint16_t xyl_y_range_max;
} _PACKED_ light_xyl_range_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_XYL_RANGE_SET)];
    uint8_t status_code;
    uint16_t xyl_x_range_min;
    uint16_t xyl_x_range_max;
    uint16_t xyl_y_range_min;
    uint16_t xyl_y_range_max;
} _PACKED_ light_xyl_range_status_t;
/** @} */

/**
 * @defgroup LIGHT_XYL_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LIGHT_XYL                         0
#define DELAY_EXECUTION_TYPE_LIGHT_XYL                            0
/** @} */


/**
 * @defgroup LIGHT_XYL_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_XYL_SERVER_GET                                     0 //!< @ref light_xyl_server_get_t
#define LIGHT_XYL_SERVER_GET_RANGE                               1 //!< @ref light_xyl_server_get_range_t
#define LIGHT_XYL_SERVER_GET_DEFAULT                             2 //!< @ref light_xyl_server_get_default_t
#define LIGHT_XYL_SERVER_GET_DEFAULT_TRANSITION_TIME             3 //!< @ref light_xyl_server_get_default_transition_time_t
#define LIGHT_XYL_SERVER_SET                                     4 //!< @ref light_xyl_server_set_t
#define LIGHT_XYL_SERVER_SET_DEFAULT                             5 //!< @ref light_xyl_server_set_default_t
#define LIGHT_XYL_SERVER_SET_RANGE                               6 //!< @ref light_xyl_server_set_range_t

typedef light_xyl_t light_xyl_server_get_t;

typedef struct
{
    uint16_t xyl_x_range_min;
    uint16_t xyl_x_range_max;
    uint16_t xyl_y_range_min;
    uint16_t xyl_y_range_max;
} light_xyl_server_get_range_t;

typedef light_xyl_t light_xyl_server_get_default_t;

typedef struct
{
    generic_transition_time_t trans_time;
} light_xyl_server_get_default_transition_time_t;

typedef struct
{
    light_xyl_t xyl;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_xyl_server_set_t;

typedef light_xyl_t light_xyl_server_set_default_t;

typedef struct
{
    uint16_t xyl_x_range_min;
    uint16_t xyl_x_range_max;
    uint16_t xyl_y_range_min;
    uint16_t xyl_y_range_max;
} light_xyl_server_set_range_t;
/** @} */

/**
 * @defgroup LIGHT_XYL_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_XYL_CLIENT_STATUS                                  0 //!< @ref light_xyl_client_status_t
#define LIGHT_XYL_CLIENT_STATUS_TARGET                           1 //!< @ref light_xyl_client_status_t
#define LIGHT_XYL_CLIENT_STATUS_DEFAULT                          2 //!< @ref light_xyl_client_status_default_t
#define LIGHT_XYL_CLIENT_STATUS_RANGE                            3 //!< @ref light_xyl_client_status_range_t

typedef struct
{
    uint16_t src;
    light_xyl_t xyl;
    bool optional;
    generic_transition_time_t remaining_time;
} light_xyl_client_status_t;

typedef struct
{
    uint16_t src;
    light_xyl_t xyl;
} light_xyl_client_status_default_t;

typedef struct
{
    uint16_t src;
    generic_stat_t status;
    uint16_t xyl_x_range_min;
    uint16_t xyl_x_range_max;
    uint16_t xyl_y_range_min;
    uint16_t xyl_y_range_max;
} light_xyl_client_status_range_t;
/** @} */


/**
 * @defgroup LIGHT_XYL_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register light xyl server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light xyl server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_xyl_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light xyl setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light xyl setup server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_xyl_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish light xyl
 * @param[in] pmodel_info: pointer to light xyl server model context
 * @param[in] xyl: xyl need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_xyl_publish(const mesh_model_info_p pmodel_info, light_xyl_t xyl);

/**
 * @brief convert light xyl x to cie1931 x
 * @param[in] xyl_x: light xyl_x
 * @return cie1931 x value
 */
double light_xyl_x_to_cie1931_x(uint16_t xyl_x);

/**
 * @brief convert cie1931 x to light xyl x
 * @param[in] cie1931_x: cie1931 x value
 * @return light xyl x value
 */
uint16_t light_cie1931_x_to_xyl_x(double cie1931_x);

/**
 * @brief convert light xyl y to cie1931 y
 * @param[in] xyl_y: light xyl_y
 * @return cie1931 y value
 */
double light_xyl_y_to_cie1931_y(uint16_t xyl_y);

/**
 * @brief convert cie1931 y to light xyl y
 * @param[in] cie1931_y: cie1931 y value
 * @return light xyl y value
 */
uint16_t light_cie1931_y_to_xyl_y(double cie1931_y);

/**
 * @brief convert light intensity to xyl lightness
 * @param[in] intensity: light intensity
 * @return xyl lightness value
 */
uint16_t light_intensity_to_xyl_lightness(uint16_t intensity);

/**
 * @brief convert xyl lightness to light intensity
 * @param[in] lightness: xyl lightness
 * @return light internsity value
 */
uint16_t light_xyl_lightness_to_internsity(uint16_t lightness);
/** @} */

/**
 * @defgroup LIGHT_LIGHTNESS_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief register light xyl client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_xyl_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get light xyl value
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);

/**
 * @brief set light xyl value
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] xyl: desired xyl
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index, light_xyl_t xyl, uint8_t tid,
                                    bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light xyl target value
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_target_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                           uint16_t app_key_index);

/**
 * @brief get light xyl default value
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_default_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index);

/**
 * @brief set default light xyl value
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] xyl: desired default xyl value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_default_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, light_xyl_t xyl, bool ack);

/**
 * @brief get light xyl range
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_range_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index);

/**
 * @brief set light xyl range
 * @param[in] pmodel_info: pointer to light xyl client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] xyl_x_range_min: desired light xyl_x minimum value
 * @param[in] xyl_x_range_max: desired light xyl_x maximum value
 * @param[in] xyl_y_range_min: desired light xyl_y minimum value
 * @param[in] xyl_y_range_max: desired light xyl_y maximum value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_xyl_range_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index, uint16_t xyl_x_range_min, uint16_t xyl_x_range_max,
                                          uint16_t xyl_y_range_min, uint16_t xyl_y_range_max, bool ack);

/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_XYL_H */

