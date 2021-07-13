/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     heartbeat.h
  * @brief    Head file for heartbeat.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-7-17
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H

/* Add Includes here */
#include "mesh_access.h"

BEGIN_DECLS

/** @addtogroup HB HB
  * @{
  */

/** @defgroup HB_Exported_Macros Exported Macros
  * @{
  */
#define HB_COUNT_LOG_NOT_PERIODICALLY       0
#define HB_COUNT_LOG_MAX                    0x11
#define HB_COUNT_LOG_INDEFINITELY           0xFF
#define HB_COUNT_INDEFINITELY               0xFFFF
#define HB_PERIOD_LOG_MAX                   0x11
#define HB_TTL_MAX                          0x7F

#define HB_TIMER_PUB                        0
#define HB_TIMER_SUB                        1
/** @} */

/** @defgroup HB_Exported_Types Exported Types
  * @{
  */
typedef enum
{
    HB_TRIGGER_RELAY,
    HB_TRIGGER_PROXY,
    HB_TRIGGER_FN,
    HB_TRIGGER_LPN,
    HB_TRIGGER_ALL
} hb_trigger_type_t;

typedef union
{
    uint16_t raw;
    struct
    {
        uint16_t relay: 1;
        uint16_t proxy: 1;
        uint16_t frnd: 1;
        uint16_t lpn: 1;
        uint16_t rfu: 12;
    };
} _PACKED_ hb_pub_features_t;

typedef struct
{
    /**
     * @brief publish destination address
     */
    uint16_t dst;
    /**
     * @brief publish count, value range is 0-0xffff
     * 0: heartbeat messages are not being send periodically
     * 1-0xfffe: number of heartbeat messages that remain to be sent
     * 0xffff: heartbeat messages are being sent indefinitely
     */
    uint16_t count;
    /**
     * @brief publish period, value range is 0-0xffff, unit is second
     * 0: heartbeat messages are not being sent periodically
     * 1-0xffff: period in seconds for sending periodical heartbeat messages
     */
    uint16_t period;
    /**
     * @brief publish initialize ttl
     */
    uint8_t ttl;
    /**
     * @brief publish trigger feature
     */
    hb_pub_features_t features;
    /**
     * @brief net key index that publish use
     */
    uint16_t net_key_index;
} hb_pub_t;

typedef struct
{
    /**
     * @brief source address for heartbeat messages a node shall process
     */
    uint16_t src;
    /**
     * @brief determines the destination address for heartbeat messages
     */
    uint16_t dst;
    /**
     * @brief preiod for processing periodical heartbeat transport control messages
     * 0: heartbeat messages are not being processed
     * 1-0xffff: remaining period in seconds for processing periodical heartbeat message
     */
    uint16_t period;
    /**
     * @brief number of heartbeat messages that received
     * 0-0xfffe: number of heartbeat messages received
     * 0xffff: more than 0xfffe messages have been received
     */
    uint16_t count;
    /**
     * @brief minimum hops in received heartbeat messages
     */
    uint8_t min_hops;
    /**
     * @brief maximum hops in received heartbeat messages
     */
    uint8_t max_hops;
} hb_sub_t;
/** @} */

/** @defgroup HB_DATA_CALLBACKS Data Callbacks
  * @{
  */
typedef enum
{
    HB_DATA_PUB_TIMER_STATE, //!< @ref hb_data_timer_state_t
    HB_DATA_SUB_TIMER_STATE, //!< @ref hb_data_timer_state_t
    HB_DATA_PUB_COUNT_UPDATE, //!< @ref hb_data_pub_count_update_t
    HB_DATA_SUB_PERIOD_UPDATE, //!< @ref hb_data_sub_period_update_t
    HB_DATA_SUB_RECEIVE, //!< @ref hb_data_sub_receive_t
} hb_data_type_t;

typedef enum
{
    HB_TIMER_STATE_START,
    HB_TIMER_STATE_STOP
} hb_timer_state_t;

typedef struct
{
    hb_timer_state_t state;
    uint16_t period;
} hb_data_timer_state_t;

typedef struct
{
    uint16_t count;
} hb_data_pub_count_update_t;

typedef struct
{
    uint16_t period;
} hb_data_sub_period_update_t;

typedef struct
{
    uint16_t src;
    uint8_t init_ttl;
    hb_pub_features_t features;
    uint8_t ttl;
} hb_data_sub_receive_t;

typedef void (*hb_data_cb)(hb_data_type_t type, void *pargs);
/** @} */

/** @defgroup HB_Exported_Functions Exported Functions
  * @{
  */

/**
 * @brief initialize heartbeat module
 * @param[in] pcb: heartbeat callback function
 */
bool hb_init(hb_data_cb pcb);

///@cond
void hb_run(void);
///@endcond

/**
 * @brief deinitialize heartbeat module
 */
void hb_deinit(void);

/**
  * @brief start the heartbeat timer
  * @param[in] timer: HB_TIMER_PUB or HB_TIMER_SUB
  * @return none
  */
void hb_timer_start(uint8_t timer);

/**
  * @brief stop the heartbeat timer
  * @param[in] timer: HB_TIMER_PUB or HB_TIMER_SUB
  * @return none
  */
void hb_timer_stop(uint8_t timer);

/**
  * @brief detect if the messggage need be processed
  * @param[in] src: message source addr
  * @param[in] dst: message destination addr
  * @return operation result
  * @retval true: need process
  * @retval false: do not process
  */
bool hb_receive_filter(uint16_t src, uint16_t dst);

/**
  * @brief process the message
  * @param[in] pmesh_msg: mesh message
  * @return operation result
  * @retval true: processed
  * @retval false: not processed
  */
bool hb_handle_msg(mesh_msg_p pmesh_msg);

/**
  * @brief send hb message
  * @param[in] type: the trigger source type
  * @return none
  */
void hb_msg_send(hb_trigger_type_t type);

/**
  * @brief timeout handler
  * @param[in] timer: publish or subscribe
  * @return none
  */
void hb_handle_timeout(uint8_t timer);

/**
 * @brief set heartbeat publication parameter
 * @param[in] pub: hearbeat publication parameter
 */
void hb_publication_set(hb_pub_t pub);

/**
 * @brief get heartbeat publication parameter
 * @return heartbeat publication parameter
 */
hb_pub_t hb_publication_get(void);

/**
 * @brief set heartbeat subscription parameter
 * @param[in] sub: hearbeat subscription parameter
 */
void hb_subscription_set(hb_sub_t sub);

/**
 * @brief get heartbeat subscription parameter
 * @return heartbeat subscription parameter
 */
hb_sub_t hb_subscription_get(void);

/**
 * @brief log value
 * @param[in] value: value to log, 2exp(n-1) is greater than or equal to value
 * @return log value
 */
uint8_t hb_log2_greater(uint16_t value);

/**
 * @brief log value
 * @param[in] value: value to log, 2exp(n-1) is less than or equal to value
 * @return log value
 */
uint8_t hb_log2_less(uint16_t value);

/**
 * @brief exp value
 * @param[in] log_value: value to exp
 * @return exp value
 */
uint16_t hb_exp2(uint8_t log_value);

/** @} */
/** @} */

END_DECLS

#endif /* _HEARTBEAT_H */
