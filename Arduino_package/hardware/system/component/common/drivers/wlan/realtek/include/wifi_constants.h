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
 ******************************************************************************
  * @file    wifi_constants.h
  * @author
  * @version
  * @brief   This file provides the data types used for wlan API.
  ******************************************************************************
  */

#ifndef _WIFI_CONSTANTS_H
#define _WIFI_CONSTANTS_H

/** @addtogroup nic NIC
 *  @ingroup    wlan
 *  @brief      NIC functions
 *  @{
 */

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef WLAN0_NAME
  #define WLAN0_NAME		"wlan0"
#endif
#ifndef WLAN1_NAME
  #define WLAN1_NAME		"wlan1"
#endif

#define WEP_ENABLED        0x0001
#define TKIP_ENABLED       0x0002
#define AES_ENABLED        0x0004
#define WSEC_SWFLAG        0x0008
#define AES_CMAC_ENABLED        0x0010
#define ENTERPRISE_ENABLED			0x0020

#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPA3_SECURITY		0x00800000
#define WPS_ENABLED     0x10000000

#define RTW_WPA2_MAX_PSK_LEN		(64)
#define RTW_WPA3_MAX_PSK_LEN		(128)
#define RTW_MAX_PSK_LEN		RTW_WPA3_MAX_PSK_LEN
#define RTW_MIN_PSK_LEN		(8)

#define MCSSET_LEN			16

/**
  * @}defgroup WLAN_Reason_Defs
  */
#define REASON_4WAY_HNDSHK_TIMEOUT               15     /**< Fourway handshake timeout */
#define REASON_AP_UNABLE_TO_HANDLE_NEW_STA       17     /**< AP denied due to max sta */
#define REASON_SAE_HASH_TO_ELEMENT               126    /**< Special status code to indicate H2E extension*/
#define REASON_SAE_CONFIRM_MISMATCH              65529  /**< SAE confirm mismatch */
#define REASON_SA_QUERY_TIMEOUT                  65530  /**< SA Query Timeout */
#define REASON_STA_IN_BLACKLIST                  65531  /**< STA gets blacklisted by AP */
#define REASON_AP_BEACON_CHANGED                 65534  /**< AP configuration changed causing beacon info changed */
#define REASON_EXPIRATION_CHK                    65535  /**< STA disconnected due to no beacon for a long time */

/**
  * @brief  The enumeration lists the results of the function.
  */
enum
{
    RTW_SUCCESS                      = 0,    /**< Success */
    RTW_PENDING                      = 1,    /**< Pending */
    RTW_TIMEOUT                      = 2,    /**< Timeout */
    RTW_PARTIAL_RESULTS              = 3,    /**< Partial results */
    RTW_INVALID_KEY                  = 4,    /**< Invalid key */
    RTW_DOES_NOT_EXIST               = 5,    /**< Does not exist */
    RTW_NOT_AUTHENTICATED            = 6,    /**< Not authenticated */
    RTW_NOT_KEYED                    = 7,    /**< Not keyed */
    RTW_IOCTL_FAIL                   = 8,    /**< IOCTL fail */
    RTW_BUFFER_UNAVAILABLE_TEMPORARY = 9,    /**< Buffer unavailable temporarily */
    RTW_BUFFER_UNAVAILABLE_PERMANENT = 10,   /**< Buffer unavailable permanently */
    RTW_WPS_PBC_OVERLAP              = 11,   /**< WPS PBC overlap */
    RTW_CONNECTION_LOST              = 12,   /**< Connection lost */

    RTW_ERROR                        = -1,   /**< Generic Error */
    RTW_BADARG                       = -2,   /**< Bad Argument */
    RTW_BADOPTION                    = -3,   /**< Bad option */
    RTW_NOTUP                        = -4,   /**< Not up */
    RTW_NOTDOWN                      = -5,   /**< Not down */
    RTW_NOTAP                        = -6,   /**< Not AP */
    RTW_NOTSTA                       = -7,   /**< Not STA  */
    RTW_BADKEYIDX                    = -8,   /**< BAD Key Index */
    RTW_RADIOOFF                     = -9,   /**< Radio Off */
    RTW_NOTBANDLOCKED                = -10,  /**< Not  band locked */
    RTW_NOCLK                        = -11,  /**< No Clock */
    RTW_BADRATESET                   = -12,  /**< BAD Rate valueset */
    RTW_BADBAND                      = -13,  /**< BAD Band */
    RTW_BUFTOOSHORT                  = -14,  /**< Buffer too short */
    RTW_BUFTOOLONG                   = -15,  /**< Buffer too long */
    RTW_BUSY                         = -16,  /**< Busy */
    RTW_NOTASSOCIATED                = -17,  /**< Not Associated */
    RTW_BADSSIDLEN                   = -18,  /**< Bad SSID len */
    RTW_OUTOFRANGECHAN               = -19,  /**< Out of Range Channel */
    RTW_BADCHAN                      = -20,  /**< Bad Channel */
    RTW_BADADDR                      = -21,  /**< Bad Address */
    RTW_NORESOURCE                   = -22,  /**< Not Enough Resources */
    RTW_UNSUPPORTED                  = -23,  /**< Unsupported */
    RTW_BADLEN                       = -24,  /**< Bad length */
    RTW_NOTREADY                     = -25,  /**< Not Ready */
    RTW_EPERM                        = -26,  /**< Not Permitted */
    RTW_NOMEM                        = -27,  /**< No Memory */
    RTW_ASSOCIATED                   = -28,  /**< Associated */
    RTW_RANGE                        = -29,  /**< Not In Range */
    RTW_NOTFOUND                     = -30,  /**< Not Found */
    RTW_WME_NOT_ENABLED              = -31,  /**< WME Not Enabled */
    RTW_TSPEC_NOTFOUND               = -32,  /**< TSPEC Not Found */
    RTW_ACM_NOTSUPPORTED             = -33,  /**< ACM Not Supported */
    RTW_NOT_WME_ASSOCIATION          = -34,  /**< Not WME Association */
    RTW_SDIO_ERROR                   = -35,  /**< SDIO Bus Error */
    RTW_WLAN_DOWN                    = -36,  /**< WLAN Not Accessible */
    RTW_BAD_VERSION                  = -37,  /**< Incorrect version */
    RTW_TXFAIL                       = -38,  /**< TX failure */
    RTW_RXFAIL                       = -39,  /**< RX failure */
    RTW_NODEVICE                     = -40,  /**< Device not present */
    RTW_UNFINISHED                   = -41,  /**< To be finished */
    RTW_NONRESIDENT                  = -42,  /**< access to nonresident overlay */
    RTW_DISABLED                     = -43   /**< Disabled in this build */
};
typedef unsigned long rtw_result_t;

/**
  * @brief  The enumeration lists the possible security types to set when connection.\n
  *			Station mode supports OPEN, WEP, and WPA2.\n
  *			AP mode support OPEN and WPA2.
  */
enum {
    RTW_SECURITY_OPEN           = 0,                                                /**< Open security                           */
    RTW_SECURITY_WEP_PSK        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
    RTW_SECURITY_WEP_SHARED     = ( WEP_ENABLED | SHARED_ENABLED ),                 /**< WEP Security with shared authentication */
    RTW_SECURITY_WPA_TKIP_PSK   = ( WPA_SECURITY  | TKIP_ENABLED ),                 /**< WPA Security with TKIP                  */
    RTW_SECURITY_WPA_AES_PSK    = ( WPA_SECURITY  | AES_ENABLED ),                  /**< WPA Security with AES                   */
    RTW_SECURITY_WPA_MIXED_PSK  = ( WPA_SECURITY | AES_ENABLED | TKIP_ENABLED ),    /**< WPA Security with AES & TKIP            */
    RTW_SECURITY_WPA2_AES_PSK   = ( WPA2_SECURITY | AES_ENABLED ),                  /**< WPA2 Security with AES                  */
    RTW_SECURITY_WPA2_TKIP_PSK  = ( WPA2_SECURITY | TKIP_ENABLED ),                 /**< WPA2 Security with TKIP                 */
    RTW_SECURITY_WPA2_MIXED_PSK = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED ),   /**< WPA2 Security with AES & TKIP           */
    RTW_SECURITY_WPA_WPA2_TKIP_PSK = ( WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED), /**< WPA/WPA2 Security with TKIP           */
    RTW_SECURITY_WPA_WPA2_AES_PSK = ( WPA_SECURITY  | WPA2_SECURITY | AES_ENABLED),   /**< WPA/WPA2 Security with AES            */
    RTW_SECURITY_WPA_WPA2_MIXED_PSK = ( WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED | AES_ENABLED), /**< WPA/WPA2 Security with AES & TKIP      */
    RTW_SECURITY_WPA2_AES_CMAC = ( WPA2_SECURITY | AES_CMAC_ENABLED),                /**< WPA2 Security with AES and Management Frame Protection */
	RTW_SECURITY_WPA_TKIP_ENTERPRISE	   = ( WPA_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED ),								 /**< WPA Security with TKIP via 802.1X authentication			  */
	RTW_SECURITY_WPA_AES_ENTERPRISE 	   = ( WPA_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED ),								 /**< WPA Security with AES via 802.1X authentication			  */
	RTW_SECURITY_WPA_MIXED_ENTERPRISE	   = ( WPA_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED ),				 /**< WPA Security with AES & TKIP via 802.1X authentication	  */
	RTW_SECURITY_WPA2_TKIP_ENTERPRISE	   = ( WPA2_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED ), 							 /**< WPA2 Security with TKI via 802.1X authenticationP 		  */
	RTW_SECURITY_WPA2_AES_ENTERPRISE	   = ( WPA2_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED ),								 /**< WPA2 Security with AES via 802.1X authentication			  */
	RTW_SECURITY_WPA2_MIXED_ENTERPRISE	   = ( WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED ),				 /**< WPA2 Security with AES & TKIP via 802.1X authentication	  */
	RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE  = ( WPA_SECURITY | WPA2_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED ),				 /**< WPA/WPA2 Security with TKIP via 802.1X authentication 	  */
	RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE   = ( WPA_SECURITY | WPA2_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED ),				 /**< WPA/WPA2 Security with AES via 802.1X authentication		  */
	RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE = ( WPA_SECURITY | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED ), /**< WPA/WPA2 Security with AES & TKIP via 802.1X authentication */

    RTW_SECURITY_WPS_OPEN       = ( WPS_ENABLED ),                                      /**< WPS with open security                  */
    RTW_SECURITY_WPS_SECURE     = ( WPS_ENABLED | AES_ENABLED ),                      /**< WPS with AES security                   */

    RTW_SECURITY_WPA3_AES_PSK 	           = ( WPA3_SECURITY | AES_ENABLED ),                                                    /**< WPA3-SAE with AES security                                  */
    RTW_SECURITY_WPA2_WPA3_MIXED           = ( WPA2_SECURITY | WPA3_SECURITY | AES_ENABLED ),                                    /**< WPA3-SAE/WPA2 with AES security                             */
    RTW_SECURITY_WPA3_ENTERPRISE           = ( WPA3_SECURITY | ENTERPRISE_ENABLED ),                                             /**< WPA3 Security via 802.1X authentication                     */

    RTW_SECURITY_UNKNOWN        = -1,                                               /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

    RTW_SECURITY_FORCE_32_BIT   = 0x7fffffff                                        /**< Exists only to force rtw_security_t type to 32 bits */
};
typedef unsigned long rtw_security_t;

enum
{
	WPA_AUTO_MODE,
	WPA_ONLY_MODE,
	WPA2_ONLY_MODE,
	WPA3_ONLY_MODE,
	WPA_WPA2_MIXED_MODE,
	WPA2_WPA3_MIXED_MODE
};
typedef unsigned long rtw_wpa_mode;

enum {
    RTW_ENCRYPTION_UNKNOWN = 0,                 
    RTW_ENCRYPTION_OPEN = 1,
    RTW_ENCRYPTION_WEP40 = 2,
    RTW_ENCRYPTION_WPA_TKIP = 3,
    RTW_ENCRYPTION_WPA_AES = 4,
    RTW_ENCRYPTION_WPA2_TKIP = 5,
    RTW_ENCRYPTION_WPA2_AES = 6,
    RTW_ENCRYPTION_WPA2_MIXED = 7,
    RTW_ENCRYPTION_WEP104 = 9,
    RTW_ENCRYPTION_UNDEF = 0xFF,                 
};
typedef unsigned long rtw_encryption_t;

enum {
	RTW_FALSE = 0,
	RTW_TRUE  = 1
};
typedef unsigned long rtw_bool_t;

/**
  * @brief  The enumeration lists the band types.
  */
enum {
	RTW_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
	RTW_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
};
typedef unsigned long rtw_802_11_band_t;

/**
  * @brief  The enumeration lists all the country codes able to set to Wi-Fi driver.
  */
enum {
	/* CHANNEL PLAN */
	RTW_COUNTRY_WORLD1,	// 0x20
	RTW_COUNTRY_ETSI1,	// 0x21
	RTW_COUNTRY_FCC1,	// 0x22
	RTW_COUNTRY_MKK1,	// 0x23
	RTW_COUNTRY_ETSI2,	// 0x24
	RTW_COUNTRY_FCC2,	// 0x2A
	RTW_COUNTRY_WORLD2,	// 0x47
	RTW_COUNTRY_MKK2,	// 0x58
	RTW_COUNTRY_GLOBAL,	// 0x41

	/* SPECIAL */
	RTW_COUNTRY_WORLD,	// WORLD1
	/*Country list*/
	RTW_COUNTRY_DZ,	// 0x00h

	RTW_COUNTRY_AU,	// 0x03h
	RTW_COUNTRY_NZ,	// 0x03h
	RTW_COUNTRY_CX,	// 0x03h
	RTW_COUNTRY_CC,	// 0x03h
	RTW_COUNTRY_HM,	// 0x03h
	RTW_COUNTRY_NU,	// 0x03h
	RTW_COUNTRY_NF,	// 0x03h
	RTW_COUNTRY_TK,	// 0x03h

	RTW_COUNTRY_TN,	// 0x04h

	RTW_COUNTRY_IQ,	// 0x05h

	RTW_COUNTRY_BH,	// 0x06h
	RTW_COUNTRY_CN,	// 0x06h
	RTW_COUNTRY_BN,	// 0x06h
	RTW_COUNTRY_NP,	// 0x06h

	RTW_COUNTRY_MY,	// 0x07h

	RTW_COUNTRY_BY,	// 0x08h

	RTW_COUNTRY_RU,	// 0x09h

	RTW_COUNTRY_GB,	// 0x0Bh

	RTW_COUNTRY_CA,	// 0x10h

	RTW_COUNTRY_BO,	// 0x11h

	RTW_COUNTRY_US,	// 0x1Bh

	RTW_COUNTRY_TV,	// 0x21h

	RTW_COUNTRY_AQ,	// 0x26h
	RTW_COUNTRY_VU,	// 0x26h

	RTW_COUNTRY_UY,	// 0x30h
	RTW_COUNTRY_VE,	// 0x30h

	RTW_COUNTRY_SN,	// 0x3Ah
	RTW_COUNTRY_ME,	// 0x3Ah
	RTW_COUNTRY_YE,	// 0x3Ah
	RTW_COUNTRY_AD,	// 0x3Ah
	RTW_COUNTRY_BJ,	// 0x3Ah
	RTW_COUNTRY_IO,	// 0x3Ah
	RTW_COUNTRY_MM,	// 0x3Ah
	RTW_COUNTRY_BI,	// 0x3Ah
	RTW_COUNTRY_CF,	// 0x3Ah
	RTW_COUNTRY_TD,	// 0x3Ah
	RTW_COUNTRY_ER,	// 0x3Ah
	RTW_COUNTRY_ET,	// 0x3Ah
	RTW_COUNTRY_GF,	// 0x3Ah
	RTW_COUNTRY_PF,	// 0x3Ah
	RTW_COUNTRY_TF,	// 0x3Ah
	RTW_COUNTRY_MQ,	// 0x3Ah
	RTW_COUNTRY_MS,	// 0x3Ah
	RTW_COUNTRY_NC,	// 0x3Ah
	RTW_COUNTRY_SM,	// 0x3Ah
	RTW_COUNTRY_SB,	// 0x3Ah
	RTW_COUNTRY_SO,	// 0x3Ah
	RTW_COUNTRY_GS,	// 0x3Ah
	RTW_COUNTRY_TO,	// 0x3Ah
	RTW_COUNTRY_TM,	// 0x3Ah
	RTW_COUNTRY_TC,	// 0x3Ah
	RTW_COUNTRY_UG,	// 0x3Ah
	RTW_COUNTRY_UZ,	// 0x3Ah
	RTW_COUNTRY_WF,	// 0x3Ah
	RTW_COUNTRY_IR,	// 0x3Ah

	RTW_COUNTRY_EG,	// 0x3Ch
	RTW_COUNTRY_MA,	// 0x3Ch
	RTW_COUNTRY_MV,	// 0x3Ch
	RTW_COUNTRY_EH,	// 0x3Ch

	RTW_COUNTRY_AF,	// 0x42h
	RTW_COUNTRY_CI,	// 0x42h

	RTW_COUNTRY_KR,	// 0x4Bh

	RTW_COUNTRY_AR,	// 0x4Dh
	RTW_COUNTRY_MX,	// 0x4Dh

	RTW_COUNTRY_ID,	// 0x5Dh

	RTW_COUNTRY_AT,	// 0x5Eh
	RTW_COUNTRY_BE,	// 0x5Eh
	RTW_COUNTRY_CY,	// 0x5Eh
	RTW_COUNTRY_EE,	// 0x5Eh
	RTW_COUNTRY_IE,	// 0x5Eh
	RTW_COUNTRY_LV,	// 0x5Eh
	RTW_COUNTRY_LI,	// 0x5Eh
	RTW_COUNTRY_LT,	// 0x5Eh
	RTW_COUNTRY_LU,	// 0x5Eh
	RTW_COUNTRY_MT,	// 0x5Eh
	RTW_COUNTRY_MC,	// 0x5Eh
	RTW_COUNTRY_PT,	// 0x5Eh
	RTW_COUNTRY_SK,	// 0x5Eh
	RTW_COUNTRY_SI,	// 0x5Eh
	RTW_COUNTRY_AL,	// 0x5Eh
	RTW_COUNTRY_AZ,	// 0x5Eh
	RTW_COUNTRY_BA,	// 0x5Eh
	RTW_COUNTRY_HR,	// 0x5Eh
	RTW_COUNTRY_GH,	// 0x5Eh
	RTW_COUNTRY_JO,	// 0x5Eh
	RTW_COUNTRY_KZ,	// 0x5Eh
	RTW_COUNTRY_KE,	// 0x5Eh
	RTW_COUNTRY_KW,	// 0x5Eh
	RTW_COUNTRY_KG,	// 0x5Eh
	RTW_COUNTRY_LB,	// 0x5Eh
	RTW_COUNTRY_LS,	// 0x5Eh
	RTW_COUNTRY_MK,	// 0x5Eh
	RTW_COUNTRY_MZ,	// 0x5Eh
	RTW_COUNTRY_NA,	// 0x5Eh
	RTW_COUNTRY_OM,	// 0x5Eh
	RTW_COUNTRY_QA,	// 0x5Eh
	RTW_COUNTRY_RO,	// 0x5Eh
	RTW_COUNTRY_AE,	// 0x5Eh
	RTW_COUNTRY_ZW,	// 0x5Eh
	RTW_COUNTRY_BD,	// 0x5Eh
	RTW_COUNTRY_KH,	// 0x5Eh
	RTW_COUNTRY_PK,	// 0x5Eh
	RTW_COUNTRY_PH,	// 0x5Eh
	RTW_COUNTRY_LK,	// 0x5Eh
	RTW_COUNTRY_VN,	// 0x5Eh
	RTW_COUNTRY_PG,	// 0x5Eh
	RTW_COUNTRY_AO,	// 0x5Eh
	RTW_COUNTRY_AI,	// 0x5Eh
	RTW_COUNTRY_AM,	// 0x5Eh
	RTW_COUNTRY_BT,	// 0x5Eh
	RTW_COUNTRY_BW,	// 0x5Eh
	RTW_COUNTRY_BV,	// 0x5Eh
	RTW_COUNTRY_BF,	// 0x5Eh
	RTW_COUNTRY_CM,	// 0x5Eh
	RTW_COUNTRY_CV,	// 0x5Eh
	RTW_COUNTRY_KM,	// 0x5Eh
	RTW_COUNTRY_CG,	// 0x5Eh
	RTW_COUNTRY_CD,	// 0x5Eh
	RTW_COUNTRY_CK,	// 0x5Eh
	RTW_COUNTRY_DJ,	// 0x5Eh
	RTW_COUNTRY_GQ,	// 0x5Eh
	RTW_COUNTRY_FK,	// 0x5Eh
	RTW_COUNTRY_FO,	// 0x5Eh
	RTW_COUNTRY_GA,	// 0x5Eh
	RTW_COUNTRY_GM,	// 0x5Eh
	RTW_COUNTRY_GE,	// 0x5Eh
	RTW_COUNTRY_GI,	// 0x5Eh
	RTW_COUNTRY_GL,	// 0x5Eh
	RTW_COUNTRY_GP,	// 0x5Eh
	RTW_COUNTRY_GG,	// 0x5Eh
	RTW_COUNTRY_GN,	// 0x5Eh
	RTW_COUNTRY_GW,	// 0x5Eh
	RTW_COUNTRY_VA,	// 0x5Eh
	RTW_COUNTRY_IM,	// 0x5Eh
	RTW_COUNTRY_JE,	// 0x5Eh
	RTW_COUNTRY_KI,	// 0x5Eh
	RTW_COUNTRY_XK,	// 0x5Eh
	RTW_COUNTRY_LA,	// 0x5Eh
	RTW_COUNTRY_LR,	// 0x5Eh
	RTW_COUNTRY_LY,	// 0x5Eh
	RTW_COUNTRY_MO,	// 0x5Eh
	RTW_COUNTRY_MG,	// 0x5Eh
	RTW_COUNTRY_MW,	// 0x5Eh
	RTW_COUNTRY_ML,	// 0x5Eh
	RTW_COUNTRY_MR,	// 0x5Eh
	RTW_COUNTRY_MU,	// 0x5Eh
	RTW_COUNTRY_YT,	// 0x5Eh
	RTW_COUNTRY_MD,	// 0x5Eh
	RTW_COUNTRY_MN,	// 0x5Eh
	RTW_COUNTRY_NR,	// 0x5Eh
	RTW_COUNTRY_NE,	// 0x5Eh
	RTW_COUNTRY_RE,	// 0x5Eh
	RTW_COUNTRY_RW,	// 0x5Eh
	RTW_COUNTRY_SH,	// 0x5Eh
	RTW_COUNTRY_PM,	// 0x5Eh
	RTW_COUNTRY_ST,	// 0x5Eh
	RTW_COUNTRY_SL,	// 0x5Eh
	RTW_COUNTRY_SJ,	// 0x5Eh
	RTW_COUNTRY_SZ,	// 0x5Eh
	RTW_COUNTRY_TJ,	// 0x5Eh
	RTW_COUNTRY_TZ,	// 0x5Eh
	RTW_COUNTRY_TG,	// 0x5Eh
	RTW_COUNTRY_ZM,	// 0x5Eh
// Belongs to CE (BG to ZA) which enables adaptivity	
	RTW_COUNTRY_BG,	// 0x5Eh
	RTW_COUNTRY_CH,	// 0x5Eh
	RTW_COUNTRY_CZ,	// 0x5Eh
	RTW_COUNTRY_DE,	// 0x5Eh
	RTW_COUNTRY_DK,	// 0x5Eh
	RTW_COUNTRY_ES,	// 0x5Eh
	RTW_COUNTRY_FI,	// 0x5Eh
	RTW_COUNTRY_FR,	// 0x5Eh
	RTW_COUNTRY_GR,	// 0x5Eh
	RTW_COUNTRY_HK,	// 0x5Eh
	RTW_COUNTRY_HU,	// 0x5Eh
	RTW_COUNTRY_IL,	// 0x5Eh
	RTW_COUNTRY_IN,	// 0x5Eh
	RTW_COUNTRY_IS,	// 0x5Eh
	RTW_COUNTRY_IT,	// 0x5Eh
	RTW_COUNTRY_NL,	// 0x5Eh
	RTW_COUNTRY_NO,	// 0x5Eh
	RTW_COUNTRY_PL,	// 0x5Eh
	RTW_COUNTRY_RS,	// 0x5Eh
	RTW_COUNTRY_SA,	// 0x5Eh
	RTW_COUNTRY_SE,	// 0x5Eh
	RTW_COUNTRY_SG,	// 0x5Eh
	RTW_COUNTRY_TH,	// 0x5Eh
	RTW_COUNTRY_TR,	// 0x5Eh
	RTW_COUNTRY_UA,	// 0x5Eh
	RTW_COUNTRY_ZA,	// 0x5Eh
	RTW_COUNTRY_PS,	// 0x5Eh

	RTW_COUNTRY_BR,	// 0x62h

	RTW_COUNTRY_SR,	// 0x74h

	RTW_COUNTRY_NG,	// 0x75h

	RTW_COUNTRY_CL,	// 0x76h
	RTW_COUNTRY_CO,	// 0x76h
	RTW_COUNTRY_CR,	// 0x76h
	RTW_COUNTRY_EC,	// 0x76h
	RTW_COUNTRY_SV,	// 0x76h
	RTW_COUNTRY_GT,	// 0x76h
	RTW_COUNTRY_HN,	// 0x76h
	RTW_COUNTRY_NI,	// 0x76h
	RTW_COUNTRY_PA,	// 0x76h
	RTW_COUNTRY_PY,	// 0x76h
	RTW_COUNTRY_PE,	// 0x76h
	RTW_COUNTRY_PR,	// 0x76h
	RTW_COUNTRY_DO,	// 0x76h
	RTW_COUNTRY_TW,	// 0x76h
	RTW_COUNTRY_JM,	// 0x76h
	RTW_COUNTRY_AN,	// 0x76h
	RTW_COUNTRY_TT,	// 0x76h
	RTW_COUNTRY_AS,	// 0x76h
	RTW_COUNTRY_AG,	// 0x76h
	RTW_COUNTRY_AW,	// 0x76h
	RTW_COUNTRY_BS,	// 0x76h
	RTW_COUNTRY_BB,	// 0x76h
	RTW_COUNTRY_BZ,	// 0x76h
	RTW_COUNTRY_BM,	// 0x76h
	RTW_COUNTRY_VG,	// 0x76h
	RTW_COUNTRY_KY,	// 0x76h
	RTW_COUNTRY_DM,	// 0x76h
	RTW_COUNTRY_FJ,	// 0x76h
	RTW_COUNTRY_GD,	// 0x76h
	RTW_COUNTRY_GU,	// 0x76h
	RTW_COUNTRY_GY,	// 0x76h
	RTW_COUNTRY_HT,	// 0x76h
	RTW_COUNTRY_MH,	// 0x76h
	RTW_COUNTRY_FM,	// 0x76h
	RTW_COUNTRY_MP,	// 0x76h
	RTW_COUNTRY_PW,	// 0x76h
	RTW_COUNTRY_KN,	// 0x76h
	RTW_COUNTRY_LC,	// 0x76h
	RTW_COUNTRY_MF,	// 0x76h
	RTW_COUNTRY_SX,	// 0x76h
	RTW_COUNTRY_VC,	// 0x76h
	RTW_COUNTRY_WS,	// 0x76h
	RTW_COUNTRY_SC,	// 0x76h
	RTW_COUNTRY_VI,	// 0x76h

	RTW_COUNTRY_JP,	// 0x7Dh
	RTW_COUNTRY_RTK_DEFAULT,	// 0x7Fh

	RTW_COUNTRY_EU,	// EU Operating Class

	RTW_COUNTRY_MAX

};
typedef unsigned long rtw_country_code_t;

/**
  * @brief  The country_map maps the 2 characters country code from beacon to RTW_COUNTRY_xx
  */
typedef struct {
    const char *country_code;
    rtw_country_code_t rtw_country_code;
} CountryMap;

static CountryMap country_map[] = {
    {"DZ", RTW_COUNTRY_DZ},
    {"AU", RTW_COUNTRY_AU},
    {"NZ", RTW_COUNTRY_NZ},
    {"CX", RTW_COUNTRY_CX},
    {"CC", RTW_COUNTRY_CC},
    {"HM", RTW_COUNTRY_HM},
    {"NU", RTW_COUNTRY_NU},
    {"NF", RTW_COUNTRY_NF},
    {"TK", RTW_COUNTRY_TK},
    {"TN", RTW_COUNTRY_TN},
    {"IQ", RTW_COUNTRY_IQ},
    {"BH", RTW_COUNTRY_BH},
    {"CN", RTW_COUNTRY_CN},
    {"BN", RTW_COUNTRY_BN},
    {"NP", RTW_COUNTRY_NP},
    {"MY", RTW_COUNTRY_MY},
    {"BY", RTW_COUNTRY_BY},
    {"RU", RTW_COUNTRY_RU},
    {"GB", RTW_COUNTRY_GB},
    {"CA", RTW_COUNTRY_CA},
    {"BO", RTW_COUNTRY_BO},
    {"US", RTW_COUNTRY_US},
    {"TV", RTW_COUNTRY_TV},
    {"AQ", RTW_COUNTRY_AQ},
    {"VU", RTW_COUNTRY_VU},
    {"UY", RTW_COUNTRY_UY},
    {"VE", RTW_COUNTRY_VE},
    {"SN", RTW_COUNTRY_SN},
    {"ME", RTW_COUNTRY_ME},
    {"YE", RTW_COUNTRY_YE},
    {"AD", RTW_COUNTRY_AD},
    {"BJ", RTW_COUNTRY_BJ},
    {"IO", RTW_COUNTRY_IO},
    {"MM", RTW_COUNTRY_MM},
    {"BI", RTW_COUNTRY_BI},
    {"CF", RTW_COUNTRY_CF},
    {"TD", RTW_COUNTRY_TD},
    {"ER", RTW_COUNTRY_ER},
    {"ET", RTW_COUNTRY_ET},
    {"GF", RTW_COUNTRY_GF},
    {"PF", RTW_COUNTRY_PF},
    {"TF", RTW_COUNTRY_TF},
    {"MQ", RTW_COUNTRY_MQ},
    {"MS", RTW_COUNTRY_MS},
    {"NC", RTW_COUNTRY_NC},
    {"SM", RTW_COUNTRY_SM},
    {"SB", RTW_COUNTRY_SB},
    {"SO", RTW_COUNTRY_SO},
    {"GS", RTW_COUNTRY_GS},
    {"TO", RTW_COUNTRY_TO},
    {"TM", RTW_COUNTRY_TM},
    {"TC", RTW_COUNTRY_TC},
    {"UG", RTW_COUNTRY_UG},
    {"UZ", RTW_COUNTRY_UZ},
    {"WF", RTW_COUNTRY_WF},
    {"IR", RTW_COUNTRY_IR},
    {"EG", RTW_COUNTRY_EG},
    {"MA", RTW_COUNTRY_MA},
    {"MV", RTW_COUNTRY_MV},
    {"EH", RTW_COUNTRY_EH},
    {"AF", RTW_COUNTRY_AF},
    {"CI", RTW_COUNTRY_CI},
    {"KR", RTW_COUNTRY_KR},
    {"AR", RTW_COUNTRY_AR},
    {"MX", RTW_COUNTRY_MX},
    {"ID", RTW_COUNTRY_ID},
    {"AT", RTW_COUNTRY_AT},
    {"BE", RTW_COUNTRY_BE},
    {"CY", RTW_COUNTRY_CY},
    {"CZ", RTW_COUNTRY_CZ},
    {"DK", RTW_COUNTRY_DK},
    {"EE", RTW_COUNTRY_EE},
    {"FI", RTW_COUNTRY_FI},
    {"FR", RTW_COUNTRY_FR},
    {"DE", RTW_COUNTRY_DE},
    {"GR", RTW_COUNTRY_GR},
    {"HU", RTW_COUNTRY_HU},
    {"IS", RTW_COUNTRY_IS},
    {"IE", RTW_COUNTRY_IE},
    {"IT", RTW_COUNTRY_IT},
    {"LV", RTW_COUNTRY_LV},
    {"LI", RTW_COUNTRY_LI},
    {"LT", RTW_COUNTRY_LT},
    {"LU", RTW_COUNTRY_LU},
    {"MT", RTW_COUNTRY_MT},
    {"MC", RTW_COUNTRY_MC},
    {"NL", RTW_COUNTRY_NL},
    {"NO", RTW_COUNTRY_NO},
    {"PL", RTW_COUNTRY_PL},
    {"PT", RTW_COUNTRY_PT},
    {"SK", RTW_COUNTRY_SK},
    {"SI", RTW_COUNTRY_SI},
    {"ES", RTW_COUNTRY_ES},
    {"SE", RTW_COUNTRY_SE},
    {"CH", RTW_COUNTRY_CH},
    {"AL", RTW_COUNTRY_AL},
    {"AZ", RTW_COUNTRY_AZ},
    {"BA", RTW_COUNTRY_BA},
    {"BG", RTW_COUNTRY_BG},
    {"HR", RTW_COUNTRY_HR},
    {"GH", RTW_COUNTRY_GH},
    {"IL", RTW_COUNTRY_IL},
    {"JO", RTW_COUNTRY_JO},
    {"KZ", RTW_COUNTRY_KZ},
    {"KE", RTW_COUNTRY_KE},
    {"KW", RTW_COUNTRY_KW},
    {"KG", RTW_COUNTRY_KG},
    {"LB", RTW_COUNTRY_LB},
    {"LS", RTW_COUNTRY_LS},
    {"MK", RTW_COUNTRY_MK},
    {"MZ", RTW_COUNTRY_MZ},
    {"NA", RTW_COUNTRY_NA},
    {"OM", RTW_COUNTRY_OM},
    {"QA", RTW_COUNTRY_QA},
    {"RO", RTW_COUNTRY_RO},
    {"SA", RTW_COUNTRY_SA},
    {"RS", RTW_COUNTRY_RS},
    {"ZA", RTW_COUNTRY_ZA},
    {"TR", RTW_COUNTRY_TR},
    {"UA", RTW_COUNTRY_UA},
    {"AE", RTW_COUNTRY_AE},
    {"ZW", RTW_COUNTRY_ZW},
    {"BD", RTW_COUNTRY_BD},
    {"KH", RTW_COUNTRY_KH},
    {"HK", RTW_COUNTRY_HK},
    {"IN", RTW_COUNTRY_IN},
    {"PK", RTW_COUNTRY_PK},
    {"PH", RTW_COUNTRY_PH},
    {"SG", RTW_COUNTRY_SG},
    {"LK", RTW_COUNTRY_LK},
    {"TH", RTW_COUNTRY_TH},
    {"VN", RTW_COUNTRY_VN},
    {"PG", RTW_COUNTRY_PG},
    {"AO", RTW_COUNTRY_AO},
    {"AI", RTW_COUNTRY_AI},
    {"AM", RTW_COUNTRY_AM},
    {"BT", RTW_COUNTRY_BT},
    {"BW", RTW_COUNTRY_BW},
    {"BV", RTW_COUNTRY_BV},
    {"BF", RTW_COUNTRY_BF},
    {"CM", RTW_COUNTRY_CM},
    {"CV", RTW_COUNTRY_CV},
    {"KM", RTW_COUNTRY_KM},
    {"CG", RTW_COUNTRY_CG},
    {"CD", RTW_COUNTRY_CD},
    {"CK", RTW_COUNTRY_CK},
    {"DJ", RTW_COUNTRY_DJ},
    {"GQ", RTW_COUNTRY_GQ},
    {"FK", RTW_COUNTRY_FK},
    {"FO", RTW_COUNTRY_FO},
    {"GA", RTW_COUNTRY_GA},
    {"GM", RTW_COUNTRY_GM},
    {"GE", RTW_COUNTRY_GE},
    {"GI", RTW_COUNTRY_GI},
    {"GL", RTW_COUNTRY_GL},
    {"GP", RTW_COUNTRY_GP},
    {"GG", RTW_COUNTRY_GG},
    {"GN", RTW_COUNTRY_GN},
    {"GW", RTW_COUNTRY_GW},
    {"VA", RTW_COUNTRY_VA},
    {"IM", RTW_COUNTRY_IM},
    {"JE", RTW_COUNTRY_JE},
    {"KI", RTW_COUNTRY_KI},
    {"XK", RTW_COUNTRY_XK},
    {"LA", RTW_COUNTRY_LA},
    {"LR", RTW_COUNTRY_LR},
    {"LY", RTW_COUNTRY_LY},
    {"MO", RTW_COUNTRY_MO},
    {"MG", RTW_COUNTRY_MG},
    {"MW", RTW_COUNTRY_MW},
    {"ML", RTW_COUNTRY_ML},
    {"MR", RTW_COUNTRY_MR},
    {"MU", RTW_COUNTRY_MU},
    {"YT", RTW_COUNTRY_YT},
    {"MD", RTW_COUNTRY_MD},
    {"MN", RTW_COUNTRY_MN},
    {"NR", RTW_COUNTRY_NR},
    {"NE", RTW_COUNTRY_NE},
    {"RE", RTW_COUNTRY_RE},
    {"RW", RTW_COUNTRY_RW},
    {"SH", RTW_COUNTRY_SH},
    {"PM", RTW_COUNTRY_PM},
    {"ST", RTW_COUNTRY_ST},
    {"SL", RTW_COUNTRY_SL},
    {"SJ", RTW_COUNTRY_SJ},
    {"SZ", RTW_COUNTRY_SZ},
    {"TJ", RTW_COUNTRY_TJ},
    {"TZ", RTW_COUNTRY_TZ},
    {"TG", RTW_COUNTRY_TG},
    {"ZM", RTW_COUNTRY_ZM},
    {"PS", RTW_COUNTRY_PS},
    {"BR", RTW_COUNTRY_BR},
    {"SR", RTW_COUNTRY_SR},
    {"NG", RTW_COUNTRY_NG},
    {"CL", RTW_COUNTRY_CL},
    {"CO", RTW_COUNTRY_CO},
    {"CR", RTW_COUNTRY_CR},
    {"EC", RTW_COUNTRY_EC},
    {"SV", RTW_COUNTRY_SV},
    {"GT", RTW_COUNTRY_GT},
    {"HN", RTW_COUNTRY_HN},
    {"NI", RTW_COUNTRY_NI},
    {"PA", RTW_COUNTRY_PA},
    {"PY", RTW_COUNTRY_PY},
    {"PE", RTW_COUNTRY_PE},
    {"PR", RTW_COUNTRY_PR},
    {"DO", RTW_COUNTRY_DO},
    {"TW", RTW_COUNTRY_TW},
    {"JM", RTW_COUNTRY_JM},
    {"AN", RTW_COUNTRY_AN},
    {"TT", RTW_COUNTRY_TT},
    {"AS", RTW_COUNTRY_AS},
    {"AG", RTW_COUNTRY_AG},
    {"AW", RTW_COUNTRY_AW},
    {"BS", RTW_COUNTRY_BS},
    {"BB", RTW_COUNTRY_BB},
    {"BZ", RTW_COUNTRY_BZ},
    {"BM", RTW_COUNTRY_BM},
    {"VG", RTW_COUNTRY_VG},
    {"KY", RTW_COUNTRY_KY},
    {"DM", RTW_COUNTRY_DM},
    {"FJ", RTW_COUNTRY_FJ},
    {"GD", RTW_COUNTRY_GD},
    {"GU", RTW_COUNTRY_GU},
    {"GY", RTW_COUNTRY_GY},
    {"HT", RTW_COUNTRY_HT},
    {"MH", RTW_COUNTRY_MH},
    {"FM", RTW_COUNTRY_FM},
    {"MP", RTW_COUNTRY_MP},
    {"PW", RTW_COUNTRY_PW},
    {"KN", RTW_COUNTRY_KN},
    {"LC", RTW_COUNTRY_LC},
    {"MF", RTW_COUNTRY_MF},
    {"SX", RTW_COUNTRY_SX},
    {"VC", RTW_COUNTRY_VC},
    {"WS", RTW_COUNTRY_WS},
    {"SC", RTW_COUNTRY_SC},
    {"VI", RTW_COUNTRY_VI},
    {"JP", RTW_COUNTRY_JP}
};

/**
  * @brief  The enumeration lists the adaptivity types.
  */
enum {
	RTW_ADAPTIVITY_DISABLE = 0,
	RTW_ADAPTIVITY_NORMAL,			// CE
	RTW_ADAPTIVITY_CARRIER_SENSE	// MKK
};
typedef unsigned long rtw_adaptivity_mode_t;

/**
  * @brief  The enumeration lists the trp_tis types.
  */
enum {
	RTW_TRP_TIS_DISABLE = 0,
	RTW_TRP_TIS_NORMAL,
	RTW_TRP_TIS_DYNAMIC,				// enable dynamic mechanism
	RTW_TRP_TIS_FIX_ACK_RATE,			// fix ack rate to 6M
};
typedef unsigned long rtw_trp_tis_mode_t;


/**
  * @brief  The enumeration lists the supported operation mode by WIFI driver,
  *			including station and AP mode.
  */
enum {
	RTW_MODE_NONE = 0,
	RTW_MODE_STA,
	RTW_MODE_AP,
	RTW_MODE_STA_AP,
	RTW_MODE_PROMISC,
	RTW_MODE_P2P
};
typedef unsigned long rtw_mode_t;

enum {
	RTW_SCAN_FULL = 0,
	RTW_SCAN_SOCIAL,
	RTW_SCAN_ONE
};
typedef unsigned long rtw_scan_mode_t;

/**
  * @brief  The enumeration lists the supported autoreconnect mode by WIFI driver.
  */
typedef enum{
    RTW_AUTORECONNECT_DISABLE,
    RTW_AUTORECONNECT_FINITE,
    RTW_AUTORECONNECT_INFINITE
} rtw_autoreconnect_mode_t;

/**
  * @brief  The enumeration lists the status to describe the connection link.
  */
enum {
	RTW_LINK_DISCONNECTED = 0,
	RTW_LINK_CONNECTED
};
typedef unsigned long rtw_link_status_t;

/**
  * @brief  The enumeration lists the scan types.
  */
enum {
    RTW_SCAN_TYPE_ACTIVE              = 0x00,  /**< Actively scan a network by sending 802.11 probe(s)         */
    RTW_SCAN_TYPE_PASSIVE             = 0x01,  /**< Passively scan a network by listening for beacons from APs */
    RTW_SCAN_TYPE_PROHIBITED_CHANNELS = 0x04   /**< Passively scan on channels not enabled by the country code */
};
typedef unsigned long rtw_scan_type_t;

/**
  * @brief  The enumeration lists the bss types.
  */
enum {
    RTW_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    RTW_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    RTW_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */

    RTW_BSS_TYPE_UNKNOWN        = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
};
typedef unsigned long rtw_bss_type_t;

enum {
	RTW_SCAN_COMMAMD = 0x01
};
typedef unsigned long rtw_scan_command_t;

enum{
	COMMAND1					= 0x01
};
typedef unsigned long rtw_command_type;

enum {
	RTW_WPS_TYPE_DEFAULT 		    	= 0x0000,
	RTW_WPS_TYPE_USER_SPECIFIED 		= 0x0001,
	RTW_WPS_TYPE_MACHINE_SPECIFIED   	= 0x0002,
	RTW_WPS_TYPE_REKEY 			        = 0x0003,
	RTW_WPS_TYPE_PUSHBUTTON 		    = 0x0004,
	RTW_WPS_TYPE_REGISTRAR_SPECIFIED 	= 0x0005,
    RTW_WPS_TYPE_NONE                   = 0x0006,
    RTW_WPS_TYPE_WSC                    = 0x0007
};
typedef unsigned long rtw_wps_type_t;

/**
  * @brief  The enumeration lists all the network bgn mode.
  */
enum {
    RTW_NETWORK_B   = 1,
	RTW_NETWORK_BG  = 3,
	RTW_NETWORK_BGN = 11,
	RTW_NETWORK_ABGN = 31
};
typedef unsigned long rtw_network_mode_t;

/**
  * @brief  The enumeration lists the interfaces.
  */
enum {
    RTW_STA_INTERFACE     = 0, /**< STA or Client Interface  */
    RTW_AP_INTERFACE      = 1, /**< SoftAP Interface         */
};
typedef unsigned long rtw_interface_t;

/**
  * @brief  The enumeration lists the packet filter rules.
  */
enum {
	RTW_POSITIVE_MATCHING  = 0, /**< Receive the data matching with this pattern and discard the other data   */
	RTW_NEGATIVE_MATCHING  = 1  /**< Discard the data matching with this pattern and receive the other data */
};
typedef unsigned long rtw_packet_filter_rule_t;

/**
  * @brief  The enumeration lists the promisc levels.
  */
enum {
	RTW_PROMISC_DISABLE = 0,  /**< Disable the promisc */
	RTW_PROMISC_ENABLE = 1,   /**< Fetch all ethernet packets */
	RTW_PROMISC_ENABLE_1 = 2, /**< Fetch only B/M packets */
	RTW_PROMISC_ENABLE_2 = 3, /**< Fetch all 802.11 packets*/
	RTW_PROMISC_ENABLE_3 = 4, /**< Fetch only B/M 802.11 packets*/
	RTW_PROMISC_ENABLE_4 = 5, /**< Fetch all 802.11 packets & MIMO PLCP headers. Please note that the PLCP header would be struct rtw_rx_info_t defined in wifi_structures.h*/
};
typedef unsigned long rtw_rcr_level_t;

/**
  * @brief  The enumeration lists the promisc rx type.
  */
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
enum {
	RTW_RX_NORMAL = 0,  /**< The supported 802.11 packet*/
	RTW_RX_UNSUPPORT = 1,  /**<  Unsupported 802.11 packet info */
};
typedef unsigned long rtw_rx_type_t;
#endif
/**
  * @brief  The enumeration lists the disconnect reasons.
  */
enum{
	RTW_NO_ERROR = 0,               /**< no error */
	RTW_NONE_NETWORK = 1,           /**< none network */
	RTW_CONNECT_FAIL = 2,           /**< connect fail */
	RTW_WRONG_PASSWORD = 3,         /**< wrong password */
	RTW_4WAY_HANDSHAKE_TIMEOUT = 4, /**< 4 way handshake timeout*/
	RTW_DHCP_FAIL = 5,              /**< dhcp fail*/
	RTW_AUTH_FAIL = 6,              /**< auth fail */
	RTW_ASSOC_FAIL = 7,             /**< association fail */
	RTW_DEAUTH_DEASSOC = 8,         /**< deauth or deassoc */
	RTW_UNKNOWN,
};
typedef unsigned long rtw_connect_error_flag_t;

enum {
	RTW_TX_PWR_PERCENTAGE_100 = 0, /* 100%, default target output power.	 */
	RTW_TX_PWR_PERCENTAGE_75 = 1, /* 75% */
	RTW_TX_PWR_PERCENTAGE_50 = 2, /* 50% */
	RTW_TX_PWR_PERCENTAGE_25 = 3, /* 25% */
	RTW_TX_PWR_PERCENTAGE_12_5 = 4, /* 12.5% */
};
typedef unsigned long rtw_tx_pwr_percentage_t;

/**
  * @brief  The enumeration is event type indicated from wlan driver.
  */
enum _WIFI_EVENT_INDICATE{
	WIFI_EVENT_CONNECT = 0,
	WIFI_EVENT_DISCONNECT = 1,
	WIFI_EVENT_FOURWAY_HANDSHAKE_DONE = 2,	
	WIFI_EVENT_SCAN_RESULT_REPORT = 3,
	WIFI_EVENT_SCAN_DONE = 4,
	WIFI_EVENT_RECONNECTION_FAIL = 5,
	WIFI_EVENT_SEND_ACTION_DONE = 6,
	WIFI_EVENT_RX_MGNT = 7,
	WIFI_EVENT_STA_ASSOC = 8,
	WIFI_EVENT_STA_DISASSOC = 9,
	WIFI_EVENT_STA_WPS_START = 10,
	WIFI_EVENT_WPS_FINISH = 11,
	WIFI_EVENT_EAPOL_START = 12,
	WIFI_EVENT_EAPOL_RECVD = 13,
	WIFI_EVENT_NO_NETWORK = 14,
	WIFI_EVENT_BEACON_AFTER_DHCP = 15,
	WIFI_EVENT_IP_CHANGED = 16,
	WIFI_EVENT_ICV_ERROR = 17,
	WIFI_EVENT_CHALLENGE_FAIL = 18,
	WIFI_EVENT_SOFTAP_START = 19,
	WIFI_EVENT_SOFTAP_STOP = 20,
	WIFI_EVENT_DHCP6_DONE = 21,
	WIFI_EVENT_MAX,
};
typedef unsigned long rtw_event_indicate_t;
#ifdef	__cplusplus
}
#endif

/*\@}*/

#endif /* _WIFI_CONSTANTS_H */
