/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     model_fsm.h
* @brief    Head file for model finite state machine
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2020-4-20
* @version  v1.0
* *************************************************************************************
*/
#ifndef _MODEL_FSM_H_
#define _MODEL_FSM_H_

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_LC_FSM
 * @{
 */

/**
 * @defgroup MODEL_FSM_TYPE Model FSM Types
 * @brief Model fsl types
 * @{
 */
typedef struct
{
    uint32_t event;
    bool (*event_action)(uint32_t state);
    uint32_t next_state;
} model_fsm_event_group_t;

typedef struct
{
    uint32_t state;
    void (*state_enter)(void);
    const model_fsm_event_group_t *pevent_group;
    uint32_t event_group_len;
    void (*state_exit)(void);
} model_fsm_table_t;

typedef struct _model_fsm_t model_fsm_t;
/** @} */

/**
 * @defgroup MODEL_FSL_API Model FSM API
 * @brief Model fsm api
 * @{
 */

/**
 * @brief register model fsm
 * @param[in] ptabel: fsm table
 * @param[in] table_len: tabel length
 * @param[in] init_state: fsm initialize state
 * @return model fsm handle
 */
model_fsm_t *model_fsm_reg(const model_fsm_table_t *ptable, uint32_t table_len,
                           uint32_t init_state);

/**
 * @brief unregister model fsm
 * @param[in] pfsm: model fsm handle
 */
void model_fsm_unreg(model_fsm_t *pfsm);

/**
 * @brief run model fsm
 * @param[in] pfsm: model fsm handle
 */
void model_fsm_run(model_fsm_t *pfsm);

/**
 * @brief pause model fsm
 * @param[pfsm] pfsm: model fsm handle
 */
void model_fsm_pause(model_fsm_t *pfsm);

/**
 * @brief stop model fsm
 * @param[in] pfsm: model fsm handle
 */
void model_fsm_stop(model_fsm_t *pfsm);

/**
 * @brief model fsm handle event
 * @param[in] pfsm: model fsm handle
 * @param[in] event: model event
 */
void model_fsm_handle_event(model_fsm_t *pfsm, uint32_t event);
/** @} */

/** @} */
END_DECLS


#endif /* _MODEL_SM_H_ */
