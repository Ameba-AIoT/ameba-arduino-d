/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      aci_low_power.h
* @brief     low power handle when using ACI.
* @details   none.
* @author    tifnan
* @date      2014-11-19
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _BTLTP_UART_BB3_H_
#define _BTLTP_UART_BB3_H_

#ifdef __cplusplus
extern "C" {
#endif

void TxAssistTask(void *pParameters);
void ltpPeripheralInit(void);
void ltpStartTransmit(void);

#ifdef __cplusplus
}
#endif

#endif  /* _BTLTP_UART_BB3_H_ */

