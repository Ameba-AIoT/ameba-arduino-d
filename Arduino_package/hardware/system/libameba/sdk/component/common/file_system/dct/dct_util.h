/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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

#ifndef __RTK_DCT_UTIL_H__
#define __RTK_DCT_UTIL_H__

#include <osdep_service.h>

int32_t	dct_flash_init(void);
int32_t	dct_flash_read(uint32_t address, uint8_t *buffer, uint32_t length);
int32_t	dct_flash_write(uint32_t address, uint8_t *buffer, uint32_t length);
int32_t	dct_flash_erase(uint32_t address, uint32_t length);
void	dct_mutex_init(_mutex *mutex);
void	dct_mutex_free(_mutex *mutex);
int32_t dct_mutex_lock(_mutex *mutex, uint32_t timeout_ms);
void	dct_mutex_unlock(_mutex *mutex);


#endif // __RTK_DCT_UTIL_H__
