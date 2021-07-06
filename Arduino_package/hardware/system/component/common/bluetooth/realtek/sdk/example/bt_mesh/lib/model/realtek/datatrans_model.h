/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     datatrans_model.h
* @brief    Head file for data transport module
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-10-29
* @version  v1.0
* *************************************************************************************
*/

#ifndef _DATATRANS_MODEL_H_
#define _DATATRANS_MODEL_H_

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup DATA_TRANSMISSION
 * @{
 */

/**
 * @defgroup DATATRANS_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_DATATRANS_WRITE                        0xC95D00
#define MESH_MSG_DATATRANS_WRITE_UNACK                  0xCA5D00
#define MESH_MSG_DATATRANS_STATUS                       0xCB5D00
#define MESH_MSG_DATATRANS_READ                         0xCC5D00
#define MESH_MSG_DATATRANS_DATA                         0xCD5D00
/** @} */

/**
 * @defgroup DATATRANS_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_DATATRANS                            0x0004005D
/** @} */

/**
 * @defgroup DATATRANS_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
enum
{
    DATATRANS_SUCCESS,
    DATATRANS_FAIL,
} _SHORT_ENUM_;
typedef uint8_t datatrans_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_DATATRANS_WRITE)];
    uint8_t data[0];
} _PACKED_ datatrans_write_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_DATATRANS_STATUS)];
    datatrans_stat_t status;
    uint16_t written_len;
} _PACKED_ datatrans_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_DATATRANS_READ)];
    uint16_t read_len;
} _PACKED_ datatrans_read_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_DATATRANS_DATA)];
    uint8_t data[0];
} _PACKED_ datatrans_data_t;

/** @} */

/**
 * @defgroup DATATRANS_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define DATATRANS_SERVER_WRITE                            0 //!< @ref datatrans_server_write_t
#define DATATRANS_SERVER_READ                             1 //!< @ref datatrans_server_read_t

typedef struct
{
    uint16_t data_len;
    uint8_t *data;
    /**
     * app modify status and success written length if need to,
     * the default value is DATATRANS_SUCCESS and data_len
     */
#if defined(CONFIG_BT_MESH_TEST) && CONFIG_BT_MESH_TEST
    uint16_t src;
#endif
    datatrans_stat_t status;
    uint16_t written_len;
} datatrans_server_write_t;;

typedef struct
{
    /** app read length field first, then may change length and data field */
    uint16_t data_len;
    uint8_t *data;
} datatrans_server_read_t;
/** @} */

/**
 * @defgroup DATATRANS_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define DATATRANS_CLIENT_STATUS                           2 //!< @ref datatrans_client_status_t
#define DATATRANS_CLIENT_DATA                             3 //!< @ref datatrans_client_data_t  

typedef struct
{
    datatrans_stat_t status;
    uint16_t written_len;
} datatrans_client_status_t;

typedef struct
{
    uint16_t data_len;
    uint8_t *data;
} datatrans_client_data_t;
/** @} */



/**
 * @defgroup DATATRANS_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register data transmission server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to data transimission server model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool datatrans_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief publish data transmission data
 * @param[in] pmodel_info: pointer to data transmission server model context
 * @param[in] data_len: data length to publish
 * @param[in] data: data need to publish
 * @return publish status
 */
mesh_msg_send_cause_t datatrans_publish(const mesh_model_info_p pmodel_info,
                                        uint16_t data_len, uint8_t *data);
/** @} */

/**
 * @defgroup DATATRANS_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register data transmission
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to data transimission model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool datatrans_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief data transmission write data
 * @param[in] pmodel_info: pointer to data transmission client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] data_len: data length to write
 * @param[in] data: data to write
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t datatrans_write(const mesh_model_info_p pmodel_info, uint16_t dst,
                                      uint16_t app_key_index, uint16_t data_len, uint8_t *data,
                                      bool ack);

/**
 * @brief data transmission read data
 * @param[in] pmodel_info: pointer to data transmission client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] read_len: data length to read
 * @return send status
 */
mesh_msg_send_cause_t datatrans_read(const mesh_model_info_p pmodel_info, uint16_t dst,
                                     uint16_t app_key_index, uint16_t read_len);
/** @} */
/** @} */

END_DECLS

#endif /** _DATATRANS_MODEL_H_ */
