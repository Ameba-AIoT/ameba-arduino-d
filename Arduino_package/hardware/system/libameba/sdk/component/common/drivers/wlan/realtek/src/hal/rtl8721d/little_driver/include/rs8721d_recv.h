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

#ifndef __RTL_RTL8721D_XMIT_H__
#define __RTL_RTL8721D_XMIT_H__

/* Constant Value */
extern const u8 SNAP_ETH_TYPE_IPX[2] ;

extern const u8 SNAP_ETH_TYPE_APPLETALK_AARP[2] ;
extern const u8 SNAP_ETH_TYPE_APPLETALK_DDP[2] ;
extern const u8 SNAP_ETH_TYPE_TDLS[2] ;
extern const u8 SNAP_HDR_APPLETALK_DDP[3] ; // Datagram Delivery Protocol

extern const u8 oui_8021h[] ;
extern const u8 oui_rfc1042[];

extern const u8 rtw_rfc1042_header[] ;
/* Bridge-Tunnel header (for EtherTypes ETH_P_AARP and ETH_P_IPX) */
extern const u8 rtw_bridge_tunnel_header[] ;

#define rs8721d_query_rx_desc_status rtl8721d_query_rx_desc_status

#endif


