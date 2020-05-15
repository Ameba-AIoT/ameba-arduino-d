/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#ifndef _RTL8721D_USI_I2C_TEST_SAL_H_
#define _RTL8721D_USI_I2C_TEST_SAL_H_

#define USI_I2C_VERI_DATA_LEN_SAL       1024
#define USI_I2C_VERI_DMA_LEN_SAL       8

#define USI_I2C_VERI_RXBUF_LEN_SAL      32
#define USI_I2C_VERI_TXBUF_LEN_SAL      32

#define USI_I2C_VERI_PANT_CNT_SAL       6

#define USI_I2C_VERI_ACK_ADDR_SAL       0x23
#define USI_I2C_VERI_ACK_ADDR1_SAL       0x46

#define USI_I2C_VERI_USER_REG_ADDR_SAL  0x75
//#define USII2CMULTISLV
//#define ONESLV_MULTIMST_TEST

typedef enum _USI_I2C_VERI_ITEM_SAL_ {
	USI_I2C_MASTER_TEST  = 1,
	USI_I2C_SLAVE_TEST  = 2,	
	USI_I2C_GENERAL_CALL_TEST = 3,
	USI_I2C_M_NULLDATA_TEST = 4,
	USI_I2C_TEST_MultiMrtInit = 5,
	USI_I2C_TEST_ALL_CASE = 10
}USI_I2C_VERI_ITEM_SAL, *PUSI_I2C_VERI_ITEM_SAL;


typedef struct _USI_I2C_VERI_PARA_SAL_ {
	u32	VeriItem;
	u32	VeriLoop;
	u32	VeriDMA;
	u32 VeriItem1;
	u32 VeriItem2;
	u8	*VeriPattern;
	void	*VeriDatSrc;
	void	*VeriDatDes;
	u8	SpdMod;
	u8	AddrMod;
}USI_I2C_VERI_PARA_SAL,*PUSI_I2C_VERI_PARA_SAL;

#endif

