/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     sensor.h
* @brief    Head file for sensor models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-29
* @version  v1.0
* *************************************************************************************
*/

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup SENSOR
 * @{
 */

/**
 * @defgroup SENSOR_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_SENSOR_DESCRIPTOR_GET                    0x8230
#define MESH_MSG_SENSOR_DESCRIPTOR_STATUS                 0x51
#define MESH_MSG_SENSOR_GET                               0x8231
#define MESH_MSG_SENSOR_STATUS                            0x52
#define MESH_MSG_SENSOR_COLUMN_GET                        0x8232
#define MESH_MSG_SENSOR_COLUMN_STATUS                     0x53
#define MESH_MSG_SENSOR_SERIES_GET                        0x8233
#define MESH_MSG_SENSOR_SERIES_STATUS                     0x54
#define MESH_MSG_SENSOR_CADENCE_GET                       0x8234
#define MESH_MSG_SENSOR_CADENCE_SET                       0x55
#define MESH_MSG_SENSOR_CADENCE_SET_UNACK                 0x56
#define MESH_MSG_SENSOR_CADENCE_STATUS                    0x57
#define MESH_MSG_SENSOR_SETTINGS_GET                      0x8235
#define MESH_MSG_SENSOR_SETTINGS_STATUS                   0x58
#define MESH_MSG_SENSOR_SETTING_GET                       0x8236
#define MESH_MSG_SENSOR_SETTING_SET                       0x59
#define MESH_MSG_SENSOR_SETTING_SET_UNACK                 0x5A
#define MESH_MSG_SENSOR_SETTING_STATUS                    0x5B
/** @} */

/**
 * @defgroup SENSOR_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_SENSOR_SERVER                          0x1100FFFF
#define MESH_MODEL_SENSOR_SETUP_SERVER                    0x1101FFFF
#define MESH_MODEL_SENSOR_CLIENT                          0x1102FFFF
/** @} */

/**
 * @defgroup SENSOR_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define SENSOR_PROPERTY_ID_INVALID                        0
#define SENSOR_SETTING_PROPERTY_ID_INVALID                0
#define SENSOR_MEASUREMENT_PERIOD_NOT_APPLICABLE          0
#define IS_SENSOR_PROPERTY_ID_VALID(id)                   ((id) != SENSOR_PROPERTY_ID_INVALID)
#define IS_SENSOR_SETTING_PROPERTY_ID_VALID(id)           ((id) != SENSOR_SETTING_PROPERTY_ID_INVALID)
#define IS_FAST_CADENCE_DIVISIOR_VALID(divisor)           ((divisor) <= 15)
#define IS_STATUS_MIN_INTERVAL_VALID(interval)            ((interval) <= 26)

typedef enum
{
    SENSOR_SAMPLING_UNSPECIFIED,
    SENSOR_SAMPLING_INSTANTANEOUS,
    SENSOR_SAMPLING_ARITHMETIC_MEAN,
    SENSOR_SAMPLING_RMS,
    SENSOR_SAMPLING_MAXIMUM,
    SENSOR_SAMPLING_MINIMUM,
    SENSOR_SAMPLING_ACCUMULATED,
    SENSOR_SAMPLING_COUNT,
} sensor_sampling_function_t;

enum
{
    SENSOR_SETTING_ACCESS_READ_ONLY = 0x01,
    SENSOR_SETTING_ACCESS_READ_WRITE = 0x03,
} _SHORT_ENUM_;
typedef uint8_t sensor_setting_access_t;

typedef enum
{
    /**
     * trigger value format is the same as characterstic that
     * sensor property id state reference
     */
    SENSOR_TRIGGER_TYPE_CHARACTERISTIC,

    /**
     * trigger value is unitless, the format type is uint16_t,
     * and the value is represented as percentage change with
     * a resilution of 0.01 percent
     */
    SENSOR_TRIGGER_TYPE_UNITLESS,
} sensor_trigger_type_t;

typedef struct
{
    uint16_t property_id;
    uint32_t positive_tolerance: 12;
    uint32_t negative_tolerance: 12;
    uint32_t sampling_function: 8; /**< @ref sensor_sampling_function_t */
    uint8_t measurement_period;
    uint8_t update_interval;
} _PACKED_ sensor_descriptor_t;

typedef struct
{
    uint16_t setting_property_id;
    sensor_setting_access_t setting_access;
    uint8_t setting_raw_len;
    void *setting_raw;
} _PACKED_ sensor_setting_t;

typedef struct
{
    uint8_t raw_value_len;
    uint8_t fast_cadence_period_divisor: 7;
    uint8_t status_trigger_type: 1; /**< @ref sensor_trigger_type_t */
    void *status_trigger_delta_down;
    void *status_trigger_delta_up;
    uint8_t status_min_interval;
    void *fast_cadence_low;
    void *fast_cadence_high;
} _PACKED_ sensor_cadence_t;

typedef struct
{
    sensor_descriptor_t descriptor;
    uint8_t sensor_raw_data_len;
    uint16_t num_settings;
    sensor_setting_t *settings;
    sensor_cadence_t *cadence;
    int32_t pub_count;
} sensor_db_t;

typedef struct
{
    uint16_t property_id;
    uint8_t raw_data_len;
    void *raw_data;
} sensor_data_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_DESCRIPTOR_GET)];
    uint16_t property_id;
} _PACKED_ sensor_descriptor_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_DESCRIPTOR_STATUS)];
    uint8_t descriptors[0];
} _PACKED_ sensor_descriptor_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_GET)];
    uint16_t property_id;
} _PACKED_ sensor_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_STATUS)];
    uint8_t sensor_data[0];
} _PACKED_ sensor_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_COLUMN_GET)];
    uint16_t property_id;
    uint8_t raw_value_x[0];
} _PACKED_ sensor_column_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_COLUMN_STATUS)];
    uint16_t property_id;
    uint8_t column_raw_value[0];
} _PACKED_ sensor_column_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SERIES_GET)];
    uint16_t property_id;
    uint8_t raw_value_x[0];
} _PACKED_ sensor_series_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SERIES_STATUS)];
    uint16_t property_id;
    uint8_t series_raw_value[0];
} _PACKED_ sensor_series_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_CADENCE_GET)];
    uint16_t property_id;
} _PACKED_ sensor_cadence_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_CADENCE_SET)];
    uint16_t property_id;
    uint8_t fast_cadence_period_divisor: 7;
    uint8_t status_trigger_type: 1; /**< @ref sensor_trigger_type_t */
    uint8_t cadence[0];
} _PACKED_ sensor_cadence_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_CADENCE_STATUS)];
    uint16_t property_id;
    uint8_t cadence[0];
} _PACKED_ sensor_cadence_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SETTINGS_GET)];
    uint16_t property_id;
} _PACKED_ sensor_settings_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SETTINGS_STATUS)];
    uint16_t property_id;
    uint16_t setting_ids[0];
} _PACKED_ sensor_settings_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SETTING_GET)];
    uint16_t property_id;
    uint16_t setting_property_id;
} _PACKED_ sensor_setting_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SETTING_SET)];
    uint16_t property_id;
    uint16_t setting_property_id;
    uint8_t setting[0];
} _PACKED_ sensor_setting_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SENSOR_SETTING_STATUS)];
    uint16_t property_id;
    uint16_t setting_property_id;
    uint8_t setting[0];
} _PACKED_ sensor_setting_status_t;

/** @} */

/**
 * @defgroup SENSOR_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SENSOR_SERVER_GET                                      0 //!< @ref sensor_server_get_t
#define SENSOR_SERVER_GET_COLUMN                               1 //!< @ref sensor_server_get_column_t
#define SENSOR_SERVER_GET_SERIES                               2 //!< @ref sensor_server_get_series_t   
#define SENSOR_SERVER_SET_CADENCE                              3 //!< @ref sensor_server_set_cadence_t
#define SENSOR_SERVER_SET_SETTING                              4 //!< @ref sensor_server_set_setting_t
#define SENSOR_SERVER_COMPARE_CADENCE                          5 //!< @ref sensor_server_compare_cadence_t

typedef struct
{
    uint16_t property_id;
    /* app need to modify pointer blew, default is NULL */
    void *raw_data;
} sensor_server_get_t;

typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    const void *raw_value_x;
    /* app need to modify pointer blew, default is NULL */
    uint16_t column_len;
    /* contains raw value x, column width and raw value y */
    void *column;
} sensor_server_get_column_t;

typedef struct
{
    uint16_t property_id;
    uint8_t raw_value_x_len;
    const void *raw_value_x1;
    const void *raw_value_x2;
    /* app need to modify pointer blew, default is NULL */
    uint16_t series_len;
    /* contains n raw value x, column width and raw value y */
    void *series;
} sensor_server_get_series_t;

typedef struct
{
    uint16_t property_id;
    /*
     * just notify new cadence value in database,
     * do not need to modify it, just use it if needed
     */
    sensor_cadence_t *cadence;
} sensor_server_set_cadence_t;

typedef struct
{
    uint16_t property_id;
    /*
     * just notify new setting value in database,
     * do not need to modify it, just use it if needed
     */
    sensor_setting_t *setting;
} sensor_server_set_setting_t;

typedef struct
{
    uint16_t property_id;
    bool need_fast_divisor;
} sensor_server_compare_cadence_t;
/** @} */

/**
 * @defgroup SENSOR_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SENSOR_CLIENT_STATUS_DESCRIPTOR                        0 //!< @ref sensor_client_status_descriptor_t
#define SENSOR_CLIENT_STATUS_CADENCE                           1 //!< @ref sensor_client_status_cadence_t
#define SENSOR_CLIENT_STATUS_SETTINGS                          2 //!< @ref sensor_client_status_settings_t
#define SENSOR_CLIENT_STATUS_SETTING                           3 //!< @ref sensor_client_status_settings_t
#define SENSOR_CLIENT_STATUS                                   4 //!< @ref sensor_client_status_t
#define SENSOR_CLIENT_STATUS_COLUMN                            5 //!< @ref sensor_client_status_column_t
#define SENSOR_CLIENT_STATUS_SERIES                            6 //!< @ref sensor_client_status_series_t

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t num_descriptors;
    sensor_descriptor_t *descriptors;
} sensor_client_status_descriptor_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    sensor_cadence_t *cadence;
} sensor_client_status_cadence_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t num_ids;
    uint16_t *setting_ids;
} sensor_client_status_settings_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t setting_property_id;
    sensor_setting_t *setting;
} sensor_client_status_setting_t;

typedef struct
{
    uint16_t src;
    uint16_t marshalled_sensor_data_len;
    void *marshalled_sensor_data;
} sensor_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t column_raw_value_len;
    void *column_raw_value;
} sensor_client_status_column_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint16_t series_raw_value_len;
    void *series_raw_value;
} sensor_client_status_series_t;
/** @} */

/**
 * @defgroup SENSOR_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register sensor server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to sensor server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool sensor_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register sensor setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to sensor setup server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool sensor_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief set user defined sensor database
 * @param[in] pmodel_info: pointer to sensor server model context
 * @param[in] sensors: user defined sensor database
 * @param[in] num_sensors: sensor numbers
 */
void sensor_server_set_db(mesh_model_info_p pmodel_info, sensor_db_t *sensors,
                          uint16_t num_sensors);

/**
 * @brief set user defined sensor database
 * @param[in] pmodel_info: pointer to sensor setup server model context
 * @param[in] sensors: user defined sensor database
 * @param[in] num_sensors: sensor numbers
 */
void sensor_setup_server_set_db(mesh_model_info_p pmodel_info, sensor_db_t *sensors,
                                uint16_t num_sensors);

/**
 * @brief publish sensor information
 * @param[in] pmodel_info: pointer to sensor server model context
 * @param[in] sensor_data: sensor data need to publish
 * @param[in] num_sensor_data: sensor data count
 * @return publish status
 */
mesh_msg_send_cause_t sensor_publish(const mesh_model_info_p pmodel_info,
                                     const sensor_data_t *sensor_data,
                                     uint16_t num_sensor_data);

/**
 * @brief publish sensor cadence information
 * @param[in] pmodel_info: pointer to sensor setup server model context
 * @param[in] property_id: sensor property id
 * @param[in] cadence: sensor cadence need to publish
 * @return publish status
 */
mesh_msg_send_cause_t sensor_cadence_publish(const mesh_model_info_p pmodel_info,
                                             uint16_t property_id,
                                             const sensor_cadence_t *cadence);

/**
 * @brief publish sensor setting information
 * @param[in] pmodel_info: pointer to sensor setup server model context
 * @param[in] property_id: sensor property id
 * @param[in] setting: sensor setting need to publish
 * @return publish status
 */
mesh_msg_send_cause_t sensor_setting_publish(const mesh_model_info_p pmodel_info,
                                             uint16_t property_id,
                                             const sensor_setting_t *setting);

/**
 * @brief convert sensor tolerance value to percentage
 * @param[in] tolerance: sensor tolerance digital value
 * @return sensor tolerance percentage value
 */
uint8_t sensor_tolerance_to_percentage(uint16_t tolerance);

/**
 * @brief convert measurement period to seconds
 * @param[in] period: measurement period
 * @return measurement period in seconds
 */
double sensor_measurement_period_to_seconds(uint8_t period);

/**
 * @brief convert update interval to seconds
 * @param[in] period: sensor update interval
 * @return sensor update interval in seconds
 */
double sensor_update_interval_to_seconds(uint8_t period);
/** @} */

/**
 * @defgroup SENSOR_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register sensor client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to sensor client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool sensor_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get sensor descriptor
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @return send status
 */
mesh_msg_send_cause_t sensor_descriptor_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index, uint16_t property_id);

/**
 * @brief get sensor cadence
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @return send status
 */
mesh_msg_send_cause_t sensor_cadence_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t property_id);

/**
 * @brief get sensor cadence
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @param[in] cadence: sensor cadence data
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t sensor_cadence_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t property_id, const sensor_cadence_t *cadence, bool ack);

/**
 * @brief get sensor settings
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @return send status
 */
mesh_msg_send_cause_t sensor_settings_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                          uint16_t app_key_index, uint16_t property_id);

/**
 * @brief get sensor settings
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @param[in] setting_property_id: sensor setting property id
 * @return send status
 */
mesh_msg_send_cause_t sensor_setting_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t property_id, uint16_t setting_property_id);

/**
 * @brief set sensor settings
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @param[in] setting_property_id: sensor setting property id
 * @param[in] setting_raw_len: setting raw data length
 * @param[in] setting_raw: sensor setting raw data
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t sensor_setting_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t property_id, uint16_t setting_property_id,
                                         uint8_t setting_raw_len, const void *setting_raw, bool ack);

/**
 * @brief get sensor data
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @return send status
 */
mesh_msg_send_cause_t sensor_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                 uint16_t app_key_index, uint16_t property_id);

/**
 * @brief get sensor column data
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @param[in] raw_value_x_len: column raw value x length
 * @param[in] raw_value_x: column raw value x
 * @return send status
 */
mesh_msg_send_cause_t sensor_column_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index, uint16_t property_id, uint8_t raw_value_x_len, const void *raw_value_x);

/**
 * @brief get sensor series data
 * @param[in] pmodel_info: pointer to sensor client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: sensor property id
 * @param[in] raw_value_x_len: column raw value x length
 * @param[in] raw_value_x1: column raw value x1
 * @param[in] raw_value_x2: column raw value x2
 * @return send status
 */
mesh_msg_send_cause_t sensor_series_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index, uint16_t property_id, uint8_t raw_value_x_len, const void *raw_value_x1,
                                        const void *raw_value_x2);

/** @} */
/** @} */

END_DECLS

#endif /* _SENSOR_H_ */
