/**
********************************************************************************************************
Copyright (c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file       Gattsvc_bas.h
* @brief     GATT built-in BAS (Battery Service)
* @details
*
* @author       gordon
* @date         2015-07-09
* @version  v0.1
*/

#ifndef __GATTSVC_BAS_H
#define __GATTSVC_BAS_H

#ifndef __GATT_H
#include <gatt.h>
#endif



#ifdef __cplusplus
extern "C" {
#endif


/** service definition */

extern const T_ATTRIB_APPL gatt_svc_bas[];

extern const int gatt_svc_bas_size;
extern const int gatt_svc_bas_nbr_of_attrib;

/** BAS Battery Service */
#define GATT_UUID_BATTERY                      0x180F

#define GATT_UUID_CHAR_BAS_LEVEL               0x2A19
/** attribute indices signaled to GATT server application. */
/** do NOT change these values !!!!                        */

#define GATT_SVC_BAS_BATTERY_LEVEL_INDEX    2     /**< battery level value */


#ifdef __cplusplus
}
#endif

#endif  /**< __GATTSVC_BAS_H */
