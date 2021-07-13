/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     vendor_tp_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author   ethan
  * @date     2016-02-18
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _VENDOR_PXPEXT_SERVICE_H_
#define _VENDOR_PXPEXT_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif

/* Add Includes here */
#include "profile_server.h"



T_SERVER_ID vendor_pxpext_service_add(void *pFunc);
#ifdef __cplusplus
}
#endif

#endif
