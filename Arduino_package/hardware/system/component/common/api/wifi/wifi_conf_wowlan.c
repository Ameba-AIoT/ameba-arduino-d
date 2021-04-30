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
 *               Variables Definitions
 ******************************************************/
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
#define IP_HDR_LEN   20
#define TCP_HDR_LEN  20
#define ETH_HDR_LEN  14
#define ETH_ALEN     6
static uint32_t _checksum32(uint32_t start_value, uint8_t *data, size_t len)
{
	uint32_t checksum32 = start_value;
	uint16_t data16 = 0;
	int i;

	for(i = 0; i < (len / 2 * 2); i += 2) {
		data16 = (data[i] << 8) | data[i + 1];
		checksum32 += data16;
	}

	if(len % 2) {
		data16 = data[len - 1] << 8;
		checksum32 += data16;
	}

	return checksum32;
}

static uint16_t _checksum32to16(uint32_t checksum32)
{
	uint16_t checksum16 = 0;

	checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
	checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
	checksum16 = (uint16_t) ~(checksum32 & 0xffff);

	return checksum16;
}
#endif

int wifi_set_tcp_keep_alive_offload(int socket_fd, uint8_t *content, size_t len, uint32_t interval_ms)
{
	/* To avoid gcc warnings */
	( void ) socket_fd;
	( void ) content;
	( void ) len;
	( void ) interval_ms;
	
#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
	struct sockaddr_in peer_addr, sock_addr;
	socklen_t peer_addr_len = sizeof(peer_addr);
	socklen_t sock_addr_len = sizeof(sock_addr);
	getpeername(socket_fd, (struct sockaddr *) &peer_addr, &peer_addr_len);
	getsockname(socket_fd, (struct sockaddr *) &sock_addr, &sock_addr_len);
	uint8_t *peer_ip = (uint8_t *) &peer_addr.sin_addr;
	uint16_t peer_port = ntohs(peer_addr.sin_port);
	uint8_t *sock_ip = (uint8_t *) &sock_addr.sin_addr;
	uint16_t sock_port = ntohs(sock_addr.sin_port);

	// ip header
	uint8_t ip_header[IP_HDR_LEN] = {0x45, 0x00, /*len*/ 0x00, 0x00 /*len*/, /*id*/ 0x00, 0x00 /*id*/, 0x00, 0x00, 0xff, /*protocol*/ 0x00 /*protocol*/,
		/*chksum*/ 0x00, 0x00 /*chksum*/, /*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/};
	// len
	uint16_t ip_len = IP_HDR_LEN + TCP_HDR_LEN + len;
	ip_header[2] = (uint8_t) (ip_len >> 8);
	ip_header[3] = (uint8_t) (ip_len & 0xff);
	// id
	extern u16_t ip4_getipid(void);
	uint16_t ip_id = ip4_getipid();
	ip_header[4] = (uint8_t) (ip_id >> 8);
	ip_header[5] = (uint8_t) (ip_id & 0xff);
	// protocol
	ip_header[9] = 0x06;
	// src ip
	ip_header[12] = sock_ip[0];
	ip_header[13] = sock_ip[1];
	ip_header[14] = sock_ip[2];
	ip_header[15] = sock_ip[3];
	// dst ip
	ip_header[16] = peer_ip[0];
	ip_header[17] = peer_ip[1];
	ip_header[18] = peer_ip[2];
	ip_header[19] = peer_ip[3];
	// checksum
	uint32_t ip_checksum32 = 0;
	uint16_t ip_checksum16 = 0;
	ip_checksum32 = _checksum32(ip_checksum32, ip_header, sizeof(ip_header));
	ip_checksum16 = _checksum32to16(ip_checksum32);
	ip_header[10] = (uint8_t) (ip_checksum16 >> 8);
	ip_header[11] = (uint8_t) (ip_checksum16 & 0xff);

	// pseudo header
	uint8_t pseudo_header[12] = {/*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/,
		0x00, /*protocol*/ 0x00 /*protocol*/, /*l4len*/ 0x00, 0x00 /*l4len*/};
	// src ip
	pseudo_header[0] = sock_ip[0];
	pseudo_header[1] = sock_ip[1];
	pseudo_header[2] = sock_ip[2];
	pseudo_header[3] = sock_ip[3];
	// dst ip
	pseudo_header[4] = peer_ip[0];
	pseudo_header[5] = peer_ip[1];
	pseudo_header[6] = peer_ip[2];
	pseudo_header[7] = peer_ip[3];
	// protocol
	pseudo_header[9] = 0x06;
	// layer 4 len
	uint16_t l4_len = TCP_HDR_LEN + len;
	pseudo_header[10] = (uint8_t) (l4_len >> 8);
	pseudo_header[11] = (uint8_t) (l4_len & 0xff);

	// tcp header
	uint8_t tcp_header[TCP_HDR_LEN] = {/*srcport*/ 0x00, 0x00 /*srcport*/, /*dstport*/ 0x00, 0x00 /*dstport*/, /*seqno*/ 0x00, 0x00, 0x00, 0x00 /*seqno*/,
		/*ackno*/ 0x00, 0x00, 0x00, 0x00 /*ackno*/, 0x50, 0x18, /*window*/ 0x00, 0x00 /*window*/, /*checksum*/ 0x00, 0x00 /*checksum*/, 0x00, 0x00};
	// src port
	tcp_header[0] = (uint8_t) (sock_port >> 8);
	tcp_header[1] = (uint8_t) (sock_port & 0xff);
	// dst port
	tcp_header[2] = (uint8_t) (peer_port >> 8);
	tcp_header[3] = (uint8_t) (peer_port & 0xff);

	uint32_t seqno = 0;
	uint32_t ackno = 0;
	uint16_t wnd = 0;
	extern int lwip_gettcpstatus(int s, uint32_t *seqno, uint32_t *ackno, uint16_t *wnd);
	lwip_gettcpstatus(socket_fd, &seqno, &ackno, &wnd);
	// seqno
	tcp_header[4] = (uint8_t) (seqno >> 24);
	tcp_header[5] = (uint8_t) ((seqno & 0x00ff0000) >> 16);
	tcp_header[6] = (uint8_t) ((seqno & 0x0000ff00) >> 8);
	tcp_header[7] = (uint8_t) (seqno & 0x000000ff);
	// ackno
	tcp_header[8] = (uint8_t) (ackno >> 24);
	tcp_header[9] = (uint8_t) ((ackno & 0x00ff0000) >> 16);
	tcp_header[10] = (uint8_t) ((ackno & 0x0000ff00) >> 8);
	tcp_header[11] = (uint8_t) (ackno & 0x000000ff);
	// window
	tcp_header[14] = (uint8_t) (wnd >> 8);
	tcp_header[15] = (uint8_t) (wnd & 0xff);
	// checksum
	uint32_t tcp_checksum32 = 0;
	uint16_t tcp_checksum16 = 0;
	tcp_checksum32 = _checksum32(tcp_checksum32, pseudo_header, sizeof(pseudo_header));
	tcp_checksum32 = _checksum32(tcp_checksum32, tcp_header, sizeof(tcp_header));
	tcp_checksum32 = _checksum32(tcp_checksum32, content, len);
	tcp_checksum16 = _checksum32to16(tcp_checksum32);
	tcp_header[16] = (uint8_t) (tcp_checksum16 >> 8);
	tcp_header[17] = (uint8_t) (tcp_checksum16 & 0xff);

	// eth header
	uint8_t eth_header[ETH_HDR_LEN] = {/*dstaddr*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*dstaddr*/,
		/*srcaddr*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*srcaddr*/, 0x08, 0x00};
	struct eth_addr *dst_eth_ret = NULL;
	ip4_addr_t *dst_ip, *dst_ip_ret = NULL;
	dst_ip = (ip4_addr_t *) peer_ip;
	if(!ip4_addr_netcmp(dst_ip, netif_ip4_addr(&xnetif[0]), netif_ip4_netmask(&xnetif[0]))) {
		//outside local network
		dst_ip = (ip4_addr_t *) netif_ip4_gw(&xnetif[0]);
	}
	// dst addr
	if(etharp_find_addr(&xnetif[0], dst_ip, &dst_eth_ret, &dst_ip_ret) >= 0) {
		memcpy(eth_header, dst_eth_ret->addr, ETH_ALEN);
	}
	// src addr
	memcpy(eth_header + ETH_ALEN, LwIP_GetMAC(&xnetif[0]), ETH_ALEN);

	// eth frame without FCS
	uint32_t frame_len = sizeof(eth_header) + sizeof(ip_header) + sizeof(tcp_header) + len;
	uint8_t *eth_frame = (uint8_t *) malloc(frame_len);
	memcpy(eth_frame, eth_header, sizeof(eth_header));
	memcpy(eth_frame + sizeof(eth_header), ip_header, sizeof(ip_header));
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header), tcp_header, sizeof(tcp_header));
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header) + sizeof(tcp_header), content, len);

	//extern void rtw_set_keepalive_offload(uint8_t *eth_frame, uint32_t frame_len, uint32_t interval_ms);
	rtw_set_keepalive_offload(eth_frame, frame_len, interval_ms);

	free(eth_frame);

	return 0;
#else
	return -1;
#endif
}

#ifdef CONFIG_WOWLAN
int wifi_wlan_redl_fw(void)
{
	int ret = 0;
	
	ret = wext_wlan_redl_fw(WLAN0_NAME);
	
	return ret;  
}

int wifi_wowlan_ctrl(int enable)
{
	int ret = 0;
	
	ret = wext_wowlan_ctrl(WLAN0_NAME, enable);
	
	return ret;  
}

#ifdef CONFIG_WOWLAN_CUSTOM_PATTERN
int wifi_wowlan_set_pattern(wowlan_pattern_t pattern)
{
	int ret = 0;
	
	ret = wext_wowlan_set_pattern(WLAN0_NAME, pattern);
	
	return ret;  
}
#endif
#endif

#endif	//#if CONFIG_WLAN
