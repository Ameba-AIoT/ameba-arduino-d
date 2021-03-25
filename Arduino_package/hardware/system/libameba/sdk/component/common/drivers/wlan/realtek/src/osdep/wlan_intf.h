/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __WLAN_INTF_H__
#define __WLAN_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif
#include <autoconf.h>

#include <wireless.h>
#include "wifi_constants.h"
#include "wifi_structures.h"

#ifndef WLAN0_IDX
	#define WLAN0_IDX	0
#endif
#ifndef WLAN1_IDX
	#define WLAN1_IDX	1
#endif
#ifndef WLAN_UNDEF
	#define WLAN_UNDEF	-1
#endif

/***********************************************************/
/* 
struct sk_buff {
	// These two members must be first.
	struct sk_buff		*next;		// Next buffer in list
	struct sk_buff		*prev;		// Previous buffer in list
	
	struct sk_buff_head	*list;			// List we are on	
	unsigned char		*head;		// Head of buffer
	unsigned char		*data;		// Data head pointer
	unsigned char		*tail;		// Tail pointer
	unsigned char		*end;		//End pointer
	struct net_device 	*dev;		//Device we arrived on/are leaving by 	
	unsigned int 		len;			// Length of actual data 
};
*/
/************************************************************/

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
int rltk_wlan_init(int idx_wlan, rtw_mode_t mode);				//return 0: success. -1:fail
void rltk_wlan_deinit(void);
void rltk_wlan_deinit_fastly(void);
int rltk_wlan_start(int idx_wlan);
void rltk_wlan_statistic(unsigned char idx);
unsigned char rltk_wlan_running(unsigned char idx);		// interface is up. 0: interface is down
int rltk_wlan_control(unsigned long cmd, void *data);
int rltk_wlan_get_sta_max_data_rate(unsigned char *inidata_rate);
int rltk_wlan_get_ccmp_tk(unsigned char *uncst_key);
int rltk_wlan_handshake_done(void);
int rltk_wlan_rf_on(void);
int rltk_wlan_rf_off(void);
int rltk_wlan_check_bus(void);
int rltk_wlan_wireless_mode(unsigned char mode);
int rltk_wlan_get_wireless_mode(unsigned char *pmode);
int rltk_wlan_set_wps_phase(unsigned char is_trigger_wps);
int rltk_wlan_set_eap_phase(unsigned char is_trigger_eap);
unsigned char rltk_wlan_get_eap_phase(void);
int rltk_wlan_set_eap_method(unsigned char eap_method);
int rtw_ps_enable(int enable);
int rltk_wlan_is_connected_to_ap(void);
void rltk_wlan_btcoex_set_bt_state(unsigned char state);
void rltk_wlan_set_scan_chan_interval(unsigned short interval_ms);
int rltk_wlan_change_channel_plan(unsigned char channel_plan);

void rltk_wlan_enable_adaptivity(unsigned char enable);
void rltk_wlan_set_adaptivity_mode(unsigned char mode);
void rltk_wlan_enable_trp_tis_cert(unsigned char enable);
void rltk_wlan_set_tx_pwr_lmt(unsigned char value);
void rltk_wlan_set_tx_pwr_by_rate(unsigned char value);
#ifdef CONFIG_POWER_SAVING
void rltk_wlan_enable_powersave(unsigned char enable);
#endif

#ifdef CONFIG_INCLUDE_WPA_PSK
void rltk_psk_info_set(struct psk_info * psk_data);
void rltk_psk_info_get(struct psk_info * psk_data);
#endif

#ifdef CONFIG_WLAN_SWITCH_MODE
int rltk_wlan_reinit_drv_sw(const char *ifname, rtw_mode_t mode);
int rltk_set_mode_prehandle(rtw_mode_t curr_mode, rtw_mode_t next_mode, const char *ifname);
int rltk_set_mode_posthandle(rtw_mode_t curr_mode, rtw_mode_t next_mode, const char *ifname);
#endif

#ifdef CONFIG_IEEE80211W
void rltk_wlan_tx_sa_query(unsigned char key_type);
void rltk_wlan_tx_deauth(unsigned char b_broadcast, unsigned char key_type);
void rltk_wlan_tx_auth(void);
#endif

#if defined (CONFIG_AP_MODE)
int rltk_set_hidden_ssid(const char * ifname, unsigned char value);
#endif
unsigned char rltk_get_band_type(void);

#ifdef	__cplusplus
}
#endif



#endif //#ifndef __WLAN_INTF_H__
