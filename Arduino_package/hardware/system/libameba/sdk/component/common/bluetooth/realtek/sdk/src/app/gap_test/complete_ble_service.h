/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     complete_ble_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _COMPLETE_BLE_SERVICE_H_
#define _COMPLETE_BLE_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_server.h>
#include <simple_ble_config.h>

/** @addtogroup PROFILE_API Profile APIs
  * @{
  */


/** @addtogroup Bluetooth_Services Bluetooth Services
  * @{
  */

/** @defgroup SIMP_Service Simple Ble Service
  * @brief Simple BLE service
  * @{
  */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Macros SIMP Service Exported Macros
  * @brief
  * @{
  */

/** @defgroup SIMP_Service_Application_Parameters SIMP Service Application Parameters
  * @brief  Type of parameters set/got from application.
  * @{
  */
typedef enum
{
    SIMPLE_BLE_SERVICE_PARAM_V1_READ_CHAR_VAL = 0x01,
    SIMPLE_BLE_SERVICE_PARAM_V7_LEN           = 0x02,
} T_SIMP_PARAM_TYPE;

/** @} */



/** @defgroup SIMP_Service_Upstream_Message SIMP Service Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */
/** @brief  Index of each characteristic in Demo Profile service database. */
#define SIMPLE_BLE_SERVICE_CHAR_V1_READ_INDEX               0x02
#define SIMPLE_BLE_SERVICE_CHAR_V2_WRITE_INDEX              0x05
#define SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX             0x07
#define SIMPLE_BLE_SERVICE_CHAR_V4_INDICATE_INDEX           0x0a
#define SIMPLE_BLE_SERVICE_CHAR_NOTIFY_CCCD_INDEX (SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX + 1)
#define SIMPLE_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX (SIMPLE_BLE_SERVICE_CHAR_V4_INDICATE_INDEX + 1)
#define SIMPLE_BLE_SERVICE_CHAR_V6_WRITE_INDEX              0xd
#define SIMPLE_BLE_SERVICE_CHAR_V7_READ_LONG_INDEX          0xf
#define SIMPLE_BLE_SERVICE_CHAR_V8_NOTIFY_INDICATE_INDEX    0x11
#define SIMPLE_BLE_SERVICE_CHAR_V8_CCCD_INDEX (SIMPLE_BLE_SERVICE_CHAR_V8_NOTIFY_INDICATE_INDEX + 1)

/** @defgroup SIMP_Service_Read_Info SIMP Service Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define SIMP_READ_V1                                        1
#define SIMP_READ_V7                                        2
/** @} */

/** @defgroup SIMP_Service_Write_Info SIMP Service Write Info
  * @brief  Parameter for writing characteristic value.
  * @{
  */
#define SIMP_WRITE_V2                                       1
#define SIMP_WRITE_V6                                       2
/** @} */

/** @defgroup SIMP_Service_Notify_Indicate_Info SIMP Service Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define SIMP_NOTIFY_INDICATE_V3_ENABLE     1
#define SIMP_NOTIFY_INDICATE_V3_DISABLE    2
#define SIMP_NOTIFY_INDICATE_V4_ENABLE     3
#define SIMP_NOTIFY_INDICATE_V4_DISABLE    4
#define SIMP_NOTIFY_INDICATE_V8_NOTIFY_ENABLE          7
#define SIMP_NOTIFY_INDICATE_V8_INDICATE_ENABLE        8
#define SIMP_NOTIFY_INDICATE_V8_NOTIFY_INDICATE_ENABLE 9
#define SIMP_NOTIFY_INDICATE_V8_DISABLE                10
/** @} */

#define SIMP_READ_V7_MAX_LEN               300

/** @} End of SIMP_Service_Upstream_Message */



/** @} End of SIMP_Service_Exported_Macros */
/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Types SIMP Service Exported Types
  * @brief
  * @{
  */

/** @defgroup TSIMP_WRITE_MSG TSIMP_WRITE_MSG
  * @brief Simple BLE service written msg to application.
  * @{
  */
typedef struct
{
    uint8_t opcode; //!< ref: @ref SIMP_Control_Point_OpCodes, @ref SIMP_Service_Write_Info
    T_WRITE_TYPE write_type;
    uint16_t len;
    uint8_t *p_value;
} TSIMP_WRITE_MSG;
/** @} End of TSIMP_WRITE_MSG */


/** @defgroup TSIMP_UPSTREAM_MSG_DATA TSIMP_UPSTREAM_MSG_DATA
  * @brief Simple BLE service callback message content.
  * @{
  */
typedef union
{
    uint8_t notification_indification_index; //!< ref: @ref SIMP_Service_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref SIMP_Service_Read_Info
    TSIMP_WRITE_MSG write;
} TSIMP_UPSTREAM_MSG_DATA;
/** @} End of TSIMP_UPSTREAM_MSG_DATA */

/** @defgroup TSIMP_CALLBACK_DATA TSIMP_CALLBACK_DATA
  * @brief Simple BLE service data to inform application.
  * @{
  */
typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    TSIMP_UPSTREAM_MSG_DATA msg_data;
} TSIMP_CALLBACK_DATA;
/** @} End of TSIMP_CALLBACK_DATA */

/** @} End of SIMP_Service_Exported_Types */
/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Functions SIMP Service Exported Functions
  * @brief
  * @{
  */

/**
  * @brief add Simple BLE service to application.
  *
  * @param[in] p_func pointer of app callback function called by profile.
  * @return service ID auto generated by profile layer.
  */
T_SERVER_ID simp_ble_service_add_service(void *p_func);

/**
  * @brief  Set service related data from application.
  *
  * @param[in] param_type            parameter type to set.
  * @param[in] len                   value length to be set.
  * @param[in] p_value             value to set.
  * @return parameter set result.
  * @retval 0 false
  * @retval 1 true
  */
bool simp_ble_service_set_parameter(T_SIMP_PARAM_TYPE param_type, uint16_t len, void *p_value);

/**
  * @brief send notification of simple notify characteristic value.
  *
  * @param[in] conn_id
  * @param[in] service_id        service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool simp_ble_service_send_v3_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                     void *p_value, uint16_t length);
/**
  * @brief send indication of simple indicate characteristic value.
  *
  * @param[in] conn_id           connection id
  * @param[in] service_id        service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic value length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool simp_ble_service_send_v4_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                                       void *p_value, uint16_t length);
/**
  * @brief send notification of simple notify characteristic value.
  *
  * @param[in] conn_id
  * @param[in] service_id         service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic value length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool simp_ble_service_simple_v8_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                       void *p_value,
                                       uint16_t length);

/**
  * @brief send indication of simple indicate characteristic value.
  *
  * @param[in] conn_id           connection id
  * @param[in] service_id         service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic value length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool simp_ble_service_simple_v8_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                                         void *p_value, uint16_t length);
/** @} End of SIMP_Service_Exported_Functions */

/** @} End of SIMP_Service */

/** @} End of Bluetooth_Services */

/** @} End of PROFILE_API */

#ifdef __cplusplus
}
#endif

#endif /* _SIMPLE_BLE_SERVICE_H_ */
