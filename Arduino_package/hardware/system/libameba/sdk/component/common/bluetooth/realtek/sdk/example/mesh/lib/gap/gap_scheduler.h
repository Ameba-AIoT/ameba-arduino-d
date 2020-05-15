/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gap_scheduler.h
  * @brief    Head file for mesh gap scheduler.
  * @details  The dedicated gap layer designed for mesh.
  * @author   bill
  * @date     2017-5-3
  * @version  v1.0
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

#define GAP_SCHED_ONE_SHOT_ADV                  1 //!< adv
/** Advertising interval (units of 625us, 160=100ms), Value range: 0x0020 - 0x4000 (20ms - 10240ms 0.625ms/step) */
#define GAP_SCHED_ADV_INTERVAL_MIN              0xA0 /* 100ms */
#define GAP_SCHED_ADV_INTERVAL_MAX              0xA0 /* 100ms */

/** Scan interval and window (units of 625us, 160=100ms), Value range: 0x0004 - 0x4000 (2.5ms - 10240ms, 0.625ms/step) */
#define GAP_SCHED_SCAN_INTERVAL                 0x20 /* 20ms */
#define GAP_SCHED_SCAN_WINDOW                   0x20 /* 18.75ms */

/** Connection interval, slave latency, supervision timeout */
#define GAP_SCHED_LE_LINK_NUM                   1
#define GAP_SCHED_CONN_INTERVAL_MIN             0x0006  /* 100 milliseconds */
#define GAP_SCHED_CONN_INTERVAL_MAX             0x0C80  /* 4 seconds */
#define GAP_SCHED_SLAVE_LATENCY                 0
#define GAP_SCHED_SUPERVISION_TIMEOUT           1000 /* unit: 10ms */
#define GAP_SCHED_CONN_SCAN_TIMEOUT             1000 /* unit: 10ms */

/** @} */

/** @defgroup Gap_Scheduler_Exported_Types Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_SCHED_PARAMS_DEVICE_NAME,
    GAP_SCHED_PARAMS_APPEARANCE,
    GAP_SCHED_PARAMS_TX_POWER,
    GAP_SCHED_PARAMS_SCAN_INTERVAL,
    GAP_SCHED_PARAMS_SCAN_WINDOW
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
typedef uint8_t gap_sched_link_t;

typedef void (*gap_sched_link_disc_cb_t)(gap_sched_link_t link);
typedef struct _gap_sched_link_disc_cb_e_t
{
    struct _gap_sched_link_disc_cb_e_t *pnext;
    gap_sched_link_disc_cb_t disc_cb;
} gap_sched_link_disc_cb_e_t;

typedef struct
{
    gap_sched_task_p ptask_cur;
    gap_sched_task_t *task; //!< task[]
    plt_list_t task_queue_free;
    plt_list_t task_queue_adv;
    plt_list_t task_queue_adv1; //!< high priority: fn_adv or lpn_adv
    plt_timer_t timer;
    bool timer_flag;
    bool bg_scan;
    bool scan_modify;
    uint16_t scan_interval;
    uint16_t scan_window;
    uint8_t adv_times;
    uint8_t adv_count;
    bool quit;
    gap_sched_state_t state;
    plt_list_t link_disc_cb_q; //!< gap_sched_link_disc_cb_e_t
} gap_sched_t, *gap_sched_p;

typedef struct _mesh_inner_msg_t mesh_inner_msg_t;

extern gap_sched_t gap_scheduler;

/** @} */

/** @defgroup Gap_Scheduler_Exported_Functions Exported Functions
  * @brief
  * @{
  */

///@cond
void gap_sched_init(void);
void gap_sched_run(gap_sched_run_type_t run_type);
void gap_sched_task_free(void *pbuffer);
void gap_sched_try(gap_sched_task_p ptask);
void gap_sched_quit(void);
bool gap_sched_disc_cb_reg(gap_sched_link_disc_cb_t pf);
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

/** @} */
/** @} */

END_DECLS

#endif /* GAP_SCHEDULER_H */
