/**
 * @file
 * lwIP netif implementing an IEEE 802.1D MAC Bridge
 */

/*
 * Copyright (c) 2017 Simon Goldschmidt.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Simon Goldschmidt <goldsimon@gmx.de>
 *
 */

/**
 * @defgroup bridgeif IEEE 802.1D bridge
 * @ingroup netifs
 * This file implements an IEEE 802.1D bridge by using a multilayer netif approach
 * (one hardware-independent netif for the bridge that uses hardware netifs for its ports).
 * On transmit, the bridge selects the outgoing port(s).
 * On receive, the port netif calls into the bridge (via its netif->input function) and
 * the bridge selects the port(s) (and/or its netif->input function) to pass the received pbuf to.
 *
 * Usage:
 * - add the port netifs just like you would when using them as dedicated netif without a bridge
 *   - only NETIF_FLAG_ETHARP/NETIF_FLAG_ETHERNET netifs are supported as bridge ports
 *   - add the bridge port netifs without IPv4 addresses (i.e. pass 'NULL, NULL, NULL')
 *   - don't add IPv6 addresses to the port netifs!
 * - set up the bridge configuration in a global variable of type 'bridgeif_initdata_t' that contains
 *   - the MAC address of the bridge
 *   - some configuration options controlling the memory consumption (maximum number of ports
 *     and FDB entries)
 *   - e.g. for a bridge MAC address 00-01-02-03-04-05, 2 bridge ports, 1024 FDB entries + 16 static MAC entries:
 *     bridgeif_initdata_t mybridge_initdata = BRIDGEIF_INITDATA1(2, 1024, 16, ETH_ADDR(0, 1, 2, 3, 4, 5));
 * - add the bridge netif (with IPv4 config):
 *   struct netif bridge_netif;
 *   netif_add(&bridge_netif, &my_ip, &my_netmask, &my_gw, &mybridge_initdata, bridgeif_init, tcpip_input);
 *   NOTE: the passed 'input' function depends on BRIDGEIF_PORT_NETIFS_OUTPUT_DIRECT setting,
 *         which controls where the forwarding is done (netif low level input context vs. tcpip_thread)
 * - set up all ports netifs and the bridge netif
 *
 * - When adding a port netif, NETIF_FLAG_ETHARP flag will be removed from a port
 *   to prevent ETHARP working on that port netif (we only want one IP per bridge not per port).
 * - When adding a port netif, its input function is changed to call into the bridge.
 *
 *
 * @todo:
 * - compact static FDB entries (instead of walking the whole array)
 * - add FDB query/read access
 * - add FDB change callback (when learning or dropping auto-learned entries)
 * - prefill FDB with MAC classes that should never be forwarded
 * - multicast snooping? (and only forward group addresses to interested ports)
 * - support removing ports
 * - check SNMP integration
 * - VLAN handling / trunk ports
 * - priority handling? (although that largely depends on TX queue limitations and lwIP doesn't provide tx-done handling)
 */

#include "netif/bridgeif.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
#include <string.h>
#include "lwip/prot/tcp.h"
#include "lwip/prot/dns.h"
#include <lwip/icmp.h>
#include "lwip/udp.h"
#include <lwip/inet_chksum.h>



#include "autoconf.h"

#if LWIP_NUM_NETIF_CLIENT_DATA && CONFIG_BRIDGE

#ifdef CONFIG_CONCURRENT_MODE 
extern int rpt_handle_init(unsigned char *br_mac);
extern err_t br_rpt_handle(struct netif *br, struct pbuf *p, u8_t portif_idex);
extern int br_rpt_handle_frame(struct netif *br, struct pbuf *pd);
#endif


/* Define those to better describe your network interface. */
#define IFNAME0 'b'
#define IFNAME1 'r'

struct bridgeif_private_s;
typedef struct bridgeif_port_private_s {
  struct bridgeif_private_s *bridge;
  struct netif *port_netif;
  u8_t port_num;
} bridgeif_port_t;

typedef struct bridgeif_fdb_static_entry_s {
  u8_t used;
  bridgeif_portmask_t dst_ports;
  struct eth_addr addr;
} bridgeif_fdb_static_entry_t;

typedef struct bridgeif_private_s {
  struct netif     *netif;
  struct eth_addr   ethaddr;
  u8_t              max_ports;
  u8_t              num_ports;
  bridgeif_port_t  *ports;
  u16_t             max_fdbs_entries;
  bridgeif_fdb_static_entry_t *fdbs;
  u16_t             max_fdbd_entries;
  void             *fdbd;
} bridgeif_private_t;

/* netif data index to get the bridge on input */
u8_t bridgeif_netif_client_id = 0xff;

//#undef BRIDGEIF_FW_DEBUG
//#define BRIDGEIF_FW_DEBUG LWIP_DBG_ON


/**
 * @ingroup bridgeif
 * Add a static entry to the forwarding database.
 * A static entry marks where frames to a specific eth address (unicast or group address) are
 * forwarded.
 * bits [0..(BRIDGEIF_MAX_PORTS-1)]: hw ports
 * bit [BRIDGEIF_MAX_PORTS]: cpu port
 * 0: drop
 */
err_t
bridgeif_fdb_add(struct netif *bridgeif, const struct eth_addr *addr, bridgeif_portmask_t ports)
{
  int i;
  bridgeif_private_t *br;
  BRIDGEIF_DECL_PROTECT(lev);
  LWIP_ASSERT("invalid netif", bridgeif != NULL);
  br = (bridgeif_private_t *)bridgeif->state;
  LWIP_ASSERT("invalid state", br != NULL);


  BRIDGEIF_READ_PROTECT(lev);
  for (i = 0; i < br->max_fdbs_entries; i++) {
    if (!br->fdbs[i].used) {
      BRIDGEIF_WRITE_PROTECT(lev);
      if (!br->fdbs[i].used) {
        br->fdbs[i].used = 1;
        br->fdbs[i].dst_ports = ports;
        memcpy(&br->fdbs[i].addr, addr, sizeof(struct eth_addr));
		printf("\n\r%s:BR: create src %02X:%02X:%02X:%02X:%02X:%02X (from %d) @ idx %d\n",__func__,
												 addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3], addr->addr[4], addr->addr[5],
												 ports, i);

        BRIDGEIF_WRITE_UNPROTECT(lev);
        BRIDGEIF_READ_UNPROTECT(lev);
        return ERR_OK;
      }
      BRIDGEIF_WRITE_UNPROTECT(lev);
    }
  }
  BRIDGEIF_READ_UNPROTECT(lev);
  return ERR_MEM;
}

/**
 * @ingroup bridgeif
 * Remove a static entry from the forwarding database
 */
err_t
bridgeif_fdb_remove(struct netif *bridgeif, const struct eth_addr *addr)
{
  int i;
  bridgeif_private_t *br;
  BRIDGEIF_DECL_PROTECT(lev);
  LWIP_ASSERT("invalid netif", bridgeif != NULL);
  br = (bridgeif_private_t *)bridgeif->state;
  LWIP_ASSERT("invalid state", br != NULL);

  BRIDGEIF_READ_PROTECT(lev);
  for (i = 0; i < br->max_fdbs_entries; i++) {
    if (br->fdbs[i].used && !memcmp(&br->fdbs[i].addr, addr, sizeof(struct eth_addr))) {
      BRIDGEIF_WRITE_PROTECT(lev);
      if (br->fdbs[i].used && !memcmp(&br->fdbs[i].addr, addr, sizeof(struct eth_addr))) {
        memset(&br->fdbs[i], 0, sizeof(bridgeif_fdb_static_entry_t));
        BRIDGEIF_WRITE_UNPROTECT(lev);
        BRIDGEIF_READ_UNPROTECT(lev);
        return ERR_OK;
      }
      BRIDGEIF_WRITE_UNPROTECT(lev);
    }
  }
  BRIDGEIF_READ_UNPROTECT(lev);
  return ERR_VAL;
}

/** Get the forwarding port(s) (as bit mask) for the specified destination mac address */
static bridgeif_portmask_t
bridgeif_find_dst_ports(bridgeif_private_t *br, struct eth_addr *dst_addr)
{
  int i;
  BRIDGEIF_DECL_PROTECT(lev);
  BRIDGEIF_READ_PROTECT(lev);
  /* first check for static entries */
  for (i = 0; i < br->max_fdbs_entries; i++) {
    if (br->fdbs[i].used) {
      if (!memcmp(&br->fdbs[i].addr, dst_addr, sizeof(struct eth_addr))) {
        bridgeif_portmask_t ret = br->fdbs[i].dst_ports;
        BRIDGEIF_READ_UNPROTECT(lev);
        return ret;
      }
    }
  }
  if (dst_addr->addr[0] & 1) {
    /* no match found: flood remaining group address */
    BRIDGEIF_READ_UNPROTECT(lev);
    return BR_FLOOD;
  }
  BRIDGEIF_READ_UNPROTECT(lev);
  /* no match found: check dynamic fdb for port or fall back to flooding */
  return bridgeif_fdb_get_dst_ports(br->fdbd, dst_addr);
}

/** Helper function to see if a destination mac belongs to the bridge
 * (bridge netif or one of the port netifs), in which case the frame
 * is sent to the cpu only.
 */
static int
bridgeif_is_local_mac(bridgeif_private_t *br, struct eth_addr *addr)
{
  int i;
  BRIDGEIF_DECL_PROTECT(lev);
  if (!memcmp(br->netif->hwaddr, addr, sizeof(struct eth_addr))) {
    return 1;
  }
  BRIDGEIF_READ_PROTECT(lev);
  for (i = 0; i < br->num_ports; i++) {
    struct netif *portif = br->ports[i].port_netif;
    if (portif != NULL) {
      if (!memcmp(portif->hwaddr, addr, sizeof(struct eth_addr))) {
        BRIDGEIF_READ_UNPROTECT(lev);
        return 1;
      }
    }
  }
  BRIDGEIF_READ_UNPROTECT(lev);
  return 0;
}

/* Output helper function */
static err_t
bridgeif_send_to_port(bridgeif_private_t *br, struct pbuf *p, u8_t dstport_idx)
{
  if (dstport_idx < BRIDGEIF_MAX_PORTS) {
    /* possibly an external port */
    if (dstport_idx < br->max_ports) {
      struct netif *portif = br->ports[dstport_idx].port_netif;
	  
      if ((portif != NULL) && (portif->linkoutput != NULL)) {
	  	//printf("%s:dstports:%d,name:%c,ifidx:%d,p->if_idx:%d\n",__func__,dstport_idx,portif->name[1],portif->num + 1,p->if_idx);
        /* prevent sending out to rx port */
        if (netif_get_index(portif) != p->if_idx) {
          if (netif_is_link_up(portif)) {
            LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> flood(%p:%d) -> %d\n", (void *)p, p->if_idx, netif_get_index(portif)));
			//printf("br -> flood(%p:%d) -> %d\n", (void *)p, p->if_idx, netif_get_index(portif));
            #ifdef CONFIG_CONCURRENT_MODE 
            br_rpt_handle(br->netif, p, netif_get_index(portif));
            #endif
            return portif->linkoutput(portif, p);
          }
        }
      }
    }
  } else {
    LWIP_ASSERT("invalid port index", dstport_idx == BRIDGEIF_MAX_PORTS);
  }
  return ERR_OK;
}

/** Helper function to pass a pbuf to all ports marked in 'dstports'
 */
static err_t
bridgeif_send_to_ports(bridgeif_private_t *br, struct pbuf *p, bridgeif_portmask_t dstports)
{
  err_t err, ret_err = ERR_OK;
  u8_t i;
  bridgeif_portmask_t mask = 1;
  BRIDGEIF_DECL_PROTECT(lev);
  BRIDGEIF_READ_PROTECT(lev);
  for (i = 0; i < BRIDGEIF_MAX_PORTS; i++, mask = (bridgeif_portmask_t)(mask << 1)) {
    if (dstports & mask) {
      err = bridgeif_send_to_port(br, p, i);
      if (err != ERR_OK) {
        ret_err = err;
      }
    }
  }
  BRIDGEIF_READ_UNPROTECT(lev);
  return ret_err;
}

/** Output function of the application port of the bridge (the one with an ip address).
 * The forwarding port(s) where this pbuf is sent on is/are automatically selected
 * from the FDB.
 */
static err_t
bridgeif_output(struct netif *netif, struct pbuf *p)
{
  err_t err;
  bridgeif_private_t *br = (bridgeif_private_t *)netif->state;
  struct eth_addr *dst = (struct eth_addr *)(p->payload);

  bridgeif_portmask_t dstports = bridgeif_find_dst_ports(br, dst);
  err = bridgeif_send_to_ports(br, p, dstports);

  MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
  if (((u8_t *)p->payload)[0] & 1) {
    /* broadcast or multicast packet*/
    MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
  } else {
    /* unicast packet */
    MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
  }
  /* increase ifoutdiscards or ifouterrors on error */

  LINK_STATS_INC(link.xmit);

  return err;
}

typedef struct src_port_s {
	u16_t port;
	u8_t flag;
	u8_t type;
	u32_t ts;
}src_port_t;
typedef struct bridgeif_nat_entry_s {
	u8_t used;
	src_port_t src_port[8];
	u16_t dst_port;
	u8_t port_num;
	u8_t ip_addr[4];
	struct eth_addr mac;
} bridgeif_nat_entry_t;

typedef struct bridgeif_pkt_attrib_s {
	u16_t protocol;
	u16_t src_port;
	u16_t dst_port;
	u8_t src_ip[4];
	u8_t dst_ip[4];
	u8_t flags;
	u8_t type;
	struct eth_addr src_mac;
	struct eth_addr dst_mac;
	u8_t port_idx;
}bridgeif_pkt_attrib_t;

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_ILEN	4		/* Octets in one ip addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/

/* input port num == 0 (wlan0) case , port means tcp/udp port */
#define PORT_TO_NONE	0x0   /* dst port == 0, send to all ports(br and sdio host) */
#define PORT_TO_BR	0x1		  /* dst port == br's port, send to br netif */
#define PORT_TO_HOST	0x2	  /* dst port == sdio host's port, send to sdio host */

/* input port num == 1 (sdio host) case */
#define IP_TO_NONE	0x0       /* dst ip == NULL, send to all ports(br and wlan0)*/
#define IP_TO_BR	0x1		  /* dst ip == br's ip, send to br netif */
#define IP_TO_WLAN1	0x2		  /* dst ip == wlan1's ip, send to wlan1 netif */
#define IP_TO_HOST	0x3       /* dst ip == sdio host's ip, send to sdio host */

#define BR_NAT_TIMEOUT_SEC  (60*5) /* 5 minutes FDB timeout */



bridgeif_nat_entry_t br_nat_entry[4] = {0};

void bridgeif_update_attrib(struct pbuf *p, bridgeif_pkt_attrib_t *pattrib)
{
	u16_t protocol, src_port = 0, dst_port = 0;
	u8_t type = 0;
	u8_t flags = 0;
	struct ip_hdr  *iph = NULL;
  	struct etharp_hdr *arph = NULL;
	struct tcp_hdr *tcph = NULL;
	struct udp_hdr *udph = NULL;
	u8_t *src_ip = NULL, *dst_ip = NULL;
	struct eth_addr *src_addr, *dst_addr;
	

	dst_addr = (struct eth_addr *)((u8_t *)p->payload);
	src_addr = (struct eth_addr *)(((u8_t *)p->payload) + sizeof(struct eth_addr));
	protocol = *((unsigned short *)(p->payload + 2 * ETH_ALEN));

  	if(protocol == lwip_htons(ETH_P_IP)) {
		/* update src ip/mac mapping */
		iph = (struct ip_hdr *)(p->payload + ETH_HLEN);
		src_ip = (u8_t *)&(iph->src.addr);
		dst_ip = (u8_t *)&(iph->dest.addr);
		type = iph->_proto;
		switch(iph->_proto) {
		case IP_PROTO_TCP://TCP
			tcph = (struct tcp_hdr *)(p->payload + ETH_HLEN + sizeof(struct ip_hdr));
			if(tcph != NULL) {
				src_port = PP_NTOHS(tcph->src);
				dst_port = PP_NTOHS(tcph->dest);
				flags = TCPH_FLAGS(tcph);
			}			
			break;
		case IP_PROTO_UDP://UDP
			udph = (struct udp_hdr *)(p->payload + ETH_HLEN + sizeof(struct ip_hdr));
			if(udph != NULL) {
				src_port = PP_NTOHS(udph->src);
				dst_port = PP_NTOHS(udph->dest);
			}
			break;
		default:
			src_port = 0;
			dst_port = 0;
		}

  	} else if(protocol == lwip_htons(ETH_P_ARP)) {	 
		arph = (struct etharp_hdr *)(p->payload + ETH_HLEN);
		src_ip = (u8 *)&(arph->sipaddr);
		dst_ip = (u8 *)&(arph->dipaddr);
	}

	//printf("%s,src_port:%d, dst_port:%d\n",__func__,src_port, dst_port);

	pattrib->protocol = protocol;
	pattrib->src_port = src_port;
	pattrib->dst_port = dst_port;
	pattrib->type = type;
	pattrib->flags = flags;
	memcpy(&pattrib->src_mac, src_addr, sizeof(struct eth_addr));
	memcpy(&pattrib->dst_mac, dst_addr, sizeof(struct eth_addr));
	if(src_ip != NULL)
		memcpy(pattrib->src_ip, src_ip, ETH_ILEN);
	if(dst_ip != NULL)
		memcpy(pattrib->dst_ip, dst_ip, ETH_ILEN);	

}

void bridgeif_nat_age_one_second()
{
  int i,j;
  BRIDGEIF_DECL_PROTECT(lev);

  BRIDGEIF_READ_PROTECT(lev);

  for (i = 0; i < 4; i++) {
    bridgeif_nat_entry_t *e = &br_nat_entry[i];
    if (e->used) {
		for(j = 0; j < 8 ; j++) {
	      BRIDGEIF_WRITE_PROTECT(lev);
	      /* check again when protected */
	      if (e->src_port[j].ts) {
		  	if(e->src_port[j].type == IP_PROTO_TCP)
				continue;
	        if (--e->src_port[j].ts == 0) {
	          e->src_port[j].port = 0;
	        }
	      }
	      BRIDGEIF_WRITE_UNPROTECT(lev);
		}
    }
  }
  BRIDGEIF_READ_UNPROTECT(lev);
}


void bridgeif_nat_update_src(bridgeif_pkt_attrib_t *pattrib)
{
	int i=0, j=0;
	u8_t exist = 0;

	BRIDGEIF_DECL_PROTECT(lev);
  	
	if(pattrib->port_idx == 0)
		return;

	if(pattrib->dst_ip == NULL || pattrib->src_ip == NULL)
		return;

	BRIDGEIF_READ_PROTECT(lev);
	for(i=0; i<4; i++) {
		bridgeif_nat_entry_t *e = &br_nat_entry[i];
		if(e->used == 1) {
			//update entry			
			if(!memcmp(&e->mac, &pattrib->src_mac, sizeof(struct eth_addr))) {
				BRIDGEIF_WRITE_PROTECT(lev);
				memcpy(e->ip_addr, pattrib->src_ip, ETH_ILEN);
				e->port_num = pattrib->port_idx;
				e->dst_port = pattrib->dst_port;				

				for(j=0; j<8; j++) {
					if(e->src_port[j].port == pattrib->src_port) {
						e->src_port[j].flag = pattrib->flags;
						e->src_port[j].ts = BR_NAT_TIMEOUT_SEC;
						e->src_port[j].type = pattrib->type;
						exist = 1;
						BRIDGEIF_WRITE_UNPROTECT(lev);
        				BRIDGEIF_READ_UNPROTECT(lev);
						return;
					}
				}

				for(j=0; j<8; j++) {
					if(e->src_port[j].port == 0){
						e->src_port[j].port = pattrib->src_port;
						e->src_port[j].flag = pattrib->flags;
						e->src_port[j].ts = BR_NAT_TIMEOUT_SEC;
						e->src_port[j].type = pattrib->type;
						LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,src_port[%d]:%d\n",__func__,j,e->src_port[j].port));
						BRIDGEIF_WRITE_UNPROTECT(lev);
        				BRIDGEIF_READ_UNPROTECT(lev);
						return;
					}
				}
			}
		}
	}
	// not found, create entry
	for(i=0; i<4; i++) {
		bridgeif_nat_entry_t *e = &br_nat_entry[i];
		BRIDGEIF_WRITE_PROTECT(lev);
		if(!e->used) {
			printf("%s==> create src_port:%d,src_ip:%d.%d.%d.%d,src_mac:%02x:%02x:%02x:%02x:%02x:%02x\n",__func__, pattrib->src_port,
					pattrib->src_ip[0], pattrib->src_ip[1], pattrib->src_ip[2], pattrib->src_ip[3],
					pattrib->src_mac.addr[0],pattrib->src_mac.addr[1],pattrib->src_mac.addr[2],pattrib->src_mac.addr[3],pattrib->src_mac.addr[4],pattrib->src_mac.addr[5]);
			
			memcpy(&e->mac, &pattrib->src_mac, sizeof(struct eth_addr));
			memcpy(e->ip_addr, pattrib->src_ip, ETH_ILEN);
			e->src_port[0].port = pattrib->src_port;
			e->src_port[0].flag = pattrib->flags;
			e->src_port[0].ts = BR_NAT_TIMEOUT_SEC;
			e->src_port[0].type = pattrib->type;
			e->dst_port = pattrib->dst_port;
			e->port_num = pattrib->port_idx;
			e->used = 1;
			BRIDGEIF_WRITE_UNPROTECT(lev);
        	BRIDGEIF_READ_UNPROTECT(lev);
			return;
		}
		BRIDGEIF_WRITE_UNPROTECT(lev);
	}
	BRIDGEIF_READ_UNPROTECT(lev);
}

u16_t bridgeif_nat_find_dst(bridgeif_pkt_attrib_t *pattrib)
{
	int i=0, j=0;
	BRIDGEIF_DECL_PROTECT(lev);

	LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,src_port:%d,dst_port:%d\n",__func__,pattrib->src_port,pattrib->dst_port));

	if(pattrib->src_port == 0 || pattrib->dst_port == 0)
		return PORT_TO_NONE;

	BRIDGEIF_READ_PROTECT(lev);
	for(i=0; i<4; i++) {
		bridgeif_nat_entry_t *e = &br_nat_entry[i];
		if(e->used == 1) {
			for(j=0; j<8; j++) {
				if(e->src_port[j].port == pattrib->dst_port) {
					// remove port if TCP_FIN or TCP_RST
					if(e->src_port[j].flag & TCP_FIN || e->src_port[j].flag & TCP_RST){
						LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s, remove port %d\n",__func__, e->src_port[j].port));
						BRIDGEIF_WRITE_PROTECT(lev);
						e->src_port[j].port = 0;
						BRIDGEIF_WRITE_UNPROTECT(lev);
					}
					return PORT_TO_HOST;
				}
			}
		}
	}
	BRIDGEIF_READ_UNPROTECT(lev);

	return PORT_TO_BR;
}

static u8_t
bridgeif_is_local_ip(bridgeif_private_t *br, bridgeif_pkt_attrib_t *pattrib)
{	
	u8_t *local_ip = LwIP_GetIP(br->netif);
	u8_t *pri_ip = LwIP_GetIP(br->ports[1].port_netif);
	struct udp_pcb *pcb;
	u8_t for_us = 0;
	
	if(pattrib->dst_ip == NULL)
		return IP_TO_NONE;

	//printf("%s==> dst_ip:%d.%d.%d.%d\n",__func__,
	//pattrib->dst_ip[0], pattrib->dst_ip[1], pattrib->dst_ip[2], pattrib->dst_ip[3]);
	if(!memcmp(local_ip, pattrib->dst_ip, ETH_ILEN))
		return IP_TO_BR;
	else if (!memcmp(pri_ip, pattrib->dst_ip, ETH_ILEN)){
		LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s==> pri_ip:%d.%d.%d.%d,dst_ip:%d.%d.%d.%d\n",__func__, 
		pri_ip[0], pri_ip[1], pri_ip[2], pri_ip[3],
		pattrib->dst_ip[0], pattrib->dst_ip[1], pattrib->dst_ip[2], pattrib->dst_ip[3]));

		switch(pattrib->type) {
		case IP_PROTO_UDP:
			pcb = NULL;
			for(pcb = udp_pcbs; pcb != NULL; pcb = pcb->next) {
				if(pcb->local_port == pattrib->dst_port) {
					for_us = 1;
					return IP_TO_WLAN1;
				}	
					
			}
			if(!for_us)
				return IP_TO_NONE;
			break;
		}

		return IP_TO_WLAN1;
	}

	return IP_TO_NONE;
}

u32_t bridgeif_tcpiph_chksum(struct ip_hdr *iphdr, struct pbuf *p)
{
	u32_t ip_chk_sum = 0;
	u16_t iphdr_hlen = 0;
	u8_t bak[ETH_HLEN];
	struct tcp_hdr *tcphdr = NULL;
	struct udp_hdr *udphdr = NULL;
	struct icmp_echo_hdr *icmphdr = NULL;
	
	if(iphdr != NULL) {
		u8_t proto = iphdr->_proto;
		u8_t ttl = iphdr->_ttl;
		u8_t tos = iphdr->_tos;

		iphdr_hlen = IPH_HL(iphdr) * 4;
		
		ip_chk_sum += PP_NTOHS(tos | (iphdr->_v_hl << 8));
		
		ip_chk_sum += PP_NTOHS(proto | (ttl << 8));
		ip_chk_sum += ip4_addr_get_u32(&iphdr->dest) & 0xFFFF;
	    ip_chk_sum += ip4_addr_get_u32(&iphdr->dest) >> 16;
		
		ip_chk_sum += iphdr->_len;	
		ip_chk_sum += iphdr->_id;
		ip_chk_sum += iphdr->_offset;

		ip_chk_sum += ip4_addr_get_u32(&iphdr->src) & 0xFFFF;
	    ip_chk_sum += ip4_addr_get_u32(&iphdr->src) >> 16;
	    ip_chk_sum = (ip_chk_sum >> 16) + (ip_chk_sum & 0xFFFF);
	    ip_chk_sum = (ip_chk_sum >> 16) + ip_chk_sum;
	    ip_chk_sum = ~ip_chk_sum;
		iphdr->_chksum = (u16_t)ip_chk_sum;

		memcpy(bak, p->payload, ETH_HLEN);
		pbuf_header(p, - (ETH_HLEN + (s16_t)iphdr_hlen));

		switch(iphdr->_proto) {
		case IP_PROTO_ICMP://ICMP
			//printf("===============ICMP\n");
			icmphdr = (struct icmp_echo_hdr *)p->payload;
			if(icmphdr != NULL) {
				icmphdr->chksum = 0;
				icmphdr->chksum = inet_chksum(p->payload, p->tot_len);
			}
			break;
		case IP_PROTO_TCP://TCP
			//printf("===============TCP\n");
			tcphdr = (struct tcp_hdr *)p->payload;
			if(tcphdr != NULL) {
				tcphdr->chksum = 0;
				tcphdr->chksum = ip_chksum_pseudo(p, iphdr->_proto,
		     						 p->tot_len, &iphdr->src, &iphdr->dest);

			}			
			break;
		case IP_PROTO_UDP://UDP
			//printf("===============UDP\n");
			udphdr = (struct udp_hdr *)p->payload;
			udphdr->chksum = 0;			
			udphdr->chksum = ip_chksum_pseudo(p, IP_PROTO_UDP, lwip_htons(udphdr->len),
            &iphdr->src, &iphdr->dest);

	        /* chksum zero must become 0xffff, as zero means 'no checksum' */
	        if (udphdr->chksum == 0) {
	          udphdr->chksum = 0xffff;
	        }
			break;
		}

		pbuf_header_force(p, (ETH_HLEN + iphdr_hlen)); /* Move to ip header, no check necessary. */
        memcpy(p->payload, bak , ETH_HLEN);
		memcpy(p->payload + ETH_HLEN, iphdr, sizeof(struct ip_hdr));
	}

	return 0;	
}

u32_t bridgeif_ip_send_to_ports(bridgeif_private_t *br,  bridgeif_pkt_attrib_t *pattrib, struct pbuf *p)
{
	int i=0;
	bridgeif_portmask_t dstports = 2;
	u8_t *local_ip = LwIP_GetIP(br->netif);
	u8_t *pri_ip = LwIP_GetIP(br->ports[1].port_netif);
	u8_t last_ip[ETH_ILEN] = {0};
	struct netif *portif;

	struct ip_hdr *iphdr = (struct ip_hdr *)(p->payload + ETH_HLEN);

	for(i=0; i<4; i++) {
		if(br_nat_entry[i].used) {
			if(!memcmp(last_ip, br_nat_entry[i].ip_addr, ETH_ILEN))
				continue;
			// replace src mac dst mac, dst ip
			// AP->STA
			if(pattrib->port_idx == 0) {
				dstports = 1;
				// dst mac
				memcpy(p->payload, br_nat_entry[i].mac.addr, ETH_ALEN);
				//dst ip
				//broadcast or multicast not replace
				if(!ip4_addr_ismulticast(&iphdr->dest) && 
				   !ip4_addr_isbroadcast(&iphdr->dest, br->netif)) {
					memcpy(&iphdr->dest, br_nat_entry[i].ip_addr, ETH_ILEN);
				} else {
					LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("broadcast or multicast packet!\n"));
				}
				// src mac
				memcpy(p->payload + ETH_ALEN, br->netif->hwaddr, ETH_ALEN);
				// src ip
				if(pattrib->src_port == DNS_SERVER_PORT)
					memcpy(&iphdr->src, pri_ip, ETH_ILEN);
				
			} else if (pattrib->port_idx == 1) {
				dstports = 0;
				/* STA->AP, replace src_ip as local_ip, replace src_mac as local_mac
				  replace dst
				*/
				//src mac
				memcpy(p->payload + ETH_ALEN, br->netif->hwaddr, ETH_ALEN);
				//src ip
				memcpy(&iphdr->src, local_ip, ETH_ILEN);	

				/* special case for DNS */
				if(pattrib->dst_port == DNS_SERVER_PORT) {
					u8 dns[4] = {0};
					//struct ip_addr* dns;
					LwIP_GetDNS((struct ip_addr* )dns);
					LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,dns:%d.%d.%d.%d\n",__func__, dns[0],dns[1],dns[2],dns[3]));
					memcpy(&iphdr->dest, dns, ETH_ILEN);
				}
			}

			bridgeif_tcpiph_chksum(iphdr, p);
			portif = br->ports[dstports].port_netif;

			if(dstports == 0) {
				//remove eth header
				pbuf_header(p, -ETH_HLEN);
				//send to etharp_output
				
				br->netif->output(br->netif, p, &iphdr->dest);
			} else {
				//direct send to sdio host
				portif->linkoutput(portif, p);
			}
			memcpy(last_ip, br_nat_entry[i].ip_addr, ETH_ILEN);
		}				
	}

	return 0;
}

u32_t bridgeif_arp_send_to_ports(bridgeif_private_t *br, bridgeif_pkt_attrib_t *pattrib, struct pbuf *p)
{
	int i=0;
	bridgeif_portmask_t dstports;
	u8_t *local_ip = LwIP_GetIP(br->netif);
	u8_t last_ip[ETH_ILEN] = {0};
	struct netif *portif;
	
	struct etharp_hdr *arphdr = (struct etharp_hdr *)(p->payload + ETH_HLEN);;

	for(i=0; i<4; i++) {
		if(br_nat_entry[i].used) {
			if(!memcmp(last_ip, br_nat_entry[i].ip_addr, ETH_ILEN))
				continue;

			// replace src mac dst mac, dst ip
			memcpy(p->payload + ETH_ALEN, br->netif->hwaddr, ETH_ALEN);
			memcpy(&arphdr->shwaddr, br->netif->hwaddr, ETH_ALEN);
	
			// AP->STA
			if(pattrib->port_idx == 0) {
				dstports = 1;
				// dst mac					
				memcpy(p->payload, br_nat_entry[i].mac.addr, ETH_ALEN);					
				// dst mac arp
				memcpy(&arphdr->dhwaddr, br_nat_entry[i].mac.addr, ETH_ALEN);
				// dst ip arp
				memcpy(&arphdr->dipaddr, br_nat_entry[i].ip_addr, ETH_ILEN);							
			} else if (pattrib->port_idx == 1) {
				dstports = 0;
				// STA->AP, replace src_ip as local_ip
				// replace src_mac as local_mac
				memcpy(&arphdr->sipaddr, local_ip, ETH_ILEN);			
			}
			portif = br->ports[dstports].port_netif;
			if(dstports == 0) {
				br->netif->linkoutput(br->netif, p);
			} else {
				//direct send to sdio host
				portif->linkoutput(portif, p);
			}
			memcpy(last_ip, br_nat_entry[i].ip_addr, ETH_ILEN);
		}			
	
	}

	return 0;
}

u32_t bridgeif_nat_send_to_ports(bridgeif_private_t *br, bridgeif_pkt_attrib_t *pattrib, struct pbuf *p)
{
	LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,protocol:%x, ip:%x, arp:%x\n",__func__,pattrib->protocol,lwip_htons(ETH_P_IP),lwip_htons(ETH_P_ARP)));
	if(pattrib->protocol == lwip_htons(ETH_P_IP)) {		
		return bridgeif_ip_send_to_ports(br, pattrib, p);				
	} else if(pattrib->protocol == lwip_htons(ETH_P_ARP)) {
		return bridgeif_arp_send_to_ports(br, pattrib, p);
	}
	return 0;
}

/** The actual bridge input function. Port netif's input is changed to call
 * here. This function decides where the frame is forwarded.
 */
static err_t
bridgeif_input(struct pbuf *p, struct netif *netif)
{
  u8_t rx_idx;
  bridgeif_portmask_t dstports;
  struct eth_addr *src, *dst;
  bridgeif_private_t *br;
  bridgeif_port_t *port;
  struct pbuf *q;
  bridgeif_pkt_attrib_t *pattrib;
  
  if (p == NULL || netif == NULL) {
    return ERR_VAL;
  }
  
  port = (bridgeif_port_t *)netif_get_client_data(netif, bridgeif_netif_client_id);
  
  LWIP_ASSERT("port data not set", port != NULL);
  if (port == NULL || port->bridge == NULL) {
    return ERR_VAL;
  }

  br = (bridgeif_private_t *)port->bridge;
  rx_idx = netif_get_index(netif);

  /* store receive index in pbuf */
  p->if_idx = rx_idx;
 
  dst = (struct eth_addr *)p->payload;
  src = (struct eth_addr *)(((u8_t *)p->payload) + sizeof(struct eth_addr)); 

  pattrib = (bridgeif_pkt_attrib_t *)malloc(sizeof(bridgeif_pkt_attrib_t));
  bridgeif_update_attrib(p, pattrib);

  pattrib->port_idx = port->port_num;

  if ((src->addr[0] & 1) == 0) {
    /* update src for all non-group addresses */
    bridgeif_fdb_update_src(br->fdbd, src, port->port_num);
	bridgeif_nat_update_src(pattrib);	
  }

  if (dst->addr[0] & 1) {
    /* group address -> flood + cpu? */
    dstports = bridgeif_find_dst_ports(br, dst);
    //bridgeif_send_to_ports(br, p, dstports);

    if (dstports & (1 << BRIDGEIF_MAX_PORTS)) {
      /* we pass the reference to ->input or have to free it */
      LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> input(%p)\n", (void *)p));
	  if (br->netif->input(p, br->netif) != ERR_OK) {
		  pbuf_free(p);	
	  }	  
    } else {
      /* all references done */
      pbuf_free(p);
    }
	free(pattrib);
    /* always return ERR_OK here to prevent the caller freeing the pbuf */
    return ERR_OK;
  } else {
    /* is this for one of the local ports? */
	LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("\n\r%s:port->port_num:%d,netif index:%d,%x,dst:%02x:%02x:%02x:%02x:%02x:%02x,src:%02x:%02x:%02x:%02x:%02x:%02x\n",
  	__func__,
  	port->port_num,
  	rx_idx,
  	pattrib->protocol,
  	dst->addr[0],dst->addr[1],dst->addr[2],dst->addr[3],dst->addr[4],dst->addr[5],
  	src->addr[0],src->addr[1],src->addr[2],src->addr[3],src->addr[4],src->addr[5])); 
  
    if (bridgeif_is_local_mac(br, dst)) {
        #ifdef CONFIG_CONCURRENT_MODE  
        br_rpt_handle_frame(br->netif, p);
        if (bridgeif_is_local_mac(br, dst)) {	
			/* input port num == 1, input from sdio host */
			if(port->port_num == 1) {
				u8_t flag = bridgeif_is_local_ip(br, pattrib);
				LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,flag = %d\n",__func__,flag));

				if(flag == IP_TO_BR || flag == IP_TO_WLAN1) {

					if (p->if_idx > 0) {
		              p->if_idx = 0;
		            }
		            /* yes, send to cpu port only */
		            LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> input(%p)\n", (void *)p));
					free(pattrib);
					return br->netif->input(p, br->ports[port->port_num].port_netif);
				} else {
					//send to all stas						
					bridgeif_nat_send_to_ports(br, pattrib, p);
					pbuf_free(p);
					free(pattrib);
					return ERR_OK;
				}
			}

			/* input port num == 0, input from wlan0 */
			u16_t tcp_port = bridgeif_nat_find_dst(pattrib);
			LWIP_DEBUGF(BRIDGEIF_FW_DEBUG,("%s,tcp port:%d\n",__func__,tcp_port));

			switch(tcp_port) {
				case PORT_TO_NONE: {
					//pbuf copy
					q = pbuf_alloc(PBUF_RAW, p->tot_len, PBUF_POOL);
					if (q == NULL) {
						LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("%s,pbuf alloc failed!\n",__func__));
						return ERR_VAL;
					}

					if(ERR_OK != pbuf_copy(q, p)) {
						LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("%s,pbuf copy failed!\n",__func__));
						pbuf_free(q);
						return ERR_VAL; 
					}

					q->if_idx = rx_idx;

					if (p->if_idx > 0) {
		              p->if_idx = 0;
		            }
		            /* yes, send to cpu port only */
		            LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> input(%p)\n", (void *)p));
					if(br->netif->input(p, br->netif) != ERR_OK ) {
						pbuf_free(p);
					}									
					// AP->STA, send to all stas 	
					bridgeif_nat_send_to_ports(br, pattrib, q);
					free(pattrib);
					pbuf_free(q);
				}break;
				case PORT_TO_BR: {
					if (p->if_idx > 0) {
		              p->if_idx = 0;
		            }
		            /* yes, send to cpu port only */
		            LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> input(%p)\n", (void *)p));
					free(pattrib);
					return br->netif->input(p, br->netif);
				}break;
				case PORT_TO_HOST: {
					// AP->STA, send to sdio host 	
					bridgeif_nat_send_to_ports(br, pattrib, p);
					free(pattrib);
					pbuf_free(p);
				}break;
			}
            return ERR_OK;
        } 
        #else
        /* yes, send to cpu port only */
        LWIP_DEBUGF(BRIDGEIF_FW_DEBUG, ("br -> input(%p)\n", (void *)p));
        return br->netif->input(p, br->netif);
        #endif
    }

	// AP->STA, send to all stas		
	bridgeif_nat_send_to_ports(br, pattrib, p);
    /* no need to send to cpu, flooding is for external ports only */
    /* by  this, we consumed the pbuf */
    pbuf_free(p);
	free(pattrib);
    /* always return ERR_OK here to prevent the caller freeing the pbuf */
    return ERR_OK;
  }
}

#if !BRIDGEIF_PORT_NETIFS_OUTPUT_DIRECT
/** Input function for port netifs used to synchronize into tcpip_thread.
 */
#ifdef CONFIG_CONCURRENT_MODE  
err_t
#else
static err_t
#endif
bridgeif_tcpip_input(struct pbuf *p, struct netif *netif)
{
  return tcpip_inpkt(p, netif, bridgeif_input);
}
#endif /* BRIDGEIF_PORT_NETIFS_OUTPUT_DIRECT */

/**
 * @ingroup bridgeif
 * Initialization function passed to netif_add().
 *
 * ATTENTION: A pointer to a @ref bridgeif_initdata_t must be passed as 'state'
 *            to @ref netif_add when adding the bridge. I supplies MAC address
 *            and controls memory allocation (number of ports, FDB size).
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
bridgeif_init(struct netif *netif)
{
  bridgeif_initdata_t *init_data;
  bridgeif_private_t *br;
  size_t alloc_len_sizet;
  mem_size_t alloc_len;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
  LWIP_ASSERT("bridgeif needs an input callback", (netif->input != NULL));
#if !BRIDGEIF_PORT_NETIFS_OUTPUT_DIRECT
  if (netif->input == tcpip_input) {
    LWIP_DEBUGF(BRIDGEIF_DEBUG | LWIP_DBG_ON, ("bridgeif does not need tcpip_input, use netif_input/ethernet_input instead"));
  }
#endif

  if (bridgeif_netif_client_id == 0xFF) {
    bridgeif_netif_client_id = netif_alloc_client_data_id();
	//printf("bridgeif_netif_client_id:%d\n",bridgeif_netif_client_id);
  }

  init_data = (bridgeif_initdata_t *)netif->state;
  LWIP_ASSERT("init_data != NULL", (init_data != NULL));
  LWIP_ASSERT("init_data->max_ports <= BRIDGEIF_MAX_PORTS",
              init_data->max_ports <= BRIDGEIF_MAX_PORTS);

  alloc_len_sizet = sizeof(bridgeif_private_t) + (init_data->max_ports * sizeof(bridgeif_port_t) + (init_data->max_fdb_static_entries * sizeof(bridgeif_fdb_static_entry_t)));
  alloc_len = (mem_size_t)alloc_len_sizet;
  LWIP_ASSERT("alloc_len == alloc_len_sizet", alloc_len == alloc_len_sizet);
  LWIP_DEBUGF(BRIDGEIF_DEBUG, ("bridgeif_init: allocating %d bytes for private data\n", (int)alloc_len));
  br = (bridgeif_private_t *)mem_calloc(1, alloc_len);
  if (br == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("bridgeif_init: out of memory\n"));
    return ERR_MEM;
  }
  memcpy(&br->ethaddr, &init_data->ethaddr, sizeof(br->ethaddr));
  br->netif = netif;

  br->max_ports = init_data->max_ports;
  br->ports = (bridgeif_port_t *)(br + 1);
  //printf("br:0x%x,br->ports:0x%x\n",br,br->ports);

  br->max_fdbs_entries = init_data->max_fdb_static_entries;
  br->fdbs = (bridgeif_fdb_static_entry_t *)(((u8_t *)(br + 1)) + (init_data->max_ports * sizeof(bridgeif_port_t)));

  br->max_fdbd_entries = init_data->max_fdb_dynamic_entries;
  br->fdbd = bridgeif_fdb_init(init_data->max_fdb_dynamic_entries);
  if (br->fdbd == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("bridgeif_init: out of memory in fdb_init\n"));
    mem_free(br);
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, 0);

  netif->state = br;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
#if LWIP_IPV4
  netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
  netif->linkoutput = bridgeif_output;

  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;

  /* set MAC hardware address */
  memcpy(netif->hwaddr, &br->ethaddr, ETH_HWADDR_LEN);

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6 | NETIF_FLAG_LINK_UP;

#if LWIP_IPV6 && LWIP_IPV6_MLD
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if (netif->mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
#ifdef CONFIG_CONCURRENT_MODE 
  rpt_handle_init(netif->hwaddr);
#endif
  return ERR_OK;
}

/**
 * @ingroup bridgeif
 * Add a port to the bridge
 */
err_t
bridgeif_add_port(struct netif *bridgeif, struct netif *portif)
{
  bridgeif_private_t *br;
  bridgeif_port_t *port;

  LWIP_ASSERT("bridgeif != NULL", bridgeif != NULL);
  LWIP_ASSERT("bridgeif->state != NULL", bridgeif->state != NULL);
  LWIP_ASSERT("portif != NULL", portif != NULL);

  if (!(portif->flags & NETIF_FLAG_ETHARP) || !(portif->flags & NETIF_FLAG_ETHERNET)) {
    /* can only add ETHERNET/ETHARP interfaces */
    return ERR_VAL;
  }

  br = (bridgeif_private_t *)bridgeif->state;

  if (br->num_ports >= br->max_ports) {
    return ERR_VAL;
  }
  port = &br->ports[br->num_ports];
  port->port_netif = portif;
  port->port_num = br->num_ports;
  port->bridge = br;
  br->num_ports++;

  /* let the port call us on input */
#if BRIDGEIF_PORT_NETIFS_OUTPUT_DIRECT
  portif->input = bridgeif_input;
#else
  portif->input = bridgeif_tcpip_input;
#endif
  /* store pointer to bridge in netif */
  //printf("port->port_num:%d,bridgeif_netif_client_id:%d\n",port->port_num, bridgeif_netif_client_id);
  netif_set_client_data(portif, bridgeif_netif_client_id, port);
  /* remove ETHARP flag to prevent sending report events on netif-up */
  netif_clear_flags(portif, NETIF_FLAG_ETHARP);

  return ERR_OK;
}

err_t bridgeif_mac_init(struct netif *netif, char *bmac)
{
  bridgeif_private_t *br;
  br = netif->state;
  
  memcpy(&br->ethaddr, bmac, sizeof(br->ethaddr));
  memcpy(netif->hwaddr, &br->ethaddr, ETH_HWADDR_LEN);
  
  return 0;
}

void show_bridgeif_fdbd(struct netif *netif)
{
  bridgeif_private_t *br;
  br = netif->state;
  bridgeif_fdbd_dump(br->fdbd);
    
}
u8_t bridgeif_get_portnum(struct netif *bridgeif)
{
  bridgeif_private_t *br;
  br = (bridgeif_private_t *)bridgeif->state;
  return br->num_ports;
}
#endif /* LWIP_NUM_NETIF_CLIENT_DATA */
