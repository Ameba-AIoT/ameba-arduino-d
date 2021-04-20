#ifndef     _HRP_GAP_LE_H_
#define     _HRP_GAP_LE_H_


#include "gap_le.h"
#include "gap_bond_le.h"
#include <app_msg.h>
#include <gap_msg.h>

#if F_BT_LE_GATT_CLIENT_SUPPORT
#include "gaps_client.h"
#include <complete_ble_client.h>
#endif
#include <complete_ble_service.h>
#if F_BT_LE_PRIVACY_SUPPORT
#include <privacy_mgnt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define HRP_MAX_LE_LINKS   4
#define HRP_MAX_EVE_PARAM_LEN  100


typedef enum
{
    HRP_GAP_MSG_LE_RESULT = 0x00,
    HRP_GAP_MSG_LE_MODIFY_WHITE_LIST = 0x01,
    HRP_GAP_MSG_LE_SET_RAND_ADDR = 0x02,
    HRP_GAP_MSG_LE_SET_HOST_CHANN_CLASSIF = 0x03,
    HRP_GAP_MSG_LE_VENDOR_ONE_SHOT_ADV = 0x04,

    HRP_GAP_MSG_LE_READ_RSSI = 0x10,
    HRP_GAP_MSG_LE_READ_CHANN_MAP,
    HRP_GAP_MSG_LE_DISABLE_SLAVE_LATENCY,
    HRP_GAP_MSG_LE_SET_DATA_LEN,
    HRP_GAP_MSG_LE_DATA_LEN_CHANGE_INFO,
    HRP_GAP_MSG_LE_CONN_UPDATE_IND,
    HRP_GAP_MSG_LE_CREATE_CONN_IND,
    HRP_GAP_MSG_LE_PHY_UPDATE_INFO,
    HRP_GAP_MSG_LE_UPDATE_PASSED_CHANN_MAP,

    HRP_GAP_MSG_LE_REMOTE_FEATS_INFO,



    HRP_GAP_MSG_LE_BOND_MODIFY_INFO = 0x20,
    HRP_GAP_MSG_LE_KEYPRESS_NOTIFY,
    HRP_GAP_MSG_LE_KEYPRESS_NOTIFY_INFO,
    HRP_GAP_MSG_LE_GATT_SIGNED_STATUS_INFO,


    HRP_GAP_MSG_LE_SCAN_INFO = 0x30,
    HRP_GAP_MSG_LE_DIRECT_ADV_INFO,

    HRP_GAP_MSG_LE_ADV_UPDATE_PARAM = 0x40,
    HRP_GAP_MSG_LE_ADV_READ_TX_POWER,

    HRP_GAP_MSG_LE_EXT_ADV_REPORT_INFO = 0x50,

    HRP_GAP_MSG_LE_EXT_ADV_START_SETTING = 0x60,
    HRP_GAP_MSG_LE_EXT_ADV_REMOVE_SET,
    HRP_GAP_MSG_LE_EXT_ADV_CLEAR_SET,
    HRP_GAP_MSG_LE_EXT_ADV_ENABLE,
    HRP_GAP_MSG_LE_EXT_ADV_DISABLE,
    HRP_GAP_MSG_LE_SCAN_REQ_RECEIVED_INFO,
    HRP_GAP_MSG_LE_EXT_ADV_STATE_CHANGE,


    HRP_GAP_MSG_LE_PRIVACY_RESOLUTION_STATUS_INFO = 0x70,
    HRP_GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT,   //0x71
    HRP_GAP_MSG_LE_PRIVACY_MODIFY_RESOLV_LIST,     //0x72

    HRP_GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR,   //0x73
    HRP_GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR,   //0x74
    HRP_GAP_MSG_LE_PRIVACY_SET_MODE,    //0x75



    HRP_GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE = 0xA0,
    HRP_GAP_MSG_LE_VENDOR_ADV_3_DATA_SET,             //0xA1
    HRP_GAP_MSG_LE_SET_LPS_BOOTUP_ACTIVE_TIME,    //0xA2
    HRP_GAP_MSG_LE_CHECK_LPS_WAKEUP_TIME_RSP,    //0xA3


    HRP_GAP_MSG_LE_DEV_STATE_CHANGE = 0xB1,
    HRP_GAP_MSG_LE_CONN_STATE_CHANGE,
    HRP_GAP_MSG_LE_CONN_PARAM_UPDATE,
    HRP_GAP_MSG_LE_CONN_MTU_INFO,
    HRP_GAP_MSG_LE_AUTHEN_STATE_CHANGE,
    HRP_GAP_MSG_LE_BOND_PASSKEY_DISPLAY,
    HRP_GAP_MSG_LE_BOND_PASSKEY_INPUT,

    HRP_GAP_MSG_LE_BOND_OOB_INPUT,
    HRP_GAP_MSG_LE_BOND_USER_CONFIRMATION,
    HRP_GAP_MSG_LE_BOND_JUST_WORK,

    HRP_GAP_MSG_LE_GEN_RAND_ADDR_RSP = 0xC0,

    HRP_GAP_MSG_LE_CBC_CHANN_STATE = 0xC1,
    HRP_GAP_MSG_LE_CBC_REG_PSM = 0xC2,
    HRP_GAP_MSG_LE_CBC_SET_PSM_SECURITY = 0xC3,
    HRP_GAP_MSG_LE_CBC_SEND_DATA = 0xC4,
    HRP_GAP_MSG_LE_CBC_RECEIVE_DATA = 0xC5,
    HRP_GAP_MSG_LE_CBC_MTU_INFO = 0xC6,


} T_PROFILE_GAP_LE_MSG_INDEX;


typedef enum
{
    HRP_PROFILE_EVT_GENERAL_RESULT = 0x00,

    HRP_PROFILE_EVT_SRV_REG_COMPLETE = 0x01,

    HRP_PROFILE_EVT_SRV_SEND_DATA_COMPLETE,

    HRP_PROFILE_EVT_CLIENT_DISC_STATE = 0x10,

    HRP_PROFILE_EVT_CLIENT_DISC_RESULT,


} T_PROFILE_LE_GENERAL_MSG_INDEX;


typedef enum
{
    NONE = 0x0000,
//server mask
    SIMPLE_BLE_SERVICE = 0x01,

    SIMPLE_BLE_ADD_SERVICE = 0x10,

//client mask
    SIMPLE_BLE_CLIENT = 0x30,


} T_HRP_PROFILE_LE_MASK;

#if F_BT_LE_GATT_CLIENT_SUPPORT
extern T_CLIENT_ID   gaps_client_id;        /**< Simple ble service client id*/
extern T_CLIENT_ID   simple_ble_client_id;  /**< gap service client id*/
#endif
extern T_SERVER_ID   simp_srv_id;           /**< Simple ble service id*/

T_APP_RESULT hrp_profile_callback(T_SERVER_ID service_id, void *p_data);
#if F_BT_LE_GATT_CLIENT_SUPPORT
T_APP_RESULT hrp_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

extern void  hrp_profile_client_gaps_callback(T_GAPS_CLIENT_CB_DATA *p_gaps_cb_data);
extern void    hrp_profile_server_simp_ble_callback(TSIMP_CALLBACK_DATA *p_simp_cb_data);
extern void    hrp_profile_client_simp_ble_callback(T_SIMP_CLIENT_CB_DATA *p_simp_client_cb_data);
#endif
T_GAP_CAUSE hrp_gap_le_set_param(uint8_t type, uint8_t len, void *p_value);

void hrp_gap_le_init(uint16_t le_profile_sever_mask, uint16_t le_profile_client_mask);

void  hrp_gap_le_server_init(uint16_t le_profile_sever_mask);

void  hrp_gap_le_client_init(uint16_t le_profile_client_mask);

void hrp_gap_le_handle_io_msg(T_IO_MSG io_msg);

void hrp_app_handle_gap_le_msg(T_IO_MSG *p_io_msg);

void hrp_app_handle_dev_state_evt(T_GAP_DEV_STATE new_state);
void hrp_gap_le_check_lps_wakeup_time_handler(void *pxTimer);
void hrp_gap_le_check_lps_event_to_app(void *pxTimer);

T_GAP_CAUSE hrp_le_set_gap_param(uint16_t type, uint8_t len, void *p_value);
T_GAP_CAUSE hrp_le_set_bond_param(uint16_t type, uint8_t len, void *p_value);
T_GAP_CAUSE hrp_le_set_scan_param(uint16_t type, uint8_t len, void *p_value);
T_GAP_CAUSE hrp_le_set_adv_param(uint16_t type, uint8_t len, void *p_value);
T_GAP_CAUSE hrp_le_set_privacy_param(uint16_t type, uint8_t len, void *p_value);
/* hrp_profile_system_api.c */
void hrp_profile_handle_initCmplete(uint8_t initiated_mode);

#if F_BT_LE_4_1_CBC_SUPPORT
T_APP_RESULT hrp_credit_based_conn_callback(uint8_t cbc_type, void *p_cbc_data);
#endif

#if F_BT_LE_PRIVACY_SUPPORT
void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data);
#endif

extern T_GAP_CAUSE le_adv_read_tx_power(void);
extern T_GAP_CAUSE le_adv_update_param(void);
extern T_GAP_CAUSE le_adv_stop(void);
extern T_GAP_CAUSE le_adv_start(void);

#ifdef __cplusplus
}
#endif

#endif /* _HRP_GAP_LE_H_ */
