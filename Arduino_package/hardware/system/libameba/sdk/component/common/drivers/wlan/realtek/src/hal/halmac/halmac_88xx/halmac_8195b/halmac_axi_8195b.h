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

#ifndef _HALMAC_AXI_8195B_H_
#define _HALMAC_AXI_8195B_H_

#include "../../halmac_api.h"

#if HALMAC_8195B_SUPPORT

enum halmac_ret_status
mac_pwr_switch_axi_8195b(struct halmac_adapter *adapter,
			  enum halmac_mac_power pwr);

#endif /* HALMAC_8195B_SUPPORT */

#endif/* _HALMAC_AXI_8195B_H_ */

