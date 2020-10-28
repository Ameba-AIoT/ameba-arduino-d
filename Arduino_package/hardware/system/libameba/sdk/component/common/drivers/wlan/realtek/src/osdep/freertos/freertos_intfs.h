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

#ifndef __FREERTOS_INTFS_H_
#define __FREERTOS_INTFS_H_

typedef struct _driver_priv {
	int drv_registered;

	_mutex hw_init_mutex;
	_mutex h2c_fwcmd_mutex;
#if defined(CONFIG_CONCURRENT_MODE)
	//global variable
	_mutex setch_mutex;
	_mutex setbw_mutex;
#endif
} drv_priv, *pdrv_priv;


struct net_device *rtw_init_netdev(_adapter *padapter);
void rtw_os_indicate_disconnect( _adapter *adapter );

extern int rtw_set_wpa_ie(_adapter *padapter, char *pie, unsigned short ielen);
extern void rtw_os_indicate_connect(_adapter *adapter);
extern void indicate_wx_custom_event(_adapter *padapter, char *msg);
extern int rtw_init_netdev_name(struct net_device *pnetdev, const char *ifname);
extern void netdev_lwip_post_sleep_processing(void);
extern void wireless_send_event(struct net_device *dev, unsigned int cmd, union iwreq_data *wrqu, char *extra);

#ifdef CONFIG_CONCURRENT_MODE
struct _io_ops;
_adapter *rtw_drv_if2_init(_adapter *primary_padapter, char *name, void (*set_intf_ops)(struct _io_ops *pops));
void rtw_drv_if2_free(_adapter *pbuddy_padapter);
#endif

#if defined(CONFIG_ISR_THREAD_MODE_POLLING) || defined(CONFIG_ISR_THREAD_MODE_INTERRUPT)
extern thread_return rtw_interrupt_thread(thread_context context);
#endif

#ifdef CONFIG_RECV_TASKLET_THREAD
extern thread_return rtw_recv_tasklet(thread_context context);
#endif

#ifdef CONFIG_XMIT_TASKLET_THREAD
extern thread_return rtw_xmit_tasklet(thread_context context);
#endif

extern struct net_device *rtw_drv_probe(struct net_device* parent_dev, u32 mode);	//Wlan driver init entry
extern void rtw_drv_entry(void);
extern void rtw_drv_halt(void);
extern int rtw_dev_remove(struct net_device *pnetdev);
extern int rtw_ioctl(struct net_device *dev, struct iwreq *rq, int cmd);

#if defined(CONFIG_LX_HCI)
u32 lextra_bus_dma_Interrupt (void* data);
#endif

#endif	//__FREERTOS_INTFS_H_

