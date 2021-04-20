#ifndef     _HRP_PROFILE_SIMPLE_BLE_H_
#define     _HRP_PROFILE_SIMPLE_BLE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "complete_ble_service.h"
#include "complete_ble_client.h"

typedef enum
{
    HRP_SIMP_BLE_CMD_RESULT = 0x01,

    HRP_SIMP_CLIENT_CB_TYPE_DISC_STATE,   //02
    HRP_SIMP_READ_V1_READ,                             //03
    HRP_SIMP_READ_V3_NOTIFY_CCCD,               //04

    HRP_SIMP_READ_V4_INDICATE_CCCD,    //05
    HRP_SIMP_READ_V8_CCCD,                      //06

    HRP_SIMP_READ_V7_READ_LONG,         //07

    HRP_SIMP_WRITE_V2_WRITE,                      //08
    HRP_SIMP_WRITE_V3_NOTIFY_CCCD,         //09

    HRP_SIMP_WRITE_V4_INDICATE_CCCD,     //0x0a

    HRP_SIMP_WRITE_V6_WRITE_LONG,         //0x0b

    HRP_SIMP_WRITE_V8_CCCD,                  //0x0c

    HRP_SIMP_V3_NOTIFY,                        //0x0d
    HRP_SIMP_V4_INDICATE,                   //0x0e
    HRP_SIMP_V8_INDICATE,                   //0x0f
    HRP_SIMP_V8_NOTIFY,                       //0x10

    HRP_SIMP_GET_HDL_CACHE_RSP,    //0x11


    HRP_SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION = 0x30,
    HRP_SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE = 0x31,
    HRP_SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE,


} T_PROFILE_SIMP_BLE_MSG_INDEX;

typedef enum
{
    HRP_GAPS_CLIENT_CMD_RESULT = 0x01,
    HRP_GAPS_CLIENT_CB_TYPE_DISC_STATE,

    HRP_GAPS_READ_DEVICE_NAME,
    HRP_GAPS_READ_APPEARANCE,
    HRP_GAPS_READ_CENTRAL_ADDR_RESOLUTION,

} T_PROFILE_GAPS_CLIENT_MSG_INDEX;

void  hrp_profile_server_simp_ble_callback(TSIMP_CALLBACK_DATA *p_simp_cb_data);
#if F_BT_LE_GATT_CLIENT_SUPPORT
void  hrp_profile_client_simp_ble_callback(T_SIMP_CLIENT_CB_DATA *p_simp_client_cb_data);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _HRP_PROFILE_SIMPLE_BLE_SERVICE_H_ */
