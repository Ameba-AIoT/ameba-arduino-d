/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_power_on_off.h
* @brief    Head file for generic power on off models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_POWER_ON_OFF_H
#define _GENERIC_POWER_ON_OFF_H

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_ON_POWER_UP
 * @{
 */

/**
 * @defgroup GENERIC_ON_POWER_UP_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_ON_POWER_UP_GET                0x8211
#define MESH_MSG_GENERIC_ON_POWER_UP_STAT               0x8212
#define MESH_MSG_GENERIC_ON_POWER_UP_SET                0x8213
#define MESH_MSG_GENERIC_ON_POWER_UP_SET_UNACK          0x8214
/** @} */

/**
 * @defgroup GENERIC_ON_POWER_UP_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_POWER_ON_OFF_SERVER          0x1006FFFF
#define MESH_MODEL_GENERIC_POWER_ON_OFF_SETUP_SERVER    0x1007FFFF
#define MESH_MODEL_GENERIC_POWER_ON_OFF_CLIENT          0x1008FFFF
/** @} */

/**
 * @defgroup GENERIC_ON_POWER_UP_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
enum
{
    /* After being powered up, the element is in an off state. */
    GENERIC_ON_POWER_UP_OFF,
    /* After being powered up, the element is in an On state and uses default state values. */
    GENERIC_ON_POWER_UP_DEFAULT,
    /* If a transition was in progress when powered down, the element restores the target state when powered up.
       Otherwise the element restores the state it was in when powered down. */
    GENERIC_ON_POWER_UP_RESTORE
} _SHORT_ENUM_;
typedef uint8_t generic_on_power_up_t;

#define IS_GENERIC_ON_POWER_UP_VALID(power) ((power) <= GENERIC_ON_POWER_UP_RESTORE)

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_POWER_UP_GET)];
} _PACKED_ generic_on_power_up_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_POWER_UP_SET)];
    generic_on_power_up_t on_power_up;
} _PACKED_ generic_on_power_up_set_t, *generic_on_power_up_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_POWER_UP_STAT)];
    generic_on_power_up_t on_power_up;
} _PACKED_ generic_on_power_up_stat_t, *generic_on_power_up_stat_p;
/** @} */

/**
 * @defgroup GENERIC_POWER_ON_OFF_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_POWER_ON_OFF_SERVER_GET          0 //!< @ref generic_power_on_off_server_get_t
#define GENERIC_POWER_ON_OFF_SERVER_SET          1 //!< @ref generic_power_on_off_server_set_t

typedef struct
{
    generic_on_power_up_t on_power_up;
} generic_power_on_off_server_get_t;

typedef struct
{
    generic_on_power_up_t on_power_up;
} generic_power_on_off_server_set_t;
/** @} */

/**
 * @defgroup GENERIC_POWER_ON_OFF_CILENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_POWER_ON_OFF_CLIENT_STATUS        0 //!< @ref generic_power_on_off_client_status_t

typedef struct
{
    uint16_t src;
    generic_on_power_up_t on_power_up;
} generic_power_on_off_client_status_t;
/** @} */


/**
 * @defgroup GENERIC_POWER_ON_OFF_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic power on/off server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power on/off server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_on_off_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish on power up status
 * @param[in] pmodel_info: pointer to generic power on/off server model context
 * @param[in] on_power_up: on power up status
 * @return send status
 */
mesh_msg_send_cause_t generic_on_power_up_publish(const mesh_model_info_p pmodel_info,
                                                  generic_on_power_up_t on_power_up);

/**
 * @brief register generic power on/off setup server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power on/off setup model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_on_off_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);
/** @} */

/**
 * @defgroup GENERIC_POWER_ON_OFF_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic power on/off client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic power on/off client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_power_on_off_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic on power up status
 * @param[in] pmodel_info: pointer to generic power on off client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_on_power_up_get(const mesh_model_info_p pmodel_info,
                                              uint16_t dst,
                                              uint16_t app_key_index);
/**
 * @brief set generic on power up
 * @param[in] pmodel_info: pointer to generic power on off client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] on_power_up: desired on power up value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_on_power_up_set(const mesh_model_info_p pmodel_info,
                                              uint16_t dst,
                                              uint16_t app_key_index,
                                              generic_on_power_up_t on_power_up, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_POWER_ON_OFF_H */
