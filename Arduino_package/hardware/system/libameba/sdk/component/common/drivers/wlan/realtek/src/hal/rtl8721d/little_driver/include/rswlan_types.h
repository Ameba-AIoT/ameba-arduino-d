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

#ifndef __RTL_AXI_H__
#define __RTL_AXI_H__
#include <drv_types.h>

/*
1: MSDU packet queue,
2: Rx Command Queue
*/
#define RTL_AXI_RX_MPDU_QUEUE			0
#define RTL_PCI_RX_CMD_QUEUE			1
#define RTL_PCI_MAX_RX_QUEUE			2

#define RTL_PCI_MAX_RX_COUNT			64
#define RTL_PCI_MAX_TX_QUEUE_COUNT		9

#define RT_TXDESC_NUM				16
#define RT_TXDESC_NUM_BE_QUEUE			16



#define LX_MAX_RX_QUEUE		1// MSDU packet queue, Rx Command Queue
#define LX_MAX_RX_COUNT		4//RX_Q_DESC_NUM// 128

#define TX_QUEUE_LENGTH		8
#define TX_PENDING_QUEUE_LENGTH TX_QUEUE_LENGTH


struct rs_rbuf_stat {
	unsigned int rxdw0;
	unsigned int rxdw1;
};

struct rs_rx_ring {

	struct rs_rbuf_stat 	*desc;
	dma_addr_t		dma;
	unsigned int		idx;
	struct sk_buff	*rx_buf[LX_MAX_RX_COUNT];
};


/*
accesser of recv_priv: rtw_recv_entry(dispatch / passive level); recv_thread(passive) ; returnpkt(dispatch)
; halt(passive) ;

using enter_critical section to protect
*/
struct rs_recv_priv
{
	_lock	lock;
	// Rx
	struct rs_rx_ring	rx_ring[LX_MAX_RX_QUEUE];
	int 	rxringcount;
	u16	rxbuffersize;

	u32 rx_overflow;
};


//only use VO Queue for simpel tx
#define AXI_MAX_TX_QUEUE_COUNT	1

struct rs_tx_ring {

	struct tx_buf_desc	*desc;

	dma_addr_t		dma;
	unsigned int		idx;
	unsigned int		entries;
	_queue			queue;
	u32				qlen;
};


struct	rs_xmit_priv	{

	_lock	lock;

	xQueueHandle tx_queue;
	xQueueHandle tx_pending_queue;

	struct rs_tx_ring	tx_ring[AXI_MAX_TX_QUEUE_COUNT];
	int	txringcount[AXI_MAX_TX_QUEUE_COUNT];
};


struct _RS_ADAPTER {

	s8	bDriverStopped;
	s8	bSurpriseRemoved;
	s8	irq_enabled;
	_mutex *hw_init_mutex;
	_mutex h2c_fwcmd_mutex;
	_lock	irq_th_lock;

	struct rs_recv_priv recvpriv;
	struct rs_xmit_priv xmitpriv;

	struct task_struct	xmittasklet_thread;
	struct task_struct	recvtasklet_thread;
	struct task_struct	isrThread;
	_sema *pExportWlanIrqSemaphore;

//hal data
	u32	IntArray[3];
	u32	IntrMask[3];

	u16	RxExpectTag;
	u8 LastHMEBoxNum;
	u8 mac_addr[6];
	u8 bssid[6];

//secpriv
	u32 dot118021XGrpPrivacy;
	u32 dot11PrivacyAlgrthm;
	union pn48		dot11txpn;

//stainfo
	u16 	tid_rxseq[16];

// pwrctrl

	u8 wowlan_wake_reason;

};

typedef struct _RS_ADAPTER rs_adapter;

#endif


