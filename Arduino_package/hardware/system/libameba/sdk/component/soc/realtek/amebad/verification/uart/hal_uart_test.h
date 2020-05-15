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

#include "ameba_soc.h"
#include "rtl8721d_uart.h"

#define AmebaD_KM0_TEST   1

#ifdef AmebaD_KM0_TEST

#define TIMx    TIMx_LP
#define TIMx_irq	TIMx_irq_LP

#endif



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
	u8 UartIndex;

	/* for rx DMA timeout */
	u32 last_dma_addr;

	GDMA_InitTypeDef UARTTxGdmaInitStruct;
	GDMA_InitTypeDef UARTRxGdmaInitStruct;
	UART_InitTypeDef UART_InitStruct;
	UART_TypeDef* UARTx;
	IRQn_Type   IrqNum;

	VOID (*TxCompCallback)(VOID *para, int left_byte);    // User Tx complete callback function
	VOID (*RxCompCallback)(VOID *para, int left_byte);    // User Rx complete callback function
	VOID *TxCompCbPara; // the pointer argument for TxCompCbPara
	VOID *RxCompCbPara; // the pointer argument for RxCompCallback
}HAL_RUART_ADAPTER, *PHAL_RUART_ADAPTER;

extern u32 HalRuartSend(PHAL_RUART_ADAPTER pHalRuartAdapter, u8 *pTxData, u32 Length, u8 mode);
extern u32 HalRuartRecv(PHAL_RUART_ADAPTER pHalRuartAdapter, u8  *pRxData, u32 Length, u8 mode);
extern void HalRuartDmaDeInitGTimer1ms(void);
void HalRuartDmaInitGTimer1ms(PHAL_RUART_ADAPTER pHalRuartAdapter, u32 Period);
u32 HalRuartIrqHandle(IN VOID *Data);
#endif

