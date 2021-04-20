/**
 * Copyright (c) 2017, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BTIF_H_
#define _BTIF_H_

#include <stdint.h>
#include <stdbool.h>
#include <bt_types.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  STACK      STACK
 * \defgroup    BTIF       BTIF
 *
 * \brief   Define parameters and structures used in Bluetooth stack APIs or messages.
 * \details The BTIF module provide parameters and structure definitions that
 *          can be used in Bluetooth stack APIs and upstream messages that Bluetooth
 *          stack sent to APP. APIs such as release btif message buffers are also
 *          provided here.\n
 *
 * \ingroup     STACK
 */

/**
 *
 * \ingroup     BTIF
 */
#define BTIF_NAME_LENGTH                     40     /**< Max length of device name. */

/**
 * btif.h
 *
 * \name  BTIF_AUTHEN_FLAGS
 * \brief Btif authentication support flags.
 * \anchor  BTIF_AUTHEN_FLAGS
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_AUTHEN_BONDING                  0x0001 /**< Dedicated bonding support. */
#define BTIF_AUTHEN_GENERAL_BONDING          0x0002 /**< General bonding support. */
#define BTIF_AUTHEN_MITM                     0x0004 /**< MITM support. */
#define BTIF_AUTHEN_SC                       0x0008 /**< LE secure connections support. */
#define BTIF_AUTHEN_KEY_PRESS                0x0010 /**< Keypress support. */
#define BTIF_AUTHEN_FORCE_BONDING            0x0100 /**< Always force bonding. */
#define BTIF_AUTHEN_SC_ONLY                  0x0200 /**< Secure connections only mode support. */
#define BTIF_AUTHEN_SC_BR                    0x0400 /**< BR/EDR secure connections support. */

#define BTIF_LE_MAX_EXTENDED_SCAN_PHY_NUM    2
#define BTIF_LE_MAX_EXTENDED_ADV_SETS_NUM    4//max is 63 by core spec
#define BTIF_LE_MAX_INIT_PHYS_NUM            3

#define LE_ADV_CHANNEL_37                    0x01   /**< Enable channel 37 */
#define LE_ADV_CHANNEL_38                    0x02   /**< Enable channel 38 */
#define LE_ADV_CHANNEL_39                    0x04   /**< Enable channel 39 */
#define LE_ADV_CHANNEL_ALL                   0x07   /**< Enable all channels */

/* PairingReq / PairingRsp initKeyDist/respKeyDist */
#define BTIF_LE_SMP_DIST_ENC_KEY             0x01
#define BTIF_LE_SMP_DIST_ID_KEY              0x02
#define BTIF_LE_SMP_DIST_SIGN_KEY            0x04
#define BTIF_LE_SMP_DIST_LINK_KEY            0x08

#define BTIF_LE_CONN_INIT_PHY_1M             0x01
#define BTIF_LE_CONN_INIT_PHY_2M             0x02
#define BTIF_LE_CONN_INIT_PHY_CODED          0x04

#define BTIF_LE_TX_POWER_UNKNOWN             127
#define BTIF_USE_LE_FIXED_DISPLAY_VALUE      0x80000000

/**
If extended advertising PDU types are being used (bit 4 = 0) then:
1. The advertisement shall not be both connectable and scannable.
2. High duty cycle directed connectable advertising shall not be used (bit 3 = 0).
*/
#define BTIF_ADV_EVENT_PROP_CONNECTABLE_ADV     0x01
#define BTIF_ADV_EVENT_PROP_SCANNABLE_ADV       0x02
#define BTIF_ADV_EVENT_PROP_DIRECTED_ADV        0x04
#define BTIF_ADV_EVENT_PROP_HDC_DIRECTED_ADV    0x08
#define BTIF_ADV_EVENT_PROP_USE_LEGACY_ADV      0x10
#define BTIF_ADV_EVENT_PROP_OMIT_ADV_ADDR       0x20
#define BTIF_ADV_EVENT_PROP_INCLUDE_TX_POWER    0x40

#define BTIF_EXT_ADV_REPORT_CONNECTABLE_ADV     0x01
#define BTIF_EXT_ADV_REPORT_SCANNABLE_ADV       0x02
#define BTIF_EXT_ADV_REPORT_DIRECTED_ADV        0x04
#define BTIF_EXT_ADV_REPORT_SCAN_RESPONSE       0x08
#define BTIF_EXT_ADV_REPORT_USE_LEGACY_ADV      0x10

#define BTIF_LE_SCAN_PHY_LE_1M                  0x01
#define BTIF_LE_SCAN_PHY_LE_CODED               0x04

#define BTIF_LE_ALL_PHY_NO_PREFERENCE_TX        0x01
#define BTIF_LE_ALL_PHY_NO_PREFERENCE_RX        0x02

#define BTIF_LE_TX_PHY_PREFERENCE_1M            0x01
#define BTIF_LE_TX_PHY_PREFERENCE_2M            0x02
#define BTIF_LE_TX_PHY_PREFERENCE_CODED         0x04

#define BTIF_LE_RX_PHY_PREFERENCE_1M            0x01
#define BTIF_LE_RX_PHY_PREFERENCE_2M            0x02
#define BTIF_LE_RX_PHY_PREFERENCE_CODED         0x04

/**
 * btif.h
 *
 * \name  BTIF_BR_SEC
 * \brief Btif BR/EDR security requirement flags.
 * \anchor  BTIF_BR_SEC
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_BR_SEC_OUTGOING            0x01    /**< Request for outgoing connection. */
#define BTIF_BR_SEC_AUTHEN              0x02    /**< Authentication is required. */
#define BTIF_BR_SEC_MITM                0x04    /**< MITM authentication is required, only possible if authentication is set. */
#define BTIF_BR_SEC_AUTHOR              0x08    /**< User level authorization is required. */
#define BTIF_BR_SEC_ENCRYPT             0x10    /**< Encryption on link is required, only possible if authentication is set. */
#define BTIF_BR_SEC_SECURE_CONNECTIONS  0x20    /**< Secure connections is required. */

/**
 * btif.h
 *
 * \name  BTIF_BR_LINK_POLICY
 * \brief Btif BR/EDR link policy.
 * \anchor  BTIF_BR_LINK_POLICY
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_BR_LINK_POLICY_DISABLE_ALL             0x00 /**< Disable all link policies for BR/EDR link. */
#define BTIF_BR_LINK_POLICY_ENABLE_ROLE_SWITCH      0x01 /**< Only enable role switch for BR/EDR link. */
#define BTIF_BR_LINK_POLICY_ENABLE_SNIFF_MODE       0x04 /**< Only enable sniff mode for BR/EDR link. */

/**
 * btif.h
 *
 * \name  BTIF_BR_ACL_PKT_TYPE
 * \brief Btif BR/EDR ACL link packet type.
 * \anchor  BTIF_BR_ACL_PKT_TYPE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_PKT_TYPE_NO_2DH1           0x0002  /**< Packet type 2DH1 shall not be used. */
#define BTIF_PKT_TYPE_NO_3DH1           0x0004  /**< Packet type 3DH1 shall not be used. */
#define BTIF_PKT_TYPE_DM1               0x0008  /**< Packet type DM1 may be used. */
#define BTIF_PKT_TYPE_DH1               0x0010  /**< Packet type DH1 may be used. */
#define BTIF_PKT_TYPE_NO_2DH3           0x0100  /**< Packet type 2DH3 shall not be used. */
#define BTIF_PKT_TYPE_NO_3DH3           0x0200  /**< Packet type 3DH3 shall not be used. */
#define BTIF_PKT_TYPE_DM3               0x0400  /**< Packet type DM3 may be used. */
#define BTIF_PKT_TYPE_DH3               0x0800  /**< Packet type DH3 may be used. */
#define BTIF_PKT_TYPE_NO_2DH5           0x1000  /**< Packet type 2DH5 shall not be used. */
#define BTIF_PKT_TYPE_NO_3DH5           0x2000  /**< Packet type 3DH5 shall not be used. */
#define BTIF_PKT_TYPE_DM5               0x4000  /**< Packet type DM5 may be used. */
#define BTIF_PKT_TYPE_DH5               0x8000  /**< Packet type DH5 may be used. */

/**
 * btif.h
 *
 * \name  BTIF_BR_SCO_PKT_TYPE
 * \brief Btif BR/EDR SCO link packet type.
 * \anchor  BTIF_BR_SCO_PKT_TYPE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_PKT_TYPE_HV1               0x0001  /**< Packet type HV1 may be used. */
#define BTIF_PKT_TYPE_HV2               0x0002  /**< Packet type HV2 may be used. */
#define BTIF_PKT_TYPE_HV3               0x0004  /**< Packet type HV3 may be used. */
#define BTIF_PKT_TYPE_EV3               0x0008  /**< Packet type EV3 may be used. */
#define BTIF_PKT_TYPE_EV4               0x0010  /**< Packet type EV4 may be used. */
#define BTIF_PKT_TYPE_EV5               0x0020  /**< Packet type EV5 may be used. */
#define BTIF_PKT_TYPE_NO_2EV3           0x0040  /**< Packet type 2EV3 shall not be used. */
#define BTIF_PKT_TYPE_NO_3EV3           0x0080  /**< Packet type 3EV3 shall not be used. */
#define BTIF_PKT_TYPE_NO_2EV5           0x0100  /**< Packet type 2EV5 shall not be used. */
#define BTIF_PKT_TYPE_NO_3EV5           0x0200  /**< Packet type 3EV5 shall not be used. */

/**
 * btif.h
 *
 * \name  BTIF_BR_L2CAP_MODE
 * \brief Btif BR/EDR L2CAP mode.
 * \anchor  BTIF_BR_L2CAP_MODE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_L2C_MODE_BASIC             (1 << 0x00)    /**< L2CAP channel mode: basic mode. */
#define BTIF_L2C_MODE_ERTM              (1 << 0x03)    /**< L2CAP channel mode: enhanced retransmission mode. */

/**
 * btif.h
 *
 * \name  BTIF_BR_L2CAP_REJ_MODE
 * \brief Btif BR/EDR L2CAP rej mode in ERTM.
 * \anchor  BTIF_BR_L2CAP_REJ_MODE
 */
/**
 * \ingroup     BTIF
 */
#define BTIF_L2C_ERTM_REJ               0x00    /**< Use REJ for L2CAP ERTM mode. */
#define BTIF_L2C_ERTM_SREJ              0x01    /**< Use SREJ for L2CAP ERTM mode. */

/**
 * btif.h
 *
 * \name  BTIF_BR_AIR_MODE
 * \brief Btif transmit air coding format on SCO/eSCO link.
 * \anchor  BTIF_BR_AIR_MODE
 */
/**
 * \ingroup     BTIF
 */
/**@{*/
#define BTIF_AIR_MODE_U_LAW             0   /**< Air mode u-law log. */
#define BTIF_AIR_MODE_A_LAW             1   /**< Air mode A-law log. */
#define BTIF_AIR_MODE_CVSD              2   /**< Air mode CVSD. */
#define BTIF_AIR_MODE_TRANSPARENT       3   /**< Air mode transparent data. */
/**@}*/

/**
 * btif.h
 *
 * \brief    Btif cause used for response messages to tell results of request.
 *           Also used in some confirmation APIs to answer indications.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_CAUSE_SUCCESS              = BTIF_SUCCESS,                             /**< Success. */
    BTIF_CAUSE_ACCEPT               = (BTIF_ERR | BTIF_ACCEPT),                 /**< Accept for indication, only used when confirmation. */
    BTIF_CAUSE_REJECT               = (BTIF_ERR | BTIF_ERR_REJECT),             /**< Reject for indication, only used when confirmation. */
    BTIF_CAUSE_RESOURCE_ERROR       = (BTIF_ERR | BTIF_ERR_NO_RESOURCE),        /**< Error happened because of resource limitation. */
    BTIF_CAUSE_INVALID_PARAM        = (BTIF_ERR | BTIF_ERR_INVALID_PARAM),      /**< Error happened because of invalid parameters. */
    BTIF_CAUSE_INVALID_STATE        = (BTIF_ERR | BTIF_ERR_INVALID_STATE),      /**< Error happened because of invalid states. */
    BTIF_CAUSE_CONN_DISCONNECT      = (BTIF_ERR | BTIF_ERR_CONN_DISCONNECT),    /**< Connection disconnected. */
    BTIF_CAUSE_CONN_LOST            = (BTIF_ERR | BTIF_ERR_CONN_LOST),          /**< Connection lost. */
    BTIF_CAUSE_AUTHEN_FAIL          = (BTIF_ERR | BTIF_ERR_AUTHEN_FAIL),        /**< Authentication failed. */
    BTIF_CAUSE_INIT_TOUT            = (BTIF_ERR | BTIF_ERR_INIT_TOUT),          /**< Timeout when stack initialize. */
    BTIF_CAUSE_INIT_OUT_OF_SYNC     = (BTIF_ERR | BTIF_ERR_INIT_OUT_OF_SYNC),   /**< Out if sync with controllor when stack initialize. */
    BTIF_CAUSE_INIT_HARDWARE_ERROR  = (BTIF_ERR | BTIF_ERR_INIT_HARDWARE_ERROR),/**< Hardware error happened when stack initialize. */
    BTIF_CAUSE_LOWER_LAYER_ERROR    = (BTIF_ERR | BTIF_ERR_LOWER_LAYER_ERROR),  /**< Error happended in lower layer. */
    BTIF_CAUSE_ADDR_NOT_RESOLVED    = (BTIF_ERR | BTIF_ERR_ADDR_NOT_RESOLVED),  /**< Address type is not resolved. */
    BTIF_CAUSE_TOUT                 = (BTIF_ERR | BTIF_ERR_TOUT),               /**< Error happened because of timeout. */

    BTIF_CAUSE_UNSPECIFIED          = (BTIF_ERR | BTIF_ERR_UNSPECIFIED),        /**< Unspecified error reason. */
    BTIF_CAUSE_NOT_SUPPORTED        = (BTIF_ERR | BTIF_ERR_NOT_SUPPORTED),      /**< Request not support. */
} T_BTIF_CAUSE;

/**
 * btif.h
 *
 * \brief    Btif key type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_KEY_COMBINATION           = 0x00, /**< Combination key. */
    BTIF_KEY_LOCAL_UNIT            = 0x01, /**< Local unit key. */
    BTIF_KEY_REMOTE_UNIT           = 0x02, /**< Remote unit key. */
    BTIF_KEY_DEBUG_COMBINATION     = 0x03, /**< Debug combination key. */
    BTIF_KEY_UNAUTHEN              = 0x04, /**< SSP generated link key without MITM protection. */
    BTIF_KEY_AUTHEN                = 0x05, /**< SSP generated link key with MITM protection. */
    BTIF_KEY_CHANGED_COMBINATION   = 0x06, /**< Changed combination key. */
    BTIF_KEY_UNAUTHEN_P256         = 0x07, /**< Security Connections generated link key without MITM protection. */
    BTIF_KEY_AUTHEN_P256           = 0x08, /**< Security Connections link key with MITM protection. */
    BTIF_KEY_LE_LOCAL_LTK          = 0x11, /**< BLE Long Term Key Blob. */
    BTIF_KEY_LE_REMOTE_LTK         = 0x12, /**< BLE Long Term Key Blob. */
    BTIF_KEY_LE_LOCAL_IRK          = 0x13, /**< BLE Identity Resolving Key. */
    BTIF_KEY_LE_REMOTE_IRK         = 0x14, /**< BLE Identity Resolving Key. */
    BTIF_KEY_LE_LOCAL_CSRK         = 0x15, /**< BLE Local CSR Key. */
    BTIF_KEY_LE_REMOTE_CSRK        = 0x16, /**< BLE Remote CSR Key. */
    BTIF_KEY_REQ_BR                = 0x80, /**< Only used to request a BR/EDR link key. */
    BTIF_KEY_DELETE                = 0xFF  /**< Link key is no longer valid and should be deleted. */
} T_BTIF_KEY_TYPE;

typedef enum
{
    BTIF_REMOTE_ADDR_LE_PUBLIC          = 0x00,
    BTIF_REMOTE_ADDR_LE_RANDOM          = 0x01,
#if F_BT_LE_PRIVACY_SUPPORT
    BTIF_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02,
    BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03,
#endif
    BTIF_REMOTE_ADDR_CLASSIC            = 0x10,
    BTIF_REMOTE_ADDR_ANY                = 0x20,
    BTIF_REMOTE_ADDR_LE_ANONYMOUS       = 0xff,
} T_BTIF_REMOTE_ADDR_TYPE;

/**
 * btif.h
 *
 * \brief    Btif operation code used to configure local device.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_DEV_CFG_OP_NAME = 0x10,        /**< Configure local name and class of device. */
    BTIF_DEV_CFG_OP_DID_EIR,            /**< Configure DID infomation in EIR. */
    BTIF_DEV_CFG_OP_EXT_EIR,            /**< Configure extra data in EIR. */
    BTIF_DEV_CFG_OP_PAGE_SCAN,          /**< Configure page scan parameters. */
    BTIF_DEV_CFG_OP_INQUIRY_SCAN,       /**< Configure inquiry scan parameters. */
    BTIF_DEV_CFG_OP_INQUIRY_MODE,       /**< Configure inquiry mode. */
    BTIF_DEV_CFG_OP_LINK_POLICY,        /**< Configure default link policy used when connection established. */
    BTIF_DEV_CFG_OP_BT_MODE,            /**< Configure Bluetooth mode. */
    BTIF_DEV_CFG_OP_ACCEPT_CONN_ROLE    /**< Configure prefered link role when accept connection request. */
} T_BTIF_DEV_CFG_OPCODE;

/**
 * btif.h
 *
 * \brief    Btif ACL status used in ACL status information message.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_ACL_CONN_ACTIVE        = 0x01, /**< ACL link in active mode. */
    BTIF_ACL_CONN_SNIFF         = 0x02, /**< ACL link in sniff mode. */
    BTIF_ACL_AUTHEN_START       = 0x03, /**< ACL link authentication start. */
    BTIF_ACL_AUTHEN_SUCCESS     = 0x04, /**< ACL link authentication success. */
    BTIF_ACL_AUTHEN_FAIL        = 0x05, /**< ACL link authentication fail. */
    BTIF_ACL_CONN_ENCRYPTED     = 0x06, /**< ACL link encrypted. */
    BTIF_ACL_CONN_DISCONN       = 0x07, /**< ACL link disconnected. */
    BTIF_ACL_CONN_NOT_ENCRYPTED = 0x08, /**< ACL link not encrypted. */
    BTIF_ACL_ADDR_RESOLVED      = 0x09, /**< Address is resolved. */
    BTIF_ACL_ROLE_MASTER        = 0x0A, /**< ACL link role is master. */
    BTIF_ACL_ROLE_SLAVE         = 0x0B, /**< ACL link role is slave. */
    BTIF_ACL_CONN_SNIFF_SUBRATE = 0x0C, /**< ACL link in sniff subrate mode. */
    BTIF_ACL_CONN_FAIL          = 0x0D, /**< ACL link is failed to connect. */
    BTIF_ACL_CONN_SUCCESS       = 0x0E, /**< ACL link is connected successfully. */
    BTIF_ACL_CONN_READY         = 0x0F, /**< ACL link is ready to be configured. */
    BTIF_ACL_CONN_BR_SC_START   = 0x10, /**< BR/EDR Secure Connections information exchange is started. */
    BTIF_ACL_CONN_BR_SC_CMPL    = 0x11  /**< BR/EDR Secure Connections information exchange is finished. */
} T_BTIF_ACL_STATUS;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_AUTHEN_START or \ref BTIF_ACL_AUTHEN_SUCCESS or \ref
 *           BTIF_ACL_AUTHEN_FAIL or \ref BTIF_ACL_CONN_ENCRYPTED or \ref BTIF_ACL_CONN_NOT_ENCRYPTED.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    T_BTIF_KEY_TYPE     key_type;       /**< Key type of the link. */
    uint8_t             key_size;       /**< Key size of the link. */
    uint8_t             encrypt_type;   /**< Encryption type of the link. */
    uint16_t            cause;          /**< Authentication result. */
} T_BTIF_ACL_STATUS_PARAM_AUTHEN;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_BR_SC_CMPL.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t             cause;         /**< BR/EDR Secure Connections information exchange result. */
} T_BTIF_ACL_STATUS_PARAM_BR_SC_CMPL;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_ADDR_RESOLVED.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                 bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE remote_addr_type;
} T_BTIF_ACL_STATUS_PARAM_RESOLVE;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_SNIFF.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    interval;           /**< Sniff interval of the ACL link. */
} T_BTIF_ACL_STATUS_PARAM_SNIFF;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_SNIFF_SUBRATE.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t max_tx_latency;  /**< Maximum latency for data transmitted to remote device. */
    uint16_t max_rx_latency;  /**< Maximum latency for data received from remote device. */
    uint16_t min_remote_tout; /**< Base sniff subrate timeout remote device shall use. */
    uint16_t min_local_tout;  /**< Base sniff subrate timeout local device will use. */
} T_BTIF_ACL_STATUS_PARAM_SNIFF_SUBRATE;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_DISCONN.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< ACL link disconnect reason. */
} T_BTIF_ACL_CONNECTION_DISCONNECT;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_FAIL.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< ACL link establish fail reason. */
} T_BTIF_ACL_CONNECTION_FAIL;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_SUCCESS.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    handle;     /**< ACL link handle. */
} T_BTIF_ACL_CONNECTION_SUCCESS;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref BTIF_ACL_CONN_READY.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    handle;     /**< ACL link handle. */
} T_BTIF_ACL_CONNECTION_READY;

/**
 * btif.h
 *
 * \brief    Parameters used in ACL status information message.
 *
 * \ingroup  BTIF
 */
typedef union
{
    T_BTIF_ACL_CONNECTION_SUCCESS         conn_success; /**< Parameter for link success to establish. */
    T_BTIF_ACL_CONNECTION_READY           conn_ready;   /**< Parameter for link ready to be used. */
    T_BTIF_ACL_CONNECTION_FAIL            conn_fail;    /**< Parameter for link fail to establish. */
    T_BTIF_ACL_CONNECTION_DISCONNECT      conn_disconn; /**< Parameter for link disconnected. */
    T_BTIF_ACL_STATUS_PARAM_AUTHEN        authen;       /**< Parameter for authentication status. */
    T_BTIF_ACL_STATUS_PARAM_RESOLVE       resolve;      /**< Address resolvable paramter. */
    T_BTIF_ACL_STATUS_PARAM_SNIFF         sniff;        /**< Parameter for ACL link sniff mode. */
    T_BTIF_ACL_STATUS_PARAM_SNIFF_SUBRATE sniff_subrate;/**< Parameter for ACL link sniff subrate. */
    T_BTIF_ACL_STATUS_PARAM_BR_SC_CMPL    br_sc_cmpl;   /**< Parameter for BR SC complete. */
} T_BTIF_ACL_STATUS_PARAM;

/**
 * btif.h
 *
 * \brief    Keypress notification type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_PASSKEY_ENTRY_STARTED,     /**< The passkey key entry protocol procedure is started. */
    BTIF_PASSKEY_DIGIT_ENTERED,     /**< A digit is entered by the remote user. */
    BTIF_PASSKEY_DIGIT_ERASED,      /**< A digit is erased by the remote user. */
    BTIF_PASSKEY_CLEARED,           /**< The display is cleared by the remote user. */
    BTIF_PASSKEY_ENTRY_CMPL,        /**< The passkey key entry protocol procedure is completed. */
} T_BTIF_KEYPRESS_NOTIF_TYPE;

/** @brief define indentify address type */
typedef enum
{
    BTIF_IDENT_ADDR_PUBLIC      = 0x00,    /*  low energy public address. */
    BTIF_IDENT_ADDR_RAND        = 0x01,    /*  low energy random address. */
} T_BTIF_IDENT_ADDR_TYPE;

/** @brief define local adress type */
typedef enum
{
    BTIF_LOCAL_ADDR_LE_PUBLIC           = 0x00,    /**<  Bluetooth low energy public address. */
    BTIF_LOCAL_ADDR_LE_RANDOM           = 0x01,    /**<  Bluetooth low energy random address. */
#if F_BT_LE_PRIVACY_SUPPORT
    BTIF_LOCAL_ADDR_LE_RAP_OR_PUBLIC    = 0x02,    /**<  Bluetooth low energy random address. */
    BTIF_LOCAL_ADDR_LE_RAP_OR_RAND      = 0x03     /**<  Bluetooth low energy random address. */
#endif
} T_BTIF_LOCAL_ADDR_TYPE;

typedef enum
{
    BTIF_ADDR_LE_RAND_STATIC            = 0x00,
    BTIF_ADDR_LE_RAND_NON_RESOLVABLE    = 0x01,
    BTIF_ADDR_LE_RAND_RESOLVABLE        = 0x02
} T_BTIF_ADDR_RAND;

typedef enum
{
    BTIF_PDU_TYPE_ANY           = 0x00,
    BTIF_PDU_TYPE_NOTIFICATION  = 0x01,
    BTIF_PDU_TYPE_INDICATION    = 0x02
} T_BTIF_GATT_PDU_TYPE;

typedef enum
{
    BTIF_GATT_DISCOVERY_SERVICE = 0x01,     /**< all primary services */
    BTIF_GATT_DISCOVERY_SERVICE_BY_UUID,    /**< service by UUID      */
    BTIF_GATT_DISCOVERY_CHARACTER,          /**< all characteristics  */
    BTIF_GATT_DISCOVERY_CHARACTER_DESC,     /**< all characteristic descriptors   */
    BTIF_GATT_DISCOVERY_RELATIONSHIP        /**< relationship (included services) */
} T_BTIF_GATT_DISCOVERY_TYPE;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_GENERIC_ELEM16;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_GENERIC_ELEM128;

typedef T_BTIF_GATT_GENERIC_ELEM16  T_BTIF_GATT_SERVICE_ELEM16;
typedef T_BTIF_GATT_GENERIC_ELEM128 T_BTIF_GATT_SERVICE_ELEM128;

typedef struct
{
    uint16_t    att_handle;
    uint16_t    end_group_handle;
} T_BTIF_GATT_SERVICE_BY_UUID_ELEM;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    properties;            /**< high nibble is reserved */
    uint16_t    value_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_CHARACT_ELEM16;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    properties;            /**< high nibble is reserved */
    uint16_t    value_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_CHARACT_ELEM128;

typedef struct
{
    uint16_t    handle;
    uint16_t    uuid16;
} T_BTIF_GATT_CHARACT_DESC_ELEM16;

typedef struct
{
    uint16_t    handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_CHARACT_DESC_ELEM128;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint16_t    uuid16;
} T_BTIF_GATT_RELATION_ELEM16;

typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint8_t     uuid128[16];
} T_BTIF_GATT_RELATION_ELEM128;

typedef enum
{
    BTIF_GATT_READ_TYPE_BASIC = 0x01,   /**< ATT "Read (Blob) Request"  */
    BTIF_GATT_READ_TYPE_BY_UUID         /**< ATT "Read By Type Request" */
} T_BTIF_GATT_READ_TYPE;

typedef enum
{
    BTIF_GATT_WRITE_TYPE_REQ = 0x01,    /**<  ATT "Write Request"  */
    BTIF_GATT_WRITE_TYPE_CMD,           /**<  ATT "Write Command"  */
    BTIF_GATT_WRITE_TYPE_PREP,          /**<  ATT "Prepare Write Request"  */
    BTIF_GATT_WRITE_TYPE_SIGNED_CMD     /**<  ATT "Signed Write Command"  */
} T_BTIF_GATT_WRITE_TYPE;

typedef enum
{
    BTIF_GATT_STORE_OP_GET_CCC_BITS,            /**< get CCC Bits for one <bd_addr,bdtype> */
    BTIF_GATT_STORE_OP_SET_CCC_BITS,            /**< set CCC Bits for one <bd_addr,bdtype> */
    BTIF_GATT_STORE_OP_DELETE_ALL_CCC_BITS,     /**< delete all CCC Bits for all peers */
} T_BTIF_GATT_STORE_OPCODE;

typedef enum
{
    BTIF_LE_ADV_MODE_DISABLED = 0,
    BTIF_LE_ADV_MODE_ENABLED
} T_BTIF_LE_ADV_MODE;

typedef enum
{
    BTIF_LE_EXT_ADV_MODE_DISABLED = 0,
    BTIF_LE_EXT_ADV_MODE_ENABLED
} T_BTIF_LE_EXT_ADV_MODE;

typedef enum
{
    BTIF_LE_PERIODIC_ADV_MODE_DISABLED = 0,
    BTIF_LE_PERIODIC_ADV_MODE_ENABLED
} T_BTIF_LE_PERIODIC_ADV_MODE;

typedef enum
{
    BTIF_LE_AE_LEGACY_ADV_CONN_N_SCAN_UNDIRECTED  = 0x13,
    BTIF_LE_AE_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED  = 0x15,
    BTIF_LE_AE_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED = 0x1D,
    BTIF_LE_AE_LEGACY_ADV_SCAN_UNDIRECTED         = 0x12,
    BTIF_LE_AE_LEGACY_ADV_NON_SCAN_NON_CONN       = 0x10,
} T_BTIF_LE_AE_LEGACY_ADV_PROPERTY;

typedef enum
{
    BTIF_LE_PRIM_ADV_PHY_LE_1M      = 1,
    BTIF_LE_PRIM_ADV_PHY_LE_CODED   = 3
} T_BTIF_LE_PRIM_ADV_PHY_TYPE;

typedef enum
{
    BTIF_LE_ADV_TYPE_UNDIRECTED = 0,
    BTIF_LE_ADV_TYPE_DIRECTED_HIGH_DUTY,
    BTIF_LE_ADV_TYPE_SCANNABLE,
    BTIF_LE_ADV_TYPE_NON_CONNECTABLE,
    BTIF_LE_ADV_TYPE_DIRECTED_LOW_DUTY
} T_BTIF_LE_ADV_TYPE;

typedef enum
{
    BTIF_LE_SCAN_REQ_NOTIFY_DISABLE = 0,
    BTIF_LE_SCAN_REQ_NOTIFY_ENABLE  = 1
} T_BTIF_LE_SCAN_REQ_NOTIFY_TYPE;

/** This enum type describes adv type for BTIF_MSG_LE_SCAN_INFO. */
typedef enum
{
    BTIF_LE_ADV_EVT_TYPE_UNDIRECTED = 0,    /**<  Connectable  undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_DIRECTED   = 1,    /**<  Connectable directed advertising. */
    BTIF_LE_ADV_EVT_TYPE_SCANNABLE  = 2,    /**<  Scanable undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_NON_CONNECTABLE,   /**<  Nonconnectable undirected advertising. */
    BTIF_LE_ADV_EVT_TYPE_SCAN_RSP           /**<  scan response. */
} T_BTIF_LE_ADV_EVT_TYPE;

typedef enum
{
    BTIF_LE_ADV_EVT_TYPE_CONNECTABLE_DIRECTED = 0x01,   /*  Connectable  undirected advertising */
} T_BTIF_LE_DIRECT_ADV_EVT_TYPE;

typedef enum
{
    BTIF_LE_ADV_FILTER_ANY = 0,
    BTIF_LE_ADV_FILTER_WHITE_LIST_SCAN,
    BTIF_LE_ADV_FILTER_WHITE_LIST_CONN,
    BTIF_LE_ADV_FILTER_WHITE_LIST_ALL,
} T_BTIF_LE_ADV_FILTER_POLICY;

typedef enum
{
    BTIF_LE_SCAN_FILTER_ANY = 0,
    BTIF_LE_SCAN_FILTER_WHITE_LIST,
    BTIF_LE_SCAN_FILTER_ANY_RPA,
    BTIF_LE_SCAN_FILTER_WHITE_LIST_RPA,
} T_BTIF_LE_SCAN_FILTER_POLICY;

typedef enum
{
    BTIF_LE_DATA_TYPE_ADV = 0,
    BTIF_LE_DATA_TYPE_SCAN_RSP
} T_BTIF_LE_ADV_DATA_TYPE;

typedef enum
{
    BTIF_LE_DATA_TYPE_EXT_ADV = 0,
    BTIF_LE_DATA_TYPE_EXT_SCAN_RSP
} T_BTIF_LE_EXT_ADV_DATA_TYPE;

typedef enum
{
    BTIF_LE_ADV_FRAG_OP_INTERMEDIATE_FRAG   = 0,
    BTIF_LE_ADV_FRAG_OP_FIRST_FRAG          = 1,
    BTIF_LE_ADV_FRAG_OP_LAST_FRAG           = 2,
    BTIF_LE_ADV_FRAG_OP_CMPL_ADV_DATA       = 3,
    BTIF_LE_ADV_FRAG_OP_UNCHANGE_UPDATE_DID = 4,
} T_BTIF_LE_ADV_FRAG_OP_TYPE;

typedef enum
{
    BTIF_LE_ADV_FRAG_PREFER_MAY_FRAG_ALL    = 0,
    BTIF_LE_ADV_FRAG_PREFER_NOT_OR_MIN_FRAG = 1
} T_BTIF_LE_ADV_FRAG_PREFERENCE_TYPE;

typedef enum
{
    BTIF_LE_SCAN_MODE_DISABLED  = 0,
    BTIF_LE_SCAN_MODE_ENABLED   = 1,
} T_BTIF_LE_SCAN_MODE;

typedef enum
{
    BTIF_LE_SCAN_DUPLICATE_FILTER_DISABLED = 0,
    BTIF_LE_SCAN_DUPLICATE_FILTER_ENABLED = 1,
    BTIF_LE_SCAN_DUPLICATE_FILTER_ENABLED_RESET_FOR_EACH_PERIOD = 2,
} T_BTIF_LE_SCAN_FILTER_DUPLICATES;

typedef enum
{
    BTIF_LE_SCAN_TYPE_PASSIVE,
    BTIF_LE_SCAN_TYPE_ACTIVE
} T_BTIF_LE_SCAN_TYPE;

typedef enum
{
    BTIF_LE_WHITE_LIST_OP_CLEAR = 0,
    BTIF_LE_WHITE_LIST_OP_ADD,
    BTIF_LE_WHITE_LIST_OP_REMOVE
} T_BTIF_LE_WHITE_LIST_OP;

typedef enum
{
    BTIF_LE_ADV_SET_OP_CLEAR = 0,
    BTIF_LE_ADV_SET_OP_ADD,
    BTIF_LE_ADV_SET_OP_REMOVE
} T_BTIF_LE_ADV_SET_OP;

typedef enum
{
    BTIF_DIRECT_ADDR_LE_PUBLIC              = 0x00,
    BTIF_DIRECT_ADDR_LE_RAND                = 0x01,
    BTIF_DIRECT_ADDR_LE_PUBLIC_IDENT        = 0x02,
    BTIF_DIRECT_ADDR_LE_RAND_IDENT          = 0x03,
    BTIF_DIRECT_ADDR_LE_RAND_UNABLE_RESOLV  = 0xFE
} T_BTIF_DIRECT_ADDR_TYPE;

typedef struct
{
    T_BTIF_LE_SCAN_TYPE scan_type;
    uint16_t            scan_interval;
    uint16_t            scan_window;
} T_BTIF_LE_EXT_SCAN_PARAM;

typedef struct
{
    uint8_t     adv_handle;
    uint16_t    duration;
    uint8_t     max_ext_adv_evt;
} T_BTIF_LE_EXT_ADV_SET_PARAM;

typedef enum
{
    BTIF_LE_PERIODIC_ADV_LIST_OP_CLEAR = 0,
    BTIF_LE_PERIODIC_ADV_LIST_OP_ADD,
    BTIF_LE_PERIODIC_ADV_LIST_OP_REMOVE
} T_BTIF_LE_PERIODIC_ADV_LIST_OP;

typedef enum
{
    BTIF_LE_RESOLVING_LIST_OP_CLEAR = 0,
    BTIF_LE_RESOLVING_LIST_OP_ADD,
    BTIF_LE_RESOLVING_LIST_OP_REMOVE
} T_BTIF_LE_RESOLVING_LIST_OP;

typedef enum
{
    BTIF_LE_NETWORK_PRIVACY = 0,
    BTIF_LE_DEVICE_PRIVACY
} T_BTIF_LE_PRIVACY_MODE;

typedef enum
{
    BTIF_LE_PHY_1M       = 0x01,
    BTIF_LE_PHY_2M       = 0x02,
    BTIF_LE_PHY_CODED    = 0x03
} T_BTIF_LE_PHY_TYPE;

typedef enum
{
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_NO   = 0,
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_S2   = 1,
    BTIF_LE_PHY_OPTIONS_LE_CODED_PREFERENCE_S8   = 2
} T_BTIF_LE_PHY_OPTIONS;

typedef struct
{
    uint16_t        scan_interval;
    uint16_t        scan_window;
    uint16_t        conn_interval_min;
    uint16_t        conn_interval_max;
    uint16_t        conn_latency;
    uint16_t        supv_tout;
    uint16_t        ce_len_min;
    uint16_t        ce_len_max;
} T_BTIF_LE_CONN_REQ_PARAM;

typedef enum
{
    T_BTIF_SERVICE_APP_DEF      = 0,   /**< application provides services */
} T_BTIF_GATT_SERVICE_ID;

typedef enum
{
    BTIF_L2C_LE_CONN_ACCEPT                       = L2C_LE_CREDIT_CONN_SUCCESS,
    BTIF_L2C_LE_CONN_PSM_NOT_SUPPORTED            = (L2C_ERR | L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED),
    BTIF_L2C_LE_CONN_NO_RESOURCES                 = (L2C_ERR | L2C_LE_CREDIT_RSP_NO_RESOURCES),
    BTIF_L2C_LE_CONN_INSUFFICIENT_AUTHEN          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN),
    BTIF_L2C_LE_CONN_INSUFFICIENT_AUTHOR          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR),
    BTIF_L2C_LE_CONN_INSUFFICIENT_ENCRYP_KEY_SIZE = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE),
    BTIF_L2C_LE_CONN_INSUFFICIENT_ENCRYP          = (L2C_ERR | L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP),
    BTIF_L2C_LE_CONN_INVAlID_SOURCE_CID           = (L2C_ERR | L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID),
    BTIF_L2C_LE_CONN_SOURCE_CID_ALREADY_ALLOC     = (L2C_ERR | L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC)
} T_BTIF_L2C_LE_CONN_STATUS;

typedef enum
{
    BTIF_LE_SECURITY_NONE,
    BTIF_LE_SECURITY_UNAUTHEN_ENCRYT,
    BTIF_LE_SECURITY_AUTHEN_ENCRYT,
    BTIF_LE_SECURITY_UNAUTHEN_DATA_SIGN,
    BTIF_LE_SECURITY_AUTHEN_DATA_SIGN,
    BTIF_LE_SECURITY_AUTHOR
} T_BTIF_LE_SECURITY_MODE;

/**
 * btif.h
 *
 * \brief    Definition of input/output capabilities.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_IOCAP_DISPLAY_ONLY,       /**< Only a Display present, no Keyboard or Yes/No Keys. */
    BTIF_IOCAP_DISPLAY_YES_NO,     /**< Display and Yes/No Keys present. */
    BTIF_IOCAP_KEYBOARD_ONLY,      /**< only a Keyboard present, no Display. */
    BTIF_IOCAP_NO_IO,              /**< No input/output capabilities. */
    BTIF_IOCAP_KEYBOARD_DISPLAY    /**< Keyboard and Display present, <b>used in BLE only.</b> */
} T_BTIF_IOCAP;

/**
 * btif.h
 *
 * \brief    BR/EDR bluetooth mode.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BT_MODE21_DISABLED,    /**< Disable all Bluetooth 2.1 functionality. */
    BTIF_BT_MODE21_ENABLED      /**< Enable Bluetooth 2.1 functionality. */
} T_BTIF_BT_MODE;

/**
 * btif.h
 *
 * \brief    BR/EDR radio mode.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_RADIO_VISIBLE_CONNECTABLE  = 0x01,  /**< Enable both inquiry scan and page scan. */
    BTIF_RADIO_VISIBLE              = 0x02,  /**< Only enable inquiry scan. */
    BTIF_RADIO_CONNECTABLE          = 0x03,  /**< Only enable page scan. */
    BTIF_RADIO_NON_DISCVBL          = 0x04,  /**< Disable both inquiry scan and page scan. */
    BTIF_RADIO_OFF                  = 0x06   /**< Radio off mode. */
} T_BTIF_RADIO_MODE;

/**
 * btif.h
 *
 * \brief    BR/EDR page scan type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BR_PAGE_SCAN_STANDARD  = 0,    /**< BR/EDR page scan type standard. */
    BTIF_BR_PAGE_SCAN_INTERLACED,       /**< BR/EDR page scan type interlaced. */
} T_BTIF_BR_PAGE_SCAN_TYPE;

/**
 * btif.h
 *
 * \brief    BR/EDR inquiry scan type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BR_INQUIRY_SCAN_STANDARD = 0,  /**< BR/EDR inquiry scan type standard. */
    BTIF_BR_INQUIRY_SCAN_INTERLACED,    /**< BR/EDR inquiry scan type interlaced. */
} T_BTIF_BR_INQUIRY_SCAN_TYPE;

/**
 * btif.h
 *
 * \brief    BR/EDR inquiry inquiry result type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BR_STANDARD_INQUIRY_RESULT = 0, /**< Inquiry reusult standard format. */
    BTIF_BR_INQUIRY_RESULT_WITH_RSSI,    /**< Inquiry result format with RSSI. */
    BTIF_BR_EXTENDED_INQUIRY_RESULT,     /**< Inquiry result with RSSI format or Extended inquiry result format. */
} T_BTIF_BR_INQUIRY_MODE;

/**
 * btif.h
 *
 * \brief    BR/EDR link policy.
 *
 * \ingroup  BTIF
 */
typedef uint8_t T_BTIF_BR_LINK_POLICY;

/**
 * btif.h
 *
 * \brief    BR/EDR link role.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_BR_LINK_ROLE_MASTER,   /**< BR/EDR link role master. */
    BTIF_BR_LINK_ROLE_SLAVE     /**< BR/EDR link role slave. */
} T_BTIF_BR_LINK_ROLE;

/**
 * btif.h
 *
 * \brief    Operation code used to configure ACL link.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_ACL_CFG_LINK_POLICY,       /**< Configure ACL link policy. */
    BTIF_ACL_CFG_LINK_SUPV_TOUT,    /**< Configure ACL link supervision timeout value. */
    BTIF_ACL_CFG_LINK_ROLE,         /**< Configure ACL link role. */
    BTIF_ACL_CFG_PKT_TYPE,          /**< Configure ACL link packet type. */
    BTIF_ACL_CFG_ENTER_SNIFF_MODE,  /**< Configure ACL to enter sniff mode. */
    BTIF_ACL_CFG_EXIT_SNIFF_MODE,   /**< Configure ACL to exit sniff mode. */
} T_BTIF_ACL_CFG_OPCODE;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link when operation code is \ref BTIF_ACL_CFG_LINK_POLICY.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    T_BTIF_BR_LINK_POLICY   policy;     /**< ACL link policy paramter to be configured. */
} T_BTIF_ACL_CFG_PARAM_LINK_POLICY;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link when operation code is \ref BTIF_ACL_CFG_LINK_SUPV_TOUT.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t                tout;       /**< ACL link supervision timeout value to be configured. */
} T_BTIF_ACL_CFG_PARAM_LINK_SUPV_TOUT;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link when operation code is \ref BTIF_ACL_CFG_LINK_ROLE.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    T_BTIF_BR_LINK_ROLE      role;      /**< ACL link role to be configured. */
} T_BTIF_ACL_CFG_PARAM_LINK_ROLE;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link when operation code is \ref BTIF_ACL_CFG_ENTER_SNIFF_MODE.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t  min_interval;    /**< Min sniff interval, only even values between 0x0002 and 0xFFFE are valid. */
    uint16_t  max_interval;    /**< Max sniff interval, only even values between 0x0002 and 0xFFFE are valid, should be larger than min_interval. */
    uint16_t  sniff_attempt;   /**< Number of baseband receive slots for sniff attempt. */
    uint16_t  sniff_tout;      /**< Number of baseband receive slots for sniff timeout. */
    uint16_t  max_latency;     /**< Max allowed sniff subrate of the remote device. Set to 0 if do not want to configure sniff subrate. */
    uint16_t  min_remote_tout; /**< Min base sniff subrate timeout the remote device may use. */
    uint16_t  min_local_tout;  /**< Min base sniff subrate timeout the local device may use. */
} T_BTIF_ACL_CFG_PARAM_SNIFF_MODE;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link when operation code is \ref BTIF_ACL_CFG_PKT_TYPE.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t        type;       /**< ACL link packet type to be configured. */
} T_BTIF_ACL_CFG_PKT_TYPE;

/**
 * btif.h
 *
 * \brief    Parameters used to configure ACL link.
 *
 * \ingroup  BTIF
 */
typedef union
{
    T_BTIF_ACL_CFG_PARAM_LINK_POLICY    link_policy; /**< Parameter for link policy. */
    T_BTIF_ACL_CFG_PARAM_LINK_SUPV_TOUT supv_tout;   /**< Parameter for supervision timeout. */
    T_BTIF_ACL_CFG_PARAM_LINK_ROLE      link_role;   /**< Parameter for link role. */
    T_BTIF_ACL_CFG_PARAM_SNIFF_MODE     sniff_mode;  /**< Parameter for sniff mode. */
    T_BTIF_ACL_CFG_PKT_TYPE             pkt_type;    /**< Parameter for link packet type. */
} T_BTIF_ACL_CFG_PARAM;

/**
 * btif.h
 *
 * \brief    SDP UUID type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_UUID16,    /**< UUID type 16 bits */
    BTIF_UUID32,    /**< UUID type 32 bits */
    BTIF_UUID128    /**< UUID type 128 bits */
} T_BTIF_UUID_TYPE;

/**
 * btif.h
 *
 * \brief    SDP UUID data.
 *
 * \ingroup  BTIF
 */
typedef union
{
    uint16_t    uuid_16;        /**< UUID data in 16 bits */
    uint32_t    uuid_32;        /**< UUID data in 32 bits */
    uint8_t     uuid_128[16];   /**< UUID data in 128 bits */
} T_BTIF_UUID_DATA;

/**
 * btif.h
 *
 * \brief    SDP discovery type.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_SDP_DISCOV_TYPE_BR,    /**< Discover normal SDP information */
    BTIF_SDP_DISCOV_TYPE_DID    /**< Discover DID information */
} T_BTIF_SDP_DISCOV_TYPE;

/**
 * btif.h
 *
 * \brief    L2CAP cause used in L2CAP connection confirm API.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_L2C_CONN_ACCEPT               = L2C_CONN_ACCEPT,                        /**< L2CAP connection accept */
    BTIF_L2C_CONN_PENDING              = (L2C_ERR | L2C_ERR_PENDING),            /**< L2CAP connection pending */
    BTIF_L2C_CONN_INVALID_PSM          = (L2C_ERR | L2C_ERR_INVALID_PSM),        /**< L2CAP connection reject because of PSM not support */
    BTIF_L2C_CONN_SECURITY_BLOCK       = (L2C_ERR | L2C_ERR_SECURITY_BLOCK),     /**< L2CAP connection reject because of security block */
    BTIF_L2C_CONN_NO_RESOURCE          = (L2C_ERR | L2C_ERR_NO_RESOURCE),        /**< L2CAP connection reject because of no resources available */
    BTIF_L2C_CONN_INVALID_PARAM        = (L2C_ERR | L2C_ERR_INVALID_PARAM),      /**< L2CAP connection reject because of invalid parameter */
    BTIF_L2C_CONN_INVALID_SOURCE_CID   = (L2C_ERR | L2C_ERR_INVALID_SOURCE_CID), /**< L2CAP connection reject because of invalid source CID */
    BTIF_L2C_CONN_SOURCE_CID_ALLOCATED = (L2C_ERR | L2C_ERR_SOURCE_CID_ALLOCATED)/**< L2CAP connection reject because of source CID already allocated */
} T_BTIF_L2C_CONN_CAUSE;

/**
 * btif.h
 *
 * \brief    SCO packet status.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_SCO_PKT_STATUS_OK           = 0,   /**< SCO packet status for correctly received data */
    BTIF_SCO_PKT_STATUS_INVALID      = 1,   /**< SCO packet status for invalid data */
    BTIF_SCO_PKT_STATUS_NO_DATA      = 2,   /**< SCO packet status for no data received */
    BTIF_SCO_PKT_STATUS_PARTIAL_LOST = 3    /**< SCO packet status for data partially lost */
} T_BTIF_SCO_PKT_STATUS;

/**
 * btif.h
 *
 * \brief    Btif message types.
 *
 * \ingroup  BTIF
 */
typedef enum
{
    BTIF_MSG_REGISTER_REQ = 0x01,
    BTIF_MSG_REGISTER_RSP,                  /**< Response of registering callback function. */
    BTIF_MSG_RELEASE_REQ,
    BTIF_MSG_RELEASE_RSP,                   /**< Response of releasing callback function. */
    BTIF_MSG_ACT_INFO,                      /**< Information of Bluetooth stack initialization. */
    BTIF_MSG_DEV_CFG_REQ,
    BTIF_MSG_DEV_CFG_RSP,                   /**< Response of configuring local device. */

    BTIF_MSG_ACL_STATUS_INFO,               /**< Information of ACL link status. */
    BTIF_MSG_ACL_PRIORITY_SET_REQ,
    BTIF_MSG_ACL_PRIORITY_SET_RSP,          /**< Response of setting acl priority. */

    BTIF_MSG_READ_RSSI_REQ,
    BTIF_MSG_READ_RSSI_RSP,                 /**< Response of reading RSSI information. */
    BTIF_MSG_VENDOR_CMD_REQ,
    BTIF_MSG_VENDOR_CMD_RSP,                /**< Response of vendor command. */
    BTIF_MSG_VENDOR_EVT_INFO,               /**< Information of vendor event. */

    BTIF_MSG_PAIRABLE_MODE_SET_REQ,
    BTIF_MSG_PAIRABLE_MODE_SET_RSP,         /**< Response of setting radio mode. */
    BTIF_MSG_USER_PASSKEY_REQ_IND,          /**< Indication of user passkey request. */
    BTIF_MSG_USER_PASSKEY_REQ_CFM,
    BTIF_MSG_USER_PASSKEY_REQ_REPLY_REQ,
    BTIF_MSG_USER_PASSKEY_REQ_REPLY_RSP,    /**< Response of user passkey request reply. */
    BTIF_MSG_USER_PASSKEY_NOTIF_INFO,       /**< Information of user passkey notification. */
    BTIF_MSG_AUTHEN_RESULT_IND,             /**< Indication of authentication result. */
    BTIF_MSG_AUTHEN_RESULT_CFM,
    BTIF_MSG_AUTHEN_KEY_REQ_IND,            /**< Indication of authentication key request. */
    BTIF_MSG_AUTHEN_KEY_REQ_CFM,
    BTIF_MSG_USER_CFM_REQ_IND,              /**< Indication of user confirm request. */
    BTIF_MSG_USER_CFM_REQ_CFM,
    BTIF_MSG_KEYPRESS_NOTIF_REQ,
    BTIF_MSG_KEYPRESS_NOTIF_RSP,            /**< Response of keypress notification. */
    BTIF_MSG_KEYPRESS_NOTIF_INFO,           /**< Information of remote keypress notification. */
    BTIF_MSG_HW_ERROR_INFO,                 /**< Indication of hardware error. */

    BTIF_MSG_REMOTE_OOB_REQ_IND = 0x0100,   /**< Indication of remote OOB data request. */
    BTIF_MSG_REMOTE_OOB_REQ_CFM,

    // GATT
    // server
    BTIF_MSG_GATT_SRV_REG_REQ,
    BTIF_MSG_GATT_SRV_REG_RSP,
    BTIF_MSG_GATT_ATTR_UPDATE_REQ,
    BTIF_MSG_GATT_ATTR_UPDATE_RSP,
    BTIF_MSG_GATT_ATTR_UPDATE_STATUS_IND,
    BTIF_MSG_GATT_ATTR_UPDATE_STATUS_CFM,
    BTIF_MSG_GATT_ATTR_READ_IND,
    BTIF_MSG_GATT_ATTR_READ_CFM,
    BTIF_MSG_GATT_ATTR_WRITE_REQ_IND,
    BTIF_MSG_GATT_ATTR_WRITE_REQ_CFM,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_IND,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_CFM,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_IND,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_CFM,
    BTIF_MSG_GATT_ATTR_WRITE_CMD_INFO,//0x110
    BTIF_MSG_GATT_ATTR_CCCD_INFO,
    // client
    BTIF_MSG_GATT_DISCOVERY_REQ,
    BTIF_MSG_GATT_DISCOVERY_RSP,
    BTIF_MSG_GATT_DISCOVERY_IND,
    BTIF_MSG_GATT_DISCOVERY_CFM,
    BTIF_MSG_GATT_ATTR_READ_REQ,
    BTIF_MSG_GATT_ATTR_READ_RSP,
    BTIF_MSG_GATT_ATTR_READ_MULTI_REQ,
    BTIF_MSG_GATT_ATTR_READ_MULTI_RSP,
    BTIF_MSG_GATT_ATTR_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_WRITE_RSP,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_PREP_WRITE_RSP,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_REQ,
    BTIF_MSG_GATT_ATTR_EXEC_WRITE_RSP,

    BTIF_MSG_GATT_ATTR_IND,//0x120
    BTIF_MSG_GATT_ATTR_CFM,
    BTIF_MSG_GATT_ATTR_NOTIF_INFO,

    // common
    BTIF_MSG_GATT_SECURITY_REQ,
    BTIF_MSG_GATT_SECURITY_RSP,
    BTIF_MSG_GATT_SERVER_STORE_IND,
    BTIF_MSG_GATT_SERVER_STORE_CFM,
    BTIF_MSG_GATT_MTU_SIZE_INFO,
    // end of GATT

    BTIF_MSG_LE_CONN_REQ,
    BTIF_MSG_LE_CONN_RSP,
    BTIF_MSG_LE_CONN_IND,
    BTIF_MSG_LE_CONN_CFM,
    BTIF_MSG_LE_CONN_CMPL_INFO,
    BTIF_MSG_LE_DISCONN_REQ,
    BTIF_MSG_LE_DISCONN_RSP,
    BTIF_MSG_LE_DISCONN_IND,
    BTIF_MSG_LE_DISCONN_CFM,//0x130

    BTIF_MSG_LE_ADV_REQ,
    BTIF_MSG_LE_ADV_RSP,
    BTIF_MSG_LE_ADV_PARAM_SET_REQ,
    BTIF_MSG_LE_ADV_PARAM_SET_RSP,
    BTIF_MSG_LE_ADV_DATA_SET_REQ,
    BTIF_MSG_LE_ADV_DATA_SET_RSP,
    BTIF_MSG_LE_SCAN_REQ,
    BTIF_MSG_LE_SCAN_RSP,
    BTIF_MSG_LE_SCAN_PARAM_SET_REQ,
    BTIF_MSG_LE_SCAN_PARAM_SET_RSP,
    BTIF_MSG_LE_SCAN_INFO,
    BTIF_MSG_LE_MODIFY_WHITE_LIST_REQ,
    BTIF_MSG_LE_MODIFY_WHITE_LIST_RSP,
    BTIF_MSG_LE_CONN_UPDATE_REQ,
    BTIF_MSG_LE_CONN_UPDATE_RSP,
    BTIF_MSG_LE_CONN_UPDATE_IND,//0x140
    BTIF_MSG_LE_CONN_UPDATE_CFM,
    BTIF_MSG_LE_CONN_PARAM_INFO,
    BTIF_MSG_LE_CONN_PARAM_REQ_EVT_INFO,
    BTIF_MSG_LE_CONN_PARAM_REQ_REPLY_REQ,
    BTIF_MSG_LE_CONN_PARAM_REQ_REPLY_RSP,
    BTIF_MSG_LE_CONN_PARAM_REQ_NEG_REPLY_REQ,
    BTIF_MSG_LE_CONN_PARAM_REQ_NEG_REPLY_RSP,

    BTIF_MSG_LE_CREDIT_BASED_CONN_REQ,
    BTIF_MSG_LE_CREDIT_BASED_CONN_RSP,
    BTIF_MSG_LE_CREDIT_BASED_CONN_IND,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CFM,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_REQ,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_IND,
    BTIF_MSG_LE_CREDIT_BASED_DISCONN_CFM,
    BTIF_MSG_LE_SEND_CREDIT_REQ,//0x150
    BTIF_MSG_LE_SEND_CREDIT_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DATA_REQ,
    BTIF_MSG_LE_CREDIT_BASED_DATA_RSP,
    BTIF_MSG_LE_CREDIT_BASED_DATA_IND,
    BTIF_MSG_LE_CREDIT_BASED_DATA_CFM,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CMPL_INFO,
    BTIF_MSG_LE_CREDIT_BASED_CONN_CREDIT_INFO,
    BTIF_MSG_LE_CREDIT_BASED_SECURITY_REG_REQ,
    BTIF_MSG_LE_CREDIT_BASED_SECURITY_REG_RSP,
    BTIF_MSG_LE_CREDIT_BASED_PSM_REG_REQ,
    BTIF_MSG_LE_CREDIT_BASED_PSM_REG_RSP,
    BTIF_MSG_LE_SET_CHANN_CLASSIF_REQ,
    BTIF_MSG_LE_SET_CHANN_CLASSIF_RSP,
    BTIF_MSG_LE_READ_CHANN_MAP_REQ,
    BTIF_MSG_LE_READ_CHANN_MAP_RSP,
    BTIF_MSG_LE_RECEIVER_TEST_REQ,
    BTIF_MSG_LE_RECEIVER_TEST_RSP,
    BTIF_MSG_LE_TRANSMITTER_TEST_REQ,//0x162
    BTIF_MSG_LE_TRANSMITTER_TEST_RSP,
    BTIF_MSG_LE_TEST_END_REQ,
    BTIF_MSG_LE_TEST_END_RSP,
    BTIF_MSG_LE_READ_ADV_TX_POWER_REQ,
    BTIF_MSG_LE_READ_ADV_TX_POWER_RSP,
    BTIF_MSG_LE_SET_RAND_ADDR_REQ,
    BTIF_MSG_LE_SET_RAND_ADDR_RSP,
    BTIF_MSG_LE_READ_MAX_DATA_LEN_REQ,
    BTIF_MSG_LE_READ_MAX_DATA_LEN_RSP,
    BTIF_MSG_LE_READ_DEFAULT_DATA_LEN_REQ,
    BTIF_MSG_LE_READ_DEFAULT_DATA_LEN_RSP,
    BTIF_MSG_LE_WRITE_DEFAULT_DATA_LEN_REQ,
    BTIF_MSG_LE_WRITE_DEFAULT_DATA_LEN_RSP,
    BTIF_MSG_LE_SET_DATA_LEN_REQ,//0x172
    BTIF_MSG_LE_SET_DATA_LEN_RSP,
    BTIF_MSG_LE_DATA_LEN_CHANGE_INFO,
    BTIF_MSG_LE_MODIFY_RESOLV_LIST_REQ,
    BTIF_MSG_LE_MODIFY_RESOLV_LIST_RSP,
    BTIF_MSG_LE_READ_PEER_RESOLV_ADDR_REQ,
    BTIF_MSG_LE_READ_PEER_RESOLV_ADDR_RSP,
    BTIF_MSG_LE_READ_LOCAL_RESOLV_ADDR_REQ,
    BTIF_MSG_LE_READ_LOCAL_RESOLV_ADDR_RSP,
    BTIF_MSG_LE_SET_RESOLUTION_ENABLE_REQ,
    BTIF_MSG_LE_SET_RESOLUTION_ENABLE_RSP,
    BTIF_MSG_LE_SET_RESOLV_PRIV_ADDR_TOUT_REQ,
    BTIF_MSG_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP,
    BTIF_MSG_LE_CONFIG_LOCAL_IRK_REQ,
    BTIF_MSG_LE_CONFIG_LOCAL_IRK_RSP,
    BTIF_MSG_LE_SET_PRIVACY_MODE_REQ,
    BTIF_MSG_LE_SET_PRIVACY_MODE_RSP,//0x182
    BTIF_MSG_LE_DIRECT_ADV_INFO,
    BTIF_MSG_LE_HIGH_DUTY_ADV_TOUT_INFO,

    BTIF_MSG_LE_SET_ADV_SET_RAND_ADDR_REQ,
    BTIF_MSG_LE_SET_ADV_SET_RAND_ADDR_RSP,

    BTIF_MSG_LE_EXT_ADV_PARAM_SET_REQ,
    BTIF_MSG_LE_EXT_ADV_PARAM_SET_RSP,
    BTIF_MSG_LE_EXT_ADV_DATA_SET_REQ,
    BTIF_MSG_LE_EXT_ADV_DATA_SET_RSP,
    BTIF_MSG_LE_EXT_ADV_ENABLE_REQ,
    BTIF_MSG_LE_EXT_ADV_ENABLE_RSP,

    BTIF_MSG_LE_EXT_SCAN_PARAM_SET_REQ,
    BTIF_MSG_LE_EXT_SCAN_PARAM_SET_RSP,
    BTIF_MSG_LE_EXT_SCAN_REQ,
    BTIF_MSG_LE_EXT_SCAN_RSP,//0x190

    BTIF_MSG_LE_SET_DEFAULT_PHY_REQ,
    BTIF_MSG_LE_SET_DEFAULT_PHY_RSP,

    BTIF_MSG_LE_SET_PHY_REQ,
    BTIF_MSG_LE_SET_PHY_RSP,

    BTIF_MSG_LE_ENHANCED_RECEIVER_TEST_REQ,
    BTIF_MSG_LE_ENHANCED_RECEIVER_TEST_RSP,
    BTIF_MSG_LE_ENHANCED_TRANSMITTER_TEST_REQ,
    BTIF_MSG_LE_ENHANCED_TRANSMITTER_TEST_RSP,

    BTIF_MSG_LE_MODIFY_PERIODIC_ADV_LIST_REQ,
    BTIF_MSG_LE_MODIFY_PERIODIC_ADV_LIST_RSP,

    BTIF_MSG_LE_READ_RF_PATH_COMPENSATION_REQ,
    BTIF_MSG_LE_READ_RF_PATH_COMPENSATION_RSP,

    BTIF_MSG_LE_WRITE_RF_PATH_COMPENSATION_REQ,
    BTIF_MSG_LE_WRITE_RF_PATH_COMPENSATION_RSP,

    BTIF_MSG_LE_MODIFY_ADV_SET_REQ,
    BTIF_MSG_LE_MODIFY_ADV_SET_RSP,//0x1A0

    BTIF_MSG_LE_SET_PERIODIC_ADV_PARAM_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_PARAM_RSP,
    BTIF_MSG_LE_SET_PERIODIC_ADV_DATA_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_DATA_RSP,
    BTIF_MSG_LE_SET_PERIODIC_ADV_ENABLE_REQ,
    BTIF_MSG_LE_SET_PERIODIC_ADV_ENABLE_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP,
    BTIF_MSG_LE_PERIODIC_ADV_TERMINATE_SYNC_REQ,
    BTIF_MSG_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP,

    BTIF_MSG_LE_PHY_UPDATE_INFO,
    BTIF_MSG_LE_EXT_ADV_REPORT_INFO,
    BTIF_MSG_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO,
    BTIF_MSG_LE_PERIODIC_ADV_REPORT_INFO,//0x1B0
    BTIF_MSG_LE_PERIODIC_ADV_SYNC_LOST_INFO,
    BTIF_MSG_LE_SCAN_TIMEOUT_INFO,
    BTIF_MSG_LE_ADV_SET_TERMINATED_INFO,
    BTIF_MSG_LE_SCAN_REQ_RECEIVED_INFO,
    BTIF_MSG_LE_LOCAL_DEV_INFO,
    BTIF_MSG_LE_ENABLE_EXT_ADV_MODE_REQ,
    BTIF_MSG_LE_ENABLE_EXT_ADV_MODE_RSP,
    BTIF_MSG_LE_REMOTE_FEATS_INFO,
    BTIF_MSG_GATT_ATTR_WRITE_CMD_SIGNED_INFO,
    BTIF_MSG_LE_SIGNED_STATUS_INFO,

    BTIF_MSG_JUST_WORK_REQ_IND,
    BTIF_MSG_JUST_WORK_REQ_CFM,

    BTIF_MSG_USER_PASSKEY_NOTIF_IND,
    BTIF_MSG_USER_PASSKEY_NOTIF_CFM,

    BTIF_MSG_GATT_SRV2_REG_REQ,

    BTIF_MSG_LE_SET_SUPPL_TEST_PARAMS_REQ = 0x0300,
    BTIF_MSG_LE_SET_SUPPL_TEST_PARAMS_RSP,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_PARAMS_REQ,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_PARAMS_RSP,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_ENABLE_REQ,
    BTIF_MSG_LE_SET_CONNLESS_SUPPL_TRANSMIT_ENABLE_RSP,
    BTIF_MSG_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_REQ,
    BTIF_MSG_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP,
    BTIF_MSG_LE_SET_CONN_SUPPL_PARAMS_REQ,
    BTIF_MSG_LE_SET_CONN_SUPPL_PARAMS_RSP,
    BTIF_MSG_LE_CONN_SUPPL_REQUEST_ENABLE_REQ,
    BTIF_MSG_LE_CONN_SUPPL_REQUEST_ENABLE_RSP,
    BTIF_MSG_LE_CONN_SUPPL_RESPONSE_ENABLE_REQ,
    BTIF_MSG_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP,
    BTIF_MSG_LE_READ_ANTENNA_INFORMATION_REQ,
    BTIF_MSG_LE_READ_ANTENNA_INFORMATION_RSP,

    BTIF_MSG_LE_CONNLESS_IQ_REPORT_INFO,
    BTIF_MSG_LE_CONN_IQ_REPORT_INFO,
    BTIF_MSG_LE_SUPPL_REQUEST_FAILED_INFO,

    BTIF_MSG_RADIO_MODE_SET_REQ = 0x1000,
    BTIF_MSG_RADIO_MODE_SET_RSP,            /**< Response of setting radio mode. */

    BTIF_MSG_INQUIRY_REQ,
    BTIF_MSG_INQUIRY_RSP,                   /**< Response of starting inquiry. */
    BTIF_MSG_INQUIRY_RESULT_INFO,           /**< Information of device found during inquiry. */
    BTIF_MSG_INQUIRY_CMPL_INFO,             /**< Information of inquiry complete. */
    BTIF_MSG_INQUIRY_CANCEL_REQ,
    BTIF_MSG_INQUIRY_CANCEL_RSP,            /**< Response of canceling inquiry. */

    BTIF_MSG_REMOTE_NAME_REQ,
    BTIF_MSG_REMOTE_NAME_RSP,               /**< Response of getting remote name. */

    BTIF_MSG_SDP_DISCOV_REQ,
    BTIF_MSG_SDP_DISCOV_RSP,                /**< Response of starting SDP discovery. */
    BTIF_MSG_SDP_ATTR_IND,                  /**< Indication of attributes found during SDP discovery. */
    BTIF_MSG_SDP_ATTR_CFM,
    BTIF_MSG_DID_ATTR_IND,                  /**< Indication of attributes found during DID discovery. */
    BTIF_MSG_DID_ATTR_CFM,
    BTIF_MSG_SDP_DISCOV_CMPL_INFO,          /**< Information of SDP discovery complete. */
    BTIF_MSG_STOP_SDP_DISCOV_REQ,
    BTIF_MSG_STOP_SDP_DISCOV_RSP,           /**< Response of stopping SDP discovery. */

    BTIF_MSG_AUTHEN_REQ,
    BTIF_MSG_AUTHEN_RSP,                    /**< Response of BR/EDR authentication. */
    BTIF_MSG_AUTHOR_REQ_IND,                /**< Indication of authorization. */
    BTIF_MSG_AUTHOR_REQ_CFM,
    BTIF_MSG_PIN_CODE_REQ_IND,              /**< Indication of PIN code request. */
    BTIF_MSG_PIN_CODE_REQ_CFM,
    BTIF_MSG_LEGACY_REMOTE_OOB_REQ_IND,     /**< Indication of legacy remote OOB data request. */
    BTIF_MSG_LEGACY_REMOTE_OOB_REQ_CFM,
    BTIF_MSG_LOCAL_OOB_REQ,
    BTIF_MSG_LOCAL_OOB_RSP,                 /**< Response of getting local OOB data. */

    BTIF_MSG_ACL_CFG_REQ,
    BTIF_MSG_ACL_CFG_RSP,                   /**< Response of configuring ACL link. */

    BTIF_MSG_SDP_REG_REQ,
    BTIF_MSG_SDP_REG_RSP,                   /**< Response of registering SDP record to Bluetooth stack. */
    BTIF_MSG_L2C_PROTO_REG_REQ,
    BTIF_MSG_L2C_PROTO_REG_RSP,             /**< Response of registering protocol over L2CAP. */
    BTIF_MSG_L2C_CONN_REQ,
    BTIF_MSG_L2C_CONN_RSP,                  /**< Response of L2CAP connection request. */
    BTIF_MSG_L2C_CONN_IND,                  /**< Indication of L2CAP connection request from remote device. */
    BTIF_MSG_L2C_CONN_CFM,
    BTIF_MSG_L2C_CONN_CMPL_INFO,            /**< Information of L2CAP channel establish result. */
    BTIF_MSG_L2C_DATA_REQ,
    BTIF_MSG_L2C_DATA_RSP,                  /**< Response of L2CAP data. */
    BTIF_MSG_L2C_DATA_IND,                  /**< Indication of received L2CAP data from remote device. */
    BTIF_MSG_L2C_DATA_CFM,
    BTIF_MSG_L2C_DISCONN_REQ,
    BTIF_MSG_L2C_DISCONN_RSP,               /**< Response of disconnecting L2CAP channel. */
    BTIF_MSG_L2C_DISCONN_IND,               /**< Indication of L2CAP disconnection from remote device. */
    BTIF_MSG_L2C_DISCONN_CFM,
    BTIF_MSG_L2C_SET_LOCAL_BUSY_REQ,
    BTIF_MSG_L2C_SET_LOCAL_BUSY_RSP,        /**< Response of setting local busy state in ERTM mode. */

    BTIF_MSG_L2C_SEC_REG_REQ,
    BTIF_MSG_L2C_SEC_REG_RSP,               /**< Response of registering L2CAP security entry. */
    BTIF_MSG_RFC_AUTHEN_REQ,
    BTIF_MSG_RFC_AUTHEN_RSP,                /**< Response of authencation over RFCOMM channel. */

    BTIF_MSG_SCO_CONN_REQ,
    BTIF_MSG_SCO_CONN_RSP,                  /**< Response of creating SCO/eSCO connection. */
    BTIF_MSG_SCO_CONN_IND,                  /**< Indication of SCO/eSCO connection request from remote device. */
    BTIF_MSG_SCO_CONN_CFM,
    BTIF_MSG_SCO_CONN_CMPL_INFO,            /**< Information of SCO/eSCO connection establish result. */
    BTIF_MSG_SCO_DATA_REQ,
    BTIF_MSG_SCO_DATA_RSP,                  /**< Response of setting SCO data to remote device. */
    BTIF_MSG_SCO_DATA_IND,                  /**< Indication of SCO data received from remote device. */
    BTIF_MSG_SCO_DATA_CFM,
    BTIF_MSG_SCO_DISCONN_REQ,
    BTIF_MSG_SCO_DISCONN_RSP,               /**< Response of disconnecting SCO/eSCO link. */
    BTIF_MSG_SCO_DISCONN_IND,               /**< Indication of SCO/eSCO disconnection from remote device. */
    BTIF_MSG_SCO_DISCONN_CFM,

    BTIF_MSG_ACL_DISCONN_REQ,
    BTIF_MSG_ACL_DISCONN_RSP,               /**< Response of disconnecting ACL link. */

    BTIF_MSG_SETUP_QOS_REQ,
    BTIF_MSG_SETUP_QOS_RSP,                 /**< Response of setting QOS for an ACL link. */

    /*connectionless slave broadcast*/
    BTIF_MSG_LT_ADDR_SET_REQ,
    BTIF_MSG_LT_ADDR_SET_RSP,
    BTIF_MSG_LT_ADDR_DEL_REQ,
    BTIF_MSG_LT_ADDR_DEL_RSP,
    BTIF_MSG_SYNC_TRAIN_PARAM_SET_REQ,
    BTIF_MSG_SYNC_TRAIN_PARAM_SET_RSP,
    BTIF_MSG_SYNC_TRAIN_REQ,
    BTIF_MSG_SYNC_TRAIN_RSP,
    BTIF_MSG_SYNC_TRAIN_CMPL_INFO,
    BTIF_MSG_SYNC_TRAIN_RCV_REQ,
    BTIF_MSG_SYNC_TRAIN_RCV_RSP,
    BTIF_MSG_SYNC_TRAIN_RCV_CMPL_INFO,
    BTIF_MSG_CSB_SET_REQ,
    BTIF_MSG_CSB_SET_RSP,
    BTIF_MSG_CSB_RCV_SET_REQ,
    BTIF_MSG_CSB_RCV_SET_RSP,
    BTIF_MSG_CSB_TOUT_INFO,
    BTIF_MSG_CSB_DATA_REQ,
    BTIF_MSG_CSB_DATA_RSP,
    BTIF_MSG_CSB_DATA_IND,
    BTIF_MSG_CSB_DATA_CFM,

    BTIF_MSG_IDLE,
} T_BTIF_MSG_TYPE;

/**
 * btif.h
 *
 * \brief    Response of registering callback function. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_REGISTER_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint32_t    version;    /**< Bluetooth stack software version. */
    uint16_t    cause;      /**< Result of register callback function. */
} T_BTIF_REGISTER_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_RELEASE_RSP;

/**
 * btif.h
 *
 * \brief    Local device information. It will be received in the callback function
 *           registered by \ref btif_register_req with message type as \ref
 *           BTIF_MSG_ACT_INFO. This message will be received right after register
 *           response \ref BTIF_MSG_REGISTER_RSP if register successfully.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t     local_addr[6];  /**< Local bluetooth address. */
    uint16_t    cause;          /**< Result of bluetooth stack initialization. */
} T_BTIF_ACT_INFO;

/**
 * btif.h
 *
 * \brief    Response of configuring local device. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_DEV_CFG_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    T_BTIF_DEV_CFG_OPCODE   op;     /**< Operation code for device configuration. */
    uint16_t                cause;  /**< Device configuration result. */
} T_BTIF_DEV_CFG_RSP;

/**
 * btif.h
 *
 * \brief    ACL status information. It will be received in the callback function
 *           registered by \ref btif_register_req with message type as \ref
 *           BTIF_MSG_ACL_STATUS_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    T_BTIF_ACL_STATUS           status;             /**< ACL status information type. */
    T_BTIF_ACL_STATUS_PARAM     p;                  /**< ACL status information parameter. */
} T_BTIF_ACL_STATUS_INFO;

/**
 * btif.h
 *
 * \brief    Response of setting link proirity. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_ACL_PRIORITY_SET_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t     bd_addr[6];       /**< Bluetooth address of remote device. */
    uint8_t     conn_type;        /**< Connection type, 0:BR/EDR ACL link,
                                       1:SCO/ESCO link, 2:LE link. */
    bool        is_high_priority; /**< Set the link to high priority or not. */
    uint16_t    cause;            /**< Result of setting priority of the link. */
} T_BTIF_ACL_PRIORITY_SET_RSP;

/**
 * btif.h
 *
 * \brief    Response of reading RSSI. It will be received in the callback function
 *           registered by \ref btif_register_req with message type as \ref BTIF_MSG_READ_RSSI_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                 bd_addr[6];       /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE remote_addr_type; /**< Address type of remote device. */
    uint16_t                cause;            /**< Result of reading RSSI. */
    int8_t                  rssi;             /**< RSSI value read if success. */
} T_BTIF_READ_RSSI_RSP;

/**
 * btif.h
 *
 * \brief    Response of vendor command. It will be received in the callback function
 *           registered by \ref btif_register_req with message type as \ref BTIF_MSG_VENDOR_CMD_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    command;        /**< Vendor command. Shoule be the same as that in request. */
    uint16_t    cause;          /**< Result of executing command. */
    bool        is_cmpl_evt;    /**< Whether the response is generated from command complete event. */
    uint8_t     param_len;      /**< Response parameter length. */
    uint8_t     param[1];       /**< Response parameter buffer. */
} T_BTIF_VENDOR_CMD_RSP;

/**
 * btif.h
 *
 * \brief    Vendor event infomation. It will be received in the callback function
 *           registered by \ref btif_register_req with message type as \ref BTIF_MSG_VENDOR_EVT_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t     param_len;  /**< Information parameter length. */
    uint8_t     param[1];   /**< Information parameter buffer. */
} T_BTIF_VENDOR_EVT_INFO;

/**
 * btif.h
 *
 * \brief    Response of setting pairable mode. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_PAIRABLE_MODE_SET_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;          /**< Result of set pairable mode. */
} T_BTIF_PAIRABLE_MODE_SET_RSP;

/**
 * btif.h
 *
 * \brief    Indication to request user to input passkey. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_USER_PASSKEY_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    bool                        key_press;          /**< Whether keypress is needed. */
} T_BTIF_USER_PASSKEY_REQ_IND;

/**
 * btif.h
 *
 * \brief    Response of user passkey request reply. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_USER_PASSKEY_REQ_REPLY_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of user passkey request reply. */
} T_BTIF_USER_PASSKEY_REQ_REPLY_RSP;

/**
 * btif.h
 *
 * \brief    Information of user passkey notification. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_USER_PASSKEY_NOTIF_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    uint32_t                    display_value;      /**< Passkey value. */
} T_BTIF_USER_PASSKEY_NOTIF_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint32_t                    display_value;
} T_BTIF_USER_PASSKEY_NOTIF_IND;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_JUST_WORK_REQ_IND;

/**
 * btif.h
 *
 * \brief    Indication of authentication result. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_AUTHEN_RESULT_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    uint8_t                     key_len;            /**< Key length. */
    T_BTIF_KEY_TYPE             key_type;           /**< Key type. */
    uint8_t                     link_key[28];       /**< Key value. */
    uint16_t                    cause;              /**< Authentication result. */
} T_BTIF_AUTHEN_RESULT_IND;

/**
 * btif.h
 *
 * \brief    Indication to request key during authentication. It will be received
 *           in the callback function registered by \ref btif_register_req with
 *           message type as \ref BTIF_MSG_AUTHEN_KEY_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    T_BTIF_KEY_TYPE             key_type;           /**< Requested Key type. */
} T_BTIF_AUTHEN_KEY_REQ_IND;

/**
 * btif.h
 *
 * \brief    Indication to request user confirmation when pairing. It will be received
 *           in the callback function registered by \ref btif_register_req with
 *           message type as \ref BTIF_MSG_USER_CFM_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    uint32_t                    display_value;      /**< Numeric value to be displayed. */
} T_BTIF_USER_CFM_REQ_IND;

/**
 * btif.h
 *
 * \brief    Response of keypress notification request. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_KEYPRESS_NOTIF_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t                    cause;              /**< Keypress notification result. */
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
} T_BTIF_KEYPRESS_NOTIF_RSP;

/**
 * btif.h
 *
 * \brief    Information of remote keypress notification. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_KEYPRESS_NOTIF_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
    T_BTIF_KEYPRESS_NOTIF_TYPE  event_type;         /**< Keypress notification type. */
} T_BTIF_KEYPRESS_NOTIF_INFO;

/**
 * btif.h
 *
 * \brief    Information of hardware error. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_HW_ERROR_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t     hw_code;    /**< Hardware error code. */
} T_BTIF_HW_ERROR_INFO;

/**
 * btif.h
 *
 * \brief    Indication to request remote LE OOB data. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_REMOTE_OOB_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                 bd_addr[6]; /**< Bluetooth address of remote device */
    T_BTIF_REMOTE_ADDR_TYPE addr_type;  /**< Address type of remote device */
} T_BTIF_REMOTE_OOB_REQ_IND;

typedef struct
{
    void       *p_srv_handle;
    uint16_t    cause;
} T_BTIF_GATT_SRV_REG_RSP;

typedef struct
{
    void          *p_srv_handle;
    void          *p_req_handle;
    uint16_t       link_id;
    uint16_t       attr_index;
    uint16_t       cause;
} T_BTIF_GATT_ATTR_UPDATE_RSP;

typedef struct
{
    void                       *p_srv_handle;
    void                       *p_req_handle;
    uint16_t                    link_id;
    uint16_t                    attr_index;
    uint16_t                    cause;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_GATT_ATTR_UPDATE_STATUS_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       attr_index;
    uint16_t       read_offset;
} T_BTIF_GATT_ATTR_READ_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       attr_index;
    uint16_t       attr_len;
    uint16_t       handle;
    uint16_t       write_offset;
    uint16_t       gap;
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_WRITE_IND;

typedef struct
{
    uint16_t    link_id;
    uint8_t     flags;      /**<  0x00-Cancel all writes, 0x01-write all values */
} T_BTIF_GATT_ATTR_EXEC_WRITE_IND;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       count;              /**< nbr. of attr_index/CCCD value pairs */
    uint16_t       gap;                /**< offset of first pair in data[]       */
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_CCCD_INFO;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    cause;
} T_BTIF_GATT_DISCOVERY_RSP;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    cause;
    uint16_t                    elem_cnt;
    uint16_t                    elem_len;
    uint16_t                    gap;
    uint8_t                     list[1];
} T_BTIF_GATT_DISCOVERY_IND;

typedef struct
{
    uint16_t                link_id;
    T_BTIF_GATT_READ_TYPE   read_type;
    uint16_t                cause;
    uint16_t                read_offset;
    uint16_t                total_len;
    uint16_t                attr_len;
    uint16_t                num_handle;
    uint16_t                gap;
    uint8_t                 handles[1];
} T_BTIF_GATT_ATTR_READ_RSP;

#if F_BT_LE_ATT_READ_MULTI_SUPPORT
typedef struct
{
    uint16_t        link_id;        /**<  local link ID. */
    uint16_t        cause;          /**<  More detailed result information. */
    uint16_t        total_len;      /**<  Total number of bytes stored in the value[] array. */
    uint16_t        gap;            /**<  offset of first handle in value[] */
    uint8_t         value[1];       /**<  Array of attribute values read. */
} T_BTIF_GATT_ATTR_READ_MULTI_RSP;
#endif

typedef struct
{
    uint16_t                link_id;
    T_BTIF_GATT_WRITE_TYPE  write_type;
    uint16_t                attr_handle;
    uint16_t                cause;
    uint16_t                attr_len;       /**< attrib. value length */
    uint16_t                gap;            /**< offset of attrib. value in data[] */
    uint8_t                 data[1];
} T_BTIF_GATT_ATTR_WRITE_RSP;

typedef struct
{
    uint16_t    link_id;        /**<  local link ID.  */
    uint16_t    cause;          /**<  More detailed result information. */
    uint16_t    write_offset;   /**<  The offset of the first octet to be written.  */
    uint16_t    attr_len;       /**<  attrib. value length.  */
    uint16_t    gap;            /**<  offset of attrib. value in data[].  */
    uint8_t     data[1];        /**<  attrib. value in data[].  */
} T_BTIF_GATT_ATTR_PREP_WRITE_RSP;

typedef struct
{
    uint16_t    link_id;    /**<  local link ID.  */
    uint16_t    cause;      /**<  More detailed result information. */
} T_BTIF_GATT_ATTR_EXEC_WRITE_RSP;

typedef struct
{
    uint16_t       link_id;
    uint16_t       attr_handle;
    uint16_t       attr_len;        /**< attrib. value length */
    uint16_t       gap;             /**< offset of attrib. value in data[] */
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_IND;

typedef T_BTIF_GATT_ATTR_IND T_BTIF_GATT_ATTR_NOTIF_INFO;

typedef struct
{
    uint16_t            link_id;
    T_BTIF_KEY_TYPE     key_type;
    uint8_t             key_size;
    uint16_t            cause;
} T_BTIF_GATT_SECURITY_RSP;

typedef struct
{
    T_BTIF_GATT_STORE_OPCODE    op;
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     bd_addr[6];
    uint8_t                     data_len;
    uint8_t                    *p_data;
} T_BTIF_GATT_SERVER_STORE_IND;

typedef struct
{
    uint16_t       link_id;
    uint16_t       mtu_size;
} T_BTIF_GATT_MTU_SIZE_INFO;

typedef struct
{
    uint16_t       link_id;
    uint16_t       cause;
    bool           update_local;
    uint32_t       local_sign_count;
    uint32_t       remote_sign_count;
} T_BTIF_LE_SIGNED_STATUS_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint16_t                    link_id;
    uint16_t                    cause;
} T_BTIF_LE_CONN_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
}  T_BTIF_LE_CONN_IND;

typedef struct
{
    uint16_t                    cause;
    uint16_t                    link_id;
    uint16_t                    mtu_size;
    uint8_t                     ds_credits;
    T_BTIF_LOCAL_ADDR_TYPE      own_addr_type;
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     bd_addr[6];
    uint16_t                    interval;
    uint16_t                    latency;
    uint16_t                    supv_tout;
    T_BTIF_LE_PHY_TYPE          tx_phy;
    T_BTIF_LE_PHY_TYPE          rx_phy;
    uint8_t                     csa;
} T_BTIF_LE_CONN_CMPL_INFO;

#if F_BT_LE_READ_REMOTE_FEATS
typedef struct
{
    uint16_t                    cause;
    uint16_t                    link_id;
    uint8_t                     remote_feats[8];
} T_BTIF_LE_REMOTE_FEATS_INFO;
#endif

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
    uint16_t        reason;
} T_BTIF_LE_DISCONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
} T_BTIF_LE_DISCONN_IND;

typedef struct
{
    T_BTIF_LE_ADV_MODE      adv_mode;
    uint16_t                cause;
} T_BTIF_LE_ADV_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ADV_PARAM_SET_RSP;

typedef struct
{
    T_BTIF_LE_ADV_DATA_TYPE data_type;
    uint16_t                cause;
} T_BTIF_LE_ADV_DATA_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SCAN_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SCAN_PARAM_SET_RSP;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_LE_ADV_EVT_TYPE      adv_type;
    int8_t                      rssi;
    uint8_t                     data_len;
    uint8_t                     data[31];
} T_BTIF_LE_SCAN_INFO;

typedef struct
{
    T_BTIF_LE_WHITE_LIST_OP     operation;
    uint16_t                    cause;
} T_BTIF_LE_MODIFY_WHITE_LIST_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
} T_BTIF_LE_CONN_UPDATE_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_UPDATE_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_PARAM_INFO;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
} T_BTIF_LE_CONN_PARAM_REQ_EVT_INFO;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_CONN_PARAM_REQ_REPLY_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_CONN_PARAM_REQ_NEG_REPLY_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_CONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_CONN_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DISCONN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DISCONN_IND;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_SEND_CREDIT_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_DATA_RSP;

typedef struct
{
    uint16_t        link_id;        /**<  local link ID.  */
    uint16_t        channel;        /**<  channel ID  */
    uint16_t        value_len;      /**<  value length  */
    uint16_t        gap;            /**<  offset of value in data[].  */
    uint8_t         data[1];        /**<  value.  */
} T_BTIF_LE_CREDIT_BASED_DATA_IND;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;            /**<  local link ID.  */
    uint16_t        channel;            /**<  channel ID  */
    uint16_t        remote_mtu;         /**<  remote Maximum Transmission Unit */
    uint16_t        remote_credits;     /**<  remote initial credits*/
    uint16_t        max_ds_credits;     /**<  max downstream credits */
} T_BTIF_LE_CREDIT_BASED_CONN_CMPL_INFO;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_CONN_CREDIT_INFO;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_SECURITY_REG_RSP;

typedef struct
{
    uint16_t        le_psm;
    uint16_t        cause;
} T_BTIF_LE_CREDIT_BASED_PSM_REG_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_CHANN_CLASSIF_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        cause;
    uint8_t         chann_map[5];
} T_BTIF_LE_READ_CHANN_MAP_RSP;
#if F_BT_LE_4_0_DTM_SUPPORT
typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_RECEIVER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_TRANSMITTER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        num_pkts;
} T_BTIF_LE_TEST_END_RSP;
#endif
typedef struct
{
    uint16_t        cause;
    uint8_t         tx_power_level;
} T_BTIF_LE_READ_ADV_TX_POWER_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_RAND_ADDR_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
    uint16_t        max_rx_oct;
    uint16_t        max_rx_time;
} T_BTIF_LE_READ_MAX_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
} T_BTIF_LE_READ_DEFAULT_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_WRITE_DEFAULT_DATA_LEN_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_SET_DATA_LEN_RSP;

typedef struct
{
    uint16_t        link_id;
    uint16_t        max_tx_oct;
    uint16_t        max_tx_time;
    uint16_t        max_rx_oct;
    uint16_t        max_rx_time;
} T_BTIF_LE_DATA_LEN_CHANGE_INFO;

typedef struct
{
    T_BTIF_LE_RESOLVING_LIST_OP operation;
    uint16_t                    cause;
} T_BTIF_LE_MODIFY_RESOLV_LIST_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     peer_resolv_addr[6];
} T_BTIF_LE_READ_PEER_RESOLV_ADDR_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     local_resolv_addr[6];
} T_BTIF_LE_READ_LOCAL_RESOLV_ADDR_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_RESOLUTION_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP;

//#if F_BT_LE_LOCAL_IRK_SETTING_SUPPORT
typedef struct
{
    uint16_t    cause;
    bool        generate;
    uint8_t     local_irk[16];
} T_BTIF_LE_CONFIG_LOCAL_IRK_RSP;
//#endif

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_SET_PRIVACY_MODE_RSP;

typedef struct
{
    uint8_t                         bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE         remote_addr_type;
    uint8_t                         direct_bd_addr[6];
    T_BTIF_DIRECT_ADDR_TYPE         direct_addr_type;
    T_BTIF_LE_DIRECT_ADV_EVT_TYPE   direct_adv_type;
    int8_t                          rssi;
} T_BTIF_LE_DIRECT_ADV_INFO;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_HIGH_DUTY_ADV_TIMEOUT_INFO;

#if F_BT_LE_5_0_SUPPORT

#if F_BT_LE_5_0_AE_ADV_SUPPORT
typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         rand_addr[6];
} T_BTIF_LE_SET_ADV_SET_RAND_ADDR_RSP;

typedef struct
{
    int8_t          selected_tx_power;
    uint16_t        cause;
} T_BTIF_LE_EXT_ADV_PARAM_SET_RSP;

typedef struct
{
    T_BTIF_LE_EXT_ADV_DATA_TYPE data_type;
    uint16_t                         cause;
} T_BTIF_LE_EXT_ADV_DATA_SET_RSP;

typedef struct
{
    T_BTIF_LE_EXT_ADV_MODE mode;
    uint16_t                    cause;
} T_BTIF_LE_EXT_ADV_ENABLE_RSP;
#endif

#if F_BT_LE_5_0_AE_SCAN_SUPPORT
typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_EXT_SCAN_PARAM_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_EXT_SCAN_RSP;
#endif

#if F_BT_LE_5_0_SET_PHYS_SUPPORT
typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_SET_DEFAULT_PHY_RSP;

typedef struct
{
    uint16_t        cause;
    uint16_t        link_id;
} T_BTIF_LE_SET_PHY_RSP;
#endif

#if F_BT_LE_5_0_DTM_SUPPORT
typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ENHANCED_RECEIVER_TEST_RSP;

typedef struct
{
    uint16_t        cause;
} T_BTIF_LE_ENHANCED_TRANSMITTER_TEST_RSP;
#endif
typedef struct
{
    T_BTIF_LE_PERIODIC_ADV_LIST_OP  op;
    uint16_t                        cause;
} T_BTIF_LE_MODIFY_PERIODIC_ADV_LIST_RSP;

typedef struct
{
    uint16_t    cause;
    int8_t      rf_tx_path_compensation;
    int8_t      rf_rx_path_compensation;
} T_BTIF_LE_READ_RF_PATH_COMPENSATION_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_WRITE_RF_PATH_COMPENSATION_RSP;

typedef struct
{
    T_BTIF_LE_ADV_SET_OP    op;
    uint16_t                cause;
} T_BTIF_LE_MODIFY_ADV_SET_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_PARAM_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_DATA_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_SET_PERIODIC_ADV_ENABLE_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP;

typedef struct
{
    uint16_t                cause;
} T_BTIF_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP;

#if F_BT_LE_5_0_SET_PHYS_SUPPORT
typedef struct
{
    uint16_t            cause;
    uint16_t            link_id;
    T_BTIF_LE_PHY_TYPE  tx_phy;
    T_BTIF_LE_PHY_TYPE  rx_phy;
} T_BTIF_LE_PHY_UPDATE_INFO;
#endif
typedef enum
{
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_COMPLETE = 0x00,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_MORE     = 0x01,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_TRUNCATED = 0x02,
    BTIF_ADV_EVENT_TYPE_DATA_STATUS_RFU      = 0x03
} T_BTIF_EXT_ADV_EVT_TYPE_DATA_STATUS;

typedef struct
{
    uint16_t                            event_type;
    T_BTIF_EXT_ADV_EVT_TYPE_DATA_STATUS data_status;
    T_BTIF_REMOTE_ADDR_TYPE             addr_type;
    uint8_t                             bd_addr[6];
    T_BTIF_LE_PRIM_ADV_PHY_TYPE         primary_phy;
    uint8_t                             secondary_phy;
    uint8_t                             adv_sid;
    int8_t                              tx_power;
    int8_t                              rssi;
    uint16_t                            peri_adv_interval;
    T_BTIF_DIRECT_ADDR_TYPE             direct_addr_type;
    uint8_t                             direct_addr[6];
    uint8_t                             data_len;
    uint8_t                            *p_data; //max 229 bytes
} T_BTIF_LE_EXT_ADV_REPORT_INFO;

typedef struct
{
    uint16_t            cause;
    uint8_t             sync_handle;
    uint8_t             advertising_sid;
    uint8_t             advertiser_address_type;
    uint8_t             advertiser_address[6];
    T_BTIF_LE_PHY_TYPE  advertiser_phy;
    uint8_t             periodic_advertising_interval;
    uint8_t             advertiser_clock_accuracy;
} T_BTIF_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO;

typedef struct
{
    uint16_t    sync_handle;
    int8_t      tx_power;
    int8_t      rssi;
    uint8_t     rfu;
    uint8_t     data_status;
    uint8_t     data_len;
    uint8_t    *p_data;
} T_BTIF_LE_PERIODIC_ADV_REPORT_INFO;

typedef struct
{
    uint8_t     sync_handle;
} T_BTIF_LE_PERIODIC_ADV_SYNC_LOST_INFO;

typedef struct
{
    uint16_t    cause;
    uint8_t     adv_handle;
    uint16_t    conn_handle;
    uint8_t     num_cmpl_ext_adv_evt;
} T_BTIF_LE_ADV_SET_TERMINATED_INFO;

typedef struct
{
    uint8_t     adv_handle;
    uint8_t     scanner_addr_type;
    uint8_t     scanner_addr[6];
} T_BTIF_LE_SCAN_REQ_RECEIVED_INFO;

typedef struct
{
    uint16_t    cause;
    uint16_t    le_max_ext_adv_len;
    uint8_t     le_max_num_of_adv_sets;
    uint8_t     le_periodic_adv_list_size;
} T_BTIF_LE_ENABLE_EXT_ADV_MODE_RSP;
#endif
typedef struct
{
    uint8_t     le_local_feats[8];
    uint16_t    le_ds_pool_id;
    uint16_t    le_ds_data_offset;
    uint8_t     le_ds_credits;
    uint8_t     le_max_wl_size;
    uint8_t     le_resolvable_list_size;
    int8_t      le_min_tx_power;
    int8_t      le_max_tx_power;
} T_BTIF_LE_LOCAL_DEV_INFO;

/**
 * btif.h
 *
 * \brief    Response of setting radio mode. It will be received in the callback
 *           function registered by \ref btif_register_req with message type
 *           as \ref BTIF_MSG_RADIO_MODE_SET_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of set radio mode */
} T_BTIF_RADIO_MODE_SET_RSP;

/**
 * btif.h
 *
 * \brief    Response of starting inquiry. It will be received in the callback
 *           function registered by \ref btif_register_req with message type
 *           as \ref BTIF_MSG_INQUIRY_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of start inquiry */
} T_BTIF_INQUIRY_RSP;

/**
 * btif.h
 *
 * \brief    Information of devices which is found duing inquiry. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_INQUIRY_RESULT_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t     bd_addr[6];             /**< Bluetooth address of device found when inquiry */
    uint32_t    cod;                    /**< Class of device of device found when inquiry */
    int8_t      rssi;                   /**< RSSI of device found when inquiry */
    uint8_t     name[BTIF_NAME_LENGTH]; /**< Name of device found when inquiry */
} T_BTIF_INQUIRY_RESULT_INFO;

/**
 * btif.h
 *
 * \brief    Information of inquiry procedure complete. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_INQUIRY_CMPL_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Status of inquiry procedure */
} T_BTIF_INQUIRY_CMPL_INFO;

/**
 * btif.h
 *
 * \brief    Response of canceling inquiry. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_INQUIRY_CANCEL_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of cancel inquiry */
} T_BTIF_INQUIRY_CANCEL_RSP;

/**
 * btif.h
 *
 * \brief    Response of getting name of remote device. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_REMOTE_NAME_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;                  /**< Result of getting remote device's name */
    uint8_t     bd_addr[6];             /**< Bluetooth address of remote device */
    uint8_t     name[BTIF_NAME_LENGTH]; /**< Name of remote device */
} T_BTIF_REMOTE_NAME_RSP;

/**
 * btif.h
 *
 * \brief    Response of starting SDP discovery. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_SDP_DISCOV_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6];     /**< Bluetooth address of remote device */
    uint16_t        cause;          /**< Result of starting SDP discovery */
} T_BTIF_SDP_DISCOV_RSP;

/**
 * btif.h
 *
 * \brief    Indication of attributes found during SDP discovery. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SDP_ATTR_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                 bd_addr[6];          /**< Bluetooth address of remote device */
    uint8_t                 server_channel;      /**< Server channel number */
    uint8_t                 supported_repos;     /**< Supported repositories */
    uint16_t                l2c_psm;             /**< L2CAP PSM value */
    T_BTIF_UUID_TYPE        srv_class_uuid_type; /**< Service class UUID type */
    T_BTIF_UUID_DATA        srv_class_uuid_data; /**< Service class UUID data */
    uint16_t                remote_version;      /**< Remote profile version */
    uint16_t                supported_feat;      /**< Supported features */
    uint8_t                *p_ext_data;          /**< Address of extra data if used */
} T_BTIF_SDP_ATTR_IND;

/**
 * btif.h
 *
 * \brief    Indication of DID information found during SDP discovery. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_DID_ATTR_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t        specific_id; /**< Specific ID of remote device */
    uint16_t        vendor_id;   /**< Vendor ID of remote device */
    uint16_t        vendor_src;  /**< Verndor source of remote device */
    uint16_t        product_id;  /**< Product ID of remote device */
    uint16_t        version;     /**< Version of remote device */
    uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
} T_BTIF_DID_ATTR_IND;

/**
 * btif.h
 *
 * \brief    Information of SDP discovery complete. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_SDP_DISCOV_CMPL_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
    uint16_t        cause;       /**< Status of SDP discovery procedure */
} T_BTIF_SDP_DISCOV_CMPL_INFO;

/**
 * btif.h
 *
 * \brief    Response of stopping SDP discovery. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_STOP_SDP_DISCOV_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
    uint16_t        cause;       /**< Result of stop SDP discovey */
} T_BTIF_STOP_SDP_DISCOV_RSP;

/**
 * btif.h
 *
 * \brief    Response of BR/EDR authentication request. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_AUTHEN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6]; /**< Bluetooth address of remote device */
    uint16_t        cause;      /**< Result of authentication procedure */
} T_BTIF_AUTHEN_RSP;

/**
 * btif.h
 *
 * \brief    Indication to request user's authorization for service connection
 *           establish. It will be received in the callback function registered by
 *           \ref btif_register_req with message type as \ref BTIF_MSG_AUTHOR_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t        psm;            /**< PSM of protocol which the service is based on */
    uint16_t        server_chann;   /**< Server channel number when PSM is RFCOMM, otherwise is 0 */
    uint16_t        uuid;           /**< UUID of the service */
    uint8_t         bd_addr[6];     /**< Bluetooth address of remote device */
    uint8_t         outgoing;       /**< Direction of the service, 1 for outgoing and 0 for incoming. */
} T_BTIF_AUTHOR_REQ_IND;

/**
 * btif.h
 *
 * \brief    Indication to request PIN code. It will be received in the callback
 *           function registered by \ref btif_register_req with message type
 *           as \ref BTIF_MSG_PIN_CODE_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6];     /**< Bluetooth address of remote device */
} T_BTIF_PIN_CODE_REQ_IND;

/**
 * btif.h
 *
 * \brief    Indication to request BR/EDR OOB data of remote device. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_LEGACY_REMOTE_OOB_REQ_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         bd_addr[6];     /**< Bluetooth address of remote device */
} T_BTIF_LEGACY_REMOTE_OOB_REQ_IND;

/**
 * btif.h
 *
 * \brief    Response of getting local BR/EDR OOB data. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_LOCAL_OOB_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t         data_c[16];     /**< OOB data simple pairing hash C. */
    uint8_t         data_r[16];     /**< OOB data simple pairing randonizer R. */
    uint16_t        cause;          /**< Result of getting local OOB data. */
} T_BTIF_LOCAL_OOB_RSP;

/**
 * btif.h
 *
 * \brief    Response of configuring ACL link. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_ACL_CFG_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t                     bd_addr[6]; /**< Bluetooth address of remote device. */
    T_BTIF_ACL_CFG_OPCODE       op;         /**< Operation code used to configure ACL link. */
    uint16_t                    cause;      /**< Result of configuring ACL link. */
} T_BTIF_ACL_CFG_RSP;

/**
 * btif.h
 *
 * \brief    Response of registering SDP record. It will be received in the callback
 *           function registered by \ref btif_register_req with message type as
 *           \ref BTIF_MSG_SDP_REG_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of registering SDP record to Bluetooth stack. */
} T_BTIF_SDP_REG_RSP;

/**
 * btif.h
 *
 * \brief    Response of registering protocol over L2CAP. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_L2C_PROTO_REG_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t  psm;      /**< PSM of the protocol. */
    uint16_t  proto_id; /**< Protocol ID assigned by upper layer. Should be the same as request. */
    uint16_t  cause;    /**< Result of registering protocol to Bluetooth stack. */
} T_BTIF_L2C_PROTO_REG_RSP;

/**
 * btif.h
 *
 * \brief    Response of starting L2CAP connection request. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_L2C_CONN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cid;        /**< Local CID assigned by Bluetooth stack. */
    uint16_t    proto_id;   /**< Protocol ID assigned by upper layer. Should be the same as request. */
    uint16_t    cause;      /**< Result of executing L2CAP connection request from upper layer. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_L2C_CONN_RSP;

/**
 * btif.h
 *
 * \brief    Indication of remote L2CAP connection request. It will be received in
 *           the callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_L2C_CONN_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cid;        /**< Local CID assigned by Bluetooth stack. */
    uint16_t    proto_id;   /**< Protocol ID which is assigned by upper layer. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_L2C_CONN_IND;

/**
 * btif.h
 *
 * \brief    Information of L2CAP connection establish result. It will be received
 *           in the callback function registered by \ref btif_register_req with
 *           message type as \ref BTIF_MSG_L2C_CONN_CMPL_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;          /**< Result of establishing a L2CAP channel with remote device. */
    uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
    uint16_t    remote_mtu;     /**< Remote MTU value. */
    uint16_t    proto_id;       /**< Protocol ID assigned by upper layer. */
    uint16_t    ds_data_offset; /**< Offset used to fill user data when sending L2CAP data to remote. */
    uint8_t     bd_addr[6];     /**< Bluetooth address of remote device. */
} T_BTIF_L2C_CONN_CMPL_INFO;

/**
 * btif.h
 *
 * \brief    Indication of L2CAP data received from remote side. It will be received
 *           in the callback function registered by \ref btif_register_req with
 *           message type as \ref BTIF_MSG_L2C_DATA_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    proto_id; /**< Protocol ID assigned by upper layer. */
    uint16_t    cid;      /**< Local CID assigned by Bluetooth stack. */
    uint16_t    length;   /**< Length of L2CAP data. */
    uint16_t    gap;      /**< Offset from data parameter to the real L2CAP data. */
    uint8_t     data[1];  /**< The rest of message which contains real L2CAP data at offset of gap. */
} T_BTIF_L2C_DATA_IND;

/**
 * btif.h
 *
 * \brief    Response of L2CAP disconnection request from upper layer. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_L2C_DISCONN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
    uint16_t    proto_id;       /**< Protocol ID assigned by upper layer. */
    uint16_t    cause;          /**< Result of disconnecting L2CAP channel with remote device. */
} T_BTIF_L2C_DISCONN_RSP;

/**
 * btif.h
 *
 * \brief    Indication of L2CAP disconnection request from remote side. It will
 *           be received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_L2C_DISCONN_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
    uint16_t    proto_id;       /**< Protocol ID assigned by upper layer. */
    uint16_t    cause;          /**< L2CAP channel disconnect reason. */
} T_BTIF_L2C_DISCONN_IND;

/**
 * btif.h
 *
 * \brief    Response of setting local device into busy state in ERTM mode. It will
 *           be received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_L2C_SET_LOCAL_BUSY_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;  /**< Result of setting local busy state in ERTM mode. */
} T_BTIF_L2C_SET_LOCAL_BUSY_RSP;

/**
 * btif.h
 *
 * \brief    Response of registering L2CAP security entry. It will be received
 *           in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_L2C_SEC_REG_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    psm;            /**< PSM of the protocol. */
    uint16_t    server_chann;   /**< Server channel when PSM is RFCOMM, otherwise is 0. */
    uint8_t     active;         /**< Active/Deactive the security entry. */
    uint16_t    uuid;           /**< UUID of the service the security entry registered for. */
    uint16_t    cause;          /**< Result of registering L2CAP level security entry. */
} T_BTIF_L2C_SEC_REG_RSP;

/**
 * btif.h
 *
 * \brief    Result of authenticaion on profile over RFCOMM channel. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_RFC_AUTHEN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    chann;      /**< Local CID assigned by Bluetooth stack. */
    uint16_t    dlci;       /**< DCLI value of the RFCOMM channel. */
    uint16_t    cause;      /**< Result of authentication on the RFCOMM channel. */
    uint8_t     outgoing;   /**< Direction of the service, 1 for outgoing and 0 for incoming. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_RFC_AUTHEN_RSP;

/**
 * btif.h
 *
 * \brief    Response of SCO/eSCO connection request from upper layer. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_CONN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of starting a SCO/eSCO connection. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_SCO_CONN_RSP;

/**
 * btif.h
 *
 * \brief    Indication of SCO/eSCO connection request from remote side. It will be
 *           received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_CONN_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint8_t  bd_addr[6]; /**< Bluetooth address of remote device. */
    uint8_t  is_esco;    /**< Connection type remote device want to create. 1 for eSCO and 0 for SCO. */
} T_BTIF_SCO_CONN_IND;

/**
 * btif.h
 *
 * \brief    Result of SCO/eSCO connection establish procedure. It will be received
 *           in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_CONN_CMPL_INFO.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    handle;     /**< Connection handle of the SCO/eSCO link. */
    uint16_t    cause;      /**< Result of establishing the SCO/eSCO link. */
    uint8_t     is_esco;    /**< Whether this link is a SCO/eSCO link. 1 for eSCO and 0 for SCO. */
    uint8_t     air_mode;   /**< Air mode of the SCO/eSCO link. */
    uint16_t    tx_length;  /**< Length of the eSCO payload in transmit direction. 0 for SCO links. */
    uint16_t    rx_length;  /**< Length of the eSCO payload in receive direction. 0 for SCO links. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_SCO_CONN_CMPL_INFO;

/**
 * btif.h
 *
 * \brief    Indication of SCO/eSCO data from remote side. It will be received
 *           in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_DATA_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t                handle; /**< Connection handle of the SCO/eSCO link. */
    T_BTIF_SCO_PKT_STATUS   status; /**< Status of the SCO/eSCO data. */
    void                   *p_buf;  /**< Address of the SCO/eSCO data. */
    uint16_t                len;    /**< Length of the SCO/eSCO data. */
} T_BTIF_SCO_DATA_IND;

/**
 * btif.h
 *
 * \brief    Response of SCO/eSCO disconnect request from upper layer. It will
 *           be received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_DISCONN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    handle; /**< Connection handle of the SCO/eSCO link. */
    uint16_t    cause;  /**< Result of disconnecting the SCO/eSCO link. */
    uint16_t    reason; /**< Disconnect reason for the SCO/eSCO link. */
} T_BTIF_SCO_DISCONN_RSP;

/**
 * btif.h
 *
 * \brief    Indication of SCO/eSCO disconnect request from remote side. It will
 *           be received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_SCO_DISCONN_IND.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    handle; /**< Connection handle of the SCO/eSCO link. */
    uint16_t    cause;  /**< Disconnect reason for the SCO/eSCO link. */
} T_BTIF_SCO_DISCONN_IND;

/**
 * btif.h
 *
 * \brief    Response of ACL disconnect request from upper layer. It will
 *           be received in the callback function registered by \ref btif_register_req
 *           with message type as \ref BTIF_MSG_ACL_DISCONN_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;      /**< Result of disconnecting ACL link. */
    uint16_t    reason;     /**< Disconnect reason for the ACL link. */
    uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_BTIF_ACL_DISCONN_RSP;

/**
 * btif.h
 *
 * \brief    Response of setup QOS for an ACL link. It will be received in the
 *           callback function registered by \ref btif_register_req with message
 *           type as \ref BTIF_MSG_SETUP_QOS_RSP.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t    cause;  /**< Result of setup QOS for the ACL link. */
    uint16_t    handle; /**< Connection handle of the ACL link. */
} T_BTIF_SETUP_QOS_RSP;

typedef enum
{
    SMP_OOB_INVALID_TYPE,
    SMP_OOB_PEER,
    SMP_OOB_LOCAL,
    SMP_OOB_BOTH
} T_BTIF_OOB_DATA_TYPE;

typedef struct
{
    uint8_t     present;
    uint8_t     rand[16];
    uint8_t     confirm[16];
    uint8_t     bd_addr_to[6];
    uint8_t     local_sk[32];
    uint8_t     local_pk[64];
} T_BTIF_LOCAL_OOB_DATA;

typedef struct
{
    uint8_t     present;
    uint8_t     rand[16];
    uint8_t     confirm[16];
    uint8_t     bd_addr_from[6];
} T_BTIF_PEER_OOB_DATA;

typedef struct
{
    T_BTIF_LOCAL_OOB_DATA   loc_oob_data;
    T_BTIF_PEER_OOB_DATA  peer_oob_data;
} T_SMP_SC_OOB_DATA;

#if F_BT_LE_6_0_AOA_AOD_SUPPORT

typedef struct
{
    uint16_t    cause;
    bool        is_receiver;
} T_BTIF_LE_SET_SUPPL_TEST_PARAMS_RSP;

#if F_BT_LE_6_0_AOX_CONNLESS_SUPPORT
typedef struct
{
    uint16_t cause;
    uint16_t sync_handle;
} T_BTIF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP;
#endif
typedef struct
{
    uint16_t    cause;
    bool        is_receiver;
} T_BTIF_LE_SET_CONN_SUPPL_PARAMS_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_CONN_SUPPL_REQUEST_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t supported_sampling_rates;
    uint8_t num_of_antennae;
    uint8_t max_suppl_length;
} T_BTIF_LE_READ_ANTENNA_INFO_RSP;



typedef struct
{
    uint16_t sync_handle;
    uint8_t  channel_index;
    uint8_t  suppl_type;
    uint8_t  slot_durations;
    uint8_t  packet_status;
    uint8_t  sample_count; //0x09 ?C 0x52
    uint8_t  iq_sample[82 + 82];
} T_BTIF_LE_CONNLESS_IQ_REPORT_INFO;

typedef struct
{
    uint16_t conn_handle;
    uint8_t  rx_phy;
    uint8_t  data_chan_index;
    uint8_t  rssi;
    uint8_t  suppl_type;
    uint8_t  slot_durations;
    uint8_t  packet_status;
    uint8_t  sample_count;//0x09 ?C 0x52
    uint8_t  iq_sample[82 + 82];
} T_BTIF_LE_CONN_IQ_REPORT_INFO;

typedef struct
{
    uint8_t  status;
    uint16_t conn_handle;

} T_BTIF_LE_SUPPL_REQUEST_FAILED_INFO;
#endif

typedef struct
{
    uint16_t    cause;
    uint8_t     lt_addr;
} T_BTIF_LT_ADDR_SET_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     lt_addr;
} T_BTIF_LT_ADDR_DEL_RSP;

typedef struct
{
    uint16_t    cause;
    uint16_t    interval;
} T_BTIF_SYNC_TRAIN_PARAM_SET_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_SYNC_TRAIN_RSP;

typedef struct
{
    uint16_t    cause;
} T_BTIF_SYNC_TRAIN_CMPL_INFO;

typedef struct
{
    uint16_t    cause;
} T_BTIF_SYNC_TRAIN_RCV_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     bd_addr[6];
    uint32_t    clk_offset;
    uint8_t     afh_chann_map[10];
    uint8_t     lt_addr;
    uint8_t     srv_data;
    uint32_t    next_broadcast_inst;
    uint16_t    csb_interval;
} T_BTIF_SYNC_TRAIN_RCV_CMPL_INFO;

typedef struct
{
    uint16_t    cause;
    uint8_t     lt_addr;
    uint16_t    interval;
} T_BTIF_CSB_SET_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     bd_addr[6];
    uint8_t     lt_addr;
} T_BTIF_CSB_RCV_SET_RSP;

typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t     lt_addr;
} T_BTIF_CSB_TOUT_INFO;

typedef struct
{
    uint16_t    cause;
    uint8_t     lt_addr;
} T_BTIF_CSB_DATA_RSP;

typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t     lt_addr;
    uint32_t    clk;
    uint32_t    offset;
    uint8_t     rcv_status;
    uint8_t     fragment;
    uint16_t    data_len;
    uint8_t     data[1];
} T_BTIF_CSB_DATA_IND;

/**
 * btif.h
 *
 * \brief    Message data in upstream message from Bluetooth stack to APP. The
 *           specific value of the data is depeneded on message type .
 *
 * \ingroup  BTIF
 */
typedef union
{
    T_BTIF_REGISTER_RSP                 register_rsp;
    T_BTIF_RELEASE_RSP                  release_rsp;
    T_BTIF_ACT_INFO                     act_info;
    T_BTIF_DEV_CFG_RSP                  dev_cfg_rsp;
    T_BTIF_ACL_STATUS_INFO              acl_status_info;
    T_BTIF_ACL_PRIORITY_SET_RSP         acl_priority_set_rsp;
    T_BTIF_READ_RSSI_RSP                read_rssi_rsp;
    T_BTIF_VENDOR_CMD_RSP               vendor_cmd_rsp;
    T_BTIF_VENDOR_EVT_INFO              vendor_evt_info;
    T_BTIF_PAIRABLE_MODE_SET_RSP        pairable_mode_set_rsp;
    T_BTIF_USER_PASSKEY_REQ_IND         user_passkey_req_ind;
    T_BTIF_JUST_WORK_REQ_IND            just_work_req_ind;
    T_BTIF_USER_PASSKEY_REQ_REPLY_RSP   user_passkey_req_reply_rsp;
    T_BTIF_USER_PASSKEY_NOTIF_INFO      user_passkey_notif_info;
    T_BTIF_USER_PASSKEY_NOTIF_IND       user_passkey_notif_ind;
    T_BTIF_AUTHEN_RESULT_IND            authen_result_ind;
    T_BTIF_AUTHEN_KEY_REQ_IND           authen_key_req_ind;
    T_BTIF_USER_CFM_REQ_IND             user_cfm_req_ind;
#if F_BT_KEY_PRESS_SUPPORT
    T_BTIF_KEYPRESS_NOTIF_RSP           keypress_notif_rsp;
    T_BTIF_KEYPRESS_NOTIF_INFO          keypress_notif_info;
#endif
    T_BTIF_HW_ERROR_INFO                hw_error_info;

    T_BTIF_REMOTE_OOB_REQ_IND           remote_ood_req_ind;
    T_BTIF_GATT_SRV_REG_RSP             gatt_srv_reg_rsp;
    T_BTIF_GATT_ATTR_UPDATE_RSP         gatt_attr_update_rsp;
    T_BTIF_GATT_ATTR_UPDATE_STATUS_IND  gatt_attr_update_status_ind;
    T_BTIF_GATT_ATTR_READ_IND           gatt_attr_read_ind;
    T_BTIF_GATT_ATTR_WRITE_IND          gatt_attr_write_ind;
    T_BTIF_GATT_ATTR_EXEC_WRITE_IND     gatt_attr_exec_write_ind;
    T_BTIF_GATT_ATTR_CCCD_INFO          gatt_attr_cccd_info;
    T_BTIF_GATT_DISCOVERY_RSP           gatt_discovery_rsp;
    T_BTIF_GATT_DISCOVERY_IND           gatt_discovery_ind;
    T_BTIF_GATT_ATTR_READ_RSP           gatt_attr_read_rsp;
#if F_BT_LE_ATT_READ_MULTI_SUPPORT
    T_BTIF_GATT_ATTR_READ_MULTI_RSP     gatt_attr_read_multi_rsp;
#endif
    T_BTIF_GATT_ATTR_WRITE_RSP          gatt_attr_write_rsp;
    T_BTIF_GATT_ATTR_PREP_WRITE_RSP     gatt_attr_prep_write_rsp;
    T_BTIF_GATT_ATTR_EXEC_WRITE_RSP     gatt_attr_exec_write_rsp;
    T_BTIF_GATT_ATTR_IND                gatt_attr_ind;
    T_BTIF_GATT_ATTR_NOTIF_INFO         gatt_attr_notif_info;
    T_BTIF_GATT_SECURITY_RSP            gatt_security_rsp;
    T_BTIF_GATT_SERVER_STORE_IND        gatt_server_store_ind;
    T_BTIF_GATT_MTU_SIZE_INFO           gatt_mtu_size_info;

    T_BTIF_LE_CONN_RSP                              le_conn_rsp;
    T_BTIF_LE_CONN_IND                              le_conn_ind;
    T_BTIF_LE_CONN_CMPL_INFO                        le_conn_cmpl_info;
#if F_BT_LE_READ_REMOTE_FEATS
    T_BTIF_LE_REMOTE_FEATS_INFO                     le_remote_feats_info;
#endif
    T_BTIF_LE_DISCONN_RSP                           le_disconn_rsp;
    T_BTIF_LE_DISCONN_IND                           le_disconn_ind;
    T_BTIF_LE_ADV_RSP                               le_adv_rsp;
    T_BTIF_LE_ADV_PARAM_SET_RSP                     le_adv_param_set_rsp;
    T_BTIF_LE_ADV_DATA_SET_RSP                      le_adv_data_set_rsp;
    T_BTIF_LE_SCAN_RSP                              le_scan_rsp;
    T_BTIF_LE_SCAN_PARAM_SET_RSP                    le_scan_param_set_rsp;
    T_BTIF_LE_SCAN_INFO                             le_scan_info;
    T_BTIF_LE_MODIFY_WHITE_LIST_RSP                 le_modify_white_list_rsp;
    T_BTIF_LE_CONN_UPDATE_RSP                       le_conn_update_rsp;
    T_BTIF_LE_CONN_UPDATE_IND                       le_conn_update_ind;
    T_BTIF_LE_CONN_PARAM_INFO                       le_conn_param_info;
#if F_BT_LE_4_2_SUPPORT
#if F_BT_LE_4_2_CONN_PARAM_UPDATE_SUPPORT
    T_BTIF_LE_CONN_PARAM_REQ_EVT_INFO               le_conn_param_req_evt_info;
    T_BTIF_LE_CONN_PARAM_REQ_REPLY_RSP              le_conn_param_req_reply_rsp;
    T_BTIF_LE_CONN_PARAM_REQ_NEG_REPLY_RSP          le_conn_param_req_neg_reply_rsp;
#endif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
    T_BTIF_LE_READ_MAX_DATA_LEN_RSP                 le_read_max_data_len_rsp;
    T_BTIF_LE_READ_DEFAULT_DATA_LEN_RSP             le_read_default_data_len_rsp;
    T_BTIF_LE_WRITE_DEFAULT_DATA_LEN_RSP            le_write_default_data_len_rsp;
    T_BTIF_LE_SET_DATA_LEN_RSP                      le_set_data_len_rsp;
    T_BTIF_LE_DATA_LEN_CHANGE_INFO                  le_data_len_change_info;
#endif
#endif
#if F_BT_LE_4_1_CBC_SUPPORT
    T_BTIF_LE_CREDIT_BASED_CONN_RSP                 le_credit_based_conn_rsp;
    T_BTIF_LE_CREDIT_BASED_CONN_IND                 le_credit_based_conn_ind;
    T_BTIF_LE_CREDIT_BASED_DISCONN_RSP              le_credit_based_disconn_rsp;
    T_BTIF_LE_CREDIT_BASED_DISCONN_IND              le_credit_based_disconn_ind;
    T_BTIF_LE_SEND_CREDIT_RSP                       le_send_credit_rsp;
    T_BTIF_LE_CREDIT_BASED_DATA_RSP                 le_credit_based_data_rsp;
    T_BTIF_LE_CREDIT_BASED_DATA_IND                 le_credit_based_data_ind;
    T_BTIF_LE_CREDIT_BASED_CONN_CMPL_INFO           le_credit_based_conn_cmpl_info;
    T_BTIF_LE_CREDIT_BASED_CONN_CREDIT_INFO         le_credit_based_conn_credit_info;
    T_BTIF_LE_CREDIT_BASED_SECURITY_REG_RSP         le_credit_based_security_reg_rsp;
    T_BTIF_LE_CREDIT_BASED_PSM_REG_RSP              le_credit_based_psm_reg_rsp;
#endif
    T_BTIF_LE_SET_CHANN_CLASSIF_RSP                 le_set_chann_classif_rsp;
    T_BTIF_LE_READ_CHANN_MAP_RSP                    le_read_chann_map_rsp;
#if F_BT_LE_4_0_DTM_SUPPORT
    T_BTIF_LE_RECEIVER_TEST_RSP                     le_receiver_test_rsp;
    T_BTIF_LE_TRANSMITTER_TEST_RSP                  le_transmitter_test_rsp;
    T_BTIF_LE_TEST_END_RSP                          le_test_end_rsp;
#endif
    T_BTIF_LE_READ_ADV_TX_POWER_RSP                 le_read_adv_tx_power_rsp;
    T_BTIF_LE_SET_RAND_ADDR_RSP                     le_set_rand_addr_rsp;
    T_BTIF_LE_CONFIG_LOCAL_IRK_RSP                  le_config_local_irk_rsp;
#if F_BT_LE_PRIVACY_SUPPORT
    T_BTIF_LE_MODIFY_RESOLV_LIST_RSP                le_modify_resolv_list_rsp;
    T_BTIF_LE_READ_PEER_RESOLV_ADDR_RSP             le_read_peer_resolv_addr_rsp;
    T_BTIF_LE_READ_LOCAL_RESOLV_ADDR_RSP            le_read_local_resolv_addr_rsp;
    T_BTIF_LE_SET_RESOLUTION_ENABLE_RSP             le_set_resolution_enable_rsp;
    T_BTIF_LE_SET_RESOLV_PRIV_ADDR_TOUT_RSP         le_set_resolv_priv_addr_tout_rsp;
    T_BTIF_LE_SET_PRIVACY_MODE_RSP                  le_set_privacy_mode_rsp;
    T_BTIF_LE_DIRECT_ADV_INFO                       le_direct_adv_info;
#endif
    T_BTIF_LE_HIGH_DUTY_ADV_TIMEOUT_INFO            le_high_duty_adv_timeout_info;
#if F_BT_LE_5_0_SUPPORT
#if F_BT_LE_5_0_AE_ADV_SUPPORT
    T_BTIF_LE_SET_ADV_SET_RAND_ADDR_RSP             le_set_adv_set_rand_addr_rsp;
    T_BTIF_LE_EXT_ADV_PARAM_SET_RSP                 le_ext_adv_param_set_rsp;
    T_BTIF_LE_EXT_ADV_DATA_SET_RSP                  le_ext_adv_data_set_rsp;
    T_BTIF_LE_EXT_ADV_ENABLE_RSP                    le_ext_adv_enable_rsp;
    T_BTIF_LE_ADV_SET_TERMINATED_INFO               le_adv_set_terminated_info;
    T_BTIF_LE_SCAN_REQ_RECEIVED_INFO                le_scan_req_received_info;
    T_BTIF_LE_ENABLE_EXT_ADV_MODE_RSP               le_enable_ext_adv_mode_rsp;
    T_BTIF_LE_MODIFY_ADV_SET_RSP                    le_modify_adv_set_rsp;
#endif
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
    T_BTIF_LE_EXT_SCAN_PARAM_SET_RSP                le_ext_scan_param_set_rsp;
    T_BTIF_LE_EXT_SCAN_RSP                          le_ext_scan_rsp;
    T_BTIF_LE_EXT_ADV_REPORT_INFO                   le_ext_adv_report_info;
#endif
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
    T_BTIF_LE_SET_DEFAULT_PHY_RSP                   le_set_default_phy_rsp;
    T_BTIF_LE_SET_PHY_RSP                           le_set_phy_rsp;
    T_BTIF_LE_PHY_UPDATE_INFO                       le_phy_update_info;
#endif
#if F_BT_LE_5_0_DTM_SUPPORT
    T_BTIF_LE_ENHANCED_RECEIVER_TEST_RSP            le_enhanced_receiver_test_rsp;
    T_BTIF_LE_ENHANCED_TRANSMITTER_TEST_RSP         le_enhanced_transmitter_test_rsp;
#endif
#if F_BT_LE_5_0_RF_PATH_SUPPORT
    T_BTIF_LE_READ_RF_PATH_COMPENSATION_RSP         le_read_rf_path_compensation_rsp;
    T_BTIF_LE_WRITE_RF_PATH_COMPENSATION_RSP        le_write_rf_path_compensation_rsp;
#endif
#if F_BT_LE_5_0_PERIODIC_ADV_SUPPORT
    T_BTIF_LE_MODIFY_PERIODIC_ADV_LIST_RSP          le_modify_periodic_adv_list_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_PARAM_RSP            le_set_periodic_adv_param_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_DATA_RSP             le_set_periodic_adv_data_rsp;
    T_BTIF_LE_SET_PERIODIC_ADV_ENABLE_RSP           le_set_periodic_adv_enable_rsp;
    T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_RSP          le_periodic_adv_create_sync_rsp;
    T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_RSP   le_periodic_adv_create_sync_cancel_rsp;
    T_BTIF_LE_PERIODIC_ADV_TERMINATE_SYNC_RSP       le_periodic_adv_terminate_sync_rsp;
    T_BTIF_LE_PERIODIC_ADV_SYNC_ESTABLISHED_INFO    le_periodic_adv_sync_established_info;
    T_BTIF_LE_PERIODIC_ADV_REPORT_INFO              le_periodic_adv_report_info;
    T_BTIF_LE_PERIODIC_ADV_SYNC_LOST_INFO           le_periodic_adv_sync_lost_info;
#endif
#endif
    T_BTIF_LE_LOCAL_DEV_INFO                        le_local_dev_info;
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
    T_BTIF_LE_SIGNED_STATUS_INFO                    le_signed_status_info;
#endif
#if F_BT_LE_6_0_AOA_AOD_SUPPORT
    T_BTIF_LE_SET_SUPPL_TEST_PARAMS_RSP             le_set_suppl_test_params_rsp;
#if F_BT_LE_6_0_AOX_CONNLESS_SUPPORT
    T_BTIF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE_RSP   le_set_connless_iq_sampling_enable_rsp;
    T_BTIF_LE_CONNLESS_IQ_REPORT_INFO               le_connless_iq_report_info;
#endif
    T_BTIF_LE_SET_CONN_SUPPL_PARAMS_RSP             le_set_conn_suppl_params_rsp;
    T_BTIF_LE_CONN_SUPPL_REQUEST_ENABLE_RSP         le_conn_suppl_request_enable_rsp;
    T_BTIF_LE_CONN_SUPPL_RESPONSE_ENABLE_RSP        le_conn_suppl_response_enable_rsp;
    T_BTIF_LE_READ_ANTENNA_INFO_RSP                 le_read_antenna_info_rsp;
    T_BTIF_LE_CONN_IQ_REPORT_INFO                   le_conn_iq_report_info;
    T_BTIF_LE_SUPPL_REQUEST_FAILED_INFO             le_suppl_request_failed_info;
#endif

#if F_BT_BREDR_SUPPORT
    T_BTIF_RADIO_MODE_SET_RSP           radio_mode_set_rsp;
    T_BTIF_INQUIRY_RSP                  inquiry_rsp;
    T_BTIF_INQUIRY_RESULT_INFO          inquiry_result_info;
    T_BTIF_INQUIRY_CMPL_INFO            inquiry_cmpl_info;
    T_BTIF_INQUIRY_CANCEL_RSP           inquiry_cancel_rsp;
    T_BTIF_REMOTE_NAME_RSP              remote_name_rsp;
    T_BTIF_SDP_DISCOV_RSP               sdp_discov_rsp;
    T_BTIF_SDP_ATTR_IND                 sdp_attr_ind;
    T_BTIF_DID_ATTR_IND                 did_attr_ind;
    T_BTIF_SDP_DISCOV_CMPL_INFO         sdp_discov_cmpl_info;
    T_BTIF_STOP_SDP_DISCOV_RSP          stop_sdp_discov_rsp;
    T_BTIF_AUTHEN_RSP                   authen_rsp;
    T_BTIF_AUTHOR_REQ_IND               author_req_ind;
    T_BTIF_PIN_CODE_REQ_IND             pin_code_req_ind;
    T_BTIF_LEGACY_REMOTE_OOB_REQ_IND    legacy_remote_oob_req_ind;
    T_BTIF_LOCAL_OOB_RSP                local_oob_rsp;
    T_BTIF_ACL_CFG_RSP                  acl_cfg_rsp;
    T_BTIF_SDP_REG_RSP                  sdp_reg_rsp;
    T_BTIF_L2C_PROTO_REG_RSP            l2c_proto_reg_rsp;
    T_BTIF_L2C_CONN_RSP                 l2c_conn_rsp;
    T_BTIF_L2C_CONN_IND                 l2c_conn_ind;
    T_BTIF_L2C_CONN_CMPL_INFO           l2c_conn_cmpl_info;
    T_BTIF_L2C_DATA_IND                 l2c_data_ind;
    T_BTIF_L2C_DISCONN_RSP              l2c_disconn_rsp;
    T_BTIF_L2C_DISCONN_IND              l2c_disconn_ind;
    T_BTIF_L2C_SET_LOCAL_BUSY_RSP       l2c_set_local_busy_rsp;
    T_BTIF_L2C_SEC_REG_RSP              l2c_sec_reg_rsp;
    T_BTIF_RFC_AUTHEN_RSP               rfc_authen_rsp;
    T_BTIF_SCO_CONN_RSP                 sco_conn_rsp;
    T_BTIF_SCO_CONN_IND                 sco_conn_ind;
    T_BTIF_SCO_CONN_CMPL_INFO           sco_conn_cmpl_info;
    T_BTIF_SCO_DATA_IND                 sco_data_ind;
    T_BTIF_SCO_DISCONN_RSP              sco_disconn_rsp;
    T_BTIF_SCO_DISCONN_IND              sco_disconn_ind;
    T_BTIF_ACL_DISCONN_RSP              acl_disconn_rsp;
    T_BTIF_SETUP_QOS_RSP                setup_qos_rsp;

    T_BTIF_LT_ADDR_SET_RSP              lt_addr_set_rsp;
    T_BTIF_LT_ADDR_DEL_RSP              lt_addr_del_rsp;
    T_BTIF_SYNC_TRAIN_PARAM_SET_RSP     sync_train_param_set_rsp;
    T_BTIF_SYNC_TRAIN_RSP               sync_train_rsp;
    T_BTIF_SYNC_TRAIN_CMPL_INFO         sync_train_cmpl_info;
    T_BTIF_SYNC_TRAIN_RCV_RSP           sync_train_rcv_rsp;
    T_BTIF_SYNC_TRAIN_RCV_CMPL_INFO     sync_train_rcv_cmpl_info;
    T_BTIF_CSB_SET_RSP                  csb_set_rsp;
    T_BTIF_CSB_RCV_SET_RSP              csb_rcv_set_rsp;
    T_BTIF_CSB_TOUT_INFO                csb_tout_info;
    T_BTIF_CSB_DATA_RSP                 csb_data_rsp;
    T_BTIF_CSB_DATA_IND                 csb_data_ind;
#endif
} T_BTIF_UP_MSG_DATA;

/**
 * btif.h
 *
 * \brief    Upstream message from Bluetooth stack to APP. It will be received
 *           in the callback function registered by \ref btif_register_req.
 *
 * \ingroup  BTIF
 */
typedef struct
{
    uint16_t            reserved;   /**< Reserved for future use. */
    uint16_t            command;    /**< Message type defined as \ref T_BTIF_MSG_TYPE. */
    T_BTIF_UP_MSG_DATA  p;          /**< Message data defined as \ref T_BTIF_UP_MSG_DATA. */
} T_BTIF_UP_MSG;

/**
 * btif.h
 *
 * \brief   Prototype of callback function to handle btif messages from Bluetooth
 *          stack to upper layer.
 * \details The callback function should be registered by \ref btif_register_req.
 *          Upstream messages will be handled in this function. Message types are
 *          defined in \ref T_BTIF_UP_MSG. <b>Sending messages to APP task is
 *          highly recommended.</b>\n
 *
 * \ingroup  BTIF
 */
typedef void (* P_BTIF_CALLBACK)(T_BTIF_UP_MSG *p_msg);

/**
 * btif.h
 *
 * \brief    Allocate a buffer from the specified pool in Bluetooth stack. This
 *           function is mainly used when sending LE ACL data to remote side.
 *           \c pool_id parameter should be the same as the one in upstream messages
 *           with message type as \ref BTIF_MSG_LE_LOCAL_DEV_INFO.
 *
 * \param[in]  pool_id   ID of the specified pool to allocate buffer from.
 * \param[in]  size      Pool buffer size in bytes.
 * \param[in]  offset    Offset in bytes. User data will be put start from the offset in the buffer.
 *
 * \return     The address of the allocated pool buffer. If the address is NULL,
 *             the buffer allocation was failed.
 *
 * <b>Example usage</b>
   \code{.c}
   uint16_t ds_data_offset;
   uint16_t ds_pool_id;

   // ...
   // Assign ds_data_offset and ds_pool_id value by btif message.
   // ...

   void test_send_notification(uint8_t *p_data, uint16_t data_len)
   {
       uint16_t length = data_len;

       if (data_len != 0 && p_data != NULL)
       {

           uint8_t *p_buf = NULL;
           uint16_t offset = ds_data_offset + 3;

           p_buf = btif_buffer_get(ds_pool_id, length, offset);
           if (p_buf != NULL)
           {
               memcpy(p_buf + offset, p_data, length);
           }
           else
           {
               return;
           }

           //local_mdl_id, p_service_handle, p_request_handle was get from btif message.
           if (btif_gatt_attr_update_req(p_buf, local_mdl_id, p_service_handle,
                                         p_request_handle, 4, length, offset, 0))
           {

           }
           else
           {
               if (p_buf != NULL)
               {
                   btif_buffer_put(p_buf);
               }
           }
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
void *btif_buffer_get(uint16_t pool_id, uint16_t size, uint16_t offset);

/**
 * btif.h
 *
 * \brief    Free a buffer from Bluetooth stack. The buffer can either be allocated
 *           by APP by \ref btif_buffer_get or allocated by Bluetooth stack and
 *           send to APP by callback function registered by \ref btif_register_req.
 *
 * \param[in]  p_buf     The address of the buffer.
 *
 * \return           The status of the buffer freeing.
 * \retval true      Buffer was freed successfully.
 * \retval false     Buffer was failed to free.
 *
 * <b>Example usage</b>
   \code{.c}
   #define BTIF_MSG_EVENT              0x01

   #define MAX_NUMBER_OF_RX_EVENT      0x10
   #define MAX_NUMBER_OF_MESSAGE       0x10

   void *evt_q_handle;
   void *msg_q_handle;

   // APP btif message callback function.
   void app_btif_msg_cb(T_BTIF_UP_MSG *p_msg)
   {
       unsigned char event = BTIF_MSG_EVENT;

       if (os_msg_send(msg_q_handle, &p_msg, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: Message Queue is full");
           btif_buffer_put(p_msg);
       }
       else if (os_msg_send(evt_q_handle, &event, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: Event Queue is full");
       }
   }

   void app_task(void *p_param)
   {
       char event;

       os_msg_queue_create(&evt_q_handle, MAX_NUMBER_OF_RX_EVENT, sizeof(unsigned char));
       os_msg_queue_create(&msg_q_handle, MAX_NUMBER_OF_MESSAGE, sizeof(T_BTIF_UP_MSG *));

       //Register callback function
       btif_register_req(app_btif_msg_cb);

       while (true)
       {
           if (os_msg_recv(evt_q_handle, &event, 0xFFFFFFFF) == true)
           {
               if (event == BTIF_MSG_EVENT)
               {
                   T_BTIF_UP_MSG *p_msg;

                   if (os_msg_recv(msg_q_handle, &p_msg, 0) == true)
                   {
                       // ...
                       // Handle btif message.
                       // ...

                       btif_buffer_put(p_msg);
                   }
               }
           }
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_buffer_put(void *p_buf);

/**
 * btif.h
 *
 * \brief    Send a request to register a callback function to handle btif
 *           upsteam messages from Bluetooth stack to APP. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_REGISTER_RSP
 *           and data is \ref T_BTIF_REGISTER_RSP will be received in the callback
 *           function. If the function was successfully registered, another message
 *           whose type is \ref BTIF_MSG_ACT_INFO and data is \ref T_BTIF_ACT_INFO
 *           will also be received after that. <b>Sending messages to APP task
 *           in the callback function is highly recommended.</b> Btif message
 *           buffer should be released after the message is <b>truely</b> handled.
 *
 * \param[in]  p_callback     Callback function to handle btif message. The function
 *                            must has prototype as \ref P_BTIF_CALLBACK.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   #define BTIF_MSG_EVENT              0x01

   #define MAX_NUMBER_OF_RX_EVENT      0x10
   #define MAX_NUMBER_OF_MESSAGE       0x10

   void *evt_q_handle;
   void *msg_q_handle;

   // APP btif message callback function.
   void app_btif_msg_cb(T_BTIF_UP_MSG *p_msg)
   {
       unsigned char event = BTIF_MSG_EVENT;

       if (os_msg_send(msg_q_handle, &p_msg, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: message queue is full");
           btif_buffer_put(p_msg);
       }
       else if (os_msg_send(evt_q_handle, &event, 0) == false)
       {
           APP_PRINT_WARN0("app_btif_msg_cb: event queue is full");
       }
   }

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_REGISTER_RSP:
           APP_PRINT_TRACE1("register response cause: 0x%04x", p_msg->p.register_rsp.cause);
           break;

       case BTIF_MSG_ACT_INFO:
           APP_PRINT_TRACE1("local address: %s", TRACE_BDADDR(p_msg->p.act_info.local_addr));
           break;

       default:
           break;
       }
   }

   void app_task(void *p_param)
   {
       char event;

       os_msg_queue_create(&evt_q_handle, MAX_NUMBER_OF_RX_EVENT, sizeof(unsigned char));
       os_msg_queue_create(&msg_q_handle, MAX_NUMBER_OF_MESSAGE, sizeof(T_BTIF_UP_MSG *));

       //Register callback function
       btif_register_req(app_btif_msg_cb);

       while (true)
       {
           if (os_msg_recv(evt_q_handle, &event, 0xFFFFFFFF) == true)
           {
               if (event == BTIF_MSG_EVENT)
               {
                   T_BTIF_UP_MSG *p_msg;

                   if (os_msg_recv(msg_q_handle, &p_msg, 0) == true)
                   {
                       app_handle_btif_msg(p_msg);
                       btif_buffer_put(p_msg);
                   }
               }
           }
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_register_req(P_BTIF_CALLBACK p_callback);

/**
 * btif.h
 *
 * \brief    Send a request to read RSSI of remote device. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_READ_RSSI_RSP
 *           and data is \ref T_BTIF_READ_RSSI_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  bd_addr            Bluetooth address of remote device.
 * \param[in]  remote_addr_type   Address type of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_READ_RSSI_RSP:
           {
               T_BTIF_READ_RSSI_RSP *p_rsp = &p_msg->p.read_rssi_rsp;

               if (!p_rsp->cause)
               {
                   APP_PRINT_TRACE1("RSSI value is %d", p_rsp->rssi);
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

       btif_read_rssi_req(bd_addr, BTIF_REMOTE_ADDR_CLASSIC);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_read_rssi_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type);

/**
 * btif.h
 *
 * \brief    Send a request to set priority for an ACL link. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_ACL_PRIORITY_SET_RSP
 *           and data is \ref T_BTIF_ACL_PRIORITY_SET_RSP will be received in the callback
 *           function registered by \ref btif_register_req. <b>To set priority for
 *           an ACL link, it must be connected and ready to be configured, which
 *           can be sure by receiving \ref BTIF_MSG_ACL_STATUS_INFO message with
 *           ACL status as \ref BTIF_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr                 Bluetooth address of remote device.
 * \param[in]  conn_type               Connection type for the link.
 * \param[in]  is_high_priority        Priority to set.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_READY:
                   btif_acl_priority_set_req(p_info->bd_addr, 0, true);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_PRIORITY_SET_RSP:
           {
               T_BTIF_ACL_PRIORITY_SET_RSP *p_rsp = &p_msg->p.acl_priority_set_rsp;
               APP_PRINT_TRACE1("Configure ACL link priority response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_priority_set_req(uint8_t *bd_addr, uint8_t conn_type, bool is_high_priority);

/**
 * btif.h
 *
 * \brief    Send a request to send vendor command to controller. If the request
 *           was successfully sent, a message whose type is \ref BTIF_MSG_VENDOR_CMD_RSP
 *           and data is \ref T_BTIF_VENDOR_CMD_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  op         Operation code of the command.
 * \param[in]  len        Length of parameters of the command.
 * \param[in]  p_param    Address of parameters of the command.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_VENDOR_CMD_RSP:
           {
               T_BTIF_VENDOR_CMD_RSP *p_rsp = &p_msg->p.vendor_cmd_rsp;

               if (p_rsp->command == 0xFC11)
               {
                   APP_PRINT_TRACE1("vendor command response: cause 0x%04x", p_rsp->cause);
               }

           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_vendor_cmd_req(0xFC11, 0, NULL);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_vendor_cmd_req(uint16_t op, uint8_t len, uint8_t *p_param);

/**
 * btif.h
 *
 * \brief    Send a request to set pairable mode. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_PAIRABLE_MODE_SET_RSP
 *           and data is \ref T_BTIF_PAIRABLE_MODE_SET_RSP will be received in
 *           the callback function registered by \ref btif_register_req.
 *
 * \param[in]  le_enable        Whether to enable le pairing or not.
 * \param[in]  br_enable        Whether to enable br pairing or not.
 * \param[in]  requirements     Authentication requirement of the device. Valid values are the combination of \ref BTIF_AUTHEN_FLAGS.
 * \param[in]  io_caps          Input/Output capabilities. Valid values is \ref T_BTIF_IOCAP.
 * \param[in]  oob_present      Whether OOB data can be used or not.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_PAIRABLE_MODE_SET_RSP:
           {
               T_BTIF_PAIRABLE_MODE_SET_RSP *p_rsp = &p_msg->p.vendor_cmd_rsp;

               APP_PRINT_TRACE1("pairable mode set response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_pairable_mode_set_req(true, true, BTIF_AUTHEN_BONDING | BTIF_AUTHEN_MITM, BTIF_IOCAP_NO_IO, false);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_pairable_mode_set_req(bool le_enable, bool br_enable, uint16_t requirements,
                                T_BTIF_IOCAP io_caps, bool oob_present);

/**
 * btif.h
 *
 * \brief    Send a confirmation for user passkey request indication. The function
 *           should be used when receiving message with type as \ref BTIF_MSG_USER_PASSKEY_REQ_IND
 *           in the callback function registered by \ref btif_register_req. If
 *           confirm with cause as \ref BTIF_CAUSE_ACCEPT, passkey should be input
 *           later using \ref btif_user_passkey_req_reply_req.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  cause                Confirm result of user passkey request indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_USER_PASSKEY_REQ_IND:
           {
               T_BTIF_USER_PASSKEY_REQ_IND *p_ind = &p_msg->p.user_passkey_req_ind;

               btif_user_passkey_req_cfm(p_ind->bd_addr, p_ind->remote_addr_type, BTIF_CAUSE_ACCEPT);
               btif_user_passkey_req_reply_req(p_ind->bd_addr, p_ind->remote_addr_type, 888888, BTIF_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_user_passkey_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                               T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a request to input user passkey. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_USER_PASSKEY_REQ_REPLY_RSP
 *           and data is \ref T_BTIF_USER_PASSKEY_REQ_REPLY_RSP will be received in
 *           the callback function registered by \ref btif_register_req.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  passkey              User passkey. Only valid when \c cause is \ref BTIF_CAUSE_ACCEPT.
 * \param[in]  cause                Status of input user passkey. \ref BTIF_CAUSE_ACCEPT to input passkey, otherwise reject.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_USER_PASSKEY_REQ_IND:
           {
               T_BTIF_USER_PASSKEY_REQ_IND *p_ind = &p_msg->p.user_passkey_req_ind;

               btif_user_passkey_req_cfm(p_ind->bd_addr, p_ind->remote_addr_type, BTIF_CAUSE_ACCEPT);
               btif_user_passkey_req_reply_req(p_ind->bd_addr, p_ind->remote_addr_type, 888888, BTIF_CAUSE_ACCEPT);
           }
           break;

       case BTIF_MSG_USER_PASSKEY_REQ_REPLY_RSP:
           {
               T_BTIF_USER_PASSKEY_REQ_REPLY_RSP *p_rsp = &p_msg->p.user_passkey_req_reply_rsp;
               APP_PRINT_TRACE1("input user passkey result 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_user_passkey_req_reply_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                     uint32_t passkey, T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a confirmation for authentication result indication. The function
 *           should be used when receiving message with type as \ref BTIF_MSG_AUTHEN_RESULT_IND
 *           in the callback function registered by \ref btif_register_req. User
 *           should save authetication information or delete old one according to
 *           key type in the indication message.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  cause                Confirm result of authentication result indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_AUTHEN_RESULT_IND:
           {
               T_BTIF_AUTHEN_RESULT_IND *p_ind = &p_msg->p.authen_result_ind;

               if (p_ind->key_type == BTIF_KEY_DELETE)
               {
                   //Delete link key saved before.
               }
               else
               {
                   //Save link key information.
               }

               btif_authen_result_cfm(p_ind->bd_addr, p_ind->remote_addr_type, BTIF_CAUSE_SUCCESS);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_authen_result_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                            T_BTIF_KEY_TYPE key_type, T_BTIF_CAUSE cause);


/**
 * btif.h
 *
 * \brief    Send a confirmation for authentication key request indication.
 *           The function should be used when receiving message with type as
 *           \ref BTIF_MSG_AUTHEN_KEY_REQ_IND in the callback function registered
 *           by \ref btif_register_req. After receiving the indication, user should
 *           try to find key information and then use this function to confirm.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  key_len              Length of the authenticaion key found.
 * \param[in]  p_key                Address of the authenticaion key found.
 * \param[in]  key_type             Type of the authenticaion key found.
 * \param[in]  cause                Confirmation status for authentication key request indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_AUTHEN_KEY_REQ_IND:
           {
               T_BTIF_AUTHEN_KEY_REQ_IND *p_ind = &p_msg->p.authen_key_req_ind;

               if (p_ind->remote_addr_type == BTIF_REMOTE_ADDR_CLASSIC)
               {
                   if (p_ind->key_type == BTIF_KEY_REQ_BR)
                   {
                       uint8_t link_key[16];
                       uint8_t key_type;

                       //Find BR/EDR link key for the address.
                       if (legacy_get_bond(p_ind->bd_addr, link_key, &key_type) == true)
                       {
                           btif_authen_key_req_cfm(p_ind->bd_addr, p_ind->remote_addr_type, 16, link_key,
                                                   (T_BTIF_KEY_TYPE)key_type, BTIF_CAUSE_ACCEPT);
                       }
                       else
                       {
                           btif_authen_key_req_cfm(p_ind->bd_addr, p_ind->remote_addr_type, 0, NULL,
                                                   p_ind->key_type, BTIF_CAUSE_REJECT);
                       }
                   }
               }
               else
               {
                   //Handle indication for LE key request.
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_authen_key_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                             uint8_t key_len, uint8_t *p_key, T_BTIF_KEY_TYPE key_type,
                             T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a confirmation for user confirm request indication. The
 *           function should be used when receiving message with type as \ref
 *           BTIF_MSG_USER_CFM_REQ_IND and specific numeric value in message
 *           data in the callback function registered by \ref btif_register_req.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  cause                Confirmation status for user confirm request indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_USER_CFM_REQ_IND:
           {
               T_BTIF_USER_CFM_REQ_IND *p_ind = &p_msg->p.user_cfm_req_ind;

               btif_user_cfm_req_cfm(p_ind->bd_addr, p_ind->remote_addr_type, BTIF_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_user_cfm_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                           T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a request to send keypress event to remote device. If the
 *           request was successfully sent, a message whose type is \ref
 *           BTIF_MSG_KEYPRESS_NOTIF_RSP and data is \ref T_BTIF_KEYPRESS_NOTIF_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  remote_addr_type     Address type of remote device.
 * \param[in]  event_type           Keypress event type.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_KEYPRESS_NOTIF_RSP:
           {
               T_BTIF_KEYPRESS_NOTIF_RSP *p_rsp = &p_msg->p.keypress_notif_rsp;
               APP_PRINT_TRACE1("keypress response cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

       btif_keypress_notif_req(bd_addr, BTIF_REMOTE_ADDR_LE_PUBLIC, BTIF_PASSKEY_ENTRY_STARTED);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_keypress_notif_req(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                             T_BTIF_KEYPRESS_NOTIF_TYPE event_type);

bool btif_just_work_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                            T_BTIF_CAUSE cause);

bool btif_user_passkey_notif_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                 T_BTIF_CAUSE cause);

bool btif_set_link_key_convert(bool is_support);

bool btif_user_defined(void *p_buf);

T_BTIF_IDENT_ADDR_TYPE btif_convert_addr_type(T_BTIF_REMOTE_ADDR_TYPE type);

bool btif_le_cfg_passkey_value(uint32_t value);

void btif_le_cfg_slave_init_mtu(bool enable);

void btif_le_cfg_local_key_distribute(uint8_t init_dist, uint8_t rsp_dist);

bool btif_le_cfg_local_identity_address(uint8_t *addr, T_BTIF_IDENT_ADDR_TYPE type);

bool btif_le_conn_req(bool use_extended_conn, uint8_t bd_addr[6],
                      T_BTIF_REMOTE_ADDR_TYPE remote_addr_type, T_BTIF_LOCAL_ADDR_TYPE local_addr_type,
                      uint8_t init_phys, uint16_t scan_timeout, T_BTIF_LE_CONN_REQ_PARAM *p_conn_req_param);

bool btif_le_conn_cfm(uint8_t *bd_addr, T_BTIF_CAUSE cause);

bool btif_le_disconn_req(uint16_t link_id, T_BTIF_CAUSE cause);

bool btif_le_disconn_cfm(uint16_t link_id);

bool btif_gatt_srv_reg_req(uint16_t num_attr, void *p_srv);

bool btif_gatt_srv2_reg_req(uint16_t num_attr, void *p_srv, uint16_t start_handle);

bool btif_gatt_attr_update_req(void *p_buf, uint16_t link_id, void *p_srv_handle,
                               void *p_req_handle, uint16_t attr_index, uint16_t attr_len,
                               uint16_t offset, T_BTIF_GATT_PDU_TYPE type);

bool btif_gatt_attr_update_status_cfm(uint16_t link_id, void *p_srv_handle,
                                      void *p_req_handle, uint16_t attr_index);

bool btif_gatt_attr_read_cfm(void *p_buf, uint16_t link_id, void *p_srv_handle, uint16_t cause,
                             uint16_t attr_index, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_write_req_cfm(uint16_t link_id, void *p_srv_handle,
                                  uint16_t cause, uint16_t attr_index);

bool btif_gatt_attr_prep_write_cfm(void *p_buf, uint16_t link_id, void *p_srv_handle,
                                   uint16_t cause, uint16_t attr_index, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_exec_write_cfm(uint16_t link_id, uint16_t cause, uint16_t handle);

bool btif_gatt_attr_prep_write_req(void *p_buf, uint16_t link_id, uint16_t attr_handle,
                                   uint16_t attr_len, uint16_t write_offset, uint16_t offset);

bool btif_gatt_attr_exec_write_req(uint16_t link_id, uint8_t flags);

bool btif_gatt_discovery_req(uint16_t link_id, T_BTIF_GATT_DISCOVERY_TYPE type,
                             uint16_t start_handle, uint16_t end_handle, uint16_t uuid16, uint8_t *p_uuid128);

bool btif_gatt_discovery_cfm(uint16_t link_id, T_BTIF_GATT_DISCOVERY_TYPE type,
                             uint16_t start_handle, uint16_t end_handle);

bool btif_gatt_attr_read_req(uint16_t link_id, T_BTIF_GATT_READ_TYPE read_type,
                             uint16_t read_offset, uint16_t start_handle, uint16_t end_handle,
                             uint16_t uuid16, uint8_t *p_uuid128);

bool btif_gatt_attr_read_multi_req(uint16_t link_id, uint16_t num_handle, uint16_t *p_handle);

bool btif_gatt_attr_write_req(void *p_buf, uint16_t link_id, T_BTIF_GATT_WRITE_TYPE write_type,
                              uint16_t attr_handle, uint16_t attr_len, uint16_t offset);

bool btif_gatt_attr_cfm(uint16_t link_id);

bool btif_gatt_security_req(uint16_t link_id, uint16_t requirements, uint8_t min_key_size);

bool btif_gatt_server_store_cfm(T_BTIF_GATT_STORE_OPCODE op, uint8_t *bd_addr,
                                T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
                                uint8_t data_len, uint8_t *data, T_BTIF_CAUSE cause);

#if F_BT_LE_SMP_OOB_SUPPORT
bool btif_remote_oob_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE addr_type,
                             uint8_t *p_data_c, T_BTIF_CAUSE cause);
#endif

bool btif_le_adv_req(T_BTIF_LE_ADV_MODE adv_mode);

bool btif_le_adv_param_set_req(T_BTIF_LE_ADV_TYPE adv_type,
                               T_BTIF_LE_ADV_FILTER_POLICY filter_policy,
                               uint16_t min_interval, uint16_t max_interval,
                               T_BTIF_LOCAL_ADDR_TYPE local_addr_type, uint8_t *bd_addr,
                               T_BTIF_REMOTE_ADDR_TYPE remote_addr_type, uint8_t chann_map);

bool btif_le_adv_data_set_req(T_BTIF_LE_ADV_DATA_TYPE data_type, uint8_t data_len, uint8_t *p_data);

bool btif_le_scan_req(T_BTIF_LE_SCAN_MODE mode, T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates);

bool btif_le_scan_param_set_req(T_BTIF_LE_SCAN_TYPE type, uint16_t interval, uint16_t window,
                                T_BTIF_LE_SCAN_FILTER_POLICY filter_policy, T_BTIF_LOCAL_ADDR_TYPE local_addr_type);

bool btif_le_ext_scan_req(T_BTIF_LE_SCAN_MODE mode,
                          T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates,
                          uint16_t duration, uint16_t period);

bool btif_le_ext_scan_param_set_req(T_BTIF_LOCAL_ADDR_TYPE local_addr_type,
                                    T_BTIF_LE_SCAN_FILTER_POLICY filter_policy,
                                    uint8_t scan_phys,
                                    T_BTIF_LE_EXT_SCAN_PARAM *p_extended_scan_param);

bool btif_le_modify_adv_set_req(T_BTIF_LE_ADV_SET_OP op, uint8_t adv_handle);

bool btif_le_modify_white_list_req(T_BTIF_LE_WHITE_LIST_OP op, uint8_t *bd_addr,
                                   T_BTIF_REMOTE_ADDR_TYPE remote_addr_type);

bool btif_le_conn_update_req(uint16_t link_id, uint16_t min_interval, uint16_t max_interval,
                             uint16_t conn_latency, uint16_t supv_tout, uint16_t min_ce_len,
                             uint16_t max_ce_len);

bool btif_le_conn_update_cfm(uint16_t link_id, T_BTIF_CAUSE cause);

bool btif_le_set_rand_addr_req(uint8_t *random_addr);

bool btif_le_enable_ext_adv_mode_req(void);

bool btif_le_set_adv_set_rand_addr_req(uint8_t *random_addr, uint8_t adv_handle);

bool btif_le_credit_based_conn_req(uint16_t link_id, uint16_t le_psm, uint16_t mtu,
                                   uint16_t initial_credits, uint16_t credits_threshold);

bool btif_le_credit_based_conn_cfm(uint16_t link_id, uint16_t channel, uint16_t mtu,
                                   uint16_t initial_credits, uint16_t credits_threshold,
                                   T_BTIF_L2C_LE_CONN_STATUS cause);

bool btif_le_credit_based_disconn_req(uint16_t link_id, uint16_t channel);

bool btif_le_credit_based_disconn_cfm(uint16_t link_id, uint16_t channel);

bool btif_le_send_credit_req(uint16_t link_id, uint16_t channel, uint16_t credits);

bool btif_le_credit_based_data_req(void *p_buf, uint16_t link_id, uint16_t channel,
                                   uint16_t length, uint16_t offset);

bool btif_le_credit_based_data_cfm(uint16_t link_id, uint16_t channel, T_BTIF_CAUSE cause);

bool btif_le_credit_based_security_reg_req(uint16_t le_psm, bool active,
                                           T_BTIF_LE_SECURITY_MODE mode, uint8_t key_size);

bool btif_le_credit_based_psm_reg_req(uint16_t le_psm, uint8_t action);

bool btif_le_set_data_len_req(uint16_t link_id, uint16_t tx_oct, uint16_t tx_time);

bool btif_le_conn_param_req_reply_req(uint16_t link_id, uint16_t min_interval,
                                      uint16_t max_interval, uint16_t latency, uint16_t supv_tout,
                                      uint16_t min_ce_len, uint16_t max_ce_len);

bool btif_le_conn_param_req_neg_reply_req(uint16_t link_id, uint8_t reason);

bool btif_le_config_local_irk_req(uint8_t *p_local_irk);

bool btif_le_gen_rand_addr(T_BTIF_ADDR_RAND rand_addr_type, uint8_t *p_addr);

bool btif_le_modify_resolv_list_req(T_BTIF_LE_RESOLVING_LIST_OP operation,
                                    T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type, uint8_t *peer_ident_addr,
                                    uint8_t *peer_irk, uint8_t *local_irk);

bool btif_le_read_peer_resolv_addr_req(T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type,
                                       uint8_t *peer_ident_addr);

bool btif_le_read_local_resolv_addr_req(T_BTIF_IDENT_ADDR_TYPE peer_ident_addr_type,
                                        uint8_t *peer_ident_addr);

bool btif_le_set_resolution_enable_req(uint8_t enable);

bool btif_le_set_resolv_priv_addr_tout_req(uint16_t timeout);

bool btif_le_set_privacy_mode_req(T_BTIF_IDENT_ADDR_TYPE peer_identity_address_type,
                                  uint8_t *peer_identity_address, T_BTIF_LE_PRIVACY_MODE privacy_mode);

bool btif_le_set_chann_classif_req(uint8_t *p_chann_map);

bool btif_le_read_chann_map_req(uint16_t link_id);

bool btif_le_receiver_test_req(uint8_t rx_chann);

bool btif_le_transmitter_test_req(uint8_t tx_chann, uint8_t data_len, uint8_t pkt_pl);

bool btif_le_test_end_req(void);

bool btif_le_read_adv_tx_power_req(void);

bool btif_le_set_default_phy_req(uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys);

bool btif_le_set_phy_req(uint16_t link_id, uint8_t all_phys, uint8_t tx_phys,
                         uint8_t rx_phys, T_BTIF_LE_PHY_OPTIONS phy_options);

bool btif_le_ext_adv_enable_req(T_BTIF_LE_EXT_ADV_MODE adv_mode, uint8_t num_of_sets,
                                T_BTIF_LE_EXT_ADV_SET_PARAM *p_adv_set_param);

bool btif_le_ext_adv_param_set_req(uint8_t adv_handle, uint16_t adv_event_prop,
                                   uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                   uint8_t primary_adv_channel_map, T_BTIF_LOCAL_ADDR_TYPE own_address_type,
                                   T_BTIF_REMOTE_ADDR_TYPE peer_address_type, uint8_t *p_peer_address,
                                   T_BTIF_LE_ADV_FILTER_POLICY filter_policy, uint8_t tx_power,
                                   T_BTIF_LE_PRIM_ADV_PHY_TYPE primary_adv_phy, uint8_t secondary_adv_max_skip,
                                   T_BTIF_LE_PHY_TYPE secondary_adv_phy, uint8_t adv_sid,
                                   T_BTIF_LE_SCAN_REQ_NOTIFY_TYPE scan_req_notification_enable);

bool btif_le_ext_adv_data_set_req(T_BTIF_LE_EXT_ADV_DATA_TYPE data_type,
                                  uint8_t adv_handle, T_BTIF_LE_ADV_FRAG_OP_TYPE op,
                                  T_BTIF_LE_ADV_FRAG_PREFERENCE_TYPE frag_preference,
                                  uint8_t data_len, uint8_t *p_data);

bool btif_le_enhanced_receiver_test_req(uint8_t rx_chann, uint8_t phy, uint8_t modulation_index);

bool btif_le_enhanced_transmitter_test_req(uint8_t tx_chann, uint8_t data_len,
                                           uint8_t pkt_pl, uint8_t phy);

bool btif_le_modify_periodic_adv_list_req(T_BTIF_LE_PERIODIC_ADV_LIST_OP op, uint8_t adv_addr_type,
                                          uint8_t *adv_addr, uint8_t adv_sid);

bool btif_le_read_rf_path_compensation_req(void);

bool btif_le_write_rf_path_compensation_req(int8_t rf_tx_path_compensation,
                                            int8_t rf_rx_path_compensation);

bool btif_le_set_periodic_adv_param_req(uint8_t adv_handle, uint16_t adv_interval_min,
                                        uint16_t adv_interval_max, uint16_t adv_prop);

bool btif_le_set_periodic_adv_data_req(uint8_t adv_handle, T_BTIF_LE_ADV_FRAG_OP_TYPE op,
                                       uint8_t data_len, uint8_t *p_data);

bool btif_le_set_periodic_adv_enable_req(T_BTIF_LE_PERIODIC_ADV_MODE mode, uint8_t adv_handle);

bool btif_le_periodic_adv_create_sync_req(uint8_t filter_policy, uint8_t adv_sid,
                                          uint8_t adv_addr_type, uint8_t adv_add[6], uint16_t skip,
                                          uint16_t sync_timeout, uint16_t unused);

bool btif_le_periodic_adv_create_sync_cancel_req(void);

bool btif_le_periodic_adv_terminate_sync_req(uint16_t sync_handle);

bool btif_remote_oob_req_cfm(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE addr_type,
                             uint8_t *p_data_c, T_BTIF_CAUSE cause);

bool btif_le_sc_local_oob_init(uint64_t *p_ecc_rand_in, uint8_t *local_rand,
                               T_BTIF_LOCAL_OOB_DATA *p_local_oob_data);

bool btif_le_sc_peer_oob_init(T_BTIF_PEER_OOB_DATA *p_peer_oob_data);

#if F_BT_LE_6_0_AOA_AOD_SUPPORT
bool btif_le_set_suppl_test_params_req(bool is_receiver, uint8_t suppl_len, uint8_t suppl_slot_type,
                                       uint8_t num_antena_ids, uint8_t antenna_switching_pattern);
#if F_BT_LE_6_0_AOX_CONNLESS_SUPPORT
bool btif_le_set_connless_suppl_transmit_params_req(uint8_t adv_handle, uint8_t suppl_type,
                                                    uint8_t suppl_len,
                                                    uint8_t num_antena_ids, uint8_t *p_antenna_id_list);
bool btif_le_set_connless_suppl_transmit_enable_req(uint8_t adv_handle, uint8_t enable);
bool btif_le_set_connless_iq_sample_enable_req(uint16_t sync_handle, uint8_t enable,
                                               uint8_t slot_duration,
                                               uint8_t num_antena_ids, uint8_t *p_antenna_id_list);
#endif
bool btif_le_set_conn_suppl_params_req(bool is_receiver, uint16_t link_id, uint8_t slot_duration,
                                       uint8_t num_antena_ids, uint8_t *p_antenna_id_list);

bool btif_le_conn_suppl_request_enable_req(uint16_t link_id, uint8_t enable,
                                           uint16_t suppl_interval,
                                           uint8_t suppl_len, uint8_t suppl_type);
bool btif_le_conn_suppl_response_enable_req(uint16_t link_id, uint8_t enable);
bool btif_le_read_antenna_info_req(void);
#endif

/**
 * btif.h
 *
 * \brief    Send a request to configure name and class of device of local
 *           device. If the request was successfully sent, a message whose type
 *           is \ref BTIF_MSG_DEV_CFG_RSP and data is \ref T_BTIF_DEV_CFG_RSP
 *           will be received in the callback function registered by \ref
 *           btif_register_req. The operation code in \ref T_BTIF_DEV_CFG_RSP
 *           is \ref BTIF_DEV_CFG_OP_NAME.
 *
 * \param[in]  cod          Class of device to be configured.
 * \param[in]  p_name       Address of name to be configured. The length of name should not be larger than \ref BTIF_NAME_LENGTH.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_NAME:
                   APP_PRINT_TRACE1("Configure local name result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_dev_cfg_name_req(0x000900, "New Name");
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_cfg_name_req(uint32_t cod, uint8_t *p_name);

/**
 * btif.h
 *
 * \brief    Send a request to configure DID information in EIR data. If the
 *           request was successfully sent, a message whose type is \ref
 *           BTIF_MSG_DEV_CFG_RSP and data is \ref T_BTIF_DEV_CFG_RSP will be
 *           received in the callback function registered by \ref btif_register_req.
 *           The operation code in \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_DID_EIR.
 *
 * \param[in]  vendor_id           Vendor ID.
 * \param[in]  vendor_id_src       Vendor ID source.
 * \param[in]  product_id          Product ID.
 * \param[in]  product_version     Product version.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_DID_EIR:
                   APP_PRINT_TRACE1("Configure DID information result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_dev_did_eir_cfg_req(0x005D, 0x0001, 0x223B, 0x0100);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_did_eir_cfg_req(uint16_t vendor_id, uint16_t vendor_id_src,
                              uint16_t product_id, uint16_t product_version);

/**
 * btif.h
 *
 * \brief    Send a request to configure extra information other than local
 *           name, service UUID and DID in EIR data. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_DEV_CFG_RSP and data is
 *           \ref T_BTIF_DEV_CFG_RSP will be received in the callback function
 *           registered by \ref btif_register_req. The operation code in \ref
 *           T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_EXT_EIR.
 *
 * \param[in]  p_data           Address of extra EIR data.<b>The buffer should not be freed until receiving the response message.</b>
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t *p_ext = NULL;

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_EXT_EIR:
                   APP_PRINT_TRACE1("Configure extra eir data result 0x%04x", p_rsp->cause);
                   os_mem_free(p_ext);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       p_ext = os_mem_zalloc(30);
       if (p_ext != NULL)
       {
           // ...
           // Fill in extra EIR data.
           // ...
           btif_dev_ext_eir_cfg_req(p_ext);
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_ext_eir_cfg_req(uint8_t *p_data);

/**
 * btif.h
 *
 * \brief    Send a request to configure page scan parameters and page timeout
 *           value. If the request was successfully sent, a message whose type is
 *           \ref BTIF_MSG_DEV_CFG_RSP and data is \ref T_BTIF_DEV_CFG_RSP will be
 *           received in the callback function registered by \ref btif_register_req.
 *           The operation code in \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_PAGE_SCAN.
 *
 * \param[in]  scan_type           Page scan type.
 * \param[in]  interval            Page scan interval. Only even values from 0x12 to 0x1000 are valid.
 * \param[in]  window              Page scan window. Only values from 0x11 to 0x1000 are valid. Should be less than or equal to interval.
 * \param[in]  page_tout           Page timeout value.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_PAGE_SCAN:
                   APP_PRINT_TRACE1("Configure page scan parameters result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_page_scan_cfg_req(BTIF_BR_PAGE_SCAN_STANDARD, 0x800, 0x12, 0x2000);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_page_scan_cfg_req(T_BTIF_BR_PAGE_SCAN_TYPE scan_type,
                                uint16_t interval, uint16_t window, uint16_t page_tout);

/**
 * btif.h
 *
 * \brief    Send a request to configure inquiry scan parameters. If the request
 *           was successfully sent, a message whose type is \ref BTIF_MSG_DEV_CFG_RSP
 *           and data is \ref T_BTIF_DEV_CFG_RSP will be received in the callback
 *           function registered by \ref btif_register_req. The operation code in
 *           \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_INQUIRY_SCAN.
 *
 * \param[in]  scan_type           Inquiry scan type.
 * \param[in]  interval            Inquiry scan interval. Only even values from 0x12 to 0x1000 are valid.
 * \param[in]  window              Inquiry scan window. Only values from 0x11 to 0x1000 are valid. Should be less than or equal to interval.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_INQUIRY_SCAN:
                   APP_PRINT_TRACE1("Configure inquiry scan parameters result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_inquiry_scan_cfg_req(BTIF_BR_INQUIRY_SCAN_STANDARD, 0x1000, 0x12);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_inquiry_scan_cfg_req(T_BTIF_BR_INQUIRY_SCAN_TYPE scan_type,
                                   uint16_t interval, uint16_t window);

/**
 * btif.h
 *
 * \brief    Send a request to configure inquiry mode. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_DEV_CFG_RSP
 *           and data is \ref T_BTIF_DEV_CFG_RSP will be received in the callback
 *           function registered by \ref btif_register_req. The operation code in
 *           \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_INQUIRY_MODE.
 *
 * \param[in]  mode           Inquiry mode.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_INQUIRY_MODE:
                   APP_PRINT_TRACE1("Configure inquiry mode result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_inquiry_mode_cfg_req(BTIF_BR_EXTENDED_INQUIRY_RESULT);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_inquiry_mode_cfg_req(T_BTIF_BR_INQUIRY_MODE mode);

/**
 * btif.h
 *
 * \brief    Send a request to configure default link policy to use after ACL
 *           connection is established and default page timeout value if local is
 *           master. If the request was successfully sent, a message whose type
 *           is \ref BTIF_MSG_DEV_CFG_RSP and data is \ref T_BTIF_DEV_CFG_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *           The operation code in \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_LINK_POLICY.
 *
 * \param[in]  policy              Default link policy to use after ACL link established. Valid values are combination of \ref BTIF_BR_LINK_POLICY.
 * \param[in]  supv_tout           Default supervision timeout value to use if local is master.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_LINK_POLICY:
                   APP_PRINT_TRACE1("Configure link policy result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_link_policy_cfg_req(BTIF_BR_LINK_POLICY_ENABLE_ROLE_SWITCH | BTIF_BR_LINK_POLICY_ENABLE_SNIFF_MODE, 0x7D00);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_link_policy_cfg_req(T_BTIF_BR_LINK_POLICY policy, uint16_t supv_tout);

/**
 * btif.h
 *
 * \brief    Send a request to configure Bluetooth mode. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_DEV_CFG_RSP
 *           and data is \ref T_BTIF_DEV_CFG_RSP will be received in the
 *           callback function registered by \ref btif_register_req. The operation
 *           code in \ref T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_BT_MODE.
 *           Default value of Bluetooth mode is \ref BTIF_BT_MODE21_DISABLED.
 *           <b>Bluetooth mode can only be configured from \ref BTIF_BT_MODE21_DISABLED
 *           to \ref BTIF_BT_MODE21_ENABLED, not the other way around.</b>
 *
 * \param[in]  mode           Bluetooth mode.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_BT_MODE:
                   APP_PRINT_TRACE1("Configure Bluetooth mode result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_bt_mode_cfg_req(BTIF_BT_MODE21_ENABLED);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_bt_mode_cfg_req(T_BTIF_BT_MODE mode);

/**
 * btif.h
 *
 * \brief    Send a request to configure accept role to use when accepting
 *           connection request from remote device. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_DEV_CFG_RSP and data is
 *           \ref T_BTIF_DEV_CFG_RSP will be received in the callback function
 *           registered by \ref btif_register_req. The operation code in \ref
 *           T_BTIF_DEV_CFG_RSP is \ref BTIF_DEV_CFG_OP_ACCEPT_CONN_ROLE.
 *
 * \param[in]  role              Accept role to use.
 * \arg \c     0             Become the Master for this connection.
 * \arg \c     1             Remain the Slave for this connection. This is the default value.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_DEV_CFG_RSP:
           {
               T_BTIF_DEV_CFG_RSP *p_rsp = &p_msg->p.dev_cfg_rsp;

               switch (p_rsp->op)
               {
               case BTIF_DEV_CFG_OP_ACCEPT_CONN_ROLE:
                   APP_PRINT_TRACE1("Configure accept role result 0x%04x", p_rsp->cause);
                   break;

               default:
                   break;
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_dev_accept_role_cfg_req(0);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_dev_accept_role_cfg_req(uint8_t role);

/**
 * btif.h
 *
 * \brief    Send a request to set radio mode of local device. If the request
 *           was successfully sent, a message whose type is \ref BTIF_MSG_RADIO_MODE_SET_RSP
 *           and data is \ref T_BTIF_RADIO_MODE_SET_RSP will be received in the
 *           callback function registered by \ref btif_register_req.
 *
 * \param[in]  mode              Radio mode of local device.
 * \param[in]  limited           Whether enable limited discoverable mode. Only valid when inquiry scan was enabled in mode.
 * \param[in]  limited_time      Limited discoverable timeout value. Only valid when limited discoverable was enabled.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_RADIO_MODE_SET_RSP:
           {
               T_BTIF_RADIO_MODE_SET_RSP *p_rsp = &p_msg->p.radio_mode_set_rsp;
               APP_PRINT_TRACE1("Set radio mode response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...
       btif_radio_mode_set_req(BTIF_RADIO_VISIBLE_CONNECTABLE, false, 0);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_radio_mode_set_req(T_BTIF_RADIO_MODE mode, bool limited, uint8_t limited_time);

/**
 * btif.h
 *
 * \brief    Send a request to start BR/EDR inquiry. If the request was successfully
 *           sent, multiple messages will be received in the callback function
 *           registered by \ref btif_register_req. The first one has message type
 *           as \ref BTIF_MSG_INQUIRY_RSP and data as \ref T_BTIF_INQUIRY_RSP,
 *           this message indicates whether inquiry was started successfully. If
 *           inquiry was started successfully, one or more messages whose type is
 *           \ref BTIF_MSG_INQUIRY_RESULT_INFO and data is \ref T_BTIF_INQUIRY_RESULT_INFO
 *           may be received, each message represents a remote device information.
 *           After inquiry procedure is done, a message whose type is \ref BTIF_MSG_INQUIRY_CMPL_INFO
 *           and data is \ref T_BTIF_INQUIRY_CMPL_INFO will be received.
 *
 * \param[in]  limited_inquiry    Start a limited inquiry or not.
 * \arg \c     true               Start a limited inquiry.
 * \arg \c     false              Start a general inquiry.
 * \param[in]  timeout            Inquiry duration in 1.28s. The real inquiry time is timeout*1.28s.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_INQUIRY_RSP:
           {
               T_BTIF_INQUIRY_RSP *p_rsp = &p_msg->p.inquiry_rsp;
               APP_PRINT_TRACE1("Inquiry response: status 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_INQUIRY_RESULT_INFO:
           {
               T_BTIF_INQUIRY_RESULT_INFO *p_info = &p_msg->p.inquiry_result_info;
               APP_PRINT_TRACE2("Inqury result: remote addr %s, name %s",
                                TRACE_BDADDR(p_info->bd_addr), TRACE_STRING(p_info->name));
           }
           break;

       case BTIF_MSG_INQUIRY_CMPL_INFO:
           {
               T_BTIF_INQUIRY_CMPL_INFO *p_info = &p_msg->p.inquiry_cmpl_info;
               APP_PRINT_TRACE1("Inquiry complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_inquiry_req(false, 8);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_inquiry_req(bool limited_inquiry, uint8_t timeout);

/**
 * btif.h
 *
 * \brief    Send a request to stop BR/EDR inquiry. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_INQUIRY_CANCEL_RSP and
 *           data is \ref T_BTIF_INQUIRY_CANCEL_RSP will be received in the callback
 *           function registered by \ref btif_register_req to indicates whether
 *           inquiry was stoped successfully. If inquiry was successfully stopped,
 *           no \ref BTIF_MSG_INQUIRY_CMPL_INFO will be received.
 *
 * \param[in]  None.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_INQUIRY_RSP:
           {
               T_BTIF_INQUIRY_RSP *p_rsp = &p_msg->p.inquiry_rsp;

               if (!p_rsp->cause)
               {
                   btif_inquiry_cancel_req;
               }
           }
           break;

       case BTIF_MSG_INQUIRY_CANCEL_RSP:
           {
               T_BTIF_INQUIRY_CANCEL_RSP *p_rsp = &p_msg->p.inquiry_cancel_rsp;
               APP_PRINT_TRACE1("Stop inquiry response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_inquiry_req(false, 8);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_inquiry_cancel_req(void);

/**
 * btif.h
 *
 * \brief    Send a request to get remote device's name. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_REMOTE_NAME_RSP and
 *           data is \ref T_BTIF_REMOTE_NAME_RSP will be received in the callback
 *           function registered by \ref btif_register_req to indicate the result
 *           of the procedure. If success, remote name will also be contained in
 *           the message data.
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_REMOTE_NAME_RSP:
           {
               T_BTIF_REMOTE_NAME_RSP *p_rsp = &p_msg->p.remote_name_rsp;

               if (!p_rsp->cause)
               {
                   APP_PRINT_TRACE1("Get remote name response: name is %s",
                                    TRACE_STRING(p_rsp->name));
               }
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       btif_remote_name_req(bd_addr);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_remote_name_req(uint8_t *bd_addr);

/**
 * btif.h
 *
 * \brief    Send a request to start SDP discovery. If the request was successfully
 *           sent, multiple messages will be received in the callback function
 *           registered by \ref btif_register_req. The first one has message type
 *           as \ref BTIF_MSG_SDP_DISCOV_RSP and data as \ref T_BTIF_SDP_DISCOV_RSP
 *           which indicates whether SDP discovery was started successfully. If
 *           SDP discovery was started successfully and discovery type is \ref
 *           BTIF_SDP_DISCOV_TYPE_BR, one or more messages whos type is \ref
 *           BTIF_MSG_SDP_ATTR_IND and data is \ref T_BTIF_SDP_ATTR_IND may be
 *           received. If discovery type is \ref BTIF_SDP_DISCOV_TYPE_DID, one
 *           message whose type is \ref BTIF_MSG_DID_ATTR_IND and data is \ref
 *           T_BTIF_DID_ATTR_IND may be received. After inquiry procedure is done,
 *           \ref BTIF_MSG_SDP_DISCOV_CMPL_INFO will be received. <b>Only one
 *           SDP/DID discovery can be executed at the same time.</b>
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 * \param[in]  type             SDP discovery type.
 * \arg \c \ref BTIF_SDP_DISCOV_TYPE_BR       Normal SDP discovery.
 * \arg \c \ref BTIF_SDP_DISCOV_TYPE_DID      DID discovery.
 * \param[in]  uuid_type        Type of UUID to discovery. Only valid when discovery type is \ref BTIF_SDP_DISCOV_TYPE_BR.
 * \arg \c     BTIF_UUID16      UUID in 16 bits.
 * \arg \c     BTIF_UUID32      UUID in 32 bits.
 * \arg \c     BTIF_UUID128     UUID in 128 bits.
 * \param[in]  uuid_data        Discovery UUID data corresponding to uuid_type. Only valid when discovery type is \ref BTIF_SDP_DISCOV_TYPE_BR.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SDP_DISCOV_RSP:
           {
               T_BTIF_SDP_DISCOV_RSP *p_rsp = &p_msg->p.sdp_discov_rsp;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_SDP_ATTR_IND:
           {
               T_BTIF_SDP_ATTR_IND *p_ind = &p_msg->p.sdp_attr_ind;
               APP_PRINT_TRACE2("SDP discovery attribute: remote addr %s, remote version 0x%04x",
                                TRACE_BDADDR(p_ind->bd_addr), p_ind->remote_version);
               btif_sdp_attr_cfm(BTIF_CAUSE_SUCCESS);
           }
           break;

       case BTIF_MSG_SDP_DISCOV_CMPL_INFO:
           {
               T_BTIF_SDP_DISCOV_CMPL_INFO *p_info = &p_msg->p.sdp_discov_cmpl_info;
               APP_PRINT_TRACE1("SDP discovery complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       T_BTIF_UUID_DATA uuid_data;

       uuid_data.uuid_16 = UUID_HANDSFREE;
       btif_sdp_discov_req(bd_addr, BTIF_SDP_DISCOV_TYPE_BR, BTIF_UUID16, uuid_data);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sdp_discov_req(uint8_t *bd_addr, T_BTIF_SDP_DISCOV_TYPE type,
                         T_BTIF_UUID_TYPE uuid_type, T_BTIF_UUID_DATA uuid_data);

/**
 * btif.h
 *
 * \brief    Send a confirmation for normal SDP attribute indication. This function
 *           is used when receiving a message whose type is \ref BTIF_MSG_SDP_ATTR_IND
 *           and data is \ref T_BTIF_SDP_ATTR_IND in the callback function
 *           registered by \ref btif_register_req. If the confirmation was
 *           successfully sent, Bluetooth stack will start next SDP attribute
 *           request procedure.
 *
 * \param[in]  cause          Confirmation cause of the SDP attribute indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SDP_DISCOV_RSP:
           {
               T_BTIF_SDP_DISCOV_RSP *p_rsp = &p_msg->p.sdp_discov_rsp;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_SDP_ATTR_IND:
           {
               T_BTIF_SDP_ATTR_IND *p_ind = &p_msg->p.sdp_attr_ind;
               APP_PRINT_TRACE2("SDP discovery attribute: remote addr %s, remote version 0x%04x",
                                TRACE_BDADDR(p_ind->bd_addr), p_ind->remote_version);
               btif_sdp_attr_cfm(BTIF_CAUSE_SUCCESS);
           }
           break;

       case BTIF_MSG_SDP_DISCOV_CMPL_INFO:
           {
               T_BTIF_SDP_DISCOV_CMPL_INFO *p_info = &p_msg->p.sdp_discov_cmpl_info;
               APP_PRINT_TRACE1("SDP discovery complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       T_BTIF_UUID_DATA uuid_data;

       uuid_data.uuid_16 = UUID_HANDSFREE;
       btif_sdp_discov_req(bd_addr, BTIF_SDP_DISCOV_TYPE_BR, BTIF_UUID16, uuid_data);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sdp_attr_cfm(T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a confirmation for DID attribute indication. This function is
 *           used when receiving a message whose type is \ref BTIF_MSG_DID_ATTR_IND
 *           and data is \ref T_BTIF_DID_ATTR_IND in the callback function registered
 *           by \ref btif_register_req. If the confirmation was successfully sent,
 *           Bluetooth stack will start next DID attribute request procedure.
 *
 * \param[in]  cause          Confirmation cause of the DID attribute indication.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SDP_DISCOV_RSP:
           {
               T_BTIF_SDP_DISCOV_RSP *p_rsp = &p_msg->p.sdp_discov_rsp;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_DID_ATTR_IND:
           {
               T_BTIF_DID_ATTR_IND *p_ind = &p_msg->p.did_attr_ind;
               APP_PRINT_TRACE2("DID discovery attribute: remote addr %s, product ID 0x%04x",
                                TRACE_BDADDR(p_ind->bd_addr), p_ind->product_id);
               btif_did_attr_cfm(BTIF_CAUSE_SUCCESS);
           }
           break;

       case BTIF_MSG_SDP_DISCOV_CMPL_INFO:
           {
               T_BTIF_SDP_DISCOV_CMPL_INFO *p_info = &p_msg->p.sdp_discov_cmpl_info;
               APP_PRINT_TRACE1("SDP discovery complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       T_BTIF_UUID_DATA uuid_data = {0};

       btif_sdp_discov_req(bd_addr, BTIF_SDP_DISCOV_TYPE_DID, (T_BTIF_UUID_TYPE)0, uuid_data);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_did_attr_cfm(T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a request to stop SDP discovery. If the request was successfully
 *           sent, a message whose type is \ref BTIF_MSG_STOP_SDP_DISCOV_RSP and
 *           data is \ref T_BTIF_STOP_SDP_DISCOV_RSP will be received in the callback
 *           function registered by \ref btif_register_req. <b>If SDP discovery was
 *           successfully stopped, no \ref BTIF_MSG_SDP_DISCOV_CMPL_INFO message
 *           will be received.</b>
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SDP_DISCOV_RSP:
           {
               T_BTIF_SDP_DISCOV_RSP *p_rsp = &p_msg->p.sdp_discov_rsp;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);

               if (!p_rsp->cause)
               {
                   btif_stop_sdp_discov_req(p_rsp->bd_addr);
               }
           }
           break;

       case BTIF_MSG_STOP_SDP_DISCOV_RSP:
           {
               T_BTIF_STOP_SDP_DISCOV_RSP *p_rsp = &p_msg->p.stop_sdp_discov_rsp;
               APP_PRINT_TRACE1("Stop SDP discovery reponse: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       T_BTIF_UUID_DATA uuid_data;

       uuid_data.uuid_16 = UUID_HANDSFREE;
       btif_sdp_discov_req(bd_addr, BTIF_SDP_DISCOV_TYPE_BR, BTIF_UUID16, uuid_data);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_stop_sdp_discov_req(uint8_t *bd_addr);

/**
 * btif.h
 *
 * \brief    Send a request to start BR/EDR pairing with remote device. If the
 *           request was successfully sent, a message whose type is \ref BTIF_MSG_AUTHEN_RSP
 *           and data is \ref T_BTIF_AUTHEN_RSP will be received in the callback
 *           function registered by \ref btif_register_req when the pairing procedure
 *           is complete.
 *
 * \param[in]  bd_addr    Bluetooth address of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_AUTHEN_RSP:
           {
               T_BTIF_AUTHEN_RSP *p_rsp = &p_msg->p.authen_rsp;
               APP_PRINT_TRACE1("BR/EDR pairing response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

       btif_authen_req(bd_addr);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_authen_req(uint8_t *bd_addr);

/**
 * btif.h
 *
 * \brief    Send a confirmation for authorization request indication. This function
 *           is used when receiving a message whose type is \ref BTIF_MSG_AUTHOR_REQ_IND
 *           and data is \ref T_BTIF_AUTHOR_REQ_IND in the callback function registered
 *           by \ref btif_register_req.
 *
 * \param[in]  bd_addr    Bluetooth address of remote device.
 * \param[in]  cause      Confirmation cause of authorization. \ref BTIF_CAUSE_ACCEPT for accept and others for reject.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_AUTHOR_REQ_IND:
           {
               T_BTIF_AUTHOR_REQ_IND *p_ind = &p_msg->p.author_req_ind;
               btif_author_req_cfm(p_ind->bd_addr, BTIF_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_author_req_cfm(uint8_t *bd_addr, T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a confirmation for PIN code request indication. This function
 *           is used when receiving a message whose type is \ref BTIF_MSG_PIN_CODE_REQ_IND
 *           and data is \ref T_BTIF_PIN_CODE_REQ_IND in the callback function
 *           registered by \ref btif_register_req. User can input PIN code by
 *           using this function.
 *
 * \param[in]  bd_addr         Bluetooth address of remote device.
 * \param[in]  pin_code_len    Length of PIN code. Should not be larger than 16.
 * \param[in]  pin_code        Address of PIN code.
 * \param[in]  cause           Confirmation cause. Pin code is only valid when cause is \ref BTIF_CAUSE_ACCEPT.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_PIN_CODE_REQ_IND:
           {
               T_BTIF_PIN_CODE_REQ_IND *p_ind = &p_msg->p.pin_code_req_ind;
               btif_pin_code_req_cfm(p_ind->bd_addr, 4, "0000",  BTIF_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_pin_code_req_cfm(uint8_t *bd_addr, uint8_t pin_code_len,
                           uint8_t *pin_code, T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a confirmation for remote legacy OOB data request indication.
 *           This function is used when receiving a message whose type is \ref
 *           BTIF_MSG_LEGACY_REMOTE_OOB_REQ_IND and data is \ref
 *           T_BTIF_LEGACY_REMOTE_OOB_REQ_IND in the callback function
 *           registered by \ref btif_register_req.
 *
 * \param[in]  bd_addr         Bluetooth address of remote device.
 * \param[in]  p_data_c        Address of BR/EDR OOB data simple pairing hash C.
 * \param[in]  p_data_r        Address of BR/EDR OOB data simple pairing randonizer R.
 * \param[in]  cause           Confirmation cause. OOB data are only valid when cause is \ref BTIF_CAUSE_ACCEPT.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_LEGACY_REMOTE_OOB_REQ_IND:
           {
               T_BTIF_LEGACY_REMOTE_OOB_REQ_IND *p_ind = &p_msg->p.legacy_remote_oob_req_ind;
               uint8_t oob_r[KEY_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
               uint8_t oob_c[KEY_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

               btif_legacy_remote_oob_req_cfm(p_ind->bd_addr, oob_c, oob_r,  BTIF_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_legacy_remote_oob_req_cfm(uint8_t *bd_addr, uint8_t *p_data_c,
                                    uint8_t *p_data_r, T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Send a request to get local BR/EDR OOB data. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_LOCAL_OOB_RSP
 *           and data is \ref T_BTIF_LOCAL_OOB_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  None.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_LOCAL_OOB_RSP:
           {
               T_BTIF_LOCAL_OOB_RSP *p_rsp = &p_msg->p.local_oob_rsp;
               APP_PRINT_TRACE2("Local OOB data response: data c %b, data r %b",
                                TRACE_BINARY(16, p_rsp->data_c), TRACE_BINARY(16, p_rsp->data_r));
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_local_oob_req();
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_local_oob_req(void);

/**
 * btif.h
 *
 * \brief    Send a request to configure link policy for an ACL link. If the
 *           request was successfully sent, a message whose type is \ref BTIF_MSG_ACL_CFG_RSP
 *           and data is \ref T_BTIF_ACL_CFG_RSP will be received in the callback
 *           function registered by \ref btif_register_req. Operation code in \ref
 *           T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_LINK_POLICY. Bluetooth
 *           stack will automatically set link policy after ACL link is connected.
 *           <b> To change the default link policy that Bluetooth stack will set,
 *           \ref btif_dev_link_policy_cfg_req can be used.</b>\ref btif_acl_link_policy_cfg_req
 *           is used to change link policy after that. <b>In order to avoid collision,
 *           user should set link policy after receiving \ref BTIF_MSG_ACL_STATUS_INFO
 *           message with ACL status as \ref BTIF_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr        Bluetooth address of remote device.
 * \param[in]  policy         Link policy to set. Valid values are combinations of \ref BTIF_BR_LINK_POLICY.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_READY:
                   btif_acl_link_policy_cfg_req(p_info->bd_addr, BTIF_BR_LINK_POLICY_DISABLE_ALL);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_LINK_POLICY)
               {
                   APP_PRINT_TRACE1("Configure ACL link policy response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_link_policy_cfg_req(uint8_t *bd_addr, T_BTIF_BR_LINK_POLICY policy);

/**
 * btif.h
 *
 * \brief    Send a request to configure link supervision timeout value for an
 *           ACL link. If the request was successfully sent, a message whose type
 *           is \ref BTIF_MSG_ACL_CFG_RSP and data is \ref T_BTIF_ACL_CFG_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *           Operation code in \ref T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_LINK_SUPV_TOUT.
 *           <b>The function is only useful when local is master of the ACL link.</b>
 *           Bluetooth stack will automatically set link supervision timeout value
 *           after ACL link is connected as master.<b> To change the default link
 *           supervision timeout value that Bluetooth stack will set, \ref btif_dev_link_policy_cfg_req
 *           can be used.</b>\ref btif_acl_link_supv_tout_cfg_req is used to change
 *           link supervision timeout value after that. <b>In order to avoid collision,
 *           user should set link policy after receiving \ref BTIF_MSG_ACL_STATUS_INFO
 *           message with ACL status as \ref BTIF_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  supv_tout           Supervision timeout value to set.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_READY:
                   btif_acl_link_supv_tout_cfg_req(p_info->bd_addr, 0x7D00);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_LINK_SUPV_TOUT)
               {
                   APP_PRINT_TRACE1("Configure ACL link supervision timeout response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_link_supv_tout_cfg_req(uint8_t *bd_addr, uint16_t supv_tout);

/**
 * btif.h
 *
 * \brief    Send a request to switch link role for an ACL link. If the request
 *           was successfully sent, a message whose type is \ref BTIF_MSG_ACL_CFG_RSP
 *           and data is \ref T_BTIF_ACL_CFG_RSP will be received in the callback
 *           function registered by \ref btif_register_req. Operation code in \ref
 *           T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_LINK_ROLE. If switch
 *           role procedure was successful, \ref BTIF_MSG_ACL_STATUS_INFO message
 *           will also be received with ACL status as \ref BTIF_ACL_ROLE_MASTER
 *           or \ref BTIF_ACL_ROLE_SLAVE depending on the switch request. If BR/EDR
 *           secure connections is supported, role change during SMP exchange is
 *           forbidden. <b>Recommended timing to switch is after receiving \ref
 *           BTIF_MSG_ACL_STATUS_INFO message with ACL status as \ref BTIF_ACL_CONN_ENCRYPTED
 *           or ACL status as \ref BTIF_ACL_CONN_BR_SC_CMPL if \ref BTIF_ACL_CONN_BR_SC_START
 *           was received before.</b>
 *
 * \param[in]  bd_addr         Bluetooth address of remote device.
 * \param[in]  role            Device role to configure.
 * \arg \c     0                   Switch to master role.
 * \arg \c     1                   Switch to slave role.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t local_role = 0;

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_ENCRYPTED:
                   if (local_role == 1)
                   {
                       legacy_cfg_acl_link_role(p_info->bd_addr, 0);
                   }
                   break;

               case BTIF_ACL_ROLE_MASTER:
                   local_role = 0;
                   break;

               case BTIF_ACL_ROLE_SLAVE:
                   local_role = 1;
                   break;

               default:
                   break;

               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_LINK_ROLE)
               {
                   APP_PRINT_TRACE1("Configure ACL link role response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_link_role_cfg_req(uint8_t *bd_addr, T_BTIF_BR_LINK_ROLE role);

/**
 * btif.h
 *
 * \brief    Send a request to change packet type for an ACL link. If the request
 *           was successfully sent, a message whose type is \ref BTIF_MSG_ACL_CFG_RSP
 *           and data is \ref T_BTIF_ACL_CFG_RSP will be received in the callback
 *           function registered by \ref btif_register_req. Operation code in \ref
 *           T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_PKT_TYPE. BT stack
 *           will automatically set link packet type after ACL link is connected.
 *           <b>In order to avoid collision, user should set link policy after
 *           receiving \ref BTIF_MSG_ACL_STATUS_INFO message with ACL status as
 *           \ref BTIF_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  pkt_type            Packet type to use. Valid values are combinations of \ref BTIF_BR_ACL_PKT_TYPE.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_READY:
                   btif_acl_pkt_type_cfg_req(p_info->bd_addr,
                                             BTIF_PKT_TYPE_NO_3DH1 | BTIF_PKT_TYPE_DM1 |
                                             BTIF_PKT_TYPE_DH1 | BTIF_PKT_TYPE_NO_3DH3 |
                                             BTIF_PKT_TYPE_DM3 | BTIF_PKT_TYPE_DH3 |
                                             BTIF_PKT_TYPE_NO_3DH5);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_PKT_TYPE)
               {
                   APP_PRINT_TRACE1("Configure packet type response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_pkt_type_cfg_req(uint8_t *bd_addr, uint16_t pkt_type);

/**
 * btif.h
 *
 * \brief    Send a request to configure ACL link with remote device into sniff
 *           mode. If the request was successfully sent, a message whose type is
 *           \ref BTIF_MSG_ACL_CFG_RSP and data is \ref T_BTIF_ACL_CFG_RSP will
 *           be received in the callback function registered by \ref btif_register_req.
 *           Operation code in \ref T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_ENTER_SNIFF_MODE.
 *           If the link enters sniff mode successfully, \ref BTIF_MSG_ACL_STATUS_INFO
 *           message will also be received with ACL status as \ref BTIF_ACL_CONN_SNIFF,
 *           the actual sniff interval will also be contained in the message data.
 *           <b>To configure an ACL link into sniff mode, the link must be connected
 *           and ready to be configured, which can be sure by receiving \ref
 *           BTIF_MSG_ACL_STATUS_INFO message with ACL status as \ref BTIF_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  min_interval        Min sniff interval, only even values between 0x0002 and 0xFFFE are valid.
 * \param[in]  max_interval        Max sniff interval, only even values between 0x0002 and 0xFFFE are valid, should be larger than min_interval.
 * \param[in]  sniff_attempt       Number of baseband receive slots for sniff attempt.
 * \param[in]  sniff_tout          Number of baseband receive slots for sniff timeout.
 * \param[in]  max_latency         Max allowed sniff subrate of the remote device. Set to 0 if do not want to configure sniff subrate.
 * \param[in]  min_remote_tout     Min base sniff subrate timeout the remote device may use.
 * \param[in]  min_local_tout      Min base sniff subrate timeout the local device may use.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_READY:
                   btif_acl_sniff_mode_enter_req(p_info->bd_addr, 192, 800, 4, 2, 0, 0, 0);
                   break;

               case BTIF_ACL_CONN_SNIFF:
                   APP_PRINT_TRACE1("ACL link into sniff mode: interval 0x%04x", p_info->p.sniff.interval);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_ENTER_SNIFF_MODE)
               {
                   APP_PRINT_TRACE1("Configure sniff mode response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_sniff_mode_enter_req(uint8_t *bd_addr, uint16_t min_interval, uint16_t max_interval,
                                   uint16_t sniff_attempt, uint16_t sniff_tout, uint16_t max_latency,
                                   uint16_t min_remote_tout, uint16_t min_local_tout);

/**
 * btif.h
 *
 * \brief    Send a request to configure ACL link with remote device to exit sniff
 *           mode. If the request was successfully sent, a message whose type is
 *           \ref BTIF_MSG_ACL_CFG_RSP and data is \ref T_BTIF_ACL_CFG_RSP will
 *           be received in the callback function registered by \ref btif_register_req.
 *           Operation code in \ref T_BTIF_ACL_CFG_RSP should be \ref BTIF_ACL_CFG_EXIT_SNIFF_MODE.
 *           If the link exits sniff mode successfully, \ref BTIF_MSG_ACL_STATUS_INFO
 *           message will also be received with ACL status as \ref BTIF_ACL_CONN_ACTIVE.
 *           <b>To configure an ACL link to exit sniff mode, the link must be in sniff
 *           mode, which can be sure by receiving \ref BTIF_MSG_ACL_STATUS_INFO
 *           message with ACL status as \ref BTIF_ACL_CONN_SNIFF.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_SNIFF:
                   APP_PRINT_TRACE1("ACL link into sniff mode: interval 0x%04x", p_info->p.sniff.interval);
                   btif_acl_sniff_mode_exit_req(p_info->bd_addr);
                   break;

               case BTIF_ACL_CONN_ACTIVE:
                   APP_PRINT_TRACE0("ACL link into active mode");
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_CFG_RSP:
           {
               T_BTIF_ACL_CFG_RSP *p_rsp = &p_msg->p.acl_cfg_rsp;

               if (p_rsp->op == BTIF_ACL_CFG_EXIT_SNIFF_MODE)
               {
                   APP_PRINT_TRACE1("EXIT sniff mode response: cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_sniff_mode_exit_req(uint8_t *bd_addr);

/**
 * btif.h
 *
 * \brief    Send a request to register a SDP record into Bluetooth stack. If the
 *           request was successfully sent, a message whose type is \ref BTIF_MSG_SDP_REG_RSP
 *           and data is \ref T_BTIF_SDP_REG_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  p_buf    Address of SDP record buffer. <b>The buffer shall not
 *                      be released since Bluetooth stack will record the buffer
 *                      address instead of copy the whole buffer.</b>
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   //DID SDP record buffer
   const uint8_t did_sdp_record[] =
   {
       SDP_DATA_ELEM_SEQ_HDR,
       0x4D,
       //attribute SDP_ATTR_SRV_CLASS_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_SRV_CLASS_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)(UUID_PNP_INFORMATION),

       //attribute SDP_ATTR_BROWSE_GROUP_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_BROWSE_GROUP_LIST >> 8),
       (uint8_t)SDP_ATTR_BROWSE_GROUP_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PUBLIC_BROWSE_GROUP >> 8),
       (uint8_t)UUID_PUBLIC_BROWSE_GROUP,

       //attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x09,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_LANG_ENGLISH >> 8),
       (uint8_t)SDP_LANG_ENGLISH,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_CHARACTER_UTF8 >> 8),
       (uint8_t)SDP_CHARACTER_UTF8,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_BASE_LANG_OFFSET >> 8),
       (uint8_t)SDP_BASE_LANG_OFFSET,

       //attribute SDP_ATTR_PROFILE_DESC_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_PROFILE_DESC_LIST >> 8),
       (uint8_t)SDP_ATTR_PROFILE_DESC_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x08,
       SDP_DATA_ELEM_SEQ_HDR,
       0x06,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)UUID_PNP_INFORMATION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,//version 1.3
       0x03,

       //attribute SDP_ATTR_DIP_SPECIFICATION_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_SPECIFICATION_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_SPECIFICATION_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,
       0x03,

       //attribute SDP_ATTR_DIP_VENDOR_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID,
       SDP_UNSIGNED_TWO_BYTE,
       //0x00,//0x005D : RealTek
       //0x5D,
       (uint8_t)(RTK_COMPANY_ID >> 8),
       (uint8_t)RTK_COMPANY_ID,

       //attribute SDP_ATTR_DIP_PRODUCT_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x22,//8763
       0x3B,

       //attribute SDP_ATTR_DIP_PRODUCT_VERSION
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_VERSION >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_VERSION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,// 1.0.0
       0x00,

       //attribute SDP_ATTR_DIP_PRIMARY_RECORD
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRIMARY_RECORD >> 8),
       (uint8_t)SDP_ATTR_DIP_PRIMARY_RECORD,
       SDP_BOOL_ONE_BYTE,
       true,

       //attribute SDP_ATTR_DIP_VENDOR_ID_SOURCE
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID_SOURCE >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID_SOURCE,
       SDP_UNSIGNED_TWO_BYTE,
       0x00,//Bluetooth SIG
       0x01
   };

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SDP_REG_RSP:
           {
               T_BTIF_SDP_REG_RSP *p_rsp = &p_msg->p.sdp_reg_rsp;
               APP_PRINT_TRACE1("Register SDP record response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_sdp_reg_req(did_sdp_record);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sdp_reg_req(void *p_buf);

/**
 * btif.h
 *
 * \brief    Send a request to register/deregister a protocol over L2CAP in Bluetooth
 *           stack. If the request was successfully sent, a message whose type is \ref
 *           BTIF_MSG_L2C_PROTO_REG_RSP and data is \ref T_BTIF_L2C_PROTO_REG_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *           <b>The protocol must be registered before creating a L2CAP connection
 *           or accepting a L2CAP connection request from other devices.</b>
 *
 * \param[in]  psm          PSM value of the protocol.
 * \param[in]  proto_id     Protocol ID assigned by upper layer. <b>The protocol ID must be less than 0x10.</b>
 * \param[in]  action       Requested action for the protocol.
 * \arg \c     0                   Register the protocol into Bluetooth stack.
 * \arg \c     1                   Deregister the protocol from Bluetoth stack.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_PROTO_REG_RSP:
           {
               T_BTIF_L2C_PROTO_REG_RSP *p_rsp = &p_msg->p.l2c_proto_reg_rsp;
               APP_PRINT_TRACE1("Register protocol response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_l2c_proto_reg_req(0x17, 0x01, 1);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_proto_reg_req(uint16_t psm, uint16_t proto_id, uint8_t action);

/**
 * btif.h
 *
 * \brief    Send a request to connect a L2CAP channel with remote device. If the
 *           request was successfully sent, a message whose type is \ref BTIF_MSG_L2C_CONN_RSP
 *           and data is \ref T_BTIF_L2C_CONN_RSP will be received in the callback
 *           function registered by \ref btif_register_req to indicate whether
 *           the procedure was started successfully. If the procedure was started,
 *           another message whose type is \ref BTIF_MSG_L2C_CONN_CMPL_INFO and
 *           data is \ref T_BTIF_L2C_CONN_CMPL_INFO will also be received to tell
 *           the result of connection establishment.
 *
 * \param[in]  psm              PSM value of protocol on the L2CAP channel. <b>The protocol ID and PSM must be registered by \ref btif_l2c_proto_reg_req.</b>
 * \param[in]  uuid             UUID of the protocol on the L2CAP channel.
 * \param[in]  bd_addr          Bluetooth address of remote device.
 * \param[in]  proto_id         Protocol ID assigned by upper layer. <b>The protocol ID and PSM must be registered by \ref btif_l2c_proto_reg_req.</b>
 * \param[in]  mtu_size         MTU size of L2CAP channel.
 * \param[in]  mode_bits        L2CAP modes that the channel can be used. Valid values are combinations of \ref BTIF_BR_L2CAP_MODE.
 * \param[in]  flush_tout       Flush timeout value to use for data transmitted on the channel. 0xFFFF for no flush.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_PROTO_REG_RSP:
           {
               T_BTIF_L2C_PROTO_REG_RSP *p_rsp = &p_msg->p.l2c_proto_reg_rsp;

               if (!p_rsp->cause)
               {
                   uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
                   btif_l2c_conn_req(0x17, 0x0017, 0x01, 672, bd_addr, BTIF_L2C_MODE_BASIC, 0xFFFF);
               }
           }
           break;

       case BTIF_MSG_L2C_CONN_RSP:
           {
               T_BTIF_L2C_CONN_RSP *p_rsp = &p_msg->p.l2c_conn_rsp;
               APP_PRINT_TRACE("L2CAP connect reponse: cause 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;
               APP_PRINT_TRACE1("L2CAP channel establish result: cause 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_l2c_proto_reg_req(0x17, 0x01, 1);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_conn_req(uint16_t psm, uint16_t uuid, uint8_t *bd_addr, uint16_t proto_id,
                       uint16_t mtu_size, uint8_t mode_bits, uint16_t flush_tout);

/**
 * btif.h
 *
 * \brief    Send a confirmation for L2CAP channel connect indication. If the
 *           confirmation was successfully sent and confirm cause is \ref BTIF_L2C_CONN_ACCEPT,
 *           a message whose type is \ref BTIF_MSG_L2C_CONN_CMPL_INFO and data is
 *           \ref T_BTIF_L2C_CONN_CMPL_INFO will be received in the callback
 *           function registered by \ref btif_register_req to tell the channel
 *           establish result. <b>The function can only be used when receive \ref
 *           BTIF_MSG_L2C_CONN_IND message in the callback function.</b>
 *
 * \param[in]  cause            Confirm cause for L2CAP channel connection request. \ref BTIF_L2C_CONN_ACCEPT to accept the connection, otherwise reject.
 * \param[in]  cid              Local CID received in \ref BTIF_MSG_L2C_CONN_IND.
 * \param[in]  mtu_size         MTU size of L2CAP channel.
 * \param[in]  mode_bits        L2CAP modes that the channel can be used. Valid values are combinations of \ref BTIF_BR_L2CAP_MODE.
 * \param[in]  flush_tout       Flush timeout value to use for data transmitted on the channel. 0xFFFF for no flush.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_IND:
           {
               T_BTIF_L2C_CONN_IND *p_ind = &p_msg->p.l2c_conn_ind;

               btif_l2c_conn_cfm(BTIF_L2C_CONN_ACCEPT, p_ind->cid, 672, BTIF_L2C_MODE_BASIC, 0xFFFF);
           }
           break;

       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;
               APP_PRINT_TRACE1("L2CAP channel establish result: cause 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_conn_cfm(T_BTIF_L2C_CONN_CAUSE cause, uint16_t cid, uint16_t mtu_size,
                       uint8_t mode_bits, uint16_t flush_tout);

/**
 * btif.h
 *
 * \brief    Get buffer from Bluetooth stack to put in L2CAP data which will be sent to remote device.
 *
 * \param[in]  size         Length of L2CAP data.
 * \param[in]  offset       Offset of real L2CAP data in the buffer. Offset value can be get from \ref BTIF_MSG_L2C_CONN_CMPL_INFO message.
 * \param[in]  ack          Whether need an ack when the buffer was released. If set to true, \ref BTIF_MSG_L2C_DATA_RSP message will be
 *                          received in the callback function registered by \ref btif_register_req.
 *
 * \return    The address of the allocated buffer. If the address in NULL, the buffer was get failed.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = btif_get_l2c_buf(330, p_info->ds_data_offset, false);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       btif_l2c_data_req(p_buffer, p_info->ds_data_offset, p_info->cid,  330, false);
                   }
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
void *btif_get_l2c_buf(uint16_t size, uint16_t offset, bool ack);

/**
 * btif.h
 *
 * \brief    Send a request to send L2CAP data to remote device.
 *
 * \param[in]  p_buf        Address of L2CAP data buffer which is allocated by \ref btif_get_l2c_buf.
 * \param[in]  offset       Offset of real L2CAP data in the buffer. Offset value can be get from \ref BTIF_MSG_L2C_CONN_CMPL_INFO message.
 * \param[in]  cid          Local CID. The value can be get from \ref BTIF_MSG_L2C_CONN_CMPL_INFO message.
 * \param[in]  length       Length of real L2CAP data.
 * \param[in]  auto_flush   Whether the L2CAP data can be flushed or not.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = btif_get_l2c_buf(330, p_info->ds_data_offset, false);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       btif_l2c_data_req(p_buffer, p_info->ds_data_offset, p_info->cid,  330, false);
                   }
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_data_req(void *p_buf, uint16_t offset, uint16_t cid,
                       uint16_t length, bool auto_flush);

/**
 * btif.h
 *
 * \brief    Send a request to disconnect a L2CAP channel. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_L2C_DISCONN_RSP
 *           and data is \ref T_BTIF_L2C_DISCONN_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  cid         Local CID. The value can be get from \ref BTIF_MSG_L2C_CONN_CMPL_INFO message.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   btif_l2c_disconn_req(p_info->cid);
               }
           }
           break;

       case BTIF_MSG_L2C_DISCONN_RSP:
           {
               T_BTIF_L2C_DISCONN_RSP *p_rsp = &p_msg->p.l2c_disconn_rsp;

               APP_PRINT_TRACE1("L2CAP channel disconnect response: status 0x%04x",
                                p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_disconn_req(uint16_t cid);

/**
 * btif.h
 *
 * \brief    Send a confirmation for L2CAP channel disconnect indication. <b>The
 *           function can only be used when receive \ref BTIF_MSG_L2C_DISCONN_IND
 *           message in the callback function registered by \ref btif_register_req.</b>
 *
 * \param[in]  cid        Local CID. The value can be get from \ref BTIF_MSG_L2C_DISCONN_IND message.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_DISCONN_IND:
           {
               T_BTIF_L2C_DISCONN_IND *p_ind = &p_msg->p.l2c_disconn_ind;
               btif_l2c_disconn_cfm(p_ind->cid);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_disconn_cfm(uint16_t cid);

/**
 * btif.h
 *
 * \brief    Send a request to register/deregister a security entry over L2CAP.
 *           The security will be used when establishing a L2CAP channel. If the
 *           request was successfully sent, a message whose type is \ref BTIF_MSG_L2C_SEC_REG_RSP
 *           and data is \ref T_BTIF_L2C_SEC_REG_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param  active             Register/Deregister the security entry.
 * \param  psm                PSM value for protocol.
 * \param  server_chann       Local server channel number for RFCOMM, for other protocols set to 0.
 * \param  uuid               UUID for profiles over rfcomm or UUID for other protocols except RFCOMM.
 * \param  requirement        Security requirement of the entry. Valid values are combinations of \ref BTIF_BR_SEC.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_SEC_REG_RSP:
           {
               T_BTIF_L2C_SEC_REG_RSP *p_rsp = &p_msg->p.l2c_sec_reg_rsp;
               APP_PRINT_TRACE1("Register L2CAP security entry response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       btif_l2c_sec_reg_req(1, 0x17, 0, 0x17, BTIF_BR_SEC_AUTHEN | BTIF_BR_SEC_MITM | BTIF_BR_SEC_ENCRYPT);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_l2c_sec_reg_req(uint8_t active, uint16_t psm, uint16_t server_chann,
                          uint16_t uuid, uint8_t requirement);

/**
 * btif.h
 *
 * \brief    Send a request to start authentication for a profile on RFCOMM channel.
 *           If the request was successfully sent, a message whose type is \ref
 *           BTIF_MSG_RFC_AUTHEN_RSP and data is \ref T_BTIF_RFC_AUTHEN_RSP will
 *           be received in the callback function registered by \ref btif_register_req.
 *
 * \param  bd_addr            Bluetooth address of remote device.
 * \param  cid                L2CAP local CID.
 * \param  dlci               DLCI value for the RFCOMM channel.
 * \param  uuid               UUID for profiles over rfcomm.
 * \param  outgoing           Direction of the profile over rfcomm channel.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_RFC_AUTHEN_RSP:
           {
               T_BTIF_RFC_AUTHEN_RSP *p_rsp = &p_msg->p.rfc_authen_rsp;
               APP_PRINT_TRACE1("RFCOMM authentication response: cause 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // Connect profile over RFCOMM.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       btif_rfc_authen_req(bd_addr, 0x40, 0x12, 0x111E, 0);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_rfc_authen_req(uint8_t *bd_addr, uint16_t cid, uint16_t dlci,
                         uint16_t uuid, uint8_t outgoing);

/**
 * btif.h
 *
 * \brief    Send a request to create a SCO/eSCO connection. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_SCO_CONN_RSP
 *           and data is \ref T_BTIF_SCO_CONN_RSP will be received in the callback
 *           function registered by \ref btif_register_req to indicate whether
 *           the procedure was started successfully. If the procedure was started,
 *           another message whose type is \ref BTIF_MSG_SCO_CONN_CMPL_INFO and
 *           data is \ref T_BTIF_SCO_CONN_CMPL_INFO will also be received to tell
 *           the result of connection establishment.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  tx_bandwidth         Transmit bandwidth in octets per second.
 * \param[in]  rx_bandwidth         Receive bandwidth in octets per second.
 * \param[in]  max_latency          A value in milliseconds representing upper limit of the sum of the synchronous interval. Valid between 0x0004 to 0xFFFE.
 * \param[in]  voice_setting        Voice setting bitmap.
 * \param[in]  retrans_effort       Retransmit effort.
 * \arg \c     0x00             No retransmissions.
 * \arg \c     0x01             At least one retransmission, optimize for power consumption.
 * \arg \c     0x02             At least one retransmission, optimize for link quality.
 * \arg \c     0xFF             Don't care.
 * \param[in]  packet_type          Transmit packet type. Valid values are combinations of \ref BTIF_BR_SCO_PKT_TYPE.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_CONN_RSP:
           {
               T_BTIF_SCO_CONN_RSP *p_rsp = &p_msg->p.sco_conn_rsp;
               APP_PRINT_TRACE1("SCO connect response: cause 0x%04x", p_rsp->cause);
           }
           break;

       case BTIF_MSG_SCO_CONN_CMPL_INFO:
           {
               T_BTIF_SCO_CONN_CMPL_INFO *p_info = &p_msg->p.sco_conn_cmpl_info;
               APP_PRINT_TRACE1("SCO connection establish result: cause 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
       btif_sco_conn_req(bd_addr, 8000, 8000, 7, 0x0360, 0, BTIF_PKT_TYPE_EV3 | BTIF_PKT_TYPE_EV4 | BTIF_PKT_TYPE_EV5);
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_conn_req(uint8_t *bd_addr, uint32_t tx_bandwidth, uint32_t rx_bandwidth,
                       uint16_t max_latency, uint16_t voice_setting, uint8_t  retrans_effort, uint16_t packet_type);

/**
 * btif.h
 *
 * \brief    Send a confirmation for SCO/eSCO connect indication. If the confirmation
 *           was successfully sent and confirm cause is \ref BTIF_CAUSE_ACCEPT,
 *           a message whose type is \ref BTIF_MSG_SCO_CONN_CMPL_INFO and data is
 *           \ref T_BTIF_SCO_CONN_CMPL_INFO will be received in the callback
 *           function registered by \ref btif_register_req to tell the connection
 *           establish result. <b>The function can only be used when receive \ref
 *           BTIF_MSG_SCO_CONN_IND message in the callback function.</b>
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  tx_bandwidth         Transmit bandwidth in octets per second.
 * \param[in]  rx_bandwidth         Receive bandwidth in octets per second.
 * \param[in]  max_latency          A value in milliseconds representing upper limit of the sum of the synchronous interval. Valid between 0x0004 to 0xFFFE.
 * \param[in]  voice_setting        Voice setting bitmap.
 * \param[in]  retrans_effort       Retransmit effort.
 * \arg \c     0x00             No retransmissions.
 * \arg \c     0x01             At least one retransmission, optimize for power consumption.
 * \arg \c     0x02             At least one retransmission, optimize for link quality.
 * \arg \c     0xFF             Don't care.
 * \param[in]  packet_type          Transmit packet type. Valid values are combinations of \ref BTIF_BR_SCO_PKT_TYPE.
 * \param[in]  cause                Confirm cause for SCO connection request. \ref BTIF_CAUSE_ACCEPT to accept the connection, otherwise reject.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_CONN_IND:
           {
               T_BTIF_SCO_CONN_IND *p_ind = &p_msg->p.sco_conn_ind;

               btif_sco_conn_cfm(p_ind->bd_addr, 8000, 8000, 13, 0x0363, 1,
                         BTIF_PKT_TYPE_EV3 | BTIF_PKT_TYPE_EV4 | BTIF_PKT_TYPE_EV5,
                         BTIF_CAUSE_ACCEPT);
           }
           break;

       case BTIF_MSG_SCO_CONN_CMPL_INFO:
           {
               T_BTIF_SCO_CONN_CMPL_INFO *p_info = &p_msg->p.sco_conn_cmpl_info;
               APP_PRINT_TRACE1("SCO connection establish result: cause 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_conn_cfm(uint8_t *bd_addr, uint32_t tx_bandwidth, uint32_t rx_bandwidth,
                       uint16_t max_latency, uint16_t voice_setting, uint8_t retrans_effort,
                       uint16_t packet_type, T_BTIF_CAUSE cause);

/**
 * btif.h
 *
 * \brief    Get buffer from Bluetooth stack to put in SCO data which will be sent to remote device.
 *
 * \param[in]  len              Length of SCO data to put into the buffer.
 *
 * \return    The address of the allocated buffer. If the address in NULL, the
 *            buffer was get failed.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_CONN_CMPL_INFO:
           {
               T_BTIF_SCO_CONN_CMPL_INFO *p_info = &p_msg->p.sco_conn_cmpl_info;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = btif_sco_buffer_get(60);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       btif_sco_data_req(p_info->handle, p_buffer, 60);
                   }
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
void *btif_sco_buffer_get(uint8_t len);

/**
 * btif.h
 *
 * \brief    Send a request to send SCO data to remote device.
 *
 * \param[in]  handle          Connection handle of the SCO/eSCO link.
 * \param[in]  p_buf           Address of SCO data buffer which is get by \ref btif_sco_buffer_get.
 * \param[in]  len             Length of SCO data buffer. Must match the one in \ref btif_sco_buffer_get.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_CONN_CMPL_INFO:
           {
               T_BTIF_SCO_CONN_CMPL_INFO *p_info = &p_msg->p.sco_conn_cmpl_info;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = btif_sco_buffer_get(60);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       btif_sco_data_req(p_info->handle, p_buffer, 60);
                   }
               }
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_data_req(uint16_t handle, void *p_buf, uint8_t len);

/**
 * btif.h
 *
 * \brief    Send a confirmation for SCO data indication. <b>The function can
 *           only be used when receive \ref BTIF_MSG_SCO_DATA_IND message in the
 *           callback function registered by \ref btif_register_req.</b>
 *
 * \param[in]  p_buf           Address of SCO data buffer. Must be same as that in \ref BTIF_MSG_SCO_DATA_IND message.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_DATA_IND:
           {
               T_BTIF_SCO_DATA_IND *p_ind = &p_msg->p.sco_data_ind;
               btif_sco_data_cfm(p_ind->p_buf);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_data_cfm(uint8_t *p_buf);

/**
 * btif.h
 *
 * \brief    Send a request to disconnect a SCO/eSCO connection. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_SCO_DISCONN_RSP
 *           and data is \ref T_BTIF_SCO_DISCONN_RSP will be received in the callback
 *           function registered by \ref btif_register_req.
 *
 * \param[in]  handle         Connection handle of the SCO/eSCO link. The handle can be get in \ref T_BTIF_SCO_CONN_CMPL_INFO message.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_CONN_CMPL_INFO:
           {
               T_BTIF_SCO_CONN_CMPL_INFO *p_info = &p_msg->p.sco_conn_cmpl_info;

               if (!p_info->cause)
               {
                   btif_sco_disconn_req(p_info->handle);
               }
           }
           break;

       case BTIF_MSG_SCO_DISCONN_RSP:
           {
               T_BTIF_SCO_DISCONN_RSP *p_rsp = &p_msg->p.sco_disconn_rsp;

               APP_PRINT_TRACE2("SCO disconnect response: status 0x%04x, reason 0x%04x",
                                p_rsp->cause, p_rsp->reason);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_disconn_req(uint16_t handle);

/**
 * btif.h
 *
 * \brief    Send a confirmation for SCO/eSCO disconnect indication. <b>The function
 *           can only be used when receive \ref BTIF_MSG_SCO_DISCONN_IND message
 *           in the callback function registered by \ref btif_register_req.</b>
 *
 * \param[in]  handle              Connection handle of the SCO/eSCO link.
 *
 * \return    The status of sending the confirmation.
 * \retval    true     The confirmation was successfully sent.
 * \retval    false    The confirmation was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_SCO_DISCONN_IND:
           {
               T_BTIF_SCO_DISCONN_IND *p_ind = &p_msg->p.sco_disconn_ind;
               btif_sco_disconn_cfm(p_ind->handle);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_sco_disconn_cfm(uint16_t handle);

/**
 * btif.h
 *
 * \brief    Send a request to disconnect an ACL link. If the request was
 *           successfully sent, a message whose type is \ref BTIF_MSG_ACL_DISCONN_RSP
 *           and data is \ref T_BTIF_ACL_DISCONN_RSP will be received in the callback
 *           function registered by \ref btif_register_req. If the ACL link was
 *           successfully disconnected, \ref BTIF_MSG_ACL_STATUS_INFO message will
 *           also be received with ACL status as \ref BTIF_ACL_CONN_DISCONN in the
 *           message data. <b>To disconnect an ACL link, it must be connected,
 *           which can be sure by receiving \ref BTIF_MSG_ACL_STATUS_INFO message
 *           with ACL status as \ref BTIF_ACL_CONN_SUCCESS in the message data.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_SUCCESS:
                   btif_acl_disconn_req(p_info->bd_addr);
                   break;

               case BTIF_ACL_CONN_DISCONN:
                   APP_PRINT_TRACE0("ACL link disconnected");
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_ACL_DISCONN_RSP:
           {
               T_BTIF_ACL_DISCONN_RSP *p_rsp = &p_msg->p.acl_disconn_rsp;
               APP_PRINT_TRACE2("ACL link disconnect response: status 0x%04x, reason 0x%04x",
                                p_rsp->cause, p_rsp->reason);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_acl_disconn_req(uint8_t *bd_addr);

/**
 * btif.h
 *
 * \brief    Send a request to specify Quality of Service parameters for an ACL
 *           link. If the request was successfully sent, a message whose type is
 *           \ref BTIF_MSG_SETUP_QOS_RSP and data is \ref T_BTIF_SETUP_QOS_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *           <b> To set the QOS of an ACL link, the link must be connected, which
 *           can be sure by receiving \ref BTIF_MSG_ACL_STATUS_INFO message
 *           with ACL status as \ref BTIF_ACL_CONN_SUCCESS in the message data.</b>
 *
 * \param[in]  handle              Connection handle of the link, which can be get by \ref BTIF_MSG_ACL_STATUS_INFO message with ACL status as \ref BTIF_ACL_CONN_SUCCESS.
 * \param[in]  flags               Reserved for feature use.
 * \param[in]  type                Service type.
 * \arg \c     0               No traffic.
 * \arg \c     1               Best effort.
 * \arg \c     2               Guaranteed.
 * \param[in]  token_rate          Token rate in octets per second.
 * \param[in]  peak_bandwdith      Peak bandwidth in octets per second.
 * \param[in]  latency             Latency in microseconds.
 * \param[in]  delay_variation     Delay variation in microseconds.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_ACL_STATUS_INFO:
           {
               T_BTIF_ACL_STATUS_INFO *p_info = &p_msg->p.acl_status_info;

               switch (p_info->status)
               {
               case BTIF_ACL_CONN_SUCCESS:
                   legacy_send_setup_qos_req(p_info->p.conn_success.handle, 0,
                                             QOS_GUARANTEED, 11250, 0, 10000 * 2, 0);
                   break;

               default:
                   break;
               }
           }
           break;

       case BTIF_MSG_SETUP_QOS_RSP:
           {
               T_BTIF_SETUP_QOS_RSP *p_rsp = &p_msg->p.setup_qos_rsp;
               APP_PRINT_TRACE1("Setup QOS response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_setup_qos_req(uint16_t handle, uint8_t flags, uint8_t type, uint32_t token_rate,
                        uint32_t peak_bandwdith, uint32_t latency, uint32_t delay_variation);

/**
 * btif.h
 *
 * \brief    Send a request to set local channel into busy/idle state in L2CAP
 *           ERTM mode. If the request was successfully sent, a message whose type
 *           is \ref BTIF_MSG_L2C_SET_LOCAL_BUSY_RSP and data is \ref T_BTIF_L2C_SET_LOCAL_BUSY_RSP
 *           will be received in the callback function registered by \ref btif_register_req.
 *
 * \param[in]  cid             Local CID. The value can be get from \ref BTIF_MSG_L2C_CONN_CMPL_INFO message.
 * \param[in]  busy            Set local to busy state or not.
 * \arg \c     true                Set local to busy state.
 * \arg \c     false               Set local to idle state.
 *
 * \return    The status of sending the request.
 * \retval    true     The request was successfully sent.
 * \retval    false    The request was failed to sent.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   btif_set_l2c_local_busy_req(p_info->cid, true);
               }

           }
           break;

       case BTIF_MSG_L2C_SET_LOCAL_BUSY_RSP:
           {
               T_BTIF_L2C_SET_LOCAL_BUSY_RSP *p_rsp = &p_msg->p.l2c_set_local_busy_rsp;
               APP_PRINT_TRACE1("Set local busy state response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_set_l2c_local_busy_req(uint16_t cid, bool busy);

/**
 * btif.h
 *
 * \brief    Select reject mode used in L2CAP ERTM mode.
 *
 * \param[in]  mode      Reject mode used in L2CAP ERTM mode. Valid values are \ref BTIF_BR_L2CAP_REJ_MODE.
 *
 * \return    The status of selecting reject mode.
 * \retval    true     Reject mode was successfully set.
 * \retval    false    Reject mode was failed to set.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   btif_set_l2c_rej_mode(BTIF_L2C_ERTM_REJ);
               }

           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_set_l2c_rej_mode(uint8_t mode);

/**
 * btif.h
 *
 * \brief    Enable/Disable FCS usage in L2CAP ERTM mode.
 *
 * \param[in]  flag      FCS usage flag in L2CAP ERTM mode.
 * \arg \c     1               Prefer to use FCS in L2CAP ERTM mode.
 * \arg \c     0               Do not use FCS in L2CAP ERTM mode.
 *
 * \return    The status of setting FCS usage.
 * \retval    true     FCS usage was successfully set.
 * \retval    false    RFCS usage was failed to set.
 *
 * <b>Example usage</b>
   \code{.c}
   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;

               if (!p_info->cause)
               {
                   btif_set_l2c_fcs(1);
               }

           }
           break;

       default:
           break;
       }
   }
   \endcode
 *
 * \ingroup  BTIF
 */
bool btif_set_l2c_fcs(uint8_t flag);

/**
 * btif.h
 *
 * \brief    Get total number for L2CAP buffer which will send \ref BTIF_MSG_L2C_DATA_RSP
 *           when released. This is mainly used for data flow control from upper
 *           layer to Bluetooth stack.
 *
 * \param[in]  None.
 *
 * \return    Total number for L2CAP buffer which will send \ref BTIF_MSG_L2C_DATA_RSP when released.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t data_quota;

   void app_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
   {
       switch (p_msg->command)
       {
       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               T_BTIF_L2C_CONN_CMPL_INFO *p_info = &p_msg->p.l2c_conn_cmpl_info;
               uint8_t i;

               if (!p_info->cause)
               {
                   for (i = 0; i < data_quota; i++)
                   {
                       // ...
                       // Get a L2CAP buffer with ack flag set to true.
                       // Fill the buffer and send it.
                       // ...
                   }
               }

           }
           break;

       case BTIF_MSG_L2C_CONN_CMPL_INFO:
           {
               // ...
               // Get a L2CAP buffer with ack flag set to true.
               // Fill the buffer and send it.
               // ...
           }
           break;

       default:
           break;
       }
   }

   void test(void)
   {
       // ...
       // Create app task.
       // Register callback function.
       // ...

       data_quota = btif_get_l2c_data_quota();
   }
   \endcode
 *
 * \ingroup  BTIF
 */
uint8_t btif_get_l2c_data_quota(void);

bool btif_lt_addr_set_req(uint8_t lt_addr);

bool btif_lt_addr_del_req(uint8_t lt_addr);

bool btif_sync_train_param_set_req(uint16_t min_interval, uint16_t max_interval,
                                   uint32_t sync_train_tout, uint8_t srv_data);

bool btif_sync_train_req(void);

bool btif_sync_train_rcv_req(uint8_t *bd_addr, uint16_t sync_scan_tout,
                             uint16_t sync_scan_window, uint16_t sync_scan_interval);

bool btif_csb_set_req(uint8_t enable, uint8_t lt_addr, uint8_t lpo_allowed, uint16_t pkt_type,
                      uint16_t min_interval, uint16_t max_interval, uint16_t csb_supv_tout);

bool btif_csb_rcv_set_req(uint8_t enable, uint8_t *bd_addr, uint8_t lt_addr, uint16_t interval,
                          uint32_t clk_offset, uint32_t next_broadcast_clk, uint16_t csb_supv_tout,
                          uint8_t remote_timing_acc, uint8_t skip, uint16_t pkt_type, uint8_t *afh_chann_map);

bool btif_csb_data_req(uint8_t lt_addr, void *p_data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif  /* _BTIF_H_ */
