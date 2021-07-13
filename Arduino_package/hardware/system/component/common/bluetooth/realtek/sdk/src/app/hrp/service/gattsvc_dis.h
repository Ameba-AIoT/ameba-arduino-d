/**
********************************************************************************************************
Copyright (c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file       gattsvc_dis.h
* @brief     device information service
* @details
*
* @author   gordon
* @date         2015-07-13
* @version  v0.1
*/


#ifndef __GATTSVC_DIS_H
#define __GATTSVC_DIS_H

#ifndef __GATT_H
#include <gatt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/** service definition */
extern const T_ATTRIB_APPL gatt_svc_dis[];

extern const int gatt_svc_dis_size;
extern const int gatt_svc_dis_nbr_of_attrib;

#define GATT_UUID_DEVICE_INFORMATION_SERVICE   0x180A

#define GATT_UUID_CHAR_SYSTEM_ID               0x2A23
#define GATT_UUID_CHAR_MODEL_NUMBER            0x2A24
#define GATT_UUID_CHAR_SERIAL_NUMBER           0x2A25
#define GATT_UUID_CHAR_FIRMWARE_REVISION       0x2A26
#define GATT_UUID_CHAR_HARDWARE_REVISION       0x2A27
#define GATT_UUID_CHAR_SOFTWARE_REVISION       0x2A28
#define GATT_UUID_CHAR_MANUFACTURER_NAME       0x2A29
#define GATT_UUID_CHAR_IEEE_CERTIF_DATA_LIST   0x2A2A
#define GATT_UUID_CHAR_PNP_ID                  0x2A50
/** attribute indices signaled to GATT server application. */
/** do NOT change these values !!!!                        */

#define GATT_SVC_DIS_SYSTEM_ID_INDEX          2   /**< index of System ID value         */
#define GATT_SVC_DIS_MANUFACTURER_NAME_INDEX  4   /**< index of Manufacturer Name value */
#define GATT_SVC_DIS_MODEL_NUMBER_INDEX       6   /**< index of Model Number value      */


#ifdef __cplusplus
}
#endif

#endif  /**< __GATTSVC_DIS_H */
