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
#ifndef	__ROM_RTL8721D_PHYDM_H__
#define __ROM_RTL8721D_PHYDM_H__

typedef struct _ROM_INFO{
	u1Byte	EEPROMVersion;
	u1Byte	CrystalCap;
	u8Byte	DebugComponents;
	u4Byte	DebugLevel;
}ROM_INFO, *PROM_INFO;

extern ROM_INFO					ROMInfo;

s32
ROM_odm_SignalScaleMapping_8721D(
	u8 SupportInterface,
	s32 CurrSig 
);

#endif
