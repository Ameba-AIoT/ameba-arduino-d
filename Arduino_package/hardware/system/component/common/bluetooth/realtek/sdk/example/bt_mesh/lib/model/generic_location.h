/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_location.h
* @brief    Head file for generic location models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2019-06-10
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_LOCATION_H
#define _GENERIC_LOCATION_H

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

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
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_STATUS         0x40
#define MESH_MSG_GENERIC_LOCATION_LOCAL_GET             0x8226
#define MESH_MSG_GENERIC_LOCATION_LOCAL_STATUS          0x8227
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_SET            0x41
#define MESH_MSG_GENERIC_LOCATION_GLOBAL_SET_UNACK      0x42
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
#define GENERIC_LOCATION_GLOBAL_LATITUDE_NOT_CONFIGURED      0x80000000
#define GENERIC_LOCATION_GLOBAL_LONGITUDE_NOT_CONFIGURED     0x80000000

#define GENERIC_LOCATION_ALTITUDE_NOT_CONFIGURED             0x7FFF
#define GENERIC_LOCATION_ALTITUDE_UPPER_LIMIT                0x7FFE //!< Altitude is greater than or equal to 32766 meters/decimeters.

#define GENERIC_LOCATION_LOCAL_NORTH_NOT_CONFIGURED          0x8000
#define GENERIC_LOCATION_LOCAL_EAST_NOT_CONFIGURED           0x8000

#define GENERIC_LOCATION_FLOOR_NUM_LOWER_LIMIT               0x00 //!< Floor -20 or any floor below -20.
#define GENERIC_LOCATION_FLOOR_NUM_UPPER_LIMIT               0xFC //!< Floor 232 or any floor above 232.
#define GENERIC_LOCATION_FLOOR_NUM_GROUND_FLOOR0             0xFD //!< Ground floor. Floor 0.
#define GENERIC_LOCATION_FLOOR_NUM_GROUND_FLOOR1             0xFE //!< Ground floor. Floor 1.
#define GENERIC_LOCATION_FLOOR_NUM_NOT_CONFIGURED            0xFF

typedef struct
{
    int32_t global_latitude;
    int32_t global_longitude;
    int16_t global_altitude;
} _PACKED_ generic_location_global_t;

typedef union
{
    struct
    {
        uint16_t stationary: 1;
        uint16_t rfu: 7;
        uint16_t update_time: 4;
        uint16_t precision: 4;
    };
    uint16_t uncertainty;
} _PACKED_ generic_location_uncertainty_t;

typedef struct
{
    int16_t local_north;
    int16_t local_east;
    int16_t local_altitude;
    uint8_t floor_num;
    generic_location_uncertainty_t uncertainty;
} _PACKED_ generic_location_local_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_GET)];
} _PACKED_ generic_location_global_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_SET)];
    generic_location_global_t global;
} _PACKED_ generic_location_global_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_GLOBAL_STATUS)];
    generic_location_global_t global;
} _PACKED_ generic_location_global_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_GET)];
} _PACKED_ generic_location_local_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_SET)];
    generic_location_local_t local;
} _PACKED_ generic_location_local_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_LOCATION_LOCAL_STATUS)];
    generic_location_local_t local;
} _PACKED_ generic_location_local_status_t;

/** @} */

/**
 * @defgroup GENERIC_LOCATION_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_LOCATION_SERVER_GET_GLOBAL                                     0 //!< @ref generic_location_server_get_global_t
#define GENERIC_LOCATION_SERVER_GET_LOCAL                                      1 //!< @ref generic_location_server_get_local_t
#define GENERIC_LOCATION_SERVER_SET_GLOBAL                                     2 //!< @ref generic_location_server_set_global_t
#define GENERIC_LOCATION_SERVER_SET_LOCAL                                      3 //!< @ref generic_location_server_set_local_t

typedef generic_location_global_t generic_location_server_get_global_t;
typedef generic_location_local_t generic_location_server_get_local_t;
typedef generic_location_global_t generic_location_server_set_global_t;
typedef generic_location_local_t generic_location_server_set_local_t;
/** @} */

/**
 * @defgroup GENERIC_LOCATION_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_LOCATION_CLIENT_STATUS_GLOBAL                                  0 //!< @ref generic_location_client_status_global_t
#define GENERIC_LOCATION_CLIENT_STATUS_LOCAL                                   1 //!< @ref generic_location_client_status_local_t

typedef struct
{
    uint16_t src;
    generic_location_global_t global;
} generic_location_client_status_global_t;

typedef struct
{
    uint16_t src;
    generic_location_local_t local;
} generic_location_client_status_local_t;
/** @} */

/**
 * @defgroup GENERIC_LOCATION_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic location server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic location server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_location_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register generic location setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic location setup server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_location_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief convert latitude based on WGS84 to latitude x in the range [-90, 90]
 * @param[in] latitude_g: global latitude based on WGS84 datum
 * @return latitude x in the range [-90, 90]
 */
double generic_location_latitude_global_to_x(int32_t latitude_g);

/**
 * @brief convert latitude x in the range [-90, 90] to latitude based on WGS84
 * @param[in] latitude_x: global latitude x in the range [-90, 90]
 * @return latitude based on WGS84
 */
int32_t generic_location_latitude_x_to_global(double latitude_x);

/**
 * @brief convert longitude based on WGS84 to longitude x in the range [-180, 180]
 * @param[in] longitude_g: global longitude based on WGS84 datum
 * @return longitude x in the range [-180, 180]
 */
double generic_location_longitude_global_to_x(int32_t longitude_g);

/**
 * @brief convert longitude x in the range [-180, 180] to longitude based on WGS84
 * @param[in] longitude_x: global longitude x in the range [-180, 180]
 * @return longitude based on WGS84
 */
int32_t generic_location_longitude_x_to_global(double longitude_x);

/**
 * @brief convert update time to seconds
 * @param[in] update_time: update time need to be converted, 0-15
 * @return converted time, 0.125-4096 seconds
 */
double generic_location_update_time_to_seconds(uint8_t update_time);

/**
 * @brief convert seconds to update time
 * @param[in] seconds: seconds need to be converted, 0.125-4096 seconds
 * @return converted update time, 0-15
 */
uint8_t generic_location_seconds_to_update_time(double senonds);

/**
 * @brief convert precision to meters
 * @param[in] precision: precision need to be converted, 0-15
 * @return converted meters, 0.125-4096 meters
 */
double generic_location_precision_to_meters(uint8_t precision);

/**
 * @brief convert meters to precision
 * @param[in] meters: meters need to be converted, 0.125-4096 meters
 * @return converted precision, 0-15
 */
uint8_t generic_location_meters_to_precision(double meters);

/**
 * @brief publish location global value
 * @param[in] pmodel_info: pointer to generic location server model context
 * @param[in] global: location global value
 * @return publish status
 */
mesh_msg_send_cause_t generic_location_global_publish(const mesh_model_info_p pmodel_info,
                                                      generic_location_global_t global);

/**
 * @brief publish location local value
 * @param[in] pmodel_info: pointer to generic location server model context
 * @param[in] local: location local value
 * @return publish status
 */
mesh_msg_send_cause_t generic_location_local_publish(const mesh_model_info_p pmodel_info,
                                                     generic_location_local_t local);

/**
 * @brief enable or disable period publish
 * @param[in] global: enable or diable period publish location global
 * @param[in] local: enable or disable period publish location local
 */
void generic_location_period_pub_enable(bool global, bool local);
/** @} */

/**
 * @defgroup GENERIC_LOCATION_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic location client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic location client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool generic_location_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get localtion global value
 * @param[in] pmodel_info: pointer to generic location client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_location_global_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                  uint16_t app_key_index);

/**
 * @brief set localtion global value
 * @param[in] pmodel_info: pointer to generic location client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] global: desired location global value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_location_global_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                  uint16_t app_key_index, generic_location_global_t global,
                                                  bool ack);
/**
 * @brief get localtion local value
 * @param[in] pmodel_info: pointer to generic location client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_location_local_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index);

/**
 * @brief set localtion local value
 * @param[in] pmodel_info: pointer to generic location client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] local: desired location local value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_location_local_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index, generic_location_local_t local,
                                                 bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_LOCATION_H */
