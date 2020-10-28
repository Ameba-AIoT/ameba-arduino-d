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
#ifndef __RTL8721D_CMD_H__
#define __RTL8721D_CMD_H__

//---------------------------------------------------------------------------------------------------------//
//----------------------------------    H2C CMD DEFINITION    ------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//

enum h2c_cmd{
	//Common Class: 000
	H2C_RSVD_PAGE = 0x00,
	H2C_MEDIA_STATUS_RPT = 0x01,
	H2C_SCAN_ENABLE = 0x02,
	H2C_KEEP_ALIVE = 0x03,
	H2C_DISCON_DECISION = 0x04,	
	H2C_PSD_OFFLOAD = 0x05,	
	H2C_AP_OFFLOAD = 0x08,	
	H2C_BCN_RSVDPAGE = 0x09,	
	H2C_PROBERSP_RSVDPAGE = 0x0A,	
	H2C_FCS_RSVDPAGE = 0x10,	
	H2C_FCS_INFO = 0x11,	
	H2C_AP_WOW_GPIO_CTRL = 0x13,

	//PoweSave Class: 001
	H2C_SET_PWR_MODE = 0x20,
	H2C_PS_TUNING_PARA = 0x21,
	H2C_PS_TUNING_PARA2 = 0x22,
	H2C_P2P_LPS_PARAM = 0x23,	
	H2C_P2P_PS_OFFLOAD = 0x24,	
	H2C_PS_SCAN_ENABLE = 0x25,	
	H2C_SAP_PS_ = 0x26,	
	H2C_INACTIVE_PS_ = 0x27, //Inactive_PS
	H2C_FWLPS_IN_IPS_ = 0x28,
	
	//Dynamic Mechanism Class: 010
	H2C_MACID_CFG = 0x40,
	H2C_TXBF = 0x41,	
	H2C_RSSI_SETTING = 0x42,	
	H2C_AP_REQ_TXRPT = 0x43,	
	H2C_INIT_RATE_COLLECT = 0x44,	
	H2C_IQ_CALIBRATION	= 0x45,

	H2C_RA_MASK_3SS = 0x46,/* for 8814A */
	H2C_RA_PARA_ADJUST = 0x47,/* CONFIG_RA_DBG_CMD */
	H2C_DYNAMIC_TX_PATH = 0x48,/* for 8814A */

	H2C_FW_TRACE_EN = 0x49,
	
	//BT Class: 011
	H2C_B_TYPE_TDMA = 0x60,
	H2C_BT_INFO = 0x61,
	H2C_FORCE_BT_TXPWR = 0x62,
	H2C_BT_IGNORE_WLANACT = 0x63,
	H2C_DAC_SWING_VALUE = 0x64,
	H2C_ANT_SEL_RSV = 0x65,
	H2C_WL_OPMODE = 0x66,
	H2C_BT_MP_OPER = 0x67,
	H2C_BT_CONTROL = 0x68,
	H2C_BT_WIFI_CTRL = 0x69,
	H2C_BT_FW_PATCH = 0x6A,
	
	//WOWLAN Class: 100
	H2C_WOWLAN = 0x80,
	H2C_REMOTE_WAKE_CTRL = 0x81,
	H2C_AOAC_GLOBAL_INFO = 0x82,	
	H2C_AOAC_RSVD_PAGE = 0x83,	
	H2C_AOAC_RSVD_PAGE2 = 0x84,
	H2C_D0_SCAN_OFFLOAD_CTRL = 0x85,
	H2C_D0_SCAN_OFFLOAD_INFO = 0x86,
	H2C_CHNL_SWITCH_OFFLOAD = 0x87,
	H2C_AOAC_RSVDPAGE3 = 0x88,
	H2C_P2P_OFFLOAD_RSVD_PAGE = 0x8A,
	H2C_P2P_OFFLOAD = 0x8B,

	H2C_RESET_TSF = 0xC0,
	H2C_UC_PS = 0xCD,
	H2C_MAXID,
};


enum rcu_sub_cmd{
	//Common Class: 000
	RCU_CLOSE_RF = 0x00,
	RCU_JOIN_STATUS = 0x01,
	RCU_SET_PWR_MODE = 0x02,
	RCU_SET_WOWLAN_MODE = 0x04,

};

#define H2C_INACTIVE_PS_LEN		3
#define H2C_RSVDPAGE_LOC_LEN		5
#define H2C_MEDIA_STATUS_RPT_LEN		3
#define H2C_KEEP_ALIVE_CTRL_LEN	2
#define H2C_DISCON_DECISION_LEN	 	3
#define H2C_AP_OFFLOAD_LEN		3
#define H2C_AP_WOW_GPIO_CTRL_LEN	4
#define H2C_AP_PS_LEN			2
#define H2C_PWRMODE_LEN			7
#define H2C_PSTUNEPARAM_LEN			4
#define H2C_MACID_CFG_LEN	 	7
#define H2C_BTMP_OPER_LEN			4
#define H2C_WOWLAN_LEN			5
#define H2C_REMOTE_WAKE_CTRL_LEN	3
#define H2C_AOAC_GLOBAL_INFO_LEN	2
#define H2C_AOAC_RSVDPAGE_LOC_LEN	7
#define H2C_SCAN_OFFLOAD_CTRL_LEN	4
#define H2C_BT_FW_PATCH_LEN			6
#define H2C_RSSI_SETTING_LEN		4
#define H2C_AP_REQ_TXRPT_LEN		2
#define H2C_FORCE_BT_TXPWR_LEN		3
#define H2C_BCN_RSVDPAGE_LEN		5
#define H2C_PROBERSP_RSVDPAGE_LEN	5
#define H2C_P2PRSVDPAGE_LOC_LEN	5
#define H2C_P2P_OFFLOAD_LEN	3
#define H2C_RCU_PS_LEN 3
#define H2C_B_TYPE_TDMA_LEN	5


//_RSVDPAGE_LOC_CMD_0x00
#define SET_H2CCMD_RSVDPAGE_LOC_PROBE_RSP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_PSPOLL(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_NULL_DATA(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_QOS_NULL_DATA(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_BT_QOS_NULL_DATA(__pH2CCmd, __Value)SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+4, 0, 8, __Value)

//_MEDIA_STATUS_RPT_PARM_CMD_0x01
#define SET_H2CCMD_MSRRPT_PARM_OPMODE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_MSRRPT_PARM_MACID_IND(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_MSRRPT_PARM_MACID(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd+1, 0, 8, __Value)
#define SET_H2CCMD_MSRRPT_PARM_MACID_END(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd+2, 0, 8, __Value)

//_KEEP_ALIVE_CMD_0x03
#define SET_H2CCMD_KEEPALIVE_PARM_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_KEEPALIVE_PARM_ADOPT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_KEEPALIVE_PARM_PKT_TYPE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_H2CCMD_KEEPALIVE_PARM_CHECK_PERIOD(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd+1, 0, 8, __Value)

//_DISCONNECT_DECISION_CMD_0x04
#define SET_H2CCMD_DISCONDECISION_PARM_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_DISCONDECISION_PARM_ADOPT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_DISCONDECISION_PARM_CHECK_PERIOD(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd+1, 0, 8, __Value)
#define SET_H2CCMD_DISCONDECISION_PARM_TRY_PKT_NUM(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd+2, 0, 8, __Value)

//_AP_Offload 0x08
#define SET_H2CCMD_AP_WOWLAN_EN(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
//_BCN_RsvdPage	0x09
#define SET_H2CCMD_AP_WOWLAN_RSVDPAGE_LOC_BCN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
//_Probersp_RsvdPage 0x0a
#define SET_H2CCMD_AP_WOWLAN_RSVDPAGE_LOC_ProbeRsp(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
//_Probersp_RsvdPage 0x13
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_INDEX(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 4, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_C2H_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_PLUS(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 5, 1, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_HIGH_ACTIVE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 6, 1, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 7, 1, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_DURATION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_AP_WOW_GPIO_CTRL_C2H_DURATION(__pH2CCmd, __Value)SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
//_AP_PS 0x26
#define SET_H2CCMD_AP_WOW_PS_EN(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_AP_WOW_PS_32K_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_AP_WOW_PS_RF(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_H2CCMD_AP_WOW_PS_DURATION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)

// _WoWLAN PARAM_CMD_0x80
#define SET_H2CCMD_WOWLAN_FUNC_ENABLE(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_WOWLAN_PATTERN_MATCH_ENABLE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_WOWLAN_MAGIC_PKT_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_H2CCMD_WOWLAN_UNICAST_PKT_ENABLE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)
#define SET_H2CCMD_WOWLAN_ALL_PKT_DROP(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)
#define SET_H2CCMD_WOWLAN_GPIO_ACTIVE(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 5, 1, __Value)
#define SET_H2CCMD_WOWLAN_REKEY_WAKE_UP(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 6, 1, __Value)
#define SET_H2CCMD_WOWLAN_DISCONNECT_WAKE_UP(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 7, 1, __Value)
#define SET_H2CCMD_WOWLAN_GPIONUM(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 7, __Value)
#define SET_H2CCMD_WOWLAN_DATAPIN_WAKE_UP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 7, 1, __Value)
#define SET_H2CCMD_WOWLAN_GPIO_DURATION(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_WOWLAN_GPIO_PULSE_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 1, __Value)
#define SET_H2CCMD_WOWLAN_GPIO_PULSE_COUNT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 1, 7, __Value)
#define SET_H2CCMD_WOWLAN_LOWPR_RX(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 1, __Value)
#define SET_H2CCMD_WOWLAN_CHANGE_UNIT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 2, 1, __Value)
// _REMOTE_WAKEUP_CMD_0x81
#define SET_H2CCMD_REMOTE_WAKECTRL_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_ARP_OFFLOAD_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_NDP_OFFLOAD_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_GTK_OFFLOAD_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_NLO_OFFLOAD_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_FW_UNICAST_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 7, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_P2P_OFFLAD_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_NBNS_FILTER_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 2, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_ARP_ACTION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 1, __Value)
#define SET_H2CCMD_REMOTE_WAKE_CTRL_FW_PARSING_UNTIL_WAKEUP(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 4, 1, __Value)

// AOAC_GLOBAL_INFO_0x82
#define SET_H2CCMD_AOAC_GLOBAL_INFO_PAIRWISE_ENC_ALG(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
#define SET_H2CCMD_AOAC_GLOBAL_INFO_GROUP_ENC_ALG(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)

// AOAC_RSVDPAGE_LOC_0x83
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_REMOTE_WAKE_CTRL_INFO(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd), 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_ARP_RSP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_NEIGHBOR_ADV(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_GTK_RSP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_GTK_INFO(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+4, 0, 8, __Value)
#ifdef CONFIG_GTK_OL
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_GTK_EXT_MEM(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+5, 0, 8, __Value)
#endif //CONFIG_GTK_OL
#ifdef CONFIG_PNO_SUPPORT
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_NLO_INFO(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd), 0, 8, __Value)
#endif

#ifdef CONFIG_PNO_SUPPORT
// D0_Scan_Offload_Info_0x86
#define SET_H2CCMD_AOAC_NLO_FUN_EN(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd), 3, 1, __Value)
#define SET_H2CCMD_AOAC_NLO_IPS_EN(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd), 4, 1, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_PROBE_PACKET(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_SCAN_INFO(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_AOAC_RSVDPAGE_LOC_SSID_INFO(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+3, 0, 8, __Value)
#endif //CONFIG_PNO_SUPPORT

#ifdef CONFIG_P2P_WOWLAN
//P2P_RsvdPage_0x8a
#define SET_H2CCMD_RSVDPAGE_LOC_P2P_BCN(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_P2P_PROBE_RSP(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_P2P_NEGO_RSP(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_P2P_INVITE_RSP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_RSVDPAGE_LOC_P2P_PD_RSP(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE_8BIT((__pH2CCmd)+4, 0, 8, __Value)
#endif //CONFIG_P2P_WOWLAN


enum h2c_cmd_8721D{
	//Common Class: 000
	H2C_8721D_RSVD_PAGE = 0x00,
	H2C_8721D_MEDIA_STATUS_RPT = 0x01,
	H2C_8721D_SCAN_ENABLE = 0x02,
	H2C_8721D_KEEP_ALIVE = 0x03,
	H2C_8721D_DISCON_DECISION = 0x04,	
	H2C_8721D_PSD_OFFLOAD = 0x05,	
	H2C_8721D_AP_OFFLOAD = 0x08,	
	H2C_8721D_BCN_RSVDPAGE = 0x09,	
	H2C_8721D_PROBERSP_RSVDPAGE = 0x0A,	
	H2C_8721D_FCS_RSVDPAGE = 0x10,	
	H2C_8721D_FCS_INFO = 0x11,	

	//PoweSave Class: 001
	H2C_8721D_SET_PWR_MODE = 0x20,
	H2C_8721D_PS_TUNING_PARA = 0x21,
	H2C_8721D_PS_TUNING_PARA2 = 0x22,
	H2C_8721D_P2P_LPS_PARAM = 0x23,	
	H2C_8721D_P2P_PS_OFFLOAD = 0x24,	
	H2C_8721D_PS_SCAN_ENABLE = 0x25,	
	H2C_8721D_SAP_PS_ = 0x26,	
	H2C_8721D_INACTIVE_PS_ = 0x27, //Inactive_PS
	H2C_8721D_FWLPS_IN_IPS_ = 0x28,
	

	//Dynamic Mechanism Class: 010
	H2C_8721D_MACID_CFG = 0x40,	
	H2C_8721D_TXBF = 0x41,	
	H2C_8721D_RSSI_SETTING = 0x42,	
	H2C_8721D_AP_REQ_TXRPT = 0x43,	
	H2C_8721D_INIT_RATE_COLLECT = 0x44,	
	
	//BT Class: 011
	H2C_8721D_B_TYPE_TDMA = 0x60,
	H2C_8721D_BT_INFO = 0x61,
	H2C_8721D_FORCE_BT_TXPWR = 0x62,
	H2C_8721D_BT_IGNORE_WLANACT = 0x63,
	H2C_8721D_DAC_SWING_VALUE = 0x64,
	H2C_8721D_ANT_SEL_RSV = 0x65,
	H2C_8721D_WL_OPMODE = 0x66,
	H2C_8721D_BT_MP_OPER = 0x67,
	H2C_8721D_BT_CONTROL = 0x68,
	H2C_8721D_BT_WIFI_CTRL = 0x69,
	H2C_8721D_BT_FW_PATCH = 0x6A,
	
	//WOWLAN Class: 100
	H2C_8721D_WOWLAN = 0x80,
	H2C_8721D_REMOTE_WAKE_CTRL = 0x81,
	H2C_8721D_AOAC_GLOBAL_INFO = 0x82,	
	H2C_8721D_AOAC_RSVD_PAGE = 0x83,	
	H2C_8721D_AOAC_RSVD_PAGE2 = 0x84,
	H2C_8721D_D0_SCAN_OFFLOAD_INFO = 0x85,
	H2C_8721D_D0_SCAN_OFFLOAD_CTRL = 0x86,
	H2C_8721D_CHNL_SWITCH_OFFLOAD = 0x87,

	H2C_8721D_RESET_TSF = 0xC0,
	H2C_8721D_BCN_IGNORE_EDCCA = 0xC2,
	
	H2C_8721D_MAXID,
};

#define H2C_8721D_RSVDPAGE_LOC_LEN		5
#define H2C_8721D_MEDIA_STATUS_RPT_LEN		3
#define H2C_8721D_KEEP_ALIVE_CTRL_LEN	2
#define H2C_8721D_DISCON_DECISION_LEN	 	3
//#define H2C_8721D_AP_OFFLOAD_LEN		3
#define H2C_8721D_PWRMODE_LEN			7
#define H2C_8721D_PSTUNEPARAM_LEN			5 ///4
#define H2C_8721D_MACID_CFG_LEN	 	7
#define H2C_8721D_BTMP_OPER_LEN			4
#define H2C_8721D_WOWLAN_LEN			3
#define H2C_8721D_REMOTE_WAKE_CTRL_LEN	1
#define H2C_8721D_AOAC_GLOBAL_INFO_LEN	2
#define H2C_8721D_AOAC_RSVDPAGE_LOC_LEN	7
//#define H2C_8723B_SCAN_OFFLOAD_CTRL_LEN	4
#define H2C_8721D_BT_FW_PATCH_LEN			6
#define H2C_8721D_RSSI_SETTING_LEN		4
#define H2C_8721D_AP_REQ_TXRPT_LEN		2
#define H2C_8721D_FORCE_BT_TXPWR_LEN		3
#define H2C_8721D_BCN_IGNORE_EDCCA_LEN		1


#ifdef CONFIG_WOWLAN	
#define eqMacAddr(a,b)						( ((a)[0]==(b)[0] && (a)[1]==(b)[1] && (a)[2]==(b)[2] && (a)[3]==(b)[3] && (a)[4]==(b)[4] && (a)[5]==(b)[5]) ? 1:0 )
#define cpMacAddr(des,src)					((des)[0]=(src)[0],(des)[1]=(src)[1],(des)[2]=(src)[2],(des)[3]=(src)[3],(des)[4]=(src)[4],(des)[5]=(src)[5])
#define cpIpAddr(des,src)					((des)[0]=(src)[0],(des)[1]=(src)[1],(des)[2]=(src)[2],(des)[3]=(src)[3])

//
// ARP packet
//
// LLC Header
#define GET_ARP_PKT_LLC_TYPE(__pHeader) 					ReadEF2Byte( ((u8*)(__pHeader)) + 6)

//ARP element
#define GET_ARP_PKT_OPERATION(__pHeader) 				ReadEF2Byte( ((u8*)(__pHeader)) + 6)
#define GET_ARP_PKT_SENDER_MAC_ADDR(__pHeader, _val) 	cpMacAddr((u8*)(_val), ((u8*)(__pHeader))+8)
#define GET_ARP_PKT_SENDER_IP_ADDR(__pHeader, _val) 		cpIpAddr((u8*)(_val), ((u8*)(__pHeader))+14)
#define GET_ARP_PKT_TARGET_MAC_ADDR(__pHeader, _val) 	cpMacAddr((u8*)(_val), ((u8*)(__pHeader))+18)
#define GET_ARP_PKT_TARGET_IP_ADDR(__pHeader, _val) 	cpIpAddr((u8*)(_val), ((u8*)(__pHeader))+24)

#define SET_ARP_PKT_HW(__pHeader, __Value)  				WriteEF2Byte( ((u8*)(__pHeader)) + 0, __Value)
#define SET_ARP_PKT_PROTOCOL(__pHeader, __Value)  			WriteEF2Byte( ((u8*)(__pHeader)) + 2, __Value)
#define SET_ARP_PKT_HW_ADDR_LEN(__pHeader, __Value)  		WriteEF1Byte( ((u8*)(__pHeader)) + 4, __Value)
#define SET_ARP_PKT_PROTOCOL_ADDR_LEN(__pHeader, __Value)  	WriteEF1Byte( ((u8*)(__pHeader)) + 5, __Value)
#define SET_ARP_PKT_OPERATION(__pHeader, __Value) 		WriteEF2Byte( ((u8*)(__pHeader)) + 6, __Value)
#define SET_ARP_PKT_SENDER_MAC_ADDR(__pHeader, _val) 	cpMacAddr(((u8*)(__pHeader))+8, (u8*)(_val))
#define SET_ARP_PKT_SENDER_IP_ADDR(__pHeader, _val) 		cpIpAddr(((u8*)(__pHeader))+14, (u8*)(_val))
#define SET_ARP_PKT_TARGET_MAC_ADDR(__pHeader, _val) 	cpMacAddr(((u8*)(__pHeader))+18, (u8*)(_val))
#define SET_ARP_PKT_TARGET_IP_ADDR(__pHeader, _val) 		cpIpAddr(((u8*)(__pHeader))+24, (u8*)(_val))

#define FW_WOWLAN_FUN_EN				BIT(0)
#define FW_WOWLAN_PATTERN_MATCH			BIT(1)
#define FW_WOWLAN_MAGIC_PKT				BIT(2)
#define FW_WOWLAN_UNICAST				BIT(3)
#define FW_WOWLAN_ALL_PKT_DROP			BIT(4)
#define FW_WOWLAN_GPIO_ACTIVE			BIT(5)
#define FW_WOWLAN_REKEY_WAKEUP			BIT(6)
#define FW_WOWLAN_DEAUTH_WAKEUP			BIT(7)

#define FW_WOWLAN_GPIO_WAKEUP_EN		BIT(0)
#define FW_FW_PARSE_MAGIC_PKT			BIT(1)

#define FW_REMOTE_WAKE_CTRL_EN			BIT(0)
#define FW_REALWOWLAN_EN				BIT(5)

#endif //CONFIG_WOWLAN

//---------------------------------------------------------------------------------------------------------//
//----------------------------------    H2C CMD CONTENT    --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//
//_RSVDPAGE_LOC_CMD_0x00
#define SET_8721D_H2CCMD_RSVDPAGE_LOC_PROBE_RSP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSVDPAGE_LOC_PSPOLL(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSVDPAGE_LOC_NULL_DATA(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSVDPAGE_LOC_QOS_NULL_DATA(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSVDPAGE_LOC_BT_QOS_NULL_DATA(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 8, __Value)

//_MEDIA_STATUS_RPT_PARM_CMD_0x01
#define SET_8721D_H2CCMD_MSRRPT_PARM_OPMODE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_8721D_H2CCMD_MSRRPT_PARM_MACID_IND(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_8721D_H2CCMD_MSRRPT_PARM_MACID(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_MSRRPT_PARM_MACID_END(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 8, __Value)

//_KEEP_ALIVE_CMD_0x03
#define SET_8721D_H2CCMD_KEEPALIVE_PARM_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_8721D_H2CCMD_KEEPALIVE_PARM_ADOPT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_8721D_H2CCMD_KEEPALIVE_PARM_PKT_TYPE(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_8721D_H2CCMD_KEEPALIVE_PARM_CHECK_PERIOD(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)

//_DISCONNECT_DECISION_CMD_0x04
#define SET_8721D_H2CCMD_DISCONDECISION_PARM_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_8721D_H2CCMD_DISCONDECISION_PARM_ADOPT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_8721D_H2CCMD_DISCONDECISION_PARM_CHECK_PERIOD(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_DISCONDECISION_PARM_TRY_PKT_NUM(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 8, __Value)

// _PWR_MOD_CMD_0x20
#define SET_8721D_H2CCMD_PWRMODE_PARM_MODE(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_RLBM(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 4, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_SMART_PS(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 4, 4, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_BCN_PASS_TIME(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_ALL_QUEUE_UAPSD(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_PWR_STATE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_BYTE5(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_TDMA_SLOT_LEN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+7, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_TDMA_PERIOD_LEN_1(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+8, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_TDMA_PERIOD_LEN_2(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+9, 0, 8, __Value)
#define SET_8721D_H2CCMD_PWRMODE_PARM_TDMA_PERIOD_LEN_3(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+10, 0, 8, __Value)

#define GET_8721D_H2CCMD_PWRMODE_PARM_MODE(__pH2CCmd)					LE_BITS_TO_1BYTE(__pH2CCmd, 0, 8)

// _PS_TUNE_PARAM_CMD_0x21
#define SET_8721D_H2CCMD_PSTUNE_PARM_BCN_TO_LIMIT(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_DTIM_TIMEOUT(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_ADOPT(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 1, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_PS_TIMEOUT(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 1, 7, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_DTIM_PERIOD(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE(__pH2CCmd+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_NULL0_RETRY_LIMIT(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd+4, 0, 5, __Value)
#define SET_8721D_H2CCMD_PSTUNE_PARM_FIX_DELAY_FORWARD(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd+4, 5, 1, __Value)

//_MACID_CFG_CMD_0x40
#define SET_8721D_H2CCMD_MACID_CFG_MACID(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RAID(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 5, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RSSI_LEVEL(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 5, 2, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_SGI_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 7, 1, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_BW(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 2, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_NO_UPDATE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 3, 1, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_VHT_EN(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 4, 2, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_DISPT(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 6, 1, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_DISRA(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 7, 1, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RATE_MASK0(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RATE_MASK1(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+4, 0, 8, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RATE_MASK2(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+5, 0, 8, __Value)
#define SET_8721D_H2CCMD_MACID_CFG_RATE_MASK3(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+6, 0, 8, __Value)

//_RSSI_SETTING_CMD_0x42
#define SET_8721D_H2CCMD_RSSI_SETTING_MACID(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSSI_SETTING_RSSI(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 7, __Value)
#define SET_8721D_H2CCMD_RSSI_SETTING_ULDL_STATE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_RSSI_SETTING_RA_THRES_DIREC_OFFSET(__pH2CCmd, __Value)     SET_BITS_TO_LE_1BYTE(__pH2CCmd+4, 0, 8, __Value) 

// _AP_REQ_TXRPT_CMD_0x43
#define SET_8721D_H2CCMD_APREQRPT_PARM_MACID1(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_APREQRPT_PARM_MACID2(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)

// _FORCE_BT_TXPWR_CMD_0x62
#define SET_8721D_H2CCMD_BT_PWR_IDX(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE_8BIT(__pH2CCmd, 0, 8, __Value)

// _FORCE_BT_MP_OPER_CMD_0x67
#define SET_8721D_H2CCMD_BT_MPOPER_VER(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 4, __Value)
#define SET_8721D_H2CCMD_BT_MPOPER_REQNUM(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 4, __Value)
#define SET_8721D_H2CCMD_BT_MPOPER_IDX(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_BT_MPOPER_PARAM1(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 8, __Value)
#define SET_8721D_H2CCMD_BT_MPOPER_PARAM2(__pH2CCmd, __Value)							SET_BITS_TO_LE_1BYTE(__pH2CCmd+3, 0, 8, __Value)

// _BT_FW_PATCH_0x6A
#define SET_8721D_H2CCMD_BT_FW_PATCH_SIZE(__pH2CCmd, __Value)					SET_BITS_TO_LE_2BYTE((pu1Byte)(__pH2CCmd), 0, 16, __Value)
#define SET_8721D_H2CCMD_BT_FW_PATCH_ADDR0(__pH2CCmd, __Value) 					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value) 
#define SET_8721D_H2CCMD_BT_FW_PATCH_ADDR1(__pH2CCmd, __Value) 					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value) 
#define SET_8721D_H2CCMD_BT_FW_PATCH_ADDR2(__pH2CCmd, __Value) 					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 8, __Value) 
#define SET_8721D_H2CCMD_BT_FW_PATCH_ADDR3(__pH2CCmd, __Value) 					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 8, __Value)

// _WoWLAN PARAM_CMD_0x80
#define SET_8721D_H2CCMD_WOWLAN_FUNC_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_PATTERN_MATCH_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_MAGIC_PKT_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_UNICAST_PKT_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_ALL_PKT_DROP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_GPIO_ACTIVE(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE(__pH2CCmd, 5, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_REKEY_WAKE_UP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 6, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_DISCONNECT_WAKE_UP(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 7, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_GPIONUM(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 7, __Value)
#define SET_8721D_H2CCMD_WOWLAN_DATAPIN_WAKE_UP(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 7, 1, __Value)
#define SET_8721D_H2CCMD_WOWLAN_GPIO_DURATION(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value)

// _REMOTE_WAKEUP_CMD_0x81
#define SET_8721D_H2CCMD_REMOTE_WAKECTRL_ENABLE(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_8721D_H2CCMD_REMOTE_WAKE_CTRL_ARP_OFFLOAD_EN(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)
#define SET_8721D_H2CCMD_REMOTE_WAKE_CTRL_NDP_OFFLOAD_EN(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)
#define SET_8721D_H2CCMD_REMOTE_WAKE_CTRL_GTK_OFFLOAD_EN(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)
#define SET_8721D_H2CCMD_REMOTE_WAKE_CTRL_FW_UNICAST_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 7, 1, __Value)
#define SET_8721D_H2CCMD_REMOTE_WAKE_CTRL_ARP_ACTION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 1, __Value)

// AOAC_GLOBAL_INFO_0x82
#define SET_8721D_H2CCMD_AOAC_GLOBAL_INFO_PAIRWISE_ENC_ALG(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_GLOBAL_INFO_GROUP_ENC_ALG(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)

// AOAC_RSVDPAGE_LOC_0x83
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_REMOTE_WAKE_CTRL_INFO(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd), 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_ARP_RSP(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_NEIGHBOR_ADV(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_GTK_RSP(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_GTK_INFO(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_PROBE_REQ(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 8, __Value)
#define SET_8721D_H2CCMD_AOAC_RSVDPAGE_LOC_NETWORK_LIST(__pH2CCmd, __Value)					SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 0, 8, __Value)

//---------------------------------------------------------------------------------------------------------//
//-------------------------------------------    Structure    --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//
typedef struct _RSVDPAGE_LOC {
	u8 LocProbeRsp;
	u8 LocPsPoll;
	u8 LocNullData;
	u8 LocQosNull;
	u8 LocBTQosNull;
#ifdef CONFIG_WOWLAN
	u8 LocRemoteCtrlInfo;
	u8 LocArpRsp;
	u8 LocNbrAdv;
	u8 LocGTKRsp;
	u8 LocGTKInfo;
	u8 LocProbeReq;
	u8 LocNetList;
#endif //CONFIG_WOWLAN	
} RSVDPAGE_LOC, *PRSVDPAGE_LOC;


typedef struct _H2CParam_RsvdPage_ {
    RSVDPAGE_LOC    RsvdPageLoc;
    u8                    *ReservedPagePacket;
    u32                   TotalPacketLen;
} H2CParam_RsvdPage, *PH2CParam_RsvdPage;



//---------------------------------------------------------------------------------------------------------//
//----------------------------------    Function Statement     --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//

// host message to firmware cmd
void rtl8721d_set_FwPwrMode_cmd(PADAPTER padapter, u8 Mode);
void rtl8721d_set_FwJoinBssRpt_cmd(PADAPTER padapter, u8 mstatus);
#ifdef CONFIG_BT_COEXIST
void rtl8721d_set_BTCoex_AP_mode_FwRsvdPkt_cmd(PADAPTER padapter);
#endif
void rtl8721d_set_rssi_cmd(PADAPTER padapter, u8 *param);
void rtl8721d_Add_RateATid(PADAPTER pAdapter, u32 bitmap, u8* arg, u8 rssi_level);
//s32 rtl8721d_set_lowpwr_lps_cmd(PADAPTER padapter, u8 enable);
void rtl8721d_set_FwPsTuneParam_cmd(PADAPTER padapter);
void rtl8721d_set_FwMacIdConfig_cmd(_adapter* padapter, u8 mac_id, u8 raid, u8 bw, u8 sgi, u32 mask);
void rtl8721d_set_FwMediaStatusRpt_cmd(PADAPTER	padapter, u8 mstatus, u8 macid);
void rtl8721d_set_FwBtMpOper_cmd(PADAPTER padapter, u8 idx, u8 ver, u8 reqnum, u8 *param);
void rtl8721d_download_rsvd_page(PADAPTER padapter, u8 mstatus, u8 state);

void rtl8721d_set_FwPwrModeInIPS_cmd(PADAPTER padapter);

#ifdef CONFIG_TSF_RESET_OFFLOAD
u8 rtl8721d_reset_tsf(_adapter *padapter, u8 reset_port);
#endif	// CONFIG_TSF_RESET_OFFLOAD
s32 FillH2CCmd8721D(PADAPTER padapter, u8 ElementID, u32 CmdLen, u8 *pCmdBuffer);

#define FillH2CCmd FillH2CCmd8721D

#ifdef CONFIG_WOWLAN
u8 rtl8721d_set_keep_alive_cmd(_adapter *adapter, u8 enable, u8 pkt_type);
u8 rtl8721d_set_disconnect_decision_cmd(_adapter *adapter, u8 enable);
u8 rtl8721d_set_wowlan_ctrl_cmd(_adapter *adapter, u8 enable, u8 change_unit);
u8 rtl8721d_set_remote_wake_ctrl_cmd(_adapter *adapter, u8 enable);
u8 rtl8721d_set_global_info_cmd(_adapter* adapter, u8 group_alg, u8 pairwise_alg);
void rtl8721d_set_wowlan_cmd(_adapter* padapter, u8 enable);
void rtl8721d_dlps_backup(_adapter * padapter);
#endif
#endif
