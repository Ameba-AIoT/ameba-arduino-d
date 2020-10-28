#ifndef __MESH_ROUTE_H__
#define __MESH_ROUTE_H__

#include "mesh.h"

#ifndef CONFIG_RTK_MESH	//if"N"def For 8190n.h wlan_device[]
#define MESH_SHIFT			0
#define MESH_NUM_CFG		0
#else
#define MESH_SHIFT			8 // ACCESS_MASK uses 2 bits, WDS_MASK use 4 bits
#define MESH_MASK			0x3
#define MESH_NUM_CFG		NUM_MESH

#define MESH_MAX_PREQ_RETRIES		4
#define HWMP_MAX_PREQ_RETRIES 		MESH_MAX_PREQ_RETRIES-1

#define HWMP_MAX_PREQ_RETRIES 		2

#define PROXY_TBL_AGING_LIMIT		300	//proxy table expire time
#define HWMP_ACTIVE_PATH_TIMEOUT	300	//pathsel table expire time, chuangch
#define HWMP_PREQ_REFRESH_PERIOD	60	//route maintenance, chuangch 10.19
#define HWMP_PREQ_REFRESH_PERIOD2	200	//route maintenance, chuangch 10.19


#if defined(RTK_MESH_AODV_STANDALONE_TIMER)
#define HWMP_NETDIAMETER_TRAVERSAL_TIME MESH_AODV_EXPIRE_TO*3 //miliseconds
#else
#define HWMP_NETDIAMETER_TRAVERSAL_TIME HZ
#endif

#ifdef PU_STANDARD	// For 802.11 Mesh proxy update
enum PU_flag
{
    PU_add,
    PU_delete
};
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct mesh_rreq_retry_entry {
    unsigned char		MACAddr[ETH_ALEN];
    long int			TimeStamp;
    long int			createTime;
    unsigned char		Retries; // add by chuangch, the num of rreq retries
    u8*					da;
    u8					flag;
    u8					back_off_retries;
    struct pkt_queue    pktqueue;
    struct mesh_rreq_retry_entry *rreq_next;
    struct mesh_rreq_retry_entry *rreq_prev;    
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211s_hdr {
    u8    flags;
    u8    ttl;//s8 TTL
    u32   seqnum;//u16 segNum;
    u8    eaddr1[ETH_ALEN]; // modify for 6 address
    u8    eaddr2[ETH_ALEN];
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct proxy_table_entry	 {
    unsigned char 	sta[ETH_ALEN];
    unsigned char 	owner[ETH_ALEN];
    // unsigned char	type; // 0: others, 1: my_sta, 2: my bridge
    short aging_time;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct proxyupdate_table_entry	 {
    u8	PUflag, isMultihop, PUSN, retry;
    unsigned short	STAcount;
    unsigned char	nexthopmac[ETH_ALEN];
    unsigned char	destproxymac[ETH_ALEN];
    unsigned char 	proxymac[ETH_ALEN];
    unsigned char   proxiedmac[ETH_ALEN];
    _adapter *priv;    
    unsigned long update_time;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef SUPPORT_TX_MCAST2UNI
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct mesh_ip_mcast_info {
	int					used;
	unsigned char		mcmac[ETH_ALEN];

    /*src record*/
    unsigned char srcCount;
    unsigned char srcValid[MAX_IP_SRC_ENTRY];
    unsigned char srcMac[MAX_IP_SRC_ENTRY][ETH_ALEN];	
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct path_sel_entry {
    unsigned char destMAC[ETH_ALEN];
    unsigned char nexthopMAC[ETH_ALEN];
    unsigned int dsn;
    unsigned int metric;
    unsigned char hopcount;
    // the following entries are NOT shared by daemon and driver
    //	unsigned char isvalid;
    //	unsigned char modify_time[8];
    unsigned int start, end;
    unsigned short RecentSeq[SZ_HASH_IDX2];
    _adapter *priv;
    unsigned long update_time;
    unsigned long routeMaintain;
    #ifdef  SUPPORT_TX_MCAST2UNI
	int	ipmc_num;
	struct mesh_ip_mcast_info	ipmc[MAX_IP_MC_ENTRY];
    #endif

	u8 rann_snd_addr[ETH_ALEN];
	u32 rann_metric;
	bool is_root;
	bool is_gate;
	u32 sn;
	unsigned long exp_time;
	enum mesh_path_flags flags;
	u32 discovery_timeout;
	unsigned long last_preq_to_root;
	
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

struct pann_mpp_tb_entry {
    unsigned char mac[ETH_ALEN];
    unsigned int timeout;
    unsigned int seqNum;
    short int flag;
    int beRoot;
};


struct mpp_tb {
    struct pann_mpp_tb_entry pann_mpp_pool[MAX_MPP_NUM];
    unsigned int 		 pool_count ;
};



// _DOT11s_XXX serires struct size need less than MAXDATALEN(1560)	, which defined in iwreq.c
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
typedef  struct _DOT11s_GEN_RREQ_PACKET{
    unsigned char	EventId;
    unsigned char   IsMoreEvent;
    unsigned char	MyMACAddr[ETH_ALEN];
    unsigned char	destMACAddr[ETH_ALEN];
    unsigned char   TTL;
    unsigned short  Seq_num;
}
RTW_PACK_STRUCT_STRUCT
DOT11s_GEN_RREQ_PACKET;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
typedef struct _DOT11s_GEN_RERR_PACKET{
    unsigned char	EventId;
    unsigned char   IsMoreEvent;
    unsigned char	MyMACAddr[ETH_ALEN];
    unsigned char	SorNMACAddr[ETH_ALEN];	//this address is the neighbor that hello message finds it is unreachabel.
    											// if flag == 1, this field means disconnected neighbor MAC addr
    											// if flag == 2, this field means source MAC addr of the data frame
    unsigned char	DataDestMAC[ETH_ALEN];	// this field is valid if and only if flag == 2
    unsigned char	PrehopMAC[ETH_ALEN];		// this field is valid if and only if flag == 2
    unsigned char   TTL;
    unsigned short  Seq_num;
    unsigned char   Flag;						// flag = 1 means the link to neighbor has broken
												// flag = 2 means this MP doesn't have the nexthop information for the destination in pathseleciton table
}
RTW_PACK_STRUCT_STRUCT
DOT11s_GEN_RERR_PACKET;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
typedef struct _DOT11s_RECV_PACKET{
    unsigned char	EventId;
    unsigned char   IsMoreEvent;
    unsigned char	MyMACAddr[ETH_ALEN];
    unsigned char	PreHopMACAddr[ETH_ALEN];
    unsigned char	DesMACAddr[ETH_ALEN];
    unsigned int  Pre_Hop_Metric;
    unsigned char ReceiveData[128];
    unsigned char   TTL;
    unsigned short  Seq_num;
    unsigned char Is6AddrFormat;
    unsigned char	MACAddr5[ETH_ALEN];
    unsigned char	MACAddr6[ETH_ALEN];
#ifdef RTK_MESH_METRIC_REFINE
    unsigned char	InterfaceMACAddr[ETH_ALEN];
    unsigned int	RSSI_Metric;
    unsigned int	Cu_Metric;
    unsigned int	Noise_Metric;
#endif
}
RTW_PACK_STRUCT_STRUCT
DOT11s_RECV_PACKET;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

void init_mpp_pool(struct mpp_tb* pTB);
void aodv_expire(_adapter *padapter);

#endif // end of not define CONFIG_RTK_MESH

#endif//__MESH_ROUTE_H__