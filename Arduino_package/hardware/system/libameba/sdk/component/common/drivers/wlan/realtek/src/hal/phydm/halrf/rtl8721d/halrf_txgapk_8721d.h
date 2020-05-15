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

#ifndef __HALRF_TXGAPK_8721D_H__
#define __HALRF_TXGAPK_8721D_H__

#if (RTL8721D_SUPPORT == 1)
/*============================================================*/
/*Definition */
/*============================================================*/

void do_txgapk_8721d(void *dm_void,
		     u8 delta_thermal_index,
		     u8 thermal_value,
		     u8 threshold);

void phy_txgap_calibrate_8721d(void *dm_void,
			       boolean clear);

#else /* (RTL8721D_SUPPORT == 0)*/

#define do_txgapk_8721d(_pdm_void, clear)

#endif /* RTL8721D_SUPPORT */

#endif /*#ifndef __HALRF_IQK_8721D_H__*/
