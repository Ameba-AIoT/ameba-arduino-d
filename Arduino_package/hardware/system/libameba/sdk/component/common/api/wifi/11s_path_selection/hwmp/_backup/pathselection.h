#ifndef PATHSELECTION_H
#define PATHSELECTION_H

#include "common.h"
#include "sockets.h"

// Original define in \linux-2.4.18\include\linux\byteorder\generic.h
//#define cpu_to_le16 __cpu_to_le16
//#define le16_to_cpu __le16_to_cpu

#define PATHSEL_FIFO		"/var/pathselection.fifo"
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define MAX_MSG_SIZE	1600
//#define WLAN_HDR_A4_MESH_MGT_LEN 34
#define MACADDRLEN		6
#define MAX_MPP_NUM		15
#define _MESH_HEADER_TTL_  			MAX_MPP_NUM

#define printMac(addr)	printf("%02x%02x%02x%02x%02x%02x", \
		0xff& addr[0],			0xff& addr[1],\
		0xff& addr[2],			0xff& addr[3],\
		0xff& addr[4],			0xff& addr[5] );


#if defined(_RTL865X_) || defined(_AMEBA_)
// for IOCTL to get/set kernel table info
#define SIOCQPATHTABLE  0x8BA0  // query pathselection table
#define SIOCUPATHTABLE  0x8BA1  // update  existing entry's date in pathselection table
#define SIOCAPATHTABLE  0x8BA2  // add a new entry into pathselection table
//#define INIT_PATHTABLE  0x8BA5 // initial the path selection table, set all isvalid bit to 0
#define SET_PORTAL_POOL 0x8BA8
#define SIOC_NOTIFY_PATH_CREATE 0x8BA9 // path selection daemon notify dirver that the path to des mac has created
#define SIOC_UPDATE_ROOT_INFO 0x8BAA // update root mac into driver
#define  SIOC_GET_ROUTING_INFO 0x8BAB // send routing info to user space
#define  SIOC_SET_ROUTING_INFO 0x8BAD
//#define  SIOC_GET_PATHSWITCH_THD 0x8BAD // 

#define  DOT11_EVENT_PATHSEL_GEN_RREQ  59 
#define  DOT11_EVENT_PATHSEL_GEN_RERR  60
#define  DOT11_EVENT_PATHSEL_RECV_RREQ  61                    
#define  DOT11_EVENT_PATHSEL_RECV_RREP  62                   
#define  DOT11_EVENT_PATHSEL_RECV_RERR  63
#define  DOT11_EVENT_PATHSEL_RECV_PANN  65
#define  DOT11_EVENT_PATHSEL_RECV_RANN  66
#define MacAddrLen 6

//tsananiu begin
#define SAVEPID_IOCTL   0x8BB0   //PID ioctl
#define DEQUEUEDATA_IOCTL 0x8BB1   //DEQUEUE ioctl
//tsananiu

#else   // 8186, 8187
// for IOCTL to get/set kernel table info
#define SIOCQPATHTABLE  0x8B80  // query pathselection table
#define SIOCUPATHTABLE  0x8B81  // update  existing entry's date in pathselection table
#define SIOCAPATHTABLE  0x8B82  // add a new entry into pathselection table
#define INIT_PATHTABLE  0x8B85  // initial the path selection table, set all isvalid bit to 0
#define SET_PORTAL_POOL 0x8B88
#define SIOC_NOTIFY_PATH_CREATE 0x8B89 // path selection daemon notify dirver that the path to des mac has created
#define SIOC_UPDATE_ROOT_INFO 0x8B8A // update root mac into driver
#define  SIOC_GET_ROUTING_INFO 0x8B8B // send routing info to user space
//#define  SIOC_GET_PATHSWITCH_THD 0x8B8D // 

//tsananiu begin
#define SAVEPID_IOCTL   0x8B90   //PID ioctl
#define DEQUEUEDATA_IOCTL 0x8B91   //DEQUEUE ioctl
//tsananiu
#endif   // _RTL865X_

#define _MESH_CATEGORY_ID_             13

/*used in Mesh category */
#define _HWMP_MESH_PATHSEL_ACTION_ID_  1
#define _GANN_ACTION_ID_               2


#define _MESH_GANN_IE_	            125
#define _MESH_RANN_IE_	            126
#define _MESH_PREQ_IE_	            130
#define _MESH_PREP_IE_	            131
#define _MESH_PERR_IE_	            132



// The following info for 8186 is defined in AP/auth/include/1x_ioct.h
#if defined(_RTL865X_) || defined(_RTL8187B_) || defined(_AMEBA_)

// for backward compatible with non-ipc-based
// struct size need less than MAXDATALEN(1560)	, which defined in iwreq.c

#pragma pack(1)
typedef struct _DOT11s_GEN_RREQ_PACKET{
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[MACADDRLEN];
	unsigned char	destMACAddr[MACADDRLEN];
	unsigned char	TTL;
	unsigned short	Seq_num;
}DOT11s_GEN_RREQ_PACKET;
#pragma pack()

#pragma pack(1)
typedef struct _DOT11s_GEN_RERR_PACKET{
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[MACADDRLEN];
	unsigned char	SorNMACAddr[MACADDRLEN];   //this address is the neighbor that hello message finds it is unreachabel.
                                                // if flag == 1, this field means disconnected neighbor MAC addr
                                                // if flag == 2, this field means source MAC addr of the data frame
	unsigned char	DataDestMAC[MACADDRLEN];	// this field is valid if and only if flag == 2
	unsigned char	PrehopMAC[MACADDRLEN];		// this field is valid if and only if flag == 2
    unsigned char   TTL;
    unsigned short  Seq_num;
    unsigned char   Flag;       // flag = 1 means the link to neighbor has broken
                                // flag = 2 means this MP doesn't have the nexthop information for the destination in pathseleciton table
}DOT11s_GEN_RERR_PACKET;
#pragma pack()

#pragma pack(1)
typedef struct _DOT11s_RECV_PACKET{
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[MACADDRLEN];
	unsigned char	PreHopMACAddr[MACADDRLEN];
	unsigned char	DesMACAddr[MACADDRLEN];
	unsigned int	Pre_Hop_Metric;
	unsigned char	ReceiveData[128];
	unsigned char	TTL;
	unsigned short	Seq_num;
	unsigned char	Is6AddrFormat;
	unsigned char	MACAddr5[MACADDRLEN];
	unsigned char	MACAddr6[MACADDRLEN];
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned char	InterfaceMACAddr[MACADDRLEN];
	unsigned int	RSSI_Metric;
	unsigned int	Cu_Metric;
	unsigned int	Noise_Metric;
#endif
}DOT11s_RECV_PACKET;
#pragma pack()

#endif // defined(_RTL865X_) || defined(_RTL8187B_)

#define _TO_DS_		BIT(8)
#define _FROM_DS_	BIT(9)

// Copy from realtek code
#define GetAddr1Ptr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 4))
#define GetAddr2Ptr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 10))
#define GetAddr3Ptr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 16))
#define GetAddr4Ptr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 24))

#define GetFrameSubType(pbuf)	(cpu_to_le16(*(unsigned short *)(pbuf)) & (BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2)))

#define SetFrameSubType(pbuf,type) \
	do {    \
		*(unsigned short *)(pbuf) &= cpu_to_le16(~(BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2))); \
		*(unsigned short *)(pbuf) |= cpu_to_le16(type); \
	} while(0)
	
#define SetToDs(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_TO_DS_); \
	} while(0)

#define SetFrDs(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_FROM_DS_); \
	} while(0)
	
/*
// Pointer to packet (Skuo MAC header)
#define GetMeshDataPtr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 36))
#define GetMeshMgtPtr(pbuf)	((unsigned char *)((unsigned int)(pbuf) + 34))

#define _TO_DS_			BIT(8)	///< To DS
#define _FROM_DS_		BIT(9)	///< Frome DS
#define _MORE_FRAG_		BIT(10)	///< More Frag
#define _RETRY_			BIT(11)	///< Retry
#define _PWRMGT_		BIT(12)	///< Power Management
#define _MORE_DATA_		BIT(13)	///< More Data
#define _PRIVACY_		BIT(14)	///< WEP


#define GetToDs(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_TO_DS_)) != 0)

#define ClearToDs(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_TO_DS_)); \
	} while(0)

#define GetFrDs(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_FROM_DS_)) != 0)

#define ClearFrDs(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_FROM_DS_)); \
	} while(0)

#define SetMFrag(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_MORE_FRAG_); \
	} while(0)

#define GetMFrag(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_MORE_FRAG_)) != 0)

#define ClearMFrag(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_MORE_FRAG_)); \
	} while(0)

#define SetRetry(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_RETRY_); \
	} while(0)

#define GetRetry(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_RETRY_)) != 0)

#define ClearRetry(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_RETRY_)); \
	} while(0)

#define SetPwrMgt(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_PWRMGT_); \
	} while(0)

#define GetPwrMgt(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_PWRMGT_)) != 0)

#define ClearPwrMgt(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_PWRMGT_)); \
	} while(0)

#define SetMData(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_MORE_DATA_); \
	} while(0)

#define GetMData(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_MORE_DATA_)) != 0)

#define ClearMData(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_MORE_DATA_)); \
	} while(0)

#define SetPrivacy(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) |= cpu_to_le16(_PRIVACY_); \
	} while(0)

#define GetPrivacy(pbuf)	(((*(unsigned short *)(pbuf)) & le16_to_cpu(_PRIVACY_)) != 0)

#define ClearPrivacy(pbuf)	\
	do	{	\
		*(unsigned short *)(pbuf) &= (~cpu_to_le16(_PRIVACY_)); \
	} while(0)

#define GetFrameType(pbuf)	(le16_to_cpu(*(unsigned short *)(pbuf)) & (BIT(3) | BIT(2)))

#define SetFrameType(pbuf,type)	\
	do { 	\
		*(unsigned short *)(pbuf) &= __constant_cpu_to_le16(~(BIT(3) | BIT(2))); \
		*(unsigned short *)(pbuf) |= __constant_cpu_to_le16(type); \
	} while(0)


#define GetSequence(pbuf)	(cpu_to_le16(*(unsigned short *)((unsigned int)(pbuf) + 22)) >> 4)

#define GetFragNum(pbuf)	(cpu_to_le16(*(unsigned short *)((unsigned int)(pbuf) + 22)) & 0x0f)

#define GetTupleCache(pbuf)	(cpu_to_le16(*(unsigned short *)((unsigned int)(pbuf) + 22)))

#define SetFragNum(pbuf, num) \
	do {    \
		*(unsigned short *)((unsigned int)(pbuf) + 22) = \
			((*(unsigned short *)((unsigned int)(pbuf) + 22)) & le16_to_cpu(~(0x000f))) | \
			cpu_to_le16(0x0f & (num));     \
	} while(0)

#define SetSeqNum(pbuf, num) \
	do {    \
		*(unsigned short *)((unsigned int)(pbuf) + 22) = \
			((*(unsigned short *)((unsigned int)(pbuf) + 22)) & le16_to_cpu((unsigned short)~0xfff0)) | \
			le16_to_cpu((unsigned short)(0xfff0 & (num << 4))); \
	} while(0)

#define SetDuration(pbuf, dur) \
	do {    \
		*(unsigned short *)((unsigned int)(pbuf) + 2) |= cpu_to_le16(0xffff & (dur)); \
	} while(0)

#define GetAid(pbuf)	(cpu_to_le16(*(unsigned short *)((unsigned int)(pbuf) + 2)) & 0x3fff)	//

#define GetTid(pbuf)	(cpu_to_le16(*(unsigned short *)((unsigned int)(pbuf) + (((GetToDs(pbuf)<<1)|GetFrDs(pbuf))==3?30:24))) & 0x000f)
	
*/

enum WIFI_FRAME_TYPE {
	WIFI_MGT_TYPE  =	(0),		///< 00 is Management Frame
	WIFI_CTRL_TYPE =	(BIT(2)),	///< 01 is control Frame
	WIFI_DATA_TYPE =	(BIT(3)),	///< 10 is Data Frame

// notice... _DOT11_MESH_MODE_ is undefined.
#ifdef _DOT11_MESH_MODE_
	// Hardware of 8186 doesn't support it. Confirm by David, 2007/1/5
	WIFI_EXT_TYPE  =	(BIT(2) | BIT(3))	///< 11 is 802.11S Extended Type
#endif
};

enum FRAME_SUBTYPE {
	WIFI_11S_MESH	     = (BIT(7) | WIFI_DATA_TYPE),	///< Mesh Data
    WIFI_11S_MESH_CFACK  = (BIT(4) | WIFI_11S_MESH),	///< Mesh Data + CF-ACK
    WIFI_11S_MESH_ACTION = (BIT(5) | WIFI_11S_MESH),	///< Mesh Action
};

enum
{
	HOSTAPD_DEBUG_NO = 0,
	HOSTAPD_DEBUG_MINIMAL = 1,
	HOSTAPD_DEBUG_VERBOSE = 2,
	HOSTAPD_DEBUG_MSGDUMPS = 3
} ;
/*
struct itimerval { 
struct timeval it_interval; 
struct timeval it_value; 
} ;
*/

// context
struct pathsel_context {
	int				driver_ver;
	u16				pathsel_identifier;	/* next IAPP identifier */

	//int			readfifo;
    u8*				readfifo;
	
	u8				RecvBuf[MAX_MSG_SIZE];

	char			wlan_iface[2][16];//hunter: wlan_iface[10][16];
	int				wlan_sock[10];
	int				is_daemon;

	int				debug;				/* debug verbosity level */
};

// 802.11s Path selection table
#pragma pack(1)
struct path_sel_entry {
#ifdef _RTL865X_
	unsigned char destMAC[MACADDRLEN];
	unsigned char nexthopMAC[MACADDRLEN];
	unsigned int  dsn;
	unsigned int  metric;
	unsigned char hopcount;
#else
	unsigned char isvalid;
	unsigned char destMAC[MACADDRLEN];
	unsigned int  dsn;
	unsigned char nexthopMAC[MACADDRLEN];
	unsigned int  metric;
	unsigned char hopcount;
	unsigned char modify_time[8];
#ifndef _RTL8187B_
	unsigned int  start, end;
#endif
#ifdef _RTL8187B_
	struct timespec update_time;    // for rreq retry 
	
	#ifdef MESH_ROUTE_MAINTENANCE
	struct timespec routeMaintain;  // timeout of maintenance rreq
	#endif
#else
	struct timeval update_time;             // for rreq retry 

    #ifdef MESH_ROUTE_MAINTENANCE
	struct timeval routeMaintain;   // timeout of maintenance rreq
	#endif
#endif
#endif // _not 865x_
};
#pragma pack()

#pragma pack(1)
struct relay_node {
	unsigned char DO:1;
	unsigned char RF:1;
	unsigned char Reserved:6;
	unsigned char DesAddress[MACADDRLEN];
	unsigned int  DesSeqNum;
//	struct relay_node* next;
//	struct relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rreq {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned char HopCount;
	unsigned char ttl;
	unsigned int  RreqId;
	unsigned char SourceAddress[MACADDRLEN];
	unsigned int  SourceSeqNum;
	unsigned int  Lifetime;
	unsigned int  Metric;
	struct relay_node first;
//	struct rrep_relay_node* next;
//	struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rrep_relay_node {
	unsigned char SourceAddress[MACADDRLEN];
	unsigned int  SourceSeqNum;
	unsigned char DepCount;
	unsigned char DepMacAddress[MACADDRLEN];
	unsigned int  DepDesSeqNum;
	//struct rrep_relay_node* next;
	//struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rrep {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned char HopCount;
	unsigned char ttl;
	unsigned char DesAddress[MACADDRLEN];
	unsigned int  DesSeqNum;
	unsigned int  LifeTime;
	unsigned int  Metric;
	struct rrep_relay_node first;
	//struct rrep_relay_node* next;
	//struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rerr {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned int  NumOfDes;     // 4 bytes format
	unsigned char DesAddress[MACADDRLEN];
	unsigned int  DesSeqNum;
};
#pragma pack()

#pragma pack(1)
struct rrep_ack {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char DesAddress[MACADDRLEN];
	unsigned int  DesSeqNum;
	unsigned char SourceAddress[MACADDRLEN];
	unsigned int  SourceSeqNum;
};
#pragma pack()


// debug printing
#define HOSTAPD_DEBUG(level, args...) \
do { \
	if (hapd->debug >= (level)) \
		printf(args); \
} while (0)

// not use #define HWMP_NET_DIAMETER 20 // rreq TTL, chuangch 0928

#ifdef _RTL865X_
//#define REMOVE_PATH_ENTRY_NEIGHBOR_DISPEAR  0x8BA3	// remove invalide path entry form path selection table
#define REMOVE_PATH_ENTRY 0x8BAC					// remove specified path entry
#define UPDATE_MESH_METRIC 0x8BAE					//update mesh metric
#define GET_MESH_METRIC_MIBVALUES 0x8BAF			//get mesh metric mib values
//#define DUMP_NBRTABLE  0x8BA4						// dump neighbor table
#define GET_STA_LIST 0x8BA6
//#define UPDATE_PROXY_TABLE 0x8BA7
#else
//#define REMOVE_PATH_ENTRY_NEIGHBOR_DISPEAR  0x8B83	// remove invalide path entry form path selection table
#define REMOVE_PATH_ENTRY 0x8B8C					// remove specified path entry
//#define DUMP_NBRTABLE  0x8B84						// dump neighbor table
#define GET_STA_LIST 0x8B86
//#define UPDATE_PROXY_TABLE 0x8B87
#endif //_RTL865X_

#define WLAN_HDR_A4_MESH_MGT_LEN 34		// mesh mgt frame header length
#define WLAN_HDR_A6_MESH_MGT_LEN 46		// mesh mgt frame header length
//#define PATH_SEL_TBL_SIZE 128			// path selection table size, defined in wireless_ag/8185s_pathsel.h

#ifndef _RTL865X_
#define NUM_STAT 64						// neighbor table size, defined in wireless_ag/rtk8185ag_cfg.h
#endif

/*
// neighbor table structure, defined in wireless_ag/mesh.h
enum dot11_MP_NEIGHBOR_STATE {
    MP_UNUSED = 0, // a state to indicate whether an entry is invalid or not

    // the following state is defiend by the table of "State Values" in page 62, D0.02
    MP_NEIGHBOR,
    MP_CANDIDATE_PEER,
    MP_ASSOCIATION_PENDING,
    MP_SUBORDINATE_LINK_DOWN,
    MP_SUBORDINATE_LINK_UP,
    MP_SUPERORDINATE_LINK_DOWN,
    MP_SUPERORDINATE_LINK_UP
};

#pragma pack(1)
struct MESH_Neighbor_Entry {

    // when state = MP_UNUSED, the entry is invalid
    enum dot11_MP_NEIGHBOR_STATE    State;	// type of dot11_MP_NEIGHBOR_STATE

    // The following entries represents the "MP Meighbor Table Entry" in page 61, D0.02
    // UINT8    NeighborMACADDR[MACADDRLEN];	// in fact, this info can be obtained by this.pstate->hwaddr
    // UINT8    PrimaryMACADDR[MACADDRLEN];	// (popen) No need,Because interface have  priv 
    u_int32_t          Directionality;		// peer link
    u_int8_t           Co;					// operating channel
    u_int32_t          Pl;					// CPI
    u_int16_t          r;					// byte rate (PS:Undefine use byte number !!)
    u_int16_t          ept;					// error rate
    u_int16_t          Q;					// strength or quality (PS:Undefine use byte number !!)
    unsigned long      expire;				// alive time

    // The following entries are internal data structure

    // a counter
    //   Set it to zero, when State changed from MP_UNUSED to MP_NEIGHBOR or MP_CANDIDATE_PEER
    //   Reset it to zero, after association success.
    //   Path selection or data transmission can reuse it for their propose
    u_int8_t           retry;
};
*/
#pragma pack()

#pragma pack(1)
struct pann {
    unsigned char category;
    unsigned char action;
    unsigned char eid;
    unsigned char length;
    unsigned char flag;
    unsigned char hopCount;
    unsigned char ttl;
    unsigned char origAddr[MACADDRLEN];
    unsigned int  seqNum;
    unsigned int  metric;
};
#pragma pack()

typedef struct _pann_retx_instance_{
    struct pann  pann;
    unsigned int timeout;
    short int    flag;
} pann_retx_t;

//pann_retx_t pann_retx_pool[MAX_MPP_NUM];

typedef struct _pann_mpp_entry_{
    unsigned char mac[MACADDRLEN];
    unsigned int timeout;
    unsigned int seqNum;
    short int    flag;
    int          beRoot;
} pann_mpp_t;


//pann_mpp_t pann_mpp_pool[MAX_MPP_NUM];


extern int update_root_info(unsigned char root_addr[MACADDRLEN]);
extern void tbl_pann();
extern int gen_rrep(unsigned char* DesAdd,unsigned int DesSeq,unsigned char* Source,unsigned int SourceSeq,unsigned int NewMetric,unsigned char hopcount,unsigned char TTL,unsigned short SeqNum, unsigned char Is6AddrFormat,unsigned char* Addr5,unsigned char* Addr6);


// defined in Path_OnDemand.c
extern int g_rawsocket_fd; //  raw socket descriptor 
extern int g_ioctrl_socket_fd; // ioctl descriptor
extern unsigned int My_SeqNum;
extern int My_RreqId;

// defined in Tree_Proactive.c
extern int isPortal;
extern int toBeRoot;
extern int isRoot;


// defined in pathselection.c
extern char interface_ioctl[16];
extern char interface_rawsocket[16];
extern unsigned char my_address[MACADDRLEN];

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
#define IS_5G_BAND 2

extern char* wlan0_ifName;
extern char* wlan1_ifName;
extern unsigned char wlan0_hwAddress[MACADDRLEN];
extern unsigned char wlan1_hwAddress[MACADDRLEN];
extern unsigned char wlan0_crossbandEnable;
extern unsigned char wlan1_crossbandEnable;
//configurable mib parameters
#pragma pack(1)
struct mibValue {
    unsigned char rssi_threshold;
    unsigned char cu_threshold;
    unsigned char noise_threshold;
	unsigned char rssi_weight;
	unsigned char cu_weight;
	unsigned char noise_weight;
};
#pragma pack()

extern unsigned char wlan0_is5G;
extern unsigned char wlan1_is5G;

extern struct mibValue mibValueOf2G;
extern struct mibValue mibValueOf5G;
extern struct mibValue* mibValuePointer;

#endif

extern int  pathSwitchThreshold ;

#endif /* PATHSELECTION_H */
