/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _FLOADER_FW_UPDATE_H_
#define _FLOADER_FW_UPDATE_H_

#include "ameba_soc.h"

extern u32 sector_addr;
_LONG_CALL_ VOID UARTIMG_Write(char *ptr,  unsigned int wr_offset, unsigned int frame_size);
_LONG_CALL_ VOID UARTIMG_WriteParser(char *ptr,  unsigned int wr_offset, unsigned int frame_size);
#endif  // end of "#define _FLOADER_FW_UPDATE_H_"

