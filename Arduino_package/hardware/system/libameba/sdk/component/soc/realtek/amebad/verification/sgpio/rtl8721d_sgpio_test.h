/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _RTL8195A_SGPIO_TEST_H_
#define _RTL8195A_SGPIO_TEST_H_

// SGPIO debug output
#define SGPIO_PREFIX      "RTL8195A[sgpio]: "
#define SGPIO_PREFIX_LVL  "    [sgpio_DBG]: "

#define SGPIO_VERI_DATA_LEN_DMA 32*2
#undef SGPIO_APP_TEST

typedef enum _SGPIO_VERI_PROC_CMD_SAL_ {
	SGPIO_TEST_ITEMS_SAL	= 0,
	SGPIO_TEST_ALL_SAL	= 1,
}SGPIO_VERI_PROC_CMD_SAL, *PSGPIO_VERI_PROC_CMD_SAL;

typedef enum _SGPIO_VERI_ITEM_SAL_ {
	SGPIO_TEST_SIMPLE_RX	= 1,
	SGPIO_TEST_SIMPLE_MUL	= 2,
	SGPIO_TEST_CAPTURE	= 3,
	SGPIO_TEST_TX	= 4,
	SGPIO_TEST_DALLAS	= 5,
	SGPIO_TEST_DHT22	= 6,
	SGPIO_TEST_PIR	= 7,
}SGPIO_VERI_ITEM_SAL, *PSGPIO_VERI_ITEM_SAL;

typedef enum _SGPIO_VERI_ITEMS_SAL_ {
	SGPIO_SIMPLE_RX_BEGIN = 1,
	SGPIO_SIMPLE_RX_STOP = 2,
	SGPIO_SIMPLE_MUL_BEGIN = 3,
	SGPIO_SIMPLE_MUL_STOP = 4,
	SGPIO_CAPTURE_EDGE = 5,
	SGPIO_CAPTURE_COMPARE = 6,
	SGPIO_TX_BASIC = 7,
	SGPIO_TX_WAVEFORM = 8,
	SGPIO_TX_FIFO = 9,
}SGPIO_VERI_ITEMS_SAL, *PSGPIO_VERI_ITEMS_SAL;

typedef enum  _SGPIO_TX_MODE_
{
	SGPIO_TX_REGISTER_MODE = 0x00,
	SGPIO_TX_FIFO_MODE = 0x01,
	SGPIO_TX_DMA_MODE = 0x02,
} SGPIO_TX_MODE, *PSGPIO_TX_MODE;

typedef enum  _SGPIO_TEST_OBJ_
{
	SGPIO_TEST_TO_DALLAS = 0x01,
	SGPIO_TEST_TO_DHT22 = 0x02,
	SGPIO_TEST_TO_PIR = 0x03,
} SGPIO_TEST_OBJ, *PSGPIO_TEST_OBJ;

// SGPIO device status 
typedef enum _SGPIO_Device_STATUS_ {
	SGPIO_STS_UNINITIAL = 0x00,
	SGPIO_STS_INITIALIZED = 0x01,
		
	SGPIO_STS_SLV_ACCESS = 0x02,
	SGPIO_STS_NO_SLV = 0x03,
	
	SGPIO_STS_TRX_READY = 0x04,
	SGPIO_STS_CAP = 0x05,
	SGPIO_STS_TX_READY = 0x06,
	SGPIO_STS_RX_READY = 0x07,
}SGPIO_Device_STATUS, *PSGPIO_Device_STATUS;

typedef struct _HAL_SGPIO_ADAPTER_ {
	SGPIO_TypeDef* SGPIOx;
	IRQn_Type   IrqNum;
	u32 TxDataMode;
	GDMA_InitTypeDef       	SGPIOGdmaInitStruct;              //Pointer to GDMA_InitTypeDef
	volatile u8 DevSts;
	u8 TestObj;
}HAL_SGPIO_ADAPTER, *PHAL_SGPIO_ADAPTER;

typedef struct _SGPIO_VERI_PARA_SAL_ {
	u32 VeriItem;
	u32 VeriLoop;
	u32 VeriPara1;
	u32 VeriPara2;
	u32 VeriPara3;
}SGPIO_VERI_PARA_SAL,*PSGPIO_VERI_PARA_SAL;

#endif

