/*
* Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
* All rights reserved.	
*
* File Name 		: ms_wlan.h
* Introduction	: wlan control interface
*
* Current Version	: v0.3
* Author			: zouwei
* Create Time	: 2015/08/01
* Change Log	:(2015/08/26) rename this file
*
* All software, firmware and related documentation herein ("M-Smart Software") are
* intellectual property of M-SMART Research Institute of Midea Group and protected 
* by law, including, but not limited to, copyright law and international treaties.
*
* Any use, modification, reproduction, retransmission, or republication of all
* or part of M-Smart Software is expressly prohibited, unless prior written
* permission has been granted by M-Smart.
*
*/

#ifndef __MS_HAL_WLAN_H__
#define __MS_HAL_WLAN_H__

#include "FreeRTOS.h"

#define MS_HAL_SHARED_ENABLED  	0x00008000
#define MS_HAL_WPA_SECURITY    	0x00200000
#define MS_HAL_WPA2_SECURITY   	0x00400000
#define MS_HAL_WPS_ENABLED     	0x10000000

#define MS_HAL_WEP_ENABLED     		0x0001
#define MS_HAL_TKIP_ENABLED         0x0002
#define MS_HAL_AES_ENABLED        	0x0004
#define MS_HAL_WSEC_SWFLAG        	0x0008

#define MS_HAL_WLAN_MAC_ADDR_LENGTH	(6)
#define MS_HAL_WLAN_MAX_SSID_LENGTH	(32)

typedef enum
{
    /* universal successful return result.*/
	MS_HAL_WLAN_RESULT_SUCCESS            = 0x00,
    /* universal error return result.*/
	MS_HAL_WLAN_RESULT_ERROR              = 0x01,
} ms_hal_wlan_result_t;

typedef enum
{
	/*  wlan event    */
	MS_HAL_EVT_LINK_LOST				= 0x10,
	MS_HAL_EVT_CHANNEL_SWITCH			= 0x12,
	MS_HAL_EVT_NETWORK_RESET			= 0x13,
	MS_HAL_EVT_DHCP_RENEW				= 0x14,
	/*  sta event.     */
	MS_HAL_EVT_STA_CONNECT_SUCCESS		= 0x82,
	MS_HAL_EVT_STA_CONNECT_FAILED		= 0x84,	
	/*  ap event.      */
	MS_HAL_EVT_AP_START_SUCCESS			= 0xC2,
	MS_HAL_EVT_AP_START_FAILED			= 0xC4,
}ms_hal_wlan_event_t;

typedef enum
{
    /* Idle status, wait for a connection.*/
	MS_HAL_WLAN_STA_IDLE               = 0x00,
    /* In Connection.*/
	MS_HAL_WLAN_STA_CONNECTING         = 0x01,
    /* Already connected.*/
	MS_HAL_WLAN_STA_CONNECTED          = 0x02,
	/* unknown status, probably an error occur.*/
	MS_HAL_WLAN_STA_UNKNOWN_STATUS     = 0x03,
} ms_hal_wlan_sta_status_t;

typedef enum
{
    /* Idle status, wait to start ap.*/
	MS_HAL_WLAN_AP_IDLE                = 0x00,
    /* In starting..*/
	MS_HAL_WLAN_AP_STARTING            = 0x01,
    /* Normally running.*/
	MS_HAL_WLAN_AP_RUNNING             = 0x02,
    /* Unknown status, probably an error occur.*/
	MS_HAL_WLAN_AP_UNKNOWN_STATUS      = 0x03,
} ms_hal_wlan_ap_status_t;

typedef enum
{
    /* Active mode.*/
	MS_HAL_WLAN_SCAN_TYPE_ACTIVE 	= 0x00,
    /* Passive mode.*/
	MS_HAL_WLAN_SCAN_TYPE_PASSIVE 	= 0x01,
} ms_hal_wlan_scan_type_t;

typedef enum
{
	MS_HAL_WLAN_BSS_TYPE_INFRASTRUCTURE		= 0x00,
	MS_HAL_WLAN_BSS_TYPE_ADHOC				= 0x01,
	MS_HAL_WLAN_BSS_TYPE_BOTH				= 0x02,
}ms_hal_wlan_bss_type_t;


typedef enum
{
	MS_HAL_WLAN_POWER_MODE_RUNTIME			= 0x00,
	MS_HAL_WLAN_POWER_MODE_STANDBY			= 0x01,
	MS_HAL_WLAN_POWER_MODE_SLEEP			= 0x02,
	MS_HAL_WLAN_POWER_MODE_DEEP_SLEEP		= 0x03,
}ms_hal_wlan_powersaving_mode_t;

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum
{
    MS_HAL_WLAN_SECURITY_OPEN           = 0,                                                				/* Open security                           		*/
    MS_HAL_WLAN_SECURITY_WEP_PSK        = MS_HAL_WEP_ENABLED,                                      			/* WEP Security with open authentication.*/ 
    MS_HAL_WLAN_SECURITY_WEP_SHARED     = ( MS_HAL_WEP_ENABLED | MS_HAL_SHARED_ENABLED ),                 	/* WEP Security with shared authentication 	*/
    MS_HAL_WLAN_SECURITY_WPA_TKIP_PSK   = ( MS_HAL_WPA_SECURITY  | MS_HAL_TKIP_ENABLED ),                 	/* WPA Security with TKIP                  		*/
    MS_HAL_WLAN_SECURITY_WPA_AES_PSK    = ( MS_HAL_WPA_SECURITY  | MS_HAL_AES_ENABLED ),                  	/* WPA Security with AES                   		*/
    MS_HAL_WLAN_SECURITY_WPA2_AES_PSK   = ( MS_HAL_WPA2_SECURITY | MS_HAL_AES_ENABLED ),                  	/* WPA2 Security with AES                  		*/
    MS_HAL_WLAN_SECURITY_WPA2_TKIP_PSK  = ( MS_HAL_WPA2_SECURITY | MS_HAL_TKIP_ENABLED ),                 	/* WPA2 Security with TKIP                 		*/
    MS_HAL_WLAN_SECURITY_WPA2_MIXED_PSK = ( MS_HAL_WPA2_SECURITY | MS_HAL_AES_ENABLED | MS_HAL_TKIP_ENABLED ),   /* WPA2 Security with AES & TKIP           	*/
    MS_HAL_WLAN_SECURITY_WPS_OPEN       = MS_HAL_WPS_ENABLED,                                      			/* WPS with open security                  		*/
    MS_HAL_WLAN_SECURITY_WPS_SECURE     = (MS_HAL_WPS_ENABLED | MS_HAL_AES_ENABLED),                      	/* WPS with AES security                   		*/
} ms_hal_wlan_security_t;

/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
	 /* Denotes 5GHz radio band */
    MS_HAL_WLAN_802_11_BAND_5GHZ   = 0,
     /* Denotes 2.4GHz radio band */
    MS_HAL_WLAN_802_11_BAND_2_4GHZ = 1 
} ms_hal_wlan_802_11_band_t;

typedef enum
{
	MS_HAL_WLAN_MODE_AP		= 0x00,
	MS_HAL_WLAN_MODE_STA	= 0x01,
}ms_hal_wlan_mode_t;

typedef enum 
{
    /* Beacon Frame.*/
	MS_HAL_BEACON = 0x80,
	/* Data Frame.*/
	MS_HAL_DATA   = 0x08,
} ms_hal_wlan_frame_type_t;


typedef enum
{
	MS_HAL_WLAN_CHANNEL_WIDTH_20MHZ	= 0x00,
	MS_HAL_WLAN_CHANNEL_WIDTH_40MHZ	= 0x01,
	MS_HAL_WLAN_CHANNEL_WIDTH_80MHZ	= 0x02,
}ms_hal_wlan_channel_width_t;


typedef struct
{
	/* ssid length */
	uint8_t len;	
	/* ssid name(AP name)	*/
	uint8_t	val[32];
}ms_hal_wlan_ssid_t;

typedef struct
{	
	/*unique 6-byte MAC address */
	uint8_t mac_addr[6]; 
}ms_hal_wlan_mac_t;

typedef struct
{

	/*unique 4-byte ip address*/
	uint32_t ipv4_addr; 
}ms_hal_wlan_ip_t;

#pragma pack(1)
typedef struct
{
    ms_hal_wlan_ssid_t              	SSID;             /* Service Set Identification (i.e. Name of Access Point)					*/
    ms_hal_wlan_mac_t               	BSSID;            /* Basic Service Set Identification (i.e. MAC address of Access Point)       	*/
    int16_t               				signal_strength;  /* Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    uint32_t                  			max_data_rate;    /* Maximum data rate in kilobits/s                                           			*/
    ms_hal_wlan_bss_type_t          	bss_type;         /* Network type                                                              				*/
    ms_hal_wlan_security_t          	security;         /* Security type                                                            	 			*/
    uint8_t                   			channel;          /* Radio channel that the AP beacon was received on                          		*/
    ms_hal_wlan_802_11_band_t       	band;             /* Radio band                                                                				*/
    bool              					record_on_channel;/* True if scan result was recorded on the channel advertised in the packet  	*/
    //struct ms_hal_wlan_scan_result_t* 	next;             /* Pointer to the next scan result                                           			*/
} ms_hal_wlan_scan_result_t;

typedef void (*ms_hal_wlan_scan_result_callback_t)( ms_hal_wlan_scan_result_t *result_list, uint8_t num);
#pragma pack()
typedef struct
{
	uint8_t				number;					/* Specifies the max scan number of AP list*/
	ms_hal_wlan_scan_type_t 	scan_type;				/* Specifies whether the scan should be Active or Passive */
	ms_hal_wlan_bss_type_t		bss_type;				/* Specifies whether the scan should search for Infrastructure networks (those using  \
														an Access Point), Ad-hoc networks, or both types.*/
	ms_hal_wlan_ssid_t			*ssid;					/* If this is non-Null, then the scan will only search for networks using the specified SSID.	*/
	ms_hal_wlan_mac_t			*mac;					/* If this is non-Null, then the scan will only search for networks where the BSSID (MAC \
														address of the Access Point) matches the specified MAC address.*/
	uint16_t				*channel_list;				/* If this is non-Null, then the scan will only search for networks on the specified channels\
														array of channel numbers to search, terminated with a zero*/
	ms_hal_wlan_scan_result_callback_t	callback;		/* the callback function which will receive and process the result data.	*/
	ms_hal_wlan_scan_result_t	*result_list;			/* a pointer to a result storage structure.*/
}ms_hal_wlan_ap_scan_param_t;

typedef int (*ms_hal_wlan_event_handler_t)(ms_hal_wlan_event_t event, void *data);
typedef void (*ms_hal_sniffer_callback_t)(uint8_t *frame, uint16_t len);

typedef struct
{
	/*Specifies the ssid that will be scanned for*/
	uint8_t						*ssid;
	/*Specifies the length of ssid*/
	uint8_t 					ssid_length;
	/*Specifies the authentiation type*/
	ms_hal_wlan_security_t		auth_type;
	/*Specifies the security key*/
	const uint8_t				*security_key;
	/*Specifies the length of key*/
	uint8_t 					key_length;
}ms_hal_wlan_ap_connect_param_t;

typedef struct
{
	/*Specifies the ap that exists in ap scan list*/
	const ms_hal_wlan_scan_result_t	*ap;
	/*Specifies the key*/
	const uint8_t				*security_key;
	/*Specifies the length of key*/
	uint8_t 					key_length;
}ms_hal_wlan_ap_join_param_t;


typedef struct
{
	uint8_t			*ssid;
	uint8_t 		ssid_length;
	ms_hal_wlan_security_t	auth_type;
	uint8_t			*security_key;
	uint8_t 		key_length;
	uint8_t			channel;
}ms_hal_wlan_ap_start_param_t;

typedef struct
{
    /* filter_flags A filter for control, management and data frames.
         Filter flags:
 		   Bit[0]: Enable/disable management frame receive,
 		   Bit[1]: Enable/disable control frame receive,
 		   Bit[2]: Enable/Disable data frame receive.
		   Bit[3]: Enable/Disable multicast filter(MacAddr[0], 0x01 0x00 0x5e) 
       */
	uint8_t filter_flags;
	/*  sniffer channel width, for example. 20MHz. */
	ms_hal_wlan_channel_width_t channel_width;
	/*  radio_type Radio type 2.4 or 5 GHz. */
	ms_hal_wlan_802_11_band_t radio_band_type;
	/* Channel number to sniff as per radio type.*/
	uint8_t channel;
    /*An application callback to process captured frames*/
	ms_hal_sniffer_callback_t callback;
	/* Buffer to receive frame data*/
	uint8_t *frame_buffer;
	/* max length of frame buffer*/
	uint16_t len;
}ms_hal_wlan_sniffer_param_t;

#pragma pack(1)
typedef struct 
{
	/*  Frame Control flags. */
	uint16_t frame_ctrl_flags;
	/*  Duration. */
	uint16_t duration;
	/* MAC Address. */
	char addr[MS_HAL_WLAN_MAC_ADDR_LENGTH*3];
	/*  Sequence and Fragmentation number. */
	uint16_t seq_frag_num;
	/*  Time stamp. */
	uint8_t timestamp[8];
	/*  Beacon Interval. */
	uint16_t beacon_interval;
	/*  Capability Information. */
	uint16_t cap_info;
	/*  SSID Element ID = 00. */
	uint8_t ssid_element_id;
	/*  SSID Length. */
	uint8_t ssid_len;
	/*  SSID. */
	char ssid[MS_HAL_WLAN_MAX_SSID_LENGTH];
} ms_hal_wlan_beacon_info_t;
#pragma pack()

#pragma pack(1)
typedef struct 
{
	/*  Frame Control flags. */
	uint16_t frame_ctrl_flags;
	/*  Duration. */
	uint16_t duration;
	/* MAC Address. */
	uint8_t addr[MS_HAL_WLAN_MAC_ADDR_LENGTH*3];
	/*  Sequence and Fragmentation number. */
	uint16_t seq_frag_num;
	/*  QoS Control bits. */
	uint16_t qos_ctrl;
} ms_hal_wlan_data_info_t;
#pragma pack()




/** Initiates the wlan network.
 *
 * @return WLAN_RESULT_SUCCESS: initialize successfully
 *            WLAN_RESULT_ERROR: initialize failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_init(void);


/** Start wlan network.
 *
 * @param event_handler: Wlan callback function
 * @return WLAN_RESULT_SUCCESS: call successfully
 *            WLAN_RESULT_ERROR: call failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_start(ms_hal_wlan_event_handler_t event_handler);


/** Stop wlan network.
 *
 * @return WLAN_RESULT_SUCCESS: stop successfully
 *            WLAN_RESULT_ERROR: stop failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_stop(void);


/** Initiates a scan to search for 802.11 networks.
 *
 * @param scan_param:	Specifies the scan parameter in scan param structure.
 *
 * @return WLAN_RESULT_SUCCESS: call successfully
 *             WLAN_RESULT_ERROR: call failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_start_scan(ms_hal_wlan_ap_scan_param_t *scan_param);


/** stop scanning process.
 *
 * @return WLAN_RESULT_SUCCESS: stop scanning successfully
 *             WLAN_RESULT_ERROR: If an error occurred	
 */
ms_hal_wlan_result_t ms_hal_wlan_stop_scan(void);


/** Scans for, associates and authenticates with a Wi-Fi network.
 * On successful return, the system is ready to send data packets.
 *
 * @param connect_param: Specifies the connect parameter in connect param structure.
 * 
 * @return WLAN_RESULT_SUCCESS: call successfully
 *             WLAN_RESULT_ERROR: call failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_connect_ap(ms_hal_wlan_ap_connect_param_t	*connect_param);


/** Associates and authenticates with a specific Wi-Fi access point.
 * On successful return, the system is ready to send data packets.
 *
 * @param join_param: Specifies the join parameter in join param structure.
 *
 * @return WLAN_RESULT_SUCCESS: call successfully
 *             WLAN_RESULT_ERROR: call failed	
 */
ms_hal_wlan_result_t ms_hal_wlan_join_ap(ms_hal_wlan_ap_join_param_t	*join_param);


/** Disassociates from a Wi-Fi network.
 *
 * @return    WLAN_RESULT_SUCCESS: disconnect successfully
 *			WLAN_RESULT_ERROR: disconnect failed
 */
ms_hal_wlan_result_t ms_hal_wlan_disconnect_ap(void);


/** Retrieve the status information of the station interface.(e.g. idle/connecting/connected)
 *
 *  @return   WLAN_STA_IDLE: Idle status, wait for a connection.
 *               WLAN_STA_CONNECTING: In connection.
 *               WLAN_STA_CONNECTED: Already connected.
 *               WLAN_STA_UNKNOWN_STATUS: unknown status, probably an error occur.
 */
ms_hal_wlan_sta_status_t ms_hal_wlan_get_sta_status(void);


/** Starts an infrastructure WiFi network
 *
 * @param ap_start_param: Specifies the ap start parameter in ap start param structure.
 *
 * @return    WLAN_RESULT_SUCCESS : call successfully
 *                WLAN_RESULT_ERROR   : call failed
 */
ms_hal_wlan_result_t ms_hal_wlan_start_ap(ms_hal_wlan_ap_start_param_t *ap_start_param);


/** Stops an existing WiFi network
 *
 * @return    WLAN_RESULT_SUCCESS: close successfully
 *			WLAN_RESULT_ERROR: close failed
 */
ms_hal_wlan_result_t ms_hal_wlan_stop_ap(void);


/** Retrieve the status information of the ap interface. (e.g. idle/starting/running)
 *
 *  @return 	WLAN_AP_IDLE: idle status, wait to start ap.
 *			WLAN_AP_STARTING: In starting.
 *			WLAN_AP_RUNNING: normally running.
 *			WLAN_AP_UNKNOWN_STATUS: unknown status, probably an error occur.
 */
ms_hal_wlan_ap_status_t ms_hal_wlan_get_ap_status(void);


/** Retrieves MAC address of 802.11 device
 *
 * @param mac: A pointer to a variable that the current MAC address will be written to.
 *
 * @return    WLAN_RESULT_SUCCESS: get mac address successfully
 *			WLAN_RESULT_ERROR: get mac address failed
 */
ms_hal_wlan_result_t ms_hal_wlan_get_mac_address(ms_hal_wlan_mac_t *mac);


/** Set MAC Address of 802.11 device.
 *
 * @param mac The MAC Address in 6 byte array format like
 *                uint8_t mac[] = { 0x00, 0x50, 0x43, 0x21, 0x19, 0x6E};
 * @return   	WLAN_RESULT_SUCCESS: set mac address successfully
 *			WLAN_RESULT_ERROR: set mac address failed
 */
ms_hal_wlan_result_t ms_hal_wlan_set_mac_address(ms_hal_wlan_mac_t *mac);


/** Retrieve the current IP address of STA mode or AP mode.
 *
 * @param mode: a variable that represent the STA mode or AP mode 
 * @param ip: Pointer to a variable that the current IP address will be written to
 *
 * @return    	WLAN_RESULT_SUCCESS: get ip address successfully.
 *			WLAN_RESULT_ERROR: get ip address failed.
 */
ms_hal_wlan_result_t ms_hal_wlan_get_ip_address(ms_hal_wlan_mode_t mode, ms_hal_wlan_ip_t *ip);


/** Set IP address of AP mode.
 *
 * @param ip: The IP Address in uint32_t format.(e.g. If the ip address is 192.168. 1.1, the value of ip is 0xC0A80101);
 *
 * @return    	WLAN_RESULT_SUCCESS: set ip address successfully
 *			WLAN_RESULT_ERROR: set ip address failed
 */
ms_hal_wlan_result_t ms_hal_wlan_set_ip_address(ms_hal_wlan_ip_t *ip);


/** Enables powersaving mode.
 *
 * @param mode: Specifies the powersaving mode type.
 *
 * @return 	WLAN_RESULT_SUCCESS: enable powersaving successfully
 *			WLAN_RESULT_ERROR: enable powersaving failed
 */
ms_hal_wlan_result_t ms_hal_wlan_enable_powersaving(ms_hal_wlan_powersaving_mode_t mode);


/** Disables powersaving mode. The system will run in runtime mode.
 *
 * @return  	WLAN_RESULT_SUCCESS: disable powersaving successfully
 *			WLAN_RESULT_ERROR: disable powersaving failed
 */
ms_hal_wlan_result_t ms_hal_wlan_disable_powersaving(void);


/** Gets the tx power in dBm units
 *
 * @param dbm: The variable to receive the tx power in dbm.
 *
 * @return  	WLAN_RESULT_SUCCESS: get tx power successfully
 *			WLAN_RESULT_ERROR: get tx power failed
 */
ms_hal_wlan_result_t ms_hal_wlan_get_tx_power( uint8_t *dbm );


/** Sets the tx power in dBm units
 *
 * @param dbm : The desired tx power in dbm. If set to 0xFF, the default value is restored.
 *
 * @return  	WLAN_RESULT_SUCCESS: set tx power successfully
 *			WLAN_RESULT_ERROR: set tx power failed
 */
ms_hal_wlan_result_t ms_hal_wlan_set_tx_power( uint8_t dbm );


/** Retrieve the latest RSSI value
 *
 * @param rssi: The location where the RSSI value will be stored
 *
 * @return  	WLAN_RESULT_SUCCESS: get rssi successfully
 *			WLAN_RESULT_ERROR: get rssi failed
 */
ms_hal_wlan_result_t ms_hal_wlan_get_rssi( int32_t *rssi );


/** Sets the 802.11 powersave listen interval for a Wi-Fi client, 
 * and communicates the listen interval to the Access Point. The 
 * listen interval will be set to listen_interval x BEACON_PERIOD(100 ms).
 *
 * @param listen_interval : The desired beacon listen interval
 *
 * @return  	WLAN_RESULT_SUCCESS : set listen interval successfully.
 *          	WLAN_RESULT_ERROR   : set listen interval failed.
 */
ms_hal_wlan_result_t ms_hal_wlan_set_listen_interval(uint8_t listen_interval);


/** Gets the current value of listen interval
 *
 * @param interval: A pointer to the value where interval will be stored.
 *
 * @return  	WLAN_RESULT_SUCCESS : get listen interval successfully
 *              	WLAN_RESULT_	ERROR   : get listen interval failed
 */
ms_hal_wlan_result_t ms_hal_wlan_get_listen_interval( uint8_t *interval );


/** Get the current channel on STA or AP interface
 *
 * @param band: Specifies the radio band is 5G or 2.4G.
 * @param channel: A pointer to the variable where the channel value will be written.
 *
 * @return  	WLAN_RESULT_SUCCESS : get channel successfully
 *          	WLAN_RESULT_ERROR: get channel failed
 */
ms_hal_wlan_result_t ms_hal_wlan_get_channel(ms_hal_wlan_802_11_band_t band, uint32_t *channel);


/** Set the current channel on STA or AP interface.
 *
 * @param channel: The desired channel
 *
 * @return  	WLAN_RESULT_SUCCESS : set channel successfully
 *          	WLAN_RESULT_ERROR   : set channel failed
 */
ms_hal_wlan_result_t ms_hal_wlan_set_channel( uint32_t channel);


/**
 * Starts sniffing with station interface on given channel and filters.
 * The captured frames are delivered to application using registered
 * sniffer callback.
 *
 * @param sniffer_param: Specifies the sniffer parameter in sniffer param structure.
 *
 * @return 	WLAN_RESULT_SUCCESS: start sniffer successfully
 *			WLAN_RESULT_ERROR: start sniffer failed
 */
ms_hal_wlan_result_t ms_hal_wlan_start_sniffer(ms_hal_wlan_sniffer_param_t *sniffer_param);


/**
 * Stops sniffing on station interface.
 *
 * @return 	WLAN_RESULT_SUCCESS: stop sniffer successfully
 *			WLAN_RESULT_ERROR: stop sniffer failed
 */
ms_hal_wlan_result_t ms_hal_wlan_stop_sniffer(void);

/******
Print Scan Result
功能 打印 Scan Result 
参数 psScanMidea, psScanRealtek
返回值 Error or Success
说明 两个指针都非空返回 Success，否则 Error。
******/	
int PrintScanResult(ms_hal_wlan_scan_result_t* psScanRecord, int iRecordNumber);

/******
Print Suniffer Result.
功能 Print Suniffer Result 
参数 psSnifferResult 
返回值 None
说明 none
******/
void PrintSnifferResult(uint8_t* pucFrameBuffer, int iBufferLen, bool bIsPrintAll);

#endif
