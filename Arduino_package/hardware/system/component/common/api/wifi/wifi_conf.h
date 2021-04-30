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
 */

/**
  ******************************************************************************
  * @file    wifi_conf.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __WIFI_API_H
#define __WIFI_API_H

/** @addtogroup nic NIC
 *  @ingroup    wlan
 *  @brief      NIC functions
 *  @{
 */
#include "basic_types.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "wifi_util.h"
#include "wifi_ind.h"
#include <platform/platform_stdlib.h>

#ifdef __cplusplus
	extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/
#define RTW_ENABLE_API_INFO

#ifdef RTW_ENABLE_API_INFO
#if defined(CONFIG_MBED_ENABLED)
	extern __u32 GlobalDebugEnable;
	#define RTW_API_INFO(...)     do {\
		if (GlobalDebugEnable) \
			printf(__VA_ARGS__);\
	}while(0)
#else
	#define RTW_API_INFO printf
#endif
#else
	#define RTW_API_INFO(args)
#endif

#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]
#define CMP_MAC( a, b)		(((a[0])==(b[0]))&& \
				((a[1])==(b[1]))&& \
				((a[2])==(b[2]))&& \
				((a[3])==(b[3]))&& \
				((a[4])==(b[4]))&& \
				((a[5])==(b[5])))

/******************************************************
 *                    Constants
 ******************************************************/
#define SCAN_LONGEST_WAIT_TIME	(4500)

#define MAC_FMT			"%02x:%02x:%02x:%02x:%02x:%02x"

/* enable for partial channel scan*/
#define PSCAN_ENABLE		0x01
/* set to select scan time to FAST_SURVEY_TO, otherwise SURVEY_TO*/
#define PSCAN_FAST_SURVEY	0x02
/* set to select scan time to FAST_SURVEY_TO and resend probe request*/
#define PSCAN_SIMPLE_CONFIG	0x04

#ifndef WLAN0_NAME
	#define WLAN0_NAME	"wlan0"
#endif
#ifndef WLAN1_NAME
	#define WLAN1_NAME	"wlan1"
#endif

#ifndef NET_IF_NUM
#ifdef CONFIG_CONCURRENT_MODE
#define NET_IF_NUM		2
#else
#define NET_IF_NUM		1
#endif
#endif

 typedef enum _WL_BAND_TYPE{
	WL_BAND_2_4G = 0,
	WL_BAND_5G,
	WL_BAND_2_4G_5G_BOTH,
	WL_BANDMAX
}WL_BAND_TYPE,*PWL_BAND_TYPE;

/******************************************************
 *                 Type Definitions
 ******************************************************/
/** Scan result callback function pointer type
 * @param  result_ptr:
 * A pointer to the pointer that indicates where to put the next scan result
 * @param  user_data:
 * User provided data
 */
typedef void (*rtw_scan_result_callback_t)(\
	rtw_scan_result_t** result_ptr, void* user_data);

typedef rtw_result_t (*rtw_scan_result_handler_t)(\
	rtw_scan_handler_result_t* malloced_scan_result);

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct {
	char *buf;
	int buf_len;
} scan_buf_arg;

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct internal_scan_handler{
	rtw_scan_result_t**		pap_details;
	rtw_scan_result_t *		ap_details;
	int				scan_cnt;
	rtw_bool_t			scan_complete;
	unsigned char			max_ap_size;
	rtw_scan_result_handler_t	gscan_result_handler;
#if defined(SCAN_USE_SEMAPHORE) && SCAN_USE_SEMAPHORE
	void				*scan_semaphore;
#else
	int				scan_running;
#endif
	void*				user_data;
	unsigned int			scan_start_time;
} internal_scan_handler_t;

typedef struct {
	rtw_network_info_t		network_info;
	void				*join_sema;
} internal_join_result_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief  Initialize Realtek WiFi API System.
 * - Initialize the required parts of the software platform.
 * 	i.e. worker, event registering, semaphore, etc.
 * - Initialize the RTW API thread which handles the asynchronous event.
 * @return RTW_SUCCESS if initialization is successful, RTW_ERROR otherwise
 */
int wifi_manager_init(void);

/**
 * @brief  Join a Wi-Fi network.
 * 	Scan for, associate and authenticate with a Wi-Fi network.
 * 	On successful return, the system is ready to send data packets.
 * @param[in]  ssid: A null terminated string containing the SSID name of the
 *	network to join.
 * @param[in]  security_type: Authentication type:
 * 	RTW_SECURITY_OPEN           - Open Security
 * 	RTW_SECURITY_WEP_PSK        - WEP Security with open authentication
 * 	RTW_SECURITY_WEP_SHARED     - WEP Security with shared authentication
 * 	RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 * 	RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 * 	RTW_SECURITY_WPA2_TKIP_PSK  - WPA2 Security using TKIP cipher
 * 	RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * @param[in]  password: A byte array containing either the cleartext security
 * 	key for WPA/WPA2
 * 	secured networks, or a pointer to an array of rtw_wep_key_t
 * 	structures for WEP secured networks.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  key_id: The index of the wep key (0, 1, 2, or 3). If not using it,
 * 	leave it with value -1.
 * @param[in]  semaphore: A user provided semaphore that is flagged
 * 	when the join is complete. If not using it, leave it with NULL value.
 * @return  RTW_SUCCESS: when the system is joined and ready to send data packets.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 */
int wifi_connect(
	char 		*ssid,
	rtw_security_t	security_type,
	char 		*password,
	int 		ssid_len,
	int 		password_len,
	int 		key_id,
	void 		*semaphore);

/**
 * @brief  Join a Wi-Fi network with specified BSSID.
 * 	Scan for, associate and authenticate with a Wi-Fi network.
 * 	On successful return, the system is ready to send data packets.
 * @param[in]  bssid: The specified BSSID to connect.
 * @param[in]  ssid: A null terminated string containing the SSID name of
 * 	the network to join.
 * @param[in]  security_type: Authentication type:
 * 	RTW_SECURITY_OPEN		- Open Security
 * 	RTW_SECURITY_WEP_PSK		- WEP Security with open authentication
 * 	RTW_SECURITY_WEP_SHARED		- WEP Security with shared authentication
 * 	RTW_SECURITY_WPA_TKIP_PSK	- WPA Security
 * 	RTW_SECURITY_WPA2_AES_PSK	- WPA2 Security using AES cipher
 * 	RTW_SECURITY_WPA2_TKIP_PSK	- WPA2 Security using TKIP cipher
 * 	RTW_SECURITY_WPA2_MIXED_PSK	- WPA2 Security using AES and/or TKIP ciphers
 * 	RTW_SECURITY_WPA3_AES_PSK	- WPA3 Secrity using AES if supported
 * @param[in]  password: A byte array containing either the cleartext security key
 * 	for WPA/WPA2
 * 	- secured networks, or a pointer to an array of rtw_wep_key_t
 * 	- structures for WEP secured networks.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  key_id: The index of the wep key.
 * @param[in]  semaphore: A user provided semaphore that is flagged when the join
 * 	is complete.
 * @return  RTW_SUCCESS: when the system is joined and ready to send data packets.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 * @note  The difference between @ref wifi_connect_bssid() and @ref wifi_connect()
 * 	is that BSSID has higher priority as the basis of connection in
 * 	@ref wifi_connect_bssid.
 */
int wifi_connect_bssid(
	unsigned char 	bssid[ETH_ALEN],
	char 		*ssid,
	rtw_security_t	security_type,
	char 		*password,
	int 		bssid_len,
	int 		ssid_len,
	int 		password_len,
	int 		key_id,
	void 		*semaphore);

/**
 * @brief  Disassociates from current Wi-Fi network.
 * @param  None
 * @return  RTW_SUCCESS: On successful disassociation from the AP.
 * @return  RTW_ERROR: If an error occurred.
 */
int wifi_disconnect(void);

/**
 * @brief  Check if Wi-Fi has connected to AP before dhcp.
 * @param  None
 * @return  RTW_SUCCESS: If conneced.
 * @return  RTW_ERROR: If not connect.
 */
int wifi_is_connected_to_ap(void);

/**
 * @brief  Check if the specified interface is up.
 * @param[in]  interface: The interface can be set as
 * 	RTW_STA_INTERFACE or RTW_AP_INTERFACE.
 * 	(@ref rtw_interface_t)
 * @return  If the function succeeds, the return value is 1.
 * 	Otherwise, return 0.
 */
int wifi_is_up(rtw_interface_t interface);

/**
 * @brief  Check if the specified wlan_idx is running.
 * @param[in]  wlan_idx: can be set as WLAN0_IDX or WLAN1_IDX.
 * @return  If the function succeeds, the return value is 1.
 * 	Otherwise, return 0.
 * @note  For STA mode, only use WLAN0_IDX
 * 	For AP mode, only use WLAN1_IDX
 * 	For CONCURRENT mode, use WLAN0_IDX for sta and WLAN1_IDX for ap
 */
int wifi_is_running(unsigned char wlan_idx);

/**
 * @brief  Determines if a particular interface is ready
 * 	to transceive ethernet packets
 * @param  Radio interface to check, options are
 * 	RTW_STA_INTERFACE, RTW_AP_INTERFACE
 * @return  RTW_SUCCESS  : if the interface is ready to
 * 	transceive ethernet packets
 * @return  RTW_ERROR: if the interface is not ready to
 * 	transceive ethernet packets
 */
int wifi_is_ready_to_transceive(rtw_interface_t interface);

/**
 * @brief  This function sets the current Media Access Control (MAC)
 * 	address of the 802.11 device.
 * @param[in]  mac: Wi-Fi MAC address.
 * @return  RTW_SUCCESS or RTW_ERROR
 * note:  This MAC address will be applied to WLAN0
 */
int wifi_set_mac_address(char *mac);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  mac: Point to the result of the mac address will be get.
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_mac_address(char *mac);

/**
 * @brief  Enable Wi-Fi powersave mode.
 * @param  None
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_enable_powersave(void);

/**
 * @brief  Resume Wi-Fi powersave mode.
 * @param  None
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_resume_powersave(void);

/**
 * @brief  Disable Wi-Fi powersave mode.
 * @param  None
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_disable_powersave(void);

/**
 * @brief  Get enctryption ccmp key used by wifi(sta mode only)
 * @param[out]  UncstKey: the location where the CCMP TK(tempary key)
 * 	will be stored
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got
 */
int wifi_get_ccmp_tk(unsigned char *uncst_key);

/**
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client
 * 	list will be stored.
 * @param[in]  buffer_length: The buffer length is reserved for future use.
 * 	Currently, buffer length is set to a fixed value: 25.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(
	void			*client_list_buffer,
	unsigned short	buffer_length);

/**
 * @brief  Get connected AP's BSSID
 * @param[out]  bssid: the location where the AP BSSID will be stored
 * @return  RTW_SUCCESS: if result was successfully get
 * @return  RTW_ERROR: if result was not successfully get
 */
int wifi_get_ap_bssid(unsigned char *bssid);

/**
 * @brief  Get the SoftAP information.
 * @param[out]  ap_info: The location where the AP info will be stored.
 * @param[out]  security: The location where the security type will be stored.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_ap_info(rtw_bss_info_t *ap_info, rtw_security_t *security);

/**
 * @brief  Get bssid of connected AP for STA mode
 * 	or bssid of current soft AP which already started.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  bssid: The location where the bssid will be stored.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_get_bssid(const char *ifname, unsigned char *bssid);

/**
 * @brief  Get bssid of connected AP for STA mode
 * 	or bssid of current soft AP which already started.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  ssid: The location where the bssid will be stored.
 * @return  ssid length or RTW_ERROR.
 */
int wifi_get_ssid(const char *ifname, unsigned char *ssid);

/**
 * @brief  set max number of station for AP mode(AP mode only)
 * @param[in]  ap_sta_num: max number of station which will be set
 * @return  RTW_SUCCESS: The max number is successfully set.
 * @return  RTW_ERROR: The max number is not successfully set.
 */
int wifi_set_max_sta_num(unsigned char ap_sta_num);

/**
 * @brief  Set the country code to driver
 *	which is used to determine the channel set.
 * @param[in]  country_code: Specify the country code.
 * @return  RTW_SUCCESS: The country code is successfully set.
 * @return  RTW_ERROR: The country code is not successfully set.
 */
int wifi_set_country(rtw_country_code_t country_code);

/**
 * @brief  get sta mode MAX data rate.
 * @param[out]  inidata_rate: the location will the MAX data rate
 * 	will be stored.
 * @return  RTW_SUCCESS: If the INIDATA_RATE is successfully retrieved.
 * @return  RTW_ERROR: If the INIDATA_RATE is not retrieved.
 * note  inidata_rate = 2 * (data rate), you need inidata_rate/2.0
 * 	to get the real rate
 */
int wifi_get_sta_max_data_rate(__u8 * inidata_rate);

/**
 * @brief  get the latest RSSI value of WLAN0_NAME.
 * @param[out]  pRSSI: the location where the latest RSSI value
 * 	will be stored.
 * @return  RTW_SUCCESS: If the RSSI is succesfully retrieved.
 * @return  RTW_ERROR: If the RSSI is not retrieved.
 */
int wifi_get_rssi(int *pRSSI);

/**
 * @brief  get the latest SNR value.
 * @param[out]  pSNR: The location where the latest SNR value
 * 	will be stored.
 * @return  RTW_SUCCESS: If the SNR is succesfully retrieved.
 * @return  RTW_ERROR: If the SNR is not retrieved.
 */
 int wifi_get_snr(int *pSNR);

/**
 * @brief  Set the listening channel for promiscuous mode.
 * 	Promiscuous mode will receive all the packets in
 * 	this channel.
 * @param[in]  channel: The desired channel.
 * @return  RTW_SUCCESS: If the channel is successfully set.
 * @return  RTW_ERROR: If the channel is not successfully set.
 * @note  DO NOT call this function for STA mode wifi driver,
 * 	since driver will determine the channel from its
 * 	received beacon.
 */
int wifi_set_channel(int channel);

/**
 * @brief  Get the current channel on STA interface(WLAN0_NAME).
 * @param[out]  channel: A pointer to the variable where the
 * 	channel value will be written.
 * @return  RTW_SUCCESS: If the channel is successfully read.
 * @return  RTW_ERROR: If the channel is not successfully read.
 */
int wifi_get_channel(int *channel);

/**
 * @brief  switch the current channelPlan on STA or AP interface.
 * @param[in]  channel_plan: the available channelPlan Map index
 * 	from 0x20 to 0x79
 * @return  RTW_SUCCESS: If the channel plan is successfully changed.
 * @return  RTW_ERROR: If the channel plan is not successfully changed.
 */
int wifi_change_channel_plan(uint8_t channel_plan);

/**
 * @brief  Register interest in a multicast address.\n
 * 	Once a multicast address has been registered, all packets
 * 	detected on the medium destined for that address are forwarded
 * 	to the host. Otherwise they are ignored.
 * @param[in]  mac: Ethernet MAC address
 * @return  RTW_SUCCESS: If the address is registered successfully.
 * @return  RTW_ERROR: If the address is not registered.
 */
int wifi_register_multicast_address(rtw_mac_t *mac);

/**
 * @brief  Unregister interest in a multicast address.\n
 * 	Once a multicast address has been unregistered, all packets
 * 	detected on the medium destined for that address are ignored.
 * @param[in]  mac: Ethernet MAC address
 * @return  RTW_SUCCESS: If the address is unregistered successfully.
 * @return  RTW_ERROR: If the address is not unregistered.
 */
int wifi_unregister_multicast_address(rtw_mac_t *mac);

/**
 * @brief  Setup the adaptivity mode.
 * 	You can replace this weak function by the same name funcation
 * 	to setup adaptivity mode you want.
 * @param  None
 * @return  None.
 */
_WEAK void wifi_set_mib(void);

/**
 * @brief  Aim to setup country code, but do nothing currently.
 * 	You can replace this weak function by the same name funcation
 * 	to setup country code you want.
 * @param  None
 * @return  None.
 */
//----------------------------------------------------------------------------//
_WEAK void wifi_set_country_code(void);

/**
 * @brief  Enable Wi-Fi RF.
 * @param  None
 * @return  If the function succeeds, the return value is 0.
 * @note  The difference between @ref wifi_rf_on() and @ref wifi_on()
 * 	is that @ref wifi_rf_on() simply enable RF HAL,
 * 	it does not enable the driver or allocate memory.
 */
int wifi_rf_on(void);

/**
 * @brief  Disable Wi-Fi RF.
 * @param  None
 * @return  If the function succeeds, the return value is 0.
 * @note  The difference between @ref wifi_rf_off() and @ref wifi_off()
 * 	is that @ref wifi_rf_off() simply disable RF HAL,
 * 	the driver and used heap memory will NOT be released.
 */
int wifi_rf_off(void);

/**
 * @brief  Enable Wi-Fi.
 * - Bring the Wireless interface "Up".
 * - Initialize the driver thread which arbitrates access
 * 	to the SDIO/SPI bus.
 * @param[in]  mode: Decide to enable WiFi in which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC and RTW_MODE_P2P.
 * @return  RTW_SUCCESS: if the WiFi chip initialized successfully.
 * @return  RTW_ERROR: if the WiFi chip initialization failed.
 */
int wifi_on(rtw_mode_t mode);

/**
 * @brief  Disable Wi-Fi.
 * @param  None
 * @return  RTW_SUCCESS: deinit success,
 * 	wifi mode is changed to RTW_MODE_NONE.
 * @return  RTW_ERROR: otherwise.
 */
int wifi_off(void);

/**
 * @brief  Switch Wifi Mode
 * - Switch Wifi Mode to @param[in]
 * @param[in]  mode: Decide to switch WiFi to which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC.
 * @return  RTW_SUCCESS: WiFi switch mode success.
 * @return  RTW_ERROR: WiFi switch mdoe fail.
 */
int wifi_set_mode(rtw_mode_t mode);

/**
 * @brief  Get current WiFi mode
 * @param[in]  ifname: the wlan interface name,
 * 	can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  MODE: The location where the result
 * 	of WiFi mode will be restored.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_get_mode(const char *ifname, int * mode);

/**
 * @brief  Turn off the Wi-Fi device fastly
 * - Bring the Wireless interface "Down"
 * - De-Initialises the driver thread which arbitrates access
 * 	to the SDIO/SPI bus
 * - Fastly means that the tx and rx of driver will stop immediately,
 * 	will not deinit_mem_monitor, will not deinit_skb_data_pool.
 * @param[in]  None
 * @return  RTW_SUCCESS if fast wifi off is successful,
 * 	RTW_ERROR otherwise
 */
int wifi_off_fastly(void);

/**
 * @brief  Set IPS/LPS mode.
 * @param[in]  ips_mode: The desired IPS mode. It becomes effective
 * 	when wlan enter IPS.
 * 	ips_mode is the abbreviation of Inactive Power Save mode.
 * 	Wi-Fi automatically turns RF off if it is not associated to AP.
 * 	- Set 1 to enable inactive power save mode.
 * @param[in]  lps_mode: The desired LPS mode. It becomes effective
 * 	when wlan enter LPS.
 * 	lps_mode is the abbreviation of Leisure Power Save mode.
 * 	Wi-Fi automatically turns RF off during the association to AP
 * 	if traffic is not busy, while it also automatically turns RF on
 * 	to listen to the beacon of the associated AP.
 * 	- Set 1 to enable leisure power save mode.
 * @return  RTW_SUCCESS if setting the corresponding mode successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_power_mode(unsigned char ips_mode, unsigned char lps_mode);

/**
 * @brief  Set TDMA parameters
 * @param[in]  slot_period: We separate TBTT into 2 or 3 slots.
 * 	- If we separate TBTT into 2 slots,
 * 	then slot_period should be larger or equal to 50ms.
 * 	It means 2 slot period is (slot_period) and (100-slot_period).
 * 	- If we separate TBTT into 3 slots,
 * 	then slot_period should be less or equal to 33ms.
 * 	It means 3 slot period is (100 - 2 * slot_period),
 * 	(slot_period) and (slot_period)
 * @param[in]  rfon_period_len_1: rf will turn on for
 * 	(rfon_period_len_1)ms in slot 1
 * @param[in]  rfon_period_len_2: rf will turn on for
 * 	(rfon_period_len_2)ms in slot 2
 * @param[in]  rfon_period_len_3: rf will turn on for
 * 	(rfon_period_len_3)ms in slot 3
 * @return  RTW_SUCCESS if setting TDMA parameters successful
 * 	RTW_ERROR otherwise
 * @e.g.
 * slot_period = 60, rfon_period_len_1 = 10, rfon_period_len_2 = 20,
 * 	rfon_period_len_3 = 0
 * slot_period >= 50ms, so the TBTT is separated into 2 slots as follows:
 * |----TDMA period 1----|--TDMA period 2--|
 * |-10ms-|-----50ms-----|--20ms--|--20ms--|
 * |rf on-|----rf off----|--rf on-|-rf off-|
 */
int wifi_set_tdma_param(
	unsigned char	slot_period,
	unsigned char	rfon_period_len_1,
	unsigned char	rfon_period_len_2,
	unsigned char	rfon_period_len_3);

/**
 * @brief  Set LPS DTIM.
 * @param[in]  dtim: In LPS, the package can be buffered at AP side.
 * 	STA will leave LPS when dtim count of packages are buffered
 * 	at AP side.
 * @return  RTW_SUCCESS if setting LPS dtim successful.
 * @return  RTW_ERROR otherwise
 */
int wifi_set_lps_dtim(unsigned char dtim);

/**
 * @brief  Get LPS DTIM.
 * @param[out]  dtim: In LPS, the package can be buffered at AP side.
 * 	STA will leave LPS when dtim count of packages are
 * 	buffered at AP side.
 * @return  RTW_SUCCESS if getting LPS dtim successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_get_lps_dtim(unsigned char *dtim);

enum {
	LPS_THRESH_PKT_COUNT = 0,
	LPS_THRESH_DIRECT_ENTER,
	LPS_THRESH_TP,
};
typedef unsigned char rtw_lps_thresh_t;

/**
 * @brief  Set LPS threshold.
 * @param[in] mode: LPS threshold mode can be
 * 	0) LPS_THRESH_PKT_COUNT: enter power save or not,
 * 	   according to packet num,
 * 	1) LPS_THRESH_DIRECT_ENTER: enter power save directly,
 * 	2) LPS_THRESH_TP: enter power save or not, according
 * 	   to throughput.
 * @return  RTW_SUCCESS if set LPS threshold successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_lps_thresh(rtw_lps_thresh_t mode);

/**
 * @brief Set LPS LEVEL
 * @param[in] lps_level can be
 * 	0) LPS_NORMAL: only turn off RF
 * 	1) LPS_LCLK: turn off RF and stop the clock of MAC
 * 	2) LPS_PG: turn off almost all the power of MAC circuit
 * @return  RTW_SUCCESS if setting LPS level successful.
 * @return  RTW_ERROR otherwise
 */
int wifi_set_lps_level(unsigned char lps_level);

/**
 * @brief  Set Management Frame Protection Support.
 * @param[in] value:
 * 	0) NO_MGMT_FRAME_PROTECTION: not support
 * 	1) MGMT_FRAME_PROTECTION_OPTIONAL: capable
 * 	2) MGMT_FRAME_PROTECTION_REQUIRED: required
 * @return  RTW_SUCCESS if setting Management Frame Protection Support successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_mfp_support(unsigned char value);

/**
 * @brief  Trigger Wi-Fi driver to start an infrastructure Wi-Fi network.
 * @param[in]  ssid: A null terminated string containing the SSID name
 * 	of the network.
 * @param[in]  security_type:
 * 	RTW_SECURITY_OPEN:           - Open Security
 * 	RTW_SECURITY_WPA_TKIP_PSK:   - WPA Security
 * 	RTW_SECURITY_WPA2_AES_PSK:   - WPA2 Security using AES cipher
 * 	RTW_SECURITY_WPA2_MIXED_PSK: - WPA2 Security using AES and/or TKIP ciphers
 * 	WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext security
 * 	key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number.
 * @warning  If a STA interface is active when this function is called,
 * 	the softAP will start on the same channel as the STA.
 * 	It will NOT use the channel provided!
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 */
int wifi_start_ap(
	char 		*ssid,
	rtw_security_t	security_type,
	char 		*password,
	int 		ssid_len,
	int 		password_len,
	int		channel);

/**
 * @brief  Start an infrastructure Wi-Fi network with hidden SSID.
 * @param[in]  ssid: A null terminated string containing
 * 	the SSID name of the network to join.
 * @param[in]  security_type: Authentication type:
 * 	RTW_SECURITY_OPEN           - Open Security
 * 	RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 * 	RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 * 	RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * 	WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext
 * 	security key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number
 * @warning  If a STA interface is active when this function is called,
 * 	the softAP will start on the same channel as the STA.
 * 	It will NOT use the channel provided!
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 */
int wifi_start_ap_with_hidden_ssid(
	char		*ssid,
	rtw_security_t	security_type,
	char		*password,
	int		ssid_len,
	int		password_len,
	int		channel);

/**
 * @brief  Initiate a scan to search for 802.11 networks.
 * @param[in]  scan_type: Specifies whether the scan should
 * 	be Active, Passive or scan Prohibited channels
 * @param[in]  bss_type: Specifies whether the scan should
 * 	search for Infrastructure networks (those using an Access
 * 	Point), Ad-hoc networks, or both types.
 * @param[in]  result_ptr: Scan specific ssid.
 * 	The first 4 bytes is ssid lenth, and ssid name append after it.
 * 	If no specific ssid need to scan, PLEASE CLEAN result_ptr
 * 	before pass it into parameter.
 * @param[out]  result_ptr: a pointer to the result storage structure.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  The scan progressively accumulates results over time, and
 * 	may take between 1 and 3 seconds to complete. The results of
 * 	the scan will be individually provided to the callback
 * 	function.
 * @note  The callback function will be executed in the context of RTW thread.
 * @note  When scanning specific channels, devices with a strong signal
 * 	strength on nearby channels may be detected
 */
int wifi_scan(
	rtw_scan_type_t		scan_type,
	rtw_bss_type_t		bss_type,
	void			*result_ptr);

/**
 * @brief  Initiate a scan to search for 802.11 networks,a higher level
 * 	API based on wifi_scan to simplify the scan operation.
 * @param[in]  results_handler: The callback function which will receive
 * 	and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly
 * 	to the callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions, since it is called
 * 	from the context of the RTW thread.
 * 	The callback, user_data variables will be referenced after
 * 	the function returns.
 * 	Those variables must remain valid until the scan is completed.
 * 	The usage of this api can reference ATWS in atcmd_wifi.c.
 */
int wifi_scan_networks(
	rtw_scan_result_handler_t	results_handler,
	void*				user_data);

/**
 * @brief  Initiate a scan to search for 802.11 networks,
 * 	a higher level API based on wifi_scan
 * 	to simplify the scan operation.
 * 	This API separate full scan channel to partial scan each
 * 	channel for concurrent mode.
 * 	MCC means Multi-channel concurrent.
 * @param[in]  results_handler: The callback function which will receive
 * 	and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly
 * 	to the callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions,
 * 	since it is called from the context of the RTW thread.
 * 	The callback, user_data variables will be referenced after
 * 	the function returns.
 * 	Those variables must remain valid until the scan is completed.
 * 	The usage of this api can reference ATWS in atcmd_wifi.c.
 */
int wifi_scan_networks_mcc(rtw_scan_result_handler_t results_handler, void* user_data);

/**
 * @brief  Initiate a scan to search for 802.11 networks with specified SSID.
 * @param[in]  results_handler: The callback function which will receive
 * 	and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly
 * 	to the callback function.
 * @param[in]  scan_buflen: The length of the result storage structure.
 * @param[in]  ssid: The SSID of target network.
 * @param[in]  ssid_len: The length of the target network SSID.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions,
 * 	since it is called from the context of the RTW thread.
 *	The callback, user_data variables will be referenced after
 * 	the function returns.
 *	Those variables must remain valid until the scan is completed.
 */
int wifi_scan_networks_with_ssid(
	int	(results_handler)(char*, int, char *, void *),
	void*	user_data,
	int	scan_buflen,
	char*	ssid,
	int	ssid_len);

/**
 * @brief  unregister _wifi_scan_each_report_hdl and _wifi_scan_done_hdl
 * @return  None
 */
void wifi_scan_unreg_event_handler(void);

/**
* @brief  Set the channel used to be partial scanned.
* @param[out]  channel_list: The location where the channel list
* 	will be stored.
* @param[in]  pscan_config: the pscan_config of the channel set.
* @param[in]  length: The length of the channel_list.
* @return  RTW_SUCCESS or RTW_ERROR.
* @note  This function should be used with wifi_scan function.
* 	First, use @ref wifi_set_pscan_chan to indicate which
* 	channel will be scanned, and then use @ref wifi_scan
* 	to get scanned results.
*/
int wifi_set_pscan_chan(__u8 * channel_list, __u8 * pscan_config, __u8 length);

/**
 * @brief  Get Wi-Fi security related info from driver.
 * @param[in]  ifname: the wlan interface name,
 * 	should be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  alg: the loaction where the alg will be stored
 * 	IW_ENCODE_ALG_NONE    - Open Security
 * 	IW_ENCODE_ALG_WEP     - WEP
 * 	IW_ENCODE_ALG_TKIP    - TKIP
 * 	IW_ENCODE_ALG_CCMP    - CCMP
 * @param[out]  key_idx: the location where the WiFi key index will be stored
 * @param[out]  passphrase: the location where WiFi passphrase will be stored
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  param alg should not set to NULL.
 * @note  if key_idx info is not needed, the param key_idx can be set to NULL.
 * @note  if passphrase info is not needed, the param passphrase can be set to NULL.
 */
int wifi_get_enc_ext(const char *ifname, __u16 *alg, __u8 *key_idx, __u8 *passphrase);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  ifname: the wlan interface name,
 * 	can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  pSetting: Points to the rtw_wifi_setting_t structure
 * 	to store the WIFI setting gotten from driver.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_get_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Print the network information stored in the
 * 	rtw_wifi_setting_t structure.
 * @param[in]  ifname: the wlan interface name,
 * 	can be WLAN0_NAME or WLAN1_NAME.
 * @param[in]  pSetting: Points to the rtw_wifi_setting_t structure
 * 	which information is gotten by @ref wifi_get_setting().
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_show_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Show the TX and RX statistic information.
 * @param[in]  idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX.
 * @return  NULL.
 */
void wifi_show_statistic(unsigned char idx);

/**
 * @brief  Set the network mode according to the data rate its supported.
 * 	Driver works in BGN mode in default after driver initialization.
 * 	This function is used to change wireless network mode
 * 	for station mode before connecting to AP.
 * @param[in]  mode: Network mode to set. The value can be
 * 	RTW_NETWORK_B, RTW_NETWORK_BG and RTW_NETWORK_BGN.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_network_mode(rtw_network_mode_t mode);

/**
 * @brief  Get the network mode.
 * 	Driver works in BGN mode in default after driver initialization.
 * 	This function is used to get the current wireless network mode
 * 	for station mode.
 * @param[out]  pmode: the location where network mode will be stored.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_get_network_mode(rtw_network_mode_t *pmode);

/**
 * @brief  Set the chip to start or stop the promiscuous mode.
 * @param[in]  enabled: enabled can be set 0, 1, 2, 3 and 4.
 * 	if enabled is zero, disable the promisc, else enable the promisc.
 * 	0) means disable the promisc.
 * 	1) means enable the promisc special for all ethernet frames.
 * 	2) means enable the promisc special for Broadcast/Multicast
 * 	   ethernet frames.
 * 	3) means enable the promisc special for all 802.11 frames.
 * 	4) means enable the promisc special for Broadcast/Multicast
 * 	   802.11 frames.
 * @param[in]  callback: the callback function which will
 * 	receive and process the network data.
 * @param[in]  len_used: specify if the the promisc data length is used.
 * 	If len_used set to 1, packet(frame data) length will be saved
 * 	and transferred to callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  This function can implement vendor specified simple configure.
 * @note  To fetch Ethernet frames, the len_used should be set to 1
 */
int wifi_set_promisc(
	rtw_rcr_level_t	enabled,
	void		(*callback)(unsigned char*, unsigned int, void*),
	unsigned char	len_used);

/**
 * @brief  Let Wi-Fi enter promiscuous mode.
 * @param[in]  None
 * @return  None
 */
void wifi_enter_promisc_mode(void);

/**
 * @brief  Set the wps phase
 * 	wps: Wi-Fi Protected Setup
 * @param[in]  is_trigger_wps: to trigger wps function or not
 * 	is_trigger_wps value should only be 0 or 1
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  set WPS IE in Probe Request/Probe Response/Beacon/
 * 	Association Request/Association Response
 * @param[in]  ifname: the wlan interface name,
 * 	ifname should be WLAN0_NAME or WLAN1_NAME
 * @param[in]  buf: the pointer to buf which store the WPS IE
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags:
 * 	P2PWPS_PROBE_REQ_IE: set WPS ie in Probe Request
 * 	P2PWPS_PROBE_RSP_IE: set WPS ie in Probe Response
 * 	P2PWPS_BEACON_IE: set WPS ie in Beacon
 * 	P2PWPS_ASSOC_REQ_IE: set WPS ie in Association Request
 * 	P2PWPS_ASSOC_RSP_IE: set WPS ie in Association Response
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_set_gen_ie(const char *ifname, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set the eap phase to wifi driver
 * @param[in]  is_trigger_eap: to trigger 802.1X EAP function or not
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_phase(unsigned char is_trigger_eap);

/**
 * @brief  get the current eap phase from wifi driver
 * @param[in]  None
 * @return  1: eap_phase is equal to 1, indicate 802.1X EAP is triggered
 * @return  0: eap_phase is equal to 0, indicate 802.1X EAP is not triggered
 */
unsigned char wifi_get_eap_phase(void);

/**
 * @brief  Set the current eap authentication method to wifi driver
 * @param[in]  eap_method: the current 802.1x EAP authentication method
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_method(unsigned char eap_method);

/**
 * @brief  send 802.1X EAP frame
 * @param[in]  ifname: the wlan interface name,
 * 	ifname should be WLAN0_NAME or WLAN1_NAME
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_eapol(const char *ifname, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Trigger Wi-Fi driver to restart an infrastructure Wi-Fi network.
 * @warning  If a STA interface is active when this function is called,
 * 	the softAP will start on the same channel as the STA.
 * 	It will NOT use the channel provided!
 * @param[in]  ssid: A null terminated string containing the SSID name
 * 	of the network.
 * @param[in]  security_type:
 * 	RTW_SECURITY_OPEN           - Open Security
 * 	RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 * 	RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 * 	RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * 	WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext security
 * 	key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number.
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 */
int wifi_restart_ap(
	unsigned char 		*ssid,
	rtw_security_t		security_type,
	unsigned char 		*password,
	int 			ssid_len,
	int 			password_len,
	int			channel);

/**
 * @brief  Set reconnection mode with configuration.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @param[in]  retry_times: The number of retry limit.
 * @param[in]  timeout: The timeout value (in seconds).
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 * @note  The difference between @ref wifi_config_autoreconnect()
 * 	and @ref wifi_set_autoreconnect() is that
 * 	user can specify the retry times and timeout value
 * 	in @ref wifi_config_autoreconnect().
 * 	But in @ref wifi_set_autoreconnect() these values
 * 	are set with 3 retry limit and 5 seconds timeout as default.
 */
int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout);

/**
 * @brief  Set reconnection mode with 3 retry limit and 5 seconds
 * 	timeout as default.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 * @note  The difference between @ref wifi_config_autoreconnect()
 * 	and @ref wifi_set_autoreconnect() is that
 * 	user can specify the retry times and timeout value
 * 	in @ref wifi_config_autoreconnect().
 * 	But in @ref wifi_set_autoreconnect() these values
 * 	are set with 3 retry limit and 5 seconds timeout as default.
 */
int wifi_set_autoreconnect(__u8 mode);

/**
 * @brief  Get the result of setting reconnection mode.
 * @param[out]  mode: Point to the result of setting reconnection mode.
 * 	mode = 0 means autoreconnect is off
 *	mode = 1 means autoreconnect is on
 * @return  0 if get the status of autoreconnect success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_get_autoreconnect(__u8 *mode);

/**
 * @brief  Set psk related info, including ssid, passphrase, psk.
 * @param[in]  psk_data: pointer to the structure that can set
 * 	psk related info in driver.{@ref struct psk_info}
 * @return  NULL.
 * @note  can be used to set psk related info to driver when fast connect
 */
void wifi_psk_info_set(struct psk_info * psk_data);

/**
 * @brief  Get psk related info, including ssid, passphrase, psk.
 * @param[out]  psk_data: pointer to the structure that will restore
 * 	psk related info. {ref struct psk_info}
 * @return  NULL.
 * @note  can be used to get current psk related info after connected
 * 	to AP successfully，and may write these info
 * 	to flash for fast connect next time
 */
void wifi_psk_info_get(struct psk_info * psk_data);

/**
 * @brief  Present the device disconnect reason while connecting.
 * @param  None
 * @return  @ref rtw_connect_error_flag_t
 * 	0) RTW_NO_ERROR
 * 	1) RTW_NONE_NETWORK
 * 	2) RTW_CONNECT_FAIL
 * 	3) RTW_WRONG_PASSWORD
 * 	4) RTW_4WAY_HANDSHAKE_TIMEOUT
 * 	5) RTW_DHCP_FAIL
 * 	6) RTW_UNKNOWN (initial status)
 */
int wifi_get_last_error(void);

/**
 * @brief  Set ble scan duty when coex.
 * @param[in]  duty: the desired scan duty for ble
 * @return  1: if the scan duty is successfully set
 * 	-1: if the scan duty is not successfully set.
 */
int wifi_btcoex_set_ble_scan_duty(u8 duty);

/**
 * @brief  check whether current wifi driver is mp or not.
 * @param  None
 * @return  1: CONFIG_MP_INCLUDED is open
 * 	0: CONFIG_MP_INCLUDED is not open
 */
u8 wifi_driver_is_mp(void);

#ifdef CONFIG_CUSTOM_IE
#ifndef BIT
#define BIT(x)	((__u32)1 << (x))
#endif

#ifndef _CUSTOM_IE_TYPE_
#define _CUSTOM_IE_TYPE_
/**
 * @brief  The enumeration is transmission type for wifi custom ie.
 */
enum CUSTOM_IE_TYPE{
	PROBE_REQ = BIT(0),
	PROBE_RSP = BIT(1),
	BEACON	  = BIT(2),
	ASSOC_REQ = BIT(3),
};
typedef __u32 rtw_custom_ie_type_t;
#endif /* _CUSTOM_IE_TYPE_ */

/* ie format
 * +-----------+--------+-----------------------+
 * |element ID | length | content in length byte|
 * +-----------+--------+-----------------------+
 *
 * type: refer to CUSTOM_IE_TYPE
 */
#ifndef _CUS_IE_
#define _CUS_IE_
/**
 * @brief  The structure is used to set WIFI custom ie list,
 * 	and type match CUSTOM_IE_TYPE.
 * 	The ie will be transmitted according to the type.
 */
typedef struct _cus_ie{
	__u8 *ie;
	__u8 type;
}rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */

/**
 * @brief  Setup custom ie list. (Information Element)
 * @warning  This API can't be executed twice before deleting
 * 	the previous custom ie list.
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE list.
 * @param[in]  ie_num: The number of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be done
 * 	before compiling, or this API won't be effective.
 */
int wifi_add_custom_ie(void *cus_ie, int ie_num);

/**
 * @brief  Update the item in WIFI CUSTOM IE list.
 * 	(Information Element)
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
 * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_update_custom_ie(void *cus_ie, int ie_index);

/**
 * @brief  Delete WIFI CUSTOM IE list. (Information Element)
 * @param  None
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_del_custom_ie(void);
#endif

#ifdef CONFIG_PROMISC

/**
 * @brief  Initialize packet filter related data.
 * @param  None
 * @return  None
 */
void wifi_init_packet_filter(void);

/**
 * @brief  Add packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @param[in]  patt: Point to the filter pattern.
 * @param[in]  rule: Point to the filter rule.
 * @return  0 if success, otherwise return -1.
 * @note  For now, the maximum number of filters is 5.
 */
int wifi_add_packet_filter(
	unsigned char					filter_id,
	rtw_packet_filter_pattern_t		*patt,
	rtw_packet_filter_rule_t		rule);

/**
 * @brief  Enable the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 * @note  The filter can be used only if it has been enabled.
 */
int wifi_enable_packet_filter(unsigned char filter_id);

/**
 * @brief  Disable the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_disable_packet_filter(unsigned char filter_id);

/**
 * @brief  Remove the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_remove_packet_filter(unsigned char filter_id);

/**
 * @brief: Filter out the retransmission MIMO packet in promisc mode.
 * @param[in]  enable: set 1 to enable filter retransmission pkt function,
 * 	set 0 to disable this filter function.
 * @param[in]  filter_interval_ms: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value:
 * 	- indicate the time(ms) below which an adjacent pkt received
 * 	will be claimed a retransmission pkt;
 * 	- if it has the same length with the previous pkt, and driver
 * 	will drop all retransmission pkts;
 * @return 0 if success, otherwise return -1.
 * @e.g.  For example, if the packet transmission time interval is 10ms,
 * 	but driver receives two packets with the same length within 3ms
 * 	then the second packet will be dropped if configed as
 * 	wifi_retransmit_packet_filter(1,3).
 */
int wifi_retransmit_packet_filter(u8 enable, u8 filter_interval_ms);

/**
 * @brief  Only receive the packets sent by the specified
 * 	ap and phone in promisc mode.
 * @param[in]  enable: set 1 to enable filter,
 * 	set 0 to disable this filter function.
 * @param[in]  ap_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the ap's mac address.
 * @param[in]  phone_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the phone's mac address.
 * @return  None.
 * @note  Please invoke this function as
 * 	"wifi_filter_by_ap_and_phone_mac(0,NULL,NULL)" before exiting
 * 	promisc mode if you enabled it during the promisc mode.
 */
void wifi_filter_by_ap_and_phone_mac(
	u8		enable,
	void	*ap_mac,
	void	*phone_mac);

/**
 * @brief  config to report ctrl packet or not under promisc mode.
 * @param[in]  enable: set 1 to enable ctrl packet report,
 * 	set 0 to disable ctrl packet report.
 * @return  0 if success, otherwise return -1.
 * @note this function can only be used under promisc mode,
 * 	i.e. between wifi_set_promisc(enable,...,...) and
 * 	wifi_set_promisc(disable,...,...)
 */
int wifi_promisc_ctrl_packet_rpt(u8 enable);
#endif

/**
 * @brief  Get antenna infomation.
 * @param[in]  antenna: Point to the antenna value obtained from driver,
 * 	0 means main antenna, 1 means aux antenna.
 * @return  0 if get successfully, otherwise return -1.
 */
#ifdef CONFIG_ANTENNA_DIVERSITY
int wifi_get_antenna_info(unsigned char *antenna);
#endif // #ifdef CONFIG_ANTENNA_DIVERSITY

/**
 * @brief  config mode of HW indicating packets(mgnt and data)
 * 	and SW reporting packets to wifi_indication().
 * @param[in]  enable
 * 	0) WIFI_INDICATE_DISABLE: disable mode(default), HW only
 * 	   indicate bssid-matched pkts and SW don't report.
 * 	1) WIFI_INDICATE_NORMAL: HW only indicate bssid-matched
 * 	   pkts and SW report.
 * 	2) WIFI_INDICATE_WILD: HW indicate all pkts and SW report.
 * @return  None
 */
void wifi_set_indicate_mgnt(int enable);

/**
 * @brief  send management frame
 * @param[in]  ifname: the wlan interface name,
 * 	can be WLAN0_NAME or WLAN1_NAME
 * @param[in]  buf: the pointer to buf which store the management frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags:
 * 	0: default
 * 	1: P2P_SEND_ACTION_AFTER_PROBE_RSP
 * 	2: means RTW_SEND_AND_WAIT_ACK
 * 	4: means RTW_SEND_BY_HIGH_RATE
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_mgnt(const char *ifname, char *buf, u16 buf_len, u16 flags);

/**
 * @brief  initialize mac address filter list
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_init_mac_filter(void);

/**
 * @brief  add mac address to mac filter list then this address
 * 	will be rejected during authentication
 * @param[in]  hwaddr: the mac address which will be added to
 * 	mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_add_mac_filter(unsigned char* hwaddr);

/**
 * @brief  delete mac address from mac filter list
 * @param[in]  hwaddr: the mac address which will be deleted
 * 	from mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_del_mac_filter(unsigned char* hwaddr);

/**
 * @brief  Get the information of MP driver
 * @param[out]  ability : 0x1 stand for mp driver,
 * 	and 0x0 stand for normal driver
 * @return  RTW_SUCCESS
 */
int wifi_get_drv_ability(uint32_t *ability);

/**
 * @brief  Set channel plan into flash/efuse, must reboot
 * 	after setting channel plan
 * @param[in]  channel_plan : the value of channel plan,
 * 	define in wifi_constants.h
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_channel_plan(uint8_t channel_plan);

/**
 * @brief  Get channel plan from calibration section
 * @param[out]  channel_plan: point to the value of channel plan
 * 	defined in wifi_constants.h
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_channel_plan(uint8_t *channel_plan);

#ifdef CONFIG_AP_MODE
/**
 * @brief  Enable packets forwarding in ap mode
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_enable_forwarding(void);

/**
 * @brief  Disable packets forwarding in ap mode
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_disable_forwarding(void);
#endif

#ifdef CONFIG_CONCURRENT_MODE
/**
 * @brief  Set flag for concurrent mode wlan1 issue_deauth
 * 	when channel switched by wlan0
 * @usage  wifi_set_ch_deauth(0) -> wlan0 wifi_connect -> wifi_set_ch_deauth(1)
 * @param[in]  enable :
 * 	0 for disable (do not want wlan1 to issue deauth)
 * 	1 for enable (wlan1 can issue deauth)
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_ch_deauth(__u8 enable);
#endif

///@name Ameba1 Only
///@{
/**
 * @brief enable AP sending QoS Null0 Data to poll Sta be alive
 * @param[in]  enabled: enabled can be set to 0,1.
 *	- 0 means enable.
 *	- 1 means disable.
 * @return  None
 */
void wifi_set_ap_polling_sta(__u8 enabled);
///@}

#ifdef CONFIG_SW_MAILBOX_EN
/**
 * @brief  interface for bt to set mailbox info to wifi,
 * 	mostly for coexistence usage
 * @param[in]  data : pointer of mailbox data
 * @param[in]  len : length of mailbox data
 * @return  0 if success, otherwise return -1
 */
int mailbox_to_wifi(u8 *data, u8 len);
#endif

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
/**
 * @brief  construct a tcp packet that offload to wlan.
 * 	wlan would keep sending this packet to tcp server.
 * @param[in]  socket_fd : tcp socket
 * @param[in]  content : tcp payload
 * @param[in]  len : tcp payload size
 * @param[in]  interval_ms : send this packeter every interval_ms milliseconds
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_tcp_keep_alive_offload(
	int		socket_fd,
	uint8_t		*content,
	size_t		len,
	uint32_t	interval_ms);
#endif

// WoWlan related
//-------------------------------------------------------------//
#ifdef CONFIG_WOWLAN
typedef struct {
	unsigned int	filter_id;
	unsigned int	polarity;
	unsigned int	type;
	unsigned int	offset;
	unsigned char	*bitmask;
	unsigned char	*pattern;
} wowlan_pattern_param_t;

/**
 * @brief  control wowlan mode enable and disable
 * @param[in]  enable:
 * 	0 means WOWLAN_DISABLE
 * 	1 means WOWLAN_ENABLE
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_ctrl(int enable);

/**
 * @brief  set wowlan customer pattern
 * @param[in]  pattern: refer to struct wowlan_pattern_t
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_set_pattern(wowlan_pattern_t pattern);
#endif

//-------------------------------------------------------------//
#ifdef CONFIG_APP_CTRL_RF_ONOFF
/**
 * @brief  app control rf on and off
 * @param[in]  Status: can be ON(1) and OFF(0)
 * @return  RTW_SUCCESS or RTW_ERROR
 */
extern void rtw_rf_cmd(u32 Status);

/**
 * @brief  get port0 or port1 tsf
 * @param[in]  Port can be 0 or 1
 * @return  tsf result
 */
extern u32 rtw_get_tsf(u32 Port);
#endif

/*
 * @brief get band type
 * @return  the support band type.
 * 	0) WL_BAND_2_4G: only support 2.4G
 * 	1) WL_BAND_5G: only support 5G
 * 	2) WL_BAND_2_4G_5G_BOTH: support both 2.4G and 5G
 */
WL_BAND_TYPE wifi_get_band_type(void);

/**
 * @brief  get an auto channel
 * @param[in]  ifname: the wlan interface name,
 * 	ifname should be WLAN0_NAME or WLAN1_NAME
 * @param[in]  channel_set: the pointer to the channel set
 * 	auto channel will select from this channel set
 * @param[in]  channel_num: the number of channel in channel set
 * @return  the selected auto channel
 */
int wifi_get_auto_chl(
	const char *ifname,
	unsigned char *channel_set,
	unsigned char channel_num);

/**
 * @brief  delete a STA
 * @param[in]  ifname: the wlan interface name,
 * 	ifname should be WLAN0_NAME or WLAN1_NAME
 * @param[in]  hwaddr: the pointer to the MAC address of the STA
 * 	which will be deleted
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  this function should be used when operating as AP
 */
int wifi_del_station(const char *ifname, unsigned char* hwaddr);

#ifdef CONFIG_RTK_MESH
/**
 * @brief  print current mac filter list
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_list_mac_filter(void);

/**
 * @brief  print current mesh ID
 * @param[out]  mesh_id : point to the mesh ID that will get
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_mesh_id(unsigned char *mesh_id);

/**
 * @brief  set mesh RSSI threshold
 * @param[in]  ifname: can be WLAN0_NAME or WLAN1_NAME
 * @param[in]  rssi: the mesh RSSI threshold value that will set
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_mesh_rssi_threshold(const char *ifname, s32 rssi);
#endif

#ifdef __cplusplus
  }
#endif

/*\@}*/

#endif // __WIFI_API_H
