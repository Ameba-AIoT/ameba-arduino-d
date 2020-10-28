#ifndef HWMP_COMMON_H
#define HWMP_COMMON_H

#include "basic_types.h"
#include "wireless.h"
#include "autoconf.h"
#include "netif.h"
#include "wifi_conf.h"
#include "wifi_util.h"
#include "task.h"

#ifndef __uint64_t
typedef __uint64_t	u_int64_t;
#endif

#ifndef caddr_t
typedef void* caddr_t;
#endif

#define USE_IPC             0

#if     USE_IPC
#define IPC_BASED_PATHSEL
#else
#undef  IPC_BASED_PATHSEL
#define FIFO_SIZE           1024
static void *FIFO_BUF;
#endif

#ifndef _AMEBA_
#define _AMEBA_
#endif

#ifndef _RTL8187B_
#define _RTL8187B_
#endif

#ifndef ETH_P_ALL
#define ETH_P_ALL 0x0003
#endif

#define BR_STATE_FORWARDING 3
#define BR_STATE_BLOCKING   4
#define PF_PACKET           17 //anyway, "domain" will not be used in lwip_socket() 

#define NODE_CFG_PORTOAL	1
#define NODE_CFG_BE_ROOT	0
#define NODE_CFG_DAEMON		0
#define NODE_CFG_IFNAME     "wlan0"
#define NODE_CFG_IFACE_IDX  1 //interface index for "wlan0"

#ifndef sockaddr_ll
struct sockaddr_ll {
	unsigned short  sll_family;
	unsigned short  sll_protocol;
	int		        sll_ifindex;
	unsigned short	sll_hatype;
	unsigned char	sll_pkttype;
	unsigned char	sll_halen;
	unsigned char	sll_addr[8];
};
#endif


#ifndef timespec
typedef unsigned long time_t;
struct timespec {
  time_t  tv_sec;
  time_t  tv_nsec;
};
#endif 

#define     _FIXIT_

extern int iw_ioctl(const char * ifname, unsigned long request, struct iwreq *	pwrq);

#endif /* HWMP_COMMON_H */
