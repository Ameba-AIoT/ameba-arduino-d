/******************************************************************************
 *
 * Copyright(c) 2016 - 2018 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 ******************************************************************************/

#ifndef _HALMAC_8195B_CFG_H_
#define _HALMAC_8195B_CFG_H_

#include "../../halmac_hw_cfg.h"
#include "../halmac_88xx_cfg.h"

#if HALMAC_8195B_SUPPORT

#define TX_FIFO_SIZE_8195B       32768 /* 32k */
#define RX_FIFO_SIZE_8195B       16384 /* 16k */
#define TRX_SHARE_SIZE_8195B	16384 /* 16k */

#define RX_DESC_DUMMY_SIZE_8195B      72 /*8*9 Bytes*/
//#define RX_FIFO_EXPANDING_MODE_PKT_SIZE_MAX_8195B    80 /* 8 Byte alignment*/

/* should be 8 Byte alignment*/
//#if (HALMAC_RX_FIFO_EXPANDING_MODE_PKT_SIZE <= \
//	RX_FIFO_EXPANDING_MODE_PKT_SIZE_MAX_8195B)
//#define RX_FIFO_EXPANDING_UNIT_8195B	(RX_DESC_SIZE_88XX + \
//	RX_DESC_DUMMY_SIZE_8195B + HALMAC_RX_FIFO_EXPANDING_MODE_PKT_SIZE)
//#else
//#define RX_FIFO_EXPANDING_UNIT_8195B	(RX_DESC_SIZE_88XX + \
//	RX_DESC_DUMMY_SIZE_8195B + RX_FIFO_EXPANDING_MODE_PKT_SIZE_MAX_8195B)
//#endif

#define TX_FIFO_SIZE_LA_8195B	(TX_FIFO_SIZE_8195B >> 1)
//#define TX_FIFO_SIZE_RX_EXPAND_1BLK_8195B	\
//		(TX_FIFO_SIZE_8195B - TRX_SHARE_SIZE_8195B)
//#define RX_FIFO_SIZE_RX_EXPAND_1BLK_8195B	\
//		((((RX_FIFO_EXPANDING_UNIT_8195B << 8) - 1) >> 10) << 10)

#define EFUSE_SIZE_8195B		0 /*cannot resd/write efuse*/
#define EEPROM_SIZE_8195B		0
#define BT_EFUSE_SIZE_8195B		0

#define SEC_CAM_NUM_8195B		32

#define	OQT_ENTRY_AC_8195B		32
#define OQT_ENTRY_NOAC_8195B		32
#define MACID_MAX_8195B		32

#define WLAN_FW_IRAM_MAX_SIZE_8195B	65536
#define WLAN_FW_DRAM_MAX_SIZE_8195B	49152
#define WLAN_FW_ERAM_MAX_SIZE_8195B	49152
#define WLAN_FW_MAX_SIZE_8195B		(WLAN_FW_IRAM_MAX_SIZE_8195B + \
	WLAN_FW_DRAM_MAX_SIZE_8195B + WLAN_FW_ERAM_MAX_SIZE_8195B)

#endif /* HALMAC_8195B_SUPPORT */

#endif
