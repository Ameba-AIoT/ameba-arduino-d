/******************************************************************************
 *
 * Copyright(c) 2016 - 2017 Realtek Corporation. All rights reserved.
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

#ifndef __HALMAC__HW_CFG_H__
#define __HALMAC__HW_CFG_H__

#define HALMAC_8723A_SUPPORT	0
#define HALMAC_8188E_SUPPORT	0
#define HALMAC_8821A_SUPPORT	0
#define HALMAC_8723B_SUPPORT	0
#define HALMAC_8812A_SUPPORT	0
#define HALMAC_8192E_SUPPORT	0
#define HALMAC_8881A_SUPPORT	0
#define HALMAC_8821B_SUPPORT	0
#define HALMAC_8814A_SUPPORT	0
#define HALMAC_8881A_SUPPORT	0
#define HALMAC_8703B_SUPPORT	0
#define HALMAC_8723D_SUPPORT	0
#define HALMAC_8188F_SUPPORT	0
#define HALMAC_8821BMP_SUPPORT	0
#define HALMAC_8814AMP_SUPPORT	0
#define HALMAC_8195A_SUPPORT	0
#define HALMAC_8821B_SUPPORT	0
#define HALMAC_8196F_SUPPORT	0
#define HALMAC_8197F_SUPPORT	0
#define HALMAC_8198F_SUPPORT	0
#define HALMAC_8822C_SUPPORT	0

/* Halmac support IC version */
#define HALMAC_8814B_SUPPORT	0
#define HALMAC_8822B_SUPPORT	0
#if defined(CONFIG_HARDWARE_8821C)
	#define HALMAC_8821C_SUPPORT	1
#else
	#define HALMAC_8821C_SUPPORT	0
#endif
#if defined(CONFIG_PLATFORM_8195BHP)
	#define HALMAC_8195B_SUPPORT	1
#else
	#define HALMAC_8195B_SUPPORT	0
#endif

#endif


