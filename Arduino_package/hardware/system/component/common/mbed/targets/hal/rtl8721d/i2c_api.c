/** mbed Microcontroller Library
  ******************************************************************************
  * @file    i2c_api.c
  * @author 
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for I2C.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */
#include "objects.h"
#include "PinNames.h"
#include "i2c_api.h"

#include "pinmap.h"

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

static uint16_t i2c_target_addr[2];
extern u32 ConfigDebugErr;
extern u32 ConfigDebuginfo;
I2C_InitTypeDef I2CInitDat[2];
static u32 restart_enable = 0;
static u32 master_addr_retry = 1;

static u8 i2c_sel;     //0: i2c 1:usi_i2c
static uint16_t usi_i2c_target_addr[2];
USI_I2C_InitTypeDef USI_I2CInitDat[2];

/** @addtogroup AmebaD_Mbed_API 
  * @{
  */

/** @defgroup MBED_I2C
 *  @brief      MBED_I2C driver modules.
 *  @{
 */

/** @defgroup MBED_I2C_Exported_Functions MBED_I2C Exported Functions
  * @{
  */
void i2c_send_restart(I2C_TypeDef *I2Cx, u8* pBuf, u32 len, u8 restart);
void usi_i2c_send_restart(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len, u8 restart);
/**
  * @brief  Read data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV.
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @note   deal with condition that master send address while slave no ack
  * @retval the length of data read. 
  */
u32 I2C_MasterRead_Patch(I2C_TypeDef *I2Cx, u8* pBuf, u32 len)
{
	u32 cnt = 0;
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));

	/* read in the DR register the data to be received */
	for(cnt = 0; cnt < len; cnt++) {
		if(cnt >= len - 1) {
			/* generate stop singal */
			I2Cx->IC_DATA_CMD = 0x0003 << 8;
		} else {
			I2Cx->IC_DATA_CMD = 0x0001 << 8;
		}
		/* wait for I2C_FLAG_RFNE flag */			
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_RFNE)) == 0) {
			if(I2C_GetRawINT(I2Cx) & BIT_IC_RAW_INTR_STAT_TX_ABRT) {
				DBG_8195A(" TX_ABRT\n");
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}
		}
		*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
	}
	return cnt;
}

/**
  * @brief  Write data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that send.
  * @note   deal with condition that master send address while slave no ack
  * @retval the length of data send. 
  */
u32 I2C_MasterWrite_Patch(I2C_TypeDef *I2Cx, u8* pBuf, u32 len)
{
	u32 cnt = 0;
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFNF)) == 0);
		
		if(cnt >= len - 1)
		{
			/*generate stop signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
		}
		else
		{
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFE)) == 0) {
			if(I2C_GetRawINT(I2Cx) & BIT_IC_RAW_INTR_STAT_TX_ABRT) {
				DBG_8195A(" TX_ABRT= %x\n",I2Cx->IC_TX_ABRT_SOURCE);
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}
		}
	}
	return cnt;
}

/**
  * @brief  Read data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV .
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.  
  * @retval the length of data read. 
  */
int I2C_MasterRead_TimeOut(I2C_TypeDef *I2Cx, u8* pBuf, u32 len, u32 timeout_ms)
{
	int cnt = 0;
	u32 InTimeoutCount = 0;
	
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));

	/* read in the DR register the data to be received */
	for(cnt = 0; cnt < len; cnt++) {
		InTimeoutCount = timeout_ms*500;
		
		if(cnt >= len - 1) {
			/* generate stop singal */
			I2Cx->IC_DATA_CMD = 0x0003 << 8;
		} else {
			I2Cx->IC_DATA_CMD = 0x0001 << 8;
		}
		
		/* wait for I2C_FLAG_RFNE flag */
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_RFNE)) == 0) {
			if(I2C_GetRawINT(I2Cx) & BIT_IC_RAW_INTR_STAT_TX_ABRT) {
				DBG_8195A(" TX_ABRT\n");
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}

			DelayUs(2);

			if (InTimeoutCount == 0) {
				DBG_8195A("MasterRead_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}
		
		*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
	}
	
	return cnt;
}

/**
  * @brief  Write data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.  
  * @retval the length of data send. 
  */
int I2C_MasterWrite_TimeOut(I2C_TypeDef *I2Cx, u8* pBuf, u32 len, u32 timeout_ms)
{
	int cnt = 0;
	u32 InTimeoutCount = 0;

	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		InTimeoutCount = timeout_ms*500;
		
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFNF)) == 0);
		
		if(cnt >= len - 1)
		{
			/*generate stop signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
		}
		else
		{
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}

		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFE)) == 0) {
			if(I2C_GetRawINT(I2Cx) & BIT_IC_RAW_INTR_STAT_TX_ABRT) {
				DBG_8195A(" TX_ABRT\n");
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}
			
			DelayUs(2);

			if (InTimeoutCount == 0) {
				DBG_8195A("MasterWrite_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}
	}

	return cnt;
}

/**
  * @brief  Master sends single byte through the I2Cx peripheral to detect slave device.
  * @param  obj: i2c object defined in application software.
  * @param  address: the address of slave that to be detected.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval Slave ack condition:
  *          - 0: Slave available
  *          - -1: Slave not available 
  */
int I2C_MasterSendNullData_TimeOut(I2C_TypeDef *I2Cx, int address, u32 timeout_ms) 
{
	u8 I2CTemp = (u8)(address<<1);
	I2C_MasterSendNullData(I2Cx, &I2CTemp, 0, 1, 0);

	DelayMs(timeout_ms);

	if(I2C_GetRawINT(I2Cx) & BIT_IC_RAW_INTR_STAT_TX_ABRT) {
		I2C_ClearAllINT(I2Cx);
		
		/* Wait for i2c enter trap state from trap_stop state*/
		DelayUs(100);
		I2C_Cmd(I2Cx, DISABLE);
		I2C_Cmd(I2Cx, ENABLE);
		
		return -1;
	}
	return 0;
}

u8 USI_I2C_MasterRead_Patch(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len)
{
	u8 cnt = 0;
	/* Check the parameters */
	assert_param(IS_USI_I2C_ALL_PERIPH(USI_I2Cx));

	/* read in the DR register the data to be received */
	for(cnt = 0; cnt < len; cnt++) {
		if(cnt >= len - 1) {
			/* generate stop singal */
			USI_I2Cx->TX_FIFO_WRITE = 0x0003 << 8;
		} else {
			USI_I2Cx->TX_FIFO_WRITE = 0x0001 << 8;
		}
		/* wait for USI_I2C_FLAG_RFNE flag */

		while((USI_I2Cx->RX_FIFO_STATUS&USI_RXFIFO_EMPTY) != 0){
			if(USI_I2C_GetRawINT(USI_I2Cx) & USI_I2C_TX_ABRT_RSTS) {
				DBG_8195A(" TX_ABRT\n");
				USI_I2C_ClearAllINT(USI_I2Cx);
				return cnt;
			}	
		}
		*pBuf++ = (u8)USI_I2Cx->RX_FIFO_READ;
	}
	return cnt;
}

/**
  * @brief  Write data with special length in master mode through the USI_I2Cx peripheral under in-house IP.
  * @param  USI_I2Cx: where USI_I2Cx can be USI0_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that send.
  * @note   deal with condition that master send address while slave no ack
  * @retval the length of data send. 
  */
u8 USI_I2C_MasterWrite_Patch(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len)
{
	u8 cnt = 0;
	/* Check the parameters */
	assert_param(IS_USI_I2C_ALL_PERIPH(USI_I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_FULL != 0);
		
		if(cnt >= len - 1)
		{
			/*generate stop signal*/
			USI_I2Cx->TX_FIFO_WRITE= (*pBuf++) | (1 << 9);
		}
		else
		{
			USI_I2Cx->TX_FIFO_WRITE= (*pBuf++);
		}

		while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_EMPTY == 0){
			if(USI_I2C_GetRawINT(USI_I2Cx) & USI_I2C_TX_ABRT_RSTS) {
				DBG_8195A(" TX_ABRT= %x\n",USI_I2Cx->I2C_TX_ABRT_SOURCE);
				USI_I2C_ClearAllINT(USI_I2Cx);
				return cnt;
			}	
		}
	}
	return cnt;
}

/**
  * @brief  Read data with special length in master mode through the USI_I2Cx peripheral under in-house IP.
  * @param  USI_I2Cx: where USI_I2Cx can be USI0_DEV .
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.  
  * @retval the length of data read. 
  */
int USI_I2C_MasterRead_TimeOut(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len, u32 timeout_ms)
{
	int cnt = 0;
	u32 InTimeoutCount = 0;
	
	/* Check the parameters */
	assert_param(IS_USI_I2C_ALL_PERIPH(USI_I2Cx));

	/* read in the DR register the data to be received */
	for(cnt = 0; cnt < len; cnt++) {
		InTimeoutCount = timeout_ms*500;
		
		if(cnt >= len - 1) {
			/* generate stop singal */
			USI_I2Cx->TX_FIFO_WRITE= 0x0003 << 8;
		} else {
			USI_I2Cx->TX_FIFO_WRITE= 0x0001 << 8;
		}
		
		/* wait for USI_I2C_FLAG_RFNE flag */
		while(USI_I2Cx->RX_FIFO_STATUS&USI_RXFIFO_EMPTY != 0){
			if(USI_I2C_GetRawINT(USI_I2Cx) & USI_I2C_TX_ABRT_RSTS) {
				DBG_8195A(" TX_ABRT\n");
				USI_I2C_ClearAllINT(USI_I2Cx);
				return cnt;
			}	

			DelayUs(2);

			if (InTimeoutCount == 0) {
				DBG_8195A("MasterRead_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}
		
		*pBuf++ = (u8)USI_I2Cx->RX_FIFO_READ;
	}
	
	return cnt;
}

/**
  * @brief  Write data with special length in master mode through the USI_I2Cx peripheral under in-house IP.
  * @param  USI_I2Cx: where USI_I2Cx can be USI0_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.  
  * @retval the length of data send. 
  */
int USI_I2C_MasterWrite_TimeOut(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len, u32 timeout_ms)
{
	int cnt = 0;
	u32 InTimeoutCount = 0;

	/* Check the parameters */
	assert_param(IS_USI_I2C_ALL_PERIPH(USI_I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		InTimeoutCount = timeout_ms*500;
		
		while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_FULL != 0);
		
		if(cnt >= len - 1)
		{
			/*generate stop signal*/
			USI_I2Cx->TX_FIFO_WRITE= (*pBuf++) | (1 << 9);
		}
		else
		{
			USI_I2Cx->TX_FIFO_WRITE= (*pBuf++);
		}
		while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_EMPTY == 0){
			if(USI_I2C_GetRawINT(USI_I2Cx) & USI_I2C_TX_ABRT_RSTS) {
				DBG_8195A(" TX_ABRT\n");
				USI_I2C_ClearAllINT(USI_I2Cx);
				return cnt;
			}
			
			DelayUs(2);

			if (InTimeoutCount == 0) {
				DBG_8195A("MasterWrite_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}
	}

	return cnt;
}

/**
  * @brief  Master sends single byte through the USI_I2Cx peripheral to detect slave device.
  * @param  obj: usi_i2c object defined in application software.
  * @param  address: the address of slave that to be detected.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval Slave ack condition:
  *          - 0: Slave available
  *          - -1: Slave not available 
  */
int USI_I2C_MasterSendNullData_TimeOut(USI_TypeDef *USI_I2Cx, int address, u32 timeout_ms) 
{
	u8 USI_I2CTemp = (u8)(address<<1);
	USI_I2C_MasterSendNullData(USI_I2Cx, &USI_I2CTemp, 0, 1, 0);

	DelayMs(timeout_ms);

	if(USI_I2C_GetRawINT(USI_I2Cx) & USI_I2C_TX_ABRT_RSTS) {
		USI_I2C_ClearAllINT(USI_I2Cx);
		
		/* Wait for usi_i2c enter trap state from trap_stop state*/
		DelayUs(100);
		USI_I2C_Cmd(USI_I2Cx, DISABLE);
		USI_I2C_Cmd(USI_I2Cx, ENABLE);
		
		return -1;
	}
	return 0;
}

static void i2c_usi_sel(PinName sda)
{
	if ((sda == _PA_26) || (sda == _PB_6) || (sda == _PB_0)) {
		i2c_sel = 0;
	}else if((sda == _PA_24) || (sda == _PB_21)|| (sda == _PB_25)){
		i2c_sel = 1;
	}
}

/**
  * @brief  Get i2c index according to the SDA PinName.
  * @param  sda: SDA PinName according to pinmux spec.
  * @retval i2c index:
  *          - 0: I2C0 Device
  */
static uint32_t
i2c_index_get(PinName sda)
{
	if(i2c_sel ==0){ 
		if ((sda == _PA_26) || (sda == _PB_6) || (sda == _PB_0)) {
			return 0;
		} else {
			assert_param(0);
		}
	}else{
		if ((sda == _PA_24) || (sda == _PB_21)|| (sda == _PB_25)) {
			return 0;
		} else {
			assert_param(0);
		}
	}
	return 2;
}

/**
  * @brief  Initializes the I2C device, include clock/function/I2C registers.
  * @param  obj: i2c object defined in application software.
  * @param  sda: SDA PinName according to pinmux spec.
  * @param  scl: SCL PinName according to pinmux spec.
  * @retval none  
  */
void i2c_init(i2c_t *obj, PinName sda, PinName scl) 
{
	i2c_usi_sel(sda);
	uint32_t i2c_idx = i2c_index_get(sda);

	//ConfigDebugErr &= (~(_DBG_I2C_|_DBG_GDMA_));
	//ConfigDebugInfo&= (~(_DBG_I2C_|_DBG_GDMA_));

	DBG_8195A("i2c_idx:%x\n",i2c_idx);

	if(i2c_sel == 0){
		obj->i2c_idx = i2c_idx;
		obj->I2Cx = I2C_DEV_TABLE[i2c_idx].I2Cx;
		
		/* Set I2C Device Number */
		I2CInitDat[obj->i2c_idx].I2CIdx = i2c_idx;

		/* Load I2C default value */
		I2C_StructInit(&I2CInitDat[obj->i2c_idx]);

		/* Assign I2C Pin Mux */
		I2CInitDat[obj->i2c_idx].I2CMaster     = I2C_MASTER_MODE;
		I2CInitDat[obj->i2c_idx].I2CSpdMod     = I2C_SS_MODE;
		I2CInitDat[obj->i2c_idx].I2CClk        = 100;
		I2CInitDat[obj->i2c_idx].I2CAckAddr    = 0;    

		/* Init I2C now */
		//if(I2CInitDat[obj->i2c_idx].I2CIdx == 0) {
			//RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, DISABLE);
			//RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, ENABLE);
		//} else {
			//RCC_PeriphClockCmd(APBPeriph_I2C1, APBPeriph_I2C1_CLOCK, DISABLE);
			//RCC_PeriphClockCmd(APBPeriph_I2C1, APBPeriph_I2C1_CLOCK, ENABLE);
		//}

		/* I2C Pin Mux Initialization */
		Pinmux_Config(sda, PINMUX_FUNCTION_I2C);
		Pinmux_Config(scl, PINMUX_FUNCTION_I2C);

		PAD_PullCtrl(sda, GPIO_PuPd_UP);               
		PAD_PullCtrl(scl, GPIO_PuPd_UP);
		/* I2C HAL Initialization */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);

		/* I2C Enable Module */
		I2C_Cmd(obj->I2Cx, ENABLE);

	}else{
		obj->usi_i2c_idx = i2c_idx;
		obj->USI_I2Cx = USI_DEV_TABLE[i2c_idx].USIx;
		
		/* Set USI_I2C Device Number */
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CIdx = i2c_idx;

		/* Load USI_I2C default value */
		USI_I2C_StructInit(&USI_I2CInitDat[obj->usi_i2c_idx]);

		/* Assign USI_I2C Pin Mux */
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CMaster     = USI_I2C_MASTER_MODE;
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CSpdMod     = USI_I2C_SS_MODE;
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CClk        = 100;
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr    = 0;    

		/* USI_I2C Pin Mux Initialization */
		Pinmux_Config(sda, PINMUX_FUNCTION_I2C);
		Pinmux_Config(scl, PINMUX_FUNCTION_I2C);

		PAD_PullCtrl(sda, GPIO_PuPd_UP);               
		PAD_PullCtrl(scl, GPIO_PuPd_UP);
		/* USI_I2C HAL Initialization */
		USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);

		/* USI_I2C Enable Module */
		USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
	}
}

/**
  * @brief  Set i2c frequency.
  * @param  obj: i2c object defined in application software.
  * @param  hz: i2c clock(unit is Hz).
  * @retval none  
  */
void i2c_frequency(i2c_t *obj, int hz) 
{
	if(i2c_sel == 0){
		uint16_t i2c_default_clk    = (uint16_t) I2CInitDat[obj->i2c_idx].I2CClk;
		uint16_t i2c_user_clk       = (uint16_t) (hz/1000);

		if (i2c_default_clk != i2c_user_clk) {
			/* Deinit I2C first */
			i2c_reset(obj);
			if (i2c_user_clk <= 100) {
				I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_SS_MODE;
			}
			else if ((i2c_user_clk > 100) && (i2c_user_clk <= 400)) {
				I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_FS_MODE;
			}
			else if (i2c_user_clk > 400) {
				I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_HS_MODE;
			}
			else {
				I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_SS_MODE;
			}

			/* Load the user defined I2C clock */
			I2CInitDat[obj->i2c_idx].I2CClk = i2c_user_clk;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);     
		}
	}else{
		uint16_t usi_i2c_default_clk    = (uint16_t) USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CClk;
		uint16_t usi_i2c_user_clk       = (uint16_t) (hz/1000);

		if (usi_i2c_default_clk != usi_i2c_user_clk) {
			/* Deinit USI_I2C first */
			i2c_reset(obj);
			if (usi_i2c_user_clk <= 100) {
				USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CSpdMod = USI_I2C_SS_MODE;
			}
			else if ((usi_i2c_user_clk > 100) && (usi_i2c_user_clk <= 400)) {
				USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CSpdMod = USI_I2C_FS_MODE;
			}
			else if (usi_i2c_user_clk > 400) {
				USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CSpdMod = USI_I2C_HS_MODE;
			}
			else {
				USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CSpdMod = USI_I2C_SS_MODE;
			}
			DBG_8195A("usi_i2c_user_clk:%d\n",usi_i2c_user_clk);
			/* Load the user defined USI_I2C clock */
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CClk = usi_i2c_user_clk;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);     
		}
	}
}

/**
  * @brief  Start i2c device.
  * @param  obj: i2c object defined in application software.
  * @retval 0  
  */
inline int i2c_start(i2c_t *obj) 
{
	/* To avoid gcc warnings */
	( void ) obj;
	
	return 0;
}

/**
  * @brief  Stop i2c device.
  * @param  obj: i2c object defined in application software.
  * @retval 0  
  */
inline int i2c_stop(i2c_t *obj) 
{
	/* To avoid gcc warnings */
	( void ) obj;
	return 0;
}

/**
  * @brief  I2C master read in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  address: slave address which will be transmitted.
  * @param  data: point to the buffer to hold the received data.
  * @param  length: the length of data that to be received.
  * @param  stop: specifies whether a STOP is issued after all the bytes are received.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval the length of data received. If the retval less than the data length that want to be received, this transfer is not success.
  */
int i2c_read_timeout(i2c_t *obj, int address, char *data, int length, int stop, int timeout_ms) 
{
	/* To avoid gcc warnings */
	( void ) stop;

	if(i2c_sel == 0){
		if (i2c_target_addr[obj->i2c_idx] != address) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);
		}

		return (I2C_MasterRead_TimeOut(obj->I2Cx, (unsigned char*)data, length,timeout_ms));
	}else{
		if (usi_i2c_target_addr[obj->usi_i2c_idx] != address) {
		/* Deinit USI_I2C first */
		usi_i2c_reset(obj);

		/* Load the user defined USI_I2C target slave address */
		usi_i2c_target_addr[obj->usi_i2c_idx] = address;
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;

		/* Init USI_I2C now */
		USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
		USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
	}

	return (USI_I2C_MasterRead_TimeOut(obj->USI_I2Cx, (unsigned char*)data, length,timeout_ms));
	}

}

/**
  * @brief  I2C master wite in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  address: slave address which will be transmitted.
  * @param  data: point to the buffer to hold the received data.
  * @param  length: the length of data that to be received.
  * @param  stop: specifies whether a STOP is issued after all the bytes are written.
  * @param  timeout_ms:  specifies timeout time, unit is ms.  
  * @note   If the length equal to zero, will call I2C_MasterSendNullData_TimeOut()
  * @retval the length of data written.  If the retval less than the data length that want to be sent, this transfer is not success.
  */
int i2c_write_timeout(i2c_t *obj, int address, char *data, int length, int stop, int timeout_ms) 
{
	/* To avoid gcc warnings */
	( void ) stop;
	
	if(i2c_sel == 0){	
		if (i2c_target_addr[obj->i2c_idx] != address) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);
		}

		if(!length) {
			return (I2C_MasterSendNullData_TimeOut(obj->I2Cx, address, timeout_ms));
		}
		
		return (I2C_MasterWrite_TimeOut(obj->I2Cx, (unsigned char*)data, length,timeout_ms));
	}else{
		if (usi_i2c_target_addr[obj->usi_i2c_idx] != address) {
		/* Deinit USI_I2C first */
			usi_i2c_reset(obj);

			/* Load the user defined USI_I2C target slave address */
			usi_i2c_target_addr[obj->usi_i2c_idx] = address;
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
		}

		if(!length) {
			return (USI_I2C_MasterSendNullData_TimeOut(obj->USI_I2Cx, address, timeout_ms));
		}
		
		return (USI_I2C_MasterWrite_TimeOut(obj->USI_I2Cx, (unsigned char*)data, length,timeout_ms));
	}
}

/**
  * @brief  I2C master read in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  address: slave address which will be transmitted.
  * @param  data: point to the buffer to hold the received data.
  * @param  length: the length of data that to be received.
  * @param  stop: specifies whether a STOP is issued after all the bytes are received.
  * @retval the length of data received. 
  */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) 
{
	/* To avoid gcc warnings */
	( void ) stop;

	if(i2c_sel == 0){
		if (i2c_target_addr[obj->i2c_idx] != address) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);   
		}

		if (!master_addr_retry) {
			I2C_MasterRead(obj->I2Cx, (unsigned char*)data, length);
		} else {
			while (0 == I2C_MasterRead_Patch(obj->I2Cx, (unsigned char*)data, length)) {
				/* Wait for i2c enter trap state from trap_stop state*/
				DelayUs(100);
				
				/* Deinit I2C first */
				i2c_reset(obj);

				/* Load the user defined I2C target slave address */
				i2c_target_addr[obj->i2c_idx] = address;
				I2CInitDat[obj->i2c_idx].I2CAckAddr = address;
				
				/* Init I2C now */		
				I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
				I2C_Cmd(obj->I2Cx, ENABLE); 		
			}
		}
		return length;
	}else{
		if (usi_i2c_target_addr[obj->usi_i2c_idx] != address) {
			/* Deinit USI_I2C first */
			i2c_reset(obj);

			/* Load the user defined USI_I2C target slave address */
			usi_i2c_target_addr[obj->usi_i2c_idx] = address;
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);   
		}

		if (!master_addr_retry) {
			USI_I2C_MasterRead(obj->USI_I2Cx, (unsigned char*)data, length);
		} else {
			while (0 == USI_I2C_MasterRead_Patch(obj->USI_I2Cx, (unsigned char*)data, length)) {
				/* Wait for usi_i2c enter trap state from trap_stop state*/
				DelayUs(100);
				/* Deinit USI_I2C first */
				i2c_reset(obj);

				/* Load the user defined USI_I2C target slave address */
				usi_i2c_target_addr[obj->usi_i2c_idx] = address;
				USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;
				
				/* Init USI_I2C now */		
				USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
				USI_I2C_Cmd(obj->USI_I2Cx, ENABLE); 	
			}
		}
		return length;
	}
}

/**
  * @brief  I2C master write in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  address: slave address which will be transmitted.
  * @param  data: point to the data to be sent.
  * @param  length: the length of data that to be sent.
  * @param  stop: specifies whether a STOP is issued after all the bytes are sent.
  * @retval the length of data send. 
  */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) 
{
	if(i2c_sel == 0){
		if (i2c_target_addr[obj->i2c_idx] != address) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);   
		}

		if ((!restart_enable) |(1==stop)) {
			return (I2C_MasterWrite_Patch(obj->I2Cx, (unsigned char*)data, length));
		} else {
			i2c_send_restart(obj->I2Cx, (unsigned char*)data, length, 1);
		} 
		return length;
	}else{
		if (usi_i2c_target_addr[obj->usi_i2c_idx] != address) {
			/* Deinit USI_I2C first */
			i2c_reset(obj);

			/* Load the user defined USI_I2C target slave address */
			usi_i2c_target_addr[obj->usi_i2c_idx] = address;
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);   
		}

		if ((!restart_enable) |(1==stop)) {
			return (USI_I2C_MasterWrite_Patch(obj->USI_I2Cx, (unsigned char*)data, length));
		} else {
			usi_i2c_send_restart(obj->USI_I2Cx, (unsigned char*)data, length, 1);
		} 
		return length;
	}
}

/**
  * @brief  I2C master send data and read data in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  address: slave address which will be transmitted.
  * @param  pWriteBuf: point to the data to be sent.
  * @param  Writelen: the length of data that to be sent.
  * @param  pReadBuf: point to the buffer to hold the received data.
  * @param  Readlen: the length of data that to be received.  
  * @retval the length of data received. 
  */
int i2c_repeatread(i2c_t *obj, int address, uint8_t *pWriteBuf, int Writelen, uint8_t *pReadBuf, int Readlen) 
{
	u8 cnt = 0;
	
	if(i2c_sel == 0){
		if (i2c_target_addr[obj->i2c_idx] != address) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE); 
		}
		
		/* write in the DR register the data to be sent */
		for(cnt = 0; cnt < Writelen; cnt++)
		{
			while((I2C_CheckFlagState(obj->I2Cx, BIT_IC_STATUS_TFNF)) == 0);
			
			if(cnt >= Writelen - 1)
			{
				/*generate stop signal*/
				obj->I2Cx->IC_DATA_CMD = (*pWriteBuf++) | (1 << 10);
			}
			else
			{
				obj->I2Cx->IC_DATA_CMD = (*pWriteBuf++);
			}
		}
		/*Wait I2C TX FIFO not full*/
		while((I2C_CheckFlagState(obj->I2Cx, BIT_IC_STATUS_TFNF)) == 0);

		I2C_MasterRead(obj->I2Cx, pReadBuf, Readlen);
		
		return Readlen;
	}else{
		if (usi_i2c_target_addr[obj->usi_i2c_idx] != address) {
			/* Deinit USI_I2C first */
			usi_i2c_reset(obj);

			/* Load the user defined USI_I2C target slave address */
			usi_i2c_target_addr[obj->usi_i2c_idx] = address;
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr = address;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE); 
		}
		
		/* write in the DR register the data to be sent */
		for(cnt = 0; cnt < Writelen; cnt++)
		{
			while(obj->USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_FULL != 0);
			
			if(cnt >= Writelen - 1)
			{
				/*generate stop signal*/
				obj->USI_I2Cx->TX_FIFO_WRITE= (*pWriteBuf++) | (1 << 10);
			}
			else
			{
				obj->USI_I2Cx->TX_FIFO_WRITE= (*pWriteBuf++);
			}
		}
		/*Wait USI_I2C TX FIFO not full*/
		while(obj->USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_FULL != 0);

		USI_I2C_MasterRead(obj->USI_I2Cx, pReadBuf, Readlen);
		
		return Readlen;
		
	}
}

/**
  * @brief  I2C master restart after all bytes are sent.
  * @param  I2Cx: where I2Cx can be I2C0_DEV to select the I2C peripheral.
  * @param  pBuf: point to the data to be sent.
  * @param  len: the length of data that to be sent.
  * @param  restart: specifies whether a RESTART is issued after all the bytes are sent.
  * @retval none 
  */
void i2c_send_restart(I2C_TypeDef *I2Cx, u8* pBuf, u32 len, u8 restart)
{
	u32 cnt = 0;
	
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFNF)) == 0);	
		
		if(cnt >= len - 1){
			/*generate restart signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (restart << 10);
		}else{
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}
	}
	
	while((I2C_CheckFlagState(I2Cx, BIT_IC_STATUS_TFE)) == 0);
}

/**
  * @brief  I2C master restart after all bytes are sent.
  * @param  I2Cx: where I2Cx can be USI0_DEV to select the I2C peripheral.
  * @param  pBuf: point to the data to be sent.
  * @param  len: the length of data that to be sent.
  * @param  restart: specifies whether a RESTART is issued after all the bytes are sent.
  * @retval none 
  */
void usi_i2c_send_restart(USI_TypeDef *USI_I2Cx, u8* pBuf, u8 len, u8 restart)
{
	u8 cnt = 0;
	
	/* Check the parameters */
	assert_param(IS_USI_I2C_ALL_PERIPH(USI_I2Cx));
	
	/* Write in the DR register the data to be sent */
	for(cnt = 0; cnt < len; cnt++)
	{
		while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_FULL != 0);
		
		if(cnt >= len - 1)
		{
			/*generate restart signal*/
			USI_I2Cx->TX_FIFO_WRITE = (*pBuf++) | (restart << 10);
		}
		else
		{
			USI_I2Cx->TX_FIFO_WRITE = (*pBuf++);
		}
	}
	while(USI_I2Cx->TX_FIFO_STATUS&USI_TXFIFO_EMPTY == 0);
}

/**
  * @brief  I2C master receive single byte.
  * @param  obj: i2c object defined in application software.
  * @param  last: hold the received data.
  * @retval the data that received. 
  */
int i2c_byte_read(i2c_t *obj, int last)
{
	/* To avoid gcc warnings */
	( void ) last;
	
	uint8_t i2cdatlocal;

	if(i2c_sel == 0){
		I2C_MasterRead(obj->I2Cx, &i2cdatlocal, 1);
	}else{
		USI_I2C_MasterRead(obj->USI_I2Cx, &i2cdatlocal, 1);
	}

	return (int)i2cdatlocal;
}

/**
  * @brief  I2C master send single byte.
  * @param  obj: i2c object defined in application software.
  * @param  data: the data to be sent.
  * @retval result. 
  */
int i2c_byte_write(i2c_t *obj, int data) 
{
	uint8_t i2cdatlocal = data;

	if(i2c_sel == 0){
		I2C_MasterWrite(obj->I2Cx, &i2cdatlocal, 1);
	}else{
		USI_I2C_MasterRead(obj->USI_I2Cx, &i2cdatlocal, 1);
	}

	return 1;
}

/**
  * @brief  Deinitializes the I2C device
  * @param  obj: i2c object defined in application software.
  * @retval none  
  */
void i2c_reset(i2c_t *obj) 
{
	/* Deinit I2C directly */
	/* I2C HAL DeInitialization */
	if(i2c_sel == 0){
		I2C_Cmd(obj->I2Cx, DISABLE);
	}else{
		USI_I2C_Cmd(obj->USI_I2Cx, DISABLE);
	}
}

/**
  * @brief  Enable i2c master RESTART function
  * @param  obj: i2c object defined in application software.
  * @retval none  
  */
void i2c_restart_enable(i2c_t *obj)
{
	if(i2c_sel == 0){
		uint32_t i2cen;

		if (obj->I2Cx->IC_ENABLE & BIT_CTRL_IC_ENABLE) {
			I2C_Cmd(obj->I2Cx, DISABLE);
			i2cen = 1;
		}

		obj->I2Cx->IC_CON |= BIT_CTRL_IC_CON_IC_RESTART_EN;

		if (i2cen) {
			I2C_Cmd(obj->I2Cx, ENABLE);
		}

		restart_enable = 1;
	}else{
		uint32_t usi_i2cen;

		if (obj->USI_I2Cx->I2C_ENABLE & USI_I2C_ENABLE) {
			USI_I2C_Cmd(obj->USI_I2Cx, DISABLE);
			usi_i2cen = 1;
		}

		obj->USI_I2Cx->I2C_CTRL|= USI_I2C_RESTART_EN;
		
		if (usi_i2cen) {
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
		}

		restart_enable = 1;
	}
}

/**
  * @brief  Disable i2c Master RESTART function
  * @param  obj: i2c object defined in application software.
  * @retval none  
  */
void i2c_restart_disable(i2c_t *obj)
{
	if(i2c_sel == 0){
		uint32_t i2cen;

		if (obj->I2Cx->IC_ENABLE & BIT_CTRL_IC_ENABLE) {
			I2C_Cmd(obj->I2Cx, DISABLE);
			i2cen = 1;
		}

		obj->I2Cx->IC_CON &= ~BIT_CTRL_IC_CON_IC_RESTART_EN;

		if (i2cen) {
			I2C_Cmd(obj->I2Cx, ENABLE);
		}

		restart_enable = 0;
	}else{
		uint32_t usi_i2cen;

		if (obj->USI_I2Cx->I2C_ENABLE & USI_I2C_ENABLE) {
			USI_I2C_Cmd(obj->USI_I2Cx, DISABLE);
			usi_i2cen = 1;
		}

		obj->USI_I2Cx->I2C_CTRL &= ~USI_I2C_RESTART_EN;

		if (usi_i2cen) {
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
		}

		restart_enable = 0;
	}
}

/**
  * @brief  Enable/Disable i2c Device
  * @param  obj: i2c object defined in application software.
  * @param  enable: this parameter can be one of the following values:
  *		@arg 0 disable
  *		@arg 1 enable
  * @retval result  
  */
int i2c_enable_control(i2c_t *obj, int enable)
{
	if(i2c_sel == 0){
		if (enable) {
			I2C_Cmd(obj->I2Cx, ENABLE);
		} else {
			I2C_Cmd(obj->I2Cx, DISABLE);
		}
	}else{
		if (enable) {
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
		} else {
			USI_I2C_Cmd(obj->USI_I2Cx, DISABLE);
		}
	}
        return 0;
}

//#if DEVICE_I2CSLAVE
/**
  * @brief  Set i2c slave address.
  * @param  obj: i2c object defined in application software.
  * @param  idx: i2c index, this parameter can be :
  *		@arg 0: I2C0 Device
  * @param  address: slave address.
  * @param  mask: the mask of address  
  * @retval none  
  */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) 
{
	/* To avoid gcc warnings */
	( void ) idx;
	( void ) mask;

	if(i2c_sel == 0){
		uint16_t i2c_default_addr   = (uint16_t) I2CInitDat[obj->i2c_idx].I2CAckAddr;
		uint16_t i2c_user_addr      = (uint16_t) address;

		if (i2c_default_addr != i2c_user_addr) {
			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C clock */
			I2CInitDat[obj->i2c_idx].I2CAckAddr    = i2c_user_addr;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);  
		}
	}else{
		uint16_t usi_i2c_default_addr   = (uint16_t) USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr;
		uint16_t usi_i2c_user_addr      = (uint16_t) address;

		if (usi_i2c_default_addr != usi_i2c_user_addr) {
			/* Deinit USI_I2C first */
			i2c_reset(obj);

			/* Load the user defined USI_I2C clock */
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CAckAddr    = usi_i2c_user_addr;

			/* Init USI_I2C now */
			USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
			USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);  
		}
	}
}

/**
  * @brief  Set i2c device to be slave.
  * @param  obj: i2c object defined in application software.
  * @param  enable_slave: enable slave function, this parameter can be one of the following values:
  *		@arg 0 disable
  *		@arg 1 enable
  * @retval none  
  */
void i2c_slave_mode(i2c_t *obj, int enable_slave) 
{
	if(i2c_sel == 0){
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C clock */
		I2CInitDat[obj->i2c_idx].I2CMaster = I2C_MASTER_MODE;
		if (enable_slave) 
			I2CInitDat[obj->i2c_idx].I2CMaster = I2C_SLAVE_MODE;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}else{
		/* Deinit USI_I2C first */
		i2c_reset(obj);

		/* Load the user defined USI_I2C clock */
		USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CMaster = USI_I2C_MASTER_MODE;
		if (enable_slave) 
			USI_I2CInitDat[obj->usi_i2c_idx].USI_I2CMaster = USI_I2C_SLAVE_MODE;

		/* Init USI_I2C now */
		USI_I2C_Init(obj->USI_I2Cx, &USI_I2CInitDat[obj->usi_i2c_idx]);
		USI_I2C_Cmd(obj->USI_I2Cx, ENABLE);
	}
}

/**
  * @brief  Get i2c slave state.
  * @param  obj: i2c object defined in application software.
  * @retval the state of i2c slave.
  */
int i2c_slave_receive(i2c_t *obj) 
{
	if(i2c_sel == 0){
		u32 I2CLocalTemp = I2C_GetRawINT(obj->I2Cx);

		if (I2CLocalTemp & BIT_IC_RAW_INTR_STAT_GEN_CALL) {
			return WriteGeneral;
		}
		else if (I2CLocalTemp & BIT_IC_RAW_INTR_STAT_RD_REQ) {
			return ReadAddressed;
		}

		if (I2C_CheckFlagState(obj->I2Cx, BIT_IC_STATUS_RFNE)) {
			return WriteAddressed;
		}
	        return 0;
	}else{
		u32 USI_I2CLocalTemp = USI_I2C_GetRawINT(obj->USI_I2Cx);

		if (USI_I2CLocalTemp & USI_I2C_GEN_CALL_RSTS) {
			return WriteGeneral;
		}
		else if (USI_I2CLocalTemp & USI_I2C_RD_REQ_RSTS) {
			return ReadAddressed;
		}

		if (obj->USI_I2Cx->RX_FIFO_STATUS&USI_RXFIFO_EMPTY != 0) {
			return WriteAddressed;
		}
	        return 0;
	}
}

/**
  * @brief  I2C slave read in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  data: point to the buffer to hold the received data.
  * @param  length: the length of data that to be received.
  * @retval the length of data received. 
  */
int i2c_slave_read(i2c_t *obj, char *data, int length) 
{
	if(i2c_sel == 0)
		I2C_SlaveRead(obj->I2Cx, (unsigned char*)data, length);
	else
		USI_I2C_SlaveRead(obj->USI_I2Cx, (unsigned char*)data, length);
	
	return length;
}

/**
  * @brief  I2C slave write in poll mode.
  * @param  obj: i2c object defined in application software.
  * @param  data: point to the data to be sent.
  * @param  length: the length of data that to be sent.
  * @retval result. 
  */
int i2c_slave_write(i2c_t *obj, const char *data, int length) 
{	
	if(i2c_sel == 0)
		I2C_SlaveWrite(obj->I2Cx, (unsigned char*)data, length);
	else
		USI_I2C_SlaveWrite(obj->USI_I2Cx, (unsigned char*)data, length);
	return 1;
}

/**
  * @brief  Set/clear i2c slave RD_REQ interrupt mask.
  * @param  obj: i2c object defined in application software.
  * @param  set: set or clear for read request.
  * @retval result. 
  */
int i2c_slave_set_for_rd_req(i2c_t *obj, int set)
{
	if(i2c_sel == 0){
		if (set) {
			I2C_INTConfig(obj->I2Cx, BIT_IC_INTR_MASK_M_RD_REQ, ENABLE);
		} else {
			I2C_INTConfig(obj->I2Cx, BIT_IC_INTR_MASK_M_RD_REQ, DISABLE);
		}
	}else{
			if (set) {
			USI_I2C_INTConfig(obj->USI_I2Cx, USI_I2C_RD_REQ_INTER_EN, ENABLE);
		} else {
			USI_I2C_INTConfig(obj->USI_I2Cx, USI_I2C_RD_REQ_INTER_EN, DISABLE);
		}
	}

	return _TRUE;
}

/**
  * @brief  Set/clear i2c slave NAK or ACK data part in transfer.
  * @param  obj: i2c object defined in application software.
  * @param  set_nak: set or clear for data NAK.
  * @retval result. 
  */
int i2c_slave_set_for_data_nak(i2c_t *obj, int set_nak)
{
	if(i2c_sel == 0){
		I2C_TypeDef * I2Cx = obj->I2Cx;
		u32 temp;

		while (1) {
			temp = I2Cx->IC_STATUS;

			if ((BIT_IC_STATUS_SLV_ACTIVITY & temp) == 0) {
				break;
			}
		}
		
		I2Cx->IC_SLV_DATA_NACK_ONLY = set_nak;
	        return 0;
	}else{
		USI_TypeDef * USI_I2Cx = obj->USI_I2Cx;
		u32 temp;

		while (1) {
			temp = USI_I2Cx->I2C_STATUS;

			if ((USI_I2C_SLV_ACTIVITY & temp) == 0) {
				break;
			}
		}

		if(0 == set_nak){
			USI_I2Cx->I2C_SLV_ACK_CTRL &= (~USI_I2C_SLV_DATA_NACK_ONLY);
		}else{
			USI_I2Cx->I2C_SLV_ACK_CTRL &= (USI_I2C_SLV_DATA_NACK_ONLY);
		}
	        return 0;
	}
}
/** 
  * @}
  */

/** 
  * @}
  */

/** 
  * @}
  */
  
//#endif // CONFIG_I2C_SLAVE_EN
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
