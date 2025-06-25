/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved. 
  *
******************************************************************************/

#ifndef _WIRELESS_H
#define _WIRELESS_H

/***************************** INCLUDES *****************************/


//#include <sockets.h>
#define IFNAMSIZ	16
#define	ARPHRD_ETHER	1	/* ethernet hardware format */


#define WIRELESS_EXT	22



/**************************** CONSTANTS ****************************/
typedef unsigned char __u8;
typedef char __s8;
typedef unsigned short __u16;
typedef short __s16;
typedef unsigned int __u32;
typedef int __s32;
typedef	unsigned long long __u64;
typedef	long long __i64;

#define	E2BIG		 7	/* Argument list too long */

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */

 
#define RTKIOCDEVPRIVATE	0x89F0	/* to 89FF */


#define RTKIOCSIWFREQ	0x8B04		/* set channel/frequency (Hz) */
#define RTKIOCGIWFREQ	0x8B05		/* get channel/frequency (Hz) */
#define RTKIOCSIWMODE	0x8B06		/* set operation mode */
#define RTKIOCGIWMODE	0x8B07		/* get operation mode */
#define RTKIOCGIWSENS	0x8B09		/* get sensitivity (dBm) */

#define RTKIOCSIWAP		0x8B14		/* set access point MAC addresses */
#define RTKIOCGIWAP		0x8B15		/* get access point MAC addresses */
#define RTKIOCSIWSCAN	0x8B18		/* trigger scanning (list cells) */
#define RTKIOCGIWSCAN	0x8B19		/* get scanning results */

#define RTKIOCSIWESSID	0x8B1A		/* set ESSID (network name) */
#define RTKIOCGIWESSID	0x8B1B		/* get ESSID */

#define RTKIOCSIWGENIE		0x8B30		/* set generic IE */

#define RTKIOCSIWAUTH		0x8B32		/* WPA : set authentication mode params */
#define RTKIOCGIWAUTH		0x8B33		/* get authentication mode params */

#define RTKIOCSIWENCODEEXT 	0x8B34		/* WPA : set encoding token & mode */
#define RTKIOCGIWENCODEEXT 	0x8B35		/* WPA : get encoding token & mode */

#define RTKIOCSIWPMKSA		0x8B36		/* WPA2/WPA3 : PMKSA cache operation */


#define RTKIOCSIWMGNTSEND	0x8B37		/* Send Mgnt Frame or Action Frame */

#define RTKIOCSIWEAPOLSEND	0x8B38		/* Send WPS EAPOL Frame */

#define RTKIOCSIMAILBOX		0x8B39		/* Set MailBox Info */

#define RTKIOCSIWMFP		0x8B3A		/* Set Management Frame Protection Support */

#define RTKIOCSIWGRPID		0x8B3B		/* Set Finite cyclic groups id for SAE  */

#define RTKIOCGIWBCNSENS	0x8B40 		/* Get beacon average rssi */


#define RTKIOCIWFIRSTPRIV	0x8BE0
#define RTKIOCIWLASTPRIV	0x8BFF

#define RTKIOCSIWPRIVADAPTIVITY		0x8BFB
#define RTKIOCGIWPRIVPASSPHRASE		0x8BFC
#define RTKIOCSIWPRIVCOUNTRY		0x8BFD
#define RTKIOCSIWPRIVAPESSID		0x8BFE
#define RTKIOCSIWPRIVPASSPHRASE		0x8BFF

/* The first and the last (range) */
#define RTKIOCIWFIRST	0x8B00
#define RTKIOCIWLAST	RTKIOCIWLASTPRIV		/* 0x8BFF */

/* Odd : get (world access), even : set (root access) */
#define RTW_IS_SET(cmd)	(!((cmd) & 0x1))
#define RTW_IS_GET(cmd)	((cmd) & 0x1)


#define RTWEVCUSTOM		0x8C02		/* Driver specific ascii string */
#define RTWEVREGISTERED	0x8C03		/* Discovered a new node (AP mode) */
#define RTWEVEXPIRED	0x8C04		/* Expired a node (AP mode) */

#define RTWEVMGNTRECV	0x8C10		/* Indicate Mgnt Frame and Action Frame to uplayer */


enum RTK_IW_PRIV_TYPE {
	RTW_PRIV_TYPE_NONE = 0x0000,		/* None type */
	RTW_PRIV_TYPE_BYTE = 0x1000,		/* Char as number */
	RTW_PRIV_TYPE_CHAR = 0x2000,		/* Char as character */
	RTW_PRIV_TYPE_INT  = 0x4000,		/* 32 bits int */
	RTW_PRIV_TYPE_FLOAT = 0x5000,		/* struct rtw_freq */
	RTW_PRIV_TYPE_ADDR = 0x6000,		/* struct sockaddr */
	RTW_PRIV_TYPE_MASK = 0x7000			/* Type of arguments */
};


#define RTW_PRIV_SIZE_FIXED	0x0800	/* Variable or fixed number of args */

#define RTW_PRIV_SIZE_MASK	0x07FF	/* Max number of those args */


#define RTW_ESSID_MAX_SIZE	32

enum RTK_IW_MODE {
	RTW_MODE_AUTO = 0,		/* Let the driver decides */
	RTW_MODE_ADHOC,			/* Single cell network */
	RTW_MODE_INFRA,			/* Multi cell network, roaming, ... */
	RTW_MODE_MASTER			/* Synchronisation master or Access Point */
};

#define RTW_ENCODE_DISABLED	0x8000	/* Encoding disabled */
#define RTW_ENCODE_TEMP		0x0400  /* Temporary key */

#define RTW_CUSTOM_MAX		256	/* In bytes */

/* RTKIOCSIWAUTH/RTKIOCGIWAUTH struct rtw_param flags */
#define RTW_AUTH_INDEX		0x0FFF

enum RTK_IW_AUTH_PARAM {	
	RTW_AUTH_WPA_VERSION = 0,
	RTW_AUTH_CIPHER_PAIRWISE,
	RTW_AUTH_CIPHER_GROUP,
	RTW_AUTH_KEY_MGMT,
	RTW_AUTH_TKIP_COUNTERMEASURES,
	RTW_AUTH_DROP_UNENCRYPTED,
	RTW_AUTH_80211_AUTH_ALG,
	RTW_AUTH_WPA_ENABLED,
	RTW_AUTH_RX_UNENCRYPTED_EAPOL,
	RTW_AUTH_ROAMING_CONTROL,
	RTW_AUTH_PRIVACY_INVOKED
};


/* RTW_AUTH_80211_AUTH_ALG values (bit field) */
#define RTW_AUTH_ALG_OPEN_SYSTEM	0x00000001
#define RTW_AUTH_ALG_SHARED_KEY		0x00000002
#define RTW_AUTH_ALG_SAE	0x00000008

/* RTKIOCSIWENCODEEXT definitions */
#define RTW_ENCODE_SEQ_MAX_SIZE	8

/* struct rtw_encode_ext ->alg */
enum RTW_IW_ENC_ALG {
	RTW_ENCODE_ALG_NONE = 0,
	RTW_ENCODE_ALG_WEP,
	RTW_ENCODE_ALG_TKIP,
	RTW_ENCODE_ALG_CCMP,
	RTW_ENCODE_ALG_PMK,
	RTW_ENCODE_ALG_AES_CMAC, //IGTK
	RTW_ENCODE_ALG_GCMP_256
};

/* struct rtw_encode_ext ->ext_flags */
#define RTW_ENCODE_EXT_RX_SEQ_VALID	0x00000002
#define RTW_ENCODE_EXT_GROUP_KEY	0x00000004
#define RTW_ENCODE_EXT_SET_TX_KEY	0x00000008
			   

struct sockaddr_t {
  __u8 sa_len;
  __u8 sa_family;
  char sa_data[14];
};

struct	rtw_param
{
  __s32		value;		/* The value of the parameter itself */
  __u8		fixed;		/* Hardware should not use auto select */
  __u8		disabled;	/* Disable the feature */
  __u16		flags;		/* Various specifc flags (if any) */
};

struct	rtw_point
{
  void		*pointer;	/* Pointer to the data  (in user space) */
  __u16		length;		/* number of fields or size in bytes */
  __u16		flags;		/* Optional params */
};

struct	rtw_freq
{
	__s32		m;		/* Mantissa */
	__s16		e;		/* Exponent */
	__u8		i;		/* List index (when in range struct) */
	__u8		flags;		/* Flags (fixed/auto) */
};

struct	rtw_encode_ext
{
	__u32		ext_flags; /* IW_ENCODE_EXT_* */
	__u8		tx_seq[RTW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	__u8		rx_seq[RTW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	struct sockaddr_t	addr; /* ff:ff:ff:ff:ff:ff for broadcast/multicast
			       * (group) keys or unicast address for
			       * individual keys */
	__u16		alg; /* IW_ENCODE_ALG_* */
	__u16		key_len;
#ifdef __CC_ARM	//Fix Keil compile error, must modify sizeof rtw_encode_ext - Alex Fang
	__u8		key[1];
#else
	__u8		key[0];
#endif
};

union	rtwreq_data
{
	/* Config - generic */
	char		name[IFNAMSIZ];
	/* Name : used to verify the presence of  wireless extensions.
	 * Name of the protocol/provider... */

	struct rtw_point	essid;		/* Extended network name */
	struct rtw_param	nwid;		/* network id (or domain - the cell) */
	struct rtw_freq	freq;		/* frequency or channel :
					 * 0-1000 = channel
					 * > 1000 = frequency in Hz */

	struct rtw_param	sens;		/* signal level threshold */
	struct rtw_param	bcnsens;	/* signal level threshold */
	struct rtw_param	bitrate;	/* default bit rate */
	struct rtw_param	txpower;	/* default transmit power */
	struct rtw_param	rts;		/* RTS threshold threshold */
	struct rtw_param	frag;		/* Fragmentation threshold */
	__u32		mode;		/* Operation mode */
	struct rtw_param	retry;		/* Retry limits & lifetime */

	struct rtw_point	encoding;	/* Encoding stuff : tokens */
	struct rtw_param	power;		/* PM duration/timeout */

	struct sockaddr_t	ap_addr;	/* Access point address */
	struct sockaddr_t	addr;		/* Destination address (hw/mac) */

	struct rtw_param	param;		/* Other small parameters */
	struct rtw_point	data;		/* Other large parameters */
	struct rtw_point	passphrase;		/* Extended network name */
};

struct	rtwreq 
{
	char	ifr_name[IFNAMSIZ];	/* if name, e.g. "eth0" */

	/* Data part (defined just above) */
	union	rtwreq_data	u;
};
 
struct	rtw_priv_args
{
	__u32		cmd;		/* Number of the ioctl to issue */
	__u16		set_args;	/* Type and number of args */
	__u16		get_args;	/* Type and number of args */
	char		name[IFNAMSIZ];	/* Name of the extension */
};


#define IW_EXT_STR_FOURWAY_DONE  		"WPA/WPA2 handshake done"
#define IW_EXT_STR_RECONNECTION_FAIL  	"RECONNECTION FAILURE"
#define IW_EVT_STR_STA_ASSOC			"STA Assoc"
#define IW_EVT_STR_STA_DISASSOC		"STA Disassoc"
#define IW_EXT_STR_FOURWAY_DONE  "WPA/WPA2 handshake done"
#define IW_EXT_STR_RECONNECTION_FAIL  "RECONNECTION FAILURE"
#define IW_EVT_STR_STA_ASSOC	"STA Assoc"
#define IW_EVT_STR_STA_DISASSOC	"STA Disassoc"
#define IW_EVT_STR_SEND_ACTION_DONE	"Send Action Done"
#define IW_EVT_STR_NO_NETWORK "No Assoc Network After Scan Done"
#define IW_EVT_STR_ICV_ERROR "ICV Eror"
#define IW_EVT_STR_CHALLENGE_FAIL "Auth Challenge Fail"
#define IW_EVT_STR_SOFTAP_START "Softap Start"
#define IW_EVT_STR_SOFTAP_STOP "Softap Stop"
#endif	/* _WIRELESS_H */
