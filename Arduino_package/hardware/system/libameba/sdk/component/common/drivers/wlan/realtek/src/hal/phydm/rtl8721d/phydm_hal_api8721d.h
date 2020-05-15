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
#ifndef __INC_PHYDM_API_H_8721D__
#define __INC_PHYDM_API_H_8721D__

#if (RTL8721D_SUPPORT == 1)

#define PHY_CONFIG_VERSION_8721D "0.0.0" /*2018.05.02*/

#define INVALID_RF_DATA 0xffffffff
#define INVALID_TXAGC_DATA 0xff

#define config_phydm_read_rf_check_8721d(data) (data != INVALID_RF_DATA)
#define config_phydm_read_txagc_check_8721d(data) (data != INVALID_TXAGC_DATA)

void phydm_init_hw_info_by_rfe_type_8721d(struct dm_struct *dm);

u32 config_phydm_read_rf_reg_8721d(struct dm_struct *dm,
				   enum rf_path rf_path,
				   u32 reg_addr,
				   u32 bit_mask);
boolean
config_phydm_write_rf_reg_8721d(struct dm_struct *dm,
				enum rf_path rf_path,
				u32 reg_addr,
				u32 bit_mask,
				u32 data);

boolean
config_phydm_write_txagc_8721d(struct dm_struct *dm,
			       u32 power_index,
			       enum rf_path path,
			       u8 hw_rate);

u8 config_phydm_read_txagc_8721d(struct dm_struct *dm,
				 enum rf_path path,
				 u8 hw_rate);

boolean
config_phydm_switch_band_8721d(struct dm_struct *dm, u8 central_ch);

boolean
config_phydm_switch_channel_8721d(struct dm_struct *dm, u8 central_ch);

boolean
config_phydm_switch_bandwidth_8721d(struct dm_struct *dm,
				    u8 primary_ch_idx,
				    enum channel_width bandwidth);

boolean
config_phydm_switch_channel_bw_8721d(struct dm_struct *dm,
				     u8 central_ch,
				     u8 primary_ch_idx,
				     enum channel_width bandwidth);

boolean
config_phydm_trx_mode_8721d(struct dm_struct *dm,
			    enum bb_path tx_path,
			    enum bb_path rx_path,
			    boolean is_tx2_path);

boolean
config_phydm_parameter_8721d_init(struct dm_struct *dm,
				  enum odm_parameter_init type);

#endif /* RTL8721D_SUPPORT == 1 */
#endif /*  __INC_PHYDM_API_H_8721D__ */
