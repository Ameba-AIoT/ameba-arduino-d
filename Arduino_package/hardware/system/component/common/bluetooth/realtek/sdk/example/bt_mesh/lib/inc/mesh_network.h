/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_network.h
  * @brief    Head file for mesh network layer.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_NETWORK_H
#define _MESH_NETWORK_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Mesh_Network
  * @{
  */

/** @defgroup Mesh_Network_Exported_Macros Exported Macros
  * @brief
  * @{
  */
#define MESH_NWK_HEADER_SIZE                    9
#define MESH_NWK_ENC_OFFSET                     7 //!< The encryption of network layer starts from the destincation address!
#define MESH_NWK_PRIVACY_HEADER_OFFSET          1
#define MESH_NWK_PRIVACY_HEADER_SIZE            6
#define MESH_NWK_PKT_MIN_SIZE                   (MESH_NWK_HEADER_SIZE + 4) //!< open interval
#define MESH_NWK_PKT_MIN_SIZE1                  (MESH_NWK_HEADER_SIZE + 8)

#define MESH_RELAY_MIN_TTL                      2
/** @} */

/** @defgroup Mesh_Network_Exported_Types Exported Types
  * @brief
  * @{
  */

/** @brief mesh network packet format */
typedef struct
{
    uint8_t nid: 7; //!< least significant 7 bits of network identity
    uint8_t ivi: 1; //!< least significant bit of initialization vector index
    uint8_t ttl: 7; //!< time to live
    uint8_t ctl: 1; //!< Network Control, NetMIC size & Access Message/Control Message indicator
    uint8_t seq[3]; //!< sequence number
    uint16_t src; //!< source address
    uint16_t dst; //!< destination address
    /** trans payload with a varying length of 1 ~ 16 bytes,
      * and 4 or 8 bytes network message integrity check (NetMIC)
      */
    uint8_t trans_payload[16 + 4];
} _PACKED_ net_pkt_t;

/** @} */

/** @defgroup Mesh_Network_Exported_Functions Exported Functions
  * @brief
  * @{
  */

///@cond
void nmc_init(void);
void nmc_deinit(void);
void nmc_clear(void);
bool nmc_check(uint32_t iv_index, uint16_t src, uint32_t seq);
void nmc_update(uint32_t iv_index, uint16_t src, uint32_t seq);
///@endcond

/**
  * @brief send mesh msg to the network layer
  * @param[in] pmesh_msg: mesh msg
  * @return send result
  */
mesh_msg_send_cause_t net_send(mesh_msg_p pmesh_msg);

/**
  * @brief send mesh msg to the bearer layer
  * @param[in] bearer_type: packet type
  * @param[in] pbuffer: mesh msg
  * @param[in] len: mesh msg length
  * @param[in] rssi: valid at the adv bearer
  * @return source address
  * @retval MESH_UNASSIGNED_ADDR: something wrong
  */
uint16_t net_receive(bearer_type_t bearer_type, uint8_t *pbuffer, uint16_t len, int8_t rssi);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_NETWORK_H */

