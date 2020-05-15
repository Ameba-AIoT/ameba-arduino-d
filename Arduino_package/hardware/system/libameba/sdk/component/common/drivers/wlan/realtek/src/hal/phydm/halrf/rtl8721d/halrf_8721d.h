/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
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

#ifndef __HALRF_8721D_H__
#define __HALRF_8721D_H__

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN))
#if RT_PLATFORM == PLATFORM_MACOSX
#include "halphyrf_win.h"
#else
#include "../halrf/halphyrf_win.h"
#endif
#elif (DM_ODM_SUPPORT_TYPE & (ODM_CE))
#include "../halphyrf_ce.h"
#elif (DM_ODM_SUPPORT_TYPE & (ODM_AP))
#include "../halphyrf_ap.h"
#endif

/*--------------------------Define Parameters-------------------------------*/

#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
#define IQK_DELAY_TIME_8721D 15 /* ms */
#else
#define IQK_DELAY_TIME_8721D 10
#endif

#define index_mapping_NUM_8721D 15

#define AVG_THERMAL_NUM_8721D 4
#define RF_T_METER_8721D 0x42

#define IQK_BB_REG_NUM_8721D 9
#define IQK_RF_REG_NUM_8721D 6

void
configure_txpower_track_8721d(struct txpwrtrack_cfg *p_config);

void
odm_tx_pwr_track_set_pwr_8721d(void *dm_void,
			       enum pwrtrack_method method,
			       u8 rf_path,
			       u8 channel_mapped_index);

void
get_delta_swing_table_8721d(void *dm_void,
			    u8 **temperature_up_ofdm_a,
			    u8 **temperature_down_ofdm_a,
			    u8 **temperature_up_ofdm_b,
			    u8 **temperature_down_ofdm_b,
			    u8 **temperature_up_cck_a,
			    u8 **temperature_down_cck_a,
			    u8 **temperature_up_cck_b,
			    u8 **temperature_down_cck_b);

void
get_delta_swing_xtal_table_8721d(void *dm_void,
				 s8 **temperature_up_xtal,
				 s8 **temperature_down_xtal);

void
odm_txxtaltrack_set_xtal_8192f(void *dm_void);

#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
void
do_iqk_8721d(void *dm_void,
	     u8 delta_thermal_index,
	     u8 thermal_value,
	     u8 threshold);
#else
void
do_iqk_8721d(void *dm_void,
	     u8 delta_thermal_index,
	     u8 thermal_value,
	     u8 threshold);
#endif

void
odm_tx_pwr_track_set_pwr_8721d(void *dm_void,
			       enum pwrtrack_method method,
			       u8 rf_path,
			       u8 channel_mapped_index);

void
odm_txxtaltrack_set_xtal_8721d(void *dm_void);

/* 1 7.IQK */
void
phy_iq_calibrate_8721d(void *dm_void, boolean is_recovery);

/* LC calibrate*/
void
phy_lc_calibrate_8721d(void *dm_void);

void
phy_set_rf_path_switch_8721d(struct dm_struct *dm, boolean is_main);

void
phy_poll_lck_8721d(struct dm_struct *dm);

void
phy_x2k_8721d(struct dm_struct *dm);

void
phy_x2_check_8721d(struct dm_struct *dm);

void
phy_syn_calibrate_part1_8721d(struct dm_struct *dm);

void
phy_syn_calibrate_part2_8721d(struct dm_struct *dm);

void
_phy_save_adda_registers_8721d(struct dm_struct *dm,
			       u32 *adda_reg,
			       u32 *adda_backup,
			       u32 register_num);

void
_phy_path_adda_on_8721d(struct dm_struct *dm,
			u32 *adda_reg,
			boolean is_path_a_on,
			u8 CH_band);

void
_phy_mac_setting_calibration_8721d(struct dm_struct *dm,
				   u32 *mac_reg,
				   u32 *mac_backup);

void halrf_rf_lna_setting_8721d(
   struct dm_struct *dm,
   enum halrf_lna_set type);

#endif /*__HALRF_8721D_H__*/
