/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     firmware_update.h
* @brief    Head file for firmware update models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _FIRMWARE_UPDATE_H
#define _FIRMWARE_UPDATE_H

/* Add Includes here */
#include "platform_misc.h"
#include "mesh_api.h"

#if MESH_DFU

BEGIN_DECLS

/**
 * @addtogroup FIRMWARE_UPDATE
 * @{
 */

/**
 * @defgroup FIRMWARE_UPDATE_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_FW_UPDATE_INFO_GET                     0xB71B
#define MESH_MSG_FW_UPDATE_INFO_STATUS                  0x7C
#define MESH_MSG_FW_UPDATE_FW_METADATA_CHECK            0x7B
#define MESH_MSG_FW_UPDATE_FW_METADATA_STATUS           0x7A
#define MESH_MSG_FW_UPDATE_GET                          0xB71C
#define MESH_MSG_FW_UPDATE_START                        0xB71D
#define MESH_MSG_FW_UPDATE_CANCEL                       0xB71E
#define MESH_MSG_FW_UPDATE_APPLY                        0xB71F
#define MESH_MSG_FW_UPDATE_STATUS                       0x70
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_FW_UPDATE_SERVER                     0xBF44FFFF
#define MESH_MODEL_FW_UPDATE_CLIENT                     0xBF45FFFF
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define FW_VERSION_MAX_LEN                              106

typedef struct
{
    uint16_t company_id;
    uint8_t version[FW_VERSION_MAX_LEN];
} _PACKED_ fw_id_t;

typedef struct
{
    fw_id_t fw_id;
    uint8_t fw_id_len;
    uint8_t update_uri_len;
    uint8_t update_uri[255];
} fw_info_t;

enum
{
    FW_UPDATE_PHASE_IDLE,
    FW_UPDATE_PHASE_TRANSFER_ERR,
    FW_UPDATE_PHASE_TRANSFER_ACTIVE,
    FW_UPDATE_PHASE_VERIFYING_UPDATE,
    FW_UPDATE_PHASE_VERIFICATION_SUCCEEDED,
    FW_UPDATE_PHASE_VERIFICATION_FAILED,
    FW_UPDATE_PHASE_APPLYING_UPDATE,
} _SHORT_ENUM_;
typedef uint8_t fw_update_phase_t;

enum
{
    FW_UPDATE_ADDI_INFO_COMP_NO_CHANGE,
    FW_UPDATE_ADDI_INFO_COMP_CHANGED_AND_NOT_SUPPORTED_RMT_PROV,
    FW_UPDATE_ADDI_INFO_COMP_CHANGED_AND_SUPPORT_RMT_PROV,
    FW_UPDATE_ADDI_INFO_NODE_UNPROVED,
} _SHORT_ENUM_;
typedef uint8_t fw_update_addi_info_t;

enum
{
    FW_RETRIEVED_UPDATE_PHASE_IDLE,
    FW_RETRIEVED_UPDATE_PHASE_TRANSFER_ERR,
    FW_RETRIEVED_UPDATE_PHASE_TRANSFER_ACTIVE,
    FW_RETRIEVED_UPDATE_PHASE_VERIFYING_UPDATE,
    FW_RETRIEVED_UPDATE_PHASE_VERIFICATION_SUCCEEDED,
    FW_RETRIEVED_UPDATE_PHASE_VERIFICATION_FAILED,
    FW_RETRIEVED_UPDATE_PHASE_APPLYING_UPDATE,
    FW_RETRIEVED_UPDATE_PHASE_TRANSFER_CANCELED,
    FW_RETRIEVED_UPDATE_PHASE_APPLY_SUCCESS,
    FW_RETRIEVED_UPDATE_PHASE_APPLY_FAILED,
    FW_RETRIEVED_UPDATE_PHASE_APPLY_UNKNOWN,
} _SHORT_ENUM_;
typedef uint8_t fw_retrieved_update_phase_t;

enum
{
    FW_UPDATE_STATUS_SUCCESS,
    FW_UPDATE_STATUS_INSUFFICIENT_RESOURCES,
    FW_UPDATE_STATUS_WRONG_PHASE,
    FW_UPDATE_STATUS_INTERNAL_ERR,
    FW_UPDATE_STATUS_WRONG_FW_INDEX,
    FW_UPDATE_STATUS_METADATA_CHECK_FAILED,
    FW_UPDATE_STATUS_TEMP_UNAVAIL,
    FW_UPDATE_STATUS_BLOB_TRANSFER_BUSY,
} _SHORT_ENUM_;
typedef uint8_t fw_update_status_code_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_INFO_GET)];
    uint8_t first_index;
    uint8_t entries_limit;
} _PACKED_ fw_update_info_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_INFO_STATUS)];
    uint8_t fw_info_list_cnt;
    uint8_t first_index;
    uint8_t fw_info_list[0];
} _PACKED_ fw_update_info_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_FW_METADATA_CHECK)];
    uint8_t fw_image_idx;
    uint8_t fw_metadata[0];
} _PACKED_ fw_update_fw_metadata_check_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_FW_METADATA_STATUS)];
    uint8_t status : 3;
    uint8_t addi_info : 5;
    uint8_t fw_image_idx;
} _PACKED_ fw_update_fw_metadata_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_GET)];
} _PACKED_ fw_update_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_START)];
    uint8_t update_ttl;
    uint16_t update_timeout_base;
    uint8_t blob_id[8];
    uint8_t fw_image_idx;
    uint8_t fw_metadata[0];
} _PACKED_ fw_update_start_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_CANCEL)];
} _PACKED_ fw_update_cancel_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_APPLY)];
} _PACKED_ fw_update_apply_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_STATUS)];
    uint8_t status : 3;
    uint8_t rfu1 : 2;
    uint8_t update_phase : 3;
    uint8_t update_ttl;
    uint8_t addi_info : 5;
    uint8_t rfu2 : 3;
    uint16_t update_timeout_base;
    uint8_t blob_id[8];
    uint8_t update_fw_image_idx;
} _PACKED_ fw_update_status_t;
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
typedef struct
{
    plt_list_t fw_info_list;
    fw_update_phase_t update_phase;
    uint8_t update_ttl;
    fw_update_addi_info_t addi_info;
    uint16_t update_timeout_base;
    uint8_t blob_id[8];
    uint8_t update_fw_image_idx;
    uint8_t fw_metadata[255];
} fw_update_server_ctx_t;

extern fw_update_server_ctx_t fw_update_server_ctx;

#define FW_UPDATE_SERVER_METADATA_CHECK                         0 //!< @ref fw_update_server_metadata_check_t
#define FW_UPDATE_SERVER_START                                  1 //!< @ref fw_update_server_start_t
#define FW_UPDATE_SERVER_VERIFY                                 2 //!< @ref fw_update_server_verify_t
#define FW_UPDATE_SERVER_VERIFY_CANCEL                          3 //!< @ref fw_update_server_verify_cancel_t
#define FW_UPDATE_SERVER_APPLY                                  4 //!< @ref fw_update_server_apply_t
#define FW_UPDATE_SERVER_BLOCK_DATA                             5 //!< @ref fw_update_server_block_data_t
#define FW_UPDATE_SERVER_FAIL                                   6 //!< @ref fw_update_server_fail_t


typedef struct
{
    uint8_t fw_image_idx;
    uint8_t *pmetadata;
    uint8_t metadata_len;
    /* upper layer should set these field */
    fw_update_status_code_t *pstatus;
    fw_update_addi_info_t *paddi_info;
} fw_update_server_metadata_check_t;

typedef struct
{
    uint8_t fw_image_idx;
    /* upper layer should set these field */
    bool *pcan_update;
} fw_update_server_start_t;

typedef struct
{
    uint8_t fw_image_idx;
} fw_update_server_verify_t;

typedef struct
{
    uint8_t fw_image_idx;
} fw_update_server_verify_cancel_t;

typedef struct
{
    uint8_t fw_image_idx;
    /* upper layer should set these field */
    bool *pcan_apply;
} fw_update_server_apply_t;

typedef struct
{
    uint16_t block_num;
    uint8_t *pdata;
    uint16_t data_len;
} fw_update_server_block_data_t;

typedef enum
{
    FW_UPDATE_FAIL_REASON_USER_CANCEL,
    FW_UPDATE_FAIL_REASON_TRANSFER_TIMEOUT,
    FW_UPDATE_FAIL_REASON_TRANSFER_ERR,
} fw_update_fail_reason_t;

typedef struct
{
    uint8_t fw_image_index;
    fw_update_fail_reason_t reason;
} fw_update_server_fail_t;
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define FW_UPDATE_CLIENT_INFO_STATUS                                 0 //!< @ref fw_update_client_info_status_t
#define FW_UPDATE_CLIENT_FW_METADATA_STATUS                          1 //!< @ref fw_update_client_fw_metadata_status_t
#define FW_UPDATE_CLIENT_STATUS                                      2 //!< @ref fw_update_client_status_t

typedef struct
{
    uint16_t src;
    uint8_t fw_info_list_cnt;
    uint8_t first_index;
    fw_info_t *pfw_info;
    uint8_t fw_info_cnt;
} fw_update_client_info_status_t;

typedef struct
{
    uint16_t src;
    fw_update_status_code_t status;
    fw_update_addi_info_t addi_info;
    uint8_t update_fw_image_index;
} fw_update_client_fw_metadata_status_t;

typedef struct
{
    uint16_t src;
    uint16_t app_key_index;
    fw_update_status_code_t status;
    fw_update_phase_t update_phase;
    uint8_t update_ttl;
    fw_update_addi_info_t addi_info;
    uint16_t update_timeout_base;
    uint8_t blob_id[8];
    uint8_t update_fw_image_index;
} fw_update_client_status_t;
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief firmware update server register
 * @param[in] element_index: element index
 * @param[in] model_data_cb: model data callback function
 * @return register status
 */
bool fw_update_server_reg(uint8_t element_index, model_data_cb_pf model_data_cb);

/**
 * @brief load firmware update server
 * @param[in] pfw_metadata: firmware metadata
 * @param[in] metadata_len: firmware metadata length
 * @param[in] fw_image_index: firmware image index
 * @param[in] update_ttl: update ttl
 * @param[in] update_timeout_base: update timeout base
 * @param[in] blob_id: blob id
 * @return load status
 */
bool fw_update_server_load(uint8_t *pfw_metadata, uint8_t metadata_len, uint8_t fw_image_index,
                           uint8_t update_ttl, uint8_t update_timeout_base, uint8_t blob_id[8]);
/**
 * @brief add firmware update information
 * @param[in] pfw_id: firmware id
 * @param[in] fw_id_len: firmware id length
 * @param[in] pupdate_uri: update uri value
 * @param[in] update_uri_len: update uri length
 * @return add status
 */
bool fw_update_server_add_info(fw_id_t *pfw_id, uint8_t fw_id_len,
                               uint8_t *pupdate_uri, uint8_t update_uri_len);

/**
 * @brief set firmware verify result
 * @param[in] success: TRUE: verify success
 *                     FALSE: verify failed
 */
void fw_update_server_set_verify_result(bool success);

/**
 * @brief clear firmware update server
 */
void fw_update_server_clear(void);

/**
 * @brief handle blob server data
 * @param[in] pmodel_info: blob transfer server model
 * @param[in] type: data callback message type
 * @param[in] pargs: data callback value
 * @return handle result
 */
int32_t fw_update_handle_blob_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                          void *pargs);
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief firmware update client register
 * @param[in] element_index: element index
 * @param[in] model_data_cb: model data callback function
 * @return register status
 */
bool fw_update_client_reg(uint8_t element_index, model_data_cb_pf model_data_cb);

/**
 * @brief get firmware update information
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @param[in] first_index: first index
 * @param[in] entries_limit: entries limit
 * @return send status
 */
mesh_msg_send_cause_t fw_update_info_get(uint16_t dst, uint16_t app_key_index, uint8_t first_index,
                                         uint8_t entries_limit);

/**
 * @brief check firmware metadata
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @param[in] update_fw_image_idx: update firmware image index
 * @param[in] pfw_metadata: firmware metadata
 * @param[in] metadata_len: firmware metadata length
 * @return send status
 */
mesh_msg_send_cause_t fw_update_fw_metadata_check(uint16_t dst, uint16_t app_key_index,
                                                  uint8_t update_fw_image_idx, uint8_t *pfw_metadata, uint8_t metadata_len);

/**
 * @brief get firmware update status
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @return send status
 */
mesh_msg_send_cause_t fw_update_get(uint16_t dst, uint16_t app_key_index);

/**
 * @brief start firmware update
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @param[in] update_ttl: update ttl value
 * @param[in] update_timeout_base: update timeout base
 * @param[in] blob_id: blob id
 * @param[in] update_fw_image_idx: update firmware image index
 * @param[in] pfw_metadata: firmware metadata
 * @param[in] metadata_len: firmware metadata length
 * @return send status
 */
mesh_msg_send_cause_t fw_update_start(uint16_t dst, uint16_t app_key_index, uint8_t update_ttl,
                                      uint16_t update_timeout_base, uint8_t blob_id[8], uint8_t update_fw_image_idx,
                                      uint8_t *pfw_metadata, uint8_t metadata_len);

/**
 * @brief cancel firmware update
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @return send status
 */
mesh_msg_send_cause_t fw_update_cancel(uint16_t dst, uint16_t app_key_index);

/**
 * @brief apply firmware update
 * @param[in] dst: remote address
 * @param[in] app_key_index: app key index
 * @return send status
 */
mesh_msg_send_cause_t fw_update_apply(uint16_t dst, uint16_t app_key_index);
/** @} */
/** @} */

END_DECLS

#endif /* MESH_DFU */

#endif /* _FIRMWARE_UPDATE_H */
