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

#ifndef _HALMAC_INIT_8195B_H_
#define _HALMAC_INIT_8195B_H_

#include "../../halmac_api.h"

#if HALMAC_8195B_SUPPORT

enum halmac_ret_status
mount_api_8195b(struct halmac_adapter *adapter);

enum halmac_ret_status
init_trx_cfg_8195b(struct halmac_adapter *adapter, enum halmac_trx_mode mode);

enum halmac_ret_status
init_protocol_cfg_8195b(struct halmac_adapter *adapter);

enum halmac_ret_status
init_h2c_8195b(struct halmac_adapter *adapter);

#endif /* HALMAC_8195B_SUPPORT */

#endif /* _HALMAC_INIT_8195B_H_ */
