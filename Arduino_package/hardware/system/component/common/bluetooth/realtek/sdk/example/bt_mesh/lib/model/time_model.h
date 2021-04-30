/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     time_model.h
* @brief    Head file for time models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-22
* @version  v1.0
* *************************************************************************************
*/

#ifndef _TIME_MODEL_H_
#define _TIME_MODEL_H_

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup TIME
 * @{
 */

/**
 * @defgroup TIME_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_TIME_GET                               0x8237
#define MESH_MSG_TIME_SET                               0x5C
#define MESH_MSG_TIME_STATUS                            0x5D
#define MESH_MSG_TIME_ROLE_GET                          0x8238
#define MESH_MSG_TIME_ROLE_SET                          0x8239
#define MESH_MSG_TIME_ROLE_STATUS                       0x823A
#define MESH_MSG_TIME_ZONE_GET                          0x823B
#define MESH_MSG_TIME_ZONE_SET                          0x823C
#define MESH_MSG_TIME_ZONE_STATUS                       0x823D
#define MESH_MSG_TAI_UTC_DELTA_GET                      0x823E
#define MESH_MSG_TAI_UTC_DELTA_SET                      0x823F
#define MESH_MSG_TAI_UTC_DELTA_STATUS                   0x8240
/** @} */

/**
 * @defgroup TIME_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_TIME_SERVER                          0x1200FFFF
#define MESH_MODEL_TIME_SETUP_SERVER                    0x1201FFFF
#define MESH_MODEL_TIME_CLIENT                          0x1202FFFF
/** @} */

/**
 * @defgroup TIME_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define MAX_UNCERTAINTY    255

typedef struct
{
    uint8_t tai_seconds[5]; /* little endian */
    uint8_t subsecond;
    uint8_t uncertainty;
    uint16_t time_authority: 1;
    uint16_t tai_utc_delta: 15;
    uint8_t time_zone_offset;
} _PACKED_ tai_time_t;

typedef struct
{
    uint8_t time_zone_offset_current;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5]; /* little endian */
} _PACKED_ time_zone_t;

typedef struct
{
    uint16_t tai_utc_delta_current: 15;
    uint16_t padding1: 1; /* always 0*/
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding2: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} _PACKED_ tai_utc_delta_t;

enum
{
    TIME_ROLE_NONE,
    TIME_ROLE_AUTHORITY,
    TIME_ROLE_RELAY,
    TIME_ROLE_CLIENT
} _SHORT_ENUM_;
typedef uint8_t time_role_t;

#define IS_TIME_ROLE_VALID(role)   (role <= TIME_ROLE_CLIENT)

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_GET)];
} _PACKED_ time_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_SET)];
    uint8_t tai_seconds[5]; /* little endian */
    uint8_t subsecond;
    uint8_t uncertainty;
    uint16_t time_authority: 1;
    uint16_t tai_utc_delta: 15;
    uint8_t time_zone_offset;
} _PACKED_ time_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_STATUS)];
    uint8_t tai_seconds[5]; /* little endian */
    uint8_t subsecond;
    uint8_t uncertainty;
    uint16_t time_authority: 1;
    uint16_t tai_utc_delta: 15;
    uint8_t time_zone_offset;
} _PACKED_ time_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ZONE_GET)];
} _PACKED_ time_zone_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ZONE_SET)];
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5]; /* little endian */
} _PACKED_ time_zone_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ZONE_STATUS)];
    uint8_t time_zone_offset_current;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5]; /* little endian */
} _PACKED_ time_zone_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TAI_UTC_DELTA_GET)];
} _PACKED_ tai_utc_delta_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TAI_UTC_DELTA_SET)];
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} _PACKED_ tai_utc_delta_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TAI_UTC_DELTA_SET)];
    uint16_t tai_utc_delta_current: 15;
    uint16_t padding1: 1; /* always 0*/
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding2: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} _PACKED_ tai_utc_delta_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ROLE_GET)];
} _PACKED_ time_role_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ROLE_SET)];
    time_role_t role;
} _PACKED_ time_role_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_TIME_ROLE_STATUS)];
    time_role_t role;
} _PACKED_ time_role_status_t;
/** @} */

/**
 * @defgroup TIME_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define TIME_SERVER_GET                                        0 //!< @ref time_server_get_t
#define TIME_SERVER_GET_ROLE                                   1 //!< @ref time_server_get_role_t 
#define TIME_SERVER_GET_ZONE                                   2 //!< @ref time_server_get_zone_t 
#define TIME_SERVER_GET_TAI_UTC_DELTA                          3 //!< @ref time_server_get_tai_utc_delta_t 
#define TIME_SERVER_SET                                        4 //!< @ref time_server_set_t
#define TIME_SERVER_SET_ROLE                                   5 //!< @ref time_server_set_role_t
#define TIME_SERVER_STATUS_SET                                 6 //!< @ref time_server_status_set_t
#define TIME_SERVER_SET_ZONE                                   7 //!< @ref time_server_set_zone_t
#define TIME_SERVER_SET_TAI_UTC_DELTA                          8 //!< @ref time_server_set_tai_utc_delta_t

typedef tai_time_t               time_server_get_t;
typedef time_zone_t              time_server_get_zone_t;
typedef tai_utc_delta_t          time_server_get_tai_utc_delta_t;
typedef tai_time_t               time_server_set_t;

typedef struct
{
    time_role_t role;
} time_server_get_role_t;

typedef struct
{
    uint8_t tai_seconds[5]; /* little endian */
    uint8_t subsecond;
    uint8_t uncertainty;
    uint16_t tai_utc_delta;
    uint8_t time_zone_offset;
} _PACKED_ time_server_status_set_t;

typedef struct
{
    time_role_t role;
} time_server_set_role_t;

typedef struct
{
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5]; /* little endian */
} time_server_set_zone_t;

typedef struct
{
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} time_server_set_tai_utc_delta_t;
/** @} */

/**
 * @defgroup TIEM_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define TIME_CLIENT_STATUS                                     0 //!< @ref time_client_status_t
#define TIME_CLIENT_STATUS_ZONE                                1 //!< @ref time_client_status_zone_t
#define TIME_CLIENT_STATUS_TAI_UTC_DELTA                       2 //!< @ref time_client_status_tai_utc_delta_t
#define TIME_CLIENT_STATUS_ROLE                                3 //!< @ref time_client_status_role_t

typedef struct
{
    uint16_t src;
    tai_time_t tai_time;
} time_client_status_t;

typedef struct
{
    uint16_t src;
    uint8_t time_zone_offset_current;
    uint8_t time_zone_offset_new;
    uint8_t tai_of_zone_change[5];
} time_client_status_zone_t;

typedef struct
{
    uint16_t src;
    uint16_t tai_utc_delta_current: 15;
    uint16_t padding1: 1; /* always 0*/
    uint16_t tai_utc_delta_new: 15;
    uint16_t padding2: 1; /* always 0*/
    uint8_t tai_of_delta_change[5]; /* little endian */
} time_client_status_tai_utc_delta_t;

typedef struct
{
    time_role_t role;
} time_client_status_role_t;
/** @} */

/**
 * @defgroup TIME_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register time server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to time server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool time_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register time setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to time setup server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool time_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish time information
 * @param[in] pmodel_info: pointer to time server model context
 * @param[in] time: time information need to publish
 * @return publish status
 */
mesh_msg_send_cause_t time_publish(const mesh_model_info_p pmodel_info,
                                   tai_time_t time);

/**
 * @brief increate time uncertainty
 * @param[in] uncertainty: current time uncertainty
 * @param[in] delta: increase delta
 */
void increase_uncertainty(uint8_t *uncertainty, uint8_t delta);

/**
 * @brief convert tai utc delta unsigned value to signed value
 * @param[in] delta: unsigned delta value
 * @return singed delta value
 * @note tai utc delta valid range is -255 through +32512
 *       (i.e., 0x00FF represents a value of 0 and 0x7FFF
 *       represents a value of 32512).
 */
int16_t tai_utc_delta_convert(uint16_t delta);

/**
 * @brief convert time zone offset unsigned value to signed value
 * @param[in] time_zone_offset: unsigned time zone offset value
 * @return singed time zone offset value
 * @note time zone offset is a uint8 value representing the
 *       valid range of -64 through +191
 *       (i.e., 0x40 represents a value of 0 and 0xFF
 *       represents a value of 191).
 *       time zone offset is 15-minute increments
 */
int16_t time_zone_offset_convert(uint8_t time_zone_offset);

/** @} */

/**
 * @defgroup TIME_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register time client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to time client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool time_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get time information
 * @param[in] pmodel_info: pointer to time client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t time_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                               uint16_t app_key_index);

/**
 * @brief get time zone
 * @param[in] pmodel_info: pointer to time client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t time_get_zone(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);

/**
 * @brief get time tai utc delta
 * @param[in] pmodel_info: pointer to time client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t time_get_tai_utc_delta(const mesh_model_info_p pmodel_info,
                                             uint16_t dst,
                                             uint16_t app_key_index);

/**
 * @brief get time role
 * @param[in] pmodel_info: pointer to time client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t time_get_role(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index);

/**
 * @brief set time information
 * @param[in] pmodel_info: pointer to time information model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] time: time information
 * @return send status
 */
mesh_msg_send_cause_t time_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                               uint16_t app_key_index, tai_time_t time);

/**
 * @brief set time zone
 * @param[in] pmodel_info: pointer to time information model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] time_zone_offset_new: new time zone offset value
 * @param[in] tai_of_zone_change: tai zone change value
 * @return send status
 */
mesh_msg_send_cause_t time_set_zone(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index, uint8_t time_zone_offset_new,
                                    uint8_t tai_of_zone_change[5]);

/**
 * @brief set time information
 * @param[in] pmodel_info: pointer to time information model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] tai_utc_delta_new: new tai utc delta value
 * @param[in] tai_of_delta_change: tai delta change value
 * @return send status
 */
mesh_msg_send_cause_t time_set_tai_utc_delta(const mesh_model_info_p pmodel_info,
                                             uint16_t dst,
                                             uint16_t app_key_index, uint16_t tai_utc_delta_new,
                                             uint8_t tai_of_delta_change[5]);
/**
 * @brief set time information
 * @param[in] pmodel_info: pointer to time information model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] role: time role
 * @return send status
 */
mesh_msg_send_cause_t time_set_role(const mesh_model_info_p pmodel_info, uint16_t dst,
                                    uint16_t app_key_index, time_role_t role);

/** @} */
/** @} */

END_DECLS

#endif /* _TIME_MODEL_H_ */
