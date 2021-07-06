/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     proxy_service.h
  * @brief    Head file for smart mesh proxy service.
  * @details
  * @author   bill
  * @date     2015-11-09
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROXY_SERVICE_H_
#define _PROXY_SERVICE_H_

/* Add Includes here */
#include "platform_types.h"
#include "proxy_protocol.h"

BEGIN_DECLS

/** @addtogroup Proxy_Service
  * @{
  */

/** @defgroup Proxy_Service_Exported_Constants Exported Constants
  * @{
  */

#define PROXY_PKT_OFFSET                            3 //!< opcode + handle
#define PROXY_PKT_HEADER_SIZE                       1 //!< @ref proxy_pkt_header_t

///@cond
/** @brief  Index of each characteristic in service database. */
#define MESH_PROXY_SERVICE_UUID                     0x1828
#define MESH_PROXY_DATA_IN_UUID                     0x2ADD
#define MESH_PROXY_DATA_OUT_UUID                    0x2ADE
///@endcond

/** @} */

/** @defgroup Proxy_Service_Exported_Types Exported Types
  * @brief
  * @{
  */
/** @} */

/** @defgroup Proxy_Service_Exported_Functions Exported Functions
  * @brief
  * @{
  */
///@cond
bool proxy_service_send(uint8_t ctx_id, proxy_pdu_type_t type, uint16_t dst, uint8_t *pvalue,
                        uint16_t len);
bool proxy_service_receive(uint8_t ctx_id, proxy_pdu_type_t type, uint8_t *pvalue, uint16_t len);
void proxy_service_handle_disc(gap_sched_link_t link);
///@endcond
/** @} */
/** @} */

END_DECLS

#endif /* _PROXY_SERVICE_H_ */
