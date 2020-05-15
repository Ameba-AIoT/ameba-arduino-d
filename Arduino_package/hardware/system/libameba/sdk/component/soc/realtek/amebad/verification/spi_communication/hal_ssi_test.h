/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_SSI_H_
#define _HAL_SSI_H_

#include "ameba_soc.h"


#define SPI_DMA_RX_EN           (1<<0)
#define SPI_DMA_TX_EN           (1<<1)

typedef struct {
	GDMA_InitTypeDef SSITxGdmaInitStruct;
	GDMA_InitTypeDef SSIRxGdmaInitStruct;
	IRQn_Type   IrqNum;
	SPI_TypeDef *spi_dev;

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
}HAL_SSI_ADAPTOR, *PHAL_SSI_ADAPTOR;

#ifdef CONFIG_GDMA_EN    
u32 HalSsiDmaSend(void *Adapter, u8 *pTxData, u32 Length);
u32 HalSsiDmaRecv(void *Adapter, u8 *pRxData, u32 Length);
#endif  // end of "#ifdef CONFIG_GDMA_EN"
// ROM code patch
u32 HalSsiIntRead(void *Adapter, void *RxData, u32 Length);
u32 HalSsiIntWrite(void *Adapter, u8 *pTxData, u32 Length);
u32 HalSsiInterruptHandle(void *Adaptor);
#endif

