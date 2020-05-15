/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include "rtl8721d_usi_uart.h"


/**
 * RUART Configurations
 */
enum uart_transfer_mode{
        POLL_BASED,
        INT_BASED,
        DMA_BASED
};

typedef struct _HAL_RUART_ADAPTER_ {
	int TxCount;     // how many byte to TX
	int RxCount;     // how many bytes to RX
	u8 *pTxBuf;
	u8 *pRxBuf;
	u8 UsiIndex;

	/* for rx DMA timeout */
	u32 last_dma_addr;

	GDMA_InitTypeDef USIUARTTxGdmaInitStruct;
	GDMA_InitTypeDef USIUARTRxGdmaInitStruct;
	USI_UARTInitTypeDef USI_UARTInitStruct;
	USI_TypeDef* USIx;
	IRQn_Type   IrqNum;

	VOID (*TxCompCallback)(VOID *para, int left_byte);    // User Tx complete callback function
	VOID (*RxCompCallback)(VOID *para, int left_byte);    // User Rx complete callback function
	VOID *TxCompCbPara; // the pointer argument for TxCompCbPara
	VOID *RxCompCbPara; // the pointer argument for RxCompCallback
}HAL_USIRUART_ADAPTER, *PHAL_USIRUART_ADAPTER;

extern u32 HalUSIRuartSend(PHAL_USIRUART_ADAPTER pHalRuartAdapter, u8 *pTxData, u32 Length, u8 mode);
extern u32 HalUSIRuartRecv(PHAL_USIRUART_ADAPTER pHalRuartAdapter, u8  *pRxData, u32 Length, u8 mode);
extern void HalUSIRuartDmaDeInitGTimer1ms(void);
void HalUSIRuartDmaInitGTimer1ms(PHAL_USIRUART_ADAPTER pHalRuartAdapter, u32 Period);
u32 HalUSIRuartIrqHandle(IN VOID *Data);
#endif

