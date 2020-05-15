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

#ifndef __RSWLAN_HAL_H__
#define __RSWLAN_HAL_H__

#include<rswlan_types.h>
#include<rtl8721d_hal.h>

#if (RTL8721D_SUPPORT == 1)

void rs8721d_init_AXIDma(rs_adapter * padapter);

u32 rs8721d_wlan_suspend(u32 expected_idle_time, void *param);
u32 rs8721d_wlan_late_resume(u32 expected_idle_time, void *param);


u32 rs8721d_init_desc_ring(rs_adapter * padapter);
void rs8721d_disable_interrupt(rs_adapter *padapter);
s32	rs8721d_interrupt_handler(rs_adapter *padapter);
u32 rs8721d_hal_init(rs_adapter * padapter);
u32 rs8721d_init_power(rs_adapter * padapter);
u32 rs8721d_wlan_resume(u32 expected_idle_time, void *param);
void rs8721d_enable_interrupt(rs_adapter *padapter);
void rs8721d_disable_int_butcpwm2(rs_adapter *padapter);
void rs8721d_recv_tasklet(void *priv);
void rs8721d_disable_DMA(rs_adapter *padapter);
void rs8721d_enable_DMA(rs_adapter *padapter);
BOOLEAN rs8721d_interrupt_recognized(rs_adapter *padapter);

extern thread_return rswlan_interrupt_thread(thread_context context);

extern thread_return rswlan_recv_tasklet(thread_context context);

extern thread_return rswlan_xmit_tasklet(thread_context context);

#endif

#endif

