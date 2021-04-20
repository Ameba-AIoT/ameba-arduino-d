/**
********************************************************************************************************
Copyright (c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file       Gattsvc_ndcs.h
* @brief     GATT built-in NDCS (Next DST (Daylight Saving Time) Change Service)
* @details
*
* @author       gordon
* @date         2015-07-09
* @version  v0.1
*/

#ifndef __GATTSVC_NDCS_H
#define __GATTSVC_NDCS_H

#include <gatt.h>

#ifdef __cplusplus
extern "C" {
#endif


/** service definition */
extern const T_ATTRIB_APPL gatt_svc_ndcs[];

extern const int gatt_svc_ndcs_size;
extern const int gatt_svc_ndcs_nbr_of_attrib;
/** NDCS Next DST Change Service */
#define GATT_UUID_NEXT_DST_CHANGE              0x1807

#define GATT_UUID_CHAR_NDCS_TIME_WITH_DST      0x2A11
/** attribute indices signaled to GATT server application. */
/** do NOT change these values !!!!                        */

#define GATT_SVC_NDCS_TIME_WITH_DST_INDEX    2  /**< Time with DST value */


#ifdef __cplusplus
}
#endif

#endif  /**< __GATTSVC_NDCS_H */
