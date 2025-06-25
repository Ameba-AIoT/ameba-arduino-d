/**
  ******************************************************************************
  * @file    littlefs_spi_nand.h
  * @author  
  * @date    2023-12
  * @brief   This file is the header file for littlefs_spi_nand.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _LITTLEFS_SPI_NAND_H
#define _LITTLEFS_SPI_NAND_H

//#include "ameba.h"
#include "osdep_service.h"

#include "device.h"


/* Exported defines ----------------------------------------------------------*/

/*SPIx pin location:

SPI0:

	S0: PB_18  (MOSI)
		PB_19  (MISO)
		PB_20  (SCLK)
		PB_21  (CS)


	S1: PA_16  (MOSI)
		PA_17  (MISO)
		PA_18  (SCLK)
		PA_19  (CS)

SPI1:

	S0: PA_12  (MOSI)
		PA_13  (MISO)
		PA_14  (SCLK)
		PA_15  (CS)

	S1: PB_4  (MOSI)
		PB_5  (MISO)
		PB_6  (SCLK)
		PB_7  (CS)
*/
// SPI0 (S1)
#define SPI0_MOSI  PA_16
#define SPI0_MISO  PA_17
#define SPI0_SCLK  PA_18
#define SPI0_CS    PA_19

// SPI1 (S1)
#define SPI1_MOSI  PA_12 //PB_4
#define SPI1_MISO  PA_13 //PB_5
#define SPI1_SCLK  PA_14 //PB_6
#define SPI1_CS    PA_15 //PB_7

#define SPI_NAND_NODE MBED_SPI1

#define SPI_DATA_FRAME_SIZE_8_BITS  8
//#define dfs_mask		0xFF
#define SPI_MODE_0      0
#define SPI_MODE_1      1
#define SPI_MODE_2      2
#define SPI_MODE_3      3

#define SPI_ROLE_MASTER     0
#define SPI_ROLE_SLAVE      1

#define SPI_SCLK_FREQ       300000
#define SPI_BUFFER_SIZE     (2048+32) //for dma mode, buffer size should be multiple of 32-byte


/** @defgroup NAND_Page_Size_Definitions
  * @{
  */
#define NAND_PAGE_SIZE_MAIN_2K_BIT_EXP 		(11)
#define NAND_PAGE_SIZE_MAIN_4K_BIT_EXP 		(12)
/**
 * @}
 */


/** @defgroup NAND_FLASH_Default_CMD
  * @{
  */
#define NAND_CMD_WREN			0x06            //write enable
#define NAND_CMD_WRDI			0x04            //write disable
#define NAND_CMD_RDID			0x9F            //read idenfication
#define NAND_CMD_WRSR			0x1F            //set feature, write status register
#define NAND_CMD_RDSR			0x0F            //get feature, read status register

#define NAND_CMD_READ			0x03            //read data
#define NAND_CMD_FREAD			0x0B            //fast read data
#define NAND_CMD_DREAD			0x3B            //Double Output Mode command	1-1-2
#define NAND_CMD_2READ			0xBB            // 2 x I/O read  command	1-2-2//Not In MXIC
#define NAND_CMD_QREAD			0x6B            // 1I / 4O read command		1-1-4
#define NAND_CMD_4READ			0xEB            // 4 x I/O read  command	1-4-4//Not In MXIC

#define NAND_CMD_RESET			0xFF            //Device Reset
#define NAND_CMD_PAGERD			0x13            //Page Read
#define NAND_CMD_PROMEXEC		0x10            //Program Execute
#define NAND_CMD_BE				0xD8            //128K Block Erase

#define NAND_CMD_PP				0x02            //Page load
#define NAND_CMD_PP_RANDOM		0x84            //Page load random(do not reset buffer)
#define NAND_CMD_QPP			0x32            //quad page load	1-1-4
#define NAND_CMD_QPP_RANDOM		0x34            //quad page load random	1-1-4(do not reset buffer)
/** @} */

/** @defgroup SPIC_ADDR_PHASE_LEN_definitions
  * @{
  * @brief Winbond/MXIC READ/PP CMD ADDR Sequence is CA15-8/CA7-0, so ADDR_LEN is x02
  * GigaDevice is Special:
  * READ/FREAD/DREAD/QREAD CMD ADDR Sequence is Dummy/CA15-8/CA7-0, so ADDR_LEN is x03,
  * 2READ/4READ CMD ADDR Sequence is CA15-8/CA7-0, so ADDR_LEN shall be changed to x02.
  */
#define NAND_COL_ADDR_2_BYTE			0x2
#define NAND_COL_ADDR_3_BYTE			0x3
#define NAND_COL_ADDR_4_BYTE			0x4
/** @} */

/** @defgroup SPI_NAND_Status_Register_group
  * @{
  */
/* feature register */
#define NAND_REG_BLOCK_LOCK				0xA0
#define NAND_BL_ALL_UNLOCKED			0x00

/* configuration register */
#define NAND_REG_CFG					0xB0
#define NAND_CFG_OTP_ENABLE				BIT(6)
#define NAND_CFG_ECC_ENABLE				BIT(4)
#define NAND_CFG_QUAD_ENABLE			BIT(0)	/*Winband is diff with MXIC*/

/* status register */
#define NAND_REG_STATUS					0xC0
#define NAND_STATUS_BUSY				BIT(0)
#define NAND_STATUS_WEL					BIT(1)
#define NAND_STATUS_ERASE_FAILED		BIT(2)
#define NAND_STATUS_PROG_FAILED			BIT(3)
#define NAND_STATUS_ECC_MASK			BIT_OFFSET_LEN_MASK_32(4, 3) /*this is diff with other Spi Nand Flash*/
#define NAND_STATUS_ECC_NO_BITFLIPS		(0 << 4)
#define NAND_STATUS_ECC_HAS_BITFLIPS	(1 << 4)
#define NAND_STATUS_ECC_UNCOR_ERROR		(2 << 4)
/** @} */



/* Exported types ------------------------------------------------------------*/

/**
  * @brief  FLASH Init Structure Definition
  */
typedef struct {
	u32 FLASH_Id;				/*!< Specifies the flash vendor ID.
								This parameter can be a value of @ref FLASH_Vendor_ID */
	u8 FLASH_cur_bitmode;		/*!< Specifies the current bitmode of SPIC.
								This parameter can be a value of @ref FLASH_Bit_Mode */
	u8 FLASH_baud_rate;			/*!< Specifies the spi_sclk divider value. The frequency of spi_sclk is derived from:
								Frequency of spi_sclk = Frequency of oc_clk / (2 * FLASH_baud_rate) */
	u8 FLASH_baud_boot;			/*!< Specifies the spi_sclk divider value for rom boot. The frequency of spi_sclk is derived from:
								Frequency of spi_sclk = Frequency of oc_clk / (2 * FLASH_baud_rate) */
	u32 FLASH_cur_cmd; 			/*0x03!< Specifies the current read cmd which is used to read data from flash
								in current bitmode. */

	/* status bits define */
	u32 FLASH_QuadEn_bit;		/*!< Specifies the QE bit in status register which is used to enable Quad I/O mode . */
	u32 FLASH_Busy_bit;			/*0x01!< Specifies the WIP(Write in Progress) bit in status register which indicates whether
								the device is busy in program/erase/write status register progress. */
	u32 FLASH_WLE_bit; 			/*0x02!< Specifies the WEL(Write Enable Latch) bit in status register which indicates whether
								the device will accepts program/erase/write status register instructions*/
	u32 FLASH_Status2_exist;		/*!< Specifies whether this flash chip has Status Register2 or not.
								This parameter can be 0/1. 0 means it doesn't have Status Register2, 1 means
								it has Status Register2.*/

	/* calibration data */
	u8 FLASH_rd_sample_phase_cal;	/*!< Specifies the read sample phase obtained from calibration. this is cal sample phase get from high speed cal */
	u8 FLASH_rd_sample_phase;	/*!< Specifies the read sample phase obtained from calibration. this is current sample phase */
	u8 FLASH_rd_dummy_cyle[5];	/*!< Specifies the read dummy cycle of different bitmode according to
								flash datasheet*/

	/* valid R/W command set */
	u32 FLASH_rd_dual_o; 			/*!< Specifies dual data read cmd */
	u32 FLASH_rd_dual_io; 			/*!< Specifies dual data/addr read cmd */
	u32 FLASH_rd_quad_o; 		/*!< Specifies quad data read cmd */
	u32 FLASH_rd_quad_io; 		/*!< Specifies quad data/addr read cmd */
	u32 FLASH_wr_dual_i; 			/*!< Specifies dual data write cmd */
	u32 FLASH_wr_dual_ii;			/*!< Specifies dual data/addr write cmd */
	u32 FLASH_wr_quad_i; 			/*!< Specifies quad data write cmd */
	u32 FLASH_wr_quad_ii;			/*!< Specifies quad data/addr write cmd */
	u32 FALSH_dual_o_valid_cmd;		/*!< Specifies valid cmd of dual o bitmode to program/read flash in auto mode */
	u32 FALSH_dual_io_valid_cmd;	/*!< Specifies valid cmd of dual io bitmode to program/read flash in auto mode */
	u32 FALSH_quad_o_valid_cmd;		/*!< Specifies valid cmd of quad o bitmode to program/read flash in auto mode */
	u32 FALSH_quad_io_valid_cmd;	/*!< Specifies valid cmd of quad io bitmode to program/read flash in auto mode */

	/* other command set */
	u8 FLASH_cmd_wr_en;			/*0x06!< Specifies the Write Enable(WREN) instruction which is for setting WEL bit*/
	u8 FLASH_cmd_rd_id;			/*0x9F!< Specifies the Read ID instruction which is for getting the identity of the flash device.*/
	u8 FLASH_cmd_rd_status;		/*0x0F!< Specifies the Read Status Register instruction which is for getting the status of flash */
	u8 FLASH_cmd_rd_status2;		/*!< Specifies the Read Status Register2 instruction which is for getting the status2 of flash */
	u8 FLASH_cmd_wr_status;		/*0x1F!< Specifies the Write Status Register instruction which is for setting the status register of flash */
	u8 FLASH_cmd_wr_status2;		/*!< Specifies the Write Status Register2 instruction which is for setting the status register2 of flash.
								 In some flash chips, status2 write cmd != status1 write cmd,
								 like: GD25Q32C, GD25Q64C,GD25Q128C etc.*/
	u8 FLASH_cmd_chip_e;			/*!< Specifies the Erase Chip instruction which is for erasing the whole chip*/
	u8 FLASH_cmd_block_e;		/*0xD8!< Specifies the Erase Block instruction which is for erasing 64kB*/
	u8 FLASH_cmd_sector_e;		/*!< Specifies the Erase Sector instruction which is for erasing 4kB*/
	u8 FLASH_cmd_pwdn_release;	/*!< Specifies the Release from Deep Power Down instruction which is for exiting power down mode.*/
	u8 FLASH_cmd_pwdn;			/*!< Specifies the Deep Power Down instruction which is for entering power down mode.*/
	u8 FLASH_dum_byte;

	/* debug log */
	u8 debug;					/*!< Specifies whether or not to print debug log.*/

	/* new calibration */
	u8 phase_shift_idx;			/*!< Specifies the phase shift idx in new calibration.*/

	u8 FLASH_addr_phase_len;	/*NAND_COL_ADDR_2_BYTE!< Specifies the number of bytes in address phase (between command phase and write/read phase).
								This parameter can be 0/1/2/3. 0 means 4-byte address mode in SPI Flash.*/
	u8 FLASH_dum_en;		/*!< Specifies enable to push one dummy byte (DUM_BYTE_VAL) after pushing address to Flash in auto read.
								If RD_DUMMY_LENGTH = 0, it won't push the dummy byte. */
	u8 FLASH_pinmux;			/*!< Specifies which pinmux is used. PINMUX_S0 or PINMUX_S1*/

	u32 FLASH_rd_fast_single;	/*0x0B!< Specifies fast read cmd in auto mode.*/

	u8 FLASH_pagesize_main_bit_exp;		/*(11)!< Specifies main aera of the page size is expressed as a power of 2*/
	u8 FLASH_pagesize_spare_bit_exp;	/*!< Specifies spare aera of the page size is expressed as a power of 2*/

	u8 FLASH_cmd_page_read;		/*0x13!< Specifies array read cmd in nand flash.*/
	u8 FLASH_cmd_page_write;	/*0x10!< Specifies array write cmd in nand flash.*/

	u8 FLASH_EFail_bit; 		/*0x04!< Specifies the E_Fail bit in status register which shows the status of erase failure or
								if host erase any invalid address or protected area (including protected blocks or protected Secure OTP area).*/
	u8 FLASH_PFail_bit;		/*0x08!< Specifies the P_Fail bit in status register which shows the status of program failure or
								if host program any invalid address or protected area (including protected blocks or protected Secure OTP area).*/
	u8 FLASH_ECC_bit;			/*!< Specifies the ECC_Sx bit in status register which shows the ECC status of the current valid Read operation.*/
	u8 FLASH_ECC_EN_bit;		/*0x10!< Specifies the ECC_EN bit in Configuration register which can enable or disable the internal ECC of nand flash.*/
} NandFlash_InitTypeDef;

extern NandFlash_InitTypeDef g_nandflash_init_para;


enum {
	LFS_NAND_ERROR = 0,
	LFS_NAND_WARNING,
	LFS_NAND_INFO,
	LFS_NAND_DEBUG,
	LFS_NAND_NONE,
};

#define LFS_NAND_DBG_ON	0
#if LFS_NAND_DBG_ON
#define LFS_NAND_DBG(level, fmt, arg...)     \
do {\
	if (level <= LFS_NAND_DEBUG) {\
		if (level <= LFS_NAND_ERROR) {\
			printf("\n\r[error] %s():%d, " fmt "\n\r", __func__,__LINE__, ##arg);\
		} \
		else if(level == LFS_NAND_WARNING){\
			printf("[warning] %s(), " fmt "\n", __func__, ##arg);\
		} \
		else if(level == LFS_NAND_INFO){\
			printf("[info] %s():%d, " fmt "\n", __func__,__LINE__, ##arg);\
		} \
		else if(level == LFS_NAND_DEBUG){\
			printf("[debug] %s(), " fmt "\n", __func__, ##arg);\
		} \
	}\
}while(0)
#else
#define LFS_NAND_DBG(level, fmt, arg...)
#endif



void NandFlash_Reset(void);
u8 NandFlash_ReadID(u8 *flash_id);
u8 NandFlash_WaitBusy(u32 WaitType);
void NandFlash_WriteEn(void);
//void NandFlash_RxCmd(u8 cmd, u8 addr_len, u8 *paddr, u32 read_len, u8 *read_data);
//void NandFlash_TxCmd(u8 cmd, u8 AddrLen, u8 *pAddr, u32 DataLen, u8 *pData);
u8 NandFlash_GetStatus(u8 FeatureID);
u8 NandFlash_SetStatus(u8 FeatureID, u8 Status);
void NandFlash_SetStatusBits(u8 FeatureID, u8 SetBits, u8 NewState);
u8 NandFlash_Erase(u32 PageAddr);
u8 NandFlash_Page_Read_ArrayToCache(u32 PageAddr);
void NandFlash_Page_Read_FromCache(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData);
void NandFlash_Page_Write_Data_Xfer(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData);
u8 NandFlash_Page_Write_Program_Execute(u32 PageAddr);
//void NandFlash_SetSpiMode(NandFlash_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode);
void NandFlash_StructInit_DS(NandFlash_InitTypeDef *FLASH_InitStruct);
void NandFlash_StructInit_Micron(NandFlash_InitTypeDef *FLASH_InitStruct);
void NandFlash_StructInit_MXIC(NandFlash_InitTypeDef *FLASH_InitStruct);
void NandFlash_StructInit_GD(NandFlash_InitTypeDef *FLASH_InitStruct);
void NandFlash_StructInit(NandFlash_InitTypeDef *FLASH_InitStruct);
//u8 NandFlash_Init(u8 SpicBitMode);

void nandflash_spi_init(void);


#endif // _LITTLEFS_SPI_NAND_H

