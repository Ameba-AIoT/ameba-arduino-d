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
#ifndef __RTW_MLME_H_
#define __RTW_MLME_H_

#ifdef CONFIG_INTEL_WIDI
#include <rtw_intel_widi.h>
#endif

#if defined(PLATFORM_ECOS)
#define	MAX_BSS_CNT	10	//alloc less wlan_network due to memory limitation - Alex Fang
#elif defined(PLATFORM_FREERTOS) || defined(PLATFORM_CMSIS_RTOS) || defined(PLATFORM_CUSTOMER_RTOS)
#define	MAX_BSS_CNT	1	//alloc less wlan_network due to memory limitation - Alex Fang
#else
#define	MAX_BSS_CNT	128
#endif
//#define   MAX_JOIN_TIMEOUT	2000
//#define   MAX_JOIN_TIMEOUT	2500
#define   MAX_JOIN_TIMEOUT	6500

#ifdef CONFIG_MULTICAST
#define MULTICAST_LIST_SIZE		4
#endif

//	Commented by Albert 20101105
//	Increase the scanning timeout because of increasing the SURVEY_TO value.

#ifdef CONFIG_BT_COEXIST_SOC 
#define 	SCANNING_TIMEOUT 	12000
#else
#define 	SCANNING_TIMEOUT 	8000
#endif

#define	SCAN_INTERVAL	(30) // unit:2sec, 30*2=60sec

#ifdef PALTFORM_OS_WINCE
#define	SCANQUEUE_LIFETIME 12000000 // unit:us
#else
#define	SCANQUEUE_LIFETIME 20 // unit:sec
#endif

#ifdef CONFIG_P2P
	#define MLME_IS_PD(adapter) rtw_p2p_chk_role(&(adapter)->wdinfo, P2P_ROLE_DEVICE)
	#define MLME_IS_GC(adapter) rtw_p2p_chk_role(&(adapter)->wdinfo, P2P_ROLE_CLIENT)
	#define MLME_IS_GO(adapter) rtw_p2p_chk_role(&(adapter)->wdinfo, P2P_ROLE_GO)
#else /* !CONFIG_P2P */
	#define MLME_IS_PD(adapter) 0
	#define MLME_IS_GC(adapter) 0
	#define MLME_IS_GO(adapter) 0
#endif /* !CONFIG_P2P */

#define 	WIFI_NULL_STATE		0x00000000
#define	WIFI_ASOC_STATE		0x00000001		// Under Linked state...
#define 	WIFI_REASOC_STATE	       0x00000002
#define	WIFI_SLEEP_STATE	       0x00000004
#define	WIFI_STATION_STATE	0x00000008
#define	WIFI_AP_STATE				0x00000010
#define	WIFI_ADHOC_STATE			0x00000020
#define   WIFI_ADHOC_MASTER_STATE 0x00000040
#define   WIFI_UNDER_LINKING		0x00000080
#define WIFI_MESH_STATE					0x00000200
//#define WIFI_UNDER_CMD			0x00000200
// ========== P2P Section Start ===============
#define	WIFI_P2P_LISTEN_STATE		0x00010000
#define	WIFI_P2P_GROUP_FORMATION_STATE		0x00020000
// ========== P2P Section End ===============
#define WIFI_UNDER_WPS			0x00000100
#define WIFI_SITE_MONITOR		0x00000800		//to indicate the station is under site surveying

#ifdef WDS
#define	WIFI_WDS				0x00001000
#define	WIFI_WDS_RX_BEACON	0x00002000		// already rx WDS AP beacon
#endif
#ifdef AUTO_CONFIG
#define	WIFI_AUTOCONF			0x00004000
#define	WIFI_AUTOCONF_IND	0x00008000
#endif

//#ifdef UNDER_MPTEST
#define	WIFI_MP_STATE						0x00010000
#define	WIFI_MP_CTX_BACKGROUND			0x00020000	// in continous tx background
#define	WIFI_MP_CTX_ST					0x00040000	// in continous tx with single-tone
#define	WIFI_MP_CTX_BACKGROUND_PENDING	0x00080000	// pending in continous tx background due to out of skb
#define	WIFI_MP_CTX_CCK_HW				0x00100000	// in continous tx
#define	WIFI_MP_CTX_CCK_CS				0x00200000	// in continous tx with carrier suppression
#define   WIFI_MP_LPBK_STATE				0x00400000
//#endif
#define WIFI_OP_CH_SWITCHING			0x00800000

enum {
	MLME_ACTION_UNKNOWN,
	MLME_ACTION_NONE,
	MLME_SCAN_ENABLE, /* WIFI_SITE_MONITOR */
	MLME_SCAN_ENTER, /* WIFI_SITE_MONITOR && !SCAN_DISABLE && !SCAN_BACK_OP */
	MLME_SCAN_DONE, /*  WIFI_SITE_MONITOR && (SCAN_DISABLE || SCAN_BACK_OP) */
	MLME_SCAN_DISABLE, /* WIFI_SITE_MONITOR is going to be cleared */
	MLME_STA_CONNECTING,
	MLME_STA_CONNECTED,
	MLME_STA_DISCONNECTED,
	MLME_TDLS_LINKED,
	MLME_TDLS_NOLINK,
	MLME_AP_STARTED,
	MLME_AP_STOPPED,
	MLME_ADHOC_STARTED,
	MLME_ADHOC_STOPPED,
	MLME_MESH_STARTED,
	MLME_MESH_STOPPED,
	MLME_OPCH_SWITCH,
};


//#define _FW_UNDER_CMD		WIFI_UNDER_CMD
#define _FW_UNDER_LINKING	WIFI_UNDER_LINKING
#define _FW_LINKED			WIFI_ASOC_STATE
#define _FW_UNDER_SURVEY	WIFI_SITE_MONITOR

enum dot11AuthAlgrthmNum {
 dot11AuthAlgrthm_Open = 0,
 dot11AuthAlgrthm_Shared,
 dot11AuthAlgrthm_8021X,
 dot11AuthAlgrthm_SAE,
 dot11AuthAlgrthm_Auto,
 dot11AuthAlgrthm_WAPI,
 dot11AuthAlgrthm_FT_PSK,
 dot11AuthAlgrthm_MaxNum
};

/**
 * enum mfp_options - Management frame protection (IEEE 802.11w) options
 */
enum mfp_options {
	NO_MGMT_FRAME_PROTECTION = 0,
	MGMT_FRAME_PROTECTION_OPTIONAL = 1,
	MGMT_FRAME_PROTECTION_REQUIRED = 2,
};
#define MGMT_FRAME_PROTECTION_DEFAULT 3

#define WPA_CAPABILITY_MFPR BIT(6)
#define WPA_CAPABILITY_MFPC BIT(7)

#define adapter_mac_addr(adapter) (adapter->eeprompriv.mac_addr)

// Scan type including active and passive scan.
typedef enum _RT_SCAN_TYPE
{
	SCAN_PASSIVE,
	SCAN_ACTIVE,
	SCAN_MIX,
} RT_SCAN_TYPE, *PRT_SCAN_TYPE;

/*

there are several "locks" in mlme_priv,
since mlme_priv is a shared resource between many threads,
like ISR/Call-Back functions, the OID handlers, and even timer functions.


Each _queue has its own locks, already.
Other items are protected by mlme_priv.lock.

To avoid possible dead lock, any thread trying to modifiying mlme_priv
SHALL not lock up more than one locks at a time!

*/


#define traffic_threshold	10
#define	traffic_scan_period	500

struct sitesurvey_ctrl {
	u64	last_tx_pkts;
	uint	last_rx_pkts;
	sint	traffic_busy;
	_timer	sitesurvey_ctrl_timer;
};

typedef struct _RT_LINK_DETECT_T{
	u32			NumTxOkInPeriod;
	u32			NumRxOkInPeriod;
	u32			NumRxUnicastOkInPeriod;
	u32			NumRxBcnInPeriod;
	BOOLEAN			bBusyTraffic;
	BOOLEAN			bTxBusyTraffic;
	BOOLEAN			bRxBusyTraffic;
	BOOLEAN			bHigherBusyTraffic;	// For interrupt migration purpose.
	BOOLEAN			bHigherBusyRxTraffic;	// We may disable Tx interrupt according as Rx traffic.
	BOOLEAN			bHigherBusyTxTraffic;	// We may disable Tx interrupt according as Tx traffic.
} RT_LINK_DETECT_T, *PRT_LINK_DETECT_T;

//TODO
#if 0

struct profile_info {
	u8	ssidlen;
	u8	ssid[ WLAN_SSID_MAXLEN ];
	u8	peermac[ ETH_ALEN ];
};

struct tx_invite_req_info{
	u8					token;
	u8					benable;
	u8					go_ssid[ WLAN_SSID_MAXLEN ];
	u8					ssidlen;
	u8					go_bssid[ ETH_ALEN ];
	u8					peer_macaddr[ ETH_ALEN ];
	u8					operating_ch;	//	This information will be set by using the p2p_set op_ch=x
	u8					peer_ch;		//	The listen channel for peer P2P device

};

struct tx_invite_resp_info{
	u8					token;	//	Used to record the dialog token of p2p invitation request frame.
};

#ifdef CONFIG_WFD

struct wifi_display_info{
	u16					wfd_enable;			//	Eanble/Disable the WFD function.
	u16					rtsp_ctrlport;		//	TCP port number at which the this WFD device listens for RTSP messages
	u16					peer_rtsp_ctrlport;	//	TCP port number at which the peer WFD device listens for RTSP messages
											//	This filed should be filled when receiving the gropu negotiation request

	u8					peer_session_avail;	//	WFD session is available or not for the peer wfd device.
											//	This variable will be set when sending the provisioning discovery request to peer WFD device.
											//	And this variable will be reset when it is read by using the iwpriv p2p_get wfd_sa command.
	u8					ip_address[4];
	u8					peer_ip_address[4];
	u8					wfd_pc;				//	WFD preferred connection
											//	0 -> Prefer to use the P2P for WFD connection on peer side.
											//	1 -> Prefer to use the TDLS for WFD connection on peer side.
											
	u8					wfd_device_type;	//	WFD Device Type
											//	0 -> WFD Source Device
											//	1 -> WFD Primary Sink Device

};
#endif //CONFIG_WFD

struct tx_provdisc_req_info{
	u16					wps_config_method_request;	//	Used when sending the provisioning request frame
	u16					peer_channel_num[2];		//	The channel number which the receiver stands.
	NDIS_802_11_SSID	ssid;
	u8					peerDevAddr[ ETH_ALEN ];		//	Peer device address
	u8					peerIFAddr[ ETH_ALEN ];		//	Peer interface address
	u8					benable;					//	This provision discovery request frame is trigger to send or not
};

struct rx_provdisc_req_info{	//When peer device issue prov_disc_req first, we should store the following informations
	u8					peerDevAddr[ ETH_ALEN ];		//	Peer device address
	u8					strconfig_method_desc_of_prov_disc_req[4];	//	description for the config method located in the provisioning discovery request frame.	
																	//	The UI must know this information to know which config method the remote p2p device is requiring.
};

struct tx_nego_req_info{
	u16					peer_channel_num[2];		//	The channel number which the receiver stands.
	u8					peerDevAddr[ ETH_ALEN ];		//	Peer device address
	u8					benable;					//	This negoitation request frame is trigger to send or not
};

struct group_id_info{
	u8					go_device_addr[ ETH_ALEN ];	//	The GO's device address of this P2P group
	u8					ssid[ WLAN_SSID_MAXLEN ];	//	The SSID of this P2P group
};

#ifdef CONFIG_IOCTL_CFG80211
struct cfg80211_wifidirect_info{
	_timer					remain_on_ch_timer;
	u8						restore_channel;
	struct ieee80211_channel	remain_on_ch_channel;
	enum nl80211_channel_type	remain_on_ch_type;
	u64						remain_on_ch_cookie;
	struct net_device 			*remain_on_ch_dev;
	bool is_ro_ch;
};
#endif //CONFIG_IOCTL_CFG80211

#endif

struct wifidirect_info{
	enum P2P_ROLE			role;
	enum P2P_STATE			p2p_state;
	u8						baction_tx_pending;
	u8						pending_peer[ETH_ALEN];
	struct xmit_frame			*pending_action;
	_timer					pre_tx_scan_timer;
	
#if 0
	_adapter*				padapter;
	_timer					find_phase_timer;
	_timer					restore_p2p_state_timer;
	
	//	Used to do the scanning. After confirming the peer is availalble, the driver transmits the P2P frame to peer.
	_timer					pre_tx_scan_timer;
#ifdef CONFIG_CONCURRENT_MODE
	//	Used to switch the channel between legacy AP and listen state.
	_timer					ap_p2p_switch_timer;
#endif
	struct tx_provdisc_req_info	tx_prov_disc_info;
	struct rx_provdisc_req_info rx_prov_disc_info;
	struct tx_invite_req_info	invitereq_info;
	struct profile_info			profileinfo[ P2P_MAX_PERSISTENT_GROUP_NUM ];	//	Store the profile information of persistent group
	struct tx_invite_resp_info	inviteresp_info;
	struct tx_nego_req_info	nego_req_info;
	struct group_id_info		groupid_info;	//	Store the group id information when doing the group negotiation handshake.
#ifdef CONFIG_WFD
	struct wifi_display_info		*wfd_info;
#endif	
	enum P2P_ROLE			role;
	enum P2P_STATE			pre_p2p_state;
	enum P2P_STATE			p2p_state;
	u8 						device_addr[ETH_ALEN];	//	The device address should be the mac address of this device.
	u8						interface_addr[ETH_ALEN];
	u8						social_chan[4];
	u8						listen_channel;
	u8						operating_channel;
	u8						listen_dwell;		//	This value should be between 1 and 3
	u8						support_rate[8];
	u8						p2p_wildcard_ssid[P2P_WILDCARD_SSID_LEN];
	u8						intent;		//	should only include the intent value.
	u8						p2p_peer_interface_addr[ ETH_ALEN ];
	u8						p2p_peer_device_addr[ ETH_ALEN ];
	u8						peer_intent;	//	Included the intent value and tie breaker value.
	u8						device_name[ WPS_MAX_DEVICE_NAME_LEN ];	//	Device name for displaying on searching device screen
	u8						device_name_len;
	u8						profileindex;	//	Used to point to the index of profileinfo array
	u8						peer_operating_ch;
	u8						find_phase_state_exchange_cnt;
	u16						device_password_id_for_nego;	//	The device password ID for group negotation
	u8						negotiation_dialog_token;
	u8						nego_ssid[ WLAN_SSID_MAXLEN ];	//	SSID information for group negotitation
	u8						nego_ssidlen;
	u8 						p2p_group_ssid[WLAN_SSID_MAXLEN];
	u8 						p2p_group_ssid_len;
	u8						persistent_supported;		//	Flag to know the persistent function should be supported or not.
														//	In the Sigma test, the Sigma will provide this enable from the sta_set_p2p CAPI.
														//	0: disable
														//	1: enable
	u8						session_available;			//	Flag to set the WFD session available to enable or disable "by Sigma"
														//	In the Sigma test, the Sigma will disable the session available by using the sta_preset CAPI.
														//	0: disable
														//	1: enable

	u8						wfd_tdls_enable;			//	Flag to enable or disable the TDLS by WFD Sigma
														//	0: disable
														//	1: enable
	u8						wfd_tdls_weaksec;			//	Flag to enable or disable the weak security function for TDLS by WFD Sigma
														//	0: disable
														//	In this case, the driver can't issue the tdsl setup request frame.
														//	1: enable
														//	In this case, the driver can issue the tdls setup request frame
														//	even the current security is weak security.

	enum	P2P_WPSINFO		ui_got_wps_info;			//	This field will store the WPS value (PIN value or PBC) that UI had got from the user.
	u16						supported_wps_cm;			//	This field describes the WPS config method which this driver supported.
														//	The value should be the combination of config method defined in page104 of WPS v2.0 spec.	
	uint						channel_list_attr_len;		//	This field will contain the length of body of P2P Channel List attribute of group negotitation response frame.
	u8						channel_list_attr[100];		//	This field will contain the body of P2P Channel List attribute of group negotitation response frame.
														//	We will use the channel_cnt and channel_list fields when constructing the group negotitation confirm frame.
#ifdef CONFIG_CONCURRENT_MODE
	u16						ext_listen_interval;	//	The interval to be available with legacy AP (ms)
	u16						ext_listen_period;	//	The time period to be available for P2P listen state (ms)
#endif
	u8						p2p_ps_enable;
	enum P2P_PS				p2p_ps; // indicate p2p ps state
	u8						noa_index; // Identifies and instance of Notice of Absence timing.
	u8						ctwindow; // Client traffic window. A period of time in TU after TBTT.
	u8						opp_ps; // opportunistic power save.
	u8						noa_num; // number of NoA descriptor in P2P IE.
	u8						noa_count[P2P_MAX_NOA_NUM]; // Count for owner, Type of client.
	u32						noa_duration[P2P_MAX_NOA_NUM]; // Max duration for owner, preferred or min acceptable duration for client.
	u32						noa_interval[P2P_MAX_NOA_NUM]; // Length of interval for owner, preferred or max acceptable interval of client.
	u32						noa_start_time[P2P_MAX_NOA_NUM]; // schedule expressed in terms of the lower 4 bytes of the TSF timer.
#endif
};
#if 0
struct tdls_ss_record{	//signal strength record
	u8		macaddr[ETH_ALEN];
	u8		RxPWDBAll;
	u8		is_tdls_sta;	// _TRUE: direct link sta, _FALSE: else
};

struct tdls_info{
	u8					ap_prohibited;
	uint					setup_state;
	u8					sta_cnt;
	u8					sta_maximum;	// 1:tdls sta is equal (NUM_STA-1), reach max direct link number; 0: else;
	struct tdls_ss_record	ss_record;
	u8					macid_index;	//macid entry that is ready to write
	u8					clear_cam;	//cam entry that is trying to clear, using it in direct link teardown
	u8					ch_sensing;
	u8					cur_channel;
	u8					candidate_ch;
	u8					collect_pkt_num[MAX_CHANNEL_NUM];
	_lock				cmd_lock;
	_lock				hdl_lock;
	u8					watchdog_count;
	u8					dev_discovered;		//WFD_TDLS: for sigma test
	u8					enable;
#ifdef CONFIG_WFD
	struct wifi_display_info		*wfd_info;
#endif		
};

#endif	//#if 0

struct mlme_priv {

	_lock	lock;
	sint	fw_state;	//shall we protect this variable? maybe not necessarily...
	u8 bScanInProcess;
	u8	to_join; //flag
	#ifdef CONFIG_LAYER2_ROAMING
	u8 to_roaming; // roaming trying times
	#endif

	u8		*nic_hdl;		//can be removed

	//u8		not_indic_disco;
	_list		*pscanned;
	_queue	free_bss_pool;
	_queue	scanned_queue;
	u8		*free_bss_buf;
	u16		num_of_scanned;
	
#if SUPPORT_SCAN_BUF	// Cloud 2013/12/20
	u8		*scan_buf;
	u32		scan_buf_len;
	u16		scan_cnt;
	u16		scan_type;
#endif

	NDIS_802_11_SSID	assoc_ssid;
	u8	assoc_bssid[6];

#ifdef CONFIG_WOWLAN
	u8	ip_addr[4];
#endif

	struct wlan_network	cur_network;

	//uint wireless_mode; no used, remove it

	u32	scan_interval;

	_timer assoc_timer;

	u8 assoc_by_bssid;
	u8 assoc_by_rssi;

	_timer scan_to_timer; // driver itself handles scan_timeout status.
	u32 scan_start_time; // used to evaluate the time spent in scanning

	#ifdef CONFIG_SET_SCAN_DENY_TIMER
	_timer set_scan_deny_timer;
	ATOMIC_T set_scan_deny; //0: allowed, 1: deny
	#endif

	struct qos_priv qospriv;

#ifdef CONFIG_80211N_HT

	/* Number of non-HT AP/stations */
	u16 num_sta_no_ht; //int num_sta_no_ht;

	/* Number of HT AP/stations 20 MHz */
	//int num_sta_ht_20mhz; 


	u16 num_FortyMHzIntolerant; //int num_FortyMHzIntolerant;

	struct ht_priv	htpriv;

#endif

#ifdef CONFIG_80211AC_VHT
	struct vht_priv vhtpriv;
#endif

	RT_LINK_DETECT_T	LinkDetectInfo;
	_timer	dynamic_chk_timer; //dynamic/periodic check timer

 	u8 	key_mask; //use for ips to set wep key after ips_leave
	u8	acm_mask; // for wmm acm mask
	u8	ChannelPlan;
	RT_SCAN_TYPE 	scan_mode; // active: 1, passive: 0

#ifdef CONFIG_WPS	
	u8 *wps_probe_req_ie;
	u32 wps_probe_req_ie_len;
	u8 *wps_assoc_req_ie;
	u32 wps_assoc_req_ie_len;
#endif	

#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	/* Number of associated Non-ERP stations (i.e., stations using 802.11b
	 * in 802.11g BSS) */
	u16 num_sta_non_erp;

	/* Number of associated stations that do not support Short Slot Time */
	u16 num_sta_no_short_slot_time;

	/* Number of associated stations that do not support Short Preamble */
	u16 num_sta_no_short_preamble;

	//int olbc; /* Overlapping Legacy BSS Condition */

	/* Number of HT associated stations that do not support greenfield */
	u16 num_sta_ht_no_gf;

	/* Number of associated non-HT stations */
	//int num_sta_no_ht;

	/* Number of HT associated stations 20 MHz */
	u16 num_sta_ht_20mhz;

	/* Overlapping BSS information */
	u8 olbc_ht;
	
#ifdef CONFIG_80211N_HT
	u16 ht_op_mode;
#endif /* CONFIG_80211N_HT */	

#ifdef CONFIG_WPS	
	u8 *wps_beacon_ie;	
	u8 *wps_probe_resp_ie;
	u8 *wps_assoc_resp_ie; // for CONFIG_IOCTL_CFG80211, this IE could include p2p ie

	u32 wps_beacon_ie_len;
	u32 wps_probe_resp_ie_len;
	u32 wps_assoc_resp_ie_len;
	

#ifdef CONFIG_P2P_NEW
	u8 *p2p_beacon_ie;
	u8 *p2p_probe_req_ie;
	u8 *p2p_probe_resp_ie;	
//	u8 *p2p_go_probe_resp_ie; //for GO	
	u8 *p2p_assoc_req_ie;
	u8 *p2p_assoc_rsp_ie;

	u32 p2p_beacon_ie_len;
	u32 p2p_probe_req_ie_len;
	u32 p2p_probe_resp_ie_len;
//	u32 p2p_go_probe_resp_ie_len; //for GO
	u32 p2p_assoc_req_ie_len;
	u32 p2p_assoc_rsp_ie_len;
#endif //CONFIG_P2P
#endif //CONFIG_WPS
	
	_lock	bcn_update_lock;
	u8		update_bcn;
	
	u8 ori_ch;
	u8 ori_bw;
	u8 ori_offset;

#if USE_DEDICATED_BCN_TX
	//Dedicated xmit frame and buffer for beacon update - Alex Fang
	struct xmit_frame bcn_xmit_frame;
	struct xmit_buf bcn_xmit_buf;
	//u8 bcn_buf[256];
	u8 bcn_buf[320];  //p2p go beacon size is about 272+32 bytes
#endif
#endif //#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)

#ifdef RTK_DMP_PLATFORM
	// DMP kobject_hotplug function  signal need in passive level
	_workitem	Linkup_workitem;
	_workitem	Linkdown_workitem;
#endif

#ifdef CONFIG_INTEL_WIDI
	int	widi_state;
	int	listen_state;
	_timer	listen_timer;
	ATOMIC_T	rx_probe_rsp; // 1:receive probe respone from RDS source.
	u8	*l2sdTaBuffer;
	u8	channel_idx;
	u8	group_cnt;	//In WiDi 3.5, they specified another scan algo. for WFD/RDS co-existed
	u8	sa_ext[L2SDTA_SERVICE_VE_LEN];
#endif // CONFIG_INTEL_WIDI

	u32 lastscantime;
	
#ifdef CONFIG_CONCURRENT_MODE
	u8	scanning_via_buddy_intf;
#endif

#ifdef CONFIG_FTP_PROTECT
	u8	ftp_lock_flag;
#endif //CONFIG_FTP_PROTECT

#ifdef CONFIG_MULTICAST
	u32 multicast_list[MULTICAST_LIST_SIZE];
#endif
	//For fast reconnection to keep frame info temporarily
	union recv_frame *p_copy_recv_frame;
	unsigned short disassoc_reason;
};

#ifdef CONFIG_AP_MODE

struct hostapd_priv
{
	_adapter *padapter;

#ifdef CONFIG_HOSTAPD_MLME
	struct net_device *pmgnt_netdev;
	struct usb_anchor anchored;
#endif	
	
};

extern int hostapd_mode_init(_adapter *padapter);
extern void hostapd_mode_unload(_adapter *padapter);
#endif


extern void rtw_joinbss_event_prehandle(_adapter *adapter, u8 *pbuf);
extern void rtw_survey_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_surveydone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_joinbss_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stassoc_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stadel_event_callback(_adapter *adapter, u8 *pbuf);
#if defined(CONFIG_IEEE80211W) && defined(CONFIG_AP_MODE)
extern void rtw_sta_timeout_event_callback(_adapter *adapter, u8 *pbuf);
#endif /* CONFIG_IEEE80211W */
extern void rtw_atimdone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_cpwm_event_callback(_adapter *adapter, u8 *pbuf);

#ifdef PLATFORM_WINDOWS
extern thread_return event_thread(void *context);

extern void rtw_join_timeout_handler (
	IN	PVOID					SystemSpecific1,
	IN	PVOID					FunctionContext,
	IN	PVOID					SystemSpecific2,
	IN	PVOID					SystemSpecific3
	);

extern void _rtw_scan_timeout_handler (
	IN	PVOID					SystemSpecific1,
	IN	PVOID					FunctionContext,
	IN	PVOID					SystemSpecific2,
	IN	PVOID					SystemSpecific3
	);

#endif

#if defined (PLATFORM_LINUX)|| defined (PLATFORM_FREEBSD)
extern int event_thread(void *context);
extern void rtw_join_timeout_handler(void* FunctionContext);
extern void _rtw_scan_timeout_handler(void* FunctionContext);
#endif

extern void rtw_free_network_queue(_adapter *adapter,u8 isfreeall);
extern int rtw_init_mlme_priv(_adapter *adapter);// (struct mlme_priv *pmlmepriv);

extern void rtw_free_mlme_priv (struct mlme_priv *pmlmepriv);


extern sint rtw_select_and_join_from_scanned_queue(struct mlme_priv *pmlmepriv);
extern sint rtw_set_key(_adapter *adapter,struct security_priv *psecuritypriv,sint keyid, u8 set_tx);
extern sint rtw_set_auth(_adapter *adapter,struct security_priv *psecuritypriv);
extern sint rtw_linked_check(_adapter *padapter);

__inline static u8 *get_bssid(struct mlme_priv *pmlmepriv)
{	//if sta_mode:pmlmepriv->cur_network.network.MacAddress=> bssid
	// if adhoc_mode:pmlmepriv->cur_network.network.MacAddress=> ibss mac address
	return pmlmepriv->cur_network.network.MacAddress;
}

__inline static sint check_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	if (pmlmepriv->fw_state & state)
		return _TRUE;

	return _FALSE;
}

__inline static sint get_fwstate(struct mlme_priv *pmlmepriv)
{
	return pmlmepriv->fw_state;
}

/*
 * No Limit on the calling context,
 * therefore set it to be the critical section...
 *
 * ### NOTE:#### (!!!!)
 * MUST TAKE CARE THAT BEFORE CALLING THIS FUNC, YOU SHOULD HAVE LOCKED pmlmepriv->lock
 */
__inline static void set_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	pmlmepriv->fw_state |= state;
	//FOR HW integration
	if(_FW_UNDER_SURVEY==state){
		pmlmepriv->bScanInProcess = _TRUE;
	}
}

__inline static void _clr_fwstate_(struct mlme_priv *pmlmepriv, sint state)
{
	pmlmepriv->fw_state &= ~state;
	//FOR HW integration
	if(_FW_UNDER_SURVEY==state){
		pmlmepriv->bScanInProcess = _FALSE;
	}
}

/*
 * No Limit on the calling context,
 * therefore set it to be the critical section...
 */
__inline static void clr_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	if (check_fwstate(pmlmepriv, state) == _TRUE)
		pmlmepriv->fw_state ^= state;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void clr_fwstate_ex(struct mlme_priv *pmlmepriv, sint state)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	_clr_fwstate_(pmlmepriv, state);
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void up_scanned_network(struct mlme_priv *pmlmepriv)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned++;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

#ifdef CONFIG_CONCURRENT_MODE
sint rtw_buddy_adapter_up(_adapter *padapter);
sint check_buddy_fwstate(_adapter *padapter, sint state);
#endif //CONFIG_CONCURRENT_MODE

__inline static void down_scanned_network(struct mlme_priv *pmlmepriv)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned--;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void set_scanned_network_val(struct mlme_priv *pmlmepriv, sint val)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned = val;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

extern u16 rtw_get_capability(WLAN_BSSID_EX *bss);
extern void rtw_update_scanned_network(_adapter *adapter, WLAN_BSSID_EX *target);
extern void rtw_disconnect_hdl_under_linked(_adapter* adapter, struct sta_info *psta, u8 free_assoc);
extern void rtw_generate_random_ibss(u8 *pibss);
extern struct wlan_network* rtw_find_network(_queue *scanned_queue, u8 *addr);
extern struct wlan_network* rtw_get_oldest_wlan_network(_queue *scanned_queue);

extern void rtw_free_assoc_resources(_adapter* adapter, int lock_scanned_queue);
extern void rtw_indicate_disconnect(_adapter* adapter);
extern void rtw_indicate_connect(_adapter* adapter);
void rtw_indicate_scan_done( _adapter *padapter, bool aborted);
void rtw_scan_abort(_adapter *adapter);

extern int rtw_restruct_sec_ie(_adapter *adapter,u8 *in_ie,u8 *out_ie,uint in_len);
extern int rtw_restruct_wmm_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len, uint initial_out_len);
//extern void rtw_init_registrypriv_dev_network(_adapter *adapter);

//extern void rtw_update_registrypriv_dev_network(_adapter *adapter);

extern void rtw_get_encrypt_decrypt_from_registrypriv(_adapter *adapter);

extern void _rtw_join_timeout_handler(_adapter *adapter);
extern void rtw_scan_timeout_handler(_adapter *adapter);

extern void rtw_dynamic_check_timer_handlder(_adapter *adapter);
#ifdef CONFIG_SET_SCAN_DENY_TIMER
extern void rtw_set_scan_deny_timer_hdl(_adapter *adapter);
void rtw_set_scan_deny(struct mlme_priv *mlmepriv, u32 ms);
#endif


extern int _rtw_init_mlme_priv(_adapter *padapter);

void rtw_free_mlme_priv_ie_data(struct mlme_priv *pmlmepriv);

extern void _rtw_free_mlme_priv(struct mlme_priv *pmlmepriv);

extern int _rtw_enqueue_network(_queue *queue, struct wlan_network *pnetwork);

extern struct wlan_network* _rtw_dequeue_network(_queue *queue);

extern struct wlan_network* _rtw_alloc_network(struct mlme_priv *pmlmepriv);


extern void _rtw_free_network(struct mlme_priv *pmlmepriv, struct wlan_network *pnetwork, u8 isfreeall);
extern void _rtw_free_network_nolock(struct mlme_priv *pmlmepriv, struct wlan_network *pnetwork);


extern struct wlan_network* _rtw_find_network(_queue *scanned_queue, u8 *addr);

extern void _rtw_free_network_queue(_adapter* padapter, u8 isfreeall);

extern sint rtw_if_up(_adapter *padapter);


u8 *rtw_get_capability_from_ie(u8 *ie);
u8 *rtw_get_timestampe_from_ie(u8 *ie);
u8 *rtw_get_beacon_interval_from_ie(u8 *ie);


void rtw_joinbss_reset(_adapter *padapter);

#ifdef CONFIG_80211N_HT
unsigned int rtw_restructure_ht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_update_ht_cap(_adapter *padapter, u8 *pie, uint ie_len);
void rtw_issue_addbareq_cmd(_adapter *padapter, struct xmit_frame *pxmitframe);
#endif

int rtw_is_same_ibss(_adapter *adapter, struct wlan_network *pnetwork);
int is_same_network(WLAN_BSSID_EX *src, WLAN_BSSID_EX *dst);

#ifdef CONFIG_LAYER2_ROAMING
void rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
void _rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
#endif


#ifdef CONFIG_INTEL_PROXIM
void rtw_proxim_enable(_adapter *padapter);
void rtw_proxim_disable(_adapter *padapter);
void rtw_proxim_send_packet(_adapter *padapter,u8 *pbuf,u16 len,u8 hw_rate);
#endif //CONFIG_INTEL_PROXIM

extern void rtw_os_indicate_disconnect( _adapter *adapter );
extern void rtw_os_indicate_scan_done( _adapter *padapter, bool aborted);
extern void rtw_reset_securitypriv( _adapter *adapter );
#endif //__RTL871X_MLME_H_

