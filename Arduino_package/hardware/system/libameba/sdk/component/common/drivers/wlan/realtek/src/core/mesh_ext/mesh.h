/*!	\file	mesh.h
	\brief	None

	\date 2006/12/22 
	\author	stanley, popen and owen
*/
#ifndef _RTW_MESH_PUB_HDR_H_
#define _RTW_MESH_PUB_HDR_H_

#include "mesh_cfg.h"
#include "osdep_service.h"
#include "wlan_basic_types.h"
#include "freertos/freertos_skbuff.h"
//#define RTW_MESH_ID "iot_mesh_point"
//#include "mesh_sae.h"

/**
 *	@brief	define
 *
 */

#define _MESH_DEBUG_ 
#undef _MESH_DEBUG_

//#define _MESH_PROC_DEBUG_ 
#undef _MESH_PROC_DEBUG_

//#define MESH_LOG
#undef MESH_LOG

#define MESH_DEBUG
#undef MESH_DEBUG

#define TESTME
#define FIXIT
#define TODO

#define PRINT_EXPIRE_TIMER_EN		0
#define PRINT_HWMP_TRACE_EN       	0
#define PRINT_CALL_TRACE_EN       	0
#define PRINT_MAC_EN				0
#define PRINT_LOG_MSG_EN			0
#define PRINT_MEM_DUMP_EN			0
#define PRINT_SAE_EN				0
#define PRINT_AMPE_EN				0
#define NOP                         ((void)0)

#if (PRINT_AMPE_EN == 1)
#define AMPE_MSG(msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define AMPE_MSG(msg, ...) NOP
#endif

#if (PRINT_SAE_EN == 1)
#define SAE_MSG(msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define SAE_MSG(msg, ...) NOP
#endif
#if (PRINT_SAE_EN == 1)
#define sae_debug(level,msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define sae_debug(level,msg, ...) NOP
#endif

#if (PRINT_EXPIRE_TIMER_EN == 1)
#define TIMER_MSG(msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define TIMER_MSG(msg, ...) NOP
#endif

#if (PRINT_HWMP_TRACE_EN == 1)
#define HWMP_MSG(msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define HWMP_MSG(msg, ...) NOP
#endif


#if (PRINT_CALL_TRACE_EN == 1)
#define call(function_name) do {printf("[%.5d %s] -> %s\n",__LINE__,__func__,function_name);} while(0)
#else
#define call(function_name) NOP
#endif


#if (PRINT_MAC_EN == 1)
#define print_mac(msg,addr)	do {printf("[%.5d %s]: %s: %02x:%02x:%02x:%02x:%02x:%02x\n", __LINE__, __func__, msg, 0xff& (addr)[0], 0xff& (addr)[1], 0xff& (addr)[2], 0xff& (addr)[3], 0xff& (addr)[4], 0xff& (addr)[5]);} while(0)
#else
#define print_mac(msg,addr) NOP
#endif

#if (PRINT_LOG_MSG_EN == 1)
#define log(msg, ...) do {printf("[%.5d %s]: " msg "\n",__LINE__, __func__,##__VA_ARGS__);} while(0)
#else
#define log(msg, ...) NOP
#endif

extern void __rtl_memDump_v1_00(const unsigned char *start, unsigned int size, char * strHeader);
#if (PRINT_MEM_DUMP_EN == 1)
#define dump(msg,mem,len,...) do {printf("[%.5d %s]: " msg,__LINE__, __func__,##__VA_ARGS__); __rtl_memDump_v1_00((void *)(mem),(len),NULL);} while(0)
#else
#define dump(msg,mem,len,...) NOP
#endif

//#define mesh_proxy_debug panic_printk
#if defined(MESH_DEBUG)
#define mesh_tx_debug log
#define mesh_txsc_debug log
#define mesh_sme_debug log
#define mesh_proxy_debug log
#define mesh_route_debug log
#elif defined(MESH_LOG)
#define mesh_tx_debug scrlog_printk
#define mesh_txsc_debug scrlog_printk
#define mesh_sme_debug scrlog_printk
#define mesh_proxy_debug scrlog_printk
#define mesh_route_debug scrlog_printk
#else
#define mesh_tx_debug(msg, args...) 
#define mesh_txsc_debug(msg, args...) 
#define mesh_sme_debug(msg, args...)  
#define mesh_proxy_debug(msg, args...) 
#define mesh_route_debug(msg, args...)
#endif

#define ONLY_ROOT_DO_AODV 0
#define RELAY_11S 8
#define XMIT_11S 4


#ifdef  _MESH_DEBUG_
#define MESH_DEBUG_MSG(msg, args...)		log(msg,##args)//printk(KERN_ERR msg,  ## args)
//#define MESH_BOOTSEQ_STRESS_TEST

#else
#define MESH_DEBUG_MSG(msg, args...)
#undef MESH_BOOTSEQ_STRESS_TEST
#endif

#define MESH_NEIGHTBOR_NUM AP_STA_NUM //max neightbor number

#define MESH_GATE_ANNOUNCEMENT_PROTOCOL 0
#define MESH_FORWARDING 1


/**
 *	@brief	Time to  jiffies 
 *
 *	@param	time: (unit=1ms)
 *
 *	@retval	jiffies
 */

/* timeouts in ms */
#define MESH_TTL 		31
#define MESH_DEFAULT_ELEMENT_TTL 31
#define MESH_MAX_RETR	 	3
#define MESH_PATH_TO_ROOT_TIMEOUT      6000
#define MESH_ROOT_CONFIRMATION_INTERVAL 2000
#define MESH_ROOT_INTERVAL_MS     5000
#define MESH_RANN_INTERVAL_MS     5000

/*
 * Minimum interval between two consecutive PREQs originated by the same
 * interface
 */
#define MESH_PREQ_MIN_INT	10
#define MESH_PERR_MIN_INT	100
#define MESH_DIAM_TRAVERSAL_TIME 50


#define jiffies						rtw_get_current_time()
#define net_traversal_jiffies()		rtw_ms_to_systime(50)
#define root_path_confirmation_jiffies() rtw_ms_to_systime(MESH_ROOT_CONFIRMATION_INTERVAL)

// Define of time out jiffies
#define MESH_EXPIRE_TO				rtw_ms_to_systime(8000)	// MP MAX  idle time 4000
#define MESH_EXPIRE_TO_STAGE2		rtw_ms_to_systime(8000)	// MP Prob req wai time 4000
#define MESH_TIMER_TO				rtw_ms_to_systime(250)	// MP mesh_unEstablish_hdr  peer link expire  timer.
#define MESH_LocalLinkStateANNOU_TO	rtw_ms_to_systime(5000)	// MP Local Link State Announcement time.
#ifdef CONFIG_SAE_SUPPORT
#define MESH_AUTH_SAE_TO			rtw_ms_to_systime(3000)
#endif
// Define of time out Stress test (Association expire time)
#ifdef	MESH_BOOTSEQ_STRESS_TEST
#define MESH_BS_STRESS_TEST_MIN_TIME	rtw_ms_to_systime(1000)
#define MESH_BS_STRESS_TEST_MAX_TIME	rtw_ms_to_systime(5000)
#endif	// MESH_BOOTSEQ_STRESS_TEST

#ifdef MESH_BOOTSEQ_AUTH
// Auth
#define MESH_AUTH_RETRY_TO			rtw_ms_to_systime(500)	// AUTH resend Authentication request time
#define MESH_AUTH_REQUEST_TO		rtw_ms_to_systime(1000)	// AUTH   after recived RSP , Betweend RSP and REQ time
#define MESH_AUTH_LISTEN_TO			rtw_ms_to_systime(5000)	// MP connection time
#endif

// peer link
#define MESH_PEER_LINK_RETRY_TO		rtw_ms_to_systime(1000)//rtw_ms_to_systime(500)	// start peer link resend Association request time
#define MESH_PEER_LINK_CONFIRM_TO		rtw_ms_to_systime(1000)	// peer link after recived  confirm, Between confirm and open time
#define MESH_PEER_LINK_CLOSE_TO		rtw_ms_to_systime(1000)	// peer link close wait time
#define MESH_PEER_LINK_LISTEN_TO	rtw_ms_to_systime(5000)	// MP connection time


//#define TU_TO_JIFFIES(x)	(usecs_to_jiffies((x) * 1024)) reserved
#define TU_TO_JIFFIES(x)	(x)
#define TU_TO_EXP_TIME(x)	(jiffies + TU_TO_JIFFIES(x))



// Retry 
#define MESH_AUTH_RETRY_LIMIT		6		// Retry AUTH count
#define MESH_PEER_LINK_RETRY_LIMIT	6		// Retry PeerLink count


//mesh channel switch counter
#define MESH_CHANNEL_SWITCH_COUNTER  5


//mesh dfs channel switch counter
#define MESH_DFS_SWITCH_COUNTDOWN  30



/*
 *	Length setting
*/

#define MESH_ID_LEN 				32		// Mesh ID
#define MESH_PS_IDENT_OUI_LEN 		0		// Mesh Path Select Identifier OUI
#define MESH_PS_IDENT_VALUE_LEN 	0		// Mesh Path Select Identifier VALUE
#define MESH_LINK_ID_LEN 			2		// Mesh PeerLinkID & LocalLinkID
#define MESH_VERSION_LEN			0		// IE Version
#define MESH_IE_BASE_LEN			2		// IE  ID + LEN

// WLAN mesh capacity
#define	MESH_CAP_PROTOCOL_LEN		MESH_PS_IDENT_OUI_LEN + MESH_PS_IDENT_VALUE_LEN	// Active Protocol ID
#define	MESH_CAP_METRIC_LEN			MESH_PS_IDENT_OUI_LEN + MESH_PS_IDENT_VALUE_LEN	// Active Metric ID
#define	MESH_CAP_PEER_CAP_LEN		1		// Peer Capacity
#define	MESH_CAP_POWERSAVE_CAP_LEN	1		// Power Save Capacity
#define	MESH_CAP_SYNC_CAP_LEN		1		// Synchronization Capacity
#define	MESH_CAP_MDA_CAP_LEN		1		// MDA Capacity
#define	MESH_CAP_CH_PRECEDENCE_LEN	4		// Channel precedence

#define	MESH_CAP_VERSION_OFFSET		MESH_IE_BASE_LEN
#define	MESH_CAP_PROTOCOL_OFFSET	MESH_CAP_VERSION_OFFSET + MESH_VERSION_LEN
#define	MESH_CAP_METRIC_OFFSET		MESH_CAP_PROTOCOL_OFFSET + MESH_CAP_PROTOCOL_LEN
#define	MESH_CAP_PEER_CAP_OFFSET	MESH_CAP_METRIC_OFFSET + MESH_CAP_METRIC_LEN
#define	MESH_CAP_POWERSAVE_OFFSET	MESH_CAP_PEER_CAP_OFFSET + MESH_CAP_PEER_CAP_LEN
#define	MESH_CAP_SYNC_CAP_OFFSET	MESH_CAP_POWERSAVE_OFFSET + MESH_CAP_POWERSAVE_CAP_LEN
#define	MESH_CAP_MDA_CAP_OFFSET		MESH_CAP_SYNC_CAP_OFFSET + MESH_CAP_SYNC_CAP_LEN
#define	MESH_CAP_CH_PRECEDENCE_OFFSET	MESH_CAP_MDA_CAP_OFFSET + MESH_CAP_MDA_CAP_LEN

/* IEEE 802.11s - Mesh Capability */	//add by jasmine
#define MESH_CAP_ACCEPT_ADDITIONAL_PEER	BIT(0)
#define MESH_CAP_MCCA_SUPPORTED			BIT(1)
#define MESH_CAP_MCCA_ENABLED			BIT(2)
#define MESH_CAP_FORWARDING				BIT(3)
#define MESH_CAP_MBCA_ENABLED			BIT(4)
#define MESH_CAP_TBTT_ADJUSTING			BIT(5)
#define MESH_CAP_MESH_PS_LEVEL			BIT(6)

#define NUM_MESH					1		// How many /dev/meshX devices

#define MESH_PEER_LINK_CAP_CAPACITY_MASK	(BIT(12)|BIT(11)|BIT(10)|BIT(9)|BIT(8)|BIT(7)|BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0))	// MASK of Peer capacity(capacity)
#define MESH_PEER_LINK_CAP_FLAGS_MASK		(BIT(7)|BIT(6)|BIT(5))	// MASK of Peer capacity (flags, Because 1byte)

#define PATHSEL_TABLE_SIZE          4  /* the real size is power of 2*/
#define MAX_MPP_NUM 				15  /* it should be ((1<<PATHSEL_TABLE_SIZE) - 1) */
#define DATA_SKB_BUFFER_SIZE 		7 		// acutal size is power of 2
#define PROXY_TABLE_SIZE 			8  		// acutal size is power of 2

#define _MESH_HEADER_TTL_  			MAX_MPP_NUM

// the following two parameters must be power of 2
#define SZ_HASH_IDX1 8
#define SZ_HASH_IDX2 512

#ifdef	MESH_ESTABLISH_RSSI_THRESHOLD
#define	DEFAULT_ESTABLISH_RSSI_THRESHOLD	25;	//RSSI range : 0~100 (Decimal)
#endif

#ifdef _MESH_ACL_ENABLE_
#define NUM_MESH_ACL NUM_ACL
#endif


#if 0
enum dot11_MP_NEIGHBOR_STATE {
	// Here have virtual "0" and MP_LISTEN Set "1" Because keep table integrity, Table using MP_UNUSED , CAUTION!!  :The value don't modify !!
	MP_UNUSED = 0, // a state to indicate whether an entry is invalid or not, CAUTION!!  :The value don't modify( About initial default and table) !!
	// the following state is defiend by the table of "State Values" in page 62, D0.02
	// hybrid page 116 D1.0 state transitions
	MP_LISTEN,
	MP_OPEN_SENT,
	MP_CONFIRM_RCVD,
	MP_CONFIRM_SENT,
	MP_SUBORDINATE_LINK_DOWN_E,
	MP_SUBORDINATE_LINK_UP,
	MP_SUPERORDINATE_LINK_DOWN,
	MP_SUPERORDINATE_LINK_UP,
	MP_HOLDING
};
#else//jasmine: to follow 11s state machine
enum dot11_MP_NEIGHBOR_STATE {
	// Here have virtual "0" and MP_LISTEN Set "1" Because keep table integrity, Table using MP_UNUSED , CAUTION!!  :The value don't modify !!
	MP_UNUSED = 0, // a state to indicate whether an entry is invalid or not, CAUTION!!  :The value don't modify( About initial default and table) !!
	// the following state is defiend by the table of "State Values" in page 62, D0.02
	// hybrid page 116 D1.0 state transitions
	MP_IDLE = 1,
	MP_OPN_SNT,
	MP_OPN_RCVD,
	MP_CNF_RCVD,
	MP_ESTAB,
	MP_HOLDING,
	MP_STATE_UNDEF,
};
#endif

#if 0
/*
  *	@brief	 Mesh EVENT
  *	With sme.c  PeerLink_states_table synchronic
 */
enum MESH_PEER_LINK_EVENT {
	CancelPeerLink = 0,
	PassivePeerLinkOpen,
	ActivePeerLinkOpen,
	CloseReceived,
	OpenReceived,
	ConfirmReceived,
	TimeOut
};
#else//jasmine: to follow 11s state machine
enum MESH_PEER_LINK_EVENT {
	CNCL = 0,
	OPN_ACPT,
	OPN_RJCT,
	CNF_ACPT,
	CNF_RJCT,
	CLS_ACPT,
	REQ_RJCT
};
#endif

/*
 *	@brief	Mesh Neighbor Table
 *
 */
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct MESH_Neighbor_Entry {
	// when state = MP_UNUSED, the entry is invalid	
	enum dot11_MP_NEIGHBOR_STATE	State; // type of dot11_MP_NEIGHBOR_STATE

	// The following entries represents the "MP Meighbor Table Entry" in page 61, D0.02
	// u8	NeighborMACADDR[MACADDRLEN]; 	// in fact, this info can be obtained by this.pstate->hwaddr
	// u8	PrimaryMACADDR[MACADDRLEN];		// (popen) No need,Because interface have  priv 	
	//unsigned long	LocalLinkID;		// peer link local link id (Identify connect by myself)
	u16	LocalLinkID;
	//unsigned long	PeerLinkID;			// peer link Peer link id (Identify connect by peer MP)  (PS:Some process allow NULL,  Check NULL before, If no, compare  match or not.)
	u16 PeerLinkID;
	u8			Co;					// operating channel
	u32			Pl;					// CPI
	u16			r;					// byte rate (PS:Undefine use byte number !!)
	u16			ept;				// error rate
	u16			Q;					// strength or quality (PS:Undefine use byte number !!)

	// expire time counter (upcount, use system jiffies)
	unsigned long		expire;					// Connect successful MP expire timer.
	unsigned long		BSexpire_LLSAperiod;	// 1.boot sequence process Peer link retry, open, cancel timer, 2.period send LLS timer.

	// The following entries are internal data structure
	// a counter
	//   Set it to zero, when State changed from MP_UNUSED to Start Peer Link process.
	//   Path selection or data transmission can reuse it for their propose
	//   Peer link open REQ_MAX
	u8			retry;			// retry counter
#ifdef RTK_MESH_METRIC_REFINE
	u32			rssi_metric;	// rssi metric to be passed to user space
	u32			cu_metric;
	u32			noise_metric;
#endif
	u32			metric;				// recorded metric
	u16 seqNum;     // record for  recently sent multicast packet
#if defined(RTK_MESH_MANUALMETRIC)
	u32			manual_metric;				// recorded metric
#endif
#ifdef MESH_USE_METRICOP
	u8                   retryMetric;
	u8                   isAsym; // if neighbor is non-Realtek device
	atomic_t                isMetricTesting; // if a testing is performing: 1/2: sender, 4: receiver
	u32                  timeMetricUpdate; // jiffies when metric should be updated
	struct {
		u32  toRx; // jiffies indicating a peer's test is expired
		u32  toTx; // jiffies indicating my test should be terminated
		u8   rateRx; // the data rate of the testing traffic been testing by peer
		u8   rateTx; // data rate used when issuing test traffic
		u8   prioRx, prioTx; // priority during test
		u16  lenRx0, lenRx1, cntRx0, cntRx1; // Rx0: from action frame; cntRx1: counted by rx, lenRx1: total received len
		u16  lenTx0, cntTx0, cntTx1; // Tx0: parameters for issue_xxx, cntTx1: # of pkt already sent
	} spec11kv;
#endif

	//Use  Local Link Announcement Packet error rate calculate(Not use temporary)
	// unsigned int		tx_pkts_pre;
	// unsigned int		tx_fail_pre;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

/**
 *	@brief	Path selection protocol ID
 *
 *	+-----+------------------------------------+	\n
 *	| OUI | Path selection protocol identifier |	\n
 *	+-----+------------------------------------+	\n
 *	(PACK)
 */
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct	PathSelectProtocolID_t {
	//u8	OUI[MESH_PS_IDENT_OUI_LEN];  // defined by owen: {0x00FAC}; ???
	u8	value;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

/**
 *	@brief	Path selection protocol ID
 *
 *	+-----+------------------------------------+	\n
 *	| OUI | Path selection protocol identifier |	\n
 *	+-----+------------------------------------+	\n
 *	(PACK)
 */
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct	PathSelectMetricID_t {
	//u8	OUI[MESH_PS_IDENT_OUI_LEN]; // defined by owen:  {000FAC};	???
	u8	value;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

/**
 *	@brief	MESH profile 
 */
struct MESH_Profile { // mesh_profile Configure by WEB in the future, Maybe delete, Preservation before delete
	u8				used;  								// unused: 0, used: 1
	// struct list_head		mesh_profile_ptr; 						// Not use now, (2006/12/22), Keep for expand in the future
	u8				priority;							// Mesh profile priority(maybe set from web interface)
	struct PathSelectProtocolID_t 	PathSelectProtocolID;	// Undefine(OUI)
	struct PathSelectMetricID_t	PathSelectMetricID;			// Undefine(OUI)
	u8 CongestionControlMode;//(jasmine)
	u8 SynchronizationMethod;//(jasmine)
	u8 AuthenticationProtocol;//(jasmine)
	void *config;//(jasmine)
};


struct MESH_Share {
	unsigned short	seq;
	unsigned short  seqMcast;
	u8			PUseq;
};



/**
 *	@brief	MESH fake MIB
 *
 *  		Under all one maybe place on MIB in the future.
 */
struct MESH_FAKE_MIB_T {
	u8			establish_rssi_threshold;				// Mesh check RSSI before establish sequence.
	// the following fields are for 11s-metric project
	//
#ifdef MESH_USE_METRICOP
	// note: The type of active metric is already defined in mesh profile. Here we only define
	//       the mechanism that is used to report metric
	// 0: (maybe) 11v+11k approach; 1:  Link Metric Reporting procedure;
	u8                   isPure11s;
	// note: indicate metric mode;
	// 0 : RSSI (original)
	// 1 : RSSI (before: 09/03/11)
	// 2 : 11s default(air time)
	u8                   metricID;

	// The Link metric protocol is invoked immediately during boot-sequence completeness
	// until current 'jiffies' exceeds priv->toMeshMetricAuto + this field
	u32                  intervalMetricAuto; // (jiffies)

	struct {
		// default values for test packet
		u32                  defPktTO; // (units of 100 TU/ms in spec and our implementation respectively)
		u16                  defPktLen; // (bytes) Bt = 1024 bytes
		u16                  defPktCnt; // (packet count)
		u8                   defPktPri; // (priority: 5-AC_VI by default (in tx.c).. ps: no aggregation test now )
	} spec11kv;
#endif
};


#ifdef	_MESH_DEBUG_
extern u8 mesh_proc_MAC[ETH_ALEN];
#endif	// _MESH_DEBUG_

#define NUM_TXPKT_QUEUE MAX_SKB_BUF_NUM

struct pkt_queue {
	struct sk_buff	*pSkb[NUM_TXPKT_QUEUE];
	int	head;
	int	tail;
};

enum mesh_path_flags {
	MESH_PATH_ACTIVE =	BIT(0),
	MESH_PATH_RESOLVING =	BIT(1),
	MESH_PATH_SN_VALID =	BIT(2),
	MESH_PATH_FIXED	=	BIT(3),
	MESH_PATH_RESOLVED =	BIT(4),
	MESH_PATH_REQ_QUEUED =	BIT(5)
};

enum mpath_frame_type {
	MPATH_PREQ = 0,
	MPATH_PREP,
	MPATH_PERR,
	MPATH_RANN
};


#define MAX_METRIC	0xffffffff

/* Destination only */
#define MP_F_DO	0x1
/* Reply and forward */
#define MP_F_RF	 0x2
/* Unknown Sequence Number */
#define MP_F_USN    0x01
/* Reason code Present */
#define MP_F_RCODE  0x02

#define PREQ_Q_F_START	0x1
#define PREQ_Q_F_REFRESH	0x2

/* HWMP IE processing macros */
/* HWMP IE processing macros */
#define HWMP_LEN(x)		(*(x + 3))
#define HWMP_SOURCE(x)	(x + 10)
#define HWMP_BODY(x)	(x + 24)
#define HWMP_TAG(x)		(*(x + 26))
#define HWMP_ELEMENT(x)	(x + 28)

#define get_unaligned_le32(x)	RTW_GET_LE32(x)
#define get_unaligned_le16(x)	RTW_GET_LE16(x)

static inline u32 u32_field_get(const u8 *elem, int offset, bool ae)
{
	if (ae)
		offset += 6;
	return get_unaligned_le32(elem + offset);
}

static inline u32 u16_field_get(const u8 *elem, int offset, bool ae)
{
	if (ae)
		offset += 6;
	return get_unaligned_le16(elem + offset);
}


#define AE_F			(1<<6)
#define AE_F_SET(x)		(*x & AE_F)
#define PREQ_IE_FLAGS(x)	(*(x))
#define PREQ_IE_HOPCOUNT(x)	(*(x + 1))
#define PREQ_IE_TTL(x)		(*(x + 2))
#define PREQ_IE_PREQ_ID(x)	u32_field_get(x, 3, 0)
#define PREQ_IE_ORIG_ADDR(x)	(x + 7)
#define PREQ_IE_ORIG_SN(x)	u32_field_get(x, 13, 0)
#define PREQ_IE_LIFETIME(x)	u32_field_get(x, 17, AE_F_SET(x))
#define PREQ_IE_METRIC(x) 	u32_field_get(x, 21, AE_F_SET(x))
#define PREQ_IE_TARGET_F(x)	(*(AE_F_SET(x) ? x + 32 : x + 26))
#define PREQ_IE_TARGET_ADDR(x) 	(AE_F_SET(x) ? x + 33 : x + 27)
#define PREQ_IE_TARGET_SN(x) 	u32_field_get(x, 33, AE_F_SET(x))

#define PREP_IE_FLAGS(x)	PREQ_IE_FLAGS(x)
#define PREP_IE_HOPCOUNT(x)	PREQ_IE_HOPCOUNT(x)
#define PREP_IE_TTL(x)		PREQ_IE_TTL(x)
#define PREP_IE_ORIG_ADDR(x)	(AE_F_SET(x) ? x + 27 : x + 21)
#define PREP_IE_ORIG_SN(x)	u32_field_get(x, 27, AE_F_SET(x))
#define PREP_IE_LIFETIME(x)	u32_field_get(x, 13, AE_F_SET(x))
#define PREP_IE_METRIC(x)	u32_field_get(x, 17, AE_F_SET(x))
#define PREP_IE_TARGET_ADDR(x)	(x + 3)
#define PREP_IE_TARGET_SN(x)	u32_field_get(x, 9, 0)

#define PERR_IE_TTL(x)		(*(x))
#define PERR_IE_TARGET_FLAGS(x)	(*(x + 2))
#define PERR_IE_TARGET_ADDR(x)	(x + 3)
#define PERR_IE_TARGET_SN(x)	u32_field_get(x, 9, 0)
#define PERR_IE_TARGET_RCODE(x)	u16_field_get(x, 13, 0)

//#define MSEC_TO_TU(x) (x*1000/1024)
#define SN_GT(x, y) ((s32)(y - x) < 0)
#define SN_LT(x, y) ((s32)(x - y) < 0)

/**
 * enum ieee80211_preq_flags - mesh PREQ element flags
 *
 * @IEEE80211_PREQ_PROACTIVE_PREP_FLAG: proactive PREP subfield
 */
enum ieee80211_preq_flags {
	IEEE80211_PREQ_PROACTIVE_PREP_FLAG	= 1<<2,
};

/**
 * enum ieee80211_preq_target_flags - mesh PREQ element per target flags
 *
 * @IEEE80211_PREQ_TO_FLAG: target only subfield
 * @IEEE80211_PREQ_USN_FLAG: unknown target HWMP sequence number subfield
 */
enum ieee80211_preq_target_flags {
	IEEE80211_PREQ_TO_FLAG	= 1<<0,
	IEEE80211_PREQ_USN_FLAG	= 1<<2,
};

enum ieee80211_rann_flags {
	RANN_FLAG_IS_GATE = 1 << 0,
};

enum ieee80211_root_mode_identifier {
	IEEE80211_ROOTMODE_NO_ROOT = 0,
	IEEE80211_ROOTMODE_ROOT = 1,//NO USE
	IEEE80211_PROACTIVE_PREQ_NO_PREP = 2,
	IEEE80211_PROACTIVE_PREQ_WITH_PREP = 3,
	IEEE80211_PROACTIVE_RANN = 4,
};

#define MESH_ROOT_MODE   0 //xuyan

#define MESH_RANN_INTERVAL		5
#define MESH_ROOT_INTERVAL		5


extern const u8 broadcast_mac_addr [6];


#define spin_lock_bh(x)		rtw_spin_lock(x)
#define spin_unlock_bh(x)	rtw_spin_unlock(x)

#define RTW_IEEE80211_MAX_MESH_PEERINGS	63
/**
 * enum mesh_config_capab_flags - Mesh Configuration IE capability field flags
 *
 * @IEEE80211_MESHCONF_CAPAB_ACCEPT_PLINKS: STA is willing to establish
 *	additional mesh peerings with other mesh STAs
 * @IEEE80211_MESHCONF_CAPAB_FORWARDING: the STA forwards MSDUs
 * @IEEE80211_MESHCONF_CAPAB_TBTT_ADJUSTING: TBTT adjustment procedure
 *	is ongoing
 * @IEEE80211_MESHCONF_CAPAB_POWER_SAVE_LEVEL: STA is in deep sleep mode or has
 *	neighbors in deep sleep mode
 */
enum mesh_config_capab_flags {
	RTW_IEEE80211_MESHCONF_CAPAB_ACCEPT_PLINKS		= 0x01,
	RTW_IEEE80211_MESHCONF_CAPAB_FORWARDING		= 0x08,
	RTW_IEEE80211_MESHCONF_CAPAB_TBTT_ADJUSTING		= 0x20,
	RTW_IEEE80211_MESHCONF_CAPAB_POWER_SAVE_LEVEL	= 0x40,
};

#endif // _RTW_MESH_PUB_HDR_H_
