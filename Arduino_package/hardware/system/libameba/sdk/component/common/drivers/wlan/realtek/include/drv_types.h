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
/*-------------------------------------------------------------------------------

	For type defines and data structure defines

--------------------------------------------------------------------------------*/


#ifndef __DRV_TYPES_H__
#define __DRV_TYPES_H__

#include <drv_conf.h>

#if defined (__ICCARM__)
#define _PACKED         __packed
#define _WEAK           __weak
#else
#define _PACKED         __attribute__ ((packed))
#define _WEAK           __attribute__ ((weak))
#endif

// Assign memory sectinon usage
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C)
#include <section_config.h>
//#include "rtl_utility_ram.h"
#include "platform/platform_stdlib.h"
#else
#define SRAM_BD_DATA_SECTION
#define WLAN_ROM_TEXT_SECTION
#define WLAN_ROM_DATA_SECTION
#define _LONG_CALL_     
#endif

#define CONFIG_DOSCAN_IN_BUSYTRAFFIC
//#define CONFIG_SPCT_CH_SWITCH

#ifdef CONFIG_TRACE_SKB
#define SKBLIST_ALL								0xFFFFFFFF
// receive
#define	SKBLIST_RECVBUF_MASK					0x0000000F
#define	SKBLIST_RECVBUF							0x00000001
#define	SKBLIST_RECVBUF_FREEQUEUE				0x00000002
#define	SKBLIST_RECVBUF_PENDINGQUEUE			0x00000004

#define	SKBLIST_RECVFRAME_MASK					0x000000F0
#define	SKBLIST_RECVFRAME						0x00000010
#define	SKBLIST_RECVFRAME_FREEQUEUE				0x00000020
#define	SKBLIST_RECVFRAME_SWDECQUEUE			0x00000040
#ifdef CONFIG_RECV_REORDERING_CTRL
#define	SKBLIST_RECVFRAME_REORDERQUEUE			0x00000080
#endif

// transmit
#define	SKBLIST_XMITBUF_MASK					0x0000FF00
#define	SKBLIST_XMITBUF							0x00000100
#define	SKBLIST_XMITEXTBUF						0x00000200
#define	SKBLIST_XMITBUF_FREEQUEUE				0x00000400
#define	SKBLIST_XMITEXTBUF_FREEQUEUE			0x00000800
#define	SKBLIST_XMITBUF_PENDINGQUEUE			0x00001000
#ifdef CONFIG_SDIO_TX_MULTI_QUEUE
#define	SKBLIST_XMITBUF_PENDING0QUEUE			0x00002000
#define	SKBLIST_XMITBUF_PENDING1QUEUE			0x00004000
#define	SKBLIST_XMITBUF_PENDING2QUEUE			0x00008000
#endif

#define	SKBLIST_XMITFRAME_MASK					0x0FFF0000
#define	SKBLIST_XMITFRAME						0x00010000
#define	SKBLIST_XMITFRAME_FREEQUEUE				0x00020000
#define	SKBLIST_XMITFRAME_SLEEPQUEUE			0x00040000
#define	SKBLIST_XMITFRAME_VOQUEUE				0x00100000
#define	SKBLIST_XMITFRAME_VIQUEUE				0x00200000
#define	SKBLIST_XMITFRAME_BEQUEUE				0x00400000
#define	SKBLIST_XMITFRAME_BKQUEUE				0x00800000
#define	SKBLIST_XMITFRAME_BMQUEUE				0x01000000

#define	SKBLIST_POOL							0x10000000
#endif

enum _NIC_VERSION {

	RTL8711_NIC,
	RTL8712_NIC,
	RTL8713_NIC,
	RTL8716_NIC

};

typedef struct _ADAPTER _adapter, ADAPTER,*PADAPTER;

#include "wireless.h"
#include <netdev.h>
#include <osdep_service.h>
#include <device_lock.h>
#include <rtw_byteorder.h>
#include <rtw_io.h>
#include <hci_spec.h>
#include <cmn_info/rtw_sta_info.h>
#include <wlan_bssdef.h>
#include <wifi.h>
//#include "p2p/p2p.h"
#ifdef CONFIG_80211N_HT
#include <rtw_ht.h>
#endif

#ifdef CONFIG_80211AC_VHT
#include <rtw_vht.h>
#endif

#ifdef CONFIG_RTK_MESH
	#include <mesh.h>
	#include <hash_table.h>
	#include <mesh_route.h>
	#include <mesh_security.h>
	#include <mesh_util.h>
	#include <freertos/wrapper.h>
#ifdef CONFIG_SAE_SUPPORT
	#include <mesh_sae.h>
#endif
#endif

#include <wlan_basic_types.h>
#include <rtw_intfs.h>
//#include <hal_pg.h>
#include <rtw_cmd.h>
#include <rtw_xmit.h>
#include <rtw_recv.h>
#include <rtw_rm.h>
#include <hal_com.h>
#ifdef RTW_HALMAC
#include <hal_com_h2c.h>
#endif
#include <rtw_qos.h>
#ifdef CONFIG_INCLUDE_WPA_PSK
#include <rtw_psk.h>
#endif
#ifdef CONFIG_SAE_SUPPORT
#include "rtw_sae.h"
#endif
#ifdef CONFIG_PMKSA_CACHING
#include "rtw_pmksa_cache.h"
#endif
#include <rtw_security.h>
#include <rom_rtw_security.h>
#include <rtw_pwrctrl.h>
#include <rtw_eeprom.h>
#include <sta_info.h>
#include <rtw_mlme.h>
#include <rtw_debug.h>
#include <ieee80211.h>
#include <rom_ieee80211.h>
#include <rtw_ioctl_set.h>
#include <rtw_rf.h>
#include <rtw_event.h>
#include <rtw_led.h>
#include <rtw_mlme_ext.h>
#include <rtw_mi.h>
#include <wifi_performance_monitor.h>
#ifdef CONFIG_P2P_NEW
#include <rtw_p2p.h>
#endif
//#include <rtw_tdls.h>
#include <rtw_ap.h>
#include <rtw_efuse.h>

#include <osdep_intf.h>

#include <hci_intfs.h>

#ifdef CONFIG_HARDWARE_8192E
#include <wifi_conf.h>
#endif

#ifdef CONFIG_WAPI_SUPPORT
#include <rtw_wapi.h>
#endif

#ifdef CONFIG_MP_INCLUDED
#include <rtw_mp.h>
#endif

#ifdef CONFIG_BT_COEXIST
	#include <rtw_btcoex.h>
#endif

#ifdef CONFIG_BT_COEXIST_SOC
	#include <rtw_btcoex_soc.h>
#endif

#include <rtw_btcoex_wifionly.h>

#ifdef CONFIG_MCC_MODE
#include <hal_mcc.h>
#endif

#ifdef CONFIG_IEEE80211R
#include <rtw_ft.h>
#endif
#if defined(CONFIG_RTW_WNM) || defined(CONFIG_IEEE80211K)
#include <rtw_wnm.h>
#endif

#define SPEC_DEV_ID_NONE BIT(0)
#define SPEC_DEV_ID_DISABLE_HT BIT(1)
#define SPEC_DEV_ID_ENABLE_PS BIT(2)
#define SPEC_DEV_ID_RF_CONFIG_1T1R BIT(3)
#define SPEC_DEV_ID_RF_CONFIG_2T2R BIT(4)
#define SPEC_DEV_ID_ASSIGN_IFNAME BIT(5)

typedef unsigned char			BOOLEAN,*PBOOLEAN;
typedef unsigned char			u1Byte,*pu1Byte;
typedef unsigned short			u2Byte,*pu2Byte;
typedef unsigned int			u4Byte,*pu4Byte;
typedef unsigned long long		u8Byte,*pu8Byte;
typedef signed char				s1Byte,*ps1Byte;

struct specific_device_id{

	u32		flags;

	u16		idVendor;
	u16		idProduct;

};

struct registry_priv
{
	u8	chip_version;
//	u8	rfintfs;
//	u8	lbkmode;
	u8	hci;
	NDIS_802_11_SSID	ssid;
//	u8	network_mode;	//infra, ad-hoc, auto
	u8	channel;//ad-hoc support requirement
	u8	wireless_mode;//A, B, G, auto
	u8 	scan_mode;//active, passive
//	u8	radio_enable;
//	u8	preamble;//long, short, auto
	u8	vrtl_carrier_sense;//Enable, Disable, Auto
	u8	vcs_type;//RTS/CTS, CTS-to-self
	u16	rts_thresh;
//	u8	adhoc_tx_pwr;
	u8	soft_ap;
	u8	power_mgnt;
	u8 	ps_enable;
	u8	ips_mode;
    u8	lps_level;
#ifdef CONFIG_FW_PSTIMEOUT
	u8 	ps_timeout;
#endif
	u8	smart_ps;
//	u8	long_retry_lmt;
//	u8	short_retry_lmt;
//	u16	busy_thresh;
//	u8	ack_policy;
	u8	mp_mode;
	u8	software_encrypt;
	u8	software_decrypt;
	#ifdef CONFIG_TX_EARLY_MODE
	u8   	early_mode;
	#endif
	u8	acm_method;
	  //UAPSD
	u8	wmm_enable;
	u8	uapsd_enable;
//	u8	uapsd_max_sp;
//	u8	uapsd_acbk_en;
//	u8	uapsd_acbe_en;
//	u8	uapsd_acvi_en;
//	u8	uapsd_acvo_en;

//	WLAN_BSSID_EX    dev_network;
	u8  tx_bw_mode;
	u32  beacon_period;

#ifdef CONFIG_80211N_HT
	u8	ht_enable;
#if !defined(NOT_SUPPORT_40M)
	u8	cbw40_enable;
#endif
	u8	bw_mode;
	u8	ampdu_enable;//for tx
	u8 	rx_stbc;
	u8	ampdu_amsdu;//A-MPDU Supports A-MSDU is permitted
	/* Short GI support Bit Map */
	/* BIT0 - 20MHz, 1: support, 0: non-support */
	/* BIT1 - 40MHz, 1: support, 0: non-support */
	/* BIT2 - 80MHz, 1: support, 0: non-support */
	/* BIT3 - 160MHz, 1: support, 0: non-support */
	u8	short_gi;
	/* BIT0: Enable VHT LDPC Rx, BIT1: Enable VHT LDPC Tx, BIT4: Enable HT LDPC Rx, BIT5: Enable HT LDPC Tx */
	u8	ldpc_cap;
	/* BIT0: Enable VHT STBC Rx, BIT1: Enable VHT STBC Tx, BIT4: Enable HT STBC Rx, BIT5: Enable HT STBC Tx */
	u8	stbc_cap;
#endif

#ifdef CONFIG_80211AC_VHT
	u8	vht_enable; /* 0:disable, 1:enable, 2:auto */
	u8	ampdu_factor;
	u8  vht_rx_mcs_map[2];
#endif /* CONFIG_80211AC_VHT */

	//u8	lowrate_two_xmit;

	u8	rf_config ;
//	u8	low_power ;
	u8	power_percentage_idx;
	u8	wifi_spec;// !turbo_mode
	u8	channel_plan;
	u8	map_in_efuse;
	
#ifdef CONFIG_BT_COEXIST
	u8	btcoex;
	u8	bt_iso;
	u8	bt_sco;
	u8	bt_ampdu;
	u8	ant_num;
#endif
#if RX_AGGREGATION 
	BOOLEAN	bAcceptAddbaReq;
#endif
#ifdef CONFIG_ANTENNA_DIVERSITY
	u8	antdiv_cfg;
	u8	antdiv_type;
#endif

#ifdef CONFIG_SUPPORT_HW_WPS_PBC
	u8	hw_wps_pbc;//0:disable,1:enable
#endif

#ifdef CONFIG_ADAPTOR_INFO_CACHING_FILE
	char	adaptor_info_caching_file_path[PATH_LENGTH_MAX];
#endif

#ifdef CONFIG_LAYER2_ROAMING
	u8	max_roaming_times; // the max number driver will try to roaming
#endif

#ifdef CONFIG_IOL
	bool 	force_iol; //enable iol without other concern
#endif

#ifdef CONFIG_80211D
	u8 	enable80211d;
#endif

	u8 	ifname[16];
	u8 	if2name[16];
	
#if (RTW_NOTCH_FILTER != 0)
	u8 	notch_filter;
#endif

#ifdef CONFIG_SPECIAL_SETTING_FOR_FUNAI_TV
	u8 	force_ant;//0 normal,1 main,2 aux
	u8 	force_igi;//0 normal
#endif

	//define for tx power adjust
	u8	RegEnableTxPowerLimit;
	u8	RegEnableTxPowerByRate;
#ifdef CONFIG_RF_GAIN_OFFSET
	u8	RegEnableKFree;
#endif
	u8	RegPowerBase;
	u8	RegPwrTblSel;

	u8 adaptivity_en;
	u8 adaptivity_mode;
	u8 adaptivity_dml;
	u8 adaptivity_dc_backoff;
	s8 adaptivity_th_l2h_ini;
//	u8 nhm_en;

#ifdef RTW_HALMAC
	u8 boffefusemask;
	BOOLEAN bFileMaskEfuse;
#endif

#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C)
	s8	TxBBSwing_2G;
	s8	TxBBSwing_5G;
	u8	AmplifierType_2G;
	u8	AmplifierType_5G;
	u8	RFE_Type;
#endif

#if defined(CONFIG_RTL8192E)
	s8	TxBBSwing_2G;
	u8	AmplifierType_2G;
	u8	RFE_Type;
	u8	bEn_RFE;
	u8	PowerTracking_Type;
	u8	GLNA_Type;
#endif

#ifdef CONFIG_DFS_MASTER
	u8 dfs_region_domain;
#endif

#ifdef CONFIG_MCC_MODE
	u8 en_mcc;
	u32 rtw_mcc_single_tx_cri;
	u32 rtw_mcc_ap_bw20_target_tx_tp;
#if !defined(NOT_SUPPORT_40M)
	u32 rtw_mcc_ap_bw40_target_tx_tp;
#endif
#if !defined(NOT_SUPPORT_80M)
	u32 rtw_mcc_ap_bw80_target_tx_tp;
#endif
	u32 rtw_mcc_sta_bw20_target_tx_tp;
#if !defined(NOT_SUPPORT_40M)
	u32 rtw_mcc_sta_bw40_target_tx_tp;
#endif
#if !defined(NOT_SUPPORT_80M)
	u32 rtw_mcc_sta_bw80_target_tx_tp;
#endif
	s8 rtw_mcc_policy_table_idx;
	u8 rtw_mcc_duration;
	u8 rtw_mcc_tsf_sync_offset;
	u8 rtw_mcc_start_time_offset;
	u8 rtw_mcc_interval;
	s8 rtw_mcc_guard_offset0;
	s8 rtw_mcc_guard_offset1;
	u8 rtw_mcc_pattern_table_idx;
	u8 rtw_mcc_enable_rmcc;
#endif
	//trp tis certification
	u8 trp_tis_cert;
#ifdef TX_SHORTCUT
	u8 disable_txsc;
#endif
};

//For registry parameters
#define RGTRY_OFT(field) ((u32)FIELD_OFFSET(struct registry_priv,field))
#define RGTRY_SZ(field)   sizeof(((struct registry_priv*) 0)->field)
#define BSSID_OFT(field) ((u32)FIELD_OFFSET(WLAN_BSSID_EX,field))
#define BSSID_SZ(field)   sizeof(((PWLAN_BSSID_EX) 0)->field)

#define BW_MODE_2G(bw_mode) ((bw_mode) & 0x0F)
#define BW_MODE_5G(bw_mode) ((bw_mode) >> 4)
#ifdef CONFIG_80211N_HT
#define REGSTY_BW_2G(regsty) BW_MODE_2G((regsty)->bw_mode)
#define REGSTY_BW_5G(regsty) BW_MODE_5G((regsty)->bw_mode)
#else
#define REGSTY_BW_2G(regsty) CHANNEL_WIDTH_20
#define REGSTY_BW_5G(regsty) CHANNEL_WIDTH_20
#endif
#define REGSTY_IS_BW_2G_SUPPORT(regsty, bw) (REGSTY_BW_2G((regsty)) >= (bw))
#define REGSTY_IS_BW_5G_SUPPORT(regsty, bw) (REGSTY_BW_5G((regsty)) >= (bw))

#define REGSTY_IS_11AC_ENABLE(regsty) ((regsty)->vht_enable != 0)
#define REGSTY_IS_11AC_AUTO(regsty) ((regsty)->vht_enable == 2)

#define MAX_CONTINUAL_URB_ERR 4

#ifdef CONFIG_CONCURRENT_MODE
#define is_primary_adapter(adapter) (adapter->adapter_type == PRIMARY_ADAPTER)
#define get_iface_type(adapter) (adapter->iface_type)
#define get_hw_port(adapter) (adapter->hw_port)
#else
#define is_primary_adapter(adapter) (1)
#define get_iface_type(adapter) (IFACE_PORT0)
#define get_hw_port(adapter) (HW_PORT0)
#endif

enum _IFACE_ID {
	IFACE_ID0, /*PRIMARY_ADAPTER*/
	IFACE_ID1,
	IFACE_ID2,
	IFACE_ID3,
	IFACE_ID4,
	IFACE_ID5,
	IFACE_ID6,
	IFACE_ID7,
	IFACE_ID_MAX,
};

enum _IFACE_TYPE {
	IFACE_PORT0, //mapping to port0 for C/D series chips
	IFACE_PORT1, //mapping to port1 for C/D series chip
	MAX_IFACE_PORT,
};

enum _hw_port {
	HW_PORT0,
	HW_PORT1,
	HW_PORT2,
	HW_PORT3,
	HW_PORT4,
	MAX_HW_PORT,
};

_adapter *dvobj_get_port0_adapter(struct dvobj_priv *dvobj);

enum _ADAPTER_TYPE {
	PRIMARY_ADAPTER,
	SECONDARY_ADAPTER,
	MAX_ADAPTER,
};

struct rf_ctl_t {
	const struct country_chplan *country_ent;
	u8 ChannelPlan;
	u8 max_chan_nums;
	//RT_CHANNEL_INFO channel_set[MAX_CHANNEL_NUM];
	//struct p2p_channels channel_list;

	_mutex offch_mutex;
	u8 offch_state;

	/* used for debug or by tx power limit */
	u16 rate_bmp_cck_ofdm;		/* 20MHz */
	u32 rate_bmp_ht_by_bw[2];	/* 20MHz, 40MHz. 4SS supported */
	u32 rate_bmp_vht_by_bw[4];	/* 20MHz, 40MHz, 80MHz, 160MHz. up to 3SS supported */

	/* used by tx power limit */
	u8 highest_ht_rate_bw_bmp;
	u8 highest_vht_rate_bw_bmp;

#ifdef CONFIG_TXPWR_LIMIT
	_mutex txpwr_lmt_mutex;
	_list reg_exc_list;
	u8 regd_exc_num;
	_list txpwr_lmt_list;
	u8 txpwr_regd_num;
	const char *regd_name;

	u8 txpwr_lmt_2g_cck_ofdm_state;
	#ifdef CONFIG_IEEE80211_BAND_5GHZ
	u8 txpwr_lmt_5g_cck_ofdm_state;
	u8 txpwr_lmt_5g_20_40_ref;
	#endif
#endif

	u8 ch_sel_same_band_prefer;

#ifdef CONFIG_DFS
	u8 csa_ch;

#ifdef CONFIG_DFS_MASTER
	_timer radar_detect_timer;
	bool radar_detect_by_others;
	u8 radar_detect_enabled;
	bool radar_detected;

	u8 radar_detect_ch;
	u8 radar_detect_bw;
	u8 radar_detect_offset;

	systime cac_start_time;
	systime cac_end_time;
	u8 cac_force_stop;
	
#ifdef CONFIG_DFS_SLAVE_WITH_RADAR_DETECT
	u8 dfs_slave_with_rd;
#endif
	u8 dfs_ch_sel_d_flags;

	u8 dbg_dfs_master_fake_radar_detect_cnt;
	u8 dbg_dfs_master_radar_detect_trigger_non;
	u8 dbg_dfs_master_choose_dfs_ch_first;
#endif
#endif
};

#define RTW_CAC_STOPPED 0
#ifdef CONFIG_DFS_MASTER
#define IS_CAC_STOPPED(rfctl) ((rfctl)->cac_end_time == RTW_CAC_STOPPED)
#define IS_CH_WAITING(rfctl) (!IS_CAC_STOPPED(rfctl) && time_after((rfctl)->cac_end_time, rtw_get_current_time()))
#define IS_UNDER_CAC(rfctl) (IS_CH_WAITING(rfctl) && time_after(rtw_get_current_time(), (rfctl)->cac_start_time))
#define IS_RADAR_DETECTED(rfctl) ((rfctl)->radar_detected)
#else
#define IS_CAC_STOPPED(rfctl) 1
#define IS_CH_WAITING(rfctl) 0
#define IS_UNDER_CAC(rfctl) 0
#define IS_RADAR_DETECTED(rfctl) 0
#endif /* CONFIG_DFS_MASTER */


#ifdef CONFIG_DFS_SLAVE_WITH_RADAR_DETECT
#define IS_DFS_SLAVE_WITH_RD(rfctl) ((rfctl)->dfs_slave_with_rd)
#else
#define IS_DFS_SLAVE_WITH_RD(rfctl) 0
#endif

#ifdef RTW_HALMAC
struct halmac_indicator {
	struct submit_ctx *sctx;
	u8 *buffer;
	u32 buf_size;
	u32 ret_size;
	u32 status;
};

struct halmacpriv {
	/* flags */

	/* For asynchronous functions */
	struct halmac_indicator *indicator;

#ifdef CONFIG_SDIO_HCI
	/* Store hardware tx queue page number setting */
	u16 txpage[HW_QUEUE_ENTRY];
#endif /* CONFIG_SDIO_HCI */
};
#endif 

#if 0
struct rtw_traffic_statistics {
	/* tx statistics */
	u64	tx_bytes;
	u64	tx_pkts;
	u64	tx_drop;
	u64	cur_tx_bytes;
	u64	last_tx_bytes;
	u32	cur_tx_tp; /* Tx throughput in MBps. */

	/* rx statistics */
	u64	rx_bytes;
	u64	rx_pkts;
	u64	rx_drop;
	u64	cur_rx_bytes;
	u64	last_rx_bytes;
	u32	cur_rx_tp; /* Rx throughput in MBps. */
};
#endif

#define SEC_CAP_CHK_BMC	BIT0

#define RTW_DEFAULT_MGMT_MACID 1

struct macid_bmp {
	u32 m0;
#if (MACID_NUM_SW_LIMIT > 32)
	u32 m1;
#endif
#if (MACID_NUM_SW_LIMIT > 64)
	u32 m2;
#endif
#if (MACID_NUM_SW_LIMIT > 96)
	u32 m3;
#endif
};

struct macid_ctl_t {
	_lock lock;
	u8 num;
	struct macid_bmp used;
	struct macid_bmp bmc;
	struct macid_bmp if_g[CONFIG_IFACE_NUMBER];
	struct macid_bmp ch_g[2]; /* 2 ch concurrency */

	u8 iface_bmc[CONFIG_IFACE_NUMBER]; /* bmc TX macid for each iface*/

	u8 h2c_msr[MACID_NUM_SW_LIMIT];
	u8 bw[MACID_NUM_SW_LIMIT];
	u8 vht_en[MACID_NUM_SW_LIMIT];
	u32 rate_bmp0[MACID_NUM_SW_LIMIT];
	u32 rate_bmp1[MACID_NUM_SW_LIMIT];

	struct sta_info *sta[MACID_NUM_SW_LIMIT]; /* corresponding stainfo when macid is not shared */

	/* macid sleep registers */
	u16 reg_sleep_m0;
#if (MACID_NUM_SW_LIMIT > 32)
	u16 reg_sleep_m1;
#endif
#if (MACID_NUM_SW_LIMIT > 64)
	u16 reg_sleep_m2;
#endif
#if (MACID_NUM_SW_LIMIT > 96)
	u16 reg_sleep_m3;
#endif
};

struct dvobj_priv
{
	void *if1;
#ifdef CONFIG_CONCURRENT_MODE	
	void *if2;
#endif
	
	unsigned char	oper_channel; /* saved channel info when call set_channel_bw */
	unsigned char	oper_bwmode;
	unsigned char	oper_ch_offset;/* PRIME_CHNL_OFFSET */
	systime on_oper_ch_time;

	//For 92D, DMDP have 2 interface.
	//u8	InterfaceNumber;
	//u8	NumInterfaces;
	void *padapters[MAX_IFACE_PORT];
	u8 iface_nums; // total number of ifaces used runtime

	struct mi_state iface_state;

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8195A) || defined(CONFIG_RTL8711B) 
	struct macid_ctl_t macid_ctl;
#endif
//#ifdef CONFIG_80211AC_VHT
	struct rf_ctl_t rf_ctl;
//#endif

	//In /Out Pipe information
	//int	RtInPipe[2];
	u8	RtOutPipe[3];//int	RtOutPipe[3];
	u8	Queue2Pipe[HW_QUEUE_ENTRY];//for out pipe mapping

	//u8	irq_alloc;
	
#ifdef RTW_HALMAC
	void *halmac;
	struct halmacpriv hmpriv;
#endif 
/*-------- below is for SDIO INTERFACE --------*/
#if defined(CONFIG_SDIO_HCI) || defined(CONFIG_GSPI_HCI)
	//
	// SDIO ISR Related
	//
	u32 sdio_himr;

	//
	// SDIO Tx FIFO related.
	//
	// HIQ, MID, LOW, PUB free pages; padapter->xmitpriv.free_txpg
	u8  SdioTxFIFOFreePage[TX_FREE_PG_QUEUE];
	_lock SdioTxFIFOFreePageLock;

	//
	// SDIO Rx FIFO related.
	//
	u16 SdioRxFIFOSize;

#ifdef INTF_DATA
	INTF_DATA intf_data;
#endif
#ifdef INTF_OPS
	INTF_OPS intf_ops;
#endif

#endif //CONFIG_SDIO_HCI

#if 0
	struct rtw_traffic_statistics	traffic_stat;
#endif


/*-------- below is for PCIE INTERFACE --------*/

#ifdef CONFIG_LX_HCI
	u8	irq_alloc; 
#if defined(PLATFORM_FREERTOS) || defined (PLATFORM_CMSIS_RTOS) || defined(PLATFORM_ALIOS)
	u8	irq_enabled;
	_lock	irq_th_lock;
#endif //PLATFORM_FREERTOS
#endif

#ifdef CONFIG_AXI_HCI
	u8	irq_alloc; 
#if defined(PLATFORM_FREERTOS) || defined (PLATFORM_CMSIS_RTOS) || defined(PLATFORM_ALIOS)
	u8	irq_enabled;
	_lock	irq_th_lock;
#endif //PLATFORM_FREERTOS

#endif

#ifdef CONFIG_MCC_MODE
	struct mcc_obj_priv	mcc_objpriv;
	//unsigned char		oper_channel;	/* saved channel info when call set_channel_bw */
	//unsigned char		oper_bwmode;
	//unsigned char		oper_ch_offset;	/* PRIME_CHNL_OFFSET */
	//u32					on_oper_ch_time;
#endif
};

#define rfctl_to_dvobj(rfctl) container_of((rfctl), struct dvobj_priv, rf_ctl)

#ifdef CONFIG_CONCURRENT_MODE
struct co_data_priv{

	//george@20120518
	//current operating channel/bw/ch_offset
	//save the correct ch/bw/ch_offset whatever the inputted values are
	//when calling set_channel_bwmode() at concurrent mode
	//for debug check or reporting to layer app (such as wpa_supplicant for nl80211)
	u8 co_ch;
	u8 co_bw;
	u8 co_ch_offset;
	u8 rsvd;

};
#endif //CONFIG_CONCURRENT_MODE

typedef enum _DRIVER_STATE{
	DRIVER_NORMAL = 0,
	DRIVER_DISAPPEAR = 1,
	DRIVER_REPLACE_DONGLE = 2,
}DRIVER_STATE;

#ifdef CONFIG_INTEL_PROXIM
struct proxim {
	bool proxim_support;
	bool proxim_on;

	void *proximity_priv;
	int (*proxim_rx)(_adapter *padapter,
		union recv_frame *precv_frame);
	u8	(*proxim_get_var)(_adapter* padapter, u8 type);
};
#endif	//CONFIG_INTEL_PROXIM

#ifdef CONFIG_MAC_LOOPBACK_DRIVER
typedef struct loopbackdata
{
	_sema	sema;
	_thread_hdl_ lbkthread;
	u8 bstop;
	u32 cnt;
	u16 size;
	u16 txsize;
	u8 txbuf[0x8000];
	u16 rxsize;
	u8 rxbuf[0x8000];
	u8 msg[100];

}LOOPBACKDATA, *PLOOPBACKDATA;
#endif

#define ADAPTER_TX_BW_2G(adapter) BW_MODE_2G((adapter)->driver_tx_bw_mode)
#define ADAPTER_TX_BW_5G(adapter) BW_MODE_5G((adapter)->driver_tx_bw_mode)

struct tsf_info {
	u8 sync_port;/*tsf sync from portx*/
	u8 offset; /*tsf timer offset*/
};

struct _ADAPTER{
	u16	HardwareType;
	u16	interface_type;//USB,SDIO,SPI,PCI
	u32	work_mode; //STA, AP, STA+AP, PROMISC, P2P

	struct	dvobj_priv	*dvobj;
	struct	mlme_priv	mlmepriv;
	struct	mlme_ext_priv	mlmeextpriv;
	struct	cmd_priv	cmdpriv;
	struct	evt_priv	evtpriv;
#ifdef CONFIG_IEEE80211K
	struct	rm_priv 	*rmpriv;
#endif
	//struct	io_queue	*pio_queue;
	struct 	io_priv	iopriv;
	struct	xmit_priv	xmitpriv;
	struct	recv_priv	recvpriv;
	struct	sta_priv	stapriv;
	struct	security_priv	securitypriv;
	//_lock   security_key_mutex; /* add for CONFIG_IEEE80211W, none 11w also can use */
	struct	registry_priv	registrypriv;
	struct	pwrctrl_priv	pwrctrlpriv;
	struct 	eeprom_priv eeprompriv;
//TODO
//	struct	led_priv	ledpriv;


#ifdef CONFIG_MP_INCLUDED
       struct	mp_priv	mppriv;
#endif

#if defined(CONFIG_HOSTAPD_MLME) && defined (CONFIG_AP_MODE)
	struct	hostapd_priv	*phostapdpriv;
#endif

#ifdef CONFIG_P2P_NEW
	struct wifidirect_info	wdinfo;
#endif

#ifdef CONFIG_TDLS
	struct tdls_info	tdlsinfo;
#endif //CONFIG_TDLS

#ifdef CONFIG_WAPI_SUPPORT
	u8	WapiSupport;
	RT_WAPI_T	wapiInfo;
#endif


#ifdef CONFIG_WFD
	struct wifi_display_info wfd_info;
#endif //CONFIG_WFD

	PVOID		HalData;
	u32 		hal_data_sz;
	struct hal_ops	HalFunc;

	s32	bDriverStopped;
	s32	bSurpriseRemoved;
	s32	bCardDisableWOHSM;
	u8	RxStop;	//Used to stop rx thread as early as possible
	_sema	RxProtect_sema;	//Used to protect RX when in rtw_set_mode

	u32	IsrContent;
	u32	ImrContent;

	u8	EepromAddressSize;
	u8	hw_init_completed;
	u8	bDriverIsGoingToUnload;
	u8	init_adpt_in_progress;
	u8	bMpDriver;

#ifdef CONFIG_AP_MODE
	u8	bForwardingDisabled;
#endif

#if defined(CONFIG_EVENT_THREAD_MODE)
	_thread_hdl_	evtThread;
#endif
#if defined(CONFIG_ISR_THREAD_MODE_POLLING) || defined(CONFIG_ISR_THREAD_MODE_INTERRUPT)
	struct task_struct	isrThread;
#endif
	struct task_struct	cmdThread;
#ifdef CONFIG_XMIT_THREAD_MODE
	struct task_struct	xmitThread;
#endif
#if defined(CONFIG_RECV_THREAD_MODE)
	struct task_struct	recvThread;
#endif
#ifdef CONFIG_RECV_TASKLET_THREAD
	struct task_struct	recvtasklet_thread;
#endif
#ifdef CONFIG_XMIT_TASKLET_THREAD
	struct task_struct	xmittasklet_thread;
#endif
#ifdef CONFIG_SDIO_XMIT_THREAD
	struct task_struct	SdioXmitThread;
#endif //CONFIG_XMIT_TASKLET_THREAD

#ifdef CONFIG_RECV_TASK_THREAD_MODE
	struct task_struct	recvtaskThread;
#endif

#ifdef CONFIG_BT_COEXIST_SOC
	struct task_struct	btmailboxThread;
	_queue	btmailboxQueue;
	struct coex_priv *coexpriv;
#endif

#if !defined(PLATFORM_ECOS) && !defined(PLATFORM_FREERTOS) && !defined(PLATFORM_CMSIS_RTOS) && !defined(PLATFORM_CUSTOMER_RTOS)
	NDIS_STATUS (*dvobj_init)(struct dvobj_priv *dvobj);
	void (*dvobj_deinit)(struct dvobj_priv *dvobj);
#endif

	void (*intf_start)(_adapter * adapter);
	void (*intf_stop)(_adapter * adapter);

#ifdef PLATFORM_ECOS
	_nic_hdl pnetdev;
	int bup;
	struct net_device_stats stats;
#endif	//#ifdef PLATFORM_ECOS

#if defined(PLATFORM_FREERTOS) || defined(PLATFORM_CMSIS_RTOS) || defined(PLATFORM_CUSTOMER_RTOS)
	_nic_hdl	pnetdev;
	int		bup;
	struct net_device_stats stats;
#endif	//#ifdef PLATFORM_FREERTOS

	u8 net_closed;

	u8 bFWReady;
	//u8 bBTFWReady;
	//u8 bReadPortCancel;
	//u8 bWritePortCancel;
	u8 bLinkInfoDump;
	u8 bRxRSSIDisplay;

	_adapter *pbuddy_adapter;

	_mutex *hw_init_mutex;
	u8 iface_type; //interface port type
	_mutex *ph2c_fwcmd_mutex;

	/*extend to support multi interface*/
	u8 iface_id;

#if defined(CONFIG_CONCURRENT_MODE) 
	u8 isprimary; //is primary adapter or not
	u8 adapter_type;
	struct tsf_info tsf;

	//for global synchronization
	_mutex *psetch_mutex;
	_mutex *psetbw_mutex;

	struct co_data_priv *pcodatapriv;//data buffer shared among interfaces
#endif

#ifdef CONFIG_INTEL_PROXIM
	/* intel Proximity, should be alloc mem
	 * in intel Proximity module and can only
	 * be used in intel Proximity mode */
	struct proxim proximity;
#endif	//CONFIG_INTEL_PROXIM

#ifdef CONFIG_MAC_LOOPBACK_DRIVER
	PLOOPBACKDATA ploopback;
#endif

	u8 fix_rate;
	u8 fix_bw;
	u8 driver_tx_bw_mode;

#ifdef CONFIG_CAC_TEST
	unsigned char     in_cta_test;
#endif
	/* This flag is used to dynamically enabling debug message if
	  	certain sympton happen. Use iwpriv command to enable it */
#if defined(CONFIG_DEBUG_DYNAMIC)	
	u8	debug_level;
#endif

	// for user define DM
	u8 ra_mask_user_en;
	u32 ra_mask_define;
	u8 auto_rate_fallback_user_en;

#if defined (CONFIG_AP_MODE)
	/*
	 * If is true, driver won't process any rx packets from ap port
	*/
	u8 b_suspend_ap_rx;
#endif

#ifdef CONFIG_MCC_MODE
	struct mcc_adapter_priv mcc_adapterpriv;
	/* notes:
	**	if isprimary is true, the adapter_type value is 0, iface_id is IFACE_ID0 for PRIMARY_ADAPTER
	**	if isprimary is false, the adapter_type value is 1, iface_id is IFACE_ID1 for VIRTUAL_ADAPTER
	**	refer to iface_id if iface_nums>2 and isprimary is false and the adapter_type value is 0xff.*/
	u8 hw_port; /*interface port type, it depends on HW port */

	/*extend to support multi interface*/
	/*IFACE_ID0 is equals to PRIMARY_ADAPTER
	IFACE_ID1 is equals to VIRTUAL_ADAPTER*/
	//u8 iface_id;
#endif
};

#define dvobj_to_macidctl(dvobj) (&(dvobj->macid_ctl))
#define dvobj_to_rfctl(dvobj) (&(dvobj->rf_ctl))
#define dvobj_get_primary_adapter(dvobj)	((_adapter*)((dvobj)->if1))
#define dvobj_to_regsty(dvobj) (&(((_adapter*)(dvobj->if1))->registrypriv))

#define adapter_to_dvobj(adapter) (adapter->dvobj)
#define adapter_to_regsty(adapter) dvobj_to_regsty(adapter_to_dvobj((adapter)))
#define adapter_to_pwrctl(adapter) (&adapter->pwrctrlpriv)
#define adapter_to_rfctl(adapter) dvobj_to_rfctl(adapter_to_dvobj((adapter)))
#define adapter_to_macidctl(adapter) dvobj_to_macidctl(adapter_to_dvobj((adapter)))
#define adapter_mac_addr(adapter) (adapter->eeprompriv.mac_addr)
#define adapter_to_chset(adapter) (adapter->mlmeextpriv.channel_set)

#define adapter_to_phydm(adapter) (&(GET_HAL_DATA(adapter)->odmpriv))
#define dvobj_to_phydm(dvobj) adapter_to_phydm(dvobj_get_primary_adapter(dvobj))

#define rtw_get_intf_type(adapter) (((PADAPTER)adapter)->interface_type)

__inline static u8 *myid(struct eeprom_priv *peepriv)
{
	return (peepriv->mac_addr);
}

#if 0 //#if (CONFIG_LWIP_LAYER == 0)
// For FPGA test program
#define _htons(x) (x)
#define _htons(x) (x)
#define _htons(x) (x)
#define _htons(x) (x)
#endif

//fast reconnection function prototype
typedef int (*init_done_ptr)(void);
#endif //__DRV_TYPES_H__

