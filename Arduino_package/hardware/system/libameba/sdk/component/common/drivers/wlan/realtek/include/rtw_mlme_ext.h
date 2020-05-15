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
#ifndef __RTW_MLME_EXT_H_
#define __RTW_MLME_EXT_H_


//	Commented by Albert 20101105
//	Increase the SURVEY_TO value from 100 to 150  ( 100ms to 150ms )
//	The Realtek 8188CE SoftAP will spend around 100ms to send the probe response after receiving the probe request.
//	So, this driver tried to extend the dwell time for each scanning channel.
//	This will increase the chance to receive the probe response from SoftAP.

#if CONFIG_AUTO_RECONNECT

#endif

//TODO
#define FAST_SURVEY_TO	(25) //Fast connection time, scan only partial channel
#define SURVEY_TO		(100) //Reduce connection time
#define PASSIVE_SURVEY_TO		(110) //Passive scan should > 102.4ms
//#define SURVEY_TO		(300) //Increase time to stay each channel - Alex Fang
#define REAUTH_TO		(800) //(50)
#define REASSOC_TO		(300) //(50)
//#define DISCONNECT_TO	(3000)
#define ADDBA_TO			(2000)

#define LINKED_TO (1) //unit:2 sec, 1x2=2 sec

#define REAUTH_LIMIT	(4)
#define REASSOC_LIMIT	(4)
#define READDBA_LIMIT	(2)

#if (defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI)
	#define ROAMING_LIMIT	5
#else
	#define ROAMING_LIMIT	8
#endif
//#define	IOCMD_REG0		0x10250370		 	
//#define	IOCMD_REG1		0x10250374
//#define	IOCMD_REG2		0x10250378

//#define	FW_DYNAMIC_FUN_SWITCH	0x10250364

//#define	WRITE_BB_CMD		0xF0000001
//#define	SET_CHANNEL_CMD	0xF3000000
//#define	UPDATE_RA_CMD	0xFD0000A2

#define _HW_STATE_NOLINK_		0x00
#define _HW_STATE_ADHOC_		0x01
#define _HW_STATE_STATION_ 	0x02
#define _HW_STATE_AP_			0x03
#define _HW_STATE_MONITOR_ 	0x04


#define		_1M_RATE_	0
#define		_2M_RATE_	1
#define		_5M_RATE_	2
#define		_11M_RATE_	3
#define		_6M_RATE_	4
#define		_9M_RATE_	5
#define		_12M_RATE_	6
#define		_18M_RATE_	7
#define		_24M_RATE_	8
#define		_36M_RATE_	9
#define		_48M_RATE_	10
#define		_54M_RATE_	11

#define MAX_COUNTRY_NUM 250

extern const u8 WMM_OUI[];
extern const u8 WPS_OUI[];
extern const u8 WFD_OUI[];
extern const u8 P2P_OUI[];

//extern const unsigned char WMM_INFO_OUI[];
extern const u8	WMM_PARA_OUI[];


//
// Channel Plan Type.
// Note: 
//	We just add new channel plan when the new channel plan is different from any of the following 
//	channel plan. 
//	If you just wnat to customize the acitions(scan period or join actions) about one of the channel plan,
//	customize them in RT_CHANNEL_INFO in the RT_CHANNEL_LIST.
// 
typedef enum _RT_CHANNEL_DOMAIN
{
	//===== old channel plan mapping =====//
	RT_CHANNEL_DOMAIN_FCC = 0x00,
	RT_CHANNEL_DOMAIN_IC = 0x01,
	RT_CHANNEL_DOMAIN_ETSI = 0x02,
	RT_CHANNEL_DOMAIN_SPAIN = 0x03,
	RT_CHANNEL_DOMAIN_FRANCE = 0x04,
	RT_CHANNEL_DOMAIN_MKK = 0x05,
	RT_CHANNEL_DOMAIN_MKK1 = 0x06,
	RT_CHANNEL_DOMAIN_ISRAEL = 0x07,
	RT_CHANNEL_DOMAIN_TELEC = 0x08,
	RT_CHANNEL_DOMAIN_GLOBAL_DOAMIN = 0x09,
	RT_CHANNEL_DOMAIN_WORLD_WIDE_13 = 0x0A,
	RT_CHANNEL_DOMAIN_TAIWAN = 0x0B,
	RT_CHANNEL_DOMAIN_CHINA = 0x0C,
	RT_CHANNEL_DOMAIN_SINGAPORE_INDIA_MEXICO = 0x0D,
	RT_CHANNEL_DOMAIN_KOREA = 0x0E,
	RT_CHANNEL_DOMAIN_TURKEY = 0x0F,
	RT_CHANNEL_DOMAIN_JAPAN = 0x10,
	RT_CHANNEL_DOMAIN_FCC_NO_DFS = 0x11,
	RT_CHANNEL_DOMAIN_JAPAN_NO_DFS = 0x12,
	RT_CHANNEL_DOMAIN_WORLD_WIDE_5G = 0x13,
	RT_CHANNEL_DOMAIN_TAIWAN_NO_DFS = 0x14,

	//===== new channel plan mapping, (2GDOMAIN_5GDOMAIN) =====//
	RT_CHANNEL_DOMAIN_WORLD_NULL = 0x20,
	RT_CHANNEL_DOMAIN_ETSI1_NULL = 0x21,
	RT_CHANNEL_DOMAIN_FCC1_NULL = 0x22,
	RT_CHANNEL_DOMAIN_MKK1_NULL = 0x23,
	RT_CHANNEL_DOMAIN_ETSI2_NULL = 0x24,
	RT_CHANNEL_DOMAIN_FCC1_FCC1 = 0x25,
	RT_CHANNEL_DOMAIN_WORLD_ETSI1 = 0x26,
	RT_CHANNEL_DOMAIN_MKK1_MKK1 = 0x27,
	RT_CHANNEL_DOMAIN_WORLD_KCC1 = 0x28,
	RT_CHANNEL_DOMAIN_WORLD_FCC2 = 0x29,
	RT_CHANNEL_DOMAIN_FCC2_NULL = 0x2A,
	RT_CHANNEL_DOMAIN_IC1_IC2 = 0x2B,
	RT_CHANNEL_DOMAIN_MKK2_NULL = 0x2C,
	RT_CHANNEL_DOMAIN_WORLD_CHILE1 = 0x2D,
	RT_CHANNEL_DOMAIN_WORLD1_WORLD1 = 0x2E,
	RT_CHANNEL_DOMAIN_WORLD_CHILE2 = 0x2F,
	RT_CHANNEL_DOMAIN_WORLD_FCC3 = 0x30,
	RT_CHANNEL_DOMAIN_WORLD_FCC4 = 0x31,
	RT_CHANNEL_DOMAIN_WORLD_FCC5 = 0x32,
	RT_CHANNEL_DOMAIN_WORLD_FCC6 = 0x33,
	RT_CHANNEL_DOMAIN_FCC1_FCC7 = 0x34,
	RT_CHANNEL_DOMAIN_WORLD_ETSI2 = 0x35,
	RT_CHANNEL_DOMAIN_WORLD_ETSI3 = 0x36,
	RT_CHANNEL_DOMAIN_MKK1_MKK2 = 0x37,
	RT_CHANNEL_DOMAIN_MKK1_MKK3 = 0x38,
	RT_CHANNEL_DOMAIN_FCC1_NCC1 = 0x39,
	RT_CHANNEL_DOMAIN_FCC1_NCC2 = 0x40,
	RT_CHANNEL_DOMAIN_GLOBAL_NULL = 0x41,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI4 = 0x42,
	RT_CHANNEL_DOMAIN_FCC1_FCC2 = 0x43,
	RT_CHANNEL_DOMAIN_FCC1_NCC3 = 0x44,
	RT_CHANNEL_DOMAIN_WORLD_ACMA1 = 0x45,
	RT_CHANNEL_DOMAIN_FCC1_FCC8 = 0x46,
	RT_CHANNEL_DOMAIN_WORLD_ETSI6 = 0x47,
	RT_CHANNEL_DOMAIN_WORLD_ETSI7 = 0x48,
	RT_CHANNEL_DOMAIN_WORLD_ETSI8 = 0x49,
	RT_CHANNEL_DOMAIN_WORLD_ETSI9 = 0x50,
	RT_CHANNEL_DOMAIN_WORLD_ETSI10 = 0x51,
	RT_CHANNEL_DOMAIN_WORLD_ETSI11 = 0x52,
	RT_CHANNEL_DOMAIN_FCC1_NCC4 = 0x53,
	RT_CHANNEL_DOMAIN_WORLD_ETSI12 = 0x54,
	RT_CHANNEL_DOMAIN_FCC1_FCC9 = 0x55,
	RT_CHANNEL_DOMAIN_WORLD_ETSI13 = 0x56,
	RT_CHANNEL_DOMAIN_FCC1_FCC10 = 0x57,
	RT_CHANNEL_DOMAIN_MKK2_MKK4 = 0x58,
	RT_CHANNEL_DOMAIN_WORLD_ETSI14 = 0x59,
	RT_CHANNEL_DOMAIN_FCC1_FCC5 = 0x60,
	RT_CHANNEL_DOMAIN_FCC2_FCC7 = 0x61,
	RT_CHANNEL_DOMAIN_FCC2_FCC1 = 0x62,
	RT_CHANNEL_DOMAIN_WORLD_ETSI15 = 0x63,
	RT_CHANNEL_DOMAIN_MKK2_MKK5 = 0x64,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI16 = 0x65,
	RT_CHANNEL_DOMAIN_FCC1_FCC14 = 0x66,
	RT_CHANNEL_DOMAIN_FCC1_FCC12 = 0x67,
	RT_CHANNEL_DOMAIN_FCC2_FCC14 = 0x68,
	RT_CHANNEL_DOMAIN_FCC2_FCC12 = 0x69,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI17 = 0x6A,
	RT_CHANNEL_DOMAIN_WORLD_FCC16 = 0x6B,
	RT_CHANNEL_DOMAIN_WORLD_FCC13 = 0x6C,
	RT_CHANNEL_DOMAIN_FCC2_FCC15 = 0x6D,
	RT_CHANNEL_DOMAIN_WORLD_FCC12 = 0x6E,
	RT_CHANNEL_DOMAIN_NULL_ETSI8 = 0x6F,
	RT_CHANNEL_DOMAIN_NULL_ETSI18 = 0x70,
	RT_CHANNEL_DOMAIN_NULL_ETSI17 = 0x71,
	RT_CHANNEL_DOMAIN_NULL_ETSI19 = 0x72,
	RT_CHANNEL_DOMAIN_WORLD_FCC7 = 0x73,
	RT_CHANNEL_DOMAIN_FCC2_FCC17 = 0x74,
	RT_CHANNEL_DOMAIN_WORLD_ETSI20 = 0x75,
	RT_CHANNEL_DOMAIN_FCC2_FCC11 = 0x76,
	RT_CHANNEL_DOMAIN_WORLD_ETSI21 = 0x77,
	RT_CHANNEL_DOMAIN_FCC1_FCC18 = 0x78,
	RT_CHANNEL_DOMAIN_MKK2_MKK1 = 0x79,

	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_MAX,
	RT_CHANNEL_DOMAIN_REALTEK_DEFINE = 0x7F
}RT_CHANNEL_DOMAIN, *PRT_CHANNEL_DOMAIN;

typedef enum _RT_CHANNEL_DOMAIN_2G
{
	RTW_RD_2G_NULL = 0,
	RTW_RD_2G_WORLD = 1,	/* Worldwird 13 */
	RTW_RD_2G_ETSI1 = 2,	/* Europe */
	RTW_RD_2G_FCC1 = 3,		/* US */
	RTW_RD_2G_MKK1 = 4,		/* Japan */
	RTW_RD_2G_ETSI2 = 5,	/* France */
	RTW_RD_2G_GLOBAL = 6,	/* Global domain */
	RTW_RD_2G_MKK2 = 7,		/* Japan */
	RTW_RD_2G_FCC2 = 8,		/* US */
	RTW_RD_2G_IC1 = 9,		/* Canada */
	RTW_RD_2G_WORLD1 = 10,	/* Worldwide 11 */
	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_2G_MAX,
}RT_CHANNEL_DOMAIN_2G, *PRT_CHANNEL_DOMAIN_2G;

typedef enum _RT_CHANNEL_DOMAIN_5G
{
	RTW_RD_5G_NULL = 0,
	RTW_RD_5G_ETSI1 = 1,	/* Europe */
	RTW_RD_5G_ETSI2 = 2,	/* Australia, New Zealand */
	RTW_RD_5G_ETSI3 = 3,	/* Russia */
	RTW_RD_5G_FCC1 = 4,		/* US */
	RTW_RD_5G_FCC2 = 5,		/* FCC w/o DFS Channels */
	RTW_RD_5G_FCC3 = 6,		/* Bolivia, Chile, El Salvador, Venezuela */
	RTW_RD_5G_FCC4 = 7,		/* Venezuela */
	RTW_RD_5G_FCC5 = 8,		/* China */
	RTW_RD_5G_FCC6 = 9,		/*	*/
	RTW_RD_5G_FCC7 = 10,	/* US(w/o Weather radar) */
	RTW_RD_5G_IC1 = 11,		/* Canada(w/o Weather radar) */
	RTW_RD_5G_KCC1 = 12,	/* Korea */
	RTW_RD_5G_MKK1 = 13,	/* Japan */
	RTW_RD_5G_MKK2 = 14,	/* Japan (W52, W53) */
	RTW_RD_5G_MKK3 = 15,	/* Japan (W56) */
	RTW_RD_5G_NCC1 = 16,	/* Taiwan, (w/o Weather radar) */
	RTW_RD_5G_NCC2 = 17,	/* Taiwan, Band2, Band4 */
	RTW_RD_5G_NCC3 = 18,	/* Taiwan w/o DFS, Band4 only */
	RTW_RD_5G_ETSI4 = 19,	/* Europe w/o DFS, Band1 only */
	RTW_RD_5G_ETSI5 = 20,	/* Australia, New Zealand(w/o Weather radar) */
	RTW_RD_5G_FCC8 = 21,	/* Latin America */
	RTW_RD_5G_ETSI6 = 22,	/* Israel, Bahrain, Egypt, India, China, Malaysia */
	RTW_RD_5G_ETSI7 = 23,	/* China */
	RTW_RD_5G_ETSI8 = 24,	/* Jordan */
	RTW_RD_5G_ETSI9 = 25,	/* Lebanon */
	RTW_RD_5G_ETSI10 = 26,	/* Qatar */
	RTW_RD_5G_ETSI11 = 27,	/* Russia */
	RTW_RD_5G_NCC4 = 28,	/* Taiwan, (w/o Weather radar) */
	RTW_RD_5G_ETSI12 = 29,	/* Indonesia */
	RTW_RD_5G_FCC9 = 30,	/* (w/o Weather radar) */
	RTW_RD_5G_ETSI13 = 31,	/* (w/o Weather radar) */
	RTW_RD_5G_FCC10 = 32,	/* Argentina(w/o Weather radar) */
	RTW_RD_5G_MKK4 = 33,	/* Japan (W52) */
	RTW_RD_5G_ETSI14 = 34,	/* Russia */
	RTW_RD_5G_FCC11 = 35,	/* US(include CH144) */
	RTW_RD_5G_ETSI15 = 36,	/* Malaysia */
	RTW_RD_5G_MKK5 = 37,	/* Japan */
	RTW_RD_5G_ETSI16 = 38,	/* Europe */
	RTW_RD_5G_ETSI17 = 39,	/* Europe */
	RTW_RD_5G_FCC12 = 40,	/* FCC */
	RTW_RD_5G_FCC13 = 41,	/* FCC */
	RTW_RD_5G_FCC14 = 42,	/* FCC w/o Weather radar(w/o 5600~5650MHz) */
	RTW_RD_5G_FCC15 = 43,	/* FCC w/o Band3 */
	RTW_RD_5G_FCC16 = 44,	/* FCC w/o Band3 */
	RTW_RD_5G_ETSI18 = 45,	/* ETSI w/o DFS Band2&3 */
	RTW_RD_5G_ETSI19 = 46,	/* Europe */
	RTW_RD_5G_FCC17 = 47,	/* FCC w/o Weather radar(w/o 5600~5650MHz) */
	RTW_RD_5G_ETSI20 = 48,	/* Europe */
	RTW_RD_5G_IC2 = 49,		/* Canada(w/o Weather radar), include ch144 */
	RTW_RD_5G_ETSI21 = 50,	/* Australia, New Zealand(w/o Weather radar) */
	RTW_RD_5G_FCC18 = 51,	/*  */
	RTW_RD_5G_WORLD = 52,	/* Worldwide */
	RTW_RD_5G_CHILE1 = 53,	/* Chile */
	RTW_RD_5G_ACMA1 = 54,	/* Australia, New Zealand (w/o Weather radar) (w/o Ch120~Ch128) */
	RTW_RD_5G_WORLD1 = 55,	/* 5G Worldwide Band1&2 */
	RTW_RD_5G_CHILE2 = 56,	/* Chile (Band2,Band3) */

	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_5G_MAX
}RT_CHANNEL_DOMAIN_5G, *PRT_CHANNEL_DOMAIN_5G;

#define rtw_is_channel_plan_valid(chplan) (chplan<RT_CHANNEL_DOMAIN_MAX || chplan == RT_CHANNEL_DOMAIN_REALTEK_DEFINE)

typedef struct _RT_CHANNEL_PLAN
{
	unsigned char	Channel[MAX_CHANNEL_NUM];
	unsigned char	Len;
}RT_CHANNEL_PLAN, *PRT_CHANNEL_PLAN;

typedef struct _RT_CHANNEL_PLAN_2G
{
	unsigned char	Channel[MAX_CHANNEL_NUM_2G];
	unsigned char	Len;
}RT_CHANNEL_PLAN_2G, *PRT_CHANNEL_PLAN_2G;

typedef struct _RT_CHANNEL_PLAN_5G
{
	unsigned char	Channel[MAX_CHANNEL_NUM_5G];
	unsigned char	Len;
}RT_CHANNEL_PLAN_5G, *PRT_CHANNEL_PLAN_5G;

typedef struct _RT_CHANNEL_PLAN_MAP
{
	unsigned char	ChannelPlan;
	unsigned char	Index2G;
#if !defined(NOT_SUPPORT_5G)	
	unsigned char	Index5G;
#endif
	unsigned char	PwrLmt2G; /* value of REGULATION_TXPWR_LMT */
#if !defined(NOT_SUPPORT_5G)
	unsigned char	PwrLmt5G;
#endif
}RT_CHANNEL_PLAN_MAP, *PRT_CHANNEL_PLAN_MAP;


typedef struct _RT_CHANNEL_PLAN_COUNTRY_MAP
{
	char *country_code;
	RT_CHANNEL_DOMAIN domain;

}RT_CHANNEL_PLAN_COUNTRY_MAP, *PRT_CHANNEL_PLAN_COUNTRY_MAP;



enum Associated_AP
{
	atherosAP	= 0,
	broadcomAP	= 1,
	ciscoAP		= 2,
	marvellAP	= 3,
	ralinkAP	= 4,
	realtekAP	= 5,
	airgocapAP 	= 6,
	unknownAP	= 7,
	maxAP,
};

typedef enum _HT_IOT_PEER
{
	HT_IOT_PEER_UNKNOWN 			= 0,
	HT_IOT_PEER_REALTEK 			= 1,
	HT_IOT_PEER_REALTEK_92SE 		= 2,
	HT_IOT_PEER_BROADCOM 			= 3,
	HT_IOT_PEER_RALINK 			= 4,
	HT_IOT_PEER_ATHEROS 			= 5,
	HT_IOT_PEER_CISCO 			= 6,
	HT_IOT_PEER_MERU 			= 7,	
	HT_IOT_PEER_MARVELL 			= 8,
	HT_IOT_PEER_REALTEK_SOFTAP 		= 9,// peer is RealTek SOFT_AP, by Bohn, 2009.12.17
	HT_IOT_PEER_SELF_SOFTAP 		= 10, // Self is SoftAP
	HT_IOT_PEER_AIRGO 			= 11,
	HT_IOT_PEER_INTEL 			= 12, 
	HT_IOT_PEER_RTK_APCLIENT 		= 13, 
	HT_IOT_PEER_REALTEK_81XX 		= 14,	
	HT_IOT_PEER_REALTEK_WOW 		= 15,	
	HT_IOT_PEER_REALTEK_JAGUAR_BCUTAP = 16,
	HT_IOT_PEER_REALTEK_JAGUAR_CCUTAP = 17,	
	HT_IOT_PEER_CMW500 = 18,
	HT_IOT_PEER_MAX 				= 19
}HT_IOT_PEER_E, *PHTIOT_PEER_E;


enum SCAN_STATE
{
	SCAN_DISABLE = 0,
	SCAN_START = 1,
	SCAN_TXNULL = 2,
	SCAN_PROCESS = 3,
	SCAN_COMPLETE = 4,
	/* backop */
	SCAN_BACKING_OP = 5,
	SCAN_BACK_OP = 6,
	SCAN_LEAVING_OP = 7,
	SCAN_LEAVE_OP = 8,
	SCAN_STATE_MAX,
};

struct mlme_handler {
	unsigned int   num;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};

struct action_handler {
	unsigned int   num;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};

struct	ss_res	
{
	int	state;
	int	bss_cnt;
	int	channel_idx;
	int	scan_mode;
	NDIS_802_11_SSID ssid[RTW_SSID_SCAN_AMOUNT];
};

//#define AP_MODE	0x0C
//#define STATION_MODE	0x08
//#define AD_HOC_MODE	0x04
//#define NO_LINK_MODE	0x00

#define WIFI_FW_NULL_STATE		_HW_STATE_NOLINK_
#define	WIFI_FW_STATION_STATE		_HW_STATE_STATION_
#define	WIFI_FW_AP_STATE		_HW_STATE_AP_
#define	WIFI_FW_ADHOC_STATE		_HW_STATE_ADHOC_

#define	WIFI_FW_AUTH_NULL		0x00000100
#define	WIFI_FW_AUTH_STATE		0x00000200
#define	WIFI_FW_AUTH_SUCCESS		0x00000400

#define	WIFI_FW_ASSOC_STATE		0x00002000
#define	WIFI_FW_ASSOC_SUCCESS		0x00004000

#define	WIFI_FW_LINKING_STATE		(WIFI_FW_AUTH_NULL | WIFI_FW_AUTH_STATE | WIFI_FW_AUTH_SUCCESS |WIFI_FW_ASSOC_STATE)

#ifdef CONFIG_TDLS
// 1: Write RCR DATA BIT
// 2: Issue peer traffic indication
// 3: Go back to the channel linked with AP, terminating channel switch procedure
// 4: Init channel sensing, receive all data and mgnt frame
// 5: Channel sensing and report candidate channel
// 6: First time set channel to off channel
// 7: Go back tp the channel linked with AP when set base channel as target channel
// 8: Set channel back to base channel
// 9: Set channel back to off channel
// 10: Restore RCR DATA BIT
// 11: Check alive
// 12: Check alive
// 13: Free TDLS sta
enum TDLS_option
{
	TDLS_WRCR		= 	1,
	TDLS_SD_PTI		= 	2,
	TDLS_CS_OFF		= 	3,
	TDLS_INIT_CH_SEN	= 	4,
	TDLS_DONE_CH_SEN	=	5,
	TDLS_OFF_CH		=	6,
	TDLS_BASE_CH 		=	7,
	TDLS_P_OFF_CH		=	8,
	TDLS_P_BASE_CH		= 	9,
	TDLS_RS_RCR		=	10,
	TDLS_CKALV_PH1		=	11,
	TDLS_CKALV_PH2		=	12,
	TDLS_FREE_STA		=	13,
	maxTDLS,
};

#endif //CONFIG_TDLS

struct FW_Sta_Info
{
	struct sta_info	*psta;
	u32	status;
	u32	rx_pkt;
	u32	retry;
	NDIS_802_11_RATES_EX  SupportedRates;
};

/*
 * Usage:
 * When one iface acted as AP mode and the other iface is STA mode and scanning, 
 * it should switch back to AP's operating channel periodically.
 * Parameters info:
 * When the driver scanned RTW_SCAN_NUM_OF_CH channels, it would switch back to AP's operating channel for
 * RTW_STAY_AP_CH_MILLISECOND * SURVEY_TO milliseconds.
 * Example:
 * For chip supports 2.4G + 5GHz and AP mode is operating in channel 1, 
 * RTW_SCAN_NUM_OF_CH is 8, RTW_STAY_AP_CH_MILLISECOND is 3 and SURVEY_TO is 100.
 * When it's STA mode gets set_scan command, 
 * it would 
 * 1. Doing the scan on channel 1.2.3.4.5.6.7.8 
 * 2. Back to channel 1 for 300 milliseconds
 * 3. Go through doing site survey on channel 9.10.11.36.40.44.48.52
 * 4. Back to channel 1 for 300 milliseconds
 * 5. ... and so on, till survey done.
 */
#if defined CONFIG_STA_MODE_SCAN_UNDER_AP_MODE && defined CONFIG_CONCURRENT_MODE
#define RTW_SCAN_NUM_OF_CH			8
#define RTW_STAY_AP_CH_MILLISECOND	3	// this value is a multiplier,for example, when this value is 3, it would stay AP's op ch for 
											// 3 * SURVEY_TO millisecond. 
#endif //defined CONFIG_STA_MODE_SCAN_UNDER_AP_MODE && defined CONFIG_CONCURRENT_MODE

struct mlme_ext_info
{
	u32	state;
	u32	reauth_count;
	u32	reassoc_count;
	u32	link_count;
	u32	auth_seq;
	u32	auth_algo;	// 802.11 auth, could be open, shared, auto
	u32	authModeToggle;
	u32	enc_algo;//encrypt algorithm;
	u32	key_index;	// this is only valid for legendary wep, 0~3 for key id.
	u32	iv;
	u8	chg_txt[128];
	u16	aid;
	u16	bcn_interval;
	u16	capability;
	u8	assoc_AP_vendor;
	u8	slotTime;
	u8	preamble_mode;
	u8	WMM_enable;
    u8  DTIM_period;
	u8	ERP_enable;
	u8	ERP_IE;
	u8	HT_enable;
	u8	HT_caps_enable;	
	u8	HT_info_enable;
	u8	HT_protection;
	u8	turboMode_cts2self;
	u8	turboMode_rtsen;
	u8	SM_PS;
	u8	agg_enable_bitmap;
	u8	ADDBA_retry_count;
	u8	candidate_tid_bitmap;
	u8	dialogToken;
	// Accept ADDBA Request
#if RX_AGGREGATION
	BOOLEAN bAcceptAddbaReq;
#endif
	u8	bwmode_updated;
	u8	hidden_ssid_mode;

	struct ADDBA_request		ADDBA_req;
	struct WMM_para_element	WMM_param;
	struct HT_caps_element	HT_caps;
	struct HT_info_element		HT_info;
#ifdef CONFIG_MCC_MODE
	WLAN_BSSID_EX			network;	//join network or bss_network, if in ap mode, it is the same to cur_network.network. //use mlme_priv->cur_network.network. YJ,del,140408
#endif
	struct FW_Sta_Info		FW_sta_info[NUM_STA];

#ifdef CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
	u8 scan_cnt;
#endif //CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
#ifdef CONFIG_SAE_SUPPORT
	u8 user_group_id;			//for user to set group id
	struct sae_data *sae_priv;
#endif

#ifdef CONFIG_PMKSA_CACHING
	int pmk_cache_enable;
	struct pmksa_cache *pmksa;
#endif

};

// The channel information about this channel including joining, scanning, and power constraints.
#define PSCAN_ENABLE          0x01 //enable for partial channel scan
#define PSCAN_FAST_SURVEY     0x02 //set to select scan time to FAST_SURVEY_TO and resend probe request
#define PSCAN_SIMPLE_CONFIG   0x04 //set to select scan time to FAST_SURVEY_TO and resend probe request
#define PSCAN_SET_SSID_DONE   0x80 //When receive probe response, this bit is set to 1

#define PSCAN_DISABLE_MASK    0xFE //disable PSCAN_ENABLE
#define PSCAN_CLEAR_SSID_DONE 0x7F //clear PSCAN_SET_SSID_DONE
#define PSCAN_RETRY_TIMES        7 //the retry times of resending probe request when PSCAN_FAST_SURVEY is set

typedef struct _RT_CHANNEL_INFO
{
	u8				ChannelNum;		// The channel number.
	RT_SCAN_TYPE	ScanType;		// Scan type such as passive or active scan.
	//u16				ScanPeriod;		// Listen time in millisecond in this channel.
	//s32				MaxTxPwrDbm;	// Max allowed tx power.
	//u32				ExInfo;			// Extended Information for this channel.
#ifdef CONFIG_FIND_BEST_CHANNEL
	u32				rx_count;
#endif
#ifdef CONFIG_DFS
	#ifdef CONFIG_DFS_MASTER
	systime non_ocp_end_time;
	#endif
	u8 hidden_bss_cnt; /* per scan count */
#endif
	u8	pscan_config;
}RT_CHANNEL_INFO, *PRT_CHANNEL_INFO;

extern int rtw_is_channel_set_contains_channel(RT_CHANNEL_INFO *channel_set, const u32 channel_num, int *pchannel_idx);

#ifdef CONFIG_CUSTOM_IE
#ifndef _CUS_IE_
#define _CUS_IE_
typedef struct _cus_ie
{
	u8 *ie;
	u8 type;
}rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */
#endif

struct mlme_ext_priv
{
	_adapter	*padapter;
	u8	mlmeext_init;
	ATOMIC_T		event_seq;
	u16	mgnt_seq;
#ifdef CONFIG_IEEE80211W
	u16	sa_query_seq;
	u64 mgnt_80211w_IPN;
	u64 mgnt_80211w_IPN_rx;
	u8 key_type_11w; // for debug
#endif /* CONFIG_IEEE80211W */
	
	//struct fw_priv 	fwpriv;
	
	u8	cur_channel;
	u8	cur_bwmode;
	u8	cur_ch_offset;//PRIME_CHNL_OFFSET
	u8	cur_wireless_mode;	// NETWORK_TYPE
	u8	max_chan_nums;
	RT_CHANNEL_INFO		channel_set[MAX_CHANNEL_NUM + 1];
#ifdef CONFIG_P2P_NEW
	RT_CHANNEL_INFO		social_channel_set[4];
	RT_CHANNEL_INFO		special_channel[2];
	u8			special_mac[ETH_ALEN];
	u16		scan_mode;
	u8		bremain_on_channel;
	_timer remainon_timer;
#endif
	u8	basicrate[NumRates];
	u8	datarate[NumRates];
	
	struct ss_res		sitesurvey_res;		
	struct mlme_ext_info	mlmext_info;//for sta/adhoc mode, including current scanning/connecting/connected related info.
                                                     //for ap mode, network includes ap's cap_info
	_timer		survey_timer;
	_timer		link_timer;
	//_timer		ADDBA_timer;
	u16			chan_scan_time;

	u8	scan_abort;
	u8	tx_rate; // TXRATE when USERATE is set.

	u8	retry; //retry for issue probereq
	
	u64 TSFValue;

#ifdef CONFIG_AP_MODE	
	unsigned char bstart_bss;
#endif

#ifdef CONFIG_80211D
	u8 update_channel_plan_by_ap_done;
#endif
	//recv_decache check for Action_public frame 
	u16 	 action_public_rxseq;

	/* for softap power save */
#ifdef CONFIG_P2P_NEW
	u8 action_public_dialog_token;
#endif
#if CONFIG_AUTO_RECONNECT
	_timer reconnect_timer;
	u8 reconnect_deauth_filtered;
	u8 reconnect_times;
	u8 reconnect_cnt;
	u16 reconnect_timeout; // the unit is second
	u8 saved_alg;
	u8 saved_essid[32+1];
	u8 saved_key[32];
	u16 saved_key_len;
	u8 saved_key_idx;
	u8 saved_wpa_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
	u8 saved_eap_method;
	u8 auto_reconnect;
#endif
	u8 partial_scan;
#ifdef CONFIG_CUSTOM_IE
	p_rtw_custom_ie_t cus_ven_ie;
	u8 ie_num;
#endif

	_timer csa_timer;
	u8 csa_channel;

#ifdef CONFIG_CONCURRENT_MODE
	u8 bChDeauthDisabled;
	u8 bConcurrentFlushingSTA;
#endif
};

#define CAC_TIME_MS (60*1000)
#define CAC_TIME_CE_MS (10*60*1000)
#define NON_OCP_TIME_MS (30*60*1000)


void rtw_rfctl_init(_adapter *adapter);
void rtw_rfctl_deinit(_adapter *adapter);

#ifdef CONFIG_DFS_MASTER
struct rf_ctl_t;
#define CH_IS_NON_OCP(rt_ch_info) (time_after((rt_ch_info)->non_ocp_end_time, rtw_get_current_time()))
bool rtw_is_cac_reset_needed(struct rf_ctl_t *rfctl, u8 ch, u8 bw, u8 offset);
bool _rtw_rfctl_overlap_radar_detect_ch(struct rf_ctl_t *rfctl, u8 ch, u8 bw, u8 offset);
bool rtw_rfctl_overlap_radar_detect_ch(struct rf_ctl_t *rfctl);
bool rtw_rfctl_is_tx_blocked_by_ch_waiting(struct rf_ctl_t *rfctl);
bool rtw_chset_is_chbw_non_ocp(RT_CHANNEL_INFO *ch_set, u8 ch, u8 bw, u8 offset);
bool rtw_chset_is_ch_non_ocp(RT_CHANNEL_INFO *ch_set, u8 ch);
void rtw_chset_update_non_ocp(RT_CHANNEL_INFO *ch_set, u8 ch, u8 bw, u8 offset);
void rtw_chset_update_non_ocp_ms(RT_CHANNEL_INFO *ch_set, u8 ch, u8 bw, u8 offset, int ms);
u32 rtw_get_ch_waiting_ms(struct rf_ctl_t *rfctl, u8 ch, u8 bw, u8 offset, u32 *r_non_ocp_ms, u32 *r_cac_ms);
void rtw_reset_cac(struct rf_ctl_t *rfctl, u8 ch, u8 bw, u8 offset);
u32 rtw_force_stop_cac(struct rf_ctl_t *rfctl, u32 timeout_ms);
bool rtw_choose_shortest_waiting_ch(struct rf_ctl_t *rfctl, u8 sel_ch, u8 max_bw
	, u8 *dec_ch, u8 *dec_bw, u8 *dec_offset
	, u8 d_flags, u8 cur_ch, u8 same_band_prefer, u8 mesh_only);

#else
#define CH_IS_NON_OCP(rt_ch_info) 0
#define rtw_chset_is_chbw_non_ocp(ch_set, ch, bw, offset) _FALSE
#define rtw_chset_is_ch_non_ocp(ch_set, ch) _FALSE
#define rtw_rfctl_is_tx_blocked_by_ch_waiting(rfctl) _FALSE
#endif

enum {
	RTW_CHF_2G = BIT0,
	RTW_CHF_5G = BIT1,
	RTW_CHF_DFS = BIT2,
	RTW_CHF_LONG_CAC = BIT3,
	RTW_CHF_NON_DFS = BIT4,
	RTW_CHF_NON_LONG_CAC = BIT5,
	RTW_CHF_NON_OCP = BIT6,
};
#define mlmeext_scan_state(mlmeext) ((mlmeext)->sitesurvey_res.state)

int rtw_chset_search_ch(RT_CHANNEL_INFO *ch_set, const u32 ch);
u8 rtw_chset_is_chbw_valid(RT_CHANNEL_INFO *ch_set, u8 ch, u8 bw, u8 offset);
void rtw_chset_sync_chbw(RT_CHANNEL_INFO *ch_set, u8 *req_ch, u8 *req_bw, u8 *req_offset
	, u8 *g_ch, u8 *g_bw, u8 *g_offset);

bool rtw_mlme_band_check(_adapter *adapter, const u32 ch);
u8 init_channel_set(_adapter* padapter, u8 ChannelPlan, RT_CHANNEL_INFO *channel_set);
int init_mlme_ext_priv(_adapter* padapter);
int init_hw_mlme_ext(_adapter *padapter);
void free_mlme_ext_priv (struct mlme_ext_priv *pmlmeext);
extern void init_mlme_ext_timer(_adapter *padapter);
extern void init_addba_retry_timer(_adapter *padapter, struct sta_info *psta);
#ifdef CONFIG_IEEE80211W
extern void init_dot11w_expire_timer(_adapter *padapter, struct sta_info *psta);
#endif /* CONFIG_IEEE80211W */
extern struct xmit_frame *alloc_mgtxmitframe(struct xmit_priv *pxmitpriv);
extern struct xmit_frame *alloc_FwRsvdframe(struct xmit_priv *pxmitpriv, u32 size);
//void fill_fwpriv(_adapter * padapter, struct fw_priv *pfwpriv);

unsigned char networktype_to_raid(unsigned char network_type);
u8 judge_network_type(_adapter *padapter, unsigned char *rate, int ratelen);
void get_rate_set(_adapter *padapter, unsigned char *pbssrate, int *bssrate_len);
void UpdateBrateTbl(_adapter *padapter,u8 *mBratesOS);
void UpdateBrateTblForSoftAP(u8 *bssrateset, u32 bssratelen);

void Save_DM_Func_Flag(_adapter *padapter);
void Restore_DM_Func_Flag(_adapter *padapter);
void Switch_DM_Func(_adapter *padapter, u32 mode, u8 enable);

//void Set_NETYPE1_MSR(_adapter *padapter, u8 type);
//void Set_NETYPE0_MSR(_adapter *padapter, u8 type);
void Set_MSR(_adapter *padapter, u8 type);

u8 rtw_get_oper_ch(_adapter *adapter);
u8 rtw_get_oper_bw(_adapter *adapter);
u8 rtw_get_oper_choffset(_adapter *adapter);

systime rtw_get_on_oper_ch_time(_adapter *adapter);

u8 set_opmode(_adapter  *padapter, NDIS_802_11_NETWORK_INFRASTRUCTURE networktype);
void set_channel_bwmode(_adapter *padapter, unsigned char channel, unsigned char channel_offset, unsigned short bwmode);
void SelectChannel(_adapter *padapter, unsigned char channel);
u8 rtw_get_offset_by_chbw(u8 ch, u8 bw, u8 *r_offset);

unsigned int decide_wait_for_beacon_timeout(unsigned int bcn_interval);

void write_cam(_adapter *padapter, u8 entry, u16 ctrl, u8 *mac, u8 *key);
void clear_cam_entry(_adapter *padapter, u8 entry);

void invalidate_cam_all(_adapter *padapter);
void CAM_empty_entry(PADAPTER Adapter, u8 ucIndex);


int allocate_fw_sta_entry(_adapter *padapter);
void flush_all_cam_entry(_adapter *padapter);

BOOLEAN IsLegal5GChannel(PADAPTER Adapter, u8 channel);

void site_survey(_adapter *padapter);
u8 collect_bss_info(_adapter *padapter, union recv_frame *precv_frame, WLAN_BSSID_EX *bssid);
void update_network(WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src, _adapter * padapter, bool update_ie);

int get_bsstype(unsigned short capability);
u8* get_my_bssid(WLAN_BSSID_EX *pnetwork);
u16 get_beacon_interval(WLAN_BSSID_EX *bss);

int is_client_associated_to_ap(_adapter *padapter);
int is_client_associated_to_ibss(_adapter *padapter);
int is_IBSS_empty(_adapter *padapter);

unsigned char check_assoc_AP(u8 *pframe, uint len);

int WMM_param_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs	pIE);
#ifdef CONFIG_WFD
int WFD_info_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs	pIE);
#endif
void WMMOnAssocRsp(_adapter *padapter);

void HT_caps_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HT_info_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HTOnAssocRsp(_adapter *padapter);

void ERP_IE_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void VCS_update(_adapter *padapter, struct sta_info *psta);

void update_beacon_info(_adapter *padapter, u8 *pframe, uint len, struct sta_info *psta);
int rtw_check_bcn_info(ADAPTER *Adapter, u8 *pframe, u32 packet_len);
#ifdef CONFIG_DFS
void process_csa_ie(_adapter *padapter, u8 *pframe, uint len);
#endif //CONFIG_DFS
void update_IOT_info(_adapter *padapter);
void update_capinfo(PADAPTER Adapter, u16 updateCap);
void update_wireless_mode(_adapter * padapter);
void update_tx_basic_rate(_adapter *padapter, u8 modulation);
void update_bmc_sta_support_rate(_adapter *padapter, u32 mac_id);
int update_sta_support_rate(_adapter *padapter, u8* pvar_ie, uint var_ie_len, int cam_idx);

//for sta/adhoc mode
void update_sta_info(_adapter *padapter, struct sta_info *psta);
unsigned int update_basic_rate(unsigned char *ptn, unsigned int ptn_sz);
unsigned int update_supported_rate(unsigned char *ptn, unsigned int ptn_sz);
unsigned int update_MCS_rate(struct HT_caps_element *pHT_caps);
void Update_RA_Entry(_adapter *padapter, struct sta_info *psta);
void set_sta_rate(_adapter *padapter, struct sta_info *psta);

unsigned int receive_disconnect(_adapter *padapter, unsigned char *MacAddr, unsigned short reason);

unsigned char get_highest_rate_idx(u32 mask);
int support_short_GI(_adapter *padapter, struct HT_caps_element *pHT_caps, u8 bwmode);
unsigned int is_ap_in_tkip(_adapter *padapter);
unsigned int is_ap_in_wep(_adapter *padapter);
unsigned int should_forbid_n_rate(_adapter * padapter);

struct macid_ctl_t;
void rtw_macid_ctl_set_h2c_msr(struct macid_ctl_t *macid_ctl, u8 id, u8 h2c_msr);
void rtw_macid_ctl_set_bw(struct macid_ctl_t *macid_ctl, u8 id, u8 bw);
void rtw_macid_ctl_set_vht_en(struct macid_ctl_t *macid_ctl, u8 id, u8 en);
void rtw_macid_ctl_set_rate_bmp0(struct macid_ctl_t *macid_ctl, u8 id, u32 bmp);
void rtw_macid_ctl_set_rate_bmp1(struct macid_ctl_t *macid_ctl, u8 id, u32 bmp);
void rtw_macid_ctl_init_sleep_reg(struct macid_ctl_t *macid_ctl, u16 m0, u16 m1, u16 m2, u16 m3);
void rtw_macid_ctl_init(struct macid_ctl_t *macid_ctl);
void rtw_macid_ctl_deinit(struct macid_ctl_t *macid_ctl);

void report_join_res(_adapter *padapter, int res);
void report_survey_event(_adapter *padapter, union recv_frame *precv_frame);
void report_surveydone_event(_adapter *padapter);
void report_del_sta_event(_adapter *padapter, unsigned char* MacAddr, unsigned short reason);
void report_add_sta_event(_adapter *padapter, unsigned char* MacAddr, int cam_idx);

void beacon_timing_control(_adapter *padapter);
extern u8 set_tx_beacon_cmd(_adapter*padapter);
unsigned int setup_beacon_frame(_adapter *padapter, unsigned char *beacon_frame);
void update_mgnt_tx_rate(_adapter *padapter, u8 rate);
void update_mgntframe_attrib(_adapter *padapter, struct pkt_attrib *pattrib);
void update_mgntframe_attrib_addr(_adapter *padapter, struct xmit_frame *pmgntframe);
void dump_mgntframe(_adapter *padapter, struct xmit_frame *pmgntframe);
s32 dump_mgntframe_and_wait(_adapter *padapter, struct xmit_frame *pmgntframe, int timeout_ms);

#ifdef CONFIG_P2P
void issue_probersp_p2p(_adapter *padapter, unsigned char *da);
void issue_p2p_provision_request( _adapter *padapter, u8* pssid, u8 ussidlen, u8* pdev_raddr);
void issue_p2p_GO_request(_adapter *padapter, u8* raddr);
void issue_probereq_p2p(_adapter *padapter);
void issue_p2p_invitation_response(_adapter *padapter, u8* raddr, u8 dialogToken, u8 success);
void issue_p2p_invitation_request(_adapter *padapter, u8* raddr );
#endif //CONFIG_P2P
void issue_beacon(_adapter *padapter);
void issue_probersp(_adapter *padapter, unsigned char *da, u8 is_valid_p2p_probereq);
void issue_assocreq(_adapter *padapter);
void issue_asocrsp(_adapter *padapter, unsigned short status, struct sta_info *pstat, int pkt_type);
void issue_auth(_adapter *padapter, struct sta_info *psta, unsigned short status);
//	Added by Albert 2010/07/26
//	blnbc: 1 -> broadcast probe request
//	blnbc: 0 -> unicast probe request. The address 1 will be the BSSID.
void issue_probereq(_adapter *padapter, NDIS_802_11_SSID *pssid, u8 blnbc);
void issue_nulldata(_adapter *padapter, unsigned int power_mode);
void issue_qos_nulldata(_adapter *padapter, unsigned char *da, u16 tid);
void issue_deauth(_adapter *padapter, unsigned char *da, u32 reason);
void issue_action_BA(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short status);
#ifdef CONFIG_IEEE80211W
void issue_action_SA_Query(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short tid, u8 key_type);
#endif
unsigned int send_delba(_adapter *padapter, u8 initiator, u8 *addr);
unsigned int send_beacon(_adapter *padapter);

void start_clnt_assoc(_adapter *padapter);
void start_clnt_auth(_adapter* padapter);
void start_clnt_join(_adapter* padapter);
void start_create_ibss(_adapter* padapter);

unsigned int OnAssocReq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAssocRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeReq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int DoReserved(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnBeacon(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAtim(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDisassoc(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuthClient(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDeAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction(_adapter *padapter, union recv_frame *precv_frame);

unsigned int OnAction_qos(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_dls(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_back(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_public(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_ht(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_IEEE80211W
unsigned int OnAction_sa_query(_adapter *padapter, union recv_frame *precv_frame);
#endif
unsigned int OnAction_wmm(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_p2p(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_csa(_adapter *padapter, union recv_frame *precv_frame);


void mlmeext_joinbss_event_callback(_adapter *padapter, int join_res);
void mlmeext_sta_del_event_callback(_adapter *padapter);
void mlmeext_sta_add_event_callback(_adapter *padapter, struct sta_info *psta);

void linked_status_chk(_adapter *padapter);

void survey_timer_hdl (_adapter *padapter);
void link_timer_hdl (_adapter *padapter);
void addba_timer_hdl(struct sta_info *psta);
#ifdef CONFIG_IEEE80211W
void sa_query_timer_hdl(struct sta_info *psta);
#endif /* CONFIG_IEEE80211W */

//void reauth_timer_hdl(_adapter *padapter);
//void reassoc_timer_hdl(_adapter *padapter);

#define set_survey_timer(mlmeext, ms) \
	do { \
		/*DBG_871X("%s set_survey_timer(%p, %d)\n", __FUNCTION__, (mlmeext), (ms));*/ \
		rtw_set_timer(&(mlmeext)->survey_timer, (ms)); \
	} while(0)

#define set_link_timer(mlmeext, ms) \
	do { \
		/*DBG_871X("%s set_link_timer(%p, %d)\n", __FUNCTION__, (mlmeext), (ms));*/ \
		rtw_set_timer(&(mlmeext)->link_timer, (ms)); \
	} while(0)

//TODO
#if 0
extern int cckrates_included(unsigned char *rate, int ratelen);
extern int cckratesonly_included(unsigned char *rate, int ratelen);
extern void process_addba_req(_adapter *padapter, u8 *paddba_req, u8 *addr);
#endif

extern void update_TSF(struct mlme_ext_priv *pmlmeext, u8 *pframe, uint len);
extern void correct_TSF(_adapter *padapter, struct mlme_ext_priv *pmlmeext);

#ifdef CONFIG_CONCURRENT_MODE
 sint check_buddy_mlmeinfo_state(_adapter *padapter, u32 state);
int concurrent_chk_start_clnt_join(_adapter *padapter);
void concurrent_chk_joinbss_done(_adapter *padapter, int join_res);
#endif //CONFIG_CONCURRENT_MODE

#ifdef CONFIG_PLATFORM_ARM_SUN8I
	#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	8000
#else
	#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	12000
#endif

struct cmd_hdl {
	uint	parmsize;
	u8 (*h2cfuns)(struct _ADAPTER *padapter, u8 *pbuf);	
};

//TODO
#if 0

u8 read_macreg_hdl(_adapter *padapter, u8 *pbuf);
u8 write_macreg_hdl(_adapter *padapter, u8 *pbuf);
u8 read_bbreg_hdl(_adapter *padapter, u8 *pbuf);
u8 write_bbreg_hdl(_adapter *padapter, u8 *pbuf);
u8 read_rfreg_hdl(_adapter *padapter, u8 *pbuf);
u8 write_rfreg_hdl(_adapter *padapter, u8 *pbuf);

#endif	//#if 0

u8 NULL_hdl(_adapter *padapter, u8 *pbuf);
u8 join_cmd_hdl(_adapter *padapter, u8 *pbuf);
u8 disconnect_hdl(_adapter *padapter, u8 *pbuf);
u8 createbss_hdl(_adapter *padapter, u8 *pbuf);
u8 setopmode_hdl(_adapter *padapter, u8 *pbuf);
u8 sitesurvey_cmd_hdl(_adapter *padapter, u8 *pbuf);	
u8 setauth_hdl(_adapter *padapter, u8 *pbuf);
u8 setkey_hdl(_adapter *padapter, u8 *pbuf);
u8 set_stakey_hdl(_adapter *padapter, u8 *pbuf);
u8 set_assocsta_hdl(_adapter *padapter, u8 *pbuf);
u8 del_assocsta_hdl(_adapter *padapter, u8 *pbuf);
u8 add_ba_hdl(_adapter *padapter, unsigned char *pbuf);

u8 mlme_evt_hdl(_adapter *padapter, unsigned char *pbuf);
u8 h2c_msg_hdl(_adapter *padapter, unsigned char *pbuf);
u8 tx_beacon_hdl(_adapter *padapter, unsigned char *pbuf);
u8 set_chplan_hdl(_adapter *padapter, unsigned char *pbuf);
u8 led_blink_hdl(_adapter *padapter, unsigned char *pbuf);
u8 set_csa_hdl(_adapter *padapter, unsigned char *pbuf);	//Kurt: Handling DFS channel switch announcement ie.
u8 tdls_hdl(_adapter *padapter, unsigned char *pbuf);

#if CONFIG_AUTO_RECONNECT
extern void reconnect_timer_hdl(void *FunctionContext);
#endif

extern void csa_timer_hdl(void *FunctionContext);

#define GEN_DRV_CMD_HANDLER(size, cmd)	{size, &cmd ## _hdl},
#define GEN_MLME_EXT_HANDLER(size, cmd)	{size, cmd},

#ifdef _RTW_CMD_C_

const struct cmd_hdl wlancmds[] = 
{
	GEN_DRV_CMD_HANDLER(0, NULL) /*0*/
	GEN_DRV_CMD_HANDLER(0, NULL)
	GEN_DRV_CMD_HANDLER(0, NULL)
	GEN_DRV_CMD_HANDLER(0, NULL)
	GEN_DRV_CMD_HANDLER(0, NULL)
	GEN_DRV_CMD_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL) /*10*/
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)		
	GEN_MLME_EXT_HANDLER(sizeof (struct joinbss_parm), join_cmd_hdl) /*14*/
	GEN_MLME_EXT_HANDLER(sizeof (struct disconnect_parm), disconnect_hdl)
//TODO
//	GEN_MLME_EXT_HANDLER(sizeof (struct createbss_parm), createbss_hdl)
	GEN_MLME_EXT_HANDLER(sizeof (struct createbss_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct setopmode_parm), setopmode_hdl)
	GEN_MLME_EXT_HANDLER(sizeof (struct sitesurvey_parm), sitesurvey_cmd_hdl) /*18*/
	GEN_MLME_EXT_HANDLER(sizeof (struct setauth_parm), setauth_hdl)
	GEN_MLME_EXT_HANDLER(sizeof (struct setkey_parm), setkey_hdl) /*20*/
	GEN_MLME_EXT_HANDLER(sizeof (struct set_stakey_parm), set_stakey_hdl)
	GEN_MLME_EXT_HANDLER(sizeof (struct set_assocsta_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct del_assocsta_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct setstapwrstate_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct setbasicrate_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct getbasicrate_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct setdatarate_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct getdatarate_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct setphyinfo_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct getphyinfo_parm), NULL)  /*30*/
	GEN_MLME_EXT_HANDLER(sizeof (struct setphy_parm), NULL)
	GEN_MLME_EXT_HANDLER(sizeof (struct getphy_parm), NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)	/*40*/
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
//TODO
#if RX_AGGREGATION
	GEN_MLME_EXT_HANDLER(sizeof(struct addBaReq_parm), add_ba_hdl)
#else
	GEN_MLME_EXT_HANDLER(sizeof(struct addBaReq_parm), NULL)		
#endif	
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL) /*50*/
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL)
	GEN_MLME_EXT_HANDLER(0, NULL) 
	GEN_MLME_EXT_HANDLER(sizeof(struct Tx_Beacon_param), tx_beacon_hdl) /*55*/
	GEN_MLME_EXT_HANDLER(0, mlme_evt_hdl) /*56*/
	GEN_MLME_EXT_HANDLER(0, rtw_drvextra_cmd_hdl) /*57*/
//TODO
//	GEN_MLME_EXT_HANDLER(0, h2c_msg_hdl) /*58*/
	GEN_MLME_EXT_HANDLER(0, NULL) /*58*/
//TODO
//	GEN_MLME_EXT_HANDLER(sizeof(struct SetChannelPlan_param), set_chplan_hdl) /*59*/
	GEN_MLME_EXT_HANDLER(sizeof(struct SetChannelPlan_param), NULL) /*59*/
//TODO
//	GEN_MLME_EXT_HANDLER(sizeof(struct LedBlink_param), led_blink_hdl) /*60*/
	GEN_MLME_EXT_HANDLER(0, NULL) /*60*/
//TODO
	GEN_MLME_EXT_HANDLER(sizeof(struct SetChannelSwitch_param), set_csa_hdl) /*61*/
	//GEN_MLME_EXT_HANDLER(sizeof(struct SetChannelSwitch_param), NULL) /*61*/
//TODO
//	GEN_MLME_EXT_HANDLER(sizeof(struct TDLSoption_param), tdls_hdl) /*62*/
	GEN_MLME_EXT_HANDLER(sizeof(struct TDLSoption_param), NULL) /*62*/
#ifdef CONFIG_P2P_NEW
	GEN_MLME_EXT_HANDLER(0, rtw_p2p_cmd_hdl) /*63*/
#endif
};

#endif

struct C2HEvent_Header
{

#ifdef CONFIG_LITTLE_ENDIAN

	unsigned int len:16;
	unsigned int ID:8;
	unsigned int seq:8;
	
#elif defined(CONFIG_BIG_ENDIAN)

	unsigned int seq:8;
	unsigned int ID:8;
	unsigned int len:16;
	
#else

#  error "Must be LITTLE or BIG Endian"

#endif

	unsigned int rsvd;

};

void rtw_dummy_event_callback(_adapter *adapter , u8 *pbuf);
void rtw_fwdbg_event_callback(_adapter *adapter , u8 *pbuf);

enum rtw_c2h_event
{
	GEN_EVT_CODE(_Read_MACREG)=0, /*0*/
	GEN_EVT_CODE(_Read_BBREG),
 	GEN_EVT_CODE(_Read_RFREG),
 	GEN_EVT_CODE(_Read_EEPROM),
 	GEN_EVT_CODE(_Read_EFUSE),
	GEN_EVT_CODE(_Read_CAM),			/*5*/
 	GEN_EVT_CODE(_Get_BasicRate),  
 	GEN_EVT_CODE(_Get_DataRate),   
 	GEN_EVT_CODE(_Survey),	 /*8*/
 	GEN_EVT_CODE(_SurveyDone),	 /*9*/
 	
 	GEN_EVT_CODE(_JoinBss) , /*10*/
 	GEN_EVT_CODE(_AddSTA),
 	GEN_EVT_CODE(_DelSTA),
 	GEN_EVT_CODE(_AtimDone) ,
 	GEN_EVT_CODE(_TX_Report),  
	GEN_EVT_CODE(_CCX_Report),			/*15*/
 	GEN_EVT_CODE(_DTM_Report),
 	GEN_EVT_CODE(_TX_Rate_Statistics),
 	GEN_EVT_CODE(_C2HLBK), 
 	GEN_EVT_CODE(_FWDBG),
	GEN_EVT_CODE(_C2HFEEDBACK),               /*20*/
	GEN_EVT_CODE(_ADDBA),
	GEN_EVT_CODE(_C2HBCN),
	GEN_EVT_CODE(_ReportPwrState),		//filen: only for PCIE, USB	
	GEN_EVT_CODE(_CloseRF),				//filen: only for PCIE, work around ASPM
#ifdef CONFIG_IEEE80211W
	GEN_EVT_CODE(_TimeoutSTA),
#endif /* CONFIG_IEEE80211W */
 	MAX_C2HEVT
};


#ifdef _RTW_MLME_EXT_C_		

static const struct fwevent wlanevents[] = 
{
	{0, rtw_dummy_event_callback}, 	/*0*/
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, &rtw_survey_event_callback},		/*8*/
	{sizeof (struct surveydone_event), &rtw_surveydone_event_callback},	/*9*/
	{0, &rtw_joinbss_event_callback},		/*10*/
	{sizeof(struct stassoc_event), &rtw_stassoc_event_callback},
	{sizeof(struct stadel_event), &rtw_stadel_event_callback},	
//TODO
//	{0, &rtw_atimdone_event_callback},
	{0, NULL},	/*rtw_atimdone_event_callback*/
	{0, rtw_dummy_event_callback},
	{0, NULL},	/*15*/
	{0, NULL},
	{0, NULL},
	{0, NULL},
//TODO
//	{0, rtw_fwdbg_event_callback},
	{0, NULL},	/*rtw_fwdbg_event_callback*/
	{0, NULL},	/*20*/
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
#if defined(CONFIG_IEEE80211W) && defined(CONFIG_AP_MODE)
	{sizeof(struct stadel_event), &rtw_sta_timeout_event_callback},	/*25*/
#endif /* CONFIG_IEEE80211W */

//TODO
//	{0, &rtw_cpwm_event_callback},
	{0, NULL},	/*rtw_cpwm_event_callback*/
};

#endif//_RTL8192C_CMD_C_

#endif

