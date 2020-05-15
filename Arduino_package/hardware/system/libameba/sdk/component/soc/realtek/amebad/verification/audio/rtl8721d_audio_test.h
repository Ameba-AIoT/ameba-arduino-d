/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _RTL8721D_AUDIO_TEST_H_
#define _RTL8721D_AUDIO_TEST_H_

#include "ameba_soc.h"

struct audio_test {	
	GDMA_InitTypeDef       	SpTxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef
	GDMA_InitTypeDef       	SpRxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef

	u32 audio_tx_isr_cnt;	//current tx isr count
	u32 audio_rx_isr_cnt;	//current rx isr count
	
	u32 mono_val;	//when i2s playtone, set mono initial value
	u32 stereo_left_val;	//when i2s playtone, set stereo left channel init value
	u32 stereo_right_val;	//when i2s playtone, set stereo right channel init value
	u32 numBlock;
       u32 bRx1stBlk;
};

/* GDMA transmission define */
#define GDMA_SINGLE_MAX_SIZE			(128*30)

void AudioSpSetClk(AUDIO_SPORT_TypeDef* SPORTx, u32 Clk_Src);

_LONG_CALL_ void AUDIO_SP_SetClkDiv(AUDIO_SPORT_TypeDef* SPORTx, u32 mi, u32 ni);
_LONG_CALL_ void AUDIO_SP_SetTxCh(AUDIO_SPORT_TypeDef* SPORTx, u32 SP_TxCh);
_LONG_CALL_ void AUDIO_SP_SetRxCh(AUDIO_SPORT_TypeDef* SPORTx, u32 SP_RxCh);
_LONG_CALL_ BOOL AUDIO_SP_LPBKGDMA_Init(u32 Index, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc, u32 Length);
_LONG_CALL_ BOOL AUDIO_SP_MulTXGDMA_Init(u32 Index, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc, u8 *pTxData, u32 Length);
#endif
