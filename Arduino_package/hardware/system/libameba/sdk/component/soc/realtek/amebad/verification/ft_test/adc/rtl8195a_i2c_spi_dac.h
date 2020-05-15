/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _RTL8195A_I2C_SPI_DAC_H_
#define _RTL8195A_I2C_SPI_DAC_H_

typedef struct {
	GDMA_InitTypeDef SSITxGdmaInitStruct;
	SPI_TypeDef *spi_dev;

	void (*TxCompCallback)(void *Para);
	void *TxCompCbPara;

	void *TxData;
	u32  TxLength;

	u32   Index;
	u32   Role;
}HAL_SSI_ADAPTOR, *PHAL_SSI_ADAPTOR;

extern HAL_SSI_ADAPTOR HalSsiAdaptorMaster;

typedef struct _SAL_I2C_TRANSFER_BUF_ {
    u16     DataLen;                        //I2C Transmfer Length
    u16     TargetAddr;                     //I2C Target Address. It's only valid in Master Mode.
    u32     RegAddr;                        //I2C Register Address. It's only valid in Master Mode.
    u32     RSVD;                           //
    u8      *pDataBuf;                      //I2C Transfer Buffer Pointer
}SAL_I2C_TRANSFER_BUF,*PSAL_I2C_TRANSFER_BUF;

//I2C SAL management adapter
typedef struct _SAL_I2C_MNGT_ADPT_ {
	I2C_InitTypeDef       	HalInitDat;                //Pointer to HAL I2C initial data( HAL_I2C_INIT_DAT )
	I2C_TypeDef*            I2Cx;					   //Pointer to I2C Device
	IRQn_Type   IrqNum;
	VOID (*DMATXCCB)      (VOID *Data);
    	u32  DMATXCCBData;

	GDMA_InitTypeDef       	I2CTxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef

	PSAL_I2C_TRANSFER_BUF   pTXBuf;             //Pointer to I2C TX buffer
	u32                     TimeOut;            //I2C IO Timeout count,  in ms
	u8                      PinMux;             //I2C pin mux seletion
	volatile u8             DevSts;             //I2C device status
} SAL_I2C_MNGT_ADPT, *PSAL_I2C_MNGT_ADPT;

#define ADC_VERI_DATA_LEN       2048
//#define ADC_VERI_DATA_LEN       184

#endif