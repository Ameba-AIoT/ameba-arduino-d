/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ota_client.h
  * @brief    Head file for using ota service Client.
  * @details  Data structs and external functions declaration.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _OTA_CLIENT_H_
#define _OTA_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Add Includes here */
#include "profile_client.h"

#define GATT_UUID_OTA_SERVICE 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00

#define GATT_UUID_OTA_PROFILE

#define GATT_UUID_CHAR_OTA                          0xFFD1    //1, write
#define GATT_UUID_CHAR_MAC                          0xFFD2    //2
#define GATT_UUID_CHAR_PATCH                        0xFFD3    //3
#define GATT_UUID_CHAR_APP_VERSION                  0xFFD4    //4
#define GATT_UUID_CHAR_PATCH_EXTENSION              0xFFD5    //5 
#define GATT_UUID_CHAR_TEST_MODE                    0xFFD8    //6, write

#define GATT_UUID_CHAR_DEVICE_INFO                  0xFFF1    //7
#define GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE        0xFFF2    //8, write
#define GATT_UUID_CHAR_IMAGE_VERSION                0xFFE0


#define OTA_WRITE_OTA_CMD_CHAR_VAL                  0x01
#define OTA_WRITE_TEST_MODE_CHAR_VAL                0x02
#define OTA_WRITE_OTA_IMG_COUNTER_CHAR_VAL          0x03

/** @defgroup DFU_Client
  * @{
  */
/** @brief  Define links number. range: 0-4 */
#define OTA_MAX_LINKS           4

typedef struct _T_OTA_DEVICE_INFO
{
    uint8_t ictype;
    uint8_t ota_version;
    uint8_t secure_version;

    union
    {
        uint8_t value;
        struct
        {
            uint8_t buffercheck: 1; // 1:support,  0:don't support
            uint8_t aesflg: 1;      // 1:aes encrypt when ota,  0:not encrypt
            uint8_t aesmode: 1;     // 1:all data is encrypted, 0:only encrypt 16byte
            //uint8_t verify_algo: 1; // 1:sha256,   0:crc
            uint8_t copy_img: 1;    //1:support ,0:don't support
            uint8_t multi_img: 1;   //1:support(update multi img at a time) ,0:don't support(one img at a time)
            uint8_t rsvd: 3;
        };
    } mode;

    uint16_t maxbuffersize;
    uint16_t res;

    uint32_t img_indicator;
} T_OTA_DEVICE_INFO, *P_OTA_DEVICE_INFO;

/** @defgroup DFU_Client_Exported_Types Exported Types
  * @{
  */
/** @brief OTA client handle type*/
typedef enum
{
    HDL_OTA_SRV_START,           //!< start handle of battery service
    HDL_OTA_SRV_END,             //!< end handle of battery service
    HDL_OTA_CMD,                 //!< OTA CMD characteristic value handle
    HDL_OTA_DEVICE_MAC,
    HDL_OTA_PATCH_VER,
    HDL_OTA_APP_VER,
    HDL_OTA_PATCH_EXT_VER,
    HDL_OTA_TEST_MODE,           //!< OTA test mode characteristic handle
    HDL_OTA_DEVICE_INFO,
    HDL_OTA_IMG_COUNTER,         //!<OTA IMAGE COUNT TO UPDATE characteristic handle
    HDL_OTA_IMG_VER,
    HDL_OTA_CACHE_LEN            //!< handle cache length
} T_OTA_HANDLE_TYPE;

/** @brief OTA client discovery state*/
typedef enum
{
    DISC_OTA_IDLE,
    DISC_OTA_START,
    DISC_OTA_DONE,
    DISC_OTA_FAILED
} T_OTA_DISC_STATE;

/** @brief OTA client read type*/
typedef enum
{
    OTA_READ_DEVICE_MAC,
    OTA_READ_PATCH_VER,
    OTA_READ_APP_VER,
    OTA_READ_PATCH_EXT_VER,
    OTA_READ_DEVICE_INFO,
    OTA_READ_IMG_VER
} T_OTA_READ_TYPE;

/** @brief OTA client read data */
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_OTA_READ_DATA;

/** @brief OTA client read result*/
typedef struct
{
    T_OTA_READ_TYPE type;
    T_OTA_READ_DATA data;
    uint16_t cause;
} T_OTA_READ_RESULT;

/** @brief OTA client write type*/
typedef enum
{
    OTA_WRITE_CMD,
    OTA_WRITE_TEST_MODE,
    OTA_WRITE_IMG_COUNTER
} T_OTA_WRTIE_TYPE;

/** @brief OTA client write result*/
typedef struct
{
    T_OTA_WRTIE_TYPE type;
    uint16_t cause;
} T_OTA_WRITE_RESULT;

///** @brief OTA client notif/ind receive type*/
//typedef enum
//{
//    OTA_NOTIFY,
//    OTA_INDICATE,
//} T_OTA_NOTIF_IND_TYPE;
///** @brief OTA client notif/ind receive data*/
//typedef struct
//{
//    uint16_t value_size;
//    uint8_t *p_value;
//} T_OTA_NOTIF_IND_VALUE;

///** @brief OTA client notif/ind receive content*/
//typedef struct
//{
//    T_OTA_NOTIF_IND_TYPE type;
//    T_OTA_NOTIF_IND_VALUE data;
//} T_OTA_NOTIF_IND_DATA;

/** @brief OTA client callback type*/
typedef enum
{
    OTA_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    OTA_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    OTA_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    //OTA_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    OTA_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_OTA_CLIENT_CB_TYPE;

/** @brief OTA client callback content*/
typedef union
{
    T_OTA_DISC_STATE disc_state;
    T_OTA_READ_RESULT read_result;
    T_OTA_WRITE_RESULT write_result;
    //T_OTA_NOTIFY_DATA notify_data;
} T_OTA_CLIENT_CB_CONTENT;

/** @brief OTA client callback data*/
typedef struct
{
    T_OTA_CLIENT_CB_TYPE     cb_type;
    T_OTA_CLIENT_CB_CONTENT  cb_content;
} T_OTA_CLIENT_CB_DATA;


/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ota_client_read_by_handle(uint8_t conn_id, T_OTA_READ_TYPE read_type);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ota_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ota_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to write ota cmd write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ota_client_write_char(uint8_t conn_id, T_OTA_WRTIE_TYPE write_type);

/**
  * @brief  ota client connect target device.
  * @param  p_le_scan_info: filtered scan info
  */
void ota_client_connect_device(T_LE_SCAN_INFO *p_le_scan_info);
/**
  * @brief  Used by application, to start the discovery procedure of OTA server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ota_client_start_discovery(uint8_t conn_id);

/**
  * @brief  add Dfu client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
T_CLIENT_ID ota_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* _OTA_CLIENT_H_ */
