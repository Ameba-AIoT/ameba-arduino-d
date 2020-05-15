/******************************************************************************
 *
 * Copyright(c) 2013 Realtek Corporation. All rights reserved.
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
#ifndef __RTW_BTCOEX_SOC_H__
#define __RTW_BTCOEX_SOC_H__

#include <drv_types.h>

//wifi event
enum {
	COEX_EVENT_WIFI_OFF = 0,
	COEX_EVENT_WIFI_ON = 1,
	COEX_EVENT_WIFI_SCAN_START = 2,
	COEX_EVENT_WIFI_SCAN_DONE = 3,
	COEX_EVENT_WIFI_JOIN_START = 4,
	COEX_EVENT_WIFI_JOIN_SUCCESS = 5,
	COEX_EVENT_WIFI_JOIN_FAIL = 6,
	COEX_EVENT_WIFI_DISCONNECT = 7,
	COEX_EVENT_WIFI_CHANGE_MODE = 8,

	COEX_EVENT_WIFI_MAX
};

struct mailbox_info {
	u8	id;
	u8	*data;
	u8	len;
	_list	list;
};

struct coex_hal_fun_t{
	void (*run_case)(_adapter * padapter, u16 state);
	void (*hal_coex_init)(_adapter * padapter);
	u16 (*wifi_tdma_scan_start)(PADAPTER padapter, RT_SCAN_TYPE scan_type);
	u16 (*wifi_tdma_scan_handle)(PADAPTER padapter);
};

struct wifi_tdma_scan_t{
	u8 scaning;
	u8 slot_num;
	u8 cur_slot;
};

/*may be used before both devices on*/
struct coex_t{
	u8 coex_en;
	u8 wifi_state;
	u8 wifi_last_state;
	u8 bt_state;
	u16 state;
	_mutex coex_run_lock;
	struct wifi_tdma_scan_t wifi_tdma_scan;
	struct coex_hal_fun_t* hal_fun;
	
};

/*only be used after both devices on, so it can be free when only one device is on */
struct coex_priv{
	int table_idx;
	u8 ble_scan_duty;
};

typedef struct _coex_bt_info {
	//low byte 1
	u8 length:8;
	
	//low byte 2
	u8 Connection:1;
	u8 rsvd1:7;
	
	//low byte 3
	u8 rsvd2:8;
	
	//hign byte 0
	u8 rsvd3:8;
	
	//hign byte 1
	u8 rsvd5:5;
	u8 Ble_scan_en:1;
	u8 rsvd4:2;

	//hign byte 2
	u8 rsvd6:8;
	
	//hign byte 3
	u8 rsvd7:8;
}coex_bt_info;

#define COEX_MBOX_STACKSIZE                    256

int rtw_coex_mailbox_to_wifi(_adapter *padapter, u8 *data, u8 len);
int rtw_coex_wifi_info_update(_adapter *padapter, u8 wifi_event);
int rtw_coex_ble_scan_duty_update(_adapter *padapter, u8 duty);
void rtw_btcoex_init(PADAPTER padapter);
void rtw_btcoex_deinit(PADAPTER padapter);
int rtw_coex_bt_enable(_adapter *padapter, u8 enable);
void rtw_coex_wifi_enable(PADAPTER padapter, u8 enable);
u16 rtw_coex_wifi_tdma_scan_start(PADAPTER padapter, RT_SCAN_TYPE scan_type);
u16 rtw_coex_wifi_tdma_scan_handle(PADAPTER padapter);

#endif /* __RTW_BTCOEX_SOC_H__ */
