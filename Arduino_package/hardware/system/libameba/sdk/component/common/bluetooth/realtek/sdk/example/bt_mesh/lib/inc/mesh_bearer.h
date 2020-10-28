/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_bearer.h
  * @brief    Head file for mesh bearer layer.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_BEARER_H
#define _MESH_BEARER_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Mesh_Bearer
  * @{
  */

/** @defgroup Mesh_Bearer_Exported_Macros Exported Macros
  * @brief
  * @{
  */

/* Add all public types here */
/** advertising bearer params */
#define BEARER_ADV_LEN_OFFSET                   0
#define BEARER_ADV_TYPE_OFFSET                  1
#define BEARER_ADV_HEADER_SIZE                  2 //!< length + flag
#define BEARER_ADV_PDU_OFFSET                   BEARER_ADV_HEADER_SIZE

#define RSSI_NOT_AVAILABLE                      0x7F
/** @} */

/** @defgroup Mesh_Bearer_Exported_Types Exported Types
  * @brief
  * @{
  */

/** mesh bearer type */
typedef enum
{
    BEARER_TYPE_LOOPBACK,
    BEARER_TYPE_ADV,
    BEARER_TYPE_GATT,
    BEARER_TYPE_OTHER
} bearer_type_t;

typedef enum
{
    BEARER_FIELD_LOOPBACK = BIT0,
    BEARER_FIELD_ADV = BIT1,
    BEARER_FIELD_GATT = BIT2,
    BEARER_FIELD_OTHER = BIT3,
    BEARER_FIELD_ALL = BIT0 | BIT1 | BIT2 | BIT3
} bearer_field_t;

typedef enum
{
    BEARER_PKT_TYPE_MESH_PKT,
    BEARER_PKT_TYPE_MESH_BEACON,
    BEARER_PKT_TYPE_MESH_PB_ADV,
    BEARER_PKT_TYPE_GATT_SERVICE_ADV, //!< connectable
    BEARER_PKT_TYPE_OTHER //!< default non-connectable
} bearer_pkt_type_t;

typedef enum
{
    BEARER_MESH_PKT_TYPE_NORMAL,
    BEARER_MESH_PKT_TYPE_RELAY
} bearer_mesh_pkt_type_t;

typedef struct
{
    uint8_t length; //!< just include flag
    uint8_t flag;
    uint8_t pdu[29];
} _PACKED_ bearer_adv_pkt_t, *bearer_adv_pkt_p;

typedef bool (*pf_bearer_other_send_t)(bearer_pkt_type_t pkt_type, uint16_t dst, uint8_t *pbuffer,
                                       uint16_t len);

/** @} */

/** @defgroup Mesh_Bearer_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief reg the send api of the other bearer
  *
  * The other bearer extends the mesh network.
  * @param[in] send: send api
  * @return none
  */
void bearer_other_reg(pf_bearer_other_send_t send);

/**
  * @brief receive from the other bearer
  *
  * The other bearer extends the mesh network.
  * @param[in] pkt_type: packet type
  * @param[in] pbuffer: packet
  * @param[in] len: packet length
  * @return none
  */
void bearer_other_receive(bearer_pkt_type_t pkt_type, uint8_t *pbuffer, uint16_t len);

/**
  * @brief receive from the loopback bearer
  * @param[in] pbuffer: packet
  * @param[in] len: packet length
  * @return none
  */
void bearer_loopback_receive(uint8_t *pbuffer, uint16_t len);

/**
  * @brief receive from the adv bearer
  * @param[in] adv_report_type: packet type
  * @param[in] bt_addr: packet
  * @param[in] addr_type: packet length
  * @param[in] rssi: rssi
  * @param[in] pbuffer: packet
  * @param[in] len: packet length
  * @return none
  */
void bearer_adv_receive(gap_sched_adv_report_type_t adv_report_type, uint8_t bt_addr[6],
                        uint8_t addr_type, int8_t rssi, uint8_t *pbuffer, uint16_t len);

/**
  * @brief get the state of the adv bearer
  *
  * The adv bearer is default on after initialization in @ref mesh_init.
  * @return the adv bearer state
  * @retval true: on
  * @retval false: off
  */
bool bearer_adv_get(void);

/**
  * @brief open or close the adv bearer
  * @param[in] on_off: the adv bearer state
  * @return none
  */
void bearer_adv_set(bool on_off);

/**
  * @brief send to the bearer layer
  * @param[in] pkt_type: packet type
  * @param[in] pbuffer: packet
  * @param[in] len: packet length
  * @return none
  */
void bearer_send(bearer_pkt_type_t pkt_type, uint8_t *pbuffer, uint16_t data_len);

/**
  * @brief send mesh msg to the bearer layer
  * @param[in] pkt_type: packet type
  * @param[in] pmesh_msg: mesh msg
  * @return none
  */
void bearer_send_mesh_msg(bearer_mesh_pkt_type_t pkt_type, mesh_msg_p pmesh_msg);

/**
  * @brief initialize the bearer
  * @return none
  */
void bearer_init(void);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_BEARER_H */

