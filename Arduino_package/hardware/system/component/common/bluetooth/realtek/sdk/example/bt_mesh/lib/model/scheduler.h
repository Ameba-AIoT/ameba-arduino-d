/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     scheduler.h
* @brief    Head file for scheduler models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-9-14
* @version  v1.0
* *************************************************************************************
*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup SCHEDULER
 * @{
 */

/**
 * @defgroup SCHEDULER_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_SCHEDULER_ACTION_GET                   0x8248
#define MESH_MSG_SCHEDULER_ACTION_STATUS                0x5F
#define MESH_MSG_SCHEDULER_GET                          0x8249
#define MESH_MSG_SCHEDULER_STATUS                       0x824A
#define MESH_MSG_SCHEDULER_ACTION_SET                   0x60
#define MESH_MSG_SCHEDULER_ACTION_SET_UNACK             0x61
/** @} */

/**
 * @defgroup SCHEDULER_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_SCHEDULER_SERVER                     0x1206FFFF
#define MESH_MODEL_SCHEDULER_SETUP_SERVER               0x1207FFFF
#define MESH_MODEL_SCHEDULER_CLIENT                     0x1208FFFF
/** @} */

/** @defgroup SCHEDULER_MESH_MSG Mesh Msg
  * @brief Mesh message types used by models
  * @{
  */

#define IS_SCHEDULER_INDEX_VALID(index)     ((index <= 15))

/* scheduler year */
#define SCHEDULER_EVERY_YEAR                0x64
#define IS_SCHEDULER_YEAR_VALIE(year)       ((year) <= 0x64)

/* scheduler month */
#define SCHEDULER_IN_JANUARY                (1 << 0)
#define SCHEDULER_IN_FEBRUARY               (1 << 1)
#define SCHEDULER_IN_MARCH                  (1 << 2)
#define SCHEDULER_IN_APRIL                  (1 << 3)
#define SCHEDULER_IN_MAY                    (1 << 4)
#define SCHEDULER_IN_JUNE                   (1 << 5)
#define SCHEDULER_IN_JULY                   (1 << 6)
#define SCHEDULER_IN_AUGUST                 (1 << 7)
#define SCHEDULER_IN_SETEMBER               (1 << 8)
#define SCHEDULER_IN_OCTOBER                (1 << 9)
#define SCHEDULER_IN_NOVEMBER               (1 << 10)
#define SCHEDULER_IN_DECEMBER               (1 << 11)
#define SCHEDULER_EVERY_MONTH               0x0FFF

/* scheduler day */
#define SCHEDULER_EVERY_DAY                 0x00

/* scheduler hour */
#define SCHEDULER_EVERY_HOUR                0x18
#define SCHEDULER_RANDOM_HOUR               0x19
#define IS_SCHEDULER_HOUR_VALID(hour)       ((hour) <= 0x19)

/* scheduler minute */
#define SCHEDULER_EVERY_MINUTE              0x3C
#define SCHEDULER_EVERY_15_MINUTE           0x3D
#define SCHEDULER_EVERY_20_MINUTE           0x3E
#define SCHEDULER_RADOM_MINUTE              0x3F

/* scheduler second */
#define SCHEDULER_EVERY_SECOND              0x3C
#define SCHEDULER_EVERY_15_SECOND           0x3D
#define SCHEDULER_EVERY_20_SECOND           0x3E
#define SCHEDULER_RANDOM_SECOND             0x3F

/* scheduler day of week */
#define SCHEDULER_ON_MONDAYS               (1 << 0)
#define SCHEDULER_ON_TUESDAYS              (1 << 1)
#define SCHEDULER_ON_WEDNESDAYS            (1 << 2)
#define SCHEDULER_ON_THURSDAYS             (1 << 3)
#define SCHEDULER_ON_FRIDAYS               (1 << 4)
#define SCHEDULER_ON_SATURDAYS             (1 << 5)
#define SCHEDULER_ON_SUNDAYS               (1 << 6)

/* scheduler action */
#define SCHEDULER_TURN_OFF                 0x00
#define SCHEDULER_TURN_ON                  0x01
#define SCHEDULER_SCENE_RECALL             0x02
#define SCHEDULER_NO_ACTION                0x0F

/* scheduler scene */
#define SCHEDULER_NO_SCENE                 0x00

typedef struct
{
    uint64_t index: 4;
    uint64_t year: 7;
    uint64_t month: 12;
    uint64_t day: 5;
    uint64_t hour: 5;
    uint64_t minute: 6;
    uint64_t second: 6;
    uint64_t day_of_week: 7;
    uint64_t action: 4;
    uint64_t num_steps: 6;
    uint64_t step_resolution: 2;
    uint16_t scene_number;
} _PACKED_ scheduler_register_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCHEDULER_GET)];
} _PACKED_ scheduler_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCHEDULER_STATUS)];
    uint16_t schedulers;
} _PACKED_ scheduler_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCHEDULER_ACTION_GET)];
    uint8_t index;
} _PACKED_ scheduler_action_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCHEDULER_ACTION_SET)];
    scheduler_register_t scheduler;
} _PACKED_ scheduler_action_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCHEDULER_ACTION_STATUS)];
    scheduler_register_t scheduler;
} _PACKED_ scheduler_action_status_t;

/** @} */


/**
 * @defgroup SCHEDULER_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SCHEDULER_SERVER_GET                                  0 //!< @ref scheduler_server_get_t
#define SCHEDULER_SERVER_GET_ACTION                           1 //!< @ref scheduler_server_get_action_t
#define SCHEDULER_SERVER_SET_ACTION                           2 //!< @ref scheduler_server_set_action_t

typedef struct
{
    uint16_t schedulers;
} scheduler_server_get_t;

typedef struct
{
    uint8_t index;
    scheduler_register_t scheduler;
} scheduler_server_get_action_t;

typedef scheduler_register_t scheduler_server_set_action_t;

/** @} */

/**
 * @defgroup SCHEDULER_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SCHEDULER_CLIENT_STATUS                                0 //!< @ref scheduler_client_status_t
#define SCHEDULER_CLIENT_STATUS_ACTION                         1 //!< @ref scheduler_client_status_action_t

typedef struct
{
    uint16_t src;
    uint16_t schedulers;
} scheduler_client_status_t;

typedef struct
{
    uint16_t src;
    scheduler_register_t scheduler;
} scheduler_client_status_action_t;

/** @} */

/**
 * @defgroup SCHEDULER_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register scheduler server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scheduler server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool scheduler_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register scheduler setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scheduler setup server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool scheduler_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish scene information
 * @param[in] pmodel_info: pointer to scheduler server model context
 * @param[in] scheduler: current scheduler
 * @return publish status
 */
mesh_msg_send_cause_t scheduler_publish(const mesh_model_info_p pmodel_info,
                                        scheduler_register_t scheduler);

/** @} */

/**
 * @defgroup SCHEDULER_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register scheduler client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scheduler client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool scheduler_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get scheduler status
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t scheduler_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);

/**
 * @brief get scheduler action
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] index: action index
 * @return send status
 */
mesh_msg_send_cause_t scheduler_action_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                           uint16_t app_key_index, uint8_t index);

/**
 * @brief set scheduler action
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] scheduler: scheduler need to be setted
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t scheduler_action_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                           uint16_t app_key_index, scheduler_register_t scheduler,
                                           bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _SCHEDULER_H_ */
