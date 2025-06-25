/**
  ******************************************************************************
  * @file    littlefs_spi_nand.c
  * @author
  * @version V1.0.0
  * @date    2023-12
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the SPIC peripheral:
  *           - Read/Program/Erase configuration
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"
#include "littlefs_spi_nand.h"
#include "spi_api.h"
#include "spi_ex_api.h"

#define CMD_LENGTH_ONE          1           // Command length is 1 byte
#define CMD_LENGTH_TWO          2           // Command length is 2 byte
#define CMD_LENGTH_THREE        3           // Command length is 3 byte
#define CMD_LENGTH_FOUR         4           // Command length is 4 byte
#define CMD_LENGTH_FIVE         5           // Command length is 5 byte


NandFlash_InitTypeDef g_nandflash_init_para;

/* for dma mode, start address of buffer should be 32-byte aligned*/
static u8 Spi_MasterTxBuf[SPI_BUFFER_SIZE] __attribute__((aligned(CACHE_LINE_SIZE)));
static u8 Spi_MasterRxBuf[SPI_BUFFER_SIZE] __attribute__((aligned(CACHE_LINE_SIZE)));

spi_t nand_spi_master;

volatile int spi_master_tx_done;
volatile int spi_master_rx_done;

static void Spi_Master_tr_done_callback(uint32_t id, SpiIrq event)
{
	(void)id;
	switch (event) {
	case SpiRxIrq:
		spi_master_rx_done = 1;
		break;
	case SpiTxIrq:
		spi_master_tx_done = 1;
		break;
	default:
		LFS_NAND_DBG(LFS_NAND_WARNING, "unknown interrput evnent!");
	}
}

void NandFlash_Reset(void)
{
	spi_master_write(&nand_spi_master, 0xFF);
	DelayMs(1);
}


u8 NandFlash_ReadID(u8 *flash_id)
{
	int i = 0;
	int ret = 0;

	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_tx_done = 0;
	spi_master_rx_done = 0;

	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_rd_id;
	Spi_MasterTxBuf[1] = 0xFF;

	ret = spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_FIVE);

	//ret = spi_master_write_stream_dma(&nand_spi_master, (char*)Spi_MasterTxBuf, 2);
	//ret = spi_master_read_stream_dma(&nand_spi_master, (char*)Spi_MasterRxBuf, 3);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			ret = HAL_TIMEOUT;
			LFS_NAND_DBG(LFS_NAND_ERROR, "SPI Wait Timeout!\n");
			break;
		}
	}
	if (HAL_OK == ret) {
		flash_id[0] = Spi_MasterRxBuf[2];
		flash_id[1] = Spi_MasterRxBuf[3];
		flash_id[2] = Spi_MasterRxBuf[4];
	} else {
		LFS_NAND_DBG(LFS_NAND_INFO, ": FAILED");
	}

	return ret;
}


/**
* @brief  This function is used to wait until Flash Busy done or WLE bit set.
* @param  WaitType:  wait type, which can be one of the following parameters:
*			@arg WAIT_FLASH_BUSY: wait until Flash busy done.
*			@arg WAIT_WRITE_EN: wait until flash status WLE(Write Enbale Latch) bit is set.
* @retval status:
*			bit 0:Erase/Program In Progress (BUSY)
*			bit 1:Write Enable Latch (WEL)
*			Others.(timeout is 0xFF)
**/
u8 NandFlash_WaitBusy(u32 WaitType)
{
	u8 status = 0;
	u32 BusyCheck = 0;
	u32 i = 0;

	do {
		if (WaitType == WAIT_FLASH_BUSY) {
			i++;
			status = NandFlash_GetStatus(NAND_REG_STATUS);
			BusyCheck = (status & flash_init_para.FLASH_Busy_bit);
		} else if (WaitType == WAIT_WRITE_EN) {
			i++;
			status = NandFlash_GetStatus(NAND_REG_STATUS);
			/* write enable ok & flash not busy */
			BusyCheck = (!(status & flash_init_para.FLASH_WLE_bit)) || (status & flash_init_para.FLASH_Busy_bit);
		}
		if (!BusyCheck) {
			break;
		}

		if (i > 0x200000) {
			status = 0xFF;
			break;
		}
	} while (1);

	return status;
}

/**
* @brief  This function is used to send WREN(Write Enable) cmd to set WEL bit in flash status register.
* @param  none
* @note   This function must be used prior to every Program execute,Block Erase, OTP program/OTP protection.
* @retval none
*/

void NandFlash_WriteEn(void)
{
	/* Wait for flash busy done */
	NandFlash_WaitBusy(WAIT_FLASH_BUSY);

	/* Set flash_cmd: WREN to FIFO */
	spi_master_write(&nand_spi_master, g_nandflash_init_para.FLASH_cmd_wr_en);

	/* Wait for write enable done*/
	NandFlash_WaitBusy(WAIT_WRITE_EN);
}

/**
* @brief    NandFlash_GetStatus used to get register status.
* @param    FeatureID: the address of status register
* @retval   the value of the status register
*			C0h register: bit 0:Erase/Program In Progress (BUSY)
*			             bit 1:Write Enable Latch (WEL)
*			Others.(timeout is 0xFF)
*/
u8 NandFlash_GetStatus(u8 FeatureID)
{
	int ret = 0;
	u8 status = 0xFF;
	int i = 0;

	_memset(Spi_MasterTxBuf, 0xFF, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_rd_status;
	Spi_MasterTxBuf[1] = FeatureID;

	ret = spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_THREE);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			LFS_NAND_DBG(LFS_NAND_ERROR, "Get Feature Timeout!\n");
			return status;
		}
	}

	if (HAL_OK == ret) {
		status = Spi_MasterRxBuf[2];
	}

	return status;
}

/**
  * @brief  NandFlash_SetStatus used to set register status. NAND_WaitBusy
  *  		are included in this function to avoid hardfault when TxCmd in XIP
  * @param    FeatureID: the address of status register
  * @param    Status: pointer to byte array to be sent
  * @retval   Execute status
  */

u8 NandFlash_SetStatus(u8 FeatureID, u8 Status)
{
	int i = 0;
	int ret = 0;

	_memset(Spi_MasterTxBuf, 0xFF, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_wr_status;
	Spi_MasterTxBuf[1] = FeatureID;
	Spi_MasterTxBuf[2] = Status;

	ret = spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_THREE);

	//ret = spi_master_write_stream_dma(&nand_spi_master, (char*)Spi_MasterTxBuf, CMD_LENGTH_THREE);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			LFS_NAND_DBG(LFS_NAND_ERROR, "Set Status Timeout");
			return 0xFF;
		}
	}
	return ret;
}

/**
  * @brief  NandFlash_SetStatusBits set or clear status bits., used to set protect bit or quad enable bit
  * @param  FeatureID: the address of status register
  * @param  SetBits: bit mask in the status register
  * @param  NewState: ENABLE/DISABLE
  * @retval none
  */

void NandFlash_SetStatusBits(u8 FeatureID, u8 SetBits, u8 NewState)
{
	u8 status = 0;
	u8 status_mask = (u8)(SetBits & 0xFF);


	status = NandFlash_GetStatus(FeatureID);
	if (0xFF == status) { //Timeout
		LFS_NAND_DBG(LFS_NAND_ERROR, ": TIMEOUT");
		return;
	}

	if (NewState == ENABLE) {
		status |= status_mask;
	} else {
		status &= ~status_mask;
	}

	status = NandFlash_SetStatus(FeatureID, status);
	if (status) {
		LFS_NAND_DBG(LFS_NAND_ERROR, "Set Status FAILED");
	}
	return;
}

/**
  * @brief  This function is used to erase flash. EraseBlock: Erase specified block(128KB)
  * @param  PageAddr: should 4 byte align. The block which the address in will be erased.
  * @retval status: Success:0 or erase Fail: Others.(timeout is 0xFF)
  */

u8 NandFlash_Erase(u32 PageAddr)
{
	u8 status;
	int i = 0;

	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	/* Block Erase Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_block_e;
	Spi_MasterTxBuf[1] = (u8)((PageAddr >> 16) & 0xFF);
	Spi_MasterTxBuf[2] = (u8)((PageAddr >> 8) & 0xFF);
	Spi_MasterTxBuf[3] = (u8)(PageAddr & 0xFF);

	spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_FOUR);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			LFS_NAND_DBG(LFS_NAND_INFO, "SPI Wait Timeout");
			break;
		}
	}

	/* polling WEL and busy done */
	status = NandFlash_WaitBusy(WAIT_FLASH_BUSY);

	if (status & g_nandflash_init_para.FLASH_EFail_bit) {
		return status;
	} else {
		return 0;
	}
}

/**
  * @brief  This function is used to read data from Nand array to Nand Cache.
  * @param  PageAddr: which page will be read
  * @retval status: NAND_STATUS_ECC_NO_BITFLIPS:0 or Need to check: Others.(timeout is 0xFF)
  * @note
  * 	 if status is 0 means NAND_STATUS_ECC_NO_BITFLIPS, otherwise need confirm spi nand datasheet.
  */

u8 NandFlash_Page_Read_ArrayToCache(u32 PageAddr)
{
	u8 status = 0;
	int i = 0;
	u8 ret = 0;

	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	/* Page read Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_page_read;
	Spi_MasterTxBuf[1] = (u8)((PageAddr >> 16) & 0xFF);
	Spi_MasterTxBuf[2] = (u8)((PageAddr >> 8) & 0xFF);
	Spi_MasterTxBuf[3] = (u8)(PageAddr & 0xFF);

	ret = spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_FOUR);

	//spi_master_write_stream_dma(&nand_spi_master,(char*) Spi_MasterTxBuf, CMD_LENGTH_FOUR);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			LFS_NAND_DBG(LFS_NAND_INFO, "SPI ArrayToCache Timeout");
			break;
		}
	}
	if (HAL_OK == ret) {
		status = NandFlash_WaitBusy(WAIT_FLASH_BUSY);
	}

	return status;
}

/**
  * @brief  This function is used to read data from cache
  * @param  ByteAddr: the offset of the page
  * @param  ByteLen: the number of bytes to be read
  * @param  pData: pointer to a byte array which is used to save received data.
  * @retval none
  */
void NandFlash_Page_Read_FromCache(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
	int i = 0;
	u8 ret = 0;

	_memset(Spi_MasterTxBuf, 0xFF, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	Spi_MasterTxBuf[0] = Cmd;
	Spi_MasterTxBuf[1] = (ByteAddr >> 8) & 0x0F;
	Spi_MasterTxBuf[2] = ByteAddr & 0xFF;
	Spi_MasterTxBuf[3] = 0xFF;

	ret = spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, (ByteLen + CMD_LENGTH_FOUR));

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			LFS_NAND_DBG(LFS_NAND_ERROR, "SPI ReadFromCache Timeout");
			break;
		}
	}
	if (HAL_OK == ret) {
		memcpy(pData, Spi_MasterRxBuf + CMD_LENGTH_FOUR, ByteLen);
	}

	return;
}

/**
  * @brief  This function is used to Load data to Nand Cache
  * @param  cmd: The command need to be sent, Can be NAND_CMD_QPP_RANDOM/NAND_CMD_QPP/NAND_CMD_PP_RANDOM/NAND_CMD_PP
  * @param  ByteAddr: the offset of the page
  * @param  ByteLen: the number of bytes to be transfered
  * @param	pData: pointer to a byte array that is to be sent.
  * @retval null
  * @note
  */
void NandFlash_Page_Write_Data_Xfer(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
	int i = 0;

	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	Spi_MasterTxBuf[0] = Cmd;
	Spi_MasterTxBuf[1] = (ByteAddr >> 8) & 0xFF;
	Spi_MasterTxBuf[2] = ByteAddr & 0xFF;

	memcpy(Spi_MasterTxBuf + CMD_LENGTH_THREE, pData, ByteLen);

	spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, (ByteLen + CMD_LENGTH_THREE));
	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			printf("\r\nSPI Wait Timeout\r\n");
			break;
		}
	}

	return;
}

/**
  * @brief  This function is used to program Page
  * @param  PageAddr: which page will be program
  * @retval status: Success:0 or Program Fail: Others.(timeout is 0xFF)
  */
u8 NandFlash_Page_Write_Program_Execute(u32 PageAddr)
{
	u8 status;
	int i = 0;

	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_rx_done = 0;
	spi_master_tx_done = 0;

	/* Program Execution Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	Spi_MasterTxBuf[0] = g_nandflash_init_para.FLASH_cmd_page_write;
	Spi_MasterTxBuf[1] = (u8)((PageAddr >> 16) & 0xFF);
	Spi_MasterTxBuf[2] = (u8)((PageAddr >> 8) & 0xFF);
	Spi_MasterTxBuf[3] = (u8)(PageAddr & 0xFF);

	spi_master_write_read_stream_dma(&nand_spi_master, (char *) Spi_MasterTxBuf, (char *)Spi_MasterRxBuf, CMD_LENGTH_FOUR);

	while (spi_master_rx_done == 0) {
		DelayMs(1);
		i++;
		if (i > 150) {
			printf("\r\nSPI Wait Timeout\r\n");
			break;
		}
	}

	/* polling WEL and busy done */
	status = NandFlash_WaitBusy(WAIT_FLASH_BUSY);

	if (status & g_nandflash_init_para.FLASH_PFail_bit) {
		return status;
	} else {
		return 0;
	}
}


/**
  * @brief  Configure SPI
  * @param  void
  * @retval void
  */
void nandflash_spi_init(void)
{
	/* SPI0 is as Master */
	nand_spi_master.spi_idx = SPI_NAND_NODE;
	spi_init(&nand_spi_master, SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CS);
	spi_format(&nand_spi_master, SPI_DATA_FRAME_SIZE_8_BITS, SPI_MODE_0, SPI_ROLE_MASTER);
	spi_frequency(&nand_spi_master, SPI_SCLK_FREQ);// IpClk of SPI1 is 50MHz, IpClk of SPI0 is 100MHz

	spi_irq_hook(&nand_spi_master, (spi_irq_handler)Spi_Master_tr_done_callback, (uint32_t)&nand_spi_master);
	_memset(Spi_MasterTxBuf, 0, SPI_BUFFER_SIZE);
	_memset(Spi_MasterRxBuf, 0, SPI_BUFFER_SIZE);
	spi_master_tx_done = 0;
	spi_master_rx_done = 0;
	DelayMs(500);

	LFS_NAND_DBG(LFS_NAND_INFO, ":DONE");

	return;
}


/**
* @brief  Initializes the parameters in the NandFlash_InitTypeDef with
*		its default values according to Winbond spec.
* @param FLASH_InitStruct: pointer to a NandFlash_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NandFlash_StructInit_DS(NandFlash_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_WINBOND;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;

	/* status bit define */
	/* winbond QSPI is enable by default, set Bit1 of feature 0xA0 to disable QSPI*/
	/* WP#, HOLD# are disabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;
#if 0  //Feature to be expanded for future
	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = NAND_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = NAND_DM_CYCLE_4IO;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = NAND_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = NAND_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;
#endif
	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	FLASH_InitStruct->FLASH_pagesize_main_bit_exp = NAND_PAGE_SIZE_MAIN_2K_BIT_EXP;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the NandFlash_InitTypeDef with
*		its default values according to Micron Spec.
* @param FLASH_InitStruct: pointer to a NandFlash_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NandFlash_StructInit_Micron(NandFlash_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MICRON;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;

	/* status bit define */
	/* micron QSPI is enable by default, and cannot be disabled!! */
	/* WP#, HOLD# are enabled by default, and can be disabled by set bit1 of feature 0xA0*/
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;

#if 0  //Feature to be expanded for future
	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0; //NAND_DM_CYCLE_READ;
	/* set Micron rd_dummy_cyle based on baudrate, default 100MHz */
	/* FLASH_BAUD_100M */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x05; //NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x05; //NAND_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = 0x09; //NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0x09; //NAND_DM_CYCLE_4IO;
	/* FLASH_BAUD_50M */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x03;
	/* FLASH_BAUD_25M */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x01;
	/* < 25MHz */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x01;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_wr_dual_i = 0xA2;
	FLASH_InitStruct->FLASH_wr_dual_ii = 0xD2;
	FLASH_InitStruct->FLASH_rd_dual_io = 0xBB; //NAND_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = 0x3B; //NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_wr_quad_i = 0x32;
	FLASH_InitStruct->FLASH_wr_quad_ii = 0x12;
	FLASH_InitStruct->FLASH_rd_quad_io = 0xEB; //NAND_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = 0x6B; //NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

#endif

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	FLASH_InitStruct->FLASH_pagesize_main_bit_exp = NAND_PAGE_SIZE_MAIN_2K_BIT_EXP;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 3);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the NandFlash_InitTypeDef with
*		its default values according to MXIC Spec.
* @param FLASH_InitStruct: pointer to a NandFlash_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NandFlash_StructInit_MXIC(NandFlash_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MXIC;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;

	/* status bit define */
	/* mxic  QE bit is bit0 of feature 0xB0, QSPI is disabled by default */
	/* WP#, HOLD# are enabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = NAND_CFG_QUAD_ENABLE;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;
#if 0  //Feature to be expanded for future
	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = 0;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = 0;
#endif
	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	FLASH_InitStruct->FLASH_pagesize_main_bit_exp = NAND_PAGE_SIZE_MAIN_2K_BIT_EXP;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the NandFlash_InitTypeDef with
*		its default values according to Gigadevice spec.
* @param FLASH_InitStruct: pointer to a NandFlash_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NandFlash_StructInit_GD(NandFlash_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_GD;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;

	/* status bit define */
	/* gigadevice QE bit is bit0 of feature 0xB0, QSPI is disabled by default */
	/* WP#, HOLD# are enabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = NAND_CFG_QUAD_ENABLE;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;
#if 0  //Feature to be expanded for future
	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = 0;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = 0;
#endif
	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;

	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	FLASH_InitStruct->FLASH_pagesize_main_bit_exp = NAND_PAGE_SIZE_MAIN_2K_BIT_EXP;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the NandFlash_InitTypeDef with
*		its default values according to Winbond spec.
* @param FLASH_InitStruct: pointer to a NandFlash_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NandFlash_StructInit(NandFlash_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_WINBOND;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;

	/* status bit define */
	/* winbond QSPI is enable by default, set Bit1 of feature 0xA0 to disable QSPI*/
	/* WP#, HOLD# are disabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;
#if 0  //Feature to be expanded for future
	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = NAND_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = NAND_DM_CYCLE_4IO;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = NAND_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = NAND_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;
#endif
	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	FLASH_InitStruct->FLASH_pagesize_main_bit_exp = NAND_PAGE_SIZE_MAIN_2K_BIT_EXP;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}

#if 0  //Feature to be expanded for future
/**
* @brief  Init SPIC in specified SPI mode
* @param SpicBitMode: SPI mode to be configured.
*		This parameter can be one of the following parameters:
*		@arg SpicOneBitMode
*		@arg SpicDualOBitMode
*		@arg SpicDualIOBitMode
*		@arg SpicQuadOBitMode
*		@arg SpicQuadIOBitMode
* @retval 1
*/

u8 NandFlash_Init(u8 SpicBitMode)
{
	/* config fixed parameters */
	SPIC_Config(&g_nandflash_init_para);

	/* Set SPI Mode */
	g_nandflash_init_para.FLASH_cur_bitmode = SpicBitMode;
	NandFlash_SetSpiMode(&g_nandflash_init_para, SpicBitMode);

	return _TRUE;
}
#endif

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor *****END OF FILE****/
