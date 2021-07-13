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

#ifndef _LTPLIB_APPLICATION_H_
#define _LTPLIB_APPLICATION_H_
#include <hrp.h>

#define MAX_NUMBER_OF_RX_EVENT          0x40
#define MAX_NUMBER_OF_MESSAGE           0x20
#define MAX_NUMBER_OF_TX_DATA           12
#define MAX_NUMBER_OF_TX_REL            MAX_NUMBER_OF_TX_DATA
#define MAX_NUMBER_OF_RX_DATA           12
#define MAX_NUMBER_OF_GAP_TIMER         0x10
#define BLT_MAX_NUMBER_OF_RX_EVENT      0x20
#define BLT_MAX_NUMBER_OF_IO_EVENT      0x20
#define MAX_NUMBER_OF_PROFILE_MSG       0x10

typedef enum
{
    HRP_STATUS_IDLE           = 0,
    HRP_STATUS_RESET      = 1,
    HRP_STATUS_ACTIVE      = 2,
} T_HRP_STATUS;


uint8_t hrp_init(void);
uint8_t hrp_task_init(void);
void hrp_system_reset(void);
void hrp_timer_cb(void *xtimer);

extern T_HRP_STATUS     system_status;
extern HRP_MODULE_ID    active_module;
extern P_BT_HRP  P_BtHrp;
bool hrp_send_event(const unsigned char *pEvent);

#endif
