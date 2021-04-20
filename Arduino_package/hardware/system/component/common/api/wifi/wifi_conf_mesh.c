//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <lwip/sockets.h>
#include "tcpip.h"
#endif
#endif
#include <platform/platform_stdlib.h>
#include <wifi/wifi_conf.h>
#include <wifi/wifi_util.h>
#include <wifi/wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>

#ifdef CONFIG_RTK_MESH
extern void hwmp_hdl( char* buf, int buf_len, int flags, void* userdata);
#endif

/******************************************************
 *                    Constants
 ******************************************************/

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
 *               Variables Declarations
 ******************************************************/
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;

/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN


#ifdef CONFIG_RTK_MESH
#include "wifi.h"

int wifi_list_mac_filter(void)
{
	return wext_list_mac_filter();
}

int wifi_get_mesh_id(unsigned char * mesh_id)
{
	return wext_get_mesh_id(WLAN0_NAME, mesh_id);
}

int wifi_set_mesh_rssi_threshold(const char *ifname, s32 rssi)
{
	return wext_set_mesh_rssi_threshold(ifname, rssi);
}

static void _wifi_mesh_point_assoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	printf("Mesh point peer "MAC_FMT" successfully\r\n", MAC_ARG(get_sa(buf)));
	return;
}

int wifi_start_mesh_point(
	char 				*mesh_id,
	rtw_security_t		security_type,
	char 				*password,
	int 				mesh_id_len,
	int 				password_len,
	int					channel)
{
	const char *ifname = WLAN0_NAME;
	const unsigned char mesh_group_id = 19;
	int ret = 0;

	if(wifi_mode == RTW_MODE_STA_AP) {
		ifname = WLAN1_NAME;
	}
	
	if(is_promisc_enabled())
		promisc_set(0, NULL, 0);

	//mesh path selection handlers
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_GEN_RREQ, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_GEN_RERR, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_RREQ, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_RREP, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_RERR, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_PANN, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_RANN, hwmp_hdl, NULL);

	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_GEN_PREQ, hwmp_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_PATHSEL_RECV_PREQ, hwmp_hdl, NULL);

	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, _wifi_mesh_point_assoc_hdl, NULL);

	ret = wext_enable_mesh(ifname);	//Enable mesh before starting ap
	if(ret < 0) goto exit;
	ret = wext_set_mode(ifname, IW_MODE_MASTER);
	if(ret < 0) goto exit;
	ret = wext_set_channel(ifname, channel);	//Set channel before starting ap
	if(ret < 0) goto exit;

	switch(security_type) {
		case RTW_SECURITY_OPEN:
			break;
#ifdef CONFIG_SAE_SUPPORT
		case RTW_SECURITY_WPA3_AES_PSK:
			ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_SAE);
			if(ret == 0)
				ret = wext_set_passphrase(ifname, (u8*)password, password_len);
			if(ret == 0)
				ret = wext_set_group_id(ifname, mesh_group_id);
			if(ret == 0)
				ret = sae_initialize(ifname, (u8*)password, password_len);
			break;
#endif
		default:
			ret = -1;
			printf("\n\rWIFICONF: security type is not supported");
			break;
	}
	if(ret < 0) goto exit;

	ret = wext_set_mesh_id(ifname, (u8*)mesh_id, mesh_id_len);
	if(ret < 0) goto exit;

	ret = wext_set_ap_ssid(ifname, (u8*)mesh_id, mesh_id_len); 
	if(ret < 0) goto exit;

exit:
	return ret;
}
#endif

#endif	//#if CONFIG_WLAN
