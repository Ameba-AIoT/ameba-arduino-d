/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_lc.h
* @brief    Head file for light lc models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2019-07-17
* @version  v1.0
* *************************************************************************************
*/
#ifndef _LIGHT_LC_H_
#define _LIGHT_LC_H_

#include "mesh_api.h"
#include "generic_on_off.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_LC
 * @{
 */

/**
 * @defgroup LIGHT_LC_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_LIGHT_LC_MODE_GET                      0x8291
#define MESH_MSG_LIGHT_LC_MODE_SET                      0x8292
#define MESH_MSG_LIGHT_LC_MODE_SET_UNACK                0x8293
#define MESH_MSG_LIGHT_LC_MODE_STATUS                   0x8294
#define MESH_MSG_LIGHT_LC_OM_GET                        0x8295
#define MESH_MSG_LIGHT_LC_OM_SET                        0x8296
#define MESH_MSG_LIGHT_LC_OM_SET_UNACK                  0x8297
#define MESH_MSG_LIGHT_LC_OM_STATUS                     0x8298
#define MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_GET              0x8299
#define MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_SET              0x829A
#define MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_SET_UNACK        0x829B
#define MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_STATUS           0x829C
#define MESH_MSG_LIGHT_LC_PROPERTY_GET                  0x829D
#define MESH_MSG_LIGHT_LC_PROPERTY_SET                  0x62
#define MESH_MSG_LIGHT_LC_PROPERTY_SET_UNACK            0x63
#define MESH_MSG_LIGHT_LC_PROPERTY_STATUS               0x64
/** @} */

/**
 * @defgroup LIGHT_LC_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_LIGHT_LC_SERVER                      0x130FFFFF
#define MESH_MODEL_LIGHT_LC_SETUP_SERVER                0x1310FFFF
#define MESH_MODEL_LIGHT_LC_CLIENT                      0x1311FFFF
/** @} */

/**
 * @defgroup LIGHT_LC_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint32_t time_fade;
    uint32_t time_fade_on;
    uint32_t time_fade_standby_auto;
    uint32_t time_fade_standby_manual;
    uint32_t time_occupancy_delay;
    uint32_t time_prolong;
    uint32_t time_run_on;
} light_lc_time_t;

#define LIGHT_LC_MODE_TURNED_OFF                        0
#define LIGHT_LC_MODE_TURNED_ON                         1
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_MODE_GET)];
} _PACKED_ light_lc_mode_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_MODE_SET)];
    uint8_t mode;
} _PACKED_ light_lc_mode_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_MODE_STATUS)];
    uint8_t mode;
} _PACKED_ light_lc_mode_status_t;

#define LIGHT_LC_OM_OCCUPANCY_NOT_FROM_STANDBY           0
#define LIGHT_LC_OM_OCCUPANCY_FROM_STANDBY               1
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_OM_GET)];
} _PACKED_ light_lc_om_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_MODE_SET)];
    uint8_t mode;
} _PACKED_ light_lc_om_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_MODE_STATUS)];
    uint8_t mode;
} _PACKED_ light_lc_om_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_GET)];
} _PACKED_ light_lc_light_on_off_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_SET)];
    generic_on_off_t light_on_off;
    uint8_t tid;
    generic_transition_time_t trans_time;
    uint8_t delay;
} _PACKED_ light_lc_light_on_off_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_LIGHT_ON_OFF_STATUS)];
    generic_on_off_t present_light_on_off;
    generic_on_off_t target_light_on_off;
    generic_transition_time_t trans_time;
} _PACKED_ light_lc_light_on_off_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_PROPERTY_GET)];
    uint16_t property_id;
} _PACKED_ light_lc_property_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_PROPERTY_SET)];
    uint16_t property_id;
    uint8_t property_value[0];
} _PACKED_ light_lc_property_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_LIGHT_LC_PROPERTY_STATUS)];
    uint16_t property_id;
    uint8_t property_value[0];
} _PACKED_ light_lc_property_status_t;
/** @} */

/**
 * @defgroup LIGHT_LC_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_LIGHT_LC                          0
#define DELAY_EXECUTION_TYPE_LIGHT_LC                             0
/** @} */


/**
 * @defgroup LIGHT_LC_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_LC_SERVER_GET_MODE                                  0 //!< @ref light_lc_server_get_mode_t
#define LIGHT_LC_SERVER_GET_OM                                    1 //!< @ref light_lc_server_get_om_t
#define LIGHT_LC_SERVER_GET_LIGHT_ON_OFF                          2 //!< @ref light_lc_server_get_light_on_off_t
#define LIGHT_LC_SERVER_GET_PROPERTY                              3 //!< @ref light_lc_server_get_property_t
#define LIGHT_LC_SERVER_GET_DEFAULT_TRANSITION_TIME               4 //!< @ref light_lc_server_get_default_transition_time_t
#define LIGHT_LC_SERVER_GET_SM_TRANSITION_TIME                    5 //!< @ref light_lc_server_get_sm_transition_time_t
#define LIGHT_LC_SERVER_SET_MODE                                  6 //!< @ref light_lc_server_set_mode_t
#define LIGHT_LC_SERVER_SET_OM                                    7 //!< @ref light_lc_server_set_om_t
#define LIGHT_LC_SERVER_SET_LIGHT_ON_OFF                          8 //!< @ref light_lc_server_set_light_on_off_t
#define LIGHT_LC_SERVER_SET_PROPERTY                              9 //!< @ref light_lc_server_set_property_t


typedef struct
{
    uint8_t mode;
} light_lc_server_get_mode_t;

typedef struct
{
    uint8_t mode;
} light_lc_server_get_om_t;

typedef struct
{
    generic_on_off_t on_off;
} light_lc_server_get_light_on_off_t;

typedef struct
{
    uint16_t property_id;
    uint32_t property_value;
    uint8_t value_len;
} light_lc_server_get_property_t;

typedef struct
{
    generic_transition_time_t trans_time;
} light_lc_server_get_default_transition_time_t;

typedef struct
{
    generic_on_off_t light_on_off;
    generic_transition_time_t sm_trans_time;
} light_lc_server_get_sm_transition_time_t;

typedef struct
{
    uint8_t mode;
} light_lc_server_set_mode_t;

typedef struct
{
    uint8_t mode;
} light_lc_server_set_om_t;

typedef struct
{
    generic_on_off_t light_on_off;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} light_lc_server_set_light_on_off_t;

typedef struct
{
    uint16_t property_id;
    uint32_t property_value;
} light_lc_server_set_property_t;
/** @} */

/**
 * @defgroup LIGHT_LC_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define LIGHT_LC_CLIENT_MODE_STATUS                              0 //!< @ref light_lc_client_model_status_t
#define LIGHT_LC_CLIENT_OM_STATUS                                1 //!< @ref light_lc_client_om_status_t
#define LIGHT_LC_CLIENT_LIGHT_ON_OFF_STATUS                      2 //!< @ref light_lc_client_light_on_off_status_t
#define LIGHT_LC_CLIENT_PROPERTY_STATUS                          3 //!< @ref light_lc_client_property_status_t

typedef struct
{
    uint16_t src;
    uint8_t mode;
} light_lc_client_mode_status_t;

typedef struct
{
    uint16_t src;
    uint8_t mode;
} light_lc_client_om_status_t;

typedef struct
{
    uint16_t src;
    generic_on_off_t present_on_off;
    bool optional;
    generic_on_off_t target_on_off;
    generic_transition_time_t remaining_time;
} light_lc_client_light_on_off_status_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint32_t property_value;
} light_lc_client_property_status_t;
/** @} */


/**
 * @defgroup LIGHT_LC_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief register light lc server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lc server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_lc_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register light lc setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lc setup server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool light_lc_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish light lc mode
 * @param[in] pmodel_info: pointer to light lc server model context
 * @param[in] mode: light lc modeo need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_lc_mode_publish(const mesh_model_info_p pmodel_info,
                                            uint8_t mode);
/**
 * @brief publish light lc om
 * @param[in] pmodel_info: pointer to light lc server model context
 * @param[in] om: light lc om need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_lc_om_publish(const mesh_model_info_p pmodel_info,
                                          uint8_t om);

/**
 * @brief publish light lc light on/off
 * @param[in] pmodel_info: pointer to light lc server model context
 * @param[in] on_off: light on/off status need to publish
 * @return publish status
 */
mesh_msg_send_cause_t light_lc_light_on_off_publish(const mesh_model_info_p pmodel_info,
                                                    generic_on_off_t on_off);

/**
 * @brief publish light lc property
 * @param[in] pmodel_info: pointer to light lc setup server model context
 * @param[in] property_id: property id need to publish
 * @param[in] pproperty_value: property value need to publish
 * @param[in] len: property value length
 * @return publish status
 */
mesh_msg_send_cause_t light_lc_property_publish(const mesh_model_info_p pmodel_info,
                                                uint16_t property_id, const uint8_t *pproperty_value,
                                                uint16_t len);
/** @} */

/**
 * @defgroup LIGHT_LC_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief register light lc client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to light lc client model context
 * @retval true: register success
 * @retval false: register failed
 */
bool light_lc_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get light lc mode value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lc_mode_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index);

/**
 * @brief set light lc mode value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] mode: lc mode value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lc_mode_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index, uint8_t mode, bool ack);

/**
 * @brief get light lc om value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lc_om_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index);

/**
 * @brief set light lc om value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] mode: lc om value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lc_om_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index, uint8_t mode, bool ack);

/**
 * @brief get light lc light on off value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t light_lc_light_on_off_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index);

/**
 * @brief set light lc light on off value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] light_on_off: desired light on off value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lc_light_on_off_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint8_t light_on_off, uint8_t tid, bool optional,
                                                generic_transition_time_t trans_time, uint8_t delay, bool ack);

/**
 * @brief get light lc property value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @return send status
 */
mesh_msg_send_cause_t light_lc_property_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, uint16_t property_id);

/**
 * @brief set light lc property value
 * @param[in] pmodel_info: pointer to light lc client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @param[in] pvalue: property value
 * @param[in] value_len: property value length
 * @param[in] property_id: property id
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t light_lc_property_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, uint16_t property_id,
                                            uint8_t *pvalue, uint16_t value_len, bool ack);
/** @} */
/** @} */

END_DECLS


#endif /* _LIGHT_LC_H_ */
