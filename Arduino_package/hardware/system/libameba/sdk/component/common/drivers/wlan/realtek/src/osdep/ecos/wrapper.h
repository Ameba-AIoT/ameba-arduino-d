 /******************************************************************************
  *
  * Name: wrapper.h - Wrapper header file
  *       $Revision: 1.1.1.1 $
  *
  *****************************************************************************/

#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include <pkgconf/system.h>
#ifdef CYGPKG_IO_ETH_DRIVERS
#include <pkgconf/io_eth_drivers.h>
#endif

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>

#include "sys_support.h"
#include "skbuff.h"
#include "timer.h"
#include "wireless.h"

#define DBG_ERR(fmt, args...)	diag_printf("[%s] " fmt, __FUNCTION__, ## args)
#ifdef CONFIG_WLAN_INTF_DBG
#define DBG_TRACE(fmt, args...)	diag_printf("[%s] " fmt, __FUNCTION__, ## args)
#define DBG_INFO(fmt, args...)	diag_printf("[%s] " fmt, __FUNCTION__, ## args)
#else
#define DBG_TRACE(fmt, args...)
#define DBG_INFO(fmt, args...)
#endif
#define DBG_ASSERT		CYG_ASSERT

/*
 * Device driver private data
 */
typedef struct {
	/* Device number. Used for actually finding the device */
	cyg_uint32 device_num;

	/* Binding net device */
	void *dev;
	struct eth_drv_sc *sc;
	
	/* Our current MAC address */
	unsigned char mac[6];

	/* pending Rx packet */
	void *skb;

	/* Interrupt handling stuff */
	cyg_vector_t  vector;
	cyg_handle_t  interrupt_handle;
	cyg_interrupt interrupt;

	/* device ISR priority */
	cyg_priority_t isr_priority;

#ifdef ISR_DIRECT	
	struct sk_buff_head rx_queue;	/* queue for indication packet */		
#endif
#ifdef TX_PKT_FREE_QUEUE
	struct sk_buff_head tx_queue;	/* queue for free tx buffer */
#endif
} Rltk_wlan_t;

#define netdev_priv(dev)	dev->priv

extern void wrapper_init(void);
extern void wrapper_binding(Rltk_wlan_t *info, void *dev);
extern void wrapper_start(Rltk_wlan_t *info);
extern int wrapper_can_send(Rltk_wlan_t *info);
extern void wrapper_tx(Rltk_wlan_t *info, struct eth_drv_sg *sg_list, int sg_len, \
			int total_len, unsigned long key);
extern void wrapper_up(struct sk_buff *skb);
extern void wrapper_deliver(Rltk_wlan_t *info, struct eth_drv_sg *sg_list, int sg_len);

#endif /* __WRAPPER_H__*/

