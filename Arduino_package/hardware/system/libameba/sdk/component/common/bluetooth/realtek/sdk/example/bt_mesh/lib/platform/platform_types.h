/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     platform_types.h
  * @brief    Head file for platform types.
  * @details  Basic types related.
  * @author   bill
  * @date     2017-3-3
  * @version  v1.0
  * *************************************************************************************
  */

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "platform_macros.h"

BEGIN_DECLS

/** @addtogroup Platform_Types
  * @{
  */

/** @defgroup Platform_Types_Exported_Macros Exported Macros
  * @brief
  * @{
  */

/* boolean definition */
#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    (0L)
#else
#define NULL    ((void *)0)
#endif
#endif

/** @} */
/** @} */

END_DECLS

#endif /* _PLATFORM_TYPES_H_ */
