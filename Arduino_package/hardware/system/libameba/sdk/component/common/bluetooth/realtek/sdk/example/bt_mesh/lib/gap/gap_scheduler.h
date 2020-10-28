/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gap_scheduler.h
  * @brief    Head file for mesh gap scheduler.
  * @details  The dedicated gap layer designed for mesh.
  * @author   bill
  * @date     2019-7-22
  * @version  v2.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef GAP_SCHEDULER_H
#define GAP_SCHEDULER_H

/* Add Includes here */
#include "platform_misc.h"
#include "platform_list.h"
#include "platform_os.h"
#include "gap.h"
#include "gap_msg.h"
#include "gap_adv.h"
#include "gap_scan.h"
#include "gap_conn_le.h"
#include "gap_callback_le.h"
#include "gatt.h"
#include "mesh_config.h"

BEGIN_DECLS

/** @addtogroup Gap_Scheduler
  * @{
  */

/** @defgroup Gap_Scheduler_Exported_Macros Exported Macros
  * @brief
  * @{
  */

#define GAP_ADTYPE_MESH_MSG                     GAP_ADTYPE_MESH_PACKET
#define GAP_ADTYPE_PB_ADV                       GAP_ADTYPE_MESH_PB_ADV

#define GAP_SCHED_ADV_PARALLEL                  1

#define GAP_SCHED_SCAN_ALL_THE_TIME             1
#define GAP_SCHED_ONE_SHOT_ADV                  1 //!< adv
#define GAP_SCHED_BT5_AE                        0
#if GAP_SCHED_BT5_AE
#ifndef F_BT_LE_5_0_AE_ADV_SUPPORT
#define F_BT_LE_5_0_AE_ADV_SUPPORT              1
#endif
#ifndef F_BT_LE_5_0_AE_SCAN_SUPPORT
#define F_BT_LE_5_0_AE_SCAN_SUPPORT             1
#endif
#include "gap_ext_adv.h"
#include "gap_ext_scan.h"
#endif
/** Advertising interval (units of 625us, 160=100ms), Value range: 0x0020 - 0x4000 (20ms - 10240ms 0.625ms/step) */
#define GAP_SCHED_ADV_INTERVAL_MIN              0x20 /* 20ms */
#define GAP_SCHED_ADV_INTERVAL_MAX              0x20 /* 20ms */

/** Scan interval and window (units of 625us, 160=100ms), Value range: 0x0004 - 0x4000 (2.5ms - 10240ms, 0.625ms/step) */
#define GAP_SCHED_SCAN_INTERVAL                 0x20 /* 20ms */
#define GAP_SCHED_SCAN_WINDOW                   0x20 /* 18.75ms */
#define GAP_SCHED_INTERWAVE_SCAN_INTERVAL       0x05 /* 3.125ms */
#define GAP_SCHED_INTERWAVE_SCAN_WINDOW         0x05 /* 3.125ms */

/** Connection interval, slave latency, supervision timeout */
#define GAP_SCHED_LE_LINK_NUM                   1
#define GAP_SCHED_CONN_INTERVAL_MIN             0x0006  /* 100 milliseconds */
#define GAP_SCHED_CONN_INTERVAL_MAX             0x0C80  /* 4 seconds */
#define GAP_SCHED_SLAVE_LATENCY                 0
#define GAP_SCHED_SUPERVISION_TIMEOUT           1000 /* unit: 10ms */
#define GAP_SCHED_CONN_SCAN_TIMEOUT             1000 /* unit: 10ms */

/** Gap scheduler parameters */
#define GAP_SCHED_TASK_NUM                      15 //!< The max number of buffered GAP task

/** @} */

/** @defgroup Gap_Scheduler_Exported_Types Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_SCHED_BT5_AE_ADV_TYPE_LEGACY_ON_1M,
    GAP_SCHED_BT5_AE_ADV_TYPE_LEGACY_ON_C2,
    GAP_SCHED_BT5_AE_ADV_TYPE_LEGACY_ON_C8,
    GAP_SCHED_BT5_AE_ADV_TYPE_EXTEND_ON_1M_1M,
    GAP_SCHED_BT5_AE_ADV_TYPE_EXTEND_ON_1M_2M,
} gap_sched_bt5_ae_adv_type_t;

typedef enum
{
    GAP_SCHED_PARAMS_DEVICE_NAME,
    GAP_SCHED_PARAMS_APPEARANCE,
    GAP_SCHED_PARAMS_TX_POWER,
    GAP_SCHED_PARAMS_BG_SCAN,
    GAP_SCHED_PARAMS_SCAN_INTERVAL,
    GAP_SCHED_PARAMS_SCAN_WINDOW,
    GAP_SCHED_PARAMS_INTERWAVE_SCAN_INTERVAL,
    GAP_SCHED_PARAMS_INTERWAVE_SCAN_WINDOW,
    GAP_SCHED_PARAMS_TASK_NUM,
    GAP_SCHED_PARAMS_BT5_AE, /**< Shall be configured before mesh_node_cfg invoked */
    GAP_SCHED_PARAMS_BT5_AE_ADV_TYPE,
} gap_sched_params_type_t;

typedef enum
{
    GAP_SCHED_TYPE_IDLE, //!< idle
    GAP_SCHED_TYPE_SCAN, //!< background scan
    GAP_SCHED_TYPE_ADV, //!< idle delay(random or zero) + one shot adv
    GAP_SCHED_TYPE_LPN_ADV, //!< one shot adv + reserved idle + scan a little while(scan maybe stopped in advance)
    GAP_SCHED_TYPE_FN_ADV //!< reserverd idle + one shot adv
} gap_sched_type_t;

typedef enum
{
    GAP_SCHED_RUN_TYPE_UNKNOWN,
    GAP_SCHED_RUN_TYPE_SWITCH,
    GAP_SCHED_RUN_TYPE_ADV_CHANGE,
    GAP_SCHED_RUN_TYPE_SCAN_CHANGE,
    GAP_SCHED_RUN_TYPE_TIMEOUT
} gap_sched_run_type_t;

typedef enum
{
    GAP_SCHED_TASK_STATE_FREE,
    GAP_SCHED_TASK_STATE_OUT,
    GAP_SCHED_TASK_STATE_ADV,
    GAP_SCHED_TASK_STATE_SCAN,
    GAP_SCHED_TASK_STATE_IDLE
} gap_sched_task_state_t;

typedef enum
{
    GAP_SCHED_STATE_IDLE,
    GAP_SCHED_STATE_START,
    GAP_SCHED_STATE_STOP,
    GAP_SCHED_STATE_ACTIVE
} gap_sched_noncon_state_t;

typedef enum
{
    GAP_SCHED_LINK_ROLE_IDLE,
    GAP_SCHED_LINK_ROLE_SLAVE,
    GAP_SCHED_LINK_ROLE_MASTER,
    GAP_SCHED_LINK_ROLE_DISCONNECTING
} gap_sched_link_role_t;

typedef struct
{
    union
    {
        uint8_t device_state;
        struct
        {
            uint8_t scan: 2;
            uint8_t adv: 2;
            uint8_t initiator: 2;
            uint8_t rfu: 2;
        };
    };
    gap_sched_link_role_t link[GAP_SCHED_LE_LINK_NUM];
} gap_sched_state_t, *gap_sched_state_p;

typedef enum
{
    GAP_SCHED_ADV_TYPE_IND = 0,
    GAP_SCHED_ADV_TYPE_DIRECT_IND_HIGH = 1,
    GAP_SCHED_ADV_TYPE_SCAN_IND = 2,
    GAP_SCHED_ADV_TYPE_NONCONN_IND = 3,
    GAP_SCHED_ADV_TYPE_DIRECT_IND_LOW = 4,
    GAP_SCHED_ADV_TYPE_UNKNOWN_IND = 0xff
} gap_sched_adv_type_t;

typedef enum
{
    GAP_SCHED_ADV_END_TYPE_SUCCESS,
    GAP_SCHED_ADV_END_TYPE_CONNECTED,
    GAP_SCHED_ADV_END_TYPE_FAIL
} gap_sched_adv_end_type_t;

typedef enum
{
    GAP_SCHED_ADV_REPORT_TYPE_UNDIRECTED = 0,
    GAP_SCHED_ADV_REPORT_TYPE_DIRECTED = 1,
    GAP_SCHED_ADV_REPORT_TYPE_SCANNABLE = 2,
    GAP_SCHED_ADV_REPORT_TYPE_NONCONNECTABLE = 3,
    GAP_SCHED_ADV_REPORT_TYPE_SCAN_RESPONSE = 4,
    GAP_SCHED_ADV_REPORT_TYPE_UNKNOWN_IND = 0xff
} gap_sched_adv_report_type_t;

typedef enum _mesh_msg_send_stat_t mesh_msg_send_stat_t;
typedef void (*gap_sched_send_cb_pf)(void *cb_params, mesh_msg_send_stat_t stat, uint32_t cb_data);
typedef struct _gap_sched_task_t
{
    struct _gap_sched_task_t *pnext;
    uint32_t idle_time;
    uint32_t scan_time; //!< 0xffffffff denotes going to stop scan
    gap_sched_type_t type;
    gap_sched_task_state_t state;
    gap_sched_send_cb_pf pf_cb;
    void *cb_params;
    uint32_t cb_data;
    uint8_t retrans_count;
    uint16_t retrans_interval; //!< ms
    gap_sched_adv_type_t adv_type;
    uint8_t adv_len;
    uint8_t adv_data[31];
} gap_sched_task_t, *gap_sched_task_p;

#define GAP_SCHED_LINK_INVALID     0xff
#define GAP_SCHED_LINK_ALL         0xfe
typedef uint8_t gap_sched_link_t;

typedef void (*gap_sched_link_disc_cb_t)(gap_sched_link_t link);
typedef struct _gap_sched_link_disc_cb_e_t
{
    struct _gap_sched_link_disc_cb_e_t *pnext;
    gap_sched_link_disc_cb_t disc_cb;
} gap_sched_link_disc_cb_e_t;

typedef struct _mesh_inner_msg_t mesh_inner_msg_t;

/** @} */

/** @defgroup Gap_Scheduler_Exported_Functions Exported Functions
  * @brief
  * @{
  */

///@cond
void gap_sched_cfg(void);
void gap_sched_init(void);
void gap_sched_deinit(void);
void gap_sched_task_free(void *pbuffer);
void gap_sched_try(gap_sched_task_t *ptask);
void gap_sched_quit(gap_sched_task_t *ptask);
bool gap_sched_disc_cb_reg(gap_sched_link_disc_cb_t pf);
void gap_sched_timer_timeout(void *ptimer);
#if GAP_SCHED_ONE_SHOT_ADV
void gap_sched_adv_params_set_done(void);
void gap_sched_adv_done(gap_sched_adv_end_type_t type);
#else
#define gap_sched_adv_params_set_done(...)
#define gap_sched_adv_done(...)
#endif
///@endcond

/**
  * @brief set the parameters of gap scheduler
  *
  * @param[in] param_type: the supported parameter type
  * @param[in] pvalue: the parameter value pointer
  * @param[in] len: the parameter value length
  * @return operation result
  * @retval true: success
  * @retval false: fail
  */
bool gap_sched_params_set(gap_sched_params_type_t param_type, void *pvalue, uint16_t len);

/**
  * @brief get the parameters of gap scheduler
  *
  * @param[in] param_type: the supported parameter type
  * @param[out] pvalue: the parameter value pointer
  * @param[in] len: the parameter value length
  * @return operation result
  * @retval true: success
  * @retval false: fail
  */
bool gap_sched_params_get(gap_sched_params_type_t param_type, void *pvalue, uint16_t len);

/**
  * @brief get the advertising buffer from the gap scheduler
  *
  * All advertising must be sent by the gap scheduler, and shall be allocated with the buffer first.
  *
  * @return the advertising buffer
  * @retval 0: allocate fail due to the limited resource
  */
void *gap_sched_task_get(void);

/**
  * @brief enable/disable scan of the gap scheduler
  *
  * The scan is background.
  *
  * @param[in] on_off: the scan state, true - on, false - off
  * @return none
  */
void gap_sched_scan(bool on_off);

/**
  * @brief handle the bt status message
  *
  * The gap sheduler need process the bt messages.
  *
  * @param[in] pmsg: the bt status msg
  * @return none
  */
void gap_sched_handle_bt_status_msg(mesh_inner_msg_t *pmsg);

/**
  * @brief handle the adv report
  *
  * The gap sheduler need process the adv report.
  *
  * @param[in] ple_scan_info: the adv report
  * @return none
  */
void gap_sched_handle_adv_report(T_LE_SCAN_INFO *ple_scan_info);

/**
  * @brief check if there is link related activities
  *
  * To avoid complicated link situation, link activities contains
  * try to connect or being connected.
  * @param[in] ple_scan_info: the adv report
  * @return the check result
  * @retval true: yes
  * @retval false: no
  */
bool gap_sched_link_check(void);

#if GAP_SCHED_BT5_AE
/**
  * @brief dump all adv set state
  * @return none
  */
void gap_sched_ext_adv_state_dump(void);

/**
  * @brief process the extended adv procedure
  * @param[in] handle: the extended adv handle
  * @return the result
  */
T_GAP_CAUSE gap_sched_ext_adv_step(uint8_t handle);

/**
  * @brief process the extended adv state change event
  * @param[in] handle: the extended adv handle
  * @param[in] new_state: the new state of the extended adv set
  * @param[in] cause: the cause the state change
  * @return none
  */
void gap_sched_handle_ext_adv_state_evt(uint8_t handle, T_GAP_EXT_ADV_STATE new_state,
                                        uint16_t cause);

/**
  * @brief process the extended adv state change event
  * @param[in] pext_adv_report: the extended adv report
  * @return none
  */
void gap_sched_handle_ext_adv_report(T_LE_EXT_ADV_REPORT_INFO *pext_adv_report);

/**
  * @brief start the extended adv without worry about collision with other adv set
  * @param[in] handle: the extended adv handle
  * @return the result
  */
T_GAP_CAUSE gap_sched_ext_adv_start(uint8_t handle);

/**
  * @brief stop the extended adv without worry about collision with other adv set
  * @param[in] handle: the extended adv handle
  * @return the result
  */
T_GAP_CAUSE gap_sched_ext_adv_stop(uint8_t handle);

/**
  * @brief set the extend adv parameter flag to enable the setting flow
  * @param[in] handle: the extended adv handle
  * @param[in] param_flag: the adv setting parameters flag @ref EXT_ADV_PARAM
  * @return the result
  */
bool gap_sched_ext_adv_set_param_flag(uint8_t handle, uint8_t param_flag);

#endif

/** @} */
/** @} */

END_DECLS

#endif /* GAP_SCHEDULER_H */
