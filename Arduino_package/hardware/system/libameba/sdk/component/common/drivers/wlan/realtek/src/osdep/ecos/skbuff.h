 /******************************************************************************
  *
  * Name: skbuff.h - header file Linux skb wrapper
  *       $Revision: 1.1.1.1 $
  *
  *****************************************************************************/

#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include <ecos/wrapper_cdl.h>
#include "sys_support.h"

#define MAX_LOCAL_SKB_NUM		1024

#ifdef TX_SCATTER		
#define MAX_LIST_NUM			1
#endif

struct net_device_stats {
	unsigned long   rx_packets;             /* total packets received       */
	unsigned long   tx_packets;             /* total packets transmitted    */
	unsigned long   rx_bytes;               /* total bytes received         */
	unsigned long   tx_bytes;               /* total bytes transmitted      */
	unsigned long   rx_errors;              /* bad packets received         */
	unsigned long   tx_errors;              /* packet transmit problems     */
	unsigned long   rx_dropped;             /* no space in linux buffers    */
	unsigned long   tx_dropped;             /* no space available in linux  */
	unsigned long   multicast;              /* multicast packets received   */
	unsigned long   collisions;

	/* detailed rx_errors: */
	unsigned long   rx_length_errors;
	unsigned long   rx_over_errors;         /* receiver ring buff overflow  */
	unsigned long   rx_crc_errors;          /* recved pkt with crc error    */
	unsigned long   rx_frame_errors;        /* recv'd frame alignment error */
	unsigned long   rx_fifo_errors;         /* recv'r fifo overrun          */
	unsigned long   rx_missed_errors;       /* receiver missed packet       */

	/* detailed tx_errors */
	unsigned long   tx_aborted_errors;
	unsigned long   tx_carrier_errors;
	unsigned long   tx_fifo_errors;
	unsigned long   tx_heartbeat_errors;
	unsigned long   tx_window_errors;

	/* for cslip etc */
	unsigned long   rx_compressed;
	unsigned long   tx_compressed;
};

#ifdef TX_SCATTER
struct buf_list {
 	unsigned int	len;
	unsigned char	*buf;
};
#endif

struct skb_shared_info {
	atomic_t dataref;
};

struct  sk_buff_head {
	struct list_head	*next, *prev;
	u32			qlen;
};

#ifndef RTLPKG_DEVS_ETH_RLTK_WLAN_RX_ZERO_COPY
#define skb_shinfo(SKB)	((struct skb_shared_info *)((SKB)->ref))
#endif

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;		/* Next buffer in list */
	struct sk_buff		*prev;		/* Previous buffer in list */
	struct sk_buff_head	*list;		/* List we are on */	
	unsigned char		*head;		/* Head of buffer */
	unsigned char		*data;		/* Data head pointer */
	unsigned char		*tail;		/* Tail pointer	*/
	unsigned char		*end;		/* End pointer */ 
	struct net_device 	*dev;		/* Device we arrived on/are leaving by */	
	unsigned long 		key;		/* Tx Key id of ecos driver */	
	unsigned int 		len;		/* Length of actual data */	        
	unsigned char 		cloned;		/* head may be cloned (check refcnt to be sure). */
#ifdef TX_SCATTER
	unsigned int 		total_len;	/* Total length */	        
	int			list_idx;	/* current used buf index */
	int			list_num;	/* total list buf num */
	struct buf_list	 	list_buf[MAX_LIST_NUM];
#endif
#ifndef RTLPKG_DEVS_ETH_RLTK_WLAN_RX_ZERO_COPY
	atomic_t		data_ref;
	unsigned char		*ref;
#endif
	char			cb[16];
};

struct net_device {
	char		name[16];
	void		*priv;		/* pointer to private data */
	void		*info;		/* pointer to ecos driver private */
	unsigned char	dev_addr[6];	/* set during bootup */
	void		*pci_dev;
	int (*init)(void);
	int (*open)(struct net_device *dev);
	int (*stop)(struct net_device *dev);
	int (*hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
	int (*can_xmit)(struct net_device *dev);
	int (*do_ioctl)(struct net_device *dev, struct ifreq *ifr, int cmd);
	int (*isr)(struct net_device *dev);
	void (*dsr)(struct net_device *dev);
	struct net_device_stats* (*get_stats)(struct net_device *dev);
	unsigned short	flags;		/* flag used by device	*/
	unsigned long	base_addr;	/* device I/O address	*/
	unsigned char	irq; 		/* device IRQ number    */
  	unsigned long	last_rx;	/* Time of last Rx	*/
};

/* Copy sk_buff struct */
static inline void copy_skb_header(struct sk_buff *new, const struct sk_buff *old)
{
	memcpy(new->cb, old->cb, sizeof(old->cb));
}

/**
 *	skb_put - add data to a buffer
 *	@skb: buffer to use 
 *	@len: amount of data to add
 *
 *	This function extends the used data area of the buffer. If this would
 *	exceed the total buffer size the kernel will panic. A pointer to the
 *	first byte of the extra data is returned.
 */
 
static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp=skb->tail;
	skb->tail+=len;
	skb->len+=len;
	if(skb->tail>skb->end) {
		ASSERT(0);		
	}

#ifdef TX_SCATTER
	if (skb->list_num && skb->total_len)
		skb->total_len += len;
#endif
	
	return tmp;
}

static inline unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len-=len;
	skb->data = (unsigned char *)(((unsigned int)skb->data) + len);

#ifdef TX_SCATTER
	if (skb->list_num && skb->total_len)
		skb->total_len -= len;
#endif
	
	return skb->data;
}

/**
 *	skb_pull - remove data from the start of a buffer
 *	@skb: buffer to use 
 *	@len: amount of data to remove
 *
 *	This function removes data from the start of a buffer, returning
 *	the memory to the headroom. A pointer to the next data in the buffer
 *	is returned. Once the data has been pulled future pushes will overwrite
 *	the old data.
 */
/*
static inline unsigned char * skb_pull(struct sk_buff *skb, unsigned int len)
{	
	if (len > skb->len)
		return NULL;
	return __skb_pull(skb,len);
}
*/
unsigned char * skb_pull(struct sk_buff *skb, unsigned int len);

/**
 *	skb_reserve - adjust headroom
 *	@skb: buffer to alter
 *	@len: bytes to move
 *
 *	Increase the headroom of an empty &sk_buff by reducing the tail
 *	room. This is only allowed for an empty buffer.
 */

static inline void skb_reserve(struct sk_buff *skb, unsigned int len)
{
	skb->data+=len;
	skb->tail+=len;

#ifdef TX_SCATTER
	if (skb->list_num && skb->total_len)
		skb->total_len += len;
#endif	
}

static inline void skb_queue_head_init(struct sk_buff_head *list)
{
        list->prev = (struct list_head *)list;
        list->next = (struct list_head *)list;
        list->qlen = 0;
}

/**
 *      __skb_queue_tail - queue a buffer at the list tail
 *      @list: list to use
 *      @newsk: buffer to queue
 *
 *      Queue a buffer at the end of a list. This function takes no locks
 *      and you must therefore hold required locks before calling it.
 *
 *      A buffer cannot be placed on two lists at the same time.
 */     
 

static inline void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
        struct sk_buff *prev, *next;

        newsk->list = list;
        list->qlen++;
        next = (struct sk_buff *)list;
        prev = next->prev;
        newsk->next = next;
        newsk->prev = prev;
        next->prev = newsk;
        prev->next = newsk;
}

/**
 *      skb_queue_tail - queue a buffer at the list tail
 *      @list: list to use
 *      @newsk: buffer to queue
 *
 *      Queue a buffer at the tail of the list. This function takes the
 *      list lock and can be used safely with other locking &sk_buff functions
 *      safely.
 *
 *      A buffer cannot be placed on two lists at the same time.
 */     

static inline void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
        unsigned long flags;

        save_and_cli(flags);
        __skb_queue_tail(list, newsk);
        restore_flags(flags);
}

static inline void skb_assign_buf(struct sk_buff *skb, unsigned char *buf, unsigned int len)
{	
	skb->head = buf;
	skb->data = buf;
	skb->tail = buf;
	skb->end = buf + len;
}

static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb->tail = skb->data + offset;
}

static inline unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->end;
}

/*
 *	External functions
 */

extern void kfree_skb(struct sk_buff *skb);
#define dev_kfree_skb_any(skb)	kfree_skb_chk_key(skb, skb->dev)
extern void init_skb_pool(void);

#endif /* __SKBUFF_H__ */
