/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     delay_execution.h
* @brief    Head file for delay execution.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-11-29
* @version  v1.0
* *************************************************************************************
*/
#ifndef _DELAY_EXECUTION_H_
#define _DELAY_EXECUTION_H_

#include "platform_types.h"
#include "mesh_api.h"

BEGIN_DECLS

/**
 * @addtogroup DELAY_EXECUTION
 * @{
 */

/** @defgroup DELAY_EXECUTION_DATA Delay Execution Data
  * @brief Delay execution data and structure definition
  * @{
  */
#define DELAY_EXECUTION_STEP_RESOLUTION             5 //!< unit is ms

typedef int32_t (*delay_execution_cb)(mesh_model_info_t *pmodel_info,
                                      uint32_t data_type);
/** @} */

/** @defgroup DELAY_EXECUTION_API Delay Execution Api
  * @brief Functions declaration
  * @{
  */

/**
 * @brief initialize delay execution
 * @retval TRUE: initialize success
 * @retval FALSE: initialize failed
 */
bool delay_execution_init(void);

/**
 * @brief start model delay execution timer
 * @param[in] pmodel_info: pointer to model information context that need to delay
 * @param[in] delay_type: delay execution type
 * @param[in] delay_time: delay execution total time
 * @param[in] delay_execution: delay execution callback function
 * @retval TRUE: start delay execution success
 * @retval FALSE: start delay execution failed
 */
bool delay_execution_timer_start(const mesh_model_info_t *pmodel_info,
                                 uint32_t delay_type, uint32_t delay_time,
                                 delay_execution_cb delay_execution);

/**
 * @brief stop model delay execution timer
 * @param[in] pmodel_info: pointer to model information context that need to stop delay
 * @param[in] delay_type: delay execution type
 */
void delay_execution_timer_stop(const mesh_model_info_t *pmodel_info,
                                uint32_t delay_type);
/** @} */
/** @} */


END_DECLS


#endif /** _DELAY_EXECUTION_H_ */


