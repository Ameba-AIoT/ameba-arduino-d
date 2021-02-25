/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     private_beacon_model.h
* @brief    Head file for private beacon models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2020-08-04
* @version  v1.0
* *************************************************************************************
*/

#ifndef _PRIVATE_BEACON_MODEL_H_
#define _PRIVATE_BEACON_MODEL_H_


#include "mesh_api.h"

#if MESH_PRB

BEGIN_DECLS

/**
 * @addtogroup PRIVATE_BEACON
 * @{
 */

/**
 * @defgroup PRIVATE_BEACON_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_PRIVATE_BEACON_GET                         0xB711
#define MESH_MSG_PRIVATE_BEACON_SET                         0xB712
#define MESH_MSG_PRIVATE_BEACON_STATUS                      0xB713
#define MESH_MSG_PRIVATE_GATT_PROXY_GET                     0xB714
#define MESH_MSG_PRIVATE_GATT_PROXY_SET                     0xB715
#define MESH_MSG_PRIVATE_GATT_PROXY_STATUS                  0xB716
#define MESH_MSG_PRIVATE_NODE_IDENTITY_GET                  0xB718
#define MESH_MSG_PRIVATE_NODE_IDENTITY_SET                  0xB719
#define MESH_MSG_PRIVATE_NODE_IDENTITY_STATUS               0xB71A
/** @} */

/**
 * @defgroup PRIVATE_BEACON_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_PRIVATE_BEACON_SERVER                    0xBF40FFFF
#define MESH_MODEL_PRIVATE_BEACON_CLIENT                    0xBF41FFFF
/** @} */

/**
 * @defgroup PRIVATE_BEACON_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_BEACON_GET)];
} _PACKED_ private_beacon_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_BEACON_SET)];
    uint8_t private_beacon;
    uint8_t random_update_interval_steps;
} _PACKED_ private_beacon_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_BEACON_STATUS)];
    uint8_t private_beacon;
    uint8_t random_update_interval_steps;
} _PACKED_ private_beacon_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_GATT_PROXY_GET)];
} _PACKED_ private_gatt_proxy_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_GATT_PROXY_SET)];
    uint8_t private_gatt_proxy;
} _PACKED_ private_gatt_proxy_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_GATT_PROXY_STATUS)];
    uint8_t private_gatt_proxy;
} _PACKED_ private_gatt_proxy_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_NODE_IDENTITY_GET)];
    uint16_t net_key_index;
} _PACKED_ private_node_identity_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_NODE_IDENTITY_SET)];
    uint16_t net_key_index;
    uint8_t private_identity;
} _PACKED_ private_node_identity_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_PRIVATE_NODE_IDENTITY_STATUS)];
    uint8_t status;
    uint16_t net_key_index;
    uint8_t private_identity;
} _PACKED_ private_node_identity_status_t;



/** @} */

/**
 * @defgroup PRIVATE_BEACON_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register private beacon server model
 * @retval true: register success
 * @retval false: register failed
 */
bool private_beacon_server_reg(void);

/** @} */

/**
 * @defgroup PRIVATE_BEACON_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register private beacon client
 * @param[in] element_index: element index that model registered to
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool private_beacon_client_reg(uint8_t element_index);

/**
 * @brief private beacon get
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return message send status
 */
mesh_msg_send_cause_t private_beacon_get(uint16_t dst, uint16_t net_key_index);

/**
 * @brief private beacon set
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] private_beacon: private beacon value
 * @param[in] has_random_update: whether has random update or not
 * @param[in] random_update_interval_steps: private beacon random update interval steps
 * @return message send status
 */
mesh_msg_send_cause_t private_beacon_set(uint16_t dst, uint16_t net_key_index,
                                         uint8_t private_beacon, bool has_random_update, uint8_t random_update_interval_steps);

/**
 * @brief private gatt proxy get
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return message send status
 */
mesh_msg_send_cause_t private_gatt_proxy_get(uint16_t dst, uint16_t net_key_index);

/**
 * @brief private gatt proxy set
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] private_gatt_proxy: private gatt proxy value
 * @return message send status
 */
mesh_msg_send_cause_t private_gatt_proxy_set(uint16_t dst, uint16_t net_key_index,
                                             uint8_t private_gatt_proxy);

/**
 * @brief private node identitu get
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] sub_net_key_index: node identity network key index
 * @return message send status
 */
mesh_msg_send_cause_t private_node_identity_get(uint16_t dst, uint16_t net_key_index,
                                                uint16_t sub_net_key_index);

/**
 * @brief private node identity set
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] sub_net_key_index: node identity network key index
 * @param[in] private_identity: private node identity
 * @return message send status
 */
mesh_msg_send_cause_t private_node_identity_set(uint16_t dst, uint16_t net_key_index,
                                                uint16_t sub_net_key_index, uint8_t private_identity);

/** @} */
/** @} */

END_DECLS

#endif

#endif /* _PRIVATE_BEACON_H_ */
