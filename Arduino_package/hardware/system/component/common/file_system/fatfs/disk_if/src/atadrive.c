/*
 *  Routines to associate CF card driver with FatFs
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "integer.h"
#include "atadrive.h"

DSTATUS ATA_disk_status(void){
	return 0;
}

DSTATUS ATA_disk_initialize(void){
	// TODO: 
	return 0;
}

/* Read sector(s) --------------------------------------------*/
DRESULT ATA_disk_read(BYTE *buff, DWORD sector, UINT count){
	return RES_OK;
}

/* Write sector(s) --------------------------------------------*/
#if _USE_WRITE == 1
DRESULT ATA_disk_write(const BYTE *buff, DWORD sector, UINT count){
	return RES_OK;
}
#endif

/* Write sector(s) --------------------------------------------*/
#if _USE_IOCTL == 1
DRESULT ATA_disk_ioctl (BYTE cmd, void* buff){
	DRESULT res = RES_ERROR;
	DWORD last_blk_addr, block_size;
	switch(cmd){
		/* Generic command (used by FatFs) */

		case CTRL_SYNC:		/* Flush disk cache (for write functions) */
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:	/* Get media size (for only f_mkfs()) */
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE:	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:	/* Get erase block size (for only f_mkfs()) */
			res = RES_OK;
			break;
		case CTRL_ERASE_SECTOR:/* Force erased a block of sectors (for only _USE_ERASE) */
			res = RES_OK;
			break;

			/* ATA/CF specific ioctl command */
		case ATA_GET_REV:	/* Get F/W revision */
			res = RES_OK;
			break;
		case ATA_GET_MODEL:	/* Get model name */
			res = RES_OK;
			break;
		case ATA_GET_SN:	/* Get serial number */
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif

ll_diskio_drv ATA_disk_Driver ={
	.disk_initialize = ATA_disk_initialize,
	.disk_status = ATA_disk_status,
	.disk_read = ATA_disk_read,
#if _USE_WRITE == 1
	.disk_write = ATA_disk_write,
#endif
#if _USE_IOCTL == 1
	.disk_ioctl = ATA_disk_ioctl,
#endif
};

