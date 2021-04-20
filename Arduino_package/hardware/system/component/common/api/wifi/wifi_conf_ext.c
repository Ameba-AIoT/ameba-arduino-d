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

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
#if CONFIG_AUTO_RECONNECT
struct wifi_autoreconnect_param {
	rtw_security_t security_type;
	char *ssid;
	int ssid_len;
	char *password;
	int password_len;
	int key_id;
};
#endif
/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif
#if CONFIG_AUTO_RECONNECT
extern void (*p_wlan_autoreconnect_hdl)(rtw_security_t, char*, int, char*, int, int);
#endif

#if ATCMD_VER == ATVER_2
extern unsigned char dhcp_mode_sta;
#endif
extern unsigned char ap_bssid[6];

/******************************************************
 *               Variables Declarations
 ******************************************************/
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;

void* param_indicator;
struct task_struct wifi_autoreconnect_task;

/******************************************************
 *               Variables Definitions
 ******************************************************/

/*NETMASK*/
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0
#endif

/*Gateway Address*/
#ifndef GW_ADDR0
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1
#endif

#if CONFIG_AUTO_RECONNECT
#ifndef AUTO_RECONNECT_COUNT
#define AUTO_RECONNECT_COUNT	8
#endif
#ifndef AUTO_RECONNECT_INTERVAL
#define AUTO_RECONNECT_INTERVAL	5	// in sec
#endif
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

//----------------------------------------------------------------------------//

void wifi_rx_beacon_hdl( char* buf, int buf_len, int flags, void* userdata) {
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	
	//RTW_API_INFO("Beacon!\n");
}

void wifi_psk_info_set(struct psk_info * psk_data)
{
#ifdef CONFIG_INCLUDE_WPA_PSK
	rltk_psk_info_set(psk_data);
#endif
}

void wifi_psk_info_get(struct psk_info * psk_data)
{
#ifdef CONFIG_INCLUDE_WPA_PSK
	rltk_psk_info_get(psk_data);
#endif
}

void restore_wifi_info_to_flash(uint32_t offer_ip, uint32_t server_ip)
{
#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT || (defined(CONFIG_JD_SMART) && CONFIG_JD_SMART)
	u32 channel = 0;
	u8 index = 0;
	u8 *ifname[1] = {WLAN0_NAME};
	rtw_wifi_setting_t setting;
	struct wlan_fast_reconnect wifi_data_to_flash;
	struct psk_info PSK_info;
	//struct security_priv *psecuritypriv = &padapter->securitypriv;
	//WLAN_BSSID_EX  *pcur_bss = pmlmepriv->cur_network.network;

	if(p_write_reconnect_ptr){
		if(wifi_get_setting((const char*)ifname[0],&setting) || setting.mode == RTW_MODE_AP){
			RTW_API_INFO("\r\n %s():wifi_get_setting fail or ap mode", __func__);
			return;
		}
		channel = setting.channel;

		rtw_memset(&wifi_data_to_flash,0,sizeof(struct wlan_fast_reconnect));

		switch(setting.security_type){
			case RTW_SECURITY_OPEN:
				rtw_memcpy(wifi_data_to_flash.psk_essid, setting.ssid, strlen(setting.ssid));
				wifi_data_to_flash.security_type = RTW_SECURITY_OPEN;
				break;
			case RTW_SECURITY_WEP_PSK:
				rtw_memcpy(wifi_data_to_flash.psk_essid, setting.ssid, strlen(setting.ssid));
				rtw_memcpy(wifi_data_to_flash.psk_passphrase, setting.password, strlen(setting.password));
				channel |= (setting.key_idx) << 28;
				wifi_data_to_flash.security_type = RTW_SECURITY_WEP_PSK;
				break;
			case RTW_SECURITY_WPA_TKIP_PSK:
			case RTW_SECURITY_WPA2_AES_PSK:
#ifdef CONFIG_SAE_SUPPORT
			case RTW_SECURITY_WPA3_AES_PSK:
#endif
				rtw_memset(&PSK_info,0,sizeof(struct psk_info));
				PSK_info.index = 0;
				PSK_info.psk_essid = wifi_data_to_flash.psk_essid;
				PSK_info.psk_passphrase = wifi_data_to_flash.psk_passphrase;
				PSK_info.wpa_global_PSK = wifi_data_to_flash.wpa_global_PSK;
				wifi_psk_info_get(&PSK_info);
				wifi_data_to_flash.security_type = setting.security_type;
				break;

			default:
				break;
	}

		memcpy(&(wifi_data_to_flash.channel), &channel, 4);
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP             
		wifi_data_to_flash.offer_ip = offer_ip;
		wifi_data_to_flash.server_ip = server_ip;
#endif                
		//call callback function in user program
		p_write_reconnect_ptr((u8 *)&wifi_data_to_flash, sizeof(struct wlan_fast_reconnect));
	}
#endif
}

int wifi_set_mac_address(char * mac)
{
	char buf[13+17+1];
	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 13+17, "write_mac %s", mac);
	return wext_private_command(WLAN0_NAME, buf, 0);
}

int wifi_get_mac_address(char * mac)
{
	int ret = 0;
	char buf[32];
	rtw_memset(buf, 0, sizeof(buf));
	rtw_memcpy(buf, "read_mac", 8);
	ret = wext_private_command_with_retval(WLAN0_NAME, buf, buf, 32);
	strcpy(mac, buf);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_enable_powersave(void)
{
	return wext_enable_powersave(WLAN0_NAME, 1, 1);
}

/* wext_resume_powersave() will result in calling sscanf,
 sscanf have stack size usage different on IAR and GCC.
 use rltk_wlan_resume_powersave() to avoid calling of sscanf */
extern int rltk_wlan_resume_powersave(void);
extern int rltk_wlan_disable_powersave(void);
int wifi_resume_powersave(void)
{
	return rltk_wlan_resume_powersave();
}

int wifi_disable_powersave(void)
{
	return rltk_wlan_disable_powersave();
}

//----------------------------------------------------------------------------//
void wifi_btcoex_set_bt_on(void)
{
	rltk_wlan_btcoex_set_bt_state(1);
}

void wifi_btcoex_set_bt_off(void)
{
	rltk_wlan_btcoex_set_bt_state(0);
}

int wifi_btcoex_set_ble_scan_duty(u8 duty)
{
	return rltk_coex_set_ble_scan_duty(duty);
}

u8 wifi_driver_is_mp(void)
{
	return rltk_wlan_is_mp();
}
//----------------------------------------------------------------------------//
int wifi_get_ccmp_tk(unsigned char *uncst_key)
{
	if(uncst_key != NULL) {
		if(rltk_wlan_get_ccmp_tk(uncst_key))
			return RTW_SUCCESS;
		else {
			DBG_ERR("TK is not successfully got");
			return RTW_ERROR;
		}
	}
	else {
		DBG_ERR("Input parameter is NULL");
		return RTW_ERROR;
	}
}

//----------------------------------------------------------------------------//
int wifi_get_associated_client_list(void * client_list_buffer, uint16_t buffer_length)
{
	/* To avoid gcc warnings */
	( void ) buffer_length;
	
	const char * ifname = WLAN0_NAME;
	int ret = 0;
	char buf[25];
	
	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 25, "get_client_list %x", (unsigned int)client_list_buffer);
	ret = wext_private_command(ifname, buf, 0);

	return ret;
}

//----------------------------------------------------------------------------//
int wifi_get_ap_bssid(unsigned char *bssid)
{
	if( RTW_SUCCESS == wifi_is_ready_to_transceive(RTW_STA_INTERFACE)){
		rtw_memcpy(bssid, ap_bssid, ETH_ALEN);
		return RTW_SUCCESS;
	}
	return RTW_ERROR;
}

//----------------------------------------------------------------------------//
int wifi_get_ap_info(rtw_bss_info_t * ap_info, rtw_security_t* security)
{
	const char * ifname = WLAN0_NAME;
	int ret = 0;
	char buf[24];
	int tmp = 0;

	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 24, "get_ap_info %x", (unsigned int)ap_info);
	ret = wext_private_command(ifname, buf, 0);

	snprintf(buf, 24, "get_security");
	ret = wext_private_command_with_retval(ifname, buf, buf, 24);
	sscanf(buf, "%d", tmp);
	*security = tmp;

	return ret;
}

//----------------------------------------------------------------------------//
int wifi_get_mode(const char *ifname, int * mode)
{
	return wext_get_mode(ifname, mode);
}

int wifi_get_bssid(const char *ifname, unsigned char *bssid)
{
	return wext_get_bssid(ifname, bssid);
}

int wifi_get_ssid(const char *ifname, unsigned char *ssid)
{
	return wext_get_ssid(ifname, ssid);
}

int wifi_set_max_sta_num(unsigned char ap_sta_num)
{
	return wext_set_sta_num(ap_sta_num);
}

int wifi_get_drv_ability(uint32_t *ability)
{
	return wext_get_drv_ability(WLAN0_NAME, (__u32 *)ability);
}

/*this function is a new added API to get snr and rssi info
   wifi_get_rssi can get the ava rssi info.
   this function can get the ava rssi, rssi in latest packet, snr, snr in latest packet
*/
int wifi_get_signal_info(int32_t *signal)
{
	int ret = 0;
	char buf[24];
	char *ptmp;
	int total_info;

	rtw_memset(buf, 0, sizeof(buf));
	rtw_memcpy(buf, "get_signal_info", 15);
	ret = wext_private_command_with_retval(WLAN0_NAME, buf, buf, 24);
	total_info = strtoul(buf, &ptmp, 16);

	//get the snr info of latest received packet
	//*signal = total_info & 0xFF;

	//get the avarage snr info 
	*signal = (total_info >> 8) & 0xFF;

	//get the rssi info of latest received packet
	//*signal = (total_info >> 16) & 0xFF;

	//get the avarage rssi info  the same as (wifi_get_rssi)
	//*signal = (total_info >> 24) & 0xFF;
	
	return ret;
}


//----------------------------------------------------------------------------//
int wifi_get_sta_max_data_rate(OUT u8 * inidata_rate)
{
	return rltk_wlan_get_sta_max_data_rate(inidata_rate);
}

//----------------------------------------------------------------------------//
int wifi_get_rssi(int *pRSSI)
{
	return wext_get_rssi(WLAN0_NAME, pRSSI);
}

//----------------------------------------------------------------------------//
int wifi_get_snr(int *pSNR)
{
	return wext_get_snr(WLAN0_NAME, pSNR);
}


//----------------------------------------------------------------------------//
int wifi_register_multicast_address(rtw_mac_t *mac)
{
	return wext_register_multicast_address(WLAN0_NAME, mac);
}

int wifi_unregister_multicast_address(rtw_mac_t *mac)
{
	return wext_unregister_multicast_address(WLAN0_NAME, mac);
}


//----------------------------------------------------------------------------//
int wifi_rf_on(void)
{
	int ret;
	ret = rltk_wlan_rf_on();
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_rf_off(void)
{
	int ret;
	ret = rltk_wlan_rf_off();
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_off_fastly(void)
{
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	dhcps_deinit();
	LwIP_DHCP(0, DHCP_STOP);
#endif	
#endif	
	//RTW_API_INFO("\n\rDeinitializing WIFI ...");
	device_mutex_lock(RT_DEV_LOCK_WLAN);
	rltk_wlan_deinit_fastly();
	device_mutex_unlock(RT_DEV_LOCK_WLAN);
	return 0;
}

int wifi_set_power_mode(unsigned char ips_mode, unsigned char lps_mode)
{
	return wext_enable_powersave(WLAN0_NAME, ips_mode, lps_mode);
}

int wifi_set_tdma_param(unsigned char slot_period, unsigned char rfon_period_len_1, unsigned char rfon_period_len_2, unsigned char rfon_period_len_3)
{
	return wext_set_tdma_param(WLAN0_NAME, slot_period, rfon_period_len_1, rfon_period_len_2, rfon_period_len_3);
}

int wifi_set_lps_dtim(unsigned char dtim)
{
	return wext_set_lps_dtim(WLAN0_NAME, dtim);
}

int wifi_get_lps_dtim(unsigned char *dtim)
{
	return wext_get_lps_dtim(WLAN0_NAME, dtim);
}

// mode == 0: packet count, 1: enter directly, 2: tp threshold (default)
int wifi_set_lps_thresh(rtw_lps_thresh_t mode) 
{
	return wext_set_lps_thresh(WLAN0_NAME, mode);
}

int wifi_set_beacon_mode(int mode) {
	return wext_set_beacon_mode(WLAN0_NAME, mode);
}

int wifi_set_lps_level(unsigned char lps_level) {
	return wext_set_lps_level(WLAN0_NAME, lps_level);
}

//----------------------------------------------------------------------------//

int wifi_set_mfp_support(unsigned char value)
{
	return wext_set_mfp_support(WLAN0_NAME, value);
}

int wifi_set_group_id(unsigned char value)
{
#ifdef CONFIG_SAE_SUPPORT
	return wext_set_group_id(WLAN0_NAME, value);
#else
	return 0;
#endif
}


int wifi_set_pmk_cache_enable(unsigned char value)
{
#ifdef CONFIG_PMKSA_CACHING
	return wext_set_pmk_cache_enable(WLAN0_NAME, value);
#else
	return 0;
#endif
}

int wifi_get_enc_ext(const char *ifname, __u16 *alg, __u8 *key_idx, __u8 *passphrase)
{
	return wext_get_enc_ext(ifname, alg, key_idx, passphrase);
}
//----------------------------------------------------------------------------//
int wifi_get_setting(const char *ifname, rtw_wifi_setting_t *pSetting)
{
	int ret = 0;
	int mode = 0;
	unsigned short security = 0;
	
	memset(pSetting, 0, sizeof(rtw_wifi_setting_t));
	if(wext_get_mode(ifname, &mode) < 0)
		ret = -1;

	switch(mode) {
		case IW_MODE_MASTER:
			pSetting->mode = RTW_MODE_AP;
			break;
		case IW_MODE_INFRA:
		default:
			pSetting->mode = RTW_MODE_STA;
			break;
		//default:
			//RTW_API_INFO("\r\n%s(): Unknown mode %d\n", __func__, mode);
			//break;
	}

	if(wext_get_ssid(ifname, pSetting->ssid) < 0)
		ret = -1;
	if(wext_get_channel(ifname, &pSetting->channel) < 0)
		ret = -1;
	if(wext_get_enc_ext(ifname, &security, &pSetting->key_idx, pSetting->password) < 0)
		ret = -1;

	switch(security){
		case IW_ENCODE_ALG_NONE:
			pSetting->security_type = RTW_SECURITY_OPEN;
			break;
		case IW_ENCODE_ALG_WEP:
			pSetting->security_type = RTW_SECURITY_WEP_PSK;
			break;
		case IW_ENCODE_ALG_TKIP:
			pSetting->security_type = RTW_SECURITY_WPA_TKIP_PSK;
			break;
		case IW_ENCODE_ALG_CCMP:
			pSetting->security_type = RTW_SECURITY_WPA2_AES_PSK;
			
			break;
		default:
			break;
	}

	if(security == IW_ENCODE_ALG_TKIP || security == IW_ENCODE_ALG_CCMP)
		if(wext_get_passphrase(ifname, pSetting->password) < 0)
			ret = -1;

	return ret;
}
//----------------------------------------------------------------------------//
int wifi_show_setting(const char *ifname, rtw_wifi_setting_t *pSetting)
{
	int ret = 0;
#ifndef CONFIG_INIC_NO_FLASH

	RTW_API_INFO("\n\r\nWIFI  %s Setting:",ifname);
	RTW_API_INFO("\n\r==============================");

	switch(pSetting->mode) {
		case RTW_MODE_AP:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
			at_printf("\r\nAP,");
#endif
			RTW_API_INFO("\n\r      MODE => AP");
			break;
		case RTW_MODE_STA:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
			at_printf("\r\nSTA,");
#endif
			RTW_API_INFO("\n\r      MODE => STATION");
			break;
		default:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
			at_printf("\r\nUNKNOWN,");
#endif
			RTW_API_INFO("\n\r      MODE => UNKNOWN");
	}
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
	at_printf("%s,%d,", pSetting->ssid, pSetting->channel);
#endif
	RTW_API_INFO("\n\r      SSID => %s", pSetting->ssid);
	RTW_API_INFO("\n\r   CHANNEL => %d", pSetting->channel);

	switch(pSetting->security_type) {
		case RTW_SECURITY_OPEN:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
			at_printf("OPEN,");
#endif
			RTW_API_INFO("\n\r  SECURITY => OPEN");
			break;
		case RTW_SECURITY_WEP_PSK:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
			at_printf("WEP,%d,", pSetting->key_idx);
#endif
			RTW_API_INFO("\n\r  SECURITY => WEP");
			RTW_API_INFO("\n\r KEY INDEX => %d", pSetting->key_idx);
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
			at_printf("TKIP,");
#endif
			RTW_API_INFO("\n\r  SECURITY => TKIP");
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
			at_printf("AES,");
#endif
			RTW_API_INFO("\n\r  SECURITY => AES");
			break;
		default:
#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
			at_printf("UNKNOWN,");
#endif
			RTW_API_INFO("\n\r  SECURITY => UNKNOWN");
	}

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD) 
	at_printf("%s,", pSetting->password);
#endif
	RTW_API_INFO("\n\r  PASSWORD => %s", pSetting->password);
	RTW_API_INFO("\n\r");
#endif
	return ret;
}

//----------------------------------------------------------------------------//
void wifi_show_statistic(unsigned char idx)
{
	rltk_wlan_statistic(idx);
}

int wifi_set_network_mode(rtw_network_mode_t mode)
{
	if((mode == RTW_NETWORK_B) || (mode == RTW_NETWORK_BG) || (mode == RTW_NETWORK_BGN))
		return rltk_wlan_wireless_mode((unsigned char) mode);

	return -1;
}

int wifi_get_network_mode(rtw_network_mode_t *pmode)
{
	if(pmode != NULL)
		return rltk_wlan_get_wireless_mode((unsigned char *) pmode);

	return -1;
}

int wifi_set_wps_phase(unsigned char is_trigger_wps)
{
	return rltk_wlan_set_wps_phase(is_trigger_wps);
}

int wifi_set_gen_ie(const char * ifname, char * buf, __u16 buf_len, __u16 flags)
{
	return wext_set_gen_ie(ifname, buf, buf_len, flags);
}

int wifi_set_eap_phase(unsigned char is_trigger_eap)
{
#ifdef CONFIG_EAP
	return rltk_wlan_set_eap_phase(is_trigger_eap);
#else
	return -1;
#endif
}

unsigned char wifi_get_eap_phase(void)
{
#ifdef CONFIG_EAP
	return rltk_wlan_get_eap_phase();
#else
	return 0;
#endif
}

int wifi_set_eap_method(unsigned char eap_method)
{
#ifdef CONFIG_EAP
	return rltk_wlan_set_eap_method(eap_method);
#else
	return -1;
#endif
}

int wifi_send_eapol(const char *ifname, char *buf, __u16 buf_len, __u16 flags)
{
	return wext_send_eapol(ifname, buf, buf_len, flags);
}

int wifi_restart_ap(
	unsigned char 		*ssid,
	rtw_security_t		security_type,
	unsigned char 		*password,
	int 				ssid_len,
	int 				password_len,
	int					channel)
{
#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	unsigned char idx = 0;
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	struct netif * pnetif = &xnetif[0];
#endif
#endif
#ifdef  CONFIG_CONCURRENT_MODE
	rtw_wifi_setting_t setting;
	int sta_linked = 0;
#endif

	if(rltk_wlan_running(WLAN1_IDX)){
		idx = 1;
	}

	// stop dhcp server
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	dhcps_deinit();
#endif
#endif

#ifdef  CONFIG_CONCURRENT_MODE
	if(idx > 0){
		sta_linked = wifi_get_setting(WLAN0_NAME, &setting);
		wifi_off();
		rtw_msleep_os(20);
		wifi_on(RTW_MODE_STA_AP);
	}
	else
#endif
	{
#if CONFIG_LWIP_LAYER	
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
		//TODO
#else
#if LWIP_VERSION_MAJOR >= 2
		IP4_ADDR(ip_2_ip4(&ipaddr), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
		IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),ip_2_ip4(&gw));
#else
		IP4_ADDR(&ipaddr, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
		IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
#endif
#endif
#endif
		wifi_off();
		rtw_msleep_os(20);
		wifi_on(RTW_MODE_AP);			
	}
	// start ap
	if(wifi_start_ap((char*)ssid, security_type, (char*)password, ssid_len, password_len, channel) < 0) {
		RTW_API_INFO("\n\rERROR: Operation failed!");
		return -1;
	}

#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
	RTW_API_INFO("\r\nWebServer Thread: High Water Mark is %ld\n", uxTaskGetStackHighWaterMark(NULL));
#endif
#ifdef  CONFIG_CONCURRENT_MODE
	// connect to ap if wlan0 was linked with ap
	if(idx > 0 && sta_linked == 0){
		volatile int ret;
		(void) ret;
		RTW_API_INFO("\r\nAP: ssid=%s", (char*)setting.ssid);
		RTW_API_INFO("\r\nAP: security_type=%d", setting.security_type);
		RTW_API_INFO("\r\nAP: password=%s", (char*)setting.password);
		RTW_API_INFO("\r\nAP: key_idx =%d\n", setting.key_idx);
		ret = wifi_connect((char*)setting.ssid,
									setting.security_type,
									(char*)setting.password,
									strlen((char*)setting.ssid),
									strlen((char*)setting.password),
									setting.key_idx,
									NULL);	
#if defined(CONFIG_DHCP_CLIENT) && CONFIG_DHCP_CLIENT
		if(ret == RTW_SUCCESS) {
			/* Start DHCPClient */
			LwIP_DHCP(0, DHCP_START);
		}
#endif
	}
#endif	
#if defined(CONFIG_MBED_ENABLED)
	osThreadId_t id = osThreadGetId();
	RTW_API_INFO("\r\nWebServer Thread: High Water Mark is %ld\n", osThreadGetStackSpace(id));
#else
#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
	RTW_API_INFO("\r\nWebServer Thread: High Water Mark is %ld\n", uxTaskGetStackHighWaterMark(NULL));
#endif
#endif
#if CONFIG_LWIP_LAYER
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	//TODO
#else
	// start dhcp server
	dhcps_init(&xnetif[idx]);
#endif
#endif
#endif
	return 0;
}

static void _wifi_autoreconnect_thread(void *param)
{
#if CONFIG_AUTO_RECONNECT
#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	rtw_create_secure_context(configMINIMAL_SECURE_STACK_SIZE);
#endif
	int ret = RTW_ERROR;
	struct wifi_autoreconnect_param *reconnect_param = (struct wifi_autoreconnect_param *) param;
	RTW_API_INFO("\n\rauto reconnect ...\n");
	ret = wifi_connect(reconnect_param->ssid, reconnect_param->security_type, reconnect_param->password,
	                   reconnect_param->ssid_len, reconnect_param->password_len, reconnect_param->key_id, NULL);
#if CONFIG_LWIP_LAYER
	if(ret == RTW_SUCCESS) {
#if ATCMD_VER == ATVER_2
		if (dhcp_mode_sta == 2){
			struct netif * pnetif = &xnetif[0];
			LwIP_UseStaticIP(pnetif);
			dhcps_init(pnetif);
		}
		else
#endif
		{
			LwIP_DHCP(0, DHCP_START);
#if LWIP_AUTOIP
			uint8_t *ip = LwIP_GetIP(&xnetif[0]);
			if((ip[0] == 0) && (ip[1] == 0) && (ip[2] == 0) && (ip[3] == 0)) {
				RTW_API_INFO("\n\nIPv4 AUTOIP ...");
				LwIP_AUTOIP(&xnetif[0]);
			}
#endif
		}
	}
#endif //#if CONFIG_LWIP_LAYER
	param_indicator = NULL;
	rtw_delete_task(&wifi_autoreconnect_task);
#endif
}

void wifi_autoreconnect_hdl(rtw_security_t security_type,
                            char *ssid, int ssid_len,
                            char *password, int password_len,
                            int key_id)
{
#if CONFIG_AUTO_RECONNECT
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	RTW_API_INFO("\n\r%s Not Implemented Yet!\n", __func__);
#else
	static struct wifi_autoreconnect_param param;
	param_indicator = &param;
	param.security_type = security_type;
	param.ssid = ssid;
	param.ssid_len = ssid_len;
	param.password = password;
	param.password_len = password_len;
	param.key_id = key_id;
	rtw_create_task(&wifi_autoreconnect_task, (const char *)"wifi_autoreconnect", 512, tskIDLE_PRIORITY + 1, _wifi_autoreconnect_thread, &param);
#endif
#endif
}

int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout)
{
#if CONFIG_AUTO_RECONNECT
	if(mode == RTW_AUTORECONNECT_DISABLE)
		p_wlan_autoreconnect_hdl = NULL;
	else
		p_wlan_autoreconnect_hdl = wifi_autoreconnect_hdl;
	return wext_set_autoreconnect(WLAN0_NAME, mode, retry_times, timeout);
#else
	return -1;
#endif
}

int wifi_set_autoreconnect(__u8 mode)
{
#if CONFIG_AUTO_RECONNECT
	p_wlan_autoreconnect_hdl = wifi_autoreconnect_hdl;
	return wifi_config_autoreconnect(mode, AUTO_RECONNECT_COUNT, AUTO_RECONNECT_INTERVAL);//default retry 8 times, timeout 5 seconds
#else
	return -1;
#endif
}

int wifi_get_autoreconnect(__u8 *mode)
{
#if CONFIG_AUTO_RECONNECT
	return wext_get_autoreconnect(WLAN0_NAME, mode);
#else
	return 0;
#endif
}

//----------------------------------------------------------------------------//
/*
 * Example for custom ie
 *
 * u8 test_1[] = {221, 2, 2, 2};
 * u8 test_2[] = {221, 2, 1, 1};
 * rtw_custom_ie_t buf[2] = {{test_1, PROBE_REQ},
 *		 {test_2, PROBE_RSP | BEACON}};
 * u8 buf_test2[] = {221, 2, 1, 3} ;
 * rtw_custom_ie_t buf_update = {buf_test2, PROBE_REQ};
 *
 * add ie list 
 * static void cmd_add_ie(int argc, char **argv)
 * {
 *	 wifi_add_custom_ie((void *)buf, 2);
 * }
 *
 * update current ie
 * static void cmd_update_ie(int argc, char **argv)
 * {
 *	 wifi_update_custom_ie(&buf_update, 2);
 * }
 *
 * delete all ie
 * static void cmd_del_ie(int argc, char **argv)
 * {
 *	 wifi_del_custom_ie();
 * }
 */

int wifi_add_custom_ie(void *cus_ie, int ie_num)
{
#ifdef CONFIG_CUSTOM_IE
	return wext_add_custom_ie(WLAN0_NAME, cus_ie, ie_num);
#else
	return 0;
#endif
}


int wifi_update_custom_ie(void *cus_ie, int ie_index)
{
#ifdef CONFIG_CUSTOM_IE
	return wext_update_custom_ie(WLAN0_NAME, cus_ie, ie_index);
#else
	return 0;
#endif
}

int wifi_del_custom_ie()
{
#ifdef CONFIG_CUSTOM_IE
	return wext_del_custom_ie(WLAN0_NAME);
#else
	return 0;
#endif
}
//----------------------------------------------------------------------------//

int wifi_enable_forwarding(void)
{
#ifdef CONFIG_AP_MODE
	return wext_enable_forwarding(WLAN0_NAME);
#else
	return 0;
#endif
}

int wifi_disable_forwarding(void)
{
#ifdef CONFIG_AP_MODE
	return wext_disable_forwarding(WLAN0_NAME);
#else
	return 0;
#endif
}

/* API to set flag for concurrent mode wlan1 issue_deauth when channel switched by wlan0
 * usage: wifi_set_ch_deauth(0) -> wlan0 wifi_connect -> wifi_set_ch_deauth(1)
 */

int wifi_set_ch_deauth(__u8 enable)
{
#ifdef CONFIG_CONCURRENT_MODE
	return wext_set_ch_deauth(WLAN1_NAME, enable);
#else
	return 0;
#endif
}

void wifi_set_indicate_mgnt(int enable)
{
	wext_set_indicate_mgnt(enable);
	return;
}

int wifi_send_mgnt(const char *ifname, char *buf, u16 buf_len, u16 flags)
{
	return wext_send_mgnt(ifname, buf, buf_len, flags);
}

int wifi_init_mac_filter(void)
{
	return wext_init_mac_filter();
}

int wifi_add_mac_filter(unsigned char* hwaddr)
{
	return wext_add_mac_filter(hwaddr);
}

int wifi_del_mac_filter(unsigned char* hwaddr)
{
	return wext_del_mac_filter(hwaddr);
}

int wifi_get_antenna_info(unsigned char *antenna)
{
#ifdef CONFIG_ANTENNA_DIVERSITY
	int ret = 0;
	
	char buf[32];
	rtw_memset(buf, 0, sizeof(buf));
	rtw_memcpy(buf, "get_ant_info", 12);
	ret = wext_private_command_with_retval(WLAN0_NAME, buf, buf, 32);
	sscanf(buf, "%d", antenna); // 0: main, 1: aux
	return ret;
#else
	return -1;
#endif
}


int mailbox_to_wifi(u8 *data, u8 len)
{
#ifdef CONFIG_SW_MAILBOX_EN
	return wext_mailbox_to_wifi(WLAN0_NAME, data, len);
#else
	return -1;
#endif
}

/**
  * @brief	App can enable or disable wifi radio directly through this API.
  * @param	new_status: the new state of the WIFI RF.
  *				This parameter can be: ON/OFF.
  * @retval	0: success
  * @note	wifi can't Tx/Rx packet after radio off, so you use this API carefully!!
  */
int wifi_rf_contrl(uint32_t new_status)
{
#ifdef CONFIG_APP_CTRL_RF_ONOFF
	rtw_rf_cmd(new_status);
#endif

	return 0;  
}

/**
  * @brief	Get wifi TSF[63:0].
  * @param	port: wifi port 0/1.
  * @retval	TSF[63:0]
  */
u32 wifi_get_tsf_low(u32 port)
{
#ifdef CONFIG_APP_CTRL_RF_ONOFF
	return rtw_get_tsf(port);
#else
	return 0;
#endif
}

/*
 * @brief get WIFI band type
 *@retval  the support band type.
 * 	WL_BAND_2_4G: only support 2.4G
 *	WL_BAND_5G: only support 5G
 *      WL_BAND_2_4G_5G_BOTH: support both 2.4G and 5G
 */
WL_BAND_TYPE wifi_get_band_type(void)
{	
	u8 ret;

	ret = rltk_get_band_type();

	if(ret == 0) {
		return WL_BAND_2_4G;
	} else if(ret == 1){
		return WL_BAND_5G;
	} else {
		return WL_BAND_2_4G_5G_BOTH;
	}
}

int wifi_get_auto_chl(const char *ifname, unsigned char *channel_set, unsigned char channel_num)
{
	return wext_get_auto_chl(ifname, channel_set, channel_num);
}

int wifi_del_station(const char *ifname, unsigned char* hwaddr)
{
	return wext_del_station(ifname, hwaddr);
}

#if defined( CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE )&&( CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE == 1 )
extern void (*p_ap_polling_sta_hdl)(void *);
extern void (*p_ap_polling_sta_int_hdl)(void *, u16, u32 );
extern void ap_polling_sta_hdl(void *);
extern void ap_polling_sta_int_hdl(void *, u16, u32 );

void  wifi_set_ap_polling_sta(__u8 enabled)
{
	if(_TRUE == enabled)
	{
		p_ap_polling_sta_hdl = ap_polling_sta_hdl;
		p_ap_polling_sta_int_hdl = ap_polling_sta_int_hdl;
	}else
	{
		p_ap_polling_sta_hdl = NULL;
		p_ap_polling_sta_int_hdl = NULL;
	}
	return ;	
}
#endif

#endif	//#if CONFIG_WLAN
