/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_common.h
  * @brief    Head file for mesh common part.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-9-14
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_COMMON_H
#define _MESH_COMMON_H

/* Add Includes here */
#include "mesh_api.h"

BEGIN_DECLS

/** @addtogroup Mesh_Common
  * @{
  */

/** @defgroup Mesh_Common_Exported_Types Exported Types
  * @{
  */

/** @brief Timers used by the mesh stack */
typedef enum
{
    MESH_IV_INDEX_TIMER_ID,
    MESH_SCAN_TIMER_ID,
    MESH_ADV_TIMER_ID,
    MESH_FORWORD_TABLE_REFRESH_TIMER_ID,
    MESH_BEACON_TIMER_ID,
    MESH_SERVICE_TIMER_ID,
    MESH_SERVICE_ID_TIMER_ID,
    MESH_PROVISIONING_TIMER_ID,
    MESH_ATTN_TIMER_ID,
    MESH_PB_ADV_TIMER_ID,
    MESH_TRANS_TX_TIMER_ID,
    MESH_TRANS_RX_TIMER_ID,
    MESH_TRANS_RX_INCOMPLETE_TIMER_ID,
    MESH_FRIENDSHIP_TIMER_ID,
    MESH_PB_MESH_TIMER_ID,
    MESH_HB_TIMER_ID,
    MESH_TICK_TIMER_ID
} mesh_timer_id_t;

typedef enum
{
    MESH_REBOOT,
    MESH_BT_STATUS_UPDATE,
    MESH_GAP_SCHEDULER_TIMEOUT,
    MESH_IV_TIMEOUT,
    MESH_BEACON_TIMEOUT,
    MESH_SERVICE_TIMEOUT,
    MESH_SERVICE_ID_TIMEOUT,
    MESH_PROVISIONING_TIMEOUT,
    MESH_ATTN_TIMEOUT,
    MESH_PB_ADV_TIMEOUT,
    MESH_TRANS_TRX_TIMEOUT,
    MESH_GAP_SCHEDULER_TASK_AVAILABLE,
    MESH_GAP_SCHEDULER_TASK_FREE,
    MESH_FN_TIMEOUT,
    MESH_LPN_TIMEOUT,
    MESH_RMT_PRO_TIMEOUT,
    MESH_PROV_SRV_PDU_TX_DONE,
    MESH_PROXY_CTX_RX_TIMEOUT,
    MESH_HB_TIMEOUT,
    MESH_MODEL_PUB_TIMEOUT,
    MESH_BEARER_LOCAL_LOOPBACK,
    MESH_TICK_TIMEOUT,
    MESH_MODEL_DELAY_EXECUTION_TIMEOUT,
} mesh_inner_msg_type_t;

typedef enum
{
    MESH_UNKNOWN,
    MESH_PROVED,
    MESH_NODE_RESET,
    MESH_OTA
} mesh_reboot_reason_t;

typedef struct _mesh_inner_msg_t
{
    uint16_t type; //!< @ref mesh_inner_msg_type_t
    uint16_t sub_type;
    union
    {
        uint32_t parm;
        void *pbuf;
    };
} mesh_inner_msg_t;

/*
 * udb/provision adv/proxy adv callback
 */
typedef enum
{
    DEVICE_INFO_UDB,
    DEVICE_INFO_PROV_ADV,
    DEVICE_INFO_PROXY_ADV
} device_info_type_t;

typedef struct
{
    device_info_type_t type;
    uint16_t len;
    union
    {
        beacon_udb_t *pbeacon_udb;
        mesh_service_data_t *pservice_data;
    };
} device_info_t;

typedef void (*device_info_cb_pf)(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi,
                                  device_info_t *pinfo);
extern device_info_cb_pf pf_device_info_cb;

typedef void (*tick_timeout_cb)(void);

typedef void (*model_delay_execution_timeout_cb)(void *pargs);

/** @} */

/** @defgroup Mesh_Common_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief reg the callback to indicate the information of neighbor mesh devices
  * @param[in] cb: callback
  * @return none
  */
void device_info_cb_reg(device_info_cb_pf cb);

/**
  * @brief init the mesh stack
  * @return none
  */
void mesh_init(void);

/**
  * @brief start the mesh stack
  * @param[in] event_mesh: the event type of the mesh inner msg
  * @param[in] event_app: the event type sent to app by the mesh stack
  * @param[in] event_queue: the event queue to send event
  * @param[in] app_queue: the app queue to send message
  * @return none
  */
void mesh_start(uint8_t event_mesh, uint8_t event_app, void *event_queue, void *app_queue);

///@cond
void mesh_run(void);
///@endcond

/**
  * @brief deinit the mesh stack
  *
  * release all the resources and reset the global variables
  * @return none
  */
void mesh_deinit(void);

/**
  * @brief reboot with delay
  * @param[in] reason: reboot reason
  * @param[in] delay_ms: if it is 0, reboot right now
  * @return none
  */
void mesh_reboot(mesh_reboot_reason_t reason, uint32_t delay_ms);

/**
  * @brief send the mesh inner msg
  * @param[in] pmsg: message
  * @return operation result
  */
bool mesh_inner_msg_send(mesh_inner_msg_t *pmsg);

/**
  * @brief handle the mesh inner message
  * @param[in] event: mesh event
  * @return none
  */
void mesh_inner_msg_handle(uint8_t event);

/**
  * @brief start the tick timer
  *
  * The mesh stack provides a gerneral tick timer
  * @param[in] tick_ms: tick
  * @param[in] tick_cb: timeout callback
  * @return none
  */
void mesh_tick_timer_start(uint32_t tick_ms, tick_timeout_cb tick_cb);

/**
  * @brief stop the tick timer
  * @return none
  */
void mesh_tick_timer_stop(void);

/**
  * @brief get the tick timer state
  * @return tick timer state
  * @retval true: running
  * @retval false: not running
  */
bool mesh_tick_timer_is_running(void);

/**
 * @brief initialize model delay execution
 * @param[in] delay_cb: delay execution timeout callback function
 * @return none
 */
void mesh_model_delay_execution_init(model_delay_execution_timeout_cb delay_cb);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_COMMON_H */

