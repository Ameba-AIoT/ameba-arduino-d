/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     blob_transfer.h
* @brief    Head file for blob transfer models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

#ifndef _BLOB_TRANSFER_H
#define _BLOB_TRANSFER_H

/* Add Includes here */
#include "platform_misc.h"
#include "mesh_api.h"

#if MESH_BLOB

BEGIN_DECLS

/**
 * @addtogroup BLOB_TRANSFER
 * @{
 */

/* blob server capabilites */
#define BLOB_TRANSFER_CPAS_MIN_BLOCK_SIZE_LOG                 6
#define BLOB_TRANSFER_CPAS_MAX_BLOCK_SIZE_LOG                 12
#define BLOB_TRANSFER_CPAS_MAX_TOTAL_CHUNKS                   20
#define BLOB_TRANSFER_CPAS_MAX_CHUNK_SIZE                     256
#define BLOB_TRANSFER_CPAS_MAX_BLOB_SIZE                      5000
#define BLOB_TRANSFER_CPAS_SERVER_MTU_SIZE                    350
#define BLOB_TRANSFER_CPAS_MODE_PULL_SUPPORT                  1
#define BLOB_TRANSFER_CPAS_MODE_PUSH_SUPPORT                  1

/**
 * @defgroup BLOB_TRANSFER_ACCESS_OPCODE Access Opcode
 * @brief Mesh messsage access opcode
 * @{
 */
#define MESH_MSG_BLOB_TRANSFER_GET                       0xB701
#define MESH_MSG_BLOB_TRANSFER_START                     0xB702
#define MESH_MSG_BLOB_TRANSFER_CANCEL                    0xB703
#define MESH_MSG_BLOB_TRANSFER_STATUS                    0xB704
#define MESH_MSG_BLOB_BLOCK_GET                          0xB707
#define MESH_MSG_BLOB_BLOCK_START                        0xB705
#define MESH_MSG_BLOB_BLOCK_STATUS                       0x7E
#define MESH_MSG_BLOB_PARTIAL_BLOCK_REPORT               0x4F
#define MESH_MSG_BLOB_CHUNK_TRANSFER                     0x7D
#define MESH_MSG_BLOB_INFO_GET                           0xB70A
#define MESH_MSG_BLOB_INFO_STATUS                        0xB70B
/** @} */

/**
 * @defgroup BLOB_TRANSFER_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_BLOB_TRANSFER_SERVER                  0xBF42FFFF
#define MESH_MODEL_BLOB_TRANSFER_CLIENT                  0xBF43FFFF
/** @} */

/**
 * @defgroup BLOB_TRANSFER_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
enum {
	BLOB_TRANSFER_PHASE_IDLE,
	BLOB_TRANSFER_PHASE_WAITING_START,
	BLOB_TRANSFER_PHASE_WAITING_BLOCK,
	BLOB_TRANSFER_PHASE_WAITING_CHUNK,
	BLOB_TRANSFER_PHASE_COMPLETE,
	BLOB_TRANSFER_PHASE_SUSPENDED,
} _SHORT_ENUM_;
typedef uint8_t blob_transfer_phase_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_TRANSFER_GET)];
} _PACKED_ blob_transfer_get_t;

#define BLOB_BLOCK_SIZE_LOG_MIN              0x6
#define BLOB_BLOCK_SIZE_LOG_MAX              0x20

enum {
	BLOB_TRANSFER_MODE_IDLE, //!< No active transfer
	BLOB_TRANSFER_MODE_PUSH, //!< Push BLOB transfer mode
	BLOB_TRANSFER_MODE_PULL, //!< Pull BlOB transfer mode
} _SHORT_ENUM_;
typedef uint8_t blob_transfer_mode_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_TRANSFER_START)];
	uint8_t rfu : 6;
	uint8_t transfer_mode : 2;
	uint8_t blob_id[8];
	uint32_t blob_size;
	uint8_t block_size_log; //!< limited range
	uint16_t client_mtu_size;
} _PACKED_ blob_transfer_start_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_TRANSFER_CANCEL)];
	uint8_t blob_id[8];
} _PACKED_ blob_transfer_cancel_t;

enum {
	BLOB_TRANSFER_STATUS_SUCCESS,
	BLOB_TRANSFER_STATUS_INVALID_BLOCK_NUM,
	BLOB_TRANSFER_STATUS_INVALID_BLOCK_SIZE,
	BLOB_TRANSFER_STATUS_INVALID_CHUNK_SIZE,
	BLOB_TRANSFER_STATUS_WRONG_PHASE,
	BLOB_TRANSFER_STATUS_INVALID_PARAMETER,
	BLOB_TRANSFER_STATUS_WRONG_ID,
	BLOB_TRANSFER_STATUS_TOO_LARGE,
	BLOB_TRANSFER_STATUS_UNSUPPORTED_MODE,
	BLOB_TRANSFER_STATUS_INTERNAL_ERR,
	BLOB_TRANSFER_STATUS_INFO_UNAVAILABLE,
} _SHORT_ENUM_;
typedef uint8_t blob_transfer_status_code_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_TRANSFER_STATUS)];
	uint8_t status : 4;
	uint8_t rfu : 2;
	uint8_t transfer_mode : 2;
	blob_transfer_phase_t transfer_phase;
	uint8_t blob_id[8];
	uint32_t blob_size;
	uint8_t block_size_log; //!< limited range
	uint16_t transfer_mtu_size;
	uint8_t blocks_not_received[0];
} _PACKED_ blob_transfer_status_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_BLOCK_GET)];
} _PACKED_ blob_block_get_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_BLOCK_START)];
	uint16_t block_num;
	uint16_t chunk_size;
} _PACKED_ blob_block_start_t;

enum {
	BLOB_CHUNK_MISSING_FORMAT_ALL,
	BLOB_CHUNK_MISSING_FORMAT_NONE,
	BLOB_CHUNK_MISSING_FORMAT_SOME,
	BLOB_CHUNK_MISSING_FORMAT_ENCODED,
} _SHORT_ENUM_;
typedef uint8_t blob_chunk_missing_format_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_BLOCK_STATUS)];
	uint8_t status : 4;
	uint8_t rfu : 2;
	uint8_t format : 2;
	uint16_t block_num;
	uint16_t chunk_size;
	uint8_t missing_chunks[0];
} _PACKED_ blob_block_status_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_PARTIAL_BLOCK_REPORT)];
	uint8_t encoded_missing_chunks[0];
} _PACKED_ blob_partial_block_report_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_CHUNK_TRANSFER)];
	uint16_t chunk_num;
	uint8_t data[0]; //!< variable length
} _PACKED_ blob_chunk_transfer_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_INFO_GET)];
} _PACKED_ blob_info_get_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_BLOB_INFO_STATUS)];
	uint8_t min_block_size_log;
	uint8_t max_block_size_log;
	uint16_t max_total_chunks;
	uint16_t max_chunk_size;
	uint32_t max_blob_size;
	uint16_t server_mtu_size;
	uint8_t supported_transfer_mode;
} _PACKED_ blob_info_status_t;
/** @} */

#define BLOB_BLOCK_NUM_NOT_SET           0xFFFF
#define BLOB_CHUNK_SIZE_NOT_SET          0xFFFF

/**
 * @defgroup BLOB_TRANSFER_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
typedef struct {
	uint8_t min_block_size_log;
	uint8_t max_block_size_log;
	uint16_t max_total_chunks;
	uint16_t max_chunk_size;
	uint32_t max_blob_size; //!< supported max size
	uint16_t server_mtu_size;
	uint8_t mode_pull_support : 1;
	uint8_t mode_push_support : 1;
	uint8_t mode_support_rfu : 6;
} blob_server_capabilites_t;

typedef struct {
	blob_server_capabilites_t caps;

	blob_transfer_mode_t mode;
	blob_transfer_phase_t phase;
	uint8_t blob_id[8];
	uint32_t blob_size;
	uint32_t block_size;
	uint16_t client_mtu_size;
	uint16_t transfer_mtu_size;
	uint16_t transfer_timeout_base;
	uint8_t transfer_ttl;

	uint8_t *block_data;
	uint8_t *blocks_not_recv;
	uint16_t blocks_not_recv_len;
	uint16_t current_block_num;
	uint32_t current_block_size;
	uint16_t total_blocks;

	uint16_t current_chunk_size; //!< maybe different in different blocks
	uint16_t current_total_chunks;
	uint8_t *missing_chunks;
	uint8_t current_missing_chunks_len; //!< in bytes

	/* pull mode */
	uint16_t updater_addr;
	uint16_t updater_app_key_index;

	plt_timer_t transfer_timer;
	plt_timer_t partial_report_timer;
	uint8_t partial_report_retry_times;
	uint32_t partial_report_timeout_time;
	uint8_t partial_report_max_retry_times;
} blob_transfer_server_ctx_t;

extern blob_transfer_server_ctx_t blob_transfer_server_ctx;

#define BLOB_TRANSFER_SERVER_BLOCK_DATA                         0 //!< @ref blob_transfer_server_block_data_t
#define BLOB_TRANSFER_SERVER_COMPLETE                           1 //!< @ref blob_transfer_server_complete_t
#define BLOB_TRANSFER_SERVER_SUSPEND                            2 //!< @ref blob_transfer_server_suspend_t
#define BLOB_TRANSFER_SERVER_CANCEL                             3 //!< @ref blob_transfer_server_cancel_t
#define BLOB_TRANSFER_SERVER_FAIL                               4 //!< @ref blob_transfer_server_fail_t


typedef struct {
	uint16_t block_num;
	uint8_t *pdata;
	uint16_t data_len;
} blob_transfer_server_block_data_t;

typedef struct {
	uint8_t blob_id[8];
} blob_transfer_server_complete_t;

typedef struct {
	uint8_t blob_id[8];
} blob_transfer_server_suspend_t;

typedef struct {
	uint8_t blob_id[8];
} blob_transfer_server_cancel_t;

typedef struct {
	blob_transfer_status_code_t status;
	uint8_t blob_id[8];
} blob_transfer_server_fail_t;
/** @} */

/**
 * @defgroup BLOB_TRANSFER_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define BLOB_TRANSFER_CLIENT_TRANSFER_STATUS                             0 //!< @ref blob_transfer_client_transfer_status_t
#define BLOB_TRANSFER_CLIENT_BLOCK_STATUS                                1 //!< @ref blob_transfer_client_block_status_t
#define BLOB_TRANSFER_CLIENT_PARTIAL_BLOCK_REPORT                        2 //!< @ref blob_transfer_client_partial_block_report_t
#define BLOB_TRANSFER_CLIENT_INFO_STATUS                                 3 //!< @ref blob_transfer_client_info_status_t

typedef struct {
	uint16_t src;
	uint16_t app_key_index;
	blob_transfer_status_code_t status;
	blob_transfer_mode_t transfer_mode;
	blob_transfer_phase_t transfer_phase;
	uint8_t blob_id[8];
	uint32_t blob_size;
	uint8_t block_size_log;
	uint16_t transfer_mtu_size;
	uint16_t *pmissing_blocks;
	uint16_t missing_blocks_len;
} blob_transfer_client_transfer_status_t;

typedef struct {
	uint16_t src;
	uint16_t app_key_index;
	blob_transfer_status_code_t status;
	uint16_t block_num;
	uint16_t chunk_size;
	blob_chunk_missing_format_t missing_format;
	uint16_t *pmissing_chunks;
	uint16_t missing_chunks_len;
} blob_transfer_client_block_status_t;

typedef struct {
	uint16_t src;
	uint16_t app_key_index;
	uint16_t *pmissing_chunks;
	uint16_t missing_chunks_len;
} blob_transfer_client_partial_block_report_t;

typedef struct {
	uint16_t src;
	uint16_t app_key_index;
	uint8_t min_block_size_log;
	uint8_t max_block_size_log;
	uint16_t max_total_chunks;
	uint16_t max_chunk_size;
	uint32_t max_blob_size;
	uint16_t server_mtu_size;
	uint8_t supported_transfer_mode;
} blob_transfer_client_info_status_t;
/** @} */

/**
 * @defgroup BLOB_TRANSFER_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief blob transfer server register
 * @param[in] element_index: element index
 * @param[in] model_data_cb: model data callback function
 * @return register status
 */
bool blob_transfer_server_reg(uint8_t element_index, model_data_cb_pf model_data_cb);

/**
 * @brief set blob transfer server model data callback function
 * @param[in] model_data_cb: model data callback function
 */
void blob_transfer_server_set_data_cb(model_data_cb_pf model_data_cb);

/**
 * @brief set blob transfer server capabilites
 * @param[in] pcaps: server capabilites
 */
void blob_transfer_server_caps_set(blob_server_capabilites_t *pcaps);

/**
 * @brief get blob transfer server capabilites
 * @return server capabilites
 */
blob_server_capabilites_t blob_transfer_server_caps_get(void);

/**
 * @brief initialize blob transfer server
 * @param[in] blob_id: blob id
 * @param[in] transfer_timeout: transfer timeout time
 * @param[in] transfer_ttl: transfer ttl value
 * @return initialize status
 */
bool blob_transfer_server_init(uint8_t blob_id[8], uint16_t transfer_timeout, uint8_t transfer_ttl);

/**
 * @brief load blob transfer procedure
 * @param[in] blob_size: blob size
 * @param[in] block_size: block size
 * @param[in] start_block_num: start block number
 * @param[in] transfer_mode: transfer mode
 * @param[in] transfer_mtu_size: transfer mtu size
 * @param[in] updater_addr: updater address
 * @param[in] updater_app_key_index: updater app key index
 * @return load status
 */
bool blob_transfer_server_load(uint32_t blob_size, uint32_t block_size, uint16_t start_block_num,
							   blob_transfer_mode_t transfer_mode, uint16_t transfer_mtu_size, uint16_t updater_addr,
							   uint16_t updater_app_key_index);

/**
 * @brief clear blob transfer server
 */
void blob_transfer_server_clear(void);

/**
 * @brief get blob transfer server phase
 * @return blob transfer server phase
 */
blob_transfer_phase_t blob_transfer_server_phase_get(void);

/**
 * @brief check whether blob transfer server is busy or not
 * @return TRUE: busy
 *         FALSE: idle
 */
bool blob_transfer_server_busy(void);

/**
 * @brief handle blob transfer server timeout
 */
void blob_transfer_handle_transfer_timeout(void);

/**
 * @brief handle blob transfer partial report timeout
 */
void blob_transfer_handle_partial_report_timeout(void);

/**
 * @brief send blob transfer status
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] status: blob transfer status
 * @param[in] delay_time: message delay time
 * @return send status
 */
mesh_msg_send_cause_t blob_transfer_status(uint16_t dst, uint16_t app_key_index,
		blob_transfer_status_code_t status, uint32_t delay_time);

/**
 * @brief send blob block status
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] status: blob transfer status
 * @param[in] delay_time: message delay time
 * @return send status
 */
mesh_msg_send_cause_t blob_block_status(uint16_t dst, uint16_t app_key_index,
										blob_transfer_status_code_t status, uint32_t delay_time);

/**
 * @brief send blob partial block report
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t blob_partial_block_report(uint16_t dst, uint16_t app_key_index);

/**
 * @brief send blob information status
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] delay_time: message delay time
 * @return send status
 */
mesh_msg_send_cause_t blob_info_status(uint16_t dst, uint16_t app_key_index, uint32_t delay_time);
/** @} */

/**
 * @defgroup BLOB_TRANSFER_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief blob transfer client register
 * @param[in] element_index: element index
 * @param[in] model_data_cb: model data callback function
 * @return register status
 */
void blob_transfer_client_reg(uint8_t element_index, model_data_cb_pf model_data_cb);

/**
 * @brief get blob transfer
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t blob_transfer_get(uint16_t dst, uint16_t app_key_index);

/**
 * @brief blob transfer start
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] mode: blob transfer mode
 * @param[in] blob_id: blob id value
 * @param[in] blob_size: blob size value
 * @param[in] block_size_log: block size log value
 * @param[in] client_mtu_size: client mtu size
 * @return send status
 */
mesh_msg_send_cause_t blob_transfer_start(uint16_t dst, uint16_t app_key_index,
		blob_transfer_mode_t mode, uint8_t blob_id[8], uint32_t blob_size, uint8_t block_size_log,
		uint16_t client_mtu_size);

/**
 * @brief cancel blob transfer
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] blob_id: blob id value
 * @return send status
 */
mesh_msg_send_cause_t blob_transfer_cancel(uint16_t dst, uint16_t app_key_index,
		uint8_t blob_id[8]);

/**
 * @brief get blob block status
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] blob_id: blob id value
 * @return send status
 */
mesh_msg_send_cause_t blob_block_get(uint16_t dst, uint16_t app_key_index);

/**
 * @brief blob block transfer start
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] block_num: total block number
 * @param[in] chunk_size: transfer chunk size
 * @return send status
 */
mesh_msg_send_cause_t blob_block_start(uint16_t dst, uint16_t app_key_index, uint16_t block_num,
									   uint16_t chunk_size);

/**
 * @brief blob chunk transfer start
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] chunk_num: total chunk number
 * @param[in] pdata: chunk data
 * @param[in] len: chunk data length
 * @return send status
 */
mesh_msg_send_cause_t blob_chunk_transfer(uint16_t dst, uint16_t app_key_index, uint16_t chunk_num,
		uint8_t *pdata, uint16_t len);

/**
 * @brief get blob transfer information
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t blob_info_get(uint16_t dst, uint16_t app_key_index);
/** @} */
/** @} */

END_DECLS

#endif /* MESH_BLOB */

#endif /* _BLOB_TRANSFER_H */
