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

#ifndef WLAN0_IDX
	#define WLAN0_IDX	0
#endif
#ifndef WLAN1_IDX
	#define WLAN1_IDX	1
#endif
#ifndef WLAN_UNDEF
	#define WLAN_UNDEF	-1
#endif

typedef struct customized_chl_cfg {
	unsigned char chnl_index;//for 2.4G:{1,2,3,4,5,6,7,8,9,10,11,12,13,14}. for 5G:{36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165}
	unsigned char scan_type; //0:SCAN_PASSIVE. 1:SCAN_ACTIVE. 2:SCAN_MIX
}customized_chl_cfg_t;

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

struct long_period_tickless_param {
	unsigned char enable;
	unsigned char max_bcn_early;
	unsigned char fix_bcn_to;
	unsigned char bcn_to_lmt;
	unsigned char bcn_to_lmt_compen;
};

/************************************************************/

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
int rltk_wlan_init(int idx_wlan, rtw_mode_t mode);				//return 0: success. -1:fail
void rltk_wlan_deinit(void);
void rltk_wlan_deinit_fastly(void);
int rltk_wlan_start(int idx_wlan);
int rltk_set_tx_power_percentage(rtw_tx_pwr_percentage_t power_percentage_idx);
void rltk_wlan_statistic(unsigned char idx);
void rltk_wlan_statistic_extended(unsigned char idx);
unsigned char rltk_wlan_running(unsigned char idx);		// interface is up. 0: interface is down
int rltk_wlan_control(unsigned long cmd, void *data);
int rltk_wlan_handshake_done(void);
int rltk_wlan_rf_on(void);
int rltk_wlan_rf_off(void);
int rltk_wlan_check_bus(void);
int rltk_wlan_wireless_mode(unsigned char mode);
int rltk_wlan_get_wireless_mode(unsigned char *pmode);
int rltk_wlan_get_cur_wireless_mode(void);
int rltk_wlan_set_wpa_mode(const char *ifname, unsigned int wpa_mode);
int rltk_wlan_set_wps_phase(unsigned char is_trigger_wps);
void rltk_wlan_PRE_SLEEP_PROCESSING(void);
int rtw_ps_enable(int enable);
int rltk_wlan_is_connected_to_ap(void);
void rltk_wlan_set_no_beacon_timeout(unsigned char timeout_sec);
void rltk_wlan_set_scan_chan_interval(unsigned short interval_ms);
void rltk_wlan_map_in_efuse(unsigned char map_in_efuse);
int rltk_wlan_map_read(unsigned char *data, unsigned short cnts);
void rltk_set_mac(unsigned char * mac);
void rltk_wlan_btcoex_set_bt_state(unsigned char state);
int rltk_wlan_change_channel_plan(unsigned char channel_plan);
int rltk_set_null1_param(unsigned char check_period, unsigned char pkt_num, unsigned char limit, unsigned char interval);
int rltk_coex_ble_scan_duty_update( unsigned char duty);
int rltk_coex_set_wlan_slot_random(unsigned char temp);
int rltk_coex_set_wlan_slot_preempting(unsigned char bitmask);
int rltk_coex_set_wlan_lps_coex(unsigned char enable);
unsigned char rltk_wlan_ap_compatibility_is_enable(unsigned int bitmap);
void rltk_wlan_enable_proassocaite(unsigned char enable);
void rltk_wlan_enable_check_bcn_info(unsigned char enable);
void rltk_wlan_enable_issue_deauth(unsigned char enable);
void rltk_wlan_enable_wep_auth_algo_switch(unsigned char enable);
void rltk_wlan_enable_delayed_reordering(unsigned char enable);
void rltk_wlan_enable_delay_before_power_save(unsigned char enable);
void rltk_wlan_disable_dpk(void);
unsigned char rltk_wlan_get_ghd_enable_flag(void);
unsigned char rltk_wlan_scan_with_ssid_by_extended_security_is_enable(void);
void rltk_wlan_enable_channel_switch_announcement(unsigned char enable);
int rltk_set_tx_pause(unsigned char pause);
void rltk_set_rts_cts_mode(unsigned char mode);
void rltk_dump_skb_buffers(void);
int rltk_wlan_set_igi(unsigned char igi, unsigned char enable);
int rltk_wlan_get_pbuddy_rssi(int *rssi);
int rltk_wlan_set_tx_data_rate(unsigned char data_rate);
void rltk_wlan_set_max_assoc_timeout(int timeout_ms);
void rltk_wlan_set_max_handshake_timeout(int timeout_ms);
void rltk_wlan_set_power_limit(int enable);
int rltk_get_security_mode_full(const char *ifname);

#ifdef CONFIG_IEEE80211W
void rltk_wlan_tx_sa_query(unsigned char key_type);
void rltk_wlan_tx_deauth(unsigned char b_broadcast, unsigned char key_type);
void rltk_wlan_tx_auth(void);
int rltk_set_pmf(unsigned char mode);
#endif

#if (RTL8192E_SUPPORT == 1)
int rltk_wlan_set_ext_channel(int num);
#endif

#ifdef USER_CTL_POWER_SAVE
int rltk_wlan_ucps_enable(unsigned char enable);
int rltk_wlan_ucps_auto_rf_off_cmd(unsigned char enable,unsigned short timeout);
int rltk_wlan_ucps_close_rf(void);
int rltk_wlan_ucps_sniffer_param(unsigned char sniffer_time, unsigned short sniffer_interval);
#endif

#if defined (CONFIG_AP_MODE)
int rltk_resume_softap(const char *ifname);
#endif

#ifdef CONFIG_CONCURRENT_MODE
void rltk_wlan_enable_powersave_in_STA_AP_mode(void);
void rltk_wlan_disable_powersave_in_STA_AP_mode(void);
#endif

#if CONFIG_AUTO_RECONNECT
unsigned char* rltk_wlan_get_saved_bssid(void);
#endif

void rltk_wlan_set_partial_scan_retry_times(unsigned char times);

#ifdef	__cplusplus
}
#endif



#endif //#ifndef __WLAN_INTF_H__
