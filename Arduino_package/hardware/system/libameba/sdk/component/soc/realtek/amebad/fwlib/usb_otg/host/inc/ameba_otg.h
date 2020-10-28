/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _AMEBA_OTG_H_
#define _AMEBA_OTG_H_

#define DWC_EN_ISOC 1
#if defined(DWC_EN_ISOC)
#define USB_REQ_ISO_ASAP 1
#endif

#define HAL_OTG_READ32(addr)            HAL_READ32(USB_OTG_REG_BASE, (u32)addr)
#define HAL_OTG_WRITE32(addr, value)    HAL_WRITE32(USB_OTG_REG_BASE, (u32)addr, value)

#define HAL_OTG_MODIFY32(addr, clrmsk, setmsk)     HAL_WRITE32(USB_OTG_REG_BASE,(u32)addr,\
                                                   ((HAL_READ32(USB_OTG_REG_BASE, (u32)addr) & (~clrmsk)) | setmsk))

#define DWC_READ_REG32(_reg_)   HAL_OTG_READ32((u32)_reg_)
#define DWC_WRITE_REG32(_reg_, _val_)   HAL_OTG_WRITE32((u32)_reg_,_val_)
#define DWC_MODIFY_REG32(_reg_,_cmsk_,_smsk_)   HAL_OTG_MODIFY32((u32)_reg_,_cmsk_,_smsk_)

// USB OTG addon register
#define REG_OTG_PWCSEQ_IP_OFF		0x30004     //This is in OTG IP
#define REG_OTG_PS_INTR_STS			0x30008     //This is in OTG IP
#define REG_OTG_PS_INTR_MSK			0x3000C     //This is in OTG IP
#define REG_OTG_VND_STS_OUT			0x3001C     //This is in OTG IP

/* #define REG_OTG_PWCSEQ_IP_OFF               0x30004 */
#define BIT_UPLL_CKRDY				BIT(5)	/* R/W	0		1: USB PHY clock ready */
#define BIT_USBOTG_EN				BIT(8)	/* R/W	0		1: Enable USB OTG */
#define BIT_USBPHY_EN				BIT(9)	/* R/W	0		1: Enable USB PHY */

//define phy registers
#define REG_PHY_RX_BOOST			0xE0	//phy page0 reg
#define REG_PHY_SH_LEVEL			0xE2	//phy page0 reg
#define REG_PHY_TX_SLEW				0xE5	//phy page0 reg
#define REG_PHY_SEN_HST				0xE6	//phy page0 reg
#define REG_PHY_NSQ_CTRL			0xF7	//phy reg

#define BIT_RX_BOOST				0x30	//D2S boost   E0[5:4]
#define BIT_SWG_LEVEL				0x0F	//HSTX output swing level   E2[3:0]
#define BIT_SRC_SHIFT				2		//Slew rate control shift
#define BIT_SRC						0x1C	//Slew rate control of HS Transmitter   E5[4:2]
#define BIT_SEN_HST_SHIFT			4		//Sensitivity reference voltage for host shift
#define BIT_SEN_HST					0xF0	//Sensitivity reference voltage for host   E6[7:4]
#define BIT_NSQ_HST_LEVEL_SHIFT		4		//HOST disconnect detect period shift
#define BIT_NSQ_HST_LEVEL			0x30	//HOST disconnect detect period    F7[5:4]
#define PHY_HIGH_ADDR(x)			(((x) & 0xF0) >> 4)
#define PHY_LOW_ADDR(x)				((x) & 0x0F)
#define PHY_DATA_MASK				0xFF
#endif
