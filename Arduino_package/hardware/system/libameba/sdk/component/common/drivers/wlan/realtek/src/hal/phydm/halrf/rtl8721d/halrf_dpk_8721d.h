/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2018  Realtek Corporation.
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
 *****************************************************************************/

#ifndef __HALRF_DPK_8721D_H__
#define __HALRF_DPK_8721D_H__

/*--------------------------Define Parameters-------------------------------*/
#define DPK_RF_PATH_NUM_8721D 1
#define DPK_MAC_REG_NUM_8721D 0
#define DPK_BB_REG_NUM_8721D 15
#define DPK_RF_REG_NUM_8721D 11
#define DPK_GAINLOSS_DBG_8721D 1
#define DPK_PULSE_DBG_8721D 1
#define DPK_PAS_DBG_8721D 0
#define DPK_SRAM_IQ_DBG_8721D 0
#define DPK_SRAM_read_DBG_8721D 0
#define DPK_SRAM_write_DBG_8721D 0
#define DPK_DO_PATH_A 1
#define RF_T_METER_8721D RF_0x42
#define DPK_THRESHOLD_8721D 6

#define DPK_GROUP_2G_8721D 3
#define DPK_GROUP_5G_8721D 6
#define DPK_GROUP_NUM_8721D 9	
#define G_2G_N 3
#define G_5G_N 6

#define TX_FULL_RAMP 32
#define GAINLOSS_RAMP 10
#define TX_PULSE_RAMP 16
#define TX_INVERSE 1

void phy_dpk_track_8721d(void *dm_void);

void phy_dpk_init_8721d(void *dm_void);

u8 phy_dpk_channel_transfer_8721d(void *dm_void);

u8 phy_lut_sram_read_8721d(void *dm_void,	u8 k);

void phy_lut_sram_write_8721d(void *dm_void);

void phy_dpk_enable_disable_8721d(void *dm_void);

void dpk_sram_read_8721d(void *dm_void);

void dpk_reload_8721d(void *dm_void);

void dpk_on_off_8721d(void * dm_void, boolean turn_on);

void do_dpk_8721d(void *dm_void);

void dpk_set_dpkbychannel_8721d(void *dm_void, boolean dpk_by_ch);

boolean dpk_get_dpkbychannel_8721d(void *dm_void);

void dpk_set_is_dpk_enable_8721d(void *dm_void,
				 boolean is_dpk_enable);

boolean dpk_get_is_dpk_enable_8721d(void *dm_void);
#endif
