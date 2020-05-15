/******************************************************************************
 *
 * Copyright(c) 2016 - 2017 Realtek Corporation.
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

extern const struct btc_chip_para btc_chip_para_8721d;

u32 rtw_btc_read_indirect_reg_8721d(struct btc_coexist *btc, u16 reg_addr);

void rtw_btc_write_indirect_reg_8721d(struct btc_coexist *btc, u16 reg_addr, u32 bit_mask,u32 reg_value);


