
#ifndef _MESH_UTIL_H_
#define _MESH_UTIL_H_

#include "mesh.h"

// for counting association number
#define INCREASE		1
#define DECREASE		0

#define BE2LE16(x)		(((x) & 0xff) << 8) | ((x) >> 8)

#define GET_MY_HWADDR(padapter)		((padapter)->eeprompriv.mac_addr)
#define is_mesh_6addr_format_without_qos(pframe)	((*((unsigned char*)(pframe)+30) & 0x01))	///< AE field(mesh header) bit0 = 1

#if 0
#define isMeshPoint(p)		(p&&(p->mesh_neighbor_TBL.State==MP_SUPERORDINATE_LINK_UP||p->mesh_neighbor_TBL.State==MP_SUBORDINATE_LINK_UP))
#else //jasmine: to follow 11s state machine
#define isMeshPoint(p)		(p&&(p->mesh_neighbor_TBL.State==MP_ESTAB))

#endif
// Separate 3 define, Because decision MeshPoint/STA condition may different or NEW device (isXXX) in the future.
#define isPossibleNeighbor(p)	(MP_UNUSED != p->mesh_neighbor_TBL.State)
#define isSTA(p)  			(MP_UNUSED == p->mesh_neighbor_TBL.State)
#define isSTA2(p)  			(MP_UNUSED == p.mesh_neighbor_TBL.State)

#define CTRL_LEN_CHECK(__x__,__y__) \
	do { \
		if((__x__ < __y__) || (__y__ < 0)) { \
			printk("!!! error [%s][%d] len=%d \n",__FUNCTION__, __LINE__, __y__); \
		} \
	} while(0)


#define SMP_LOCK_MESH_PATH(__x__) spin_lock_bh(&padapter->mlmeextpriv.mesh_path_lock)
#define SMP_UNLOCK_MESH_PATH(__x__) spin_unlock_bh(&padapter->mlmeextpriv.mesh_path_lock)
#define SMP_LOCK_MESH_QUEUE(__x__) spin_lock_bh(&padapter->mlmeextpriv.mesh_queue_lock)
#define SMP_UNLOCK_MESH_QUEUE(__x__) spin_unlock_bh(&padapter->mlmeextpriv.mesh_queue_lock)
#define SMP_LOCK_MESH_PROXY(__x__) spin_lock_bh(&padapter->mlmeextpriv.mesh_proxy_lock)
#define SMP_UNLOCK_MESH_PROXY(__x__) spin_unlock_bh(&padapter->mlmeextpriv.mesh_proxy_lock)
#define SMP_LOCK_MESH_PROXYUPDATE(__x__) spin_lock_bh(&padapter->mlmeextpriv.mesh_proxyupdate_lock)
#define SMP_UNLOCK_MESH_PROXYUPDATE(__x__) spin_unlock_bh(&padapter->mlmeextpriv.mesh_proxyupdate_lock)
#define SMP_LOCK_MESH_PREQ(__x__) spin_lock_bh(&padapter->mlmeextpriv.mesh_preq_lock)
#define SMP_UNLOCK_MESH_PREQ(__x__) spin_unlock_bh(&padapter->mlmeextpriv.mesh_preq_lock)
//#define SMP_LOCK_MESH_ACL(__x__)
//#define SMP_UNLOCK_MESH_ACL(__x__)
//#define SMP_LOCK_MESH_MP_HDR(__X__)
//#define SMP_UNLOCK_MESH_MP_HDR(__X__)


/*
 *	@brief	MESH  PeerLink_CAP number routine
 */
#define MESH_PEER_LINK_CAP_NUM(padapter)	(padapter->mlmeextpriv.mesh_PeerCAP_cap)

#ifdef PU_STANDARD
typedef struct {
	UINT8 flag;
	UINT8 PUseq;
	UINT8 proxyaddr[MACADDRLEN];
	UINT16 addrNum;
	struct list_head addrs;
} ProxyUpdate;
#endif

#define SET_PSEUDO_RANDOM_NUMBER(target)	{ \
	rtw_get_random_bytes(&(target), sizeof(target)); \
	target += (padapter->eeprompriv.mac_addr[4] + padapter->eeprompriv.mac_addr[5] + rtw_get_current_time() - padapter->recvpriv.rx_bytes \
	+ padapter->xmitpriv.tx_bytes + padapter->recvpriv.rx_drop); \
}

u32 mesh_cnt_assoc_STAs (_adapter *padapter);

u8 mesh_accept_additional_peering(_adapter *padapter);

const u8* mesh_state_string(enum dot11_MP_NEIGHBOR_STATE s);

const u8* mesh_event_string(enum MESH_PEER_LINK_EVENT e);

int remove_proxy_owner(_adapter *padapter, unsigned char *owner);

int rtw_mesh_close(_adapter *padapter);

/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

#endif	// _MESH_UTIL_H_
