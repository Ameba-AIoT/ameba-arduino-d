/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     health.h
* @brief    Head file for health models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-3-10
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _HEALTH_H
#define _HEALTH_H

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup HEALTH
 * @{
 */

/**
 * @defgroup HEALTH_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_HEALTH_ATTN_GET                        0x8004
#define MESH_MSG_HEALTH_ATTN_SET                        0x8005
#define MESH_MSG_HEALTH_ATTN_SET_UNACK                  0x8006
#define MESH_MSG_HEALTH_ATTN_STAT                       0x8007
#define MESH_MSG_HEALTH_CURT_STAT                       0x04
#define MESH_MSG_HEALTH_FAULT_CLEAR                     0x802F
#define MESH_MSG_HEALTH_FAULT_CLEAR_UNACK               0x8030
#define MESH_MSG_HEALTH_FAULT_GET                       0x8031
#define MESH_MSG_HEALTH_FAULT_STAT                      0x05
#define MESH_MSG_HEALTH_FAULT_TEST                      0x8032
#define MESH_MSG_HEALTH_FAULT_TEST_UNACK                0x8033
#define MESH_MSG_HEALTH_PERIOD_GET                      0x8034
#define MESH_MSG_HEALTH_PERIOD_SET                      0x8035
#define MESH_MSG_HEALTH_PERIOD_SET_UNACK                0x8036
#define MESH_MSG_HEALTH_PERIOD_STAT                     0x8037
/** @} */

/**
 * @defgroup HEALTH_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_HEALTH_SERVER                        0x0002FFFF
#define MESH_MODEL_HEALTH_CLIENT                        0x0003FFFF
/** @} */

/**
 * @defgroup HEALTH_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */

/** standard test id */
#define HEALTH_STANDARD_TEST_ID                 0

/** health fault code */
#define HEALTH_FAULT_NO_FAULT                                     0x00
#define HEALTH_FAULT_BATTERY_LOW_WARNING                          0x01
#define HEALTH_FAULT_BATTERY_LOW_ERROR                            0x02
#define HEALTH_FAULT_SUPPLY_VOLTAGE_TOO_LOW_WARNING               0x03
#define HEALTH_FAULT_SUPPLY_VOLTAGE_TOO_LOW_ERROR                 0x04
#define HEALTH_FAULT_SUPPLY_VOLTAGE_TOO_HIGH_WARNING              0x05
#define HEALTH_FAULT_SUPPLY_VOLTAGE_TOO_HIGH_ERROR                0x06
#define HEALTH_FAULT_POWER_SUPPLY_INTERRUPTED_WARNING             0x07
#define HEALTH_FAULT_POWER_SUPPLY_INTERRUPTED_ERROR               0x08
#define HEALTH_FAULT_NO_LOAD_WARNING                              0x09
#define HEALTH_FAULT_NO_LOAD_ERROR                                0x0a
#define HEALTH_FAULT_OVERLOAD_WARNING                             0x0b
#define HEALTH_FAULT_OVERLOAD_ERROR                               0x0c
#define HEALTH_FAULT_OVERHEAT_WARNING                             0x0d
#define HEALTH_FAULT_OVERHEAT_ERROR                               0x0e
#define HEALTH_FAULT_CONDENSATION_WARNING                         0x0f
#define HEALTH_FAULT_CONDENSATION_ERROR                           0x10
#define HEALTH_FAULT_VIBRATION_WARNING                            0x11
#define HEALTH_FAULT_VIBRATION_ERROR                              0x12
#define HEALTH_FAULT_CONFIGURATION_WARNING                        0x13
#define HEALTH_FAULT_CONFIGURATION_ERROR                          0x14
#define HEALTH_FAULT_ELEMENT_NOT_CALIBRATED_WARNING               0x15
#define HEALTH_FAULT_ELEMENT_NOT_CALIBRATED_ERROR                 0x16
#define HEALTH_FAULT_MEMORY_WARNING                               0x17
#define HEALTH_FAULT_MEMORY_ERROR                                 0x18
#define HEALTH_FAULT_SELF_TEST_WARNING                            0x19
#define HEALTH_FAULT_SELF_TEST_ERROR                              0x1a
#define HEALTH_FAULT_INPUT_TOO_LOW_WARNING                        0x1b
#define HEALTH_FAULT_INPUT_TOO_LOW_ERROR                          0x1c
#define HEALTH_FAULT_INPUT_TOO_HIGH_WARNING                       0x1d
#define HEALTH_FAULT_INPUT_TOO_HIGH_ERROR                         0x1e
#define HEALTH_FAULT_INPUT_NO_CHANGE_HIGH_WARNING                 0x1f
#define HEALTH_FAULT_INPUT_NO_CHANGE_HIGH_ERROR                   0x20
#define HEALTH_FAULT_ACTUATOR_BLOCKED_WARNING                     0x21
#define HEALTH_FAULT_ACTUATOR_BLOCKED_ERROR                       0x22
#define HEALTH_FAULT_HOUSING_OPENED_WARNING                       0x23
#define HEALTH_FAULT_HOUSING_OPENED_ERROR                         0x24
#define HEALTH_FAULT_TAMPER_WARNING                               0x25
#define HEALTH_FAULT_TAMPER_ERROR                                 0x26
#define HEALTH_FAULT_DEVICE_MOVED_WARNING                         0x27
#define HEALTH_FAULT_DEVICE_MOVED_ERROR                           0x28
#define HEALTH_FAULT_DEVICE_DROPPED_WARNING                       0x29
#define HEALTH_FAULT_DEVICE_DROPPED_ERROR                         0x2a
#define HEALTH_FAULT_OVERFLOW_WARNING                             0x2b
#define HEALTH_FAULT_OVERFLOW_ERROR                               0x2c
#define HEALTH_FAULT_EMPTY_WARNING                                0x2d
#define HEALTH_FAULT_EMPTY_ERROR                                  0x2e
#define HEALTH_FAULT_INTERNAL_BUS_WARNING                         0x2f
#define HEALTH_FAULT_INTERNAL_BUS_ERROR                           0x30
#define HEALTH_FAULT_MECHANISM_JAMMED_WARNING                     0x31
#define HEALTH_FAULT_MECHAISM_JAMMED_ERROR                        0x32
#define HEALTH_FAULT_VENDOR_START                                 0x80

#define IS_FAST_PERIOD_DIVISOR_VALID(divisor)     ((divisor) <= 15)

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_CURT_STAT)];
    uint8_t test_id;
    uint16_t company_id;
    uint8_t fault_array[1]; //!< variable length
} _PACKED_ health_curt_stat_t, *health_curt_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_GET)];
    uint16_t company_id;
} _PACKED_ health_fault_get_t, *health_fault_get_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_CLEAR_UNACK)];
    uint16_t company_id;
} _PACKED_ health_fault_clear_unack_t, *health_fault_clear_unack_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_CLEAR)];
    uint16_t company_id;
} _PACKED_ health_fault_clear_t, *health_fault_clear_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_TEST)];
    uint8_t test_id;
    uint16_t company_id;
} _PACKED_ health_fault_test_t, *health_fault_test_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_TEST_UNACK)];
    uint8_t test_id;
    uint16_t company_id;
} _PACKED_ health_fault_test_unack_t, *health_fault_test_unack_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_FAULT_STAT)];
    uint8_t test_id;
    uint16_t company_id;
    uint8_t fault_array[1]; //!< variable length
} _PACKED_ health_fault_stat_t, *health_fault_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_PERIOD_GET)];
} _PACKED_ health_period_get_t, *health_period_get_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_PERIOD_SET_UNACK)];
    uint8_t fast_period_divisor;
} _PACKED_ health_period_set_unack_t, *health_period_set_unack_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_PERIOD_SET)];
    uint8_t fast_period_divisor;
} _PACKED_ health_period_set_t, *health_period_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_PERIOD_STAT)];
    uint8_t fast_period_divisor;
} _PACKED_ health_period_stat_t, *health_period_stat_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_ATTN_GET)];
} _PACKED_ health_attn_get_t, *health_attn_get_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_ATTN_SET)];
    uint8_t attn; //!< attention
} _PACKED_ health_attn_set_t, *health_attn_set_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_ATTN_SET_UNACK)];
    uint8_t attn; //!< attention
} _PACKED_ health_attn_set_unack_t, *health_attn_set_unack_p;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_HEALTH_ATTN_STAT)];
    uint8_t attn; //!< attention
} _PACKED_ health_attn_stat_t, *health_attn_stat_p;
/** @} */

/**
 * @defgroup HEALTH_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
typedef void (*health_server_test_cb_t)(const mesh_model_info_p pmodel_info, uint16_t company_id,
                                        uint8_t test_id);
typedef struct
{
    uint8_t test_id;
    health_server_test_cb_t test_cb;
} health_server_test_t;
/** @} */

/**
 * @defgroup HEALTH_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define HEALTH_CLIENT_STATUS_CURRENT                 0 //!< @ref health_client_status_t 
#define HEALTH_CLIENT_STATUS_REGISTERED              1 //!< @ref health_client_status_t 
#define HEALTH_CLIENT_STATUS_PERIOD                  2 //!< @ref health_client_status_period_t
#define HEALTH_CLIENT_STATUS_ATTENTION               3 //!< @ref health_client_status_attention_t

typedef struct
{
    uint16_t src;
    uint8_t test_id;
    uint16_t company_id;
    uint8_t *fault_array;
    uint8_t fault_array_len;
} health_client_status_t;

typedef struct
{
    uint8_t fast_period_divisor;
} health_client_status_period_t;

typedef struct
{
    uint8_t attention;
} health_client_status_attention_t;
/** @} */



/**
 * @defgroup HEALTH_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register health server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to health server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool health_server_reg(uint16_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief set selftest functions
 * @param[in] pmodel_info: pointer to health server model context
 * @param[in] ptests: selftest functions
 * @param[in] num_tests: selftest function numbers
 */
void health_server_set_tests(mesh_model_info_p pmodel_info, const health_server_test_t *ptests,
                             uint8_t num_tests);

/**
 * @brief set company id
 * @param[in] pmodel_info: pointer to health server model context
 * @param[in] company_id: new company id value
 */
void health_server_set_company_id(mesh_model_info_p pmodel_info, uint16_t company_id);

/**
 * @brief register fault to health server model
 * @param[in] pmodel_info: pointer to health server model context
 * @param[in] fault: fault need to register
 */
void health_server_fault_register(mesh_model_info_p pmodel_info, uint8_t fault);

/**
 * @brief clear fault in current fault array
 * @param[in] pmodel_info: pointer to health server model context
 * @param[in] fault: fault need to clear
 */
void health_server_fault_clear(mesh_model_info_p pmodel_info, uint8_t fault);

/**
 * @brief clear all fault in current fault array
 * @param[in] pmodel_info: pointer to health server model context
 */
void health_server_fault_clear_all(mesh_model_info_p pmodel_info);

/**
 * @brief check whether specified fault is in current fault array
 * @param[in] pmodel_info: pointer to health server model context
 * @param[in] fault: health fault value need to set
 * @retval TRUE: specified fault is in current array
 * @retval FALSE: specified fault is not in current array
 */
bool health_server_fault_is_set(const mesh_model_info_p pmodel_info, uint8_t fault);

/**
 * @brief get fault count in current fault array
 * @param[in] pmodel_info: pointer to health server model context
 */
uint8_t health_server_fault_count(const mesh_model_info_p pmodel_info);
/** @} */

/**
 * @defgroup HEALTH_CLIENT_API Client API
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
bool health_client_reg(uint16_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get fault status
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] company_id: company id
 * @return send status
 */
mesh_msg_send_cause_t health_fault_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                       uint16_t app_key_index, uint16_t company_id);

/**
 * @brief clear registered fault
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] company_id: company id value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t health_fault_clear(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t company_id,
                                         bool ack);

/**
 * @brief run specified selftest
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] test_id: test id value
 * @param[in] company_id: company id value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t health_fault_test(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index, uint8_t test_id,
                                        uint16_t company_id, bool ack);

/**
 * @brief get fast period divisor
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t health_period_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index);

/**
 * @brief set fast period divisor
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] fast_period_divisor: new fast period divisor
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t health_period_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                        uint16_t app_key_index,
                                        uint8_t fast_period_divisor, bool ack);
/**
 * @brief get attention value
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t health_attn_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index);

/**
 * @brief set attention value
 * @param[in] pmodel_info: pointer to health client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] attn: new attention value
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t health_attn_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index, uint8_t attn, bool ack);
/** @} */
/** @} */

END_DECLS

#endif /* _HEALTH_H */


