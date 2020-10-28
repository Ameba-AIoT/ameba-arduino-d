/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_lc_fsm.h
* @brief    Head file for light lc state machine.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2020-04-22
* @version  v1.0
* *************************************************************************************
*/
#ifndef _LIGHT_LC_FSM_H_
#define _LIGHT_LC_FSM_H_

#include "platform_types.h"

BEGIN_DECLS


/**
 * @addtogroup LIGHT_LC_FSM
 * @{
 */

/**
 * @defgroup LIGHT_LC_FSM_MESSAGE LC FSM Msg
 * @brief Light lc fsm message
 * @{
 */
typedef enum
{
    LIGHT_LC_EVENT_MODE_ON,
    LIGHT_LC_EVENT_MODE_OFF,
    LIGHT_LC_EVENT_OCCUPANCY_ON,
    LIGHT_LC_EVENT_LIGHT_ON,
    LIGHT_LC_EVENT_LIGHT_OFF,
    LIGHT_LC_EVENT_TIMER_OFF,
} light_lc_event_t;

typedef enum
{
    LIGHT_LC_ACTION_OFF,
    LIGHT_LC_ACTION_FADE_ON,
    LIGHT_LC_ACTION_FADE,
    LIGHT_LC_ACTION_PROLONG,
    LIGHT_LC_ACTION_STANDBY,
} light_lc_action_t;

typedef enum
{
    LIGHT_LC_TIME_TYPE_FADE_ON,
    LIGHT_LC_TIME_TYPE_RUN_ON,
    LIGHT_LC_TIME_TYPE_FADE,
    LIGHT_LC_TIME_TYPE_PROLONG,
    LIGHT_LC_TIME_TYPE_FADE_STANDBY_AUTO,
    LIGHT_LC_TIME_TYPE_FADE_STANDBY_MANUAL,
    LIGHT_LC_TIME_TYPE_OCCUPANCY_DELAY,
    LIGHT_LC_TIME_TYPE_TRANS,
} light_lc_time_type_t;

typedef enum
{
    LIGHT_LC_STATE_OFF,
    LIGHT_LC_STATE_STANDBY,
    LIGHT_LC_STATE_FADE_ON,
    LIGHT_LC_STATE_RUN,
    LIGHT_LC_STATE_FADE,
    LIGHT_LC_STATE_PROLONG,
    LIGHT_LC_STATE_FADE_STANDBY_AUTO,
    LIGHT_LC_STATE_FADE_STANDBY_MANUAL,
} light_lc_state_t;
/** @} */

/**
 * @defgroup LIGHT_LC_FSM_API LC FSM API
 * @brief Light lc fsm api
 * @{
 */
/**
 * @brief light lc action callback function
 * @param[in] action: action type
 * @param[in] trans_time: action transition time
 */
typedef void (*light_lc_action_cb_t)(light_lc_action_t action, uint32_t trans_time);

/**
 * @brief light lc timeout callback function
 * @param[in] pargs: timer handler
 */
typedef void (*light_lc_timeout_cb_t)(void *pargs);

/**
 * @brief initialize light lc fsm
 * @param[in] init_state: fsm initialize state
 * @param[in] action_cb: light lc action callback
 * @param[in] timneout_cb: light lc timeout callback
 */
void light_lc_fsm_init(light_lc_state_t init_state, light_lc_action_cb_t action_cb,
                       light_lc_timeout_cb_t timeout_cb);

/**
 * @brief set light lc fsm occupancy mode
 * @param[in] om: occupancy mode
 */
void light_lc_fsm_om_set(uint8_t om);

/**
 * @brief set light lc time
 * @param[in] type: light lc time type
 * @param[in] value: light lc time value
 */
void light_lc_time_set(light_lc_time_type_t type, uint32_t value);

/**
 * @brief get light lc time
 * @param[in] type: light lc type
 * @return light lc time
 */
uint32_t light_lc_time_get(light_lc_time_type_t type);

/**
 * @brief handle lc fsm event
 * @param[in] event: lc fsm event
 */
void light_lc_fsm_handle_event(light_lc_event_t event);

/**
 * @brief handle lc fsm timeout event
 * @param[in] ptimer: timer handle
 */
void light_lc_fsm_handle_timeout(void *ptimer);

/** @} */
/** @} */
END_DECLS

#endif /* LIGHT_LC_FSM_H_ */
