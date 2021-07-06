/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     remote_provisioning.h
* @brief    Head file for remote provisioning models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2020-11-12
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _REMOTE_PROVISIONING_H
#define _REMOTE_PROVISIONING_H

#include "mesh_api.h"
#include "provision_client.h"

#if MESH_RPR

BEGIN_DECLS

/**
 * @addtogroup REMOTE_PROVISIONING
 * @{
 */

/**
 * @defgroup REMOTE_PROVISIONING_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_RMT_PROV_SCAN_CAPS_GET                        0x804F
#define MESH_MSG_RMT_PROV_SCAN_CAPS_STATUS                     0x8050
#define MESH_MSG_RMT_PROV_SCAN_GET                             0x8051
#define MESH_MSG_RMT_PROV_SCAN_START                           0x8052
#define MESH_MSG_RMT_PROV_SCAN_STOP                            0x8053
#define MESH_MSG_RMT_PROV_SCAN_STATUS                          0x8054
#define MESH_MSG_RMT_PROV_SCAN_REPORT                          0x8055
#define MESH_MSG_RMT_PROV_EXTENED_SCAN_START                   0x8056
#define MESH_MSG_RMT_PROV_EXTENED_SCAN_REPORT                  0x8057
#define MESH_MSG_RMT_PROV_LINK_GET                             0x8058
#define MESH_MSG_RMT_PROV_LINK_OPEN                            0x8059
#define MESH_MSG_RMT_PROV_LINK_CLOSE                           0x805A
#define MESH_MSG_RMT_PROV_LINK_STATUS                          0x805B
#define MESH_MSG_RMT_PROV_LINK_REPORT                          0x805C
#define MESH_MSG_RMT_PROV_PDU_SEND                             0x805D
#define MESH_MSG_RMT_PROV_PDU_OUTBOUND_REPORT                  0x805E
#define MESH_MSG_RMT_PROV_PDU_REPORT                           0x805F
/** @} */

/**
 * @defgroup REMOTE_PROVISIONING_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_RMT_PROV_SERVER                             0x0004ffff
#define MESH_MODEL_RMT_PROV_CLIENT                             0x0005ffff
/** @} */

/**
 * @defgroup REMOTE_PROVISIONING_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */

enum {
	RMT_PROV_SCAN_STATE_IDLE,
	RMT_PROV_SCAN_STATE_MULTIPLE_DEVICES,
	RMT_PROV_SCAN_STATE_SINGLE_DEVICE,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_scan_state_t;

enum {
	RMT_PROV_SCAN_NOT_SUPPORT_ACTIVE_SCAN,
	RMT_PROV_SCAN_SUPPORT_ACTIVE_SCAN,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_active_scan_state_t;

enum {
	RMT_PROV_DKRI_DEV_KEY_REFRESH,
	RMT_PROV_DKRI_NODE_ADDR_REFRESH,
	RMT_PROV_DKRI_NODE_COMPO_REFRESH,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_dkri_procedure_t;

enum {
	RMT_PROV_LINK_CLOSE_SUCCESS,
	RMT_PROV_LINK_CLOSE_PROHIBITED,
	RMT_PROV_LINK_CLOSE_FAIL,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_link_close_reason_t;

enum {
	RMT_PROV_LINK_STATE_IDLE,
	RMT_PROV_LINK_STATE_LINK_OPENING,
	RMT_PROV_LINK_STATE_LINK_ACTIVE,
	RMT_PROV_LINK_STATE_OUTBOUND_PKT_TRANS,
	RMT_PROV_LINK_STATE_LINK_CLOSING,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_link_state_t;

enum {
	RMT_PROV_SUCCESS,
	RMT_PROV_SCANNING_CANNOT_START,
	RMT_PROV_INVALID_STATE,
	RMT_PROV_LIMITED_RESOURCES,
	RMT_PROV_LINK_CANNOT_OPEN,
	RMT_PROV_LINK_OPEN_FAILED,
	RMT_PROV_LINK_CLOSED_BY_DEVICE,
	RMT_PROV_LINK_CLOSED_BY_SERVER,
	RMT_PROV_LINK_CLOSED_BY_CLIENT,
	RMT_PROV_LINK_CLOSED_AS_CANNOT_RECEIVE_PDU,
	RMT_PROV_LINK_CLOSED_AS_CANNOT_SEND_PDU,
	RMT_PROV_LINK_CLOSED_AS_CANNOT_DELIVER_PDU_REPORT,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_status_t;

enum {
	RMT_PROV_PROCEDURE_IDLE,
	RMT_PROV_PROCEDURE_PROV,
	RMT_PROV_PROCEDURE_DKRI,
} _SHORT_ENUM_;
typedef uint8_t rmt_prov_procedure_t;


#define RMT_PROV_MAX_SCANNED_ITEMS_MIN                         4
#define RMT_PROV_MAX_SCANNED_ITEMS_MAX                         255
#define RMT_PROV_SCANNED_ITEMS_NO_LIMIT                        0

#define RMT_PROV_EXTENED_SCAN_TIMEOUT_MIN                      0x01
#define RMT_PROV_EXTENED_SCAN_TIMEOUT_MAX                      0x15

#define RMT_PROV_LINK_OPEN_TIMEOUT_MIN                         0x01
#define RMT_PROV_LINK_OPEN_TIMEOUT_MAX                         0x3C

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_CAPS_GET)];
} _PACKED_ rmt_prov_scan_caps_get_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_CAPS_STATUS)];
	uint8_t max_scanned_items;
	rmt_prov_active_scan_state_t active_scan_state;
} _PACKED_ rmt_prov_scan_caps_status_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_GET)];
} _PACKED_ rmt_prov_scan_get_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_START)];
	uint8_t scanned_items_limit;
	uint8_t timeout;
	uint8_t uuid[16];
} _PACKED_ rmt_prov_scan_start_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_STOP)];
} _PACKED_ rmt_prov_scan_stop_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_STATUS)];
	rmt_prov_status_t status;
	rmt_prov_scan_state_t scan_state;
	uint8_t scanned_items_limit;
	uint8_t timeout;
} _PACKED_ rmt_prov_scan_status_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_SCAN_REPORT)];
	int8_t rssi;
	uint8_t uuid[16];
	uint8_t oob[2];
	uint8_t uri_hash[4];
} _PACKED_ rmt_prov_scan_report_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_EXTENED_SCAN_START)];
	uint8_t ad_type_filter_count;
	uint8_t data[0];
} _PACKED_ rmt_prov_extened_scan_start_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_EXTENED_SCAN_REPORT)];
	rmt_prov_status_t status;
	uint8_t uuid[16];
	uint8_t oob_adv[0];
} _PACKED_ rmt_prov_extened_scan_report_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_GET)];
} _PACKED_ rmt_prov_link_get_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_OPEN)];
	uint8_t uuid[16];
	uint8_t link_open_timeout;
} _PACKED_ rmt_prov_link_open_prov_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_OPEN)];
	rmt_prov_dkri_procedure_t dkri_procedure;
} _PACKED_ rmt_prov_link_open_dkri_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_CLOSE)];
	rmt_prov_link_close_reason_t reason;
} _PACKED_ rmt_prov_link_close_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_STATUS)];
	rmt_prov_status_t status;
	rmt_prov_link_state_t link_state;
} _PACKED_ rmt_prov_link_status_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_LINK_REPORT)];
	rmt_prov_status_t status;
	rmt_prov_link_state_t link_state;
	rmt_prov_link_close_reason_t reason;
} _PACKED_ rmt_prov_link_report_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_PDU_SEND)];
	uint8_t outbound_pdu_num;
	uint8_t pdu[0];
} _PACKED_ rmt_prov_pdu_send_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_PDU_OUTBOUND_REPORT)];
	uint8_t outbound_pdu_num;
} _PACKED_ rmt_prov_pdu_outbound_report_t;

typedef struct {
	uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_RMT_PROV_PDU_REPORT)];
	uint8_t inbound_pdu_num;
	uint8_t pdu[0];
} _PACKED_ rmt_prov_pdu_report_t;

/** @} */

/**
 * @defgroup REMOTE_PROVISION_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define RMT_PROV_SERVER_ADV_STRUCTS_GET                         0 //!< @ref rmt_prov_server_adv_structs_get_t

typedef struct {
	uint8_t ad_type_filter_count;
	uint8_t *pad_type_filter;
	/* app shall fill this field */
	uint8_t adv_strcuts[31];
	uint8_t adv_structs_len;
} rmt_prov_server_adv_structs_get_t;
/** @} */

/**
 * @defgroup REMOTE_PROVISION_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define RMT_PROV_CLIENT_SCAN_CAPS_STATUS                        0 //!< @ref rmt_prov_client_scan_caps_status_t
#define RMT_PROV_CLIENT_SCAN_STATUS                             1 //!< @ref rmt_prov_client_scan_status_t
#define RMT_PROV_CLIENT_SCAN_REPORT                             2 //!< @ref rmt_prov_client_scan_report_t
#define RMT_PROV_CLIENT_EXTENED_SCAN_REPORT                     3 //!< @ref rmt_prov_client_extened_scan_report_t
#define RMT_PROV_CLIENT_LINK_STATUS                             4 //!< @ref rmt_prov_client_link_status_t
#define RMT_PROV_CLIENT_LINK_REPORT                             5 //!< @ref rmt_prov_client_link_report_t

typedef struct {
	uint16_t src;
	uint8_t max_scanned_items;
	bool support_active_scan;
} rmt_prov_client_scan_caps_status_t;

typedef struct {
	uint16_t src;
	rmt_prov_status_t status;
	rmt_prov_scan_state_t scan_state;
	uint8_t scanned_items_limit;
	uint8_t timeout;
} rmt_prov_client_scan_status_t;

typedef struct {
	uint16_t src;
	int8_t rssi;
	uint8_t uuid[16];
	uint8_t *poob;
	uint8_t *puri_hash;
} rmt_prov_client_scan_report_t;

typedef struct {
	uint16_t src;
	uint8_t uuid[16];
	uint8_t *poob;
	uint8_t *padv_structs;
	uint16_t adv_structs_len;
} rmt_prov_client_extened_scan_report_t;

typedef struct {
	uint16_t src;
	rmt_prov_status_t status;
	rmt_prov_link_state_t link_state;
} rmt_prov_client_link_status_t;

typedef struct {
	uint16_t src;
	rmt_prov_status_t status;
	rmt_prov_link_state_t link_state;
	rmt_prov_link_close_reason_t *preason;
} rmt_prov_client_link_report_t;
/** @} */

/**
 * @defgroup REMOTE_PROVISION_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register remote provisioning server model
 * @param[in] element_index: element index that model registered to
 * @retval true: register success
 * @retval false: register failed
 */
bool rmt_prov_server_reg(uint8_t element_index);

/**
 * @brief set model data callback
 * @param[in] pcb: model data callback
 */
void rmt_prov_server_set_cb(model_data_cb_pf pcb);

/**
 * @brief set remote provisioning server capabilites
 * @param[in] max_scanned_items: max scanned items value
 * @param[in] active_scan_state: active scan state value
 * @return set status
 */
bool rmt_prov_server_set_capabilites(uint8_t max_scanned_items,
									 rmt_prov_active_scan_state_t active_scan_state);

/**
 * @brief stop scan
 */
void rmt_prov_server_scan_stop(void);

/**
 * @brief report scan information
 * @param[in] dst: scan report destination address
 * @param[in] net_key_index: scan report used network key index
 * @param[in] rssi: received rssi value
 * @param[in] uuid: received uuid value
 * @param[in] oob: received oob value
 * @param[in] uri_hash: received uri hash value
 * @return send status
 */
mesh_msg_send_cause_t rm_prov_scan_report(uint16_t dst, uint16_t net_key_index, int8_t rssi,
		uint8_t uuid[16], uint8_t oob[2], uint8_t uri_hash[4]);

/**
 * @brief report extened scan information
 * @param[in] dst: extened scan report destination address
 * @param[in] net_key_index: extened scan report used network key index
 * @param[in] status: rextened scan status
 * @param[in] uuid: received uuid value
 * @param[in] oob: received oob value
 * @param[in] adv_data: scanned extened adv data
 * @param[in] adv_data_len: scanned extened adv data length
 * @return send status
 */
mesh_msg_send_cause_t rm_prov_extened_scan_report(uint16_t dst, uint16_t net_key_index,
		rmt_prov_status_t status, uint8_t uuid[16], uint8_t oob[2], uint8_t adv_data[],
		uint8_t adv_data_len);

/**
 * @brief handle uprovision device beacon
 * @param[in] bt_addr: device bt address
 * @param[in] bt_addr_type: device bt address type
 * @param[in] rssi: rssi received
 * @param[in] pudb: udb information
 * @param[in] len: udb information length
 */
void rmt_prov_server_handle_udb(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi,
								beacon_udb_t *pudb, uint8_t len);

/**
 * @brief handle provision service
 * @param[in] bt_addr: device bt address
 * @param[in] bt_addr_type: device bt address type
 * @param[in] rssi: rssi received
 * @param[in] pservice_data: provision service information
 * @param[in] len: udb information length
 */
void rmt_prov_server_handle_prov(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi,
								 mesh_service_data_t *pservice_data, uint8_t len);

/**
 * @brief handle connection state change
 * @param[in] conn_id: connection id
 * @param[in] conn_state: new connection state
 * @param[in] disc_cause: disconnect cause
 */
void rmt_prov_server_handle_conn_state_change(uint8_t conn_id, uint8_t new_state,
		uint16_t disc_cause);


/**
 * @brief get remote provision procedure
 * @return remote provision procedure
 */
rmt_prov_procedure_t rmt_prov_server_procedure_get(void);

/**
 * @brief handle provision timeout
 * @param[in] ptimer: timer handle
 */
void rmt_prov_handle_timeout_dev(void *ptimer);

/**
 * @brief handle externed scan data
 * @param[in] adv_report_type: adv report type
 * @param[in] bt_addr: device bt address
 * @param[in] bt_addr_type: device bt address type
 * @param[in] rssi: rssi received
 * @param[in] pbuffer: scan response data
 * @param[in] len: scan response data length
 */
void rmt_prov_server_extened_scan_handle(gap_sched_adv_report_type_t adv_report_type,
		uint8_t bt_addr[6], uint8_t addr_type, int8_t rssi, uint8_t *pbuffer, uint16_t buf_len);

/**
 * @brief link close
 * @param[in] status: provision status
 * @param[in] reason: link close reason
 */
void rmt_prov_server_link_close(rmt_prov_status_t status, uint8_t reason);

/**
 * @brief handle net key delete
 * @param[in] net_key_index: net key local index
 */
void rmt_prov_server_net_key_delete(uint16_t net_key_index);

/**
 * @brief clear scanned items
 */
void rmt_prov_scanned_items_clear(void);

/**
 * @brief remote provision node reset
 */
void rmt_prov_node_reset(void);

/**
 * @brief initialize remote provision
 */
void prov_init_rmt(void);

/**
 * @brief set provision prefer bearer
 * @param[in] bearer: prefer bearer
 */
void rmt_prov_set_prefer_bearer(bearer_field_t bearer);

/** @} */

/**
 * @defgroup REMOTE_PROVISION_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */
/**
 * @brief register remote provision client
 * @param[in] element_index: element index that model registered to
 * @param[in] pcb: client data callback function
 * @retval true: register success
 * @retval false: register failed
 */
bool rmt_prov_client_reg(uint8_t element_index, model_data_cb_pf pcb);

/**
 * @brief get scan capabilites
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_scan_caps_get(uint16_t dst, uint16_t net_key_index);

/**
 * @brief get scan status
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_scan_get(uint16_t dst, uint16_t net_key_index);

/**
 * @brief start scan
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] scanned_items_limit: max scanned items
 * @param[in] scan_timeout: scan timeout, unit is second
 * @param[in] uuid: scan uuid
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_scan_start(uint16_t dst, uint16_t net_key_index,
		uint8_t scanned_items_limit, uint8_t scan_timeout, uint8_t uuid[16]);

/**
 * @brief stop scan
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_scan_stop(uint16_t dst, uint16_t net_key_index);

/**
 * @brief start extened scan
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] ad_type_filter_cnt: ad type filter count
 * @param[in] pad_type_filter: ad type filter value
 * @param[in] uuid: remote device uuid
 * @param[in] timeout: extened scan timeout
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_extened_scan_start(uint16_t dst, uint16_t net_key_index,
		uint8_t ad_type_filter_cnt, uint8_t *pad_type_filter, uint8_t uuid[16], uint8_t timeout);

/**
 * @brief get remote link status
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_link_get(uint16_t dst, uint16_t net_key_index);

/**
 * @brief link open for provision
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] uuid: remote device uuid
 * @param[in] link_open_timeout: link open timeout time
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_link_open_prov(uint16_t dst, uint16_t net_key_index,
		uint8_t uuid[16], uint8_t link_open_timeout);

/**
 * @brief link open for dkri
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] dkri_procedure: dkri procedure
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_link_open_dkri(uint16_t dst, uint16_t net_key_index,
		rmt_prov_dkri_procedure_t dkri_procedure);

/**
 * @brief close remote provision link
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] reason: link close reason
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_link_close(uint16_t dst, uint16_t net_key_index,
		rmt_prov_link_close_reason_t reason);

/**
 * @brief send remote provision pdu
 * @param[in] dst: destination address
 * @param[in] net_key_index: network key index
 * @param[in] outbound_pdu_num: outbound pdu number
 * @param[in] pdata: outbound pdu data
 * @param[in] data_len: outbound pdu data length
 * @return send result
 */
mesh_msg_send_cause_t rmt_prov_client_pdu_send(uint16_t dst, uint16_t net_key_index,
		uint8_t outbound_pdu_num, uint8_t *pdata, uint8_t data_len);

/**
 * @brief handle provision timeout
 * @param[in] ptimer: timer handle
 */
void rmt_prov_handle_timeout_prov(void *ptimer);

/**
 * @brief close remote provision link
 * @param[in] reason: link close reason
 */
void rmt_prov_link_close(rmt_prov_link_close_reason_t reason);

/**
 * @brief get remote provision client link state
 * @return remote provision client link state
 */
rmt_prov_link_state_t rmt_prov_client_link_state(void);

/**
 * @brief get remote provision procedure
 * @return remote provision procedure
 */
rmt_prov_procedure_t rmt_prov_client_procedure(void);

/**
 * @brief get remote provision dkri procedure
 * @return remote provision dkri procedure
 */
rmt_prov_dkri_procedure_t rmt_prov_dkri_procedure(void);

/**
 * @brief refresh device key
 * @param[in] attn_dur: attention duration
 * @return refresh status
 */
bool rmt_prov_client_refresh_dev_key(uint8_t attn_dur);

/**
 * @brief refresh node address
 * @param[in] node_addr: node address
 * @param[in] attn_dur: attention duration
 * @return refresh status
 */
bool rmt_prov_client_refresh_node_addr(uint16_t node_addr, uint8_t attn_dur);

/**
 * @brief refresh composition data
 * @param[in] attn_dur: attention duration
 * @return refresh status
 */
bool rmt_prov_client_refresh_compo_data(uint8_t attn_dur);

/** @} */
/** @} */

END_DECLS

#endif /* MESH_RPR */

#endif /* _REMOTE_PROVISIONING_H */
