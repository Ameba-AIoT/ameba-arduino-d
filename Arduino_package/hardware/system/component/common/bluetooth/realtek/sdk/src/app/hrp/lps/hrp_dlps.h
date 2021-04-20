/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     hrp_dlps.h
* @brief    Hrp dlps reference api.
* @details
* @author   jane
* @date     2016-02-18
* @version  v0.1
*********************************************************************************************************
*/
#ifndef _HRP_DLPS_H_
#define  _HRP_DLPS_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    HRP_DLPS_STATUS_IDLE = 0,
    HRP_DLPS_STATUS_INITIATED = 1,
    HRP_DLPS_STATUS_ACTIVED = 2,
    HRP_DLPS_STATUS_PAUSED = 3
} HRP_DLPS_STATUS;

extern HRP_DLPS_STATUS hrp_dlps_status;
extern uint32_t enter_dlps_count;

extern bool hrp_dlps_active_dlps(bool active);
extern void hrp_dlps_allow_enter(bool enter);


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

