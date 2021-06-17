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
#define MESH_MSG_FW_INFO_GET                            0xB601
#define MESH_MSG_FW_INFO_STAT                           0xB602
#define MESH_MSG_FW_UPDATE_GET                          0xB603
#define MESH_MSG_FW_UPDATE_PREPARE                      0xB604
#define MESH_MSG_FW_UPDATE_START                        0xB605
#define MESH_MSG_FW_UPDATE_ABORT                        0xB606
#define MESH_MSG_FW_UPDATE_APPLY                        0xB607
#define MESH_MSG_FW_UPDATE_STAT                         0xB608
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_FW_UPDATE_SERVER                     0xFE00FFFF
#define MESH_MODEL_FW_UPDATE_CLIENT                     0xFE01FFFF
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define FW_UPDATE_FW_ID_LEN                             4
typedef uint8_t *fw_update_fw_id_t;
#define FW_UPDATE_FW_ID(a,b)                            memcpy(a,b,FW_UPDATE_FW_ID_LEN)

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_INFO_GET)];
} _PACKED_ fw_info_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_INFO_STAT)];
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
    uint8_t update_url[1]; //!< variable length, optional
} _PACKED_ fw_info_stat_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_GET)];
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
} _PACKED_ fw_update_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_PREPARE)];
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
    uint8_t object_id[8];
    uint8_t vendor_validate_data[1]; //!< optional, Vendor specific validation data for update
} _PACKED_ fw_update_prepare_t;

enum
{
    FW_UPDATE_POLICY_NONE, //!< Do not apply new firmware when Object transfer is completed.
    FW_UPDATE_POLICY_AUTO_UPDATE //!< Apply new firmware when Object transfer is completed.
} _SHORT_ENUM_;
typedef uint8_t fw_update_policy_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_START)];
    fw_update_policy_t policy;
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
} _PACKED_ fw_update_start_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_ABORT)];
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
} _PACKED_ fw_update_abort_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_APPLY)];
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
} _PACKED_ fw_update_apply_t;

enum
{
    FW_UPDATE_STAT_SUCCESS,
    FW_UPDATE_STAT_WRONG_COMPANY_FIRMWARE_COMBINATION,
    FW_UPDATE_STAT_DIFF_OBJECT_TRANSFER_ALREADY_ONGOING,
    FW_UPDATE_STAT_COMPANY_FIRMWARE_APPLY_FAILED,
    FW_UPDATE_STAT_NEWER_FW_VERSION_PRESENT, //!< Company ID and Firmware ID combination permanently rejected
    FW_UPDATE_STAT_NOT_AVAILABLE //!< Company ID and Firmware ID combination temporary rejected
} _SHORT_ENUM_;
typedef uint8_t fw_update_stat_stat_t;

enum
{
    FW_UPDATE_PHASE_IDLE, //!< No DFU update in progress
    FW_UPDATE_PHASE_PREPARED, //!< DFU update is prepared and awaiting start
    FW_UPDATE_PHASE_IN_PROGRESS, //!< DFU update is in progress
    FW_UPDATE_PHASE_DFU_READY //!< DFU upload is finished and waiting to be apply
} _SHORT_ENUM_;
typedef uint8_t fw_update_phase_t;

typedef struct
{
    uint8_t prov_needed: 1;
} _PACKED_ fw_update_addi_info_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_FW_UPDATE_STAT)];
    fw_update_stat_stat_t stat;
    uint8_t phase: 3; //!< @ref fw_update_phase_t
    uint8_t addi_info: 5; //!< @ref fw_update_addi_info_t
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
    uint8_t object_id[8]; //!< optional, Unique object identifier
} _PACKED_ fw_update_stat_t;
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
typedef struct
{
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
    uint8_t firmware_id_new[FW_UPDATE_FW_ID_LEN];
    fw_update_phase_t phase;
    fw_update_stat_stat_t stat;
    uint8_t object_id[8]; //!< obtained from object transfer server
    union
    {
        fw_update_addi_info_t addi_info; //!< set by upper
        uint8_t addi_info_8;
    };
    uint8_t *update_url; //!< set by upper
    uint16_t url_len;
    uint8_t *vendor_validate_data; //!< set by remote
    uint16_t validate_len;
    fw_update_policy_t policy;
} fw_update_server_ctx_t;

typedef enum
{
    FW_UPD_SERVER_CB_TYPE_PREPARE,
    FW_UPD_SERVER_CB_TYPE_START,
    FW_UPD_SERVER_CB_TYPE_ABORT,
    FW_UPD_SERVER_CB_TYPE_APPLY
} fw_update_server_cb_type_t;
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */
void fw_update_server_reg(uint16_t company_id, fw_update_fw_id_t firmware_id,
                          model_data_cb_pf model_data_cb);
void fw_update_server_load(uint8_t object_id[8], fw_update_fw_id_t firmware_id_new,
                           fw_update_policy_t policy);
void fw_update_server_clear(void);
bool fw_update_server_ready(void);
mesh_msg_send_cause_t fw_info_stat(mesh_msg_p pmesh_msg, uint16_t company_id,
                                   fw_update_fw_id_t firmware_id,
                                   uint8_t update_url[], uint16_t url_len);
mesh_msg_send_cause_t fw_update_stat(mesh_msg_p pmesh_msg, fw_update_stat_stat_t stat,
                                     uint8_t phase, uint8_t addi_info, uint16_t company_id, fw_update_fw_id_t firmware_id,
                                     uint8_t object_id[8]);
/** @} */

/**
 * @defgroup FIRMWARE_UPDATE_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
void fw_update_client_reg(model_receive_pf pf);
mesh_msg_send_cause_t fw_info_get(uint16_t dst);
mesh_msg_send_cause_t fw_update_get(uint16_t dst, uint16_t company_id,
                                    fw_update_fw_id_t firmware_id);
mesh_msg_send_cause_t fw_update_prepare(uint16_t dst, uint16_t company_id,
                                        fw_update_fw_id_t firmware_id,
                                        uint8_t object_id[8], uint8_t vendor_validate_data[], uint16_t validate_len);
mesh_msg_send_cause_t fw_update_start(uint16_t dst, fw_update_policy_t policy, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id);
mesh_msg_send_cause_t fw_update_abort(uint16_t dst, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id);
mesh_msg_send_cause_t fw_update_apply(uint16_t dst, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id);
/** @} */
/** @} */

END_DECLS

#endif /* _FIRMWARE_UPDATE_H */
