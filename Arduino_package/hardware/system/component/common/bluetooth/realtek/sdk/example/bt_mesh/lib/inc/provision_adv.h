/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     provision_adv.h
  * @brief    Head file for provision adv bearer.
  * @details
  * @author   bill
  * @date     2016-2-24
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROVISION_ADV_H_
#define _PROVISION_ADV_H_

/* Add Includes here */
#include "platform_os.h"
#include "provision_generic.h"

BEGIN_DECLS

/** @addtogroup PB_Adv
  * @brief
  * @{
  */

/** @defgroup PB_Adv_Exported_Macros Exported Macros
  * @{
  */
#define MESH_PB_ADV_PKT_OFFSET                       (2) //!< Length + AD type
#define MESH_PB_ADV_PKT_HEADER_SIZE                  (6) //!< linkId(4) + transNum(1) + Control(1)
#define MESH_PB_ADV_START_DATA_SIZE                  (20) //!< excluding the length and fcs field
#define MESH_PB_ADV_CONTINUE_DATA_SIZE               (23)
/** @} */

/** @defgroup Pb_Adv_Exported_Types Exported Types
  * @{
  */
typedef enum
{
    PB_ADV_LINK_STATE_CLOSED,
    PB_ADV_LINK_STATE_OPENING,
    PB_ADV_LINK_STATE_OPENED
} pb_adv_link_state_t;

typedef enum
{
    PB_ADV_RX_STATE_IDLE,
    PB_ADV_RX_STATE_START,
    PB_ADV_RX_STATE_CONTINUE
} pb_adv_rx_state_t;

typedef enum
{
    PB_ADV_TX_STATE_IDLE,
    PB_ADV_TX_STATE_SENDING //!< Wait for acknowledge
} pb_adv_tx_state_t;

typedef enum
{
    PB_ADV_ROLE_PROVISIONER,
    PB_ADV_ROLE_DEVICE
} pb_adv_role_t;

typedef enum
{
    PB_ADV_CB_TYPE_LINK_OPEN,
    PB_ADV_CB_TYPE_LINK_CLOSED,
    PB_ADV_CB_TYPE_TX_COMPLETE,
    PB_ADV_CB_TYPE_RX_COMPLETE
} pb_adv_cb_type_t;

typedef struct
{
    uint8_t total_length[2];
    uint8_t fcs;
    uint8_t data[MESH_PB_ADV_START_DATA_SIZE];
} _PACKED_ pb_adv_start_t, *pb_adv_start_p;

typedef struct
{
    uint8_t data[MESH_PB_ADV_CONTINUE_DATA_SIZE]; // variable length
} _PACKED_ pb_adv_continue_t, *pb_adv_continue_p;

typedef enum
{
    PB_ADV_LINK_OPEN,
    PB_ADV_LINK_ACK,
    PB_ADV_LINK_CLOSE
} pb_adv_opcode_t;

typedef struct
{
    uint8_t dev_uuid[16];
} _PACKED_ pb_adv_link_open_t, *pb_adv_link_open_p;

enum
{
    PB_ADV_LINK_CLOSE_SUCCESS, //!< link idle
    PB_ADV_LINK_CLOSE_TRANSACTION_TIMEOUT,
    PB_ADV_LINK_CLOSE_PROVISIONING_FAIL
} _SHORT_ENUM_;
typedef uint8_t pb_adv_link_close_reason_t;

typedef struct
{
    pb_adv_link_close_reason_t reason;
} _PACKED_ pb_adv_link_close_t, *pb_adv_link_close_p;

typedef union
{
    pb_adv_link_open_t link_open;
    // link_ack
    pb_adv_link_close_t link_close;
} _PACKED_ pb_adv_bearer_ctrl_t, *pb_adv_bearer_ctrl_p;

typedef struct
{
    prov_generic_ctrl_t control;
    union
    {
        pb_adv_start_t start;
        // ack
        pb_adv_continue_t conti; // continue: c keyword...
        pb_adv_bearer_ctrl_t bearer_ctrl;
    };
} _PACKED_ pb_adv_payload_t, *pb_adv_payload_p;

typedef struct
{
    uint8_t link_id[4];
    uint8_t trans_num;
    pb_adv_payload_t payload;
} _PACKED_ pb_adv_pkt_t, *pb_adv_pkt_p;

typedef struct
{
    pb_adv_link_state_t fsm;
    plt_timer_t timer; //!< multiple used for re-tx and supervision link idle (no tx, reset when rx)
    uint32_t timer_period;
    uint32_t timer_count;
    prov_bearer_cb_pf pf_cb;
    uint32_t link_id;
    uint8_t tx_trans_num;
    uint8_t *tx_buffer;
    uint16_t tx_len;
    uint8_t rx_trans_num;
    bool rx_first; //!< just for the iop with invalid trans implement
    uint8_t rx_fcs;
    uint8_t *rx_buffer;
    uint16_t rx_len;
    bool rx_frag_disorder; //!< PB_ADV_REQUIRE_FRAG_START
    uint8_t rx_frag_num;
    uint8_t rx_frag_buffer[8]; //!< only add 4 bytes from uint8_t *rx_frag_buffer
    uint8_t rmt_dev_uuid[16];
} pb_adv_ctx_t, *pb_adv_ctx_p;
extern pb_adv_ctx_t pb_adv_ctx[1];
/** @} */

/** @defgroup Pb_Adv_Exported_Functions Exported Functions
  * @{
  */
///@cond
uint8_t pb_adv_init(prov_bearer_cb_pf pf_cb);
void pb_adv_deinit(void);
bool pb_adv_send(uint8_t ctx_index, uint8_t *pvalue, uint16_t len);
void pb_adv_receive(uint8_t *pdata, uint8_t len);
bool pb_adv_link_check(void);
void pb_adv_handle_timeout(uint32_t period);
///@endcond

/**
  * @brief open the pb adv link
  * @param[in] ctx_index: the ctx idx
  * @param[in] dev_uuid: remote device uuid
  * @return send result
  */
bool pb_adv_link_open(uint8_t ctx_index, uint8_t dev_uuid[16]);

/**
  * @brief close the pb adv link
  * @param[in] ctx_index: the ctx idx
  * @param[in] reason: close reason
  * @return send result
  */
bool pb_adv_link_close(uint8_t ctx_index, pb_adv_link_close_reason_t reason);

/** @} */
/** @} */

END_DECLS

#endif /* _PROVISION_ADV_H_ */
