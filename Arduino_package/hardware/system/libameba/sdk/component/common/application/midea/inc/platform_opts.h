/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_H__
#define __PLATFORM_OPTS_H__

/*For MP mode setting*/
#define SUPPORT_MP_MODE		1

/**
 * For AT cmd Log service configurations
 */
#define SUPPORT_LOG_SERVICE	1
#if SUPPORT_LOG_SERVICE
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_LOG_HISTORY	0
#if CONFIG_LOG_HISTORY
#define LOG_HISTORY_LEN    5
#endif
#define SUPPORT_INTERACTIVE_MODE		0 //on/off wifi_interactive_mode
#endif

/**
 * For interactive mode configurations, depents on log service
 */
#if SUPPORT_INTERACTIVE_MODE
#define CONFIG_INTERACTIVE_MODE     1
#define CONFIG_INTERACTIVE_EXT   0
#else
#define CONFIG_INTERACTIVE_MODE     0
#define CONFIG_INTERACTIVE_EXT   0
#endif
/******************************************************************************/


/**
 * For Wlan configurations
 */
#define CONFIG_WLAN	1
#if CONFIG_WLAN
#define CONFIG_LWIP_LAYER	1
#define CONFIG_INIT_NET         1 //init lwip layer when start up

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT       0
#define CONFIG_WEBSERVER        0
#define CONFIG_OTA_UPDATE       1
#define CONFIG_BSD_TCP          0
#define CONFIG_ENABLE_P2P       0//on/off p2p cmd in log_service or interactive mode

/* For WPS and P2P */
//#define CONFIG_WPS
#if defined(CONFIG_WPS) 
#define CONFIG_ENABLE_WPS       0
#endif

/* For Simple Link */
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0

/*For fast reconnection*/
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0

#endif //end of #if CONFIG_WLAN
/*******************************************************************************/

/**
 * For iNIC configurations
 */
//#define CONFIG_INIC_EN 0//enable iNIC mode
#if CONFIG_INIC_EN
#undef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER	0
#define CONFIG_INIC_SDIO_HCI	0 //for SDIO or USB iNIC
#define CONFIG_INIC_USB_HCI	1
#endif


/*For google nest example, make sure the TCP is open*/
#define CONFIG_EXAMPLE_GOOGLE_NEST       0

/* For mDNS example */
#define CONFIG_EXAMPLE_MDNS              0

/* For multicast example */
#define CONFIG_EXAMPLE_MCAST             0

/* For uart adapter example */
/* Please also configure LWIP_IGMP to 1 and LWIP_TCP_DELAY_DISABLE to 1 
in lwip_opt.h for support uart adapter*/
#define CONFIG_EXAMPLE_UART_ADAPTER             0
#if CONFIG_EXAMPLE_UART_ADAPTER
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	1
#undef CONFIG_EXAMPLE_MDNS
#define CONFIG_EXAMPLE_MDNS	1
#define LWIP_IGMP                   1
#endif

#endif
