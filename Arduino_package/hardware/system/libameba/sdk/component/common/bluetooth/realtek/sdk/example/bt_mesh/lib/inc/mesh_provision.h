/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_provision.h
  * @brief    Head file for mesh provision.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_PROVISION_H
#define _MESH_PROVISION_H

/* Add Includes here */
#include "platform_types.h"
#include "platform_os.h"
#include "mesh_bearer.h"
#include "provision_adv.h"

BEGIN_DECLS

/** @addtogroup Mesh_Prov_Common
  * @{
  */

/** @defgroup Mesh_Prov_Common_Exported_Types Exported Types
  * @brief
  * @{
  */

/** @defgroup Provision_Parameters
  * @brief  parameters used by upper layers
  * @{
  */
typedef enum
{
    PROV_PARAMS_CAPABILITIES, //!< @ref prov_capabilities_t
    PROV_PARAMS_OOB_INFO, //!< @ref prov_oob_info_t
    PROV_PARAMS_CALLBACK_FUN, //!< @ref prov_cb_pf
    PROV_PARAMS_PUBLIC_KEY, //!< set by device when provisioning
    PROV_PARAMS_PRIVATE_KEY //!< set by device when provisioning
} prov_params_t, *PProvParams;

enum
{
    PROV_OOB_INFO_OTHER              = BIT0,
    PROV_OOB_INFO_ELECTRONIC_URI     = BIT1,
    PROV_OOB_INFO_2D_CODE            = BIT2,
    PROV_OOB_INFO_BAR_CODE           = BIT3,
    PROV_OOB_INFO_NFC                = BIT4,
    PROV_OOB_INFO_NUMBER             = BIT5,
    PROV_OOB_INFO_STRING             = BIT6,
    PROV_OOB_INFO_ON_BOX             = BIT11,
    PROV_OOB_INFO_INSIDE_BOX         = BIT12,
    PROV_OOB_INFO_ON_PAPER           = BIT13,
    PROV_OOB_INFO_INSIDE_MANUAL      = BIT14,
    PROV_OOB_INFO_ON_DEVICE          = BIT15
};

typedef struct
{
    uint16_t other: 1;
    uint16_t electronic_uri: 1;
    uint16_t qr_code: 1; //!< 2D machine-readable code
    uint16_t bar_code: 1;
    uint16_t nfc: 1; //!< Near Field Communication (NFC)
    uint16_t number: 1;
    uint16_t string: 1;
    uint16_t rfu: 4;
    uint16_t on_box: 1;
    uint16_t inside_box: 1;
    uint16_t on_paper: 1;
    uint16_t inside_manual: 1;
    uint16_t on_device: 1;
} _PACKED_ prov_oob_info_t, *prov_oob_info_p;
/** @} End of Provision_Parameters */

/** @defgroup Provision_PDU Provision PDU
  * @brief  PDU used for provisioning procedure
  * @{
  */
enum
{
    PROV_INVITE = 0,
    PROV_CAPABILITIES,
    PROV_START,
    PROV_PUBLIC_KEY,
    PROV_INPUT_COMPLETE,
    PROV_CONFIRMATION,
    PROV_RANDOM,
    PROV_DATA,
    PROV_COMPLETE,
    PROV_FAILED,
#if MESH_UNPROVISIONING_SUPPORT
    UNPROV_CMD,
    UNPROV_COMPLETE
#endif
} _SHORT_ENUM_;
typedef uint8_t prov_pdu_type_t;

typedef struct
{
    uint8_t attn_dur;
} _PACKED_ prov_invite_t, *prov_invite_p;

typedef enum
{
    PROV_CAP_ALGO_FIPS_P256_ELLIPTIC_CURVE = BIT0
} prov_cap_algorithm_t;

enum
{
    PROV_START_FIPS_P256_ELLIPTIC_CURVE
} _SHORT_ENUM_;
typedef uint8_t prov_start_algorithm_t;

typedef enum
{
    PROV_CAP_PUBLIC_KEY_OOB = BIT0
} prov_cap_public_key_t;

enum
{
    PROV_START_NO_OOB_PUBLIC_KEY,
    PROV_START_OOB_PUBLIC_KEY
} _SHORT_ENUM_;
typedef uint8_t prov_start_public_key_t;

typedef enum
{
    PROV_CAP_STATIC_OOB = BIT0
} prov_cap_static_oob_t;

#define OUTPUT_OOB_SIZE_MAX     8
typedef enum
{
    PROV_CAP_OUTPUT_OOB_ACTION_BLINK = BIT0,
    PROV_CAP_OUTPUT_OOB_ACTION_BEEP = BIT1,
    PROV_CAP_OUTPUT_OOB_ACTION_VIBRATE = BIT2,
    PROV_CAP_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC = BIT3,
    PROV_CAP_OUTPUT_OOB_ACTION_OUTPUT_ALPHANUMERIC = BIT4
} prov_cap_output_oob_action_t;

enum
{
    PROV_START_OUTPUT_OOB_ACTION_BLINK,
    PROV_START_OUTPUT_OOB_ACTION_BEEP,
    PROV_START_OUTPUT_OOB_ACTION_VIBRATE,
    PROV_START_OUTPUT_OOB_ACTION_OUTPUT_NUMERIC,
    PROV_START_OUTPUT_OOB_ACTION_OUTPUT_ALPHANUMERIC
} _SHORT_ENUM_;
typedef uint8_t prov_start_output_oob_action_t;

#define INPUT_OOB_SIZE_MAX     8
typedef enum
{
    PROV_CAP_INPUT_OOB_ACTION_BIT_PUSH = BIT0,
    PROV_CAP_INPUT_OOB_ACTION_BIT_TWIST = BIT1,
    PROV_CAP_INPUT_OOB_ACTION_BIT_INPUT_NUMERIC = BIT2,
    PROV_CAP_INPUT_OOB_ACTION_BIT_INPUT_ALPHANUMERIC = BIT3
} prov_cap_input_oob_action_t;

enum
{
    PROV_START_INPUT_OOB_ACTION_PUSH,
    PROV_START_INPUT_OOB_ACTION_TWIST,
    PROV_START_INPUT_OOB_ACTION_INPUT_NUMERIC,
    PROV_START_INPUT_OOB_ACTION_INPUT_ALPHANUMERIC
} _SHORT_ENUM_;
typedef uint8_t prov_start_input_oob_action_t;

typedef struct
{
    uint8_t element_num;
    uint16_t algorithm; //!< @ref prov_cap_algorithm_t
    uint8_t public_key; //!< @ref prov_cap_public_key_t
    uint8_t static_oob; //!< @ref prov_cap_static_oob_t
    uint8_t output_oob_size; //!< @ref OUTPUT_OOB_SIZE_MAX
    uint16_t output_oob_action; //!< @ref prov_cap_output_oob_action_t
    uint8_t input_oob_size; //!< @ref INPUT_OOB_SIZE_MAX
    uint16_t input_oob_action; //!< @ref prov_cap_input_oob_action_t
} _PACKED_ prov_capabilities_t, *prov_capabilities_p;

enum
{
    PROV_AUTH_METHOD_NO_OOB,
    PROV_AUTH_METHOD_STATIC_OOB,
    PROV_AUTH_METHOD_OUTPUT_OOB,
    PROV_AUTH_METHOD_INPUT_OOB
} _SHORT_ENUM_;
typedef uint8_t prov_auth_method_t;

typedef enum
{
    PROV_AUTH_VALUE_TYPE_NONE,
    PROV_AUTH_VALUE_TYPE_BINARY,
    PROV_AUTH_VALUE_TYPE_NUMERIC,
    PROV_AUTH_VALUE_TYPE_ALPHANUMERIC
} prov_auth_value_type_t;

typedef union
{
    uint8_t oob_action;
    prov_start_output_oob_action_t output_oob_action;
    prov_start_input_oob_action_t input_oob_action;
} _PACKED_ prov_auth_action_t;

typedef union
{
    uint8_t oob_size;
    uint8_t output_oob_size;
    uint8_t input_oob_size;
} _PACKED_ prov_auth_size_t;

typedef struct
{
    prov_start_algorithm_t algorithm;
    prov_start_public_key_t public_key;
    prov_auth_method_t auth_method;
    prov_auth_action_t auth_action;
    prov_auth_size_t auth_size;
} _PACKED_ prov_start_t, *prov_start_p;

typedef struct
{
    uint8_t public_key[64];
} _PACKED_ prov_public_key_t, *prov_public_key_p;

typedef struct
{
    uint8_t confirmation[16];
} _PACKED_ prov_confirmation_t, *prov_confirmation_p;

typedef struct
{
    uint8_t rand[16];
} _PACKED_ prov_random_t, *prov_random_p;

typedef struct
{
    uint8_t net_key[16];
    uint16_t net_key_index;
    uint8_t flags; //!< bit0: key refresh, bit1: iv update
    uint32_t iv_index;
    uint16_t unicast_address;
    uint8_t mic[8];
} _PACKED_ prov_data_t, *prov_data_p; //!< big endian

enum
{
    PROV_VALID,
    PROV_FAILED_INVALID_PDU = 0x01,
    PROV_FAILED_INVALID_FORMAT,
    PROV_FAILED_UNEXPECTED_PDU,
    PROV_FAILED_CONFIRMATION_FAILED,
    PROV_FAILED_OUT_OF_RESOURCE,
    PROV_FAILED_DECRYPTION_FAILED,
    PROV_FAILED_UNEXPECTED_ERROR,
    PROV_FAILED_CANNOT_ASSIGN_ADDR,
    /* The Data block contains values that cannot be accepted. (Used by RPR) */
    PROV_FAILED_INVALID_DATA
} _SHORT_ENUM_;
typedef uint8_t prov_result_t;

typedef struct
{
    prov_result_t failed_reason;
} _PACKED_ prov_failed_t, *prov_failed_p;

typedef struct
{
    uint8_t type : 6; //!< @ref prov_pdu_type_t
    uint8_t padding : 2;
    union
    {
        prov_invite_t prov_invite;
        prov_capabilities_t prov_capabilities;
        prov_start_t prov_start;
        prov_public_key_t prov_public_key;
        prov_confirmation_t prov_confirmation;
        prov_random_t prov_random;
        prov_data_t prov_data;
        prov_failed_t prov_failed;
    };
} _PACKED_ prov_pdu_t, *prov_pdu_p;
/** @} End of Provision_PDU */

#define CONFIRMATION_INPUTS_LENGTH           (sizeof(prov_invite_t) + sizeof(prov_capabilities_t) + sizeof(prov_start_t) + sizeof(prov_public_key_t) + sizeof(prov_public_key_t))
typedef struct
{
    uint8_t public_key[64];
    uint8_t private_key[32];
    uint8_t random[16];
    uint8_t ecdh_secrect[32];
    uint8_t conf[16];
    uint8_t conf_salt[16];
    uint8_t conf_key[16];
    uint8_t prov_salt[16];
    uint8_t conf_inputs[CONFIRMATION_INPUTS_LENGTH];
    uint8_t auth_value[16];
    bool auth_value_flag;
    bool confirm_rx_flag;
    prov_start_t prov_start;
} prov_ctx_tmp_t, *prov_ctx_tmp_p;

/** @defgroup Provision_Callback Provision Callback
  * @brief  Callback types used for provisioning procedure.
  * @{
  */
typedef enum
{
    PROV_CB_TYPE_PB_ADV_LINK_STATE,
    PROV_CB_TYPE_UNPROV, /**< notified when the stack is ready */
    PROV_CB_TYPE_START, /**< used by device */
    PROV_CB_TYPE_PATH_CHOOSE, /**< used by provisioner */
    PROV_CB_TYPE_PUBLIC_KEY,
    PROV_CB_TYPE_AUTH_DATA,
    PROV_CB_TYPE_RANDOM, /* notified when receive remote random value */
    PROV_CB_TYPE_COMPLETE, /**< only notified in the prov procedure */
    PROV_CB_TYPE_FAIL,
    PROV_CB_TYPE_PROV, /**< added to notify the stack is ready */
#if defined(MESH_PROV_WO_AUTH_VALUE) && MESH_PROV_WO_AUTH_VALUE
    PROV_CB_TYPE_CONF_CHECK,
#endif
} prov_cb_type_t;

typedef enum
{
    PROV_CB_FAIL_TYPE_BEARER_LOSS, //!< pb-adv link close & le link disconnect
    PROV_CB_FAIL_TYPE_BEARER_TIMEOUT, //!< pb-adv transaction timeout @ref MESH_PB_GENERIC_TIMEOUT_PERIOD
    PROV_CB_FAIL_TYPE_BEARER_IDLE, //!< pb-adv link idle timeout @ref MESH_PB_ADV_LINK_IDLE_TIMEOUT
    PROV_CB_FAIL_TYPE_PROCEDURE_TIMEOUT, //!< prov procedure timeout @ref MESH_PROV_TIMEOUT_PERIOD
    PROV_CB_FAIL_TYPE_PROCEDURE_ABNORMAL //!< prov procedure fail @ref prov_result_t
} prov_cb_fail_type_t;

typedef struct
{
    prov_cb_fail_type_t fail_type;
    union
    {
        bearer_type_t bearer_type;
        prov_result_t prov_result;
    };
} prov_cb_fail_t;

#if defined(MESH_PROV_WO_AUTH_VALUE) && MESH_PROV_WO_AUTH_VALUE
typedef struct
{
    uint8_t *rand;
    uint8_t *conf;
} prov_check_conf_t;
#endif

/** little endian interfaced with app */
typedef union
{
    prov_generic_cb_type_t pb_generic_cb_type; //!< used in PROV_CB_TYPE_PB_ADV_LINK_STATE
    prov_capabilities_t *pprov_capabilities; //!< used in PROV_CB_TYPE_PATH_CHOOSE by provisioner
    prov_start_t *pprov_start; //!< used in PROV_CB_TYPE_AUTH_DATA by device
    prov_data_t *pprov_data; //!< used in PROV_CB_TYPE_COMPLETE
    prov_random_t *pprov_random; //!< used in PROV_CB_TYPE_RANDOM by device and provisioner
    prov_cb_fail_t prov_fail; //!< used in PROV_CB_TYPE_FAIL
#if defined(MESH_PROV_WO_AUTH_VALUE) && MESH_PROV_WO_AUTH_VALUE
    prov_check_conf_t prov_check_conf;
#endif
} prov_cb_data_t;

typedef bool (*prov_cb_pf)(prov_cb_type_t cb_type, prov_cb_data_t cb_data);
typedef bool (*prov_send_t)(uint8_t *pdata, uint16_t len);

typedef struct
{
    prov_pdu_type_t fsm;
    bool fsm_flag; //!< TRUE: tx, False: Rx. Assisting to determine the FSM when there is ambiguity tx/rx the same msgs.
    prov_capabilities_t prov_cap;
    prov_cb_pf pf_prov_cb;
    plt_timer_t timer;
    prov_ctx_tmp_p pctx_tmp;
    prov_send_t prov_send;
} prov_ctx_t, *prov_ctx_p;

extern prov_ctx_t prov_ctx;
/** @} */

/** @} */

/** @defgroup Mesh_Prov_Common_Exported_Functions Exported Functions
  * @brief
  * @{
  */

///@cond
void prov_init(void);
uint32_t prov_cap_check(void);
uint32_t prov_start_check(void);
void prov_timer_restart(void);
void prov_timer_stop(void);
void prov_allocate(void);
void prov_free(void);
bool prov_ecc_key_gen(void);
bool prov_ecc_key_validate(uint8_t public_key[64]);
bool prov_ecdh_secret_gen(uint8_t public_key[64], uint8_t private_key[32], uint8_t secret[32]);
void prov_conf_keys_gen(void);
bool prov_conf_key_get(uint8_t conf_key[16]);
void prov_confirmation_gen(uint8_t confirmation[16], uint8_t rand[16], uint8_t auth_value[16]);
void prov_data_keys_gen(uint8_t random_provisioner[16], uint8_t random_device[16]);
bool prov_data_crypto(uint8_t dev_key[16], uint8_t data[38], bool encrypt_decrypt);
void prov_handle_disconnect(void);
void prov_handle_timeout(void);
void prov_set_send_cb(prov_send_t pcb);
uint16_t prov_dev_key_idx(uint16_t addr);
bool prov_replace(uint16_t old_idx, uint16_t new_idx);
///@endcond

/**
  * @brief get the provision parameters
  * @param[in] params: type
  * @param[in] pvalue: value
  * @param[in] len: length
  * @return operation result
  */
bool prov_params_get(prov_params_t params, void *pvalue, uint8_t len);

/**
  * @brief set the provision parameters
  * @param[in] params: type
  * @param[in] pvalue: value
  * @param[in] len: length
  * @return operation result
  */
bool prov_params_set(prov_params_t params, void *pvalue, uint8_t len);

/**
  * @brief set the auth value
  *
  * The function shall be called at the appropriate time. The auth value shall be
  * set with the appropriate length and data format.
  * @param[in] pvalue: auth value
  * @param[in] len: value length
  * @return operation result
  */
bool prov_auth_value_set(uint8_t *pvalue, uint8_t len);

/**
  * @brief change the auth value
  *
  * The function shall be called at the appropriate time. The auth value will changed immediately
  * @param[in] pvalue: auth value
  * @param[in] len: value length
  * @return operation result
  */
bool prov_auth_value_change(uint8_t *pvalue, uint8_t len);

/**
  * @brief set the auth random value
  *
  * The function shall be called at the appropriate time.
  * @param[in] random: random value
  * @return operation result
  */
bool prov_auth_random_set(uint8_t random[16]);

/**
  * @brief get the auth value type
  *
  * Different type of auth value is formatted differently according to the spec.
  * @param[in] pprov_start: prov start pdu
  * @return auth value type
  */
prov_auth_value_type_t prov_auth_value_type_get(prov_start_t *pprov_start);

/**
  * @brief disconnect the prov bearer
  *
  * The spec requires the provisioner to disconnect the bearer after the provision procedure.
  * The mesh stack leaves the app to decide whether to disconnect at the case @ref PROV_CB_TYPE_COMPLETE.
  * @param[in] reason: pb-adv bearer need the disconnect reason
  * @return operation result
  */
bool prov_disconnect(pb_adv_link_close_reason_t reason);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_PROVISION_H */

