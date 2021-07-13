/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     vendor_tp_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author   ethan
  * @date     2016-02-18
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _VENDOR_TP_SERVICE_H_
#define _VENDOR_TP_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"
#include "ble_throughput_vendor_tp_config.h"

/** @addtogroup SIMP SIMP
  * @brief Simple BLE Profile
  * @{
  */

/** @defgroup SIMP_Service SIMP Service
  * @brief Simple BLE Service
  * @{
  */

/** @defgroup SIMP_Service_Exported_Constants SIMP Service Exported Constants
  * @brief macros that other .c files may use all defined here
  * @{
  */

/** @defgroup SIMP_Service_Application_Parameters SIMP Service Application Parameters
  * @brief  Type of parameters set/got from application.
  * @{
  */
#define SIMPLE_BLE_SERVICE_PARAM_V1_READ_CHAR_VAL           0x01
#define SIMPLE_BLE_SERVICE_PARAM_CTL_PNT_PROG_CLR           0x02
/** @} */

///@cond
/** @brief  Index of each characteristic in Demo Profile service database. */
#define VENDOR_TP_SERVICE_CHAR_TP_WRITE_INDEX               0x02
#define VENDOR_TP_SERVICE_CHAR_TP_NOTIFY_INDICATE_CCCD_INDEX (VENDOR_TP_SERVICE_CHAR_TP_WRITE_INDEX + 1)
///@endcond

/** @defgroup SIMP_Service_Upstream_Message SIMP Service Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */

#define SIMP_WRITE_V1                                       1


/** @} */

/** @defgroup SIMP_Service_Notify_Indicate_Info SIMP Service Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define VENDOR_TP_SERVICE_V1_NOTIFICATION_ENABLE      1
#define VENDOR_TP_SERVICE_V1_NOTIFICATION_DISABLE     2
#define VENDOR_TP_SERVICE_V1_INDICATION_ENABLE          3
#define VENDOR_TP_SERVICE_V1_INDICATION_DISABLE         4



/** @} */

/** @} End of SIMP_Service_Upstream_Message */



/** @defgroup SIMP_Service_Exported_Types SIMP Service Exported Types
  * @brief  types that other.c files may use all defined here
  * @{
  */

/** @defgroup TSIMP_WRITE_MSG TSIMP_WRITE_MSG
  * @brief Simple BLE service written msg to application.
  * @{
  */

typedef enum
{
    VENDOR_TP_OP_CONFIG_NOTIFY_PARAM1 = 0x01,
    VENDOR_TP_OP_CONFIG_NOTIFY_PARAM2 = 0x02,
    VENDOR_TP_OP_NOTIFY_START_TEST = 0x03,

} VENDOR_TP_OP;

typedef struct _TVENDOR_TP_CONFIG_NOTIFY_PARAM
{
    uint32_t count;
    uint16_t length;
} TVENDOR_TP_CONFIG_NOTIFY_PARAM;

typedef struct
{
    uint16_t length;
    uint8_t  *p_value;
} TVENDOR_TP_WRITE_DATA;

typedef struct _TTP_WRITE_MSG
{
    T_WRITE_TYPE write_type;
    VENDOR_TP_OP opcode; //!< ref: @ref SIMP_Control_Point_OpCodes, @ref SIMP_Service_Write_Info
    union
    {
        TVENDOR_TP_CONFIG_NOTIFY_PARAM notify_param;
        TVENDOR_TP_WRITE_DATA write_data;
        uint8_t mode;
    } u;
} TTP_WRITE_MSG;
/** @} End of TSIMP_WRITE_MSG */


/** @defgroup TSIMP_UPSTREAM_MSG_DATA TSIMP_UPSTREAM_MSG_DATA
  * @brief Simple BLE service callback message content.
  * @{
  */
typedef union _TTP_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index; //!< ref: @ref SIMP_Service_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref SIMP_Service_Read_Info
    TTP_WRITE_MSG write;
} TTP_UPSTREAM_MSG_DATA;
/** @} End of TSIMP_UPSTREAM_MSG_DATA */

/** @defgroup TSIMP_CALLBACK_DATA TSIMP_CALLBACK_DATA
  * @brief Simple BLE service data to inform application.
  * @{
  */
typedef struct _TTP_CALLBACK_DATA
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    uint8_t                    conn_id;
    TTP_UPSTREAM_MSG_DATA    msg_data;
} TTP_CALLBACK_DATA;
/** @} End of TSIMP_CALLBACK_DATA */

/** @} End of SIMP_Service_Exported_Types */

/** @defgroup SIMP_Service_Exported_Functions SIMP Service Exported Functions
  * @brief functions that other .c files may use all defined here
  * @{
  */
T_SERVER_ID vendor_tp_service_add(void *pFunc);
void vendor_tp_service_config_param(TTP_PERFER_PARAM param);
bool vendor_tp_service_v1_notification(uint8_t conn_id, T_SERVER_ID service_id, void *p_value,
                                       uint16_t length);
bool vendor_tp_service_v1_indication(uint8_t conn_id, T_SERVER_ID service_id,  void *p_value,
                                     uint16_t length);


/** @} End of SIMP_Service_Exported_Functions */

/** @} End of SIMP_Service */

/** @} End of SIMP */

#ifdef __cplusplus
}
#endif

#endif /* _SIMPLE_BLE_SERVICE_H_ */
