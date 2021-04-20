//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <lwip/sockets.h>
#include <dhcp/dhcps.h>
#include "tcpip.h"
#endif
#endif
#include <platform/platform_stdlib.h>
#include <wifi/wifi_conf.h>
#include <wifi/wifi_util.h>
#include <wifi/wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>

#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT || (defined(CONFIG_JD_SMART) && CONFIG_JD_SMART)
#include "wlan_fast_connect/example_wlan_fast_connect.h"
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
#include "flash_api.h"
#include "device_lock.h"
#endif
#endif
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
#include "at_cmd/atcmd_wifi.h"
#endif
#if defined(CONFIG_PLATFORM_8710C)
#include "platform_opts_bt.h"
#endif
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
#include <wifi_wps_config.h>
#endif

#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
#include "inic_ipc_api.h"
#endif

/******************************************************
 *                    Constants
 ******************************************************/
#define SCAN_USE_SEMAPHORE	0

#define RTW_JOIN_TIMEOUT 20000

#define JOIN_ASSOCIATED             (uint32_t)(1 << 0)
#define JOIN_AUTHENTICATED          (uint32_t)(1 << 1)
#define JOIN_LINK_READY             (uint32_t)(1 << 2)
#define JOIN_SECURITY_COMPLETE      (uint32_t)(1 << 3)
#define JOIN_COMPLETE               (uint32_t)(1 << 4)
#define JOIN_NO_NETWORKS            (uint32_t)(1 << 5)
#define JOIN_WRONG_SECURITY         (uint32_t)(1 << 6)
#define JOIN_HANDSHAKE_DONE         (uint32_t)(1 << 7)
#define JOIN_SIMPLE_CONFIG          (uint32_t)(1 << 8)
#define JOIN_AIRKISS                (uint32_t)(1 << 9)
#define JOIN_CONNECTING             (uint32_t)(1 << 10)

#define REASON_4WAY_HNDSHK_TIMEOUT 15

/*
 * SCAN_DONE_INTERVAL is the interval between each channel scan done,
 * to make AP mode can send beacon during this interval.
 * It is to fix client disconnection when doing wifi scan in AP/concurrent mode.
 * User can fine tune SCAN_DONE_INTERVAL value.
 */
#define SCAN_DONE_INTERVAL 100 //100ms

/*
 * Noted : the scan channel list needs to be modified depending on user's channel plan.
 */
#define SCAN_CHANNEL_NUM 13+25 //2.4GHz + 5GHz

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif

/******************************************************
 *               Variables Definitions
 ******************************************************/
static internal_scan_handler_t scan_result_handler_ptr = {0, 0, 0, RTW_FALSE, 0, 0, 0, 0, 0};
static internal_join_result_t* join_user_data;
unsigned char ap_bssid[6];
#if CONFIG_WIFI_IND_USE_THREAD
static void* disconnect_sema = NULL;
#endif
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;
int error_flag = RTW_UNKNOWN;
uint32_t rtw_join_status;
/* The flag to check if wifi init is completed */
static int _wifi_is_on = 0;
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
u8 is_the_same_ap = 0;
#endif
u8 scan_channel_list[SCAN_CHANNEL_NUM] = {1,2,3,4,5,6,7,8,9,10,11,12,13,
	36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165};

/******************************************************
 *               Variables Definitions
 ******************************************************/
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN
//----------------------------------------------------------------------------//
static int _wifi_connect_local(rtw_network_info_t *pWifi)
{
	int ret = 0;

	if(is_promisc_enabled())
		promisc_set(0, NULL, 0);

	/* lock 4s to forbid suspend under linking */
	rtw_wakelock_timeout(4 *1000);

	if(!pWifi) return -1;
	switch(pWifi->security_type){
		case RTW_SECURITY_OPEN:
			ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_NONE, NULL, 0, 0, 0, 0, NULL, 0);
			break;
		case RTW_SECURITY_WEP_PSK:
		case RTW_SECURITY_WEP_SHARED:
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_SHARED_KEY);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_WEP, NULL, pWifi->key_id, 1 /* set tx key */, 0, 0, pWifi->password, pWifi->password_len);
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
		case RTW_SECURITY_WPA2_TKIP_PSK:
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_TKIP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(WLAN0_NAME, pWifi->password, pWifi->password_len);
			break;
		case RTW_SECURITY_WPA_AES_PSK:
		case RTW_SECURITY_WPA2_AES_PSK:
		case RTW_SECURITY_WPA2_MIXED_PSK:
		case RTW_SECURITY_WPA_WPA2_MIXED:
#ifdef CONFIG_SAE_SUPPORT
		case RTW_SECURITY_WPA3_AES_PSK:
#endif
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(WLAN0_NAME, pWifi->password, pWifi->password_len);
			break;
		default:
			ret = -1;
			RTW_API_INFO("\n\rWIFICONF: security type(0x%x) is not supported.\n\r", pWifi->security_type);
			break;
	}
	if(ret == 0)
		ret = wext_set_ssid(WLAN0_NAME, pWifi->ssid.val, pWifi->ssid.len);
	return ret;
}

static int _wifi_connect_bssid_local(rtw_network_info_t *pWifi)
{
	int ret = 0;
	u8 bssid[12] = {0};

	if(is_promisc_enabled())
		promisc_set(0, NULL, 0);

	/* lock 4s to forbid suspend under linking */
	rtw_wakelock_timeout(4 *1000);

	if(!pWifi) return -1;
	switch(pWifi->security_type){
		case RTW_SECURITY_OPEN:
			ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_NONE, NULL, 0, 0, 0, 0, NULL, 0);
			break;
		case RTW_SECURITY_WEP_PSK:
		case RTW_SECURITY_WEP_SHARED:
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_SHARED_KEY);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_WEP, NULL, pWifi->key_id, 1 /* set tx key */, 0, 0, pWifi->password, pWifi->password_len);
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
		case RTW_SECURITY_WPA2_TKIP_PSK:
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_TKIP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(WLAN0_NAME, pWifi->password, pWifi->password_len);
			break;
		case RTW_SECURITY_WPA_AES_PSK:
		case RTW_SECURITY_WPA2_AES_PSK:
		case RTW_SECURITY_WPA2_MIXED_PSK:
		case RTW_SECURITY_WPA_WPA2_MIXED:
#ifdef CONFIG_SAE_SUPPORT
		case RTW_SECURITY_WPA3_AES_PSK:
#endif
			ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(WLAN0_NAME, pWifi->password, pWifi->password_len);
			break;
		default:
			ret = -1;
			RTW_API_INFO("\n\rWIFICONF: security type(0x%x) is not supported.\n\r", pWifi->security_type);
			break;
	}
	if(ret == 0){
		memcpy(bssid, pWifi->bssid.octet, ETH_ALEN);
		if(pWifi->ssid.len){
			bssid[ETH_ALEN] = '#';
			bssid[ETH_ALEN + 1] = '@';
			memcpy(bssid + ETH_ALEN + 2, &pWifi, sizeof(pWifi));
		}
		ret = wext_set_bssid(WLAN0_NAME, bssid);
	}
	return ret;
}



static void _wifi_no_network_hdl(char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

  if(join_user_data!=NULL)
 	 rtw_join_status = JOIN_NO_NETWORKS | JOIN_CONNECTING;
}

static void _wifi_connected_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
		( void ) buf_len;
		( void ) flags;
		( void ) userdata;
	/* buf detail: ap bssid, buf_len = ETH_ALEN*/
	rtw_memcpy(ap_bssid, buf, ETH_ALEN);

#ifdef CONFIG_ENABLE_EAP
	if(wifi_get_eap_phase()){
		rtw_join_status = JOIN_COMPLETE | JOIN_SECURITY_COMPLETE | JOIN_ASSOCIATED | JOIN_AUTHENTICATED | JOIN_LINK_READY | JOIN_CONNECTING;
		return;
	}
#endif /* CONFIG_ENABLE_EAP */

	if((join_user_data!=NULL)&&((join_user_data->network_info.security_type == RTW_SECURITY_OPEN) ||
		(join_user_data->network_info.security_type == RTW_SECURITY_WEP_PSK) ||
		(join_user_data->network_info.security_type == RTW_SECURITY_WEP_SHARED))){
		rtw_join_status = JOIN_COMPLETE | JOIN_SECURITY_COMPLETE | JOIN_ASSOCIATED | JOIN_AUTHENTICATED | JOIN_LINK_READY | JOIN_CONNECTING;
		rtw_up_sema(&join_user_data->join_sema);
	}else if((join_user_data!=NULL)&&((join_user_data->network_info.security_type == RTW_SECURITY_WPA2_AES_PSK) )){
		rtw_join_status = JOIN_COMPLETE | JOIN_SECURITY_COMPLETE | JOIN_ASSOCIATED | JOIN_AUTHENTICATED | JOIN_LINK_READY | JOIN_CONNECTING;
	}
}
static void _wifi_handshake_done_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

	rtw_join_status = JOIN_COMPLETE | JOIN_SECURITY_COMPLETE | JOIN_ASSOCIATED | JOIN_AUTHENTICATED | JOIN_LINK_READY|JOIN_HANDSHAKE_DONE | JOIN_CONNECTING;
	if(join_user_data != NULL)
		rtw_up_sema(&join_user_data->join_sema);
}

static void _wifi_disconn_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

	u16 disconn_reason;
	if (buf != NULL){
		/* buf detail: mac addr + disconn_reason, buf_len = ETH_ALEN+2*/
		disconn_reason =*(u16*)(buf+6);
	} else {
		RTW_API_INFO("\n\r _wifi_disconn_hdl buf is NULL!\n\r");
		return;
	}
	if(join_user_data != NULL){
		if(join_user_data->network_info.security_type == RTW_SECURITY_OPEN){

			if(rtw_join_status & JOIN_NO_NETWORKS)
				error_flag = RTW_NONE_NETWORK;

		}else if(join_user_data->network_info.security_type == RTW_SECURITY_WEP_PSK){

			if(rtw_join_status & JOIN_NO_NETWORKS)
				error_flag = RTW_NONE_NETWORK;

			else if(rtw_join_status == 0)
		 		error_flag = RTW_CONNECT_FAIL;

		}else if(join_user_data->network_info.security_type == RTW_SECURITY_WPA2_AES_PSK){

			if(rtw_join_status & JOIN_NO_NETWORKS)
				error_flag = RTW_NONE_NETWORK;

			else if(rtw_join_status == 0)
		 		error_flag = RTW_CONNECT_FAIL;

			else if(rtw_join_status == (JOIN_COMPLETE | JOIN_SECURITY_COMPLETE | JOIN_ASSOCIATED | JOIN_AUTHENTICATED | JOIN_LINK_READY | JOIN_CONNECTING))
			{
				 if(disconn_reason == REASON_4WAY_HNDSHK_TIMEOUT)
					error_flag = RTW_4WAY_HANDSHAKE_TIMEOUT;
				else
					error_flag = RTW_WRONG_PASSWORD;
			}
		}

	}else{
		if(error_flag == RTW_NO_ERROR) //wifi_disconn_hdl will be dispatched one more time after join_user_data = NULL add by frankie
			error_flag = RTW_UNKNOWN;
	}

#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	dhcp_stop(&xnetif[0]);
	netif_set_link_down(&xnetif[0]);
#endif
#endif

	if(join_user_data != NULL)
		rtw_up_sema(&join_user_data->join_sema);
	//RTW_API_INFO("\r\nWiFi Disconnect. Error flag is %d.\n", error_flag);

// Need to use sema to make sure wifi_disconn_hdl invoked before setting join_user_data when connecting to another AP
#if CONFIG_WIFI_IND_USE_THREAD
	if(disconnect_sema != NULL){
		rtw_up_sema(&disconnect_sema);
	}
#endif
}

static void _check_is_the_same_ap()
{
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
	flash_t		flash;
	struct wlan_fast_reconnect data;
	u8 *ifname[1] = {WLAN0_NAME};
	rtw_wifi_setting_t setting;

	if(wifi_get_setting((const char*)ifname[0],&setting) || setting.mode == RTW_MODE_AP){
		RTW_API_INFO("\r\n %s():wifi_get_setting fail or ap mode", __func__);
		return;
	}

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, FAST_RECONNECT_DATA, sizeof(struct wlan_fast_reconnect), (uint8_t *)&data);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	if(strncmp((const char*)data.psk_essid,(const char*)setting.ssid,strlen((char const*)setting.ssid)) == 0){
		is_the_same_ap = 1;
	}else{
		is_the_same_ap = 0;
	}
#endif
}

//----------------------------------------------------------------------------//
int wifi_connect(
	char				*ssid,
	rtw_security_t		security_type,
	char				*password,
	int					ssid_len,
	int					password_len,
	int					key_id,
	void				*semaphore)
{
	_sema join_semaphore;
	rtw_result_t result = RTW_SUCCESS;
	u8 wep_hex = 0;
	u8 wep_pwd[14] = {0};

	if(rtw_join_status & JOIN_CONNECTING){
		if(wifi_disconnect() < 0){
			RTW_API_INFO("\nwifi_disconnect Operation failed!");
			return RTW_ERROR;
		}
		while(rtw_join_status & JOIN_CONNECTING){
			rtw_msleep_os(1);
		}
	}
	rtw_join_status = JOIN_CONNECTING;

	if(rtw_join_status & JOIN_SIMPLE_CONFIG || rtw_join_status & JOIN_AIRKISS){
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_BUSY;
	}

	error_flag = RTW_UNKNOWN ;//clear for last connect status
	rtw_memset(ap_bssid, 0, ETH_ALEN);
	if ( ( ( ( password_len >  RTW_MAX_PSK_LEN ) ||
		( password_len <  RTW_MIN_PSK_LEN ) ) &&
		( ( security_type == RTW_SECURITY_WPA_TKIP_PSK ) ||
		( security_type == RTW_SECURITY_WPA_AES_PSK ) ||
		( security_type == RTW_SECURITY_WPA2_AES_PSK ) ||
		( security_type == RTW_SECURITY_WPA2_TKIP_PSK ) ||
		( security_type == RTW_SECURITY_WPA2_MIXED_PSK ) ||
		( security_type == RTW_SECURITY_WPA_WPA2_MIXED )
#ifdef CONFIG_SAE_SUPPORT
		|| ( security_type == RTW_SECURITY_WPA3_AES_PSK)
#endif
	) ) ) {
		error_flag = RTW_WRONG_PASSWORD;
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_INVALID_KEY;
	}

	if ((security_type == RTW_SECURITY_WEP_PSK)||
		(security_type ==RTW_SECURITY_WEP_SHARED)) {
		if ((password_len != 5) && (password_len != 13) &&
		    (password_len != 10)&& (password_len != 26)) {
		    	error_flag = RTW_WRONG_PASSWORD;
			rtw_join_status &= ~JOIN_CONNECTING;
			return RTW_INVALID_KEY;
		} else {

			if(password_len == 10) {

				u32 p[5] = {0};
				u8 i = 0;
				sscanf((const char*)password, "%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4]);
				for(i=0; i< 5; i++)
					wep_pwd[i] = (u8)p[i];
				wep_pwd[5] = '\0';
				password_len = 5;
				wep_hex = 1;
			} else if (password_len == 26) {
				u32 p[13] = {0};
				u8 i = 0;
				sscanf((const char*)password, "%02x%02x%02x%02x%02x%02x%02x"\
					 "%02x%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4],\
					  &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11], &p[12]);
				for(i=0; i< 13; i++)
					wep_pwd[i] = (u8)p[i];
				wep_pwd[13] = '\0';
				password_len = 13;
				wep_hex = 1;
			}
		}
	}

	internal_join_result_t *join_result = (internal_join_result_t *)rtw_zmalloc(sizeof(internal_join_result_t));
	if(!join_result) {
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_NOMEM;
	}

	join_result->network_info.ssid.len = ssid_len > 32 ? 32 : ssid_len;
	rtw_memcpy(join_result->network_info.ssid.val, ssid, join_result->network_info.ssid.len);

	join_result->network_info.password_len = password_len;
	if(password_len) {
		/* add \0 to the end */
		join_result->network_info.password = rtw_zmalloc(password_len + 1);
		if(!join_result->network_info.password) {
			result =(rtw_result_t) RTW_NOMEM;
			goto error;
		}
		if (0 == wep_hex)
			rtw_memcpy(join_result->network_info.password, password, password_len);
		else
			rtw_memcpy(join_result->network_info.password, wep_pwd, password_len);

	}

	join_result->network_info.security_type = security_type;
	join_result->network_info.key_id = key_id;

	if(semaphore == NULL) {
		rtw_init_sema( &join_result->join_sema, 0 );
		if(!join_result->join_sema){
			result =(rtw_result_t) RTW_NORESOURCE;
			goto error;
		}
		join_semaphore = join_result->join_sema;
	} else {
		join_result->join_sema = semaphore;
	}
	wifi_reg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl,NULL);
	wifi_reg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, _wifi_disconn_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl, NULL);

// if is connected to ap, would trigger disconn_hdl but need to make sure it is invoked before setting join_user_data
#if CONFIG_WIFI_IND_USE_THREAD
	if(wifi_is_connected_to_ap() == RTW_SUCCESS){
		rtw_init_sema( &disconnect_sema, 0 );
	}
#endif

	result = _wifi_connect_local(&join_result->network_info);
	if(result != 0)
		goto error;

#if CONFIG_WIFI_IND_USE_THREAD
	if(disconnect_sema != NULL){
		rtw_down_timeout_sema(&disconnect_sema, 50);
		rtw_free_sema( &disconnect_sema);
	}
#endif

	join_user_data = join_result;

	if(semaphore == NULL) {
// for eap connection, timeout should be longer (default value in wpa_supplicant: 60s)
#ifdef CONFIG_ENABLE_EAP
		if(wifi_get_eap_phase()){
			if(rtw_down_timeout_sema( &join_result->join_sema, 60000 ) == RTW_FALSE) {
				RTW_API_INFO("RTW API: Join bss timeout\r\n");
				if(password_len) {
					rtw_free(join_result->network_info.password);
				}
				result = RTW_TIMEOUT;
				goto error;
			} else {
				if(wifi_is_connected_to_ap( ) != RTW_SUCCESS) {
					result = RTW_ERROR;
					goto error;
				}
			}
		}
		else
#endif
		if(rtw_down_timeout_sema( &join_result->join_sema, RTW_JOIN_TIMEOUT ) == RTW_FALSE) {
			RTW_API_INFO("RTW API: Join bss timeout\r\n");
			if(password_len) {
				rtw_free(join_result->network_info.password);
			}
			result = RTW_TIMEOUT;
			goto error;
		} else {
			if(join_result->network_info.password_len) {
				rtw_free(join_result->network_info.password);
			}
			if(wifi_is_connected_to_ap( ) != RTW_SUCCESS) {
				result = RTW_ERROR;
				goto error;
			}
		}
	}

	result = RTW_SUCCESS;
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	netif_set_link_up(&xnetif[0]);
#endif
#endif

#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT || (defined(CONFIG_JD_SMART) && CONFIG_JD_SMART)
	#if CONFIG_FAST_DHCP
		_check_is_the_same_ap();
	#else
		restore_wifi_info_to_flash(0, 0);
	#endif
#endif

error:
	if(semaphore == NULL){
		rtw_free_sema( &join_semaphore);
	}
	join_user_data = NULL;
	rtw_free((u8*)join_result);
	wifi_unreg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl);
	rtw_join_status &= ~JOIN_CONNECTING;
	return result;
}

int wifi_connect_bssid(
	unsigned char 		bssid[ETH_ALEN],
	char 				*ssid,
	rtw_security_t	security_type,
	char 				*password,
	int 				bssid_len,
	int 				ssid_len,
	int 				password_len,
	int 				key_id,
	void 				*semaphore)
{
	_sema join_semaphore;
	rtw_result_t result = RTW_SUCCESS;
	u8 wep_hex = 0;
	u8 wep_pwd[14] = {0};

	if(rtw_join_status & JOIN_CONNECTING){
		if(wifi_disconnect() < 0){
			RTW_API_INFO("\nwifi_disconnect Operation failed!");
			return RTW_ERROR;
		}
		while(rtw_join_status & JOIN_CONNECTING){
			rtw_mdelay_os(1);
		}
	}
	rtw_join_status = JOIN_CONNECTING;

	if(rtw_join_status & JOIN_SIMPLE_CONFIG || rtw_join_status & JOIN_AIRKISS){
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_BUSY;
	}

	error_flag = RTW_UNKNOWN;//clear for last connect status
	rtw_memset(ap_bssid, 0, ETH_ALEN);
	internal_join_result_t *join_result = (internal_join_result_t *)rtw_zmalloc(sizeof(internal_join_result_t));
	if(!join_result) {
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_NOMEM;
	}
	if(ssid_len && ssid){
		join_result->network_info.ssid.len = ssid_len > 32 ? 32 : ssid_len;
		rtw_memcpy(join_result->network_info.ssid.val, ssid, join_result->network_info.ssid.len);
	}
	rtw_memcpy(join_result->network_info.bssid.octet, bssid, bssid_len);

	if ( ( ( ( password_len >  RTW_MAX_PSK_LEN ) ||
             ( password_len <  RTW_MIN_PSK_LEN ) ) &&
           ( ( security_type == RTW_SECURITY_WPA_TKIP_PSK ) ||
             ( security_type == RTW_SECURITY_WPA_AES_PSK ) ||
             ( security_type == RTW_SECURITY_WPA2_AES_PSK ) ||
             ( security_type == RTW_SECURITY_WPA2_TKIP_PSK ) ||
             ( security_type == RTW_SECURITY_WPA2_MIXED_PSK ) ||
             ( security_type == RTW_SECURITY_WPA_WPA2_MIXED )
#ifdef CONFIG_SAE_SUPPORT
          || ( security_type == RTW_SECURITY_WPA3_AES_PSK)
#endif
	) ) ) {
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_INVALID_KEY;
	}

	if ((security_type == RTW_SECURITY_WEP_PSK)||
	(security_type ==RTW_SECURITY_WEP_SHARED)) {
	if ((password_len != 5) && (password_len != 13) &&
	    (password_len != 10)&& (password_len != 26)) {
		rtw_join_status &= ~JOIN_CONNECTING;
		return RTW_INVALID_KEY;
	} else {

		if(password_len == 10) {
			u32 p[5] = {0};
			u8 i = 0;
			sscanf((const char*)password, "%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4]);
			for(i=0; i< 5; i++)
				wep_pwd[i] = (u8)p[i];
			wep_pwd[5] = '\0';
			password_len = 5;
			wep_hex = 1;
		} else if (password_len == 26) {
			u32 p[13] = {0};
			u8 i = 0;
			sscanf((const char*)password, "%02x%02x%02x%02x%02x%02x%02x"\
				 "%02x%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4],\
				  &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11], &p[12]);
			for(i=0; i< 13; i++)
				wep_pwd[i] = (u8)p[i];
			wep_pwd[13] = '\0';
			password_len = 13;
			wep_hex = 1;
		}
	}
}
	join_result->network_info.password_len = password_len;
	if(password_len) {
		/* add \0 to the end */
		join_result->network_info.password = rtw_zmalloc(password_len + 1);
		if(!join_result->network_info.password) {
			rtw_join_status &= ~JOIN_CONNECTING;
			return RTW_NOMEM;
		}
		if (0 == wep_hex)
			rtw_memcpy(join_result->network_info.password, password, password_len);
		else
			rtw_memcpy(join_result->network_info.password, wep_pwd, password_len);
	}

		join_result->network_info.security_type = security_type;
		join_result->network_info.key_id = key_id;

	if(semaphore == NULL) {
		rtw_init_sema( &join_result->join_sema, 0 );
		if(!join_result->join_sema){
			rtw_join_status &= ~JOIN_CONNECTING;
		    return RTW_NORESOURCE;
		}
		join_semaphore = join_result->join_sema;
	} else {
		join_result->join_sema = semaphore;
	}
	wifi_reg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl,NULL);
	wifi_reg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, _wifi_disconn_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl, NULL);

	_wifi_connect_bssid_local(&join_result->network_info);

	join_user_data = join_result;

	if(semaphore == NULL) {
		if(rtw_down_timeout_sema( &join_result->join_sema, RTW_JOIN_TIMEOUT ) == RTW_FALSE) {
			RTW_API_INFO("RTW API: Join bss timeout\r\n");
			if(password_len) {
				rtw_free(join_result->network_info.password);
			}
			rtw_free((u8*)join_result);
			rtw_free_sema( &join_semaphore);
			result = RTW_TIMEOUT;
			goto error;
		} else {
			rtw_free_sema( &join_semaphore );
			if(join_result->network_info.password_len) {
				rtw_free(join_result->network_info.password);
			}
			rtw_free((u8*)join_result);
			if( wifi_is_connected_to_ap( ) != RTW_SUCCESS) {
				result = RTW_ERROR;
				goto error;
			}
		}
	}

	result = RTW_SUCCESS;

#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	netif_set_link_up(&xnetif[0]);
#endif
#endif

#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT || (defined(CONFIG_JD_SMART) && CONFIG_JD_SMART)
	#if CONFIG_FAST_DHCP
		_check_is_the_same_ap();
	#else
		restore_wifi_info_to_flash(0, 0);
	#endif
#endif

error:
	join_user_data = NULL;
	wifi_unreg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl);
	rtw_join_status &= ~JOIN_CONNECTING;
	return result;
}

int wifi_disconnect(void)
{
	int ret = 0;

	//set MAC address last byte to 1 since driver will filter the mac with all 0x00 or 0xff
	//add extra 2 zero byte for check of #@ in wext_set_bssid()
	const __u8 null_bssid[ETH_ALEN + 2] = {0, 0, 0, 0, 0, 1, 0, 0};

	if (wext_set_bssid(WLAN0_NAME, null_bssid) < 0){
		RTW_API_INFO("\n\rWEXT: Failed to set bogus BSSID to disconnect");
		ret = -1;
	}
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_connected_to_ap( void )
{
	return rltk_wlan_is_connected_to_ap();
}

//----------------------------------------------------------------------------//
int wifi_is_up(rtw_interface_t interface)
{
    switch (interface)
	{
		case RTW_AP_INTERFACE:
			switch (wifi_mode)
			{
				case RTW_MODE_STA_AP:
					return (rltk_wlan_running(WLAN1_IDX) && _wifi_is_on);
				case RTW_MODE_STA:
					return 0;
				default:
					return (rltk_wlan_running(WLAN0_IDX) && _wifi_is_on);
			}
		case RTW_STA_INTERFACE:
			switch (wifi_mode)
			{
				case RTW_MODE_AP:
					return 0;
				default:
					return (rltk_wlan_running(WLAN0_IDX) && _wifi_is_on);
			}
		default:
			return 0;
    }
}

int wifi_is_running(unsigned char wlan_idx)
{
	return rltk_wlan_running(wlan_idx);
}

int wifi_is_ready_to_transceive(rtw_interface_t interface)
{
    switch ( interface )
    {
        case RTW_AP_INTERFACE:
            return ( wifi_is_up(interface) == RTW_TRUE ) ? RTW_SUCCESS : RTW_ERROR;

        case RTW_STA_INTERFACE:
            switch ( error_flag)
            {
		  case RTW_NO_ERROR:
			return RTW_SUCCESS;

		  default:
			return RTW_ERROR;
            }
        default:
            return RTW_ERROR;
    }
}

//----------------------------------------------------------------------------//

int wifi_set_country(rtw_country_code_t country_code)
{
	int ret;

	ret = wext_set_country(WLAN0_NAME, country_code);

	return ret;
}

//----------------------------------------------------------------------------//
int wifi_change_channel_plan(uint8_t channel_plan)
{
	int ret;

	ret = rltk_wlan_change_channel_plan(channel_plan);

	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel_plan(uint8_t channel_plan)
{
	const char * ifname = WLAN0_NAME;
	int ret = 0;
	char buf[24];

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 24, "set_ch_plan %x", channel_plan);
	ret = wext_private_command(ifname, buf, 0);
	return ret;
}

int wifi_get_channel_plan(uint8_t *channel_plan)
{
	int ret = 0;
	char buf[24];
	char *ptmp;

	rtw_memset(buf, 0, sizeof(buf));
	rtw_memcpy(buf, "get_ch_plan", 11);
	ret = wext_private_command_with_retval(WLAN0_NAME, buf, buf, 24);
	*channel_plan = strtoul(buf, &ptmp, 16);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel(int channel)
{
	return wext_set_channel(WLAN0_NAME, channel);
}

int wifi_get_channel(int *channel)
{
	return wext_get_channel(WLAN0_NAME, (u8*)channel);
}

//----------------------------------------------------------------------------//
_WEAK void wifi_set_mib(void)
{
	// adaptivity
	wext_set_adaptivity(RTW_ADAPTIVITY_DISABLE);
	//trp tis
	wext_set_trp_tis(DISABLE);
#ifdef CONFIG_SAE_SUPPORT
	// set to 'ENABLE' when using WPA3
	wext_set_support_wpa3(ENABLE);
#endif
}

//----------------------------------------------------------------------------//
_WEAK void wifi_set_country_code(void)
{
	//wifi_set_country(RTW_COUNTRY_US); // 2.4G only
	//wifi_change_channel_plan(0x25); // Support 2.4G and 5G, ex: 0x25 = 2G_FCC1, 5G_FCC1
}
//----------------------------------------------------------------------------//

int wifi_get_last_error(void)
{
   return error_flag;
}

int wifi_on(rtw_mode_t mode)
{
	int ret = 1;
	int timeout = 20;
	int idx;
	int devnum = 1;
	static int event_init = 0;

	device_mutex_lock(RT_DEV_LOCK_WLAN);
	if(rltk_wlan_running(WLAN0_IDX)) {
		RTW_API_INFO("\n\rWIFI is already running");
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return 1;
	}

	if(event_init == 0){
		init_event_callback_list();
		event_init = 1;
	}

	wifi_mode = mode;

	if(mode == RTW_MODE_STA_AP)
		devnum = 2;

	// set wifi mib
	wifi_set_mib();
	RTW_API_INFO("\n\rInitializing WIFI ...");
	for(idx=0;idx<devnum;idx++){
		ret = rltk_wlan_init(idx, mode);
		if(ret <0){
			wifi_mode = RTW_MODE_NONE;
			device_mutex_unlock(RT_DEV_LOCK_WLAN);
			return ret;
		}
	}
	for(idx=0;idx<devnum;idx++){
		ret = rltk_wlan_start(idx);
		if(ret == 0) _wifi_is_on = 1;
		if(ret <0){
			RTW_API_INFO("\n\rERROR: Start WIFI Failed!");
			rltk_wlan_deinit();
			wifi_mode = RTW_MODE_NONE;
			device_mutex_unlock(RT_DEV_LOCK_WLAN);
			return ret;
		}
	}
	device_mutex_unlock(RT_DEV_LOCK_WLAN);

	while(1) {
		if(rltk_wlan_running(devnum-1)) {
			RTW_API_INFO("\n\rWIFI initialized\n");
			wifi_set_country_code();
			break;
		}

		if(timeout == 0) {
			RTW_API_INFO("\n\rERROR: Init WIFI timeout!");
			break;
		}

		rtw_msleep_os(1000);
		timeout --;
	}

	#if CONFIG_LWIP_LAYER
	#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
	#else
	netif_set_up(&xnetif[0]);
	if(mode == RTW_MODE_AP)
		netif_set_link_up(&xnetif[0]);
	else	 if(mode == RTW_MODE_STA_AP) {
		netif_set_up(&xnetif[1]);
		netif_set_link_up(&xnetif[1]);
	}
	#endif
	#endif

	return ret;
}

int wifi_off(void)
{
	int ret = 0;
	int timeout = 20;

	if((rltk_wlan_running(WLAN0_IDX) == 0) &&
		(rltk_wlan_running(WLAN1_IDX) == 0)) {
		RTW_API_INFO("\n\rWIFI is not running");
		return 0;
	}
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	dhcps_deinit();
	LwIP_DHCP(0, DHCP_STOP);
	netif_set_down(&xnetif[0]);
	netif_set_down(&xnetif[1]);
#endif
#endif
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
	if((wifi_mode ==  RTW_MODE_AP) || (wifi_mode == RTW_MODE_STA_AP))
		wpas_wps_deinit();
#endif
	RTW_API_INFO("\n\rDeinitializing WIFI ...");
	device_mutex_lock(RT_DEV_LOCK_WLAN);
	rltk_wlan_deinit();
	_wifi_is_on = 0;
	device_mutex_unlock(RT_DEV_LOCK_WLAN);

	while(1) {
		if((rltk_wlan_running(WLAN0_IDX) == 0) &&
			(rltk_wlan_running(WLAN1_IDX) == 0)) {
			RTW_API_INFO("\n\rWIFI deinitialized");
			break;
		}

		if(timeout == 0) {
			RTW_API_INFO("\n\rERROR: Deinit WIFI timeout!");
			break;
		}

		rtw_msleep_os(1000);
		timeout --;
	}

	wifi_mode = RTW_MODE_NONE;

	return ret;
}

int wifi_set_mode(rtw_mode_t mode)
{
	int ret = 0;
#ifdef CONFIG_WLAN_SWITCH_MODE
	rtw_mode_t curr_mode, next_mode;
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	u8 autoreconnect_mode;
#endif
#endif
	device_mutex_lock(RT_DEV_LOCK_WLAN);

	if((rltk_wlan_running(WLAN0_IDX) == 0) &&
		(rltk_wlan_running(WLAN1_IDX) == 0)) {
		RTW_API_INFO("\n\r[%s] WIFI is not running",__FUNCTION__);
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return -1;
	}

#ifdef CONFIG_WLAN_SWITCH_MODE
#ifndef CONFIG_INIC_IPC
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	wifi_get_autoreconnect(&autoreconnect_mode);
	if(autoreconnect_mode != RTW_AUTORECONNECT_DISABLE){
		wifi_set_autoreconnect(RTW_AUTORECONNECT_DISABLE);

		// if set to AP mode, delay until the autoconnect task is finished
		if((mode == RTW_MODE_AP)||(mode == RTW_MODE_STA_AP)){
			while(param_indicator != NULL){
				rtw_msleep_os(2);
			}
		}
	}
#endif
#endif
	curr_mode = wifi_mode;
	next_mode = mode;
	ret = rltk_set_mode_prehandle(curr_mode, next_mode, WLAN0_NAME);
	if(ret < 0) goto Exit;
#endif

	if((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_AP)){
		RTW_API_INFO("\n\r[%s] WIFI Mode Change: STA-->AP",__FUNCTION__);

		wifi_disconnect();
		//must add this delay, because this API may have higher priority, wifi_disconnect will rely RTW_CMD task, may not be excuted immediately.
		rtw_msleep_os(50);

#if CONFIG_LWIP_LAYER
		netif_set_link_up(&xnetif[0]);
#endif

		wifi_mode = mode;
	}else if((wifi_mode == RTW_MODE_AP) && (mode ==RTW_MODE_STA)){
		RTW_API_INFO("\n\r[%s] WIFI Mode Change: AP-->STA",__FUNCTION__);

		ret = wext_set_mode(WLAN0_NAME, IW_MODE_INFRA);
		if(ret < 0) goto Exit;

		rtw_msleep_os(50);

#if CONFIG_LWIP_LAYER
		netif_set_link_down(&xnetif[0]);
#endif

		wifi_mode = mode;

	}else if ((wifi_mode == RTW_MODE_AP) && (mode == RTW_MODE_AP)){
		RTW_API_INFO("\n\rWIFI Mode Change: AP-->AP");
		ret = wext_set_mode(WLAN0_NAME, IW_MODE_INFRA);
		if(ret < 0) goto Exit;

		vTaskDelay(50);

	}else if ((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_STA)){
		RTW_API_INFO("\n\rWIFI Mode No Need To Change: STA -->STA");
	}else if ((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_PROMISC)){
		RTW_API_INFO("\n\rWIFI Mode Change: STA-->PROMISC");
		unsigned char ssid[33];
		if(wext_get_ssid(WLAN0_NAME, ssid) > 0)
			wifi_disconnect();
	}else if ((wifi_mode == RTW_MODE_AP) && (mode == RTW_MODE_PROMISC)){
		RTW_API_INFO("\n\rWIFI Mode Change: AP-->PROMISC");//Same as AP--> STA
		ret = wext_set_mode(WLAN0_NAME, IW_MODE_INFRA);
		if(ret < 0) goto Exit;
		rtw_msleep_os(50);
#if CONFIG_LWIP_LAYER
		netif_set_link_down(&xnetif[0]);
#endif
		wifi_mode = mode;
	}
#ifdef CONFIG_CONCURRENT_MODE
#ifdef CONFIG_WLAN_SWITCH_MODE
	else if((wifi_mode == RTW_MODE_STA_AP) && (mode == RTW_MODE_STA)){
		RTW_API_INFO("\n\rWIFI Mode Change: CONCURRENT-->STA");
#if CONFIG_LWIP_LAYER
		dhcps_deinit();
		netif_set_down(&xnetif[1]);
		netif_set_link_down(&xnetif[1]);
#endif
	}else if((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_STA_AP)){
		RTW_API_INFO("\n\rWIFI Mode Change: STA-->CONCURRENT");
#if CONFIG_LWIP_LAYER
		dhcps_init(&xnetif[1]);
		netif_set_up(&xnetif[1]);
		netif_set_link_up(&xnetif[1]);
#endif
		wifi_mode = mode;
	}
#endif
#endif
	else{
		RTW_API_INFO("\n\rWIFI Mode Change: not support");
		goto Exit;
	}

#ifdef CONFIG_WLAN_SWITCH_MODE
	ret = rltk_set_mode_posthandle(curr_mode, next_mode, WLAN0_NAME);
	if(ret < 0) goto Exit;
#ifdef CONFIG_CONCURRENT_MODE
	if((wifi_mode == RTW_MODE_STA_AP) && (mode == RTW_MODE_STA)){
		wifi_mode = RTW_MODE_STA;
	}
#endif
#ifndef CONFIG_INIC_IPC
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	/* enable auto reconnect */
	if(autoreconnect_mode != RTW_AUTORECONNECT_DISABLE){
		wifi_set_autoreconnect(autoreconnect_mode);
	}
#endif
#endif
#endif

	device_mutex_unlock(RT_DEV_LOCK_WLAN);
	return 0;

Exit:
#ifdef CONFIG_WLAN_SWITCH_MODE
#ifndef CONFIG_INIC_IPC
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	/* enable auto reconnect */
	if(autoreconnect_mode != RTW_AUTORECONNECT_DISABLE){
		wifi_set_autoreconnect(autoreconnect_mode);
	}
#endif
#endif
#endif
	device_mutex_unlock(RT_DEV_LOCK_WLAN);
	return -1;
}

static void _wifi_ap_sta_assoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO

}
static void _wifi_ap_sta_disassoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO
}

int wifi_start_ap(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel)
{
	const char *ifname = WLAN0_NAME;
	int ret = 0;

	if((ssid_len < 0)|| (ssid_len > 32)){
		printf("Error: SSID should be 0-32 characters\r\n");
		ret = RTW_BADARG;
		goto exit;
	}

	if(password == NULL){
		if(security_type != RTW_SECURITY_OPEN){
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}
	if(security_type != RTW_SECURITY_OPEN){
		if(password_len <= RTW_MAX_PSK_LEN &&
			password_len >= RTW_MIN_PSK_LEN){
			if(password_len == RTW_MAX_PSK_LEN){//password_len=64 means pre-shared key, pre-shared key should be 64 hex characters
				unsigned char i,j;
				for(i = 0;i < 64;i++){
					j = password[i];
					if(!((j >='0' && j<='9') || (j >='A' && j<='F') || (j >='a' && j<='f'))){
						printf("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
						ret = RTW_INVALID_KEY;
						goto exit;
					}
				}
			}
		}
#ifdef CONFIG_FPGA
		else if((password_len == 5)&&(security_type == RTW_SECURITY_WEP_PSK)){
		}
#endif
		else{
			printf("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}

#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}

	if(is_promisc_enabled())
		promisc_set(0, NULL, 0);

	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, _wifi_ap_sta_assoc_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, _wifi_ap_sta_disassoc_hdl, NULL);

	ret = wext_set_mode(ifname, IW_MODE_MASTER);
	if(ret < 0) goto exit;
	ret = wext_set_channel(ifname, channel);	//Set channel before starting ap
	if(ret < 0) goto exit;

	switch(security_type) {
		case RTW_SECURITY_OPEN:
			break;
#if defined(CONFIG_FPGA) && CONFIG_FPGA
		case RTW_SECURITY_WEP_PSK:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_WEP, NULL, 0, 1, 0, 0, (u8*)password, password_len);
			break;
		case RTW_SECURITY_WPA2_TKIP_PSK:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_TKIP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(ifname, (u8*)password, password_len);
			break;
#endif
		case RTW_SECURITY_WPA2_AES_PSK:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(ifname, (u8*)password, password_len);
			break;
#ifdef CONFIG_IEEE80211W
		case RTW_SECURITY_WPA2_AES_CMAC:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_AES_CMAC, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(ifname, (u8*)password, password_len);
			break;
#endif
		default:
			ret = -1;
			RTW_API_INFO("\n\rWIFICONF: security type is not supported");
			break;
	}
	if(ret < 0) goto exit;

	ret = wext_set_ap_ssid(ifname, (u8*)ssid, ssid_len);
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
	wpas_wps_init(ifname);
#endif
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	if(wifi_mode == RTW_MODE_STA_AP)
		netif_set_link_up(&xnetif[1]);
	else
		netif_set_link_up(&xnetif[0]);
#endif
#endif
#endif
exit:

	return ret;
}

int wifi_start_ap_with_hidden_ssid(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel)
{
	const char *ifname = WLAN0_NAME;
	int ret = 0;

	if((ssid_len < 0)|| (ssid_len > 32)){
		printf("Error: SSID should be 0-32 characters\r\n");
		ret = RTW_BADARG;
		goto exit;
	}

	if(password == NULL){
		if(security_type != RTW_SECURITY_OPEN){
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}
	if(security_type != RTW_SECURITY_OPEN){
		if(password_len <= RTW_MAX_PSK_LEN &&
			password_len >= RTW_MIN_PSK_LEN){
			if(password_len == RTW_MAX_PSK_LEN){//password_len=64 means pre-shared key, pre-shared key should be 64 hex characters
				unsigned char i,j;
				for(i = 0;i < 64;i++){
					j = password[i];
					if(!((j >='0' && j<='9') || (j >='A' && j<='F') || (j >='a' && j<='f'))){
						printf("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
						ret = RTW_INVALID_KEY;
						goto exit;
					}
				}
			}
		}
#ifdef CONFIG_FPGA
		else if((password_len == 5)&&(security_type == RTW_SECURITY_WEP_PSK)){
		}
#endif
		else{
			printf("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}

#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}

	if(is_promisc_enabled())
		promisc_set(0, NULL, 0);

	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, _wifi_ap_sta_assoc_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, _wifi_ap_sta_disassoc_hdl, NULL);

	ret = wext_set_mode(ifname, IW_MODE_MASTER);
	if(ret < 0) goto exit;
	ret = wext_set_channel(ifname, channel);	//Set channel before starting ap
	if(ret < 0) goto exit;

	switch(security_type) {
		case RTW_SECURITY_OPEN:
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
			if(ret == 0)
				ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
			if(ret == 0)
				ret = wext_set_passphrase(ifname, (u8*)password, password_len);
			break;
		default:
			ret = -1;
			RTW_API_INFO("\n\rWIFICONF: security type is not supported");
			break;
	}
	if(ret < 0) goto exit;

	ret = rltk_set_hidden_ssid(ifname, 1);
	if(ret < 0) goto exit;

	ret = wext_set_ap_ssid(ifname, (u8*)ssid, ssid_len);
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
	wpas_wps_init(ifname);
#endif
#endif
exit:

	return ret;
}

static void _wifi_scan_each_report_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

	int i =0;
	int j =0;
	int insert_pos = 0;
	rtw_scan_result_t** result_ptr = (rtw_scan_result_t**)buf;
	rtw_scan_result_t* temp = NULL;
	/* buf detail: scan buffer/rtw_scan_result_t, buf_len = size of rtw_scan_result_t structure*/

	for(i=0; i<scan_result_handler_ptr.scan_cnt; i++){
		if(CMP_MAC(scan_result_handler_ptr.pap_details[i]->BSSID.octet, (*result_ptr)->BSSID.octet)){
			if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[i]->signal_strength){
				temp = scan_result_handler_ptr.pap_details[i];
				for(j = i-1; j >= 0; j--){
					if(scan_result_handler_ptr.pap_details[j]->signal_strength >= (*result_ptr)->signal_strength)
						break;
					else
						scan_result_handler_ptr.pap_details[j+1] = scan_result_handler_ptr.pap_details[j];
				}
				scan_result_handler_ptr.pap_details[j+1] = temp;
				scan_result_handler_ptr.pap_details[j+1]->signal_strength = (*result_ptr)->signal_strength;
			}
			memset(*result_ptr, 0, sizeof(rtw_scan_result_t));
			return;
		}
	}

	//scan_result_handler_ptr.scan_cnt++;

	if(scan_result_handler_ptr.scan_cnt >= scan_result_handler_ptr.max_ap_size){
		scan_result_handler_ptr.scan_cnt = scan_result_handler_ptr.max_ap_size;
		if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size-1]->signal_strength){
			rtw_memcpy(scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size-1], *result_ptr, sizeof(rtw_scan_result_t));
			temp = scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size -1];
			scan_result_handler_ptr.scan_cnt  = scan_result_handler_ptr.max_ap_size -1;
		}else
			return;
	}else{
		rtw_memcpy(&scan_result_handler_ptr.ap_details[scan_result_handler_ptr.scan_cnt], *result_ptr, sizeof(rtw_scan_result_t));
	}

	for(i=0; i< scan_result_handler_ptr.scan_cnt; i++){
		if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[i]->signal_strength)
			break;
	}
	insert_pos = i;

	for(i = scan_result_handler_ptr.scan_cnt; i>insert_pos; i--)
		scan_result_handler_ptr.pap_details[i] = scan_result_handler_ptr.pap_details[i-1];

	if(temp != NULL)
		scan_result_handler_ptr.pap_details[insert_pos] = temp;
	else
		scan_result_handler_ptr.pap_details[insert_pos] = &scan_result_handler_ptr.ap_details[scan_result_handler_ptr.scan_cnt];

	if(scan_result_handler_ptr.scan_cnt < scan_result_handler_ptr.max_ap_size)
		scan_result_handler_ptr.scan_cnt++;

	rtw_memset(*result_ptr, 0, sizeof(rtw_scan_result_t));
}

static void _wifi_scan_done_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

	int i = 0;
	rtw_scan_handler_result_t scan_result_report;

	for(i=0; i<scan_result_handler_ptr.scan_cnt; i++){
		rtw_memcpy(&scan_result_report.ap_details, scan_result_handler_ptr.pap_details[i], sizeof(rtw_scan_result_t));
		scan_result_report.scan_complete = scan_result_handler_ptr.scan_complete;
		scan_result_report.user_data = scan_result_handler_ptr.user_data;
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
		if(scan_result_handler_ptr.gscan_result_handler == 0xFFFFFFFF)
			inic_ipc_scan_networks_callback_indicate(&scan_result_report);
		else
#endif
			(*scan_result_handler_ptr.gscan_result_handler)(&scan_result_report);
	}

	scan_result_handler_ptr.scan_complete = RTW_TRUE;
	scan_result_report.scan_complete = RTW_TRUE;
	scan_result_report.user_data = scan_result_handler_ptr.user_data;
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	if(scan_result_handler_ptr.gscan_result_handler == 0xFFFFFFFF)
		inic_ipc_scan_networks_callback_indicate(&scan_result_report);
	else
#endif
	(*scan_result_handler_ptr.gscan_result_handler)(&scan_result_report);

	rtw_free(scan_result_handler_ptr.ap_details);
	rtw_free(scan_result_handler_ptr.pap_details);
#if SCAN_USE_SEMAPHORE
	rtw_up_sema(&scan_result_handler_ptr.scan_semaphore);
#else
	scan_result_handler_ptr.scan_running = 0;
#endif
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl);
	return;
}

static void _wifi_scan_done_hdl_mcc( char* buf, int buf_len, int flags, void* userdata)
{
#if SCAN_USE_SEMAPHORE
	rtw_up_sema(&scan_result_handler_ptr.scan_semaphore);
#else
	scan_result_handler_ptr.scan_running = 0;
#endif
	return;
}

int wifi_scan(rtw_scan_type_t                    scan_type,
				  rtw_bss_type_t                     bss_type,
				  void*                result_ptr)
{
	int ret;
	scan_buf_arg * pscan_buf;
	u16 flags = scan_type | (bss_type << 8);
	if(result_ptr != NULL){
		pscan_buf = (scan_buf_arg *)result_ptr;
		ret = wext_set_scan(WLAN0_NAME, (char*)pscan_buf->buf, pscan_buf->buf_len, flags);
	}else{
		wifi_reg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl, NULL);
		wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl, NULL);
		ret = wext_set_scan(WLAN0_NAME, NULL, 0, flags);
	}

	if(ret == 0) {
		if(result_ptr != NULL){
			ret = wext_get_scan(WLAN0_NAME, pscan_buf->buf, pscan_buf->buf_len);
		}
	}
	else if(ret == -1){
		if(result_ptr == NULL){
			wifi_unreg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl);
			wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl);
		}
	}
	return ret;
}

int wifi_scan_networks_with_ssid(int (results_handler)(char*buf, int buflen, char *ssid, void *user_data),
	OUT void* user_data, IN int scan_buflen, IN char* ssid, IN int ssid_len)
{
	int scan_cnt = 0, add_cnt = 0;
	scan_buf_arg scan_buf;
	int ret;

	scan_buf.buf_len = scan_buflen;
	scan_buf.buf = (char*)rtw_malloc(scan_buf.buf_len);
	if(!scan_buf.buf){
		RTW_API_INFO("\n\rERROR: Can't malloc memory(%d)", scan_buf.buf_len);
		return RTW_NOMEM;
	}
	//set ssid
	memset(scan_buf.buf, 0, scan_buf.buf_len);
	memcpy(scan_buf.buf, &ssid_len, sizeof(int));
	memcpy(scan_buf.buf+sizeof(int), ssid, ssid_len);

	//Scan channel
	if((scan_cnt = wifi_scan(RTW_SCAN_TYPE_ACTIVE, RTW_BSS_TYPE_ANY, &scan_buf)) < 0){
		RTW_API_INFO("\n\rERROR: wifi scan failed");
		ret = RTW_ERROR;
	}else{
		if(NULL == results_handler)
		{
			int plen = 0;
			while(plen < scan_buf.buf_len){
				int len, rssi, ssid_len, i, security_mode;
				int wps_password_id;
				char *mac, *ssid;
				//u8 *security_mode;
				RTW_API_INFO("\n\r");
				// len
				len = (int)*(scan_buf.buf + plen);
				RTW_API_INFO("len = %d,\t", len);
				// check end
				if(len == 0) break;
				// mac
				mac = scan_buf.buf + plen + 1;
				RTW_API_INFO("mac = ");
				for(i=0; i<6; i++)
					RTW_API_INFO("%02x ", (u8)*(mac+i));
				RTW_API_INFO(",\t");
				// rssi
				rssi = *(int*)(scan_buf.buf + plen + 1 + 6);
				RTW_API_INFO(" rssi = %d,\t", rssi);
				// security_mode
				security_mode = (int)*(scan_buf.buf + plen + 1 + 6 + 4);
				switch (security_mode) {
					case IW_ENCODE_ALG_NONE:
						RTW_API_INFO("sec = open    ,\t");
						break;
					case IW_ENCODE_ALG_WEP:
						RTW_API_INFO("sec = wep     ,\t");
						break;
					case IW_ENCODE_ALG_CCMP:
						RTW_API_INFO("sec = wpa/wpa2,\t");
						break;
				}
				// password id
				wps_password_id = (int)*(scan_buf.buf + plen + 1 + 6 + 4 + 1);
				RTW_API_INFO("wps password id = %d,\t", wps_password_id);

				RTW_API_INFO("channel = %d,\t", *(scan_buf.buf + plen + 1 + 6 + 4 + 1 + 1));
				// ssid
				ssid_len = len - 1 - 6 - 4 - 1 - 1 - 1;
				ssid = scan_buf.buf + plen + 1 + 6 + 4 + 1 + 1 + 1;
				RTW_API_INFO("ssid = ");
				for(i=0; i<ssid_len; i++)
					RTW_API_INFO("%c", *(ssid+i));
				plen += len;
				add_cnt++;
			}

			RTW_API_INFO("\n\rwifi_scan: add count = %d, scan count = %d", add_cnt, scan_cnt);
		}
		ret = RTW_SUCCESS;
	}

#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	if(results_handler == 0xFFFFFFFF)
		inic_ipc_scan_ssid_callback_indicate(scan_buf.buf, scan_buf.buf_len, ssid, user_data);
	else
#else
		if(results_handler)
			results_handler(scan_buf.buf, scan_buf.buf_len, ssid, user_data);
#endif
	if(scan_buf.buf)
		rtw_free(scan_buf.buf);

	return ret;
}

int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data)
{
	unsigned int max_ap_size = 64;

	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2*1000);

#if SCAN_USE_SEMAPHORE
	rtw_bool_t result;
	if(NULL == scan_result_handler_ptr.scan_semaphore)
		rtw_init_sema(&scan_result_handler_ptr.scan_semaphore, 1);

	scan_result_handler_ptr.scan_start_time = rtw_get_current_time();
	/* Initialise the semaphore that will prevent simultaneous access - cannot be a mutex, since
	* we don't want to allow the same thread to start a new scan */
	result = (rtw_bool_t)rtw_down_timeout_sema(&scan_result_handler_ptr.scan_semaphore, SCAN_LONGEST_WAIT_TIME);
	if ( result != RTW_TRUE )
	{
		/* Return error result, but set the semaphore to work the next time */
		rtw_up_sema(&scan_result_handler_ptr.scan_semaphore);
		return RTW_TIMEOUT;
	}
#else
	if(scan_result_handler_ptr.scan_running){
		int count = 100;
		while(scan_result_handler_ptr.scan_running && count > 0)
		{
			rtw_msleep_os(20);
			count --;
		}
		if(count == 0){
			RTW_API_INFO("\n\r[%d]WiFi: Scan is running. Wait 2s timeout.", rtw_get_current_time());
			return RTW_TIMEOUT;
		}
	}
	scan_result_handler_ptr.scan_start_time = rtw_get_current_time();
	scan_result_handler_ptr.scan_running = 1;
#endif

	scan_result_handler_ptr.gscan_result_handler = results_handler;

	scan_result_handler_ptr.max_ap_size = max_ap_size;
	scan_result_handler_ptr.ap_details = (rtw_scan_result_t*)rtw_zmalloc(max_ap_size*sizeof(rtw_scan_result_t));
	if(scan_result_handler_ptr.ap_details == NULL){
		goto err_exit;
	}
	rtw_memset(scan_result_handler_ptr.ap_details, 0, max_ap_size*sizeof(rtw_scan_result_t));

	scan_result_handler_ptr.pap_details = (rtw_scan_result_t**)rtw_zmalloc(max_ap_size*sizeof(rtw_scan_result_t*));
	if(scan_result_handler_ptr.pap_details == NULL)
		goto error2_with_result_ptr;
	rtw_memset(scan_result_handler_ptr.pap_details, 0, max_ap_size*sizeof(rtw_scan_result_t*));

	scan_result_handler_ptr.scan_cnt = 0;

	scan_result_handler_ptr.scan_complete = RTW_FALSE;
	scan_result_handler_ptr.user_data = user_data;

	if (wifi_scan( RTW_SCAN_COMMAMD<<4 | RTW_SCAN_TYPE_ACTIVE, RTW_BSS_TYPE_ANY, NULL) != RTW_SUCCESS)
	{
		goto error1_with_result_ptr;
	}

	return RTW_SUCCESS;

error1_with_result_ptr:
	rtw_free((u8*)scan_result_handler_ptr.pap_details);
	scan_result_handler_ptr.pap_details = NULL;

error2_with_result_ptr:
	rtw_free((u8*)scan_result_handler_ptr.ap_details);
	scan_result_handler_ptr.ap_details = NULL;

err_exit:
	rtw_memset((void *)&scan_result_handler_ptr, 0, sizeof(scan_result_handler_ptr));
	return RTW_ERROR;
}

int wifi_scan_networks_mcc(rtw_scan_result_handler_t results_handler, void* user_data)
{
	unsigned int max_ap_size = 64;
	u8 channel_index;
	u8 pscan_config;
	int ret;

	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2*1000);

	for(channel_index=0;channel_index<SCAN_CHANNEL_NUM;channel_index++){
#if SCAN_USE_SEMAPHORE
		rtw_bool_t result;
		if(NULL == scan_result_handler_ptr.scan_semaphore)
			rtw_init_sema(&scan_result_handler_ptr.scan_semaphore, 1);

		scan_result_handler_ptr.scan_start_time = rtw_get_current_time();
		/* Initialise the semaphore that will prevent simultaneous access - cannot be a mutex, since
		* we don't want to allow the same thread to start a new scan */
		result = (rtw_bool_t)rtw_down_timeout_sema(&scan_result_handler_ptr.scan_semaphore, SCAN_LONGEST_WAIT_TIME);
		if ( result != RTW_TRUE )
		{
			/* Return error result, but set the semaphore to work the next time */
			rtw_up_sema(&scan_result_handler_ptr.scan_semaphore);
			return RTW_TIMEOUT;
		}
#else
		if(scan_result_handler_ptr.scan_running){
			int count = 200;
			while(scan_result_handler_ptr.scan_running && count > 0)
			{
				rtw_msleep_os(5);
				count --;
			}
			if(count == 0){
				printf("\n\r[%d]WiFi: Scan is running. Wait 1s timeout.", rtw_get_current_time());
				return RTW_TIMEOUT;
			}
		}

		vTaskDelay(SCAN_DONE_INTERVAL);

		scan_result_handler_ptr.scan_running = 1;
		scan_result_handler_ptr.scan_start_time = rtw_get_current_time();
#endif
		if(channel_index == 0){
			scan_result_handler_ptr.gscan_result_handler = results_handler;

			scan_result_handler_ptr.max_ap_size = max_ap_size;
			scan_result_handler_ptr.ap_details = (rtw_scan_result_t*)rtw_zmalloc(max_ap_size*sizeof(rtw_scan_result_t));
			if(scan_result_handler_ptr.ap_details == NULL){
				goto err_exit;
			}
			rtw_memset(scan_result_handler_ptr.ap_details, 0, max_ap_size*sizeof(rtw_scan_result_t));

			scan_result_handler_ptr.pap_details = (rtw_scan_result_t**)rtw_zmalloc(max_ap_size*sizeof(rtw_scan_result_t*));
			if(scan_result_handler_ptr.pap_details == NULL)
				goto error2_with_result_ptr;
			rtw_memset(scan_result_handler_ptr.pap_details, 0, max_ap_size*sizeof(rtw_scan_result_t*));

			scan_result_handler_ptr.scan_cnt = 0;

			scan_result_handler_ptr.scan_complete = RTW_FALSE;
			scan_result_handler_ptr.user_data = user_data;
			wifi_reg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl, NULL);
			wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl_mcc, NULL);
		}
		if(channel_index == SCAN_CHANNEL_NUM-1){
			wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl_mcc);
			wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl, NULL);
		}

		pscan_config = PSCAN_ENABLE;
		//set partial scan for entering to listen beacon quickly
		ret = wifi_set_pscan_chan(&scan_channel_list[channel_index], &pscan_config, 1);
		if(ret < 0){
#if SCAN_USE_SEMAPHORE
            rtw_up_sema(&scan_result_handler_ptr.scan_semaphore);
#else
            scan_result_handler_ptr.scan_running = 0;
#endif
            if(channel_index == SCAN_CHANNEL_NUM-1)
            {
                _wifi_scan_done_hdl(NULL, 0, 0, NULL);
            }
            continue;

		}

		if ( wext_set_scan(WLAN0_NAME, NULL, 0, (RTW_SCAN_COMMAMD<<4 | RTW_SCAN_TYPE_ACTIVE | (RTW_BSS_TYPE_ANY << 8))) != RTW_SUCCESS)
		{
			goto error1_with_result_ptr;
		}
	}

	return RTW_SUCCESS;

error1_with_result_ptr:
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl_mcc);
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl);
	rtw_free((u8*)scan_result_handler_ptr.pap_details);
	scan_result_handler_ptr.pap_details = NULL;

error2_with_result_ptr:
	rtw_free((u8*)scan_result_handler_ptr.ap_details);
	scan_result_handler_ptr.ap_details = NULL;

err_exit:
	rtw_memset((void *)&scan_result_handler_ptr, 0, sizeof(scan_result_handler_ptr));
	return RTW_ERROR;
}

void wifi_scan_unreg_event_handler(void)
{
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, _wifi_scan_each_report_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, _wifi_scan_done_hdl);
}

int wifi_set_pscan_chan(__u8 * channel_list,__u8 * pscan_config, __u8 length)
{
	if(channel_list)
	    return wext_set_pscan_channel(WLAN0_NAME, channel_list, pscan_config, length);
	else
	    return -1;
}

//----------------------------------------------------------------------------//

#endif	//#if CONFIG_WLAN
