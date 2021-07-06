/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_transport.h
  * @brief    Head file for mesh transport layer.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_TRANSPORT_H
#define _MESH_TRANSPORT_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Mesh_Transport
  * @{
  */

/** @defgroup Mesh_Transport_Exported_Macros Exported Macros
  * @brief
  * @{
  */
#define TRANS_PDU_OFFSET                   (BEARER_ADV_HEADER_SIZE + MESH_NWK_HEADER_SIZE)
/** @} */

/** @defgroup Mesh_Transport_Exported_Types Exported Types
  * @brief
  * @{
  */
typedef enum
{
    TRANS_MSG_TYPE_UNSEG_ACCESS, //!< Unsegmented Access message
    TRANS_MSG_TYPE_SEG_ACCESS, //!< Segmented Access message
    TRANS_MSG_TYPE_UNSEG_CTL,
    TRANS_MSG_TYPE_SEG_CTL
} trans_msg_type_t;

typedef enum
{
    TRANS_SEG_ACK,
    TRANS_FRND_POLL,
    TRANS_FRND_UPDATE,
    TRANS_FRND_REQ,
    TRANS_FRND_OFFER,
    TRANS_FRND_CLEAR,
    TRANS_FRND_CLEAR_CONF,
    TRANS_FRND_SUB_LIST_ADD,
    TRANS_FRND_SUB_LIST_RM,
    TRANS_FRND_SUB_LIST_CONF,
    TRANS_HB,
#if MESH_SUPPORT_TRANS_PING
    TRANS_PING = 0x50,
    TRANS_PONG,
#endif
} trans_opcode_t;

typedef struct
{
    uint8_t aid: 6; //!< Least significant 6 bits of application key identifier
    uint8_t akf: 1; //!< Application Key Flag
    uint8_t seg: 1;
    uint8_t app_payload[15];
} _PACKED_ trans_unseg_access_t, *trans_unseg_access_p;

typedef struct
{
    uint32_t seg_n: 5;
    uint32_t seg_o: 5;
    uint32_t seq_zero: 13;
    uint32_t szmic: 1; //!< Size of TransMIC bit
} _PACKED_ trans_seg_msc_t, *trans_seg_msc_p;

typedef struct
{
    uint8_t aid: 6; //!< Least significant 5 bits of application key identifier
    uint8_t akf: 1; //!< Application Key Flag
    uint8_t seg: 1;
    union
    {
        trans_seg_msc_t msc; // shall be changed to big endian
        uint8_t msc_alias[3]; //!< miscellaneous @ref trans_seg_msc_t
    };
    uint8_t app_payload[12];
} _PACKED_ trans_seg_access_t, *trans_seg_access_p;

typedef struct
{
    uint8_t opcode: 7; //!< Reserved for future use
    uint8_t seg: 1;
    uint8_t params[11];
} _PACKED_ trans_unseg_ctl_t, *trans_unseg_ctl_p;

typedef struct
{
    uint32_t seg_n: 5;
    uint32_t seg_o: 5;
    uint32_t seq_zero: 13;
    uint32_t rfu: 1;
} _PACKED_ trans_seg_ctl_msc_t, *trans_seg_ctl_msc_p;

typedef struct
{
    uint8_t opcode: 7; //!< Transport Layer Control PDU Operation Code, @ref trans_opcode_t
    uint8_t seg: 1;
    uint8_t msc[3]; //!< miscellaneous @ref trans_seg_ctl_msc_t
    uint8_t params[8];
} _PACKED_ trans_seg_ctl_t, *trans_seg_ctl_p;

typedef struct
{
    uint16_t rfu: 2;
    uint16_t seq_zero: 13;
    uint16_t obo: 1; //!< Friend on behalf of a LPN
    uint8_t block_ack[4];
} _PACKED_ trans_seg_ack_t, *trans_seg_ack_p;

typedef struct
{
    uint8_t fsn: 1; //!< Friend Sequence Number
    uint8_t rfu: 7;
} _PACKED_ frnd_poll_t, *frnd_poll_p;

typedef struct
{
    union
    {
        struct
        {
            uint8_t key_refresh: 1;
            uint8_t iv_update: 1;
            uint8_t rfu: 6;
        } flags;
        uint8_t flags_alias;
    };
    uint8_t iv_index[4];
    uint8_t md; //!< More Data bit
} _PACKED_ frnd_update_t, *frnd_update_p;

/** @defgroup FRIENDSHIP_MSG  Friendship Msg
  * @brief Msg types used by friendship models
  * @{
  */
typedef struct
{
    uint8_t min_queue_size_log: 3; //!< N = 2^cache_size
    uint8_t rx_window_factor: 2;
    uint8_t rssi_factor: 2;
    uint8_t rfu: 1; //!< Reserved for Future Use
} _PACKED_ frnd_criteria_t, *frnd_criteria_p;

typedef struct
{
    frnd_criteria_t criteria; //!< Friend Minimum Requirements
    uint8_t rx_delay; //!< Friend Receive Delay
    uint8_t poll_to[3]; //!< Friend Poll Timeout
    uint16_t pr_addr; //!< Previous Friend node's Unicast Address
    uint8_t element_num; //!< Number of Elements on the Low Power node
    uint16_t lpn_counter;
} _PACKED_ frnd_req_t, *frnd_req_p;

typedef struct
{
    uint8_t rx_window; //!< Receive Window value supported by the Friend node.
    uint8_t queue_size;
    uint8_t sub_list_size;
    int8_t rssi;
    uint16_t frnd_counter;
} _PACKED_ frnd_offer_t, *frnd_offer_p;

typedef struct
{
    uint16_t lpn_addr; //!< Low Power Node Unicast Address
    uint16_t lpn_counter;
} _PACKED_ frnd_clear_t, *frnd_clear_p;

typedef struct
{
    uint16_t lpn_addr; //!< Low Power Node Unicast Address
    uint16_t lpn_counter;
} _PACKED_ frnd_clear_conf_t, *frnd_clear_conf_p;

typedef struct
{
    uint8_t tran_num;
    uint16_t group_addr[5]; //!< List of Group Addresses and Virtual Addresses.
} _PACKED_ frnd_sub_list_add_t, *frnd_sub_list_add_p;

typedef struct
{
    uint8_t tran_num;
    uint16_t group_addr[5]; //!< List of Group Addresses and Virtual Addresses.
} _PACKED_ frnd_sub_list_remove_t, *frnd_sub_list_remove_p;

typedef struct
{
    uint8_t tran_num;
} _PACKED_ frnd_sub_list_conf_t, *frnd_sub_list_conf_p;
/** @} */

typedef struct
{
    uint8_t init_ttl: 7;
    uint8_t rfu: 1;
    uint16_t features; //!< @ref mesh_features_t
} _PACKED_ trans_hb_t, *trans_hb_p;

#if MESH_SUPPORT_TRANS_PING
typedef struct
{
    uint8_t init_ttl; //!< Initial TTL value used when sending this message.
    uint16_t pong_max_delay; //!< unit: 10ms
} _PACKED_ trans_ping_t, *trans_ping_p;

typedef struct
{
    uint8_t init_ttl; //!< Initial TTL value used when sending this message.
    uint8_t hops_forward; //!< Hops in the forward direction.
    uint16_t pong_delay; //!< unit: 10ms
} _PACKED_ trans_pong_t, *trans_pong_p;
#endif

typedef enum
{
    TRANS_SEG_MSG_SEND_STAT_GOING,
    TRANS_SEG_MSG_SEND_STAT_ACKED,
    TRANS_SEG_MSG_SEND_STAT_TO,
    TRANS_SEG_MSG_SEND_STAT_CANCEL
} trans_seg_msg_send_stat_t;

typedef struct
{
    uint8_t *pbuffer; //!< cache of msg
    uint16_t len; //!< length of seg msg
    mesh_msg_t seg_msg; //!< save origin seg msg info
    uint32_t seq_origin; //!< save origin seq
    plt_timer_t timer; //!< used to retransimit
    uint8_t times[32]; //!< tx times of each seg
    uint32_t send; //!< need tx flag of each seg
    uint32_t send_first; //!< need tx first time flag of each seg
    uint32_t ack; //!< seg acked info
    uint8_t seg_num;
} trans_tx_ctx_t, *trans_tx_ctx_p;

typedef struct
{
    uint8_t *pbuffer; //!< cache of msg
    uint16_t len; //!< length of seg msg
    mesh_msg_t seg_msg; //!< origin seg msg info
    //uint32_t seq_origin; //!< origin seq
    plt_timer_t timer; //!< ack timer, used to retransimit
    plt_timer_t timer1; //!< incomplete timer
    //uint8_t times[32]; //!< tx times of each seg
    uint32_t receive; //!< flag of each received seg
    //uint32_t ack; //!< seg acked info
    bool obo; //!< rx for lpn
    uint8_t seg_num;
} trans_rx_ctx_t, *trans_rx_ctx_p;

typedef enum
{
    PING_PONG_TYPE_TRANSPORT,
    PING_PONG_TYPE_ACCESS,
    PING_PONG_TYPE_ACCESS_BIG
} ping_pong_type_t;

typedef void (*pf_ping_cb_t)(uint16_t src, uint16_t dst, uint8_t hops_forward,
                             ping_pong_type_t type, uint8_t init_ttl, uint8_t key_index, uint16_t pong_max_delay);
typedef void (*pf_pong_cb_t)(uint16_t src, uint16_t dst, uint8_t hops_forward,
                             ping_pong_type_t type, uint8_t hops_reverse, uint16_t pong_delay);

/** @} */

/** @defgroup Mesh_Transport_Exported_Functions Exported Functions
  * @brief
  * @{
  */

///@cond
void trans_init(void);
void trans_deinit(void);
mesh_msg_send_cause_t trans_send(mesh_msg_p pmesh_msg);
bool trans_send_check(void);
void trans_receive(mesh_msg_p pmesh_msg);
void trans_handle_timeout(uint8_t type, void *pctx);
bool trans_handle_ping_pong_msg(mesh_msg_p pmesh_msg);
mesh_msg_send_cause_t trans_hb(uint16_t dst, uint16_t net_key_index, uint8_t init_ttl,
                               mesh_features_t features);
void trans_rx_ctx_clear(void);
///@endcond

/**
  * @brief send the transport ping
  *
  * The ping msg can be customized with the dst/ttl/net_key_index/pong_max_delay.
  * Take the concurrence of multiple devices into consideration, the pong_max_delay parameter is designed.
  * @param[in] dst: the destination address
  * @param[in] ttl: the initial ttl
  * @param[in] net_key_index: the net key index
  * @param[in] pong_max_delay: the max delay of correponded pong msg
  * @return send result
  */
mesh_msg_send_cause_t trans_ping(uint16_t dst, uint8_t ttl, uint16_t net_key_index,
                                 uint16_t pong_max_delay);

/**
  * @brief send the transport pong
  * @param[in] dst: the destination address
  * @param[in] ttl: the initial ttl
  * @param[in] net_key_index: the net key index
  * @param[in] hops_forward: the calculated hops in the forward direction
  * @param[in] pong_delay: the actual delay of the pong msg
  * @return send result
  */
mesh_msg_send_cause_t trans_pong(uint16_t dst, uint8_t ttl, uint16_t net_key_index,
                                 uint8_t hops_forward, uint16_t pong_delay);

/**
  * @brief initialize the transport ping pong
  *
  * Register the callback.
  * @param[in] pf_ping_cb: the ping callback
  * @param[in] pf_pong_cb: the pong callback
  * @return none
  */
void trans_ping_pong_init(pf_ping_cb_t pf_ping_cb, pf_pong_cb_t pf_pong_cb);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_TRANSPORT_H */

