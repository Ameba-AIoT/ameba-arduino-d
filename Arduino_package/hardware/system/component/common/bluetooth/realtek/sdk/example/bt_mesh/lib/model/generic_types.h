/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_types.h
* @brief    Head file for types used by generic models.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2017-12-22
* @version  v1.0
* *************************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef _GENERIC_TYPES_H
#define _GENERIC_TYPES_H

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup GENERIC_TYPES
 * @{
 */

/**
 * @defgroup GENERIC_TYPES_TYPES Generic Types
 * @brief Error code for models
 * @{
 */
enum
{
    GENERIC_STAT_SUCCESS,
    GENERIC_STAT_CANNOT_SET_RANGE_MIN,
    GENERIC_STAT_CANNOT_SET_RANGE_MAX
} _SHORT_ENUM_;
typedef uint8_t generic_stat_t;

#define MODEL_SUCCESS               0 //!< model data process success
#define MODEL_STOP_TRANSITION       1 //!< stop transition after model data process
/** @} */
/** @} */

END_DECLS

#endif /* _GENERIC_TYPES_H */
