/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_USI_SSI_H_
#define _HAL_USI_SSI_H_

#include "ameba_soc.h"


#define USI_SPI_DMA_RX_EN           (1<<0)
#define USI_SPI_DMA_TX_EN           (1<<1)

typedef struct {
	GDMA_InitTypeDef SSITxGdmaInitStruct;
	GDMA_InitTypeDef SSIRxGdmaInitStruct;
	IRQn_Type   IrqNum;
	USI_TypeDef *usi_dev;

	void (*RxCompCallback)(void *Para);
	void *RxCompCbPara;
	void (*TxCompCallback)(void *Para);
	void *TxCompCbPara;

	void *RxData;
	void *TxData;
	u32  RxLength;
	u32  TxLength;

	u32   Index;
	u32   Role;

	/* mbed var */
	u32 dma_en;
}HAL_USISSI_ADAPTOR, *PHAL_USISSI_ADAPTOR;

#ifdef CONFIG_GDMA_EN    
u32 HalUSISsiDmaSend(void *Adapter, u8 *pTxData, u32 Length);
u32 HalUSISsiDmaRecv(void *Adapter, u8 *pRxData, u32 Length);
#endif  // end of "#ifdef CONFIG_GDMA_EN"
// ROM code patch
u32 HalUSISsiIntRead(void *Adapter, void *RxData, u32 Length);
u32 HalUSISsiIntWrite(void *Adapter, u8 *pTxData, u32 Length);
u32 HalUSISsiInterruptHandle(void *Adaptor);
#endif

