/********************************************************************************
 * Copyright (c) 2014, Realtek Semiconductor Corp.
 * All rights reserved.
 *
 * This module is a confidential and proprietary property of RealTek and
 * possession or use of this module requires written permission of RealTek.
 *******************************************************************************
 */
#ifndef _RTL8721D_FLASHLOADER_H
#define _RTL8721D_FLASHLOADER_H

#define IS_FLASH_ADDR(addr)			((addr >= SPI_FLASH_BASE) && (addr <= 0x0FFFFFFF))

#undef IMAGE_WRITE_PARSER
#ifdef IMAGE_WRITE_PARSER
#define FLASH_ERASE_AND_WRITE		0
#define FLASH_WRITE_ONLY			BIT_SOC_XMODEM_FLASH_EMPTY
#define FLASH_ERASE_WRITE_PAGE		BIT_SOC_XMODEM_PAGE_PG
#define FLASH_WRITE_PAGE_ONLY		(BIT_SOC_XMODEM_FLASH_EMPTY | BIT_SOC_XMODEM_PAGE_PG)
#endif

#define XDEBUG_GPIO					_PA_12
#define WAIT_FRAME_TIME			(830 * 1000) 			/* 1 sec, wait frame timeout */
#define WAIT_CHAR_TIME				(415 * 1000)			/* 0.5 sec, wait char timeout */
#define WAIT_TIME_SERVER			(1660 * 1000)		/* 2 sec, server wait timeout */
#define WAIT_HANDSHAKE_TIME		(830 * 1000)		/* 1 sec, handshake timeout */

#define FRAME_BUF_ADDR				(char*)(MSP_RAM_LP - 16 * 1024)
#define HANDSHAKE_BAUD				115200

u32 Get_FlashWriteMethod(void);
u32 Get_LogUartPinmux(void);
void Reboot_Uart_Download(void);

#endif

