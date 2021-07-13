/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      aci_service_handle.h
* @brief     low power handle when using ACI.
* @details   none.
* @author    tifnan
* @date      2014-11-19
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _LTPLIB_MODULE_ID_H_
#define _LTPLIB_MODULE_ID_H_

typedef enum _HRP_MODULE_ID
{
    HRP_MODULE_RESERVED     = 0,
    HRP_MODULE_LOWER_STACK  = 1,
    HRP_MODULE_UPPER_STACK  = 2,
    HRP_MODULE_PROFILE      = 3,
    HRP_MODULE_RESERVED1,

    HRP_MODULE_AUDIO        = 16,
    HRP_MODULE_BLE_AUTO_TEST = 17,


} HRP_MODULE_ID;

/************************************************/
typedef enum
{
    HRP_BTIF_CMD_GROUP_STACK   = 0,
    HRP_BTIF_CMD_GROUP_SYSTEM = 1,
} HRP_BTIF_CMD_GROUP;



/************************************************/






#endif
