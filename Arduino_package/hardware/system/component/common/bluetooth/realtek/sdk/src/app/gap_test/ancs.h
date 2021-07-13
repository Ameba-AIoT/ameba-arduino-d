/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      ancs.h
* @brief     ancs
* @details   ancs
* @author    ranhui
* @date      2015-03-27
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _ANCS_H__
#define _ANCS_H__

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <gap_le.h>
#include <app_msg.h>

/** @addtogroup  PERIPH_ANCS
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup PERIPH_ANCS_Exported_Macros ANCS Exported Macros
   * @{
   */
#define ANCS_MAX_ATTR_LEN 256 //!< Max ancs attribute length

#if F_BT_ANCS_GET_APP_ATTR
#define ANCS_APP_INDENTIFIER_MAX_LEN 30  //!< Max app indentifier length
#endif

#define ANCS_MSG_QUEUE_NUM 5  //!< ANCS message queue size


/**  @brief  ANCS event flag */
#define NS_EVENT_FLAG_SILENT          (1 << 0)
#define NS_EVENT_FLAG_IMPORTANT       (1 << 1)
#define NS_EVENT_FLAG_PRE_EXISTING    (1 << 2)
#define NS_EVENT_FLAG_POSITIVE_ACTION (1 << 3) //!<A positive action exists and is associated with this iOS notification
#define NS_EVENT_FLAG_NEGATIVE_ACTION (1 << 4) //!<A negative action exists and is associated with this iOS notification.

/** End of PERIPH_ANCS_Exported_Macros
    * @}
    */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup PERIPH_ANCS_Exported_Types ANCS Exported Types
    * @{
    */
/**  @brief  ANCS event id for the peer device notification */
typedef enum
{
    NS_EVENT_ID_NOTIFICATION_ADD = 0,      /**< The arrival of a new iOS notification on the NP */
    NS_EVENT_ID_NOTIFICATION_MODIFIED = 1, /**< The modification of an iOS notification on the NP */
    NS_EVENT_ID_NOTIFICATION_REMOVED = 2,  /**< The removal of an iOS notification on the NP */
    NS_EVENT_ID_NOTIFICATION_RESERVED = 0xff
} T_NS_EVENT_ID;

/**  @brief  ANCS category id */
typedef enum
{
    NS_CATEGORY_ID_OTHER = 0,
    NS_CATEGORY_ID_INCOMING_CALL = 1,
    NS_CATEGORY_ID_MISSED_CALL = 2,
    NS_CATEGORY_ID_VOICE_MAIL = 3,
    NS_CATEGORY_ID_SOCIAL = 4,
    NS_CATEGORY_ID_SCHEDULE = 5,
    NS_CATEGORY_ID_EMAIL = 6,
    NS_CATEGORY_ID_NEWS = 7,
    NS_CATEGORY_ID_HEALTH_AND_FITNESS = 8,
    NS_CATEGORY_ID_BUSINESS_ADN_FINANCE = 9,
    NS_CATEGORY_ID_LOCATION = 10,
    NS_CATEGORY_ID_ENTERTAINMENT = 11,
    NS_CATEGORY_ID_RESERVED = 255
} T_NS_CATEGORY_ID;

/**  @brief  ANCS notification attribute id */
typedef enum
{
    DS_NOTIFICATION_ATTR_ID_APP_IDENTIFIER = 0, /**< Format: UTF-8 strings */
    DS_NOTIFICATION_ATTR_ID_TITLE = 1, /**<Format: UTF-8 strings. Needs to be followed by a 2-bytes max length parameter */
    DS_NOTIFICATION_ATTR_ID_SUB_TITLE = 2, /**<Format: UTF-8 strings. Needs to be followed by a 2-bytes max length parameter */
    DS_NOTIFICATION_ATTR_ID_MESSAGE = 3, /**<Format: UTF-8 strings. Needs to be followed by a 2-bytes max length parameter */
    DS_NOTIFICATION_ATTR_ID_MESSAGE_SIZE = 4, /**<The format of the DS_NOTIFICATION_ATTR_ID_MESSAGE_SIZE constant is a string
    that represents the integral value of the message size. */
    DS_NOTIFICATION_ATTR_ID_DATE = 5, /**<The format of the DS_NOTIFICATION_ATTR_ID_DATE constant is a string
    that uses the Unicode Technical Standard (UTS) #35 date format pattern yyyyMMdd'T'HHmmSS. */
    DS_NOTIFICATION_ATTR_ID_POSITIVE_ACTION_LABEL = 6, /**<Format: UTF-8 strings. The label used to describe
    the positive action that can be performed on the iOS notification. */
    DS_NOTIFICATION_ATTR_ID_NEGATIVE_ACTION_LABEL = 7, /**<Format: UTF-8 strings. The label used to describe
    the negative action that can be performed on the iOS notification. */
    DS_NOTIFICATION_ATTR_ID_RESERVED = 255
} T_DS_NOTIFICATION_ATTR_ID;

/**  @brief  App parse ANCS notification attribute state */
typedef enum
{
    DS_PARSE_NOT_START = 0x00,
    DS_PARSE_GET_NOTIFICATION_COMMAND_ID = 0x01,
    DS_PARSE_UID1,
    DS_PARSE_UID2,
    DS_PARSE_UID3,
    DS_PARSE_UID4,
    DS_PARSE_ATTRIBUTE_ID,
    DS_PARSE_ATTRIBUTE_LEN1,
    DS_PARSE_ATTRIBUTE_LEN2,
    DS_PARSE_ATTRIBUTE_READY
} T_DS_NOTIFICATION_ATTR_PARSE_STATE;

/**  @brief  Smart Phone App attribute id. */
typedef enum
{
    DS_APP_ATTR_ID_DISPLAY_NAME = 0,
    DS_APP_ATTR_ID_RESERVED = 255
} T_DS_APP_ATTR_ID;

/**  @brief  App parse ANCS attribute state */
typedef enum
{
    DS_PARSE_GET_APP_COMMAND_ID = 0x10,
    DS_PARSE_APP_IDENTIFIER_START,
    DS_PARSE_APP_IDENTIFIER_END,
    DS_PARSE_APP_ATTRIBUTE_ID,
    DS_PARSE_APP_ATTRIBUTE_LEN1,
    DS_PARSE_APP_ATTRIBUTE_LEN2,
    DS_PARSE_APP_ATTRIBUTE_READY
} T_DS_APP_ATTR_PARSE_STATE;

/**  @brief  ANCS action id state */
typedef enum
{
    CP_ACTION_ID_POSITIVE = 0,
    CP_ACTION_ID_NEGATIVE = 1,
    CP_ACTION_ID_RESERVED  = 255
} T_CP_ACTION_ID_VALUES;

/**  @brief  Define notification source data for record ANCS notification parameters */
typedef struct
{
    uint8_t event_id;          /**< This field informs the accessory whether the given iOS notification
    was added, modified, or removed. The enumerated values for this field are defined in @ref T_NS_EVENT_ID*/
    uint8_t event_flags;       /**< A bitmask whose set bits inform an NC of specificities with the iOS notification. */
    uint8_t category_id;       /**< A numerical value providing a category in which the iOS notification
    can be classified. The enumerated values for this field are defined in @ref T_NS_CATEGORY_ID*/
    uint8_t category_count;    /**< The current number of active iOS notifications in the given category. */
    uint32_t notification_uid; /**< A 32-bit numerical value that is the unique identifier (UID) for the iOS notification.*/
} T_NS_DATA, *P_NS_DATA;

/**  @brief  Define notification attribute details data */
/**          App can acquire details information by attribute id */
typedef struct
{
    uint8_t    command_id;
    uint8_t    notification_uid[4];
    uint8_t    attribute_id;
    uint16_t   attribute_len;
    uint8_t    data[ANCS_MAX_ATTR_LEN];
} T_DS_NOTIFICATION_ATTR;

/**  @brief  Local app record notification attribute information */
typedef struct
{
    uint8_t    command_id;
    uint8_t    attribute_id;
    uint16_t   attribute_len;
    uint8_t    data[ANCS_MAX_ATTR_LEN];
} T_DS_APP_ATTR;

/**  @brief  ANCS Message Type */
typedef enum
{
    ANCS_MSG_TYPE_GET_NOTIFI_ATTR,
    ANCS_MSG_TYPE_PERFORM_NOTIFI_ACTION,
    ANCS_MSG_TYPE_GET_APP_ATTR,
} T_ANCS_MSG_TYPE;

/**  @brief  Message data of ANCS_MSG_TYPE_GET_NOTIFI_ATTR */
typedef struct
{
    uint8_t  conn_id;
    uint32_t notification_uid;
    uint8_t  attribute_ids[14];
    uint8_t  attribute_ids_len;
} T_ANCS_GET_NOTIFI_ATTR;

/**  @brief  Message data of ANCS_MSG_TYPE_PERFORM_NOTIFI_ACTION */
typedef struct
{
    uint8_t  conn_id;
    uint32_t notification_uid;
    uint8_t  action_id;
} T_ANCS_PERFORM_NOTIFI_ACTION;

#if F_BT_ANCS_GET_APP_ATTR
/**  @brief  Message data of ANCS_MSG_TYPE_GET_APP_ATTR */
typedef struct
{
    uint8_t  conn_id;
    char     app_identifier[ANCS_APP_INDENTIFIER_MAX_LEN];
    uint8_t  attribute_ids[1];
    uint8_t  attribute_ids_len;
} T_ANCS_GET_APP_ATTR;
#endif

/**  @brief  ANCS Message Data */
typedef struct
{
    T_ANCS_MSG_TYPE type;
    union
    {
        T_ANCS_GET_NOTIFI_ATTR       notifi_attr;
        T_ANCS_PERFORM_NOTIFI_ACTION perform_action;
#if F_BT_ANCS_GET_APP_ATTR
        T_ANCS_GET_APP_ATTR          app_attr;
#endif
    } data;
} T_ANCS_MSG;
/** End of PERIPH_ANCS_Exported_Types
    * @}
    */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup PERIPH_ANCS_Exported_Functions ANCS Exported Functions
    * @{
    */
/**
 * @brief  App register ancs client to upperstack.
 *         This ancs_client_cb callback function will handle message.
 * @param[in] link_num Initialize link number
 * @return void
 */
void ancs_init(uint8_t link_num);

/**
 * @brief    All the ancs messages are pre-handled in this function
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ancs_handle_msg(T_IO_MSG *p_io_msg);
/** @} */ /* End of group PERIPH_ANCS_Exported_Functions */
/** @} */ /* End of group PERIPH_ANCS */
#ifdef __cplusplus
}
#endif

#endif

