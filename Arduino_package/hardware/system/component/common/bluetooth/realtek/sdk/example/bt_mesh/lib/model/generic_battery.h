/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_battery.h
* @brief    Head file for generic battery models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_BATTERY_H
#define _GENERIC_BATTERY_H

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_BATTERY
 * @{
 */

/**
 * @defgroup GENERIC_BATTERY_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_BATTERY_GET                    0x8223
#define MESH_MSG_GENERIC_BATTERY_STAT                   0x8224
/** @} */

/**
 * @defgroup GENERIC_BATTERY_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_BATTERY_SERVER               0x100CFFFF
#define MESH_MODEL_GENERIC_BATTERY_CLIENT               0x100DFFFF
/** @} */

/**
 * @defgroup GENERIC_BATTERY_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define GENERIC_BATTERY_TIME_UNKNOWN                    0xFFFFFF

#define GENERIC_BATTERY_LEVEL_100 = 0x64, //!< 0x65-0xFE Prohibited
#define GENERIC_BATTERY_LEVEL_UNKNOWN = 0xFF

typedef enum
{
    GENERIC_BATTERY_FLAGS_NOT_PRESENT,
    GENERIC_BATTERY_FLAGS_PRESENT_AND_REMOVABLE,
    GENERIC_BATTERY_FLAGS_PRESENT_AND_NON_REMOVABLE,
    GENERIC_BATTERY_FLAGS_PRESENCE_UNKNOWN
} generic_battery_flags_presence_t;

typedef enum
{
    GENERIC_BATTERY_FLAGS_CHARGE_CRITICALLY_LOW_LEVEL,
    GENERIC_BATTERY_FLAGS_CHARGE_LOW_LEVEL,
    GENERIC_BATTERY_FLAGS_CHARGE_GOOD_LEVEL,
    GENERIC_BATTERY_FLAGS_CHARGE_UNKNOWN
} generic_battery_flags_indicator_t;

typedef enum
{
    GENERIC_BATTERY_FLAGS_NOT_CHARGEABLE,
    GENERIC_BATTERY_FLAGS_CHARGEABLE_AND_NOT_CHARGING,
    GENERIC_BATTERY_FLAGS_CHARGEABLE_AND_CHARGING,
    GENERIC_BATTERY_FLAGS_CHARGING_STATE_UNKNOWN
} generic_battery_flags_charging_t;

typedef enum
{
    GENERIC_BATTERY_FLAGS_NOT_REQUIRE_SERVICE = 0x1,
    GENERIC_BATTERY_FLAGS_REQUIRE_SERVICE,
    GENERIC_BATTERY_FLAGS_SERVICEABILITY_UNKNOWN
} generic_battery_flags_serviceability_t;

typedef struct
{
    uint8_t presence: 2;
    uint8_t indicator: 2;
    uint8_t charging: 2;
    uint8_t serviceability: 2;
} generic_battery_flags_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_BATTERY_GET)];
} generic_battery_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_BATTERY_STAT)];
    uint8_t battery_level; //!< @ref GENERIC_BATTERY_LEVEL_100
    uint8_t time_to_discharge[3]; //!< @ref GENERIC_BATTERY_TIME_UNKNOWN
    uint8_t time_to_charge[3]; //!< @ref GENERIC_BATTERY_TIME_UNKNOWN
    generic_battery_flags_t flags;
} _PACKED_ generic_battery_stat_t, *generic_battery_stat_p;
/** @} */

/**
 * @defgroup GENERIC_BATTERY_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_BATTERY_SERVER_GET                       0 //!< @ref generic_battery_server_get_t

typedef struct
{
    uint8_t battery_level;
    uint32_t time_to_discharge;
    uint32_t time_to_charge;
    generic_battery_flags_t flags;
} generic_battery_server_get_t;
/** @} */

/**
 * @defgroup GENERIC_BATTERY_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_BATTERY_CLIENT_STATUS                    0 //!< @ref generic_battery_client_status_t

typedef struct
{
    uint16_t src;
    uint8_t battery_level;
    uint32_t time_to_discharge;
    uint32_t time_to_charge;
    generic_battery_flags_t flags;
} generic_battery_client_status_t;
/** @} */

/**
 * @defgroup GENERIC_BATTERY_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic battery server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic battery server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_battery_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic battery status
 * @param[in] pmodel_info: pointer to generic battery server model context
 * @param[in] battery_level: battery level status need to publish
 * @param[in] time_to_discharge: battery discharge time
 * @param[in] time_to_charge: battery charge time
 * @param[in] flags: battery flags
 * @return publish status
 */
mesh_msg_send_cause_t generic_battery_publish(const mesh_model_info_p pmodel_info,
                                              uint8_t battery_level,
                                              uint32_t time_to_discharge, uint32_t time_to_charge,
                                              generic_battery_flags_t flags);
/** @} */

/**
 * @defgroup GENERIC_BATTERY_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic battery client model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic battery client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_battery_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic battery status
 * @param[in] pmodel_info: pointer to generic battery client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_battery_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_BATTERY_H */
