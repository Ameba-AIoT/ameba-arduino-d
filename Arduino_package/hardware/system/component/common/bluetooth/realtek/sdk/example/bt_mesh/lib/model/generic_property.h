/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_property.h
* @brief    Head file for generic property models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2019-06-28
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_PROPERTY_H
#define _GENERIC_PROPERTY_H

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_PROPERTY
 * @{
 */

/**
 * @defgroup GENERIC_PROPERTY_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTIES_GET        0x822A
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTIES_STATUS     0x43
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_GET          0x822B
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_SET          0x44
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_SET_UNACK    0x45
#define MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_STATUS       0x46

#define MESH_MSG_GENERIC_ADMIN_PROPERTIES_GET               0x822C
#define MESH_MSG_GENERIC_ADMIN_PROPERTIES_STATUS            0x47
#define MESH_MSG_GENERIC_ADMIN_PROPERTY_GET                 0x822D
#define MESH_MSG_GENERIC_ADMIN_PROPERTY_SET                 0x48
#define MESH_MSG_GENERIC_ADMIN_PROPERTY_SET_UNACK           0x49
#define MESH_MSG_GENERIC_ADMIN_PROPERTY_STATUS              0x4A

#define MESH_MSG_GENERIC_USER_PROPERTIES_GET                0x822E
#define MESH_MSG_GENERIC_USER_PROPERTIES_STATUS             0x4B
#define MESH_MSG_GENERIC_USER_PROPERTY_GET                  0x822F
#define MESH_MSG_GENERIC_USER_PROPERTY_SET                  0x4C
#define MESH_MSG_GENERIC_USER_PROPERTY_SET_UNACK            0x4D
#define MESH_MSG_GENERIC_USER_PROPERTY_STATUS               0x4E

#define MESH_MSG_GENERIC_CLIENT_PROPERTIES_GET              0x4F
#define MESH_MSG_GENERIC_CLIENT_PROPERTIES_STATUS           0x50
/** @} */

/**
 * @defgroup GENERIC_PROPERTY_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_GENERIC_ADMIN_PROPERTY_SERVER            0x1011FFFF
#define MESH_MODEL_GENERIC_MANUFACTURER_PROPERTY_SERVER     0x1012FFFF
#define MESH_MODEL_GENERIC_USER_PROPERTY_SERVER             0x1013FFFF
#define MESH_MODEL_GENERIC_CLIENT_PROPERTY_SERVER           0x1014FFFF
#define MESH_MODEL_GENERIC_PROPERTY_CLIENT                  0x1015FFFF
/** @} */

/**
 * @defgroup GENERIC_PROPERTY_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define GENERIC_PROPERTY_ID_PROHIBITED                      0x00

#define GENERIC_PROPERTY_ACCESS_NOT_TO_USER                 0
#define GENERIC_PROPERTY_ACCESS_READ                        1
#define GENERIC_PROPERTY_ACCESS_WRITE                       2
#define GENERIC_PROPERTY_ACCESS_READ_AND_WRITE              3


typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_USER_PROPERTIES_GET)];
} _PACKED_ generic_user_properties_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_USER_PROPERTIES_STATUS)];
    uint16_t property_ids[0]; //!< variable length
} _PACKED_ generic_user_properties_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_USER_PROPERTY_GET)];
    uint16_t property_id;
} _PACKED_ generic_user_property_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_USER_PROPERTY_SET)];
    uint16_t property_id;
    uint8_t property_value[0]; //!< variable length
} _PACKED_ generic_user_property_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_USER_PROPERTY_STATUS)];
    uint16_t property_id;
    uint8_t property_access;
    uint8_t property_value[0]; //!< variable length
} _PACKED_ generic_user_property_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ADMIN_PROPERTIES_GET)];
} _PACKED_ generic_admin_properties_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ADMIN_PROPERTIES_STATUS)];
    uint16_t property_ids[0]; //!< variable length
} _PACKED_ generic_admin_properties_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ADMIN_PROPERTY_GET)];
    uint16_t property_id;
} _PACKED_ generic_admin_property_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ADMIN_PROPERTY_SET)];
    uint16_t property_id;
    uint8_t property_access;
    uint8_t property_value[0]; //!< variable length
} _PACKED_ generic_admin_property_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_ADMIN_PROPERTY_STATUS)];
    uint16_t property_id;
    uint8_t property_access;
    uint8_t property_value[0]; //!< variable length
} _PACKED_ generic_admin_property_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MANUFACTURER_PROPERTIES_GET)];
} _PACKED_ generic_manufacturer_properties_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MANUFACTURER_PROPERTIES_STATUS)];
    uint16_t property_ids[0]; //!< variable length
} _PACKED_ generic_manufacturer_properties_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_GET)];
    uint16_t property_id;
} _PACKED_ generic_manufacturer_property_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_SET)];
    uint16_t property_id;
    uint8_t property_access;
} _PACKED_ generic_manufacturer_property_set_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_MANUFACTURER_PROPERTY_STATUS)];
    uint16_t property_id;
    uint8_t property_access;
    uint8_t property_value[0]; //!< variable length
} _PACKED_ generic_manufacturer_property_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_CLIENT_PROPERTIES_GET)];
    uint16_t property_id; //!< A starting Client Property ID present within an element
} _PACKED_ generic_client_properties_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_GENERIC_CLIENT_PROPERTIES_STATUS)];
    uint16_t property_ids[0]; //!< variable length
} _PACKED_ generic_client_properties_status_t;

typedef struct
{
    uint16_t property_id;
    uint8_t property_access;
    uint8_t *pproperty_value; //!< variable length
    uint16_t value_len;
} generic_property_db_t;

/** @} */

/**
 * @defgroup GENERIC_PROPERTY_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_USER_PROPERTY_SERVER_SET                                        0 //!< @ref generic_user_property_server_set_t
#define GENERIC_ADMIN_PROPERTY_SERVER_SET                                       1 //!< @ref generic_admin_property_server_set_t
#define GENERIC_MANUFACTURER_PROPERTY_SERVER_SET                                2 //!< @ref generic_manufacturer_property_server_set_t
#define GENERIC_CLIENT_PROPERTY_SERVER_GET                                      3 //!< @ref generic_client_property_server_get_t

typedef struct
{
    uint16_t property_id;
    /*
     * just notify property value changed in database,
     * do not need to modify it, just use it if needed
     */
    uint8_t *pproperty_value;
    uint16_t value_len;
} generic_user_property_server_set_t;

typedef struct
{
    uint16_t property_id;
    /*
     * just notify property value changed in database,
     * do not need to modify it, just use it if needed
     */
    uint8_t property_access;
    uint8_t *pproperty_value;
    uint16_t value_len;
} generic_admin_property_server_set_t;

typedef struct
{
    uint16_t property_id;
    /*
     * just notify property access changed in database,
     * do not need to modify it, just use it if needed
     */
    uint8_t property_access;
} generic_manufacturer_property_server_set_t;

typedef struct
{
    uint16_t property_id_start;
    /*
     * app modify pproperty_ids and num_ids field
     */
    const uint16_t *pproperty_ids;
    uint8_t num_ids;
} generic_client_property_server_get_t;
/** @} */

/**
 * @defgroup GENERIC_PROPERTY_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define GENERIC_USER_PROPERIES_CLIENT_STATUS                                    0 //!< @ref generic_properties_client_status_t
#define GENERIC_USER_PROPERTY_CLIENT_STATUS                                     1 //!< @ref generic_property_client_status_t
#define GENERIC_ADMIN_PROPERTIES_CLIENT_STATUS                                  2 //!< @ref generic_properties_client_status_t
#define GENERIC_ADMIN_PROPERTY_CLIENT_STATUS                                    3 //!< @ref generic_property_client_status_t
#define GENERIC_MANUFACTURER_PROPERTIES_CLIENT_STATUS                           4 //!< @ref generic_properties_client_status_t
#define GENERIC_MANUFACTURER_PROPERTY_CLIENT_STATUS                             5 //!< @ref generic_property_client_status_t
#define GENERIC_CLIENT_PROPERTIES_CLIENT_STATUS                                 6 //!< @ref generic_properties_client_status_t

typedef struct
{
    uint16_t src;
    const uint16_t *pproperty_ids;
    uint8_t num_ids;
} generic_properties_client_status_t;

typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint8_t property_access;
    uint8_t *pproperty_value;
    uint16_t value_len;
} generic_property_client_status_t;
/** @} */

/**
 * @defgroup GENERIC_PROPERTY_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic user property server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic user property server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool generic_user_property_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic user property value
 * @param[in] pmodel_info: pointer to generic user property server model context
 * @param[in] pdb: generic user property
 * @return publish status
 */
mesh_msg_send_cause_t generic_user_property_publish(const mesh_model_info_p pmodel_info,
                                                    const generic_property_db_t *pdb);

/**
 * @brief set user defined generic user property database
 * @param[in] pmodel_info: pointer to generic user property server model context
 * @param[in] pproperties: user defined generic user property database
 * @param[in] num_properties: generic user property numbers
 */
void generic_user_property_server_set_db(mesh_model_info_p pmodel_info,
                                         generic_property_db_t *pproperties, uint16_t num_properties);

/**
 * @brief register generic admin property server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic admin property server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool generic_admin_property_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic admin property value
 * @param[in] pmodel_info: pointer to generic admin property server model context
 * @param[in] pdb: generic admin property
 * @return publish status
 */
mesh_msg_send_cause_t generic_admin_property_publish(const mesh_model_info_p pmodel_info,
                                                     const generic_property_db_t *pdb);

/**
 * @brief set user defined generic admin property database
 * @param[in] pmodel_info: pointer to generic admin property server model context
 * @param[in] pproperties: user defined generic admin property database
 * @param[in] num_properties: generic admin property numbers
 */
void generic_admin_property_server_set_db(mesh_model_info_p pmodel_info,
                                          generic_property_db_t *pproperties, uint16_t num_properties);

/**
 * @brief register generic manufacturer property server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic manufacturer property server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool generic_manufacturer_property_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish generic manufacturer property value
 * @param[in] pmodel_info: pointer to generic manufacturer property server model context
 * @param[in] pdb: generic admin property
 * @return publish status
 */
mesh_msg_send_cause_t generic_manufacturer_property_publish(const mesh_model_info_p pmodel_info,
                                                            const generic_property_db_t *pdb);

/**
 * @brief set user defined generic manufacturer property database
 * @param[in] pmodel_info: pointer to generic manufacturer property server model context
 * @param[in] pproperties: user defined generic manufacturer property database
 * @param[in] num_properties: generic admin property numbers
 */
void generic_manufacturer_property_server_set_db(mesh_model_info_p pmodel_info,
                                                 generic_property_db_t *pproperties, uint16_t num_properties);

/**
 * @brief register generic client property server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic client property server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool generic_client_property_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/** @} */

/**
 * @defgroup GENERIC_PROPERTY_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register generic property client server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to generic property client server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool generic_property_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief get generic user properties
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_user_properties_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                  uint16_t app_key_index);

/**
 * @brief get generic user property
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @return send status
 */
mesh_msg_send_cause_t generic_user_property_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t property_id);

/**
 * @brief set generic user property
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @param[in] pvalue: property value
 * @param[in] value_len: property value length
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_user_property_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t property_id, const uint8_t *pvalue, uint8_t value_len, bool ack);

/**
 * @brief get generic admin properties
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_admin_properties_get(const mesh_model_info_p pmodel_info,
                                                   uint16_t dst,
                                                   uint16_t app_key_index);

/**
 * @brief get generic admin property
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @return send status
 */
mesh_msg_send_cause_t generic_admin_property_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index, uint16_t property_id);

/**
 * @brief set generic admin property
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @param[in] pvalue: property value
 * @param[in] value_len: property value length
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_admin_property_set(const mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index, uint16_t property_id, uint8_t property_access,
                                                 const uint8_t *pvalue, uint8_t value_len, bool ack);

/**
 * @brief get generic manufacturer properties
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_manufacturer_properties_get(const mesh_model_info_p pmodel_info,
                                                          uint16_t dst,
                                                          uint16_t app_key_index);

/**
 * @brief get generic manufacturer properties
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t generic_manufacturer_property_get(const mesh_model_info_p pmodel_info,
                                                        uint16_t dst,
                                                        uint16_t app_key_index, uint16_t property_id);

/**
 * @brief set generic manufacturer property
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: property id
 * @param[in] pvalue: property value
 * @param[in] value_len: property value length
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t generic_manufacturer_property_set(const mesh_model_info_p pmodel_info,
                                                        uint16_t dst,
                                                        uint16_t app_key_index, uint16_t property_id, uint8_t property_access, bool ack);

/**
 * @brief get generic client properties
 * @param[in] pmodel_info: pointer to generic property model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] property_id: start property id
 * @return send status
 */
mesh_msg_send_cause_t generic_client_properties_get(const mesh_model_info_p pmodel_info,
                                                    uint16_t dst,
                                                    uint16_t app_key_index, uint16_t property_id);

/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_PROPERTY_H */
