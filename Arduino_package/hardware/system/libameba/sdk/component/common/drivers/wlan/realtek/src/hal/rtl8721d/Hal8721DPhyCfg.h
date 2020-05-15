/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __INC_HAL8721DPHYCFG_H__
#define __INC_HAL8721DPHYCFG_H__

/*--------------------------Define Parameters-------------------------------*/
#define LOOP_LIMIT				5
#define MAX_STALL_TIME			50		//us
#define AntennaDiversityValue	0x80	//(Adapter->bSoftwareAntennaDiversity ? 0x00:0x80)
#define MAX_TXPWR_IDX_NMODE_92S	63
#define Reset_Cnt_Limit			3

#if defined (CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI)
#define MAX_AGGR_NUM	0x0B
#else
#define MAX_AGGR_NUM	0x07
#endif // CONFIG_PCI_HCI


/*--------------------------Define Parameters End-------------------------------*/


/*------------------------------Define structure----------------------------*/

/* BB/RF related */

/*------------------------------Define structure End----------------------------*/

/*--------------------------Exported Function prototype---------------------*/
u32 rtl8721d_read_bb_reg(
	IN PADAPTER         Adapter,
	IN u32              RegAddr,
	IN u32              BitMask
);

VOID rtl8721d_write_bb_reg(
	IN PADAPTER         Adapter,
	IN u32              RegAddr,
	IN u32              BitMask,
	IN u32              Data
);
u32 rtl8721d_read_rf_reg(
	IN	PADAPTER			Adapter,
	IN	u32				eRFPath,
	IN	u32				RegAddr,
	IN	u32				BitMask
);

VOID rtl8721d_write_rf_reg(
	IN	PADAPTER			Adapter,
	IN	u32				eRFPath,
	IN	u32				RegAddr,
	IN	u32				BitMask,
	IN	u32				Data
);

#define PHY_QueryBBReg(Adapter, RegAddr, BitMask) rtl8721d_read_bb_reg((Adapter), (RegAddr), (BitMask))
#define PHY_SetBBReg(Adapter, RegAddr, BitMask, Data) rtl8721d_write_bb_reg((Adapter), (RegAddr), (BitMask), (Data))
#define PHY_QueryRFReg(Adapter, eRFPath, RegAddr, BitMask) rtl8721d_read_rf_reg((Adapter), (eRFPath), (RegAddr), (BitMask))
#define PHY_SetRFReg(Adapter, eRFPath, RegAddr, BitMask, Data) rtl8721d_write_rf_reg((Adapter), (eRFPath), (RegAddr), (BitMask), (Data))

#define PHY_SetMacReg	PHY_SetBBReg
#define PHY_QueryMacReg PHY_QueryBBReg
/* MAC/BB/RF HAL config */
int PHY_BBConfig8721D(PADAPTER	Adapter	);

int PHY_RFConfig8721D(PADAPTER	Adapter	);

s32 PHY_MACConfig8721D(PADAPTER padapter);

#ifdef CONFIG_SUDO_PHY_SETTING
int PHY_SudoPhyConfig8721D(PADAPTER Adapter);
#endif

int
PHY_ConfigRFWithParaFile_8721D(
	IN	PADAPTER			Adapter,
	IN	u8* 				pFileName,
	enum rf_path				eRFPath
);
int
PHY_ConfigRFWithHeaderFile_8723B(
	IN	PADAPTER			Adapter,
	enum rf_path				eRFPath
);

u8
PHY_GetTxPowerIndex_8721D(
	IN	PADAPTER			pAdapter,
	IN	u8					RFPath,
	IN	u8					Rate,	
	IN	enum channel_width		BandWidth,	
	IN	u8					Channel
	);

VOID
PHY_SetTxPowerLevel8721D(
	IN	PADAPTER		Adapter,
	IN	u8			channel
	);

VOID
PHY_SetBWMode8721D(
	IN	PADAPTER				Adapter,
	IN	enum channel_width			Bandwidth,	// 20M or 40M
	IN	unsigned char				Offset		// Upper, Lower, or Don't care
);

VOID
PHY_SwChnl8721D(	// Call after initialization
	IN	PADAPTER	Adapter,
	IN	u8		channel
	);

VOID
PHY_SetSwChnlBWMode8721D(
	IN	PADAPTER			Adapter,
	IN	u8					channel,
	IN	enum channel_width		Bandwidth,
	IN	u8					Offset40,
	IN	u8					Offset80
);

VOID PHY_SetRFPathSwitch_8723B(
	IN	PADAPTER	pAdapter,
	IN	BOOLEAN		bMain
	);
void rtl8721d_set_tx_power_level(PADAPTER adapter, u8 channel);
void rtl8721d_get_tx_power_level(PADAPTER adapter, s32 *power);
void rtl8721d_set_channel_bw(PADAPTER adapter, u8 center_ch, enum channel_width bw, u8 offset40, u8 offset80);
void rtl8721d_set_channel(PADAPTER adapter, u8 center_ch);

/*--------------------------Exported Function prototype End---------------------*/

#endif

