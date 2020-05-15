/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#ifndef _RTL8721D_PSRAM_TEST_SAL_H_
#define _RTL8721D_PSRAM_TEST_SAL_H_

// PSRAM debug output
#define PSRAM_PREFIX      "RTL8195A[psram]: "
#define PSRAM_PREFIX_LVL  "    [PSRAM_DBG]: "
//#define CYPRESS_PSRAM
#define PSRAM_DEBUG_GPIO		_PA_15
#ifdef CYPRESS_PSRAM
#define PSRAM_SIZE_BYTE		(8*1024*1024)
#else
#define PSRAM_SIZE_BYTE		(4*1024*1024)
#endif

typedef struct _SAL_PSRAM_MNGT_ADPT_ {
	PCTL_InitTypeDef       	PCTL_InitStruct;                //Pointer to PCTL initial data( HAL_I2C_INIT_DAT )
	PCTL_TypeDef*            PSRAMx;					   //Pointer to PCTL Device
} SAL_PSRAM_MNGT_ADPT, *PSAL_PSRAM_MNGT_ADPT;

typedef enum _PSRAM_VERI_ITEM_SAL_ {
	PSRAM_INIT_TEST = 1,
	PSRAM_MEMORY_RW = 2,
	PSRAM_DMA_RW = 3,
	PSRAM_DPIN_WR = 4,
	PSRAM_CACHE_ENABLE = 5,
	PSRAM_CACHE_DISABLE = 6,
	PSRAM_POWER_TEST = 7,
	PSRAM_REG_TEST = 8,
	PSRAM_DEBUG_PORT = 9,
	PSRAM_FT_TEST = 10,
}PSRAM_VERI_ITEM_SAL, *PPSRAM_VERI_ITEM_SAL;


typedef struct _PSRAM_VERI_PARA_SAL_ {
	u32	VeriItem;
	u32	VeriLoop;
	u32	Verimode1;
	u32	Verimode2;
	u32	Verimode3;
}PSRAM_VERI_PARA_SAL,*PPSRAM_VERI_PARA_SAL;

#endif

