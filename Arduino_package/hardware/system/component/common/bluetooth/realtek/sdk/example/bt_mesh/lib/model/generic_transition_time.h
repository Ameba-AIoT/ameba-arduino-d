/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_transition_time.h
* @brief    Head file for generic transition time.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-7-11
* @version  v1.0
* *************************************************************************************
*/
#ifndef _GENERIC_TRANSITION_TIME_H_
#define _GENERIC_TRANSITION_TIME_H_

#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_TRANSITION_TIME
 * @{
 */


/**
 * @defgroup GENERIC_TRANSITION_TIME_DATA Generic Transition Time Data
 * @brief Generic transition data and structure definition
 * @{
 */
#define GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE      0x00
#define GENERIC_TRANSITION_NUM_STEPS_MAX            0x3E
#define GENERIC_TRANSITION_NUM_STEPS_UNKNOWN        0x3F
#define IS_GENERIC_TRANSITION_STEPS_VALID(step)     (step < GENERIC_TRANSITION_NUM_STEPS_UNKNOWN)


typedef enum
{
    GENERIC_TRANSITION_STEP_RESOLUTION_100MILLISECONDS,
    GENERIC_TRANSITION_STEP_RESOLUTION_1SECOND,
    GENERIC_TRANSITION_STEP_RESOLUTION_10SECONDS,
    GENERIC_TRANSITION_STEP_RESOLUTION_10MINUTS
} generic_transition_step_resolution_t;

typedef struct
{
    uint8_t num_steps: 6;
    uint8_t step_resolution: 2; //!< @ref generic_transition_step_resolution_t
} _PACKED_ generic_transition_time_t, *generic_transition_time_p;


#define IS_MAX_GENERIC_TRANSITION_TIME(trans_time) \
    ((trans_time.num_steps == GENERIC_TRANSITION_NUM_STEPS_MAX) && \
     (trans_time.step_resolution == GENERIC_TRANSITION_STEP_RESOLUTION_10MINUTS))

typedef int32_t (*generic_transition_step_change_cb)(const mesh_model_info_p pmodel_info,
                                                     uint32_t trans_type,
                                                     generic_transition_time_t total_time,
                                                     generic_transition_time_t remaining_time);
/** @} */

/**
 * @defgroup GENERIC_TRANSITION_TIME_API Generic Transition Time API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief initialize generic transition
 * @retval TRUE: initialize success
 * @retval FALSE: initialize failed
 */
bool generic_transition_time_init(void);

/**
 * @brief start mode state transition timer
 * @param[in] pmodel_info: pointer to model information context that need to transition
 * @param[in] trans_type: transition type
 * @param[in] trans_time: transition total time
 * @param[in] step_change_cb: step change done callback function
 * @retval TRUE: start transition success
 * @retval FALSE: start transition failed
 */
bool generic_transition_timer_start(const mesh_model_info_p pmodel_info,
                                    uint32_t trans_type,
                                    generic_transition_time_t trans_time,
                                    generic_transition_step_change_cb step_change_cb);

/**
 * @brief stop model state transition timer
 * @param[in] pmodel_info: pointer to model information context that need to stop transition
 * @param[in] trans_type: transition type
 */
void generic_transition_timer_stop(const mesh_model_info_p pmodel_info,
                                   uint32_t trans_type);

/**
 * @brief get model transition remaining time
 * @param[in] pmodel_info: pointer to model information context that in transition
 * @param[in] trans_type: transition type
 * @return remaining transition time
 */
generic_transition_time_t generic_transition_time_get(const mesh_model_info_p pmodel_info,
                                                      uint32_t trans_type);

/**
 * @brief convert generic transition time to ms
 * @param[in] trans_time: generic transition time
 * @return converted time, unit is ms
 */
uint32_t generic_transition_time_convert(generic_transition_time_t trans_time);

/**
 * @brief convert ms to generic transition time
 * @param[in] time: time ms
 * @return converted transition time
 */
generic_transition_time_t time_to_generic_transition_time(uint32_t time);
/** @} */
/** @} */


END_DECLS


#endif /* _GENERIC_TRANSITION_TIME_H_ */


