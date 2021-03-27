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
//#include <wifi/wifi_util.h>
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
#define RTW_JOIN_TIMEOUT 20000 //INIC_IPC_API_TODO

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
 typedef int (*init_done_ptr)(void);
/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif

init_done_ptr p_wlan_init_done_callback = 0;

/******************************************************
 *               Variables Definitions
 ******************************************************/
internal_scan_handler_t scan_result_handler_ptr = {0, 0, 0, RTW_FALSE, 0, 0, 0, 0, 0};
int (*scan_ssid_result_hdl)(char*, int, char *, void *) = NULL;

unsigned char ap_bssid[6];
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
u8 is_the_same_ap = 0;
#endif

/******************************************************
 *               Variables Definitions
 ******************************************************/
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN
//----------------------------------------------------------------------------//

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

static void _wifi_no_network_hdl(char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	
	DBG_8195A("no network hdl\n");
	wifi_unreg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl);
}

static void _wifi_connected_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
		( void ) buf_len;
		( void ) flags;
		( void ) userdata;
	/* buf detail: ap bssid, buf_len = ETH_ALEN*/
	rtw_memcpy(ap_bssid, buf, ETH_ALEN);

	DBG_8195A("AP bssid=%02x:%02x:%02x:%02x:%02x:%02x\n", \
		ap_bssid[0],ap_bssid[1],ap_bssid[2],ap_bssid[3],ap_bssid[4],ap_bssid[5]);
	wifi_unreg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl);
}

static void _wifi_handshake_done_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;

	DBG_8195A("handshake done hdl\n");
	wifi_unreg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl);
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
	DBG_8195A("disconn hdl\n");
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	dhcp_stop(&xnetif[0]);
	netif_set_link_down(&xnetif[0]);
#endif
#endif
}

//----------------------------------------------------------------------------//
int wifi_connect(
	char 				*ssid,
	rtw_security_t	security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int 				key_id,
	void 				*semaphore)
{
	assert_param(semaphore==NULL);

	int ret, connect_ret;	
	u32 param_buf[7];

	param_buf[0] = ssid;
	param_buf[1] = security_type;
	param_buf[2] = password;
	param_buf[3] = ssid_len;
	param_buf[4] = password_len;
	param_buf[5] = key_id;
	param_buf[6] = semaphore;
	DCache_Clean((u32)ssid, ssid_len);
	DCache_Clean((u32)password, password_len);

	wifi_reg_event_handler(WIFI_EVENT_NO_NETWORK,_wifi_no_network_hdl,NULL);
	wifi_reg_event_handler(WIFI_EVENT_CONNECT, _wifi_connected_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, _wifi_disconn_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, _wifi_handshake_done_hdl, NULL);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CONNECT, param_buf, 7);

	if(ret == RTW_SUCCESS){
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
	}

	return ret;
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
	assert_param(semaphore == NULL);

	int ret;	
	u32 param_buf[9];

	param_buf[0] = bssid;
	param_buf[1] = ssid;
	param_buf[2] = security_type;
	param_buf[3] = password;
	param_buf[4] = bssid_len;
	param_buf[5] = ssid_len;
	param_buf[6] = password_len;
	param_buf[7] = key_id;
	param_buf[8] = semaphore;
	DCache_Clean((u32)bssid, bssid_len);
	DCache_Clean((u32)ssid, ssid_len);
	DCache_Clean((u32)password, password_len);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CONNECT_BSSID, param_buf, 9);

	if(ret == RTW_SUCCESS){
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
	}

	return ret;
}

int wifi_disconnect(void)
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_DISCONNECT, NULL, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_connected_to_ap( void )
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_CONNECTED_TO_AP, NULL, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_up(rtw_interface_t interface)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = interface;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_UP, param_buf, 1);
	return ret;
}

int wifi_is_running(unsigned char wlan_idx)
{
	int ret;
	u32 param_buf[1];
	param_buf[0] = wlan_idx;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_RUNNING, param_buf, 1);
	return ret;
}

int wifi_is_ready_to_transceive(rtw_interface_t interface)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = interface;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_READY_TO_TRANSCEIVE, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//

int wifi_set_country(rtw_country_code_t country_code)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = country_code;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_COUNTRY, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_change_channel_plan(uint8_t channel_plan)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = channel_plan;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CHANGE_CHANNEL_PLAN, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel_plan(uint8_t channel_plan)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = channel_plan;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_CHANNEL_PLAN, param_buf, 1);
	return ret;

}

int wifi_get_channel_plan(uint8_t *channel_plan)
{
	int ret = 0;
	u32 param_buf[1];

	DCache_CleanInvalidate((u32)channel_plan, sizeof(uint8_t));
	param_buf[0] = channel_plan;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_CHANNEL_PLAN, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel(int channel)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = channel;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_CHANNEL, param_buf, 1);
	return ret;

}

int wifi_get_channel(int *channel)
{
	int ret = 0;
	u32 param_buf[1];

	DCache_CleanInvalidate((u32)channel, sizeof(int));
	param_buf[0] = channel;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_CHANNEL, param_buf, 1);
	return ret;

}
#if 0//INIC_IPC_API_TODO
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
#endif//INIC_IPC_API_TODO
int wifi_get_last_error(void)
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_LAST_ERROR, NULL, 0);
	return ret;
}
int wifi_on(rtw_mode_t mode)
{
	int ret = 1;
	u32 param_buf[1];

	param_buf[0] = mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_ON, param_buf, 1);

	if(p_wlan_init_done_callback && (mode == RTW_MODE_STA))
		p_wlan_init_done_callback();

	if(ret == RTW_SUCCESS){//wifi on success
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
	}

	return ret;
}

int wifi_off(void)
{
	int ret = 0;

	if((wifi_is_running(WLAN0_IDX) == 0) &&
		(wifi_is_running(WLAN1_IDX) == 0)) {
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

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_OFF, NULL, 0);
	return ret;
}

int wifi_set_mode(rtw_mode_t mode)
{
	int ret = 0;
	u32 param_buf[1];
#ifdef CONFIG_WLAN_SWITCH_MODE	
	rtw_mode_t curr_mode, next_mode;
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	u8 autoreconnect_mode;
#endif
#endif
	device_mutex_lock(RT_DEV_LOCK_WLAN);//INIC_IPC_API_TODO

	if((wifi_is_running(WLAN0_IDX) == 0) &&
		(wifi_is_running(WLAN1_IDX) == 0)) {
		RTW_API_INFO("\n\r[%s] WIFI is not running",__FUNCTION__);
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return -1;
	}

#ifdef CONFIG_WLAN_SWITCH_MODE
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

	param_buf[0] = mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_MODE, param_buf, 1);

	if(ret == RTW_SUCCESS){
		if((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_AP)){
#if CONFIG_LWIP_LAYER
			netif_set_link_up(&xnetif[0]);
#endif	
			wifi_mode = mode;
		}else if((wifi_mode == RTW_MODE_AP) && (mode ==RTW_MODE_STA)){
#if CONFIG_LWIP_LAYER
			netif_set_link_down(&xnetif[0]);
#endif	
			wifi_mode = mode;
		}else if ((wifi_mode == RTW_MODE_AP) && (mode == RTW_MODE_PROMISC)){
#if CONFIG_LWIP_LAYER
			netif_set_link_down(&xnetif[0]);
#endif	
			wifi_mode = mode;
		}
#ifdef CONFIG_CONCURRENT_MODE
#ifdef CONFIG_WLAN_SWITCH_MODE
		else if((wifi_mode == RTW_MODE_STA_AP) && (mode == RTW_MODE_STA)){
#if CONFIG_LWIP_LAYER
			dhcps_deinit();
			netif_set_down(&xnetif[1]);
			netif_set_link_down(&xnetif[1]);
#endif
			wifi_mode = RTW_MODE_STA;
		}else if((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_STA_AP)){
#if CONFIG_LWIP_LAYER
			dhcps_init(&xnetif[1]);
			netif_set_up(&xnetif[1]);
			netif_set_link_up(&xnetif[1]);
#endif
			wifi_mode = mode;
		}
#endif
#endif

#ifdef CONFIG_WLAN_SWITCH_MODE
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
		/* enable auto reconnect */
		if(autoreconnect_mode != RTW_AUTORECONNECT_DISABLE){
			wifi_set_autoreconnect(autoreconnect_mode);
		}	
#endif
#endif
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return ret;
	}else{
#ifdef CONFIG_WLAN_SWITCH_MODE
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
		/* enable auto reconnect */
		if(autoreconnect_mode != RTW_AUTORECONNECT_DISABLE){
			wifi_set_autoreconnect(autoreconnect_mode);
		}
#endif
#endif
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return -1;
	}
}

static void _wifi_ap_sta_assoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO
	DBG_8195A("STA ASSOC\n");
}
static void _wifi_ap_sta_disassoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO
	DBG_8195A("STA DISASSOC\n");
}

int wifi_start_ap(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel)
{
	int ret = 0;
	u32 param_buf[6];
	param_buf[0] = ssid;
	param_buf[1] = security_type;
	param_buf[2] = password;
	param_buf[3] = ssid_len;
	param_buf[4] = password_len;
	param_buf[5] = channel;
	DCache_Clean(ssid, ssid_len);
	DCache_Clean(password, password_len);

	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, _wifi_ap_sta_assoc_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, _wifi_ap_sta_disassoc_hdl, NULL);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_START_AP, param_buf, 6);

	if(ret == RTW_SUCCESS){
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
	}
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
	int ret = 0;
	u32 param_buf[6];
	const char *ifname = WLAN0_NAME;

	param_buf[0] = ssid;
	param_buf[1] = security_type;
	param_buf[2] = password;
	param_buf[3] = ssid_len;
	param_buf[4] = password_len;
	param_buf[5] = channel;
	DCache_Clean((u32)ssid, ssid_len);
	DCache_Clean((u32)password, password_len);

	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, _wifi_ap_sta_assoc_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, _wifi_ap_sta_disassoc_hdl, NULL);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_START_AP_WITH_HIDDEN_SSID, param_buf, 6);

#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}
#endif
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
	wpas_wps_init(ifname);
#endif
	return ret;
}

int wifi_scan(rtw_scan_type_t                    scan_type,
				  rtw_bss_type_t                     bss_type,
				  void*                result_ptr)
{
	assert_param(result_ptr);

	int ret;
	scan_buf_arg * pscan_buf;
	u32 param_buf[3];

	pscan_buf = (scan_buf_arg *)result_ptr;
	DCache_Clean(pscan_buf, sizeof(scan_buf_arg));
	DCache_Clean(pscan_buf->buf, pscan_buf->buf_len);
	param_buf[0] = scan_type;
	param_buf[1] = bss_type;
	param_buf[2] = result_ptr;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN, param_buf, 3);
	DCache_Invalidate(pscan_buf->buf, pscan_buf->buf_len);
	return ret;
}

int wifi_scan_networks_with_ssid(int (results_handler)(char*buf, int buflen, char *ssid, void *user_data), 
	OUT void* user_data, IN int scan_buflen, IN char* ssid, IN int ssid_len)
{
	int ret;
	u32 param_buf[5];
	
	scan_ssid_result_hdl = results_handler;
	param_buf[0] = 0xFFFFFFFF;
	param_buf[1] = user_data;
	param_buf[2] = scan_buflen;
	param_buf[3] = ssid;
	param_buf[4] = ssid_len;
	DCache_Clean(ssid, ssid_len);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_NETWORKS_WITH_SSID, param_buf, 5);
	return ret;
}

int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data)
{
	assert_param(results_handler);
	int ret = 0;
	u32 param_buf[2];

	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2*1000);

	scan_result_handler_ptr.gscan_result_handler = results_handler;
	scan_result_handler_ptr.user_data = user_data;

	param_buf[0] = 0xFFFFFFFF;
	param_buf[1] = NULL;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_NETWROKS, param_buf, 2);
	return ret;
}

int wifi_scan_networks_mcc(rtw_scan_result_handler_t results_handler, void* user_data)
{
	assert_param(results_handler);

	int ret;
	u32 param_buf[2];

	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2*1000);

	scan_result_handler_ptr.gscan_result_handler = results_handler;
	scan_result_handler_ptr.user_data = user_data;

	param_buf[0] = 0xFFFFFFFF;
	param_buf[1] = NULL;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_NETWROKS_MCC, param_buf, 2);

	return ret;
}

void wifi_scan_unreg_event_handler(void)
{
	inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_UNREG_HDL, NULL, 0);
}

int wifi_set_pscan_chan(__u8 * channel_list,__u8 * pscan_config, __u8 length)
{
	int ret = -1;
	u32 param_buf[3];

	if(channel_list){
		param_buf[0] = channel_list;
		param_buf[1] = pscan_config;
		param_buf[2] = length;
		DCache_Clean((u32)channel_list, length);
		DCache_Clean((u32)pscan_config, length);

		ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_PSCAN_CHAN, param_buf, 3);
		return ret;
	}
	else
		return ret;
}

//----------------------------------------------------------------------------//

#endif	//#if CONFIG_WLAN
