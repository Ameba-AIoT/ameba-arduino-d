/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _RTL8195A_KEYSCAN_TEST_H_
#define _RTL8195A_KEYSCAN_TEST_H_

// KEYSCAN debug output
#define KEYSCAN_PREFIX      "RTL8195A[KEYSCAN]: "
#define KEYSCAN_PREFIX_LVL  "    [KEYSCAN_DBG]: "

//#define KEYSCAN_2M

typedef enum _KEYSCAN_VERI_ITEM_SAL_ {
	KEYSCAN_TEST_REGULAR_SCAN_MODE	= 1,
	KEYSCAN_TEST_EVENT_TRIGGER_MODE = 2,
	KEYSCAN_TEST_REGISTER_DUMP = 3,
}KEYSCAN_VERI_ITEM_SAL, *PKEYSCAN_VERI_ITEM_SAL;

typedef enum _KEYSCAN_VERI_ITEMS_SAL_ {
	KEYSCAN_SIMPLE_RX_BEGIN = 1,
	KEYSCAN_SIMPLE_RX_STOP = 2,
	KEYSCAN_SIMPLE_MUL_BEGIN = 3,
	KEYSCAN_SIMPLE_MUL_STOP = 4,
	KEYSCAN_CAPTURE_EDGE = 5,
	KEYSCAN_CAPTURE_COMPARE = 6,
	KEYSCAN_TX_BASIC = 7,
	KEYSCAN_TX_WAVEFORM = 8,
	KEYSCAN_TX_FIFO = 9,
}KEYSCAN_VERI_ITEMS_SAL, *PKEYSCAN_VERI_ITEMS_SAL;

// KEYSCAN device status 
typedef enum _SKEYSCAN_Device_STATUS_ {
	KEYSCAN_STS_UNINITIAL = 0x00,
	KEYSCAN_STS_INITIALIZED = 0x01,

	KEYSCAN_STS_SCAN_FINISH = 0x02,
}KEYSCAN_Device_STATUS, *PKEYSCAN_Device_STATUS;

typedef struct _HAL_KEYSCAN_ADAPTER_ {
	KEYSCAN_TypeDef* KEYSCANx;
	IRQn_Type   IrqNum;
	u32 ColumnSel;
	u32 RowSel;
	u32 WorkMode;
	u32 KeyLimit;
	volatile u8 DevSts;
}HAL_KEYSCAN_ADAPTER, *PHAL_KEYSCAN_ADAPTER;

typedef struct _KEYSCAN_VERI_PARA_SAL_ {
	u32 VeriItem;
	u32 VeriLoop;
	u32 VeriPara1;
	u32 VeriPara2;
	u32 VeriPara3;
}KEYSCAN_VERI_PARA_SAL,*PKEYSCAN_VERI_PARA_SAL;

#endif

