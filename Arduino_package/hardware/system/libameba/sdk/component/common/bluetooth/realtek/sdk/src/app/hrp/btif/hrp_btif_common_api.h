#ifndef  _HRP_BTIF_COMMON_API_H_
#define  _HRP_BTIF_COMMON_API_H_

#include <hrp.h>
#include <hrp_utils.h>
#include <hrp_btif_entry.h>

typedef struct
{
    uint32_t    cod;
    uint8_t     name[BTIF_NAME_LENGTH];
} T_BTIF_DEV_CFG_DEV;

typedef struct
{
    uint16_t    vendor_id;
    uint16_t    vendor_id_src;
    uint16_t    product_id;
    uint16_t    product_version;
} T_BTIF_DEV_CFG_DID_EIR;

typedef struct
{
    uint8_t    *p_data;
} T_BTIF_DEV_CFG_EXT_EIR;

typedef struct
{
    T_BTIF_BR_PAGE_SCAN_TYPE    scan_type;
    uint16_t                    interval;
    uint16_t                    window;
    uint16_t                    page_tout;
} T_BTIF_DEV_CFG_PAGE_SCAN;

typedef struct
{
    T_BTIF_BR_INQUIRY_SCAN_TYPE scan_type;
    uint16_t                    interval;
    uint16_t                    window;
} T_BTIF_DEV_CFG_INQUIRY_SCAN;

typedef struct
{
    T_BTIF_BR_INQUIRY_MODE  mode;
} T_BTIF_DEV_CFG_INQUIRY_MODE;

typedef struct
{
    uint16_t    policy;
    uint16_t    supv_tout;
} T_BTIF_DEV_CFG_LINK_POLICY;

typedef struct
{
    int16_t     tx_power;
} T_BTIF_DEV_CFG_MAX_TX_POWER;

typedef struct
{
    T_BTIF_BT_MODE  mode;
} T_BTIF_DEV_CFG_BT_MODE;

typedef struct
{
    uint8_t         role;
} T_BTIF_DEV_CFG_ACCEPT_ROLE;

typedef struct
{
    uint32_t    fixed_value;
} T_BTIF_DEV_CFG_PASSKEY_VALUE;

typedef struct
{
    T_BTIF_DEV_CFG_OPCODE               op;
    union
    {
        T_BTIF_DEV_CFG_DEV              dev;
        T_BTIF_DEV_CFG_DID_EIR          did;
        T_BTIF_DEV_CFG_EXT_EIR          ext_eir;
        T_BTIF_DEV_CFG_PAGE_SCAN        page_scan;
        T_BTIF_DEV_CFG_INQUIRY_SCAN     inquiry_scan;
        T_BTIF_DEV_CFG_INQUIRY_MODE     inquiry_mode;
        T_BTIF_DEV_CFG_LINK_POLICY      link_policy;

        T_BTIF_DEV_CFG_BT_MODE          bt_mode;
        T_BTIF_DEV_CFG_ACCEPT_ROLE      accept_role;
    } p;
} T_BTIF_DEV_CFG_SET_REQ;

typedef struct
{
    uint8_t         bd_addr[6];
    uint8_t         conn_type;  /* 0: BREDR ACL; 1: SCO; 2: LE */
    bool            is_high_priority;
} T_BTIF_ACL_PRIORITY_SET_REQ;

typedef struct
{
    uint8_t                 bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE remote_addr_type;
} T_BTIF_READ_RSSI_REQ;

typedef struct
{
    uint16_t    command;
    uint8_t     param_len;
    uint8_t     param[1];
} T_BTIF_VENDOR_CMD_REQ;

typedef struct
{
    bool            enable;
    uint16_t        requirements;
    T_BTIF_IOCAP    io_capabilities;
    bool            oob_present;
} T_BTIF_PAIRABLE_MODE_SET_REQ;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_USER_PASSKEY_REQ_CFM;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint32_t                    passkey;
    T_BTIF_CAUSE                cause;
} T_BTIF_USER_PASSKEY_REQ_REPLY_REQ;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_KEY_TYPE             key_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_AUTHEN_RESULT_CFM;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     key_len;
    uint8_t                     link_key[28];
    T_BTIF_KEY_TYPE             key_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_AUTHEN_KEY_REQ_CFM;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_USER_CFM_REQ_CFM;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_KEYPRESS_NOTIF_TYPE  event_type;
} T_BTIF_KEYPRESS_NOTIF_REQ;

void hrp_register_req(uint16_t len, uint8_t *p_param_list);

void hrp_release_req(uint16_t len, uint8_t *p_param_list);

void hrp_dev_cfg_req(uint16_t len, uint8_t *p_param_list);


void hrp_read_rssi_req(uint16_t len, uint8_t *p_param_list);

void hrp_vendor_cmd_req(uint16_t len, uint8_t *p_param_list);

void hrp_pairable_mode_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_user_passkey_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_user_passkey_req_reply_req(uint16_t len, uint8_t *p_param_list);

void hrp_authen_result_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_authen_key_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_user_cfm_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_keypress_notif_req(uint16_t len, uint8_t *p_param_list);
#endif
