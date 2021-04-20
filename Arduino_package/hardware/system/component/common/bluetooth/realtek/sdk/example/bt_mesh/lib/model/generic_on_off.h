/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_on_off.h
* @brief    Head file for generic on off models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-22
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_ON_OFF_H
#define _GENERIC_ON_OFF_H

/* Add Includes here */
#include "generic_types.h"
#include "mesh_api.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_ON_OFF
 * @{
 */

/**
 * @defgroup GENERIC_ON_OFF_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_ON_OFF_GET                     0x8201
#define MESH_MSG_GENERIC_ON_OFF_SET                     0x8202
#define MESH_MSG_GENERIC_ON_OFF_SET_UNACK               0x8203
#define MESH_MSG_GENERIC_ON_OFF_STAT                    0x8204
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_ON_OFF_SERVER                0x1000FFFF
#define MESH_MODEL_GENERIC_ON_OFF_CLIENT                0x1001FFFF
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
enum
{
    GENERIC_OFF,
    GENERIC_ON
} _SHORT_ENUM_;
typedef uint8_t generic_on_off_t;

#define IS_GENERIC_ON_OFF_VALID(val)   (val <= GENERIC_ON)

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_OFF_GET)];
} _PACKED_ generic_on_off_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_OFF_SET)];
    generic_on_off_t on_off;
    uint8_t tid;
    generic_transition_time_t trans_time; //!< optional
    /* If the Transition Time field is present, the Delay field shall also be present;
       otherwise these fields shall not be present. */
    uint8_t delay; //!< Message execution delay in 5 millisecond steps
} _PACKED_ generic_on_off_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ON_OFF_STAT)];
    generic_on_off_t present_on_off;
    generic_on_off_t target_on_off; //!< optional
    /* If the Target OnOff field is present, the Remaining Time field shall also be present;
       otherwise these fields shall not be present. */
    generic_transition_time_t remaining_time;
} _PACKED_ generic_on_off_stat_t, *generic_on_off_stat_p;
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_ON_OFF           0
#define DELAY_EXECUTION_TYPE_ON_OFF              0
/** @} */


/**
 * @defgroup GENERIC_ON_OFF_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_ON_OFF_SERVER_GET                                        0 //!< @ref generic_on_off_server_get_t
#define GENERIC_ON_OFF_SERVER_GET_DEFAULT_TRANSITION_TIME                1 //!< @ref generic_on_off_server_get_default_transition_time_t
#define GENERIC_ON_OFF_SERVER_SET                                        2 //!< @ref generic_on_off_server_set_t

typedef struct
{
    generic_on_off_t on_off;
} generic_on_off_server_get_t;

typedef struct
{
    generic_transition_time_t trans_time;
} generic_on_off_server_get_default_transition_time_t;

typedef struct
{
    generic_on_off_t on_off;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} generic_on_off_server_set_t;
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_ON_OFF_CLIENT_STATUS   0 //!< @ref generic_on_off_client_status_t

typedef struct
{
    uint16_t src;
    generic_on_off_t present_on_off;
    bool optional;
    generic_on_off_t target_on_off;
    generic_transition_time_t remaining_time;
} generic_on_off_client_status_t;
/** @} */

/**
 * @defgroup GENERIC_ON_OFF_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic on off server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic on off server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_on_off_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic on off
 * @param[in] pmodel_info: pointer to generic on off server model context
 * @param[in] on_off: on off status need to publish
 * @return publish status
 */
mesh_msg_send_cause_t generic_on_off_publish(const mesh_model_info_p pmodel_info,
                                             generic_on_off_t on_off);

/** @} */

/**
 * @defgroup GENERIC_ON_OFF_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic on off client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic on off client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_on_off_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic on off status
 * @param[in] pmodel_info: pointer to generic on off client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_on_off_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index);

/**
 * @brief set generic on off
 * @param[in] pmodel_info: pointer to generic on off model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] trans_time: new transition time
 * @param[in] on_off: desired on/off value
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_on_off_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index,
                                         generic_on_off_t on_off, uint8_t tid, bool optional,
                                         generic_transition_time_t trans_time, uint8_t delay, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_ON_OFF_H */
