/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_location.h
* @brief    Head file for generic location models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-25
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_LOCATION_H
#define _GENERIC_LOCATION_H

/* Add Includes here */
#include "platform_misc.h"
#include "mesh_api.h"

BEGIN_DELCS

/**
 * @addtogroup GENERIC_LOCATION
 * @{
 */

/**
 * @defgroup GENERIC_LOCATION_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_GET            0x8225
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_STAT           0x40
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_SET            0x41
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_SET_UNACK      0x42
#define MESH_MSG_GENERIC_LOCATION_LOCAL_GET             0x8226
#define MESH_MSG_GENERIC_LOCATION_LOCAL_STAT            0x8227
#define MESH_MSG_GENERIC_LOCATION_LOCAL_SET             0x8228
#define MESH_MSG_GENERIC_LOCATION_LOCAL_SET_UNACK       0x8229
/** @} */

/**
 * @defgroup GENERIC_LOCATION_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_LOCATION_SERVER              0x100EFFFF
#define MESH_MODEL_GENERIC_LOCATION_SETUP_SERVER        0x100FFFFF
#define MESH_MODEL_GENERIC_LOCATION_CLIENT              0x1010FFFF
/** @} */

/**
 * @defgroup GENERIC_LOCATION_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define GENERIC_LOCATION_ALTITUDE_NOT_CONFIGURED        0x7FFF
#define GENERIC_LOCATION_ALTITUDE_UPPER_LIMIT           0x7FFE //!< Altitude is greater than or equal to 32766 meters/decimeters.

#define GENERIC_LOCATION_FLOOR_NUM_LOWER_LIMIT          0x00 //!< Floor -20 or any floor below -20.
#define GENERIC_LOCATION_FLOOR_NUM_UPPER_LIMIT          0xFC //!< Floor 232 or any floor above 232.
#define GENERIC_LOCATION_FLOOR_NUM_GROUND_FLOOR0        0xFD //!< Ground floor. Floor 0.
#define GENERIC_LOCATION_FLOOR_NUM_GROUND_FLOOR1        0xFE //!< Ground floor. Floor 1.
#define GENERIC_LOCATION_FLOOR_NUM_NOT_CONFIGURED       0xFF

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_GET)];
} _PACKED_ generic_location_global_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_SET)];
    int32_t global_latitude;
    int32_t global_longitude;
    int16_t global_altitude;
} _PACKED_ generic_location_global_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_STAT)];
    int32_t global_latitude;
    int32_t global_longitude;
    int16_t global_altitude;
} _PACKED_ generic_location_global_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_GET)];
} _PACKED_ generic_location_local_get_t;

typedef struct
{
    uint16_t stationary: 1;
    uint16_t rfu: 7;
    uint16_t update_time: 4;
    uint16_t precision: 4;
} generic_location_uncertainty_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_SET)];
    int16_t local_north;
    int16_t local_east;
    int16_t local_altitude;
    uint8_t floor_num;
    generic_location_uncertainty_t uncertainty;
} _PACKED_ generic_location_local_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_STAT)];
    int16_t local_north;
    int16_t local_east;
    int16_t local_altitude;
    uint8_t floor_num;
    generic_location_uncertainty_t uncertainty;
} _PACKED_ generic_location_local_set_t;

/** @} */

/**
 * @defgroup GENERIC_LOCATION_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
void generic_power_level_server_reg(mesh_model_info_p pmodel_info,
                                    model_receive_pf pf_model_receive);
/** @} */

/**
 * @defgroup GENERIC_LOCATION_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
void generic_power_level_client_reg(mesh_model_info_p pmodel_info,
                                    model_receive_pf pf_model_receive);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_LOCATION_H */
