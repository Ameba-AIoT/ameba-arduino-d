/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved. 
  *
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "tcpip.h"
#include "lwip/init.h" //for lwip version control
/* Includes ------------------------------------------------------------------*/
#include <platform/platform_stdlib.h>
#include "platform_opts.h"
#include "autoconf.h"

#include "lwip/err.h"
// macros
/* Give default value if not defined */
#ifndef NET_IF_NUM
  #ifdef CONFIG_CONCURRENT_MODE
    #define NET_IF_NUM ((CONFIG_ETHERNET) + (CONFIG_WLAN) + 1)
  #else
    #define NET_IF_NUM ((CONFIG_ETHERNET) + (CONFIG_WLAN))
  #endif  // end of CONFIG_CONCURRENT_MODE
#endif  // end of NET_IF_NUM

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{ 
	DHCP_START=0,
	DHCP_WAIT_ADDRESS,
	DHCP_ADDRESS_ASSIGNED,
	DHCP_RELEASE_IP,
	DHCP_STOP,
	DHCP_TIMEOUT
} DHCP_State_TypeDef;

/* Extern functions ------------------------------------------------------------*/
void wifi_rx_beacon_hdl( char* buf, int buf_len, int flags, void* userdata);
void rtw_wakelock_timeout(u32 timeoutms);
/** Release a DHCP lease. */
err_t dhcp_release_unicast(struct netif *netif);


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_ReleaseIP(uint8_t idx);
uint8_t LwIP_DHCP(uint8_t idx, uint8_t dhcp_state);
unsigned char* LwIP_GetMAC(struct netif *pnetif);
uint8_t* LwIP_GetMAC_Idx(uint8_t idx);
unsigned char* LwIP_GetIP(struct netif *pnetif);
unsigned char* LwIP_GetIP_Idx(uint8_t idx);
unsigned char* LwIP_GetGW(struct netif *pnetif);
unsigned char* LwIP_GetGW_Idx(uint8_t idx);
uint8_t* LwIP_GetMASK(struct netif *pnetif);
uint8_t* LwIP_GetMASK_Idx(uint8_t idx);
void LwIP_wlan_set_netif_info(int idx_wlan, void * dev, unsigned char * dev_addr);
void LwIP_ethernetif_recv(uint8_t idx, int total_len);
int LwIP_netif_is_valid_IP(int idx, unsigned char *ip_dest);
uint8_t* LwIP_GetBC(struct netif *pnetif);
#if LWIP_DNS
void LwIP_GetDNS(struct ip_addr* dns);
void LwIP_SetDNS(struct ip_addr* dns);
#endif
void LwIP_UseStaticIP(struct netif *pnetif);
#ifdef CONFIG_RTK_MESH
void LwIP_SetIP(struct netif *pnetif, u32 *addr3);
#endif
#if LWIP_AUTOIP
void LwIP_AUTOIP(struct netif *pnetif);
#endif
#if LWIP_IPV6
void LwIP_AUTOIP_IPv6(struct netif *pnetif);
#endif
uint32_t LWIP_Get_Dynamic_Sleep_Interval(void);
int netif_get_idx(struct netif* pnetif);
#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
