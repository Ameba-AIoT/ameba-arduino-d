/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_access.h
  * @brief    Head file for mesh access layer.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_ACCESS_H
#define _MESH_ACCESS_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Mesh_Access
  * @{
  */

/** @defgroup Mesh_Access_Exported_Macros Exported Macros
  * @brief
  * @{
  */

/** @brief the length includes the length of the access opcode
  * @{
  */
#define ACCESS_PAYLOAD_UNSEG_MAX_SIZE           11
#define ACCESS_PAYLOAD_SEG_ONE_MAX_SIZE         8
#define ACCESS_PAYLOAD_SEG_TWO_MAX_SIZE         20
#define ACCESS_PAYLOAD_SEG_THREE_MAX_SIZE       32
#define ACCESS_PAYLOAD_MAX_SIZE                 380 //!< segmented
/** @} */

/** @defgroup Access_Opcode Access Opcode
  * @brief
  * @{
  */
/** #pragma diag_suppress 175 */
#define ACCESS_OPCODE_SIZE(opcode)              ((opcode) >= 0xc00000 ? 3 : ((opcode) >= 0x8000 ? 2 : 1))
#define ACCESS_OPCODE_BYTE(pbuffer, opcode)\
    do\
    {\
        if(ACCESS_OPCODE_SIZE(opcode) == 1)\
        {\
            *((uint8_t *)(pbuffer)) = (opcode) & 0xff;\
        }\
        else if(ACCESS_OPCODE_SIZE(opcode) == 2)\
        {\
            *((uint8_t *)(pbuffer)) = ((opcode) >> 8) & 0xff;\
            *((uint8_t *)(pbuffer) + 1) = (opcode) & 0xff;\
        }\
        else\
        {\
            *((uint8_t *)(pbuffer)) = ((opcode) >> 16) & 0xff;\
            *((uint8_t *)(pbuffer) + 1) = ((opcode) >> 8) & 0xff;\
            *((uint8_t *)(pbuffer) + 2) = (opcode) & 0xff;\
        }\
    } while(0)
/** @} End of Access_Opcode */

/** @} */

/** @defgroup Mesh_Access_Exported_Types Exported Types
  * @brief
  * @{
  */

/** @brief synchronized return value when send a mesh message */
typedef enum
{
    MESH_MSG_SEND_CAUSE_SUCCESS,
    MESH_MSG_SEND_CAUSE_INVALID_ACCESS_OPCODE,
    MESH_MSG_SEND_CAUSE_INVALID_ELEMENT,
    MESH_MSG_SEND_CAUSE_INVALID_MODEL,
    MESH_MSG_SEND_CAUSE_NODE_UNPROVISIONED,
    MESH_MSG_SEND_CAUSE_INVALID_SRC,
    MESH_MSG_SEND_CAUSE_INVALID_DST,
    MESH_MSG_SEND_CAUSE_PAYLOAD_SIZE_EXCEED,
    MESH_MSG_SEND_CAUSE_PAYLOAD_SIZE_EXCEED1, //!< 8
    MESH_MSG_SEND_CAUSE_INVALID_VIR_ADDR,
    MESH_MSG_SEND_CAUSE_INVALID_APP_KEY_INDEX,
    MESH_MSG_SEND_CAUSE_INVALID_APP_KEY_STATE,
    MESH_MSG_SEND_CAUSE_APP_KEY_NOT_BOUND_TO_MODEL,
    MESH_MSG_SEND_CAUSE_INVALID_NET_KEY_INDEX,
    MESH_MSG_SEND_CAUSE_INVALID_NET_KEY_STATE,
    MESH_MSG_SEND_CAUSE_NO_BUFFER_AVAILABLE,
    MESH_MSG_SEND_CAUSE_NO_MEMORY, //!< 16
    MESH_MSG_SEND_CAUSE_TRANS_TX_BUSY,
    MESH_MSG_SEND_CAUSE_INVALID_ACCESS_PARAMETER
} mesh_msg_send_cause_t;

/** @brief asynchronized callback value after sending a mesh message */
typedef enum _mesh_msg_send_stat_t
{
    MESH_MSG_SEND_STAT_FAIL,
    MESH_MSG_SEND_STAT_SENT, //!< unseg access msg
    MESH_MSG_SEND_STAT_ACKED, //!< seg access msg
    MESH_MSG_SEND_STAT_ACKED_CANCEL,
    MESH_MSG_SEND_STAT_ACKED_OBO, //!< seg access msg acked by fn
    MESH_MSG_SEND_STAT_ACKED_OBO_CANCEL,
    MESH_MSG_SEND_STAT_TIMEOUT, //!< seg access msg
    MESH_MSG_SEND_STAT_CANCEL //!< seg msg canceled by myself due to some reasons like seq run out etc.
} mesh_msg_send_stat_t;

/** @brief the access layer pdu */
typedef struct
{
    /** variable length access message, composed of Operation Code and Parameters */
    uint8_t payload[ACCESS_PAYLOAD_MAX_SIZE];
} access_msg_t;

typedef struct _mesh_msg_t *mesh_msg_p;

/** @} */

/** @defgroup Mesh_Access_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief cfg the mesh message use the default or the publishing parameters
  *
  * The caller may change any parameters later if she isn't satisfied with the default values.
  * @param[in] pmesh_msg: the mesh message
  * @return operation result
  */
mesh_msg_send_cause_t access_cfg(mesh_msg_p pmesh_msg);

/**
  * @brief send the mesh message to the access layer
  *
  * @param[in] pmesh_msg: the mesh message
  * @return operation result
  */
mesh_msg_send_cause_t access_send(mesh_msg_p pmesh_msg);

/**
  * @brief receive the mesh message at the access layer
  *
  * @param[in] pmesh_msg: the mesh message
  * @return none
  */
void access_receive(mesh_msg_p pmesh_msg);

/**
  * @brief dispatch the mesh message at the access layer
  *
  * @param[in] pmesh_msg: the mesh message
  * @return none
  */
void access_dispatch(mesh_msg_p pmesh_msg);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_ACCESS_H */

