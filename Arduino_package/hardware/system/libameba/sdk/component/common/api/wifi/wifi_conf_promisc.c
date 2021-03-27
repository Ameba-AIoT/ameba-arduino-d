//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#endif
#include <platform/platform_stdlib.h>
#include <wifi/wifi_conf.h>
#include <wifi/wifi_util.h>
#include <wifi/wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>


/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Variables Declarations
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;

/******************************************************
 *               Variables Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

int wifi_set_promisc(rtw_rcr_level_t enabled, void (*callback)(unsigned char*, unsigned int, void*), unsigned char len_used)
{
	return promisc_set(enabled, callback, len_used);
}

void wifi_enter_promisc_mode(){
#ifdef CONFIG_PROMISC
	int mode = 0;
	unsigned char ssid[33];

	if(wifi_mode == RTW_MODE_STA_AP){
		wifi_off();
		rtw_msleep_os(20);
		wifi_on(RTW_MODE_PROMISC);
	}else{
		wifi_get_mode(WLAN0_NAME, &mode);

		switch(mode) {
			case IW_MODE_MASTER:    //In AP mode
#if defined(CONFIG_PLATFORM_8710C) && (defined(CONFIG_BT) && CONFIG_BT)
				wifi_set_mode(RTW_MODE_PROMISC);
#else
				wifi_off();//modified by Chris Yang for iNIC
				rtw_msleep_os(20);
				wifi_on(RTW_MODE_PROMISC);
#endif
				break;
			case IW_MODE_INFRA:		//In STA mode
				if(wifi_get_ssid(WLAN0_NAME, ssid) > 0)
					wifi_disconnect();
		}
	}
#endif
}

#ifdef CONFIG_PROMISC
void wifi_init_packet_filter()
{
	promisc_init_packet_filter();
}

int wifi_add_packet_filter(unsigned char filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule)
{
	return promisc_add_packet_filter(filter_id, patt, rule);
}

int wifi_enable_packet_filter(unsigned char filter_id)
{
	return promisc_enable_packet_filter(filter_id);
}

int wifi_disable_packet_filter(unsigned char filter_id)
{
	return promisc_disable_packet_filter(filter_id);
}

int wifi_remove_packet_filter(unsigned char filter_id)
{
	return promisc_remove_packet_filter(filter_id);
}

int wifi_retransmit_packet_filter(u8 enable, u8 filter_interval_ms)
{
	return promisc_filter_retransmit_pkt(enable, filter_interval_ms);
}

void wifi_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac)
{
	promisc_filter_by_ap_and_phone_mac(enable,ap_mac,phone_mac);
}

int wifi_promisc_ctrl_packet_rpt(u8 enable)
{
	return promisc_ctrl_packet_rpt(enable);
}
#endif

#endif	//#if CONFIG_WLAN
