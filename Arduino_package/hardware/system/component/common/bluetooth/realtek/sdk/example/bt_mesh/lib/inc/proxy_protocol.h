/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     proxy_protocol.h
  * @brief    Head file for proxy protocol.
  * @details
  * @author   bill
  * @date     2016-11-26
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROXY_PROTOCOL_H_
#define _PROXY_PROTOCOL_H_

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

/** @addtogroup Proxy_Protocol
  * @{
  */

/** @defgroup Proxy_Protocol_Exported_Macros Exported Macros
  * @{
  */
#define MESH_PROXY_PKT_OFFSET                       3 //!< opcode + handle
#define MESH_PROXY_PKT_HEADER_SIZE                  1 //!< @ref proxy_pkt_header_t
/** @} */

/** @defgroup Proxy_Protocol_Exported_Types Exported Types
  * @{
  */
typedef enum
{
    PROXY_PDU_TYPE_NET_PDU,
    PROXY_PDU_TYPE_MESH_BEACON,
    PROXY_PDU_TYPE_PROXY_CFG,
    PROXY_PDU_TYPE_PROV_PDU
} proxy_pdu_type_t;

typedef enum
{
    PROXY_SAR_COMPLETE,
    PROXY_SAR_FIRST,
    PROXY_SAR_CONTINUATE,
    PROXY_SAR_LAST
} proxy_sar_t;

typedef struct
{
    uint8_t type: 6; //!< @ref proxy_pdu_type_t
    uint8_t sar: 2; //!< @ref proxy_sar_t
} _PACKED_ proxy_pkt_header_t, *proxy_pkt_header_p;

enum
{
    PROXY_CFG_OPCODE_SET_FILTER_TYPE,
    PROXY_CFG_OPCODE_ADD_ADDR,
    PROXY_CFG_OPCODE_REMOVE_ADDR,
    PROXY_CFG_OPCODE_FILTER_STAT
} _SHORT_ENUM_;
typedef uint8_t proxy_cfg_opcode_t;

enum
{
    PROXY_CFG_FILTER_TYPE_WHITE_LIST,
    PROXY_CFG_FILTER_TYPE_BLACK_LIST
} _SHORT_ENUM_;
typedef uint8_t proxy_cfg_filter_type_t;

typedef struct
{
    proxy_cfg_filter_type_t filter_type;
} _PACKED_ proxy_cfg_set_filter_type_t, *proxy_cfg_set_filter_type_p;

typedef struct
{
    uint8_t addr[2]; //!< variable length
} _PACKED_ proxy_cfg_add_addr_t, *proxy_cfg_add_addr_p;

typedef struct
{
    uint8_t addr[2]; //!< variable length
} _PACKED_ proxy_cfg_remove_addr_t, *proxy_cfg_remove_addr_p;

typedef struct
{
    proxy_cfg_filter_type_t filter_type;
    uint8_t list_size[2];
} _PACKED_ proxy_cfg_filter_stat_t, *proxy_cfg_filter_stat_p;

typedef struct
{
    proxy_cfg_opcode_t opcode;
    union
    {
        proxy_cfg_set_filter_type_t set_filter_type;
        proxy_cfg_add_addr_t add_addr;
        proxy_cfg_remove_addr_t remove_addr;
        proxy_cfg_filter_stat_t filter_stat;
    };
} _PACKED_ proxy_cfg_msg_t, *proxy_cfg_msg_p;

#define MESH_PROXY_PROTOCOL_ALL_CTX_ID          0xFF
#define MESH_PROXY_PROTOCOL_RSVD_CTX_ID         0xFE
typedef bool (*proxy_send_sar_pf)(gap_sched_link_t link, uint8_t *pvalue, uint16_t len);
typedef bool (*proxy_receive_cb_pf)(uint8_t ctx_id, proxy_pdu_type_t type, uint8_t *pvalue,
                                    uint16_t len);

typedef enum
{
    PROXY_CTX_TYPE_INVALID,
    PROXY_CTX_TYPE_PROV,
    PROXY_CTX_TYPE_PROXY,
} proxy_ctx_type_t;

typedef struct
{
    proxy_ctx_type_t ctx_type;
    bool data_out_cccd_enabled;
    gap_sched_link_t link;
    uint16_t net_key_index;
    uint16_t rx_len;
    uint8_t *rx_buffer;
    plt_timer_t rx_timer;
    proxy_receive_cb_pf pf_rx;
    proxy_send_sar_pf pf_tx;
    proxy_cfg_filter_type_t filter_type;
    uint16_t addr_count;
    mesh_addr_member_p addr_list;
} proxy_ctx_t, *proxy_ctx_p;
/** @} */

/** @defgroup Proxy_Protocol_Exported_Functions Exported Functions
  * @{
  */

///@cond
void proxy_send(uint8_t ctx_id, proxy_pdu_type_t type, uint8_t *pvalue, uint16_t len);
bool proxy_receive(uint8_t ctx_id, uint8_t *pvalue, uint16_t len);
bool proxy_cfg_receive(uint8_t ctx_id, uint8_t *pbuffer, uint16_t len);
bool proxy_cfg_handle_msg(uint8_t ctx_id, uint8_t *pvalue, uint16_t len);

void proxy_ctx_init(uint8_t proxy_count);
void proxy_ctx_deinit(void);

uint8_t proxy_ctx_allocate(proxy_send_sar_pf proxy_send_sar, proxy_receive_cb_pf proxy_receive_cb,
                           proxy_ctx_type_t ctx_type);
bool proxy_ctx_set_filter_type(uint8_t ctx_id, proxy_cfg_filter_type_t filter_type);
bool proxy_ctx_set_cb(uint8_t ctx_id, proxy_send_sar_pf proxy_send_sar,
                      proxy_receive_cb_pf proxy_receive_cb);
bool proxy_ctx_set_link(uint8_t ctx_id, gap_sched_link_t link_id);
gap_sched_link_t proxy_ctx_get_link(uint8_t ctx_id);
bool proxy_ctx_enable_data_out_cccd(uint8_t ctx_id, bool enable);
bool proxy_ctx_is_data_out_cccd_enabled(uint8_t ctx_id);
uint8_t proxy_ctx_id_get(gap_sched_link_t link, proxy_ctx_type_t ctx_type);
void proxy_ctx_disconnect_all(proxy_ctx_type_t ctx_type);
uint8_t proxy_ctx_active_count(proxy_ctx_type_t ctx_type);
uint8_t proxy_ctx_idle_count(void);
bool proxy_ctx_free(uint8_t ctx_id);

void proxy_filter_free(uint8_t ctx_id);
bool proxy_filter(uint8_t ctx_id, uint16_t addr);
void proxy_filter_update(uint8_t ctx_id, uint16_t addr);

void proxy_handle_timeout(uint8_t ctx_id);
///@endcond

/**
  * @brief set the proxy filter type
  * @param[in] ctx_id: the context idx
  * @param[in] filter_type: the filter type
  * @return send result
  */
bool proxy_cfg_set_filter_type(uint8_t ctx_id, proxy_cfg_filter_type_t filter_type);

/**
  * @brief set the proxy filter type
  * @param[in] ctx_id: the context idx
  * @param[in] add_remove: 1 - add, 0 - remove
  * @param[in] addr: the address list
  * @param[in] count: the address count
  * @return send result
  */
bool proxy_cfg_add_remove_addr(uint8_t ctx_id, bool add_remove, uint16_t addr[], uint16_t count);

/** @} */
/** @} */

END_DECLS

#endif /* _PROXY_PROTOCOL_H_ */
