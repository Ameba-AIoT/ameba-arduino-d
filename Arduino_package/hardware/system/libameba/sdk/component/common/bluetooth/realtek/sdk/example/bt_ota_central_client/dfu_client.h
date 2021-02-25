/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     dfu_client.h
  * @brief    Head file for using dfu service Client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2018-4-17
  * @version  v2.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _DFU_CLIENT_H_
#define _DFU_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Add Includes here */
#include "profile_client.h"


/** @defgroup DFU_Client
  * @{
  */
/** @brief  Define links number. range: 0-4 */
#define DFU_MAX_LINKS                           4

#define NORMAL_OTA_MAX_BUFFER_SIZE              2000

/*each control point procedure*/
#define DFU_OPCODE_MIN                          0x00
#define DFU_OPCODE_START_DFU                    0x01
#define DFU_OPCODE_RECEIVE_FW_IMAGE_INFO        0x02
#define DFU_OPCODE_VALID_FW                     0x03
#define DFU_OPCODE_ACTIVE_IMAGE_RESET           0x04
#define DFU_OPCODE_SYSTEM_RESET                 0x05
#define DFU_OPCODE_REPORT_TARGET_INFO           0x06
#define DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ      0x07

#define DFU_OPCODE_PKT_RX_NOTIFICATION_VOICE    0x08
#define DFU_OPCODE_BUFFER_CHECK_EN              0x09 /*report current ota function version information*/
#define DFU_OPCODE_REPORT_BUFFER_CRC            0x0a /*report current buffer CRC*/

#define DFU_OPCODE_RECEIVE_IC_TYPE              0x0b
#define DFU_OPCODE_COPY_IMG                     0x0c
#define DFU_OPCODE_MAX                          0x0d

/*length of each control point procedure*/
#define DFU_LENGTH_START_DFU                    (1+12+4)/*4 bytes is padding for encrypt*/
#define DFU_LENGTH_RECEIVE_FW_IMAGE_INFO        (1+2+4) //img_id + cur_offset
#define DFU_LENGTH_VALID_FW                     (1+2)   //img_id
#define DFU_LENGTH_ACTIVE_IMAGE_RESET           0x01
#define DFU_LENGTH_SYSTEM_RESET                 0x01
#define DFU_LENGTH_REPORT_TARGET_INFO           (1+2) //img_id
#define DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ      (1+2+2+2+2) //conn_interval_min,conn_interval_max,conn_latency,superv_tout
#define DFU_LENGTH_BUFFER_CHECK_EN              (1)
#define DFU_LENGTH_REPORT_BUFFER_CRC            (1+2+2) //buf_size, buf_crc

/*notification opcode*/
#define DFU_OPCODE_NOTIFICATION                 0x10

/*length of notification*/
#define DFU_NOTIFY_LENGTH_ARV                   3  //others opcode notification length
//#define DFU_NOTIFY_LENGTH_START_DFU             (DFU_NOTIFY_LENGTH_ARV)
//#define DFU_NOTIFY_LENGTH_VALID_FW              (DFU_NOTIFY_LENGTH_ARV)
#define DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO    (DFU_NOTIFY_LENGTH_ARV+4+4)  //img_ver, cur_offset
#define DFU_NOTIFY_LENGTH_BUFFER_CHECK_EN       (DFU_NOTIFY_LENGTH_ARV+2+2)  //buf_size, mtu_size
#define DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC     (DFU_NOTIFY_LENGTH_ARV+4)   //cur_offset

/*dfu error code*/
#define DFU_ARV_SUCCESS                         0x01
#define DFU_ARV_FAIL_INVALID_PARAMETER          0x02
#define DFU_ARV_FAIL_OPERATION                  0x03
#define DFU_ARV_FAIL_DATA_SIZE_EXCEEDS_LIMIT    0x04
#define DFU_ARV_FAIL_CRC_ERROR                  0x05
#define DFU_ARV_FAIL_LENGTH_ERROR               0x06
#define DFU_ARV_FAIL_PROG_ERROR                 0x07
#define DFU_ARV_FAIL_ERASE_ERROR                0x08

/** @defgroup DFU_Client_Exported_Types Exported Types
  * @{
  */

/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_DFU_SRV_START,           // start handle of dfu service
    HDL_DFU_SRV_END,             // end handle of dfu service
    HDL_DFU_DATA,
    HDL_DFU_CP,
    HDL_DFU_CP_END,
    HDL_DFU_CP_CCCD,
    HDL_DFU_CACHE_LEN
} T_DFU_HANDLE_TYPE;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    DISC_DFU_IDLE,
    DISC_DFU_START,
    DISC_DFU_DONE,
    DISC_DFU_FAIL,
} T_DFU_DISC_STATE;

/** @brief handle information found in discovery procedure */
typedef struct
{
    T_DFU_HANDLE_TYPE handle_type;
    uint16_t handle_value;
} T_DFU_READ_DISC_HDL_INFO;

/** @brief read request type, used by app to send read request */
typedef enum
{
    DFU_READ_CP_CCCD
} T_DFU_READ_TYPE;

/** @brief read content, used to inform app read response data content */
typedef union
{
    bool dfu_cp_cccd;
} T_DFU_READ_DATA;

/** @brief read data, used to inform app read response data */
typedef struct
{
    T_DFU_READ_TYPE type;
    T_DFU_READ_DATA data;
    uint16_t cause;
} T_DFU_READ_RESULT;

/** @brief write request result */
typedef enum
{
    DFU_WRITE_CP,
    DFU_WRITE_CP_CCCD,
    DFU_WRITE_DATA
} T_DFU_WRITE_TYPE;

typedef struct
{
    T_DFU_WRITE_TYPE type;
    uint16_t cause;
} T_DFU_WRITE_RESULT;

/** @brief notif/ind receive type */
typedef enum
{
    DFU_NOTIF_RECEIVE_DFU_CP_NOTIFY,
} T_DFU_NOTIFY_INDICATE_TYPE;

/** @brief notif/ind receive content */
typedef struct
{
    uint16_t len;
    uint8_t *pdata;
} T_DFU_NOTIFY_INDICATE_VAL;

/** @brief The notif/ind data received by the client from the server */
typedef struct
{
    T_DFU_NOTIFY_INDICATE_TYPE type;
    T_DFU_NOTIFY_INDICATE_VAL value;
} T_DFU_NOTIFY_INDICATE_DATA;

/** @brief Event type to inform app */
typedef enum
{
    DFU_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    DFU_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    DFU_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    DFU_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_DFU_CLIENT_CB_TYPE;

/** @brief Callback content sent to application from the client */
typedef union
{
    T_DFU_DISC_STATE disc_state;
    T_DFU_READ_DISC_HDL_INFO disc_hdl_info;
    T_DFU_READ_RESULT read_result;
    T_DFU_WRITE_RESULT write_result;
    T_DFU_NOTIFY_INDICATE_DATA notif_ind_data;
} T_DFU_CLIENT_CB_CONTENT;

/** @brief Callback data sent to application from the client, include type and content */
typedef struct
{
    T_DFU_CLIENT_CB_TYPE cb_type;
    T_DFU_CLIENT_CB_CONTENT cb_content;
} T_DFU_CLIENT_CB_DATA;

/** @brief Used for CCCD handle discovering in discovery procedure  */
typedef enum
{
    DFU_CCCD_DISC_START,
    DFU_CCCD_DISC_CP_NOTIFY,
    DFU_CCCD_DISC_END
} T_DFU_CCCD_DISC_STATE;


typedef enum
{
    DFU_FSM_WRITE_DO_NOT_CARE,
    DFU_FSM_WRITE_CCCD_ENABLE,
    DFU_FSM_WRITE_DFU_DATA,
    DFU_FSM_WRITE_WAIT_WRITE_RESP
} T_DFU_CLIENT_FSM_WRITE;


typedef enum
{
    DFU_CB_NONE,
    DFU_CB_START,
    DFU_CB_END,
    DFU_CB_FAIL
} T_DFU_CB_TYPE;

typedef union
{
    uint8_t dull;
} T_DFU_CB_DATA;

/** Dfu service data to inform application */
typedef struct
{
    uint8_t conn_id;
    T_DFU_CB_TYPE type;
    T_DFU_CB_DATA data;
} T_DFU_CB_MSG;

typedef struct
{
    uint8_t ic_type;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //for copy image in ota
            uint16_t not_obsolete : 1; //for copy image in ota
            uint16_t rsvd: 7;
        };
    } ctrl_flag;
    uint16_t signature; //!< app then patch
    uint16_t crc16;
    uint32_t image_length;

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
    uint32_t origin_image_version;
    uint32_t curr_offset;
    uint32_t dfu_img_version;
    T_DFU_CB_TYPE fsm_client;
} T_DFU_CTX;

extern T_DFU_CTX g_dfu_ctx;
/** @} */

/** @defgroup DFU_Client_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Used by application, to set the handles in Dfu handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @param  handle_value: handle value to set.
  * @retval true--set success.
  *         flase--set failed.
  */
bool dfu_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dfu_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  add Dfu client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
T_CLIENT_ID dfu_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief start the service discovery
  *
  * contains the service/characteristic/cccd declaration
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
bool dfu_client_start_discovery(uint8_t conn_id);

/**
  * @brief handle the adv pkt
  * @param[in] p_le_scan_info: the adv pkt pointer
  * @return none
  */
void dfu_client_handle_adv_pkt(T_LE_SCAN_INFO *p_le_scan_info);

/**
  * @brief Get the conn id when actively connect.
  * @param[in] conn_id: the connection id
  * @return none
  */
void dfu_client_handle_connecting(uint8_t conn_id);

/**
  * @brief handle the establish of connection
  *
  * discovery the dfu server etc.
  * @param[in] conn_id: the connection id
  * @return none
  */
void dfu_client_handle_connect(uint8_t conn_id);

/**
  * @brief handle the disconnetion
  *
  * to release the resource and notify the dfu state
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
void dfu_client_disconnect_cb(uint8_t conn_id);

void dfu_client_connect_device(T_LE_SCAN_INFO *p_le_scan_info);

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* _DFU_CLIENT_H_ */
