/**
********************************************************************************************************
Copyright (c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file       Gattsvc_cts.h
* @brief     GATT built-in GLS (Current Time Service)
* @details
*
* @author       gordon
* @date         2015-07-09
* @version  v0.1
*/

#ifndef __GATTSVC_CTS_H
#define __GATTSVC_CTS_H

#ifndef __GATT_H
#include <gatt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/** service definition */

extern const T_ATTRIB_APPL gatt_svc_cts[];

extern const int gatt_svc_cts_size;
extern const int gatt_svc_cts_nbr_of_attrib;

/** CTS Current Time Service */
#define GATT_UUID_CURRENT_TIME                 0x1805

#define GATT_UUID_CHAR_CTS_CURRENT_TIME        0x2A2B
#define GATT_UUID_CHAR_CTS_LOCAL_TIME_INFO     0x2A0F
#define GATT_UUID_CHAR_CTS_REF_TIME_INFO       0x2A14
/** attribute indices signaled to GATT server application. */
/** do NOT change these values !!!!                        */

#define GATT_SVC_CTS_CURRENT_TIME_INDEX      2   /**< Current Time value      */
#define GATT_SVC_CTS_LOCAL_TIME_INFO_INDEX   5   /**< Local Time Information value     */
#define GATT_SVC_CTS_REF_TIME_INFO_INDEX     7   /**< Reference Time Information value */


#ifdef __cplusplus
}
#endif

#endif  /**< __GATTSVC_CTS_H */
