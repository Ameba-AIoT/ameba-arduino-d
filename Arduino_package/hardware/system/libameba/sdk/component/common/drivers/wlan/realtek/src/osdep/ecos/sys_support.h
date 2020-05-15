 /******************************************************************************
  *
  * Name: sys-support.h - System type support for Linux
  *       $Revision: 1.1.1.1 $
  *
  *****************************************************************************/

#ifndef __SYS_SUPPORT_H__
#define __SYS_SUPPORT_H__

#include <ecos/wrapper_cdl.h>

#if defined(RTLPKG_DEVS_ETH_RLTK_WLAN_KMALLOC_USE_NET_MEM) && defined(CYGPKG_NET)
#include <sys/param.h>
#include <sys/malloc.h>
#endif

#include <cyg/hal/hal_intr.h>

//#define ISR_DIRECT	/* process event in ISR directly */
//#define TX_SCATTER
//#define TX_ISR_DIRECT
#define TX_PKT_FREE_QUEUE

#ifdef CYGSEM_HAL_IMEM_SUPPORT
#define __IMEM_SECTION__	__attribute__ ((section(".iram-gen")))
#else
#define __IMEM_SECTION__
#endif

#ifdef CYGSEM_HAL_DMEM_SUPPORT
#define __DMEM_SECTION__	__attribute__ ((section(".dram-gen")))
#else
#define __DMEM_SECTION__
#endif

#define	HZ 			100

#define	RLTK_PAGE_SHIFT		12
#define	RLTK_PAGE_SIZE		(1L << RLTK_PAGE_SHIFT)

#define SMP_CACHE_BYTES	16
#define SKB_DATA_ALIGN(X)	(((X) + (SMP_CACHE_BYTES-1)) & ~(SMP_CACHE_BYTES-1))

#define KERN_ERR
#define KERN_INFO
#define KERN_NOTICE
#define SET_MODULE_OWNER(some_struct) 	do { } while (0)
#define GFP_KERNEL		1
#define GFP_ATOMIC		1

#define __init
#define __exit
#define __devinit
#define __devexit

#define printk				diag_printf
#define spin_lock_init(lock)		do { } while (0)
#define spin_lock(x)			do { } while (0)
#define spin_unlock(x)			do { } while (0)
#define spin_lock_bh(x)			do { } while (0)
#define spin_unlock_bh(x)		do { } while (0)
#define restore_flags(x)		HAL_RESTORE_INTERRUPTS(x)
#define save_and_cli(x)			HAL_DISABLE_INTERRUPTS(x)
#define spin_lock_irqsave(lock, flags)	save_and_cli(flags)
#define spin_unlock_irqrestore(l, f)	restore_flags(f)

#define SET_NETDEV_DEV(dev, obj)	do { } while (0)
#define register_netdev(dev)		(0)
#define unregister_netdev(dev)		do { } while (0)
#define netif_start_queue(dev)		do { } while (0)
#define netif_queue_stopped(dev)	(0)
#define netif_wake_queue(dev)		do { } while (0)
#define netif_stop_queue(dev)		do { } while (0)

#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_KMALLOC_USE_NET_MEM
#ifdef CYGPKG_NET
#define kmalloc(size, flag)		cyg_net_malloc((u_long)size, M_DEVBUF, M_NOWAIT)
#define kfree(buf)			cyg_net_free((caddr_t)(buf), M_DEVBUF)
#elif defined(CYGPKG_NET_LWIP)
#define kmalloc(size, flag)		mem_malloc(size)
#define kfree(buf)			mem_free(buf)
#endif
#else
#define kmalloc(size, flag)		alloc_local(size)
static inline void kfree(void *buf)
{
	diag_printf("[%s] Should not go here!! free(%p)\n", __FUNCTION__, buf);
}
#endif

#define mdelay(t)					HAL_DELAY_US(1000*t)
#define udelay(t)					HAL_DELAY_US(t)
#define ASSERT(_bool_)					CYG_ASSERTC(_bool_)

#ifdef ISR_DIRECT
#define netif_rx(skb, dev)				wrapper_que_up(skb, dev)
#else
#define netif_rx(skb)					wrapper_up(skb)
#endif
#define panic_printk					diag_printf
#define sprintf						diag_sprintf

/* 
 *	Type definition
 */

typedef unsigned char u8;
typedef char s8; 
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef long unsigned int LUINT;
typedef struct { volatile int counter; } atomic_t;
typedef unsigned long long u64;
typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_add(struct list_head * new,
	struct list_head * prev,
	struct list_head * next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_del(struct list_head * prev,
				  struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static __inline__ void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static __inline__ void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline__ int list_empty(struct list_head *head)
{
	return head->next == head;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static __inline__ void list_splice(struct list_head *list, struct list_head *head)
{
	struct list_head *first = list->next;

	if (first != list) {
		struct list_head *last = list->prev;
		struct list_head *at = head->next;

		first->prev = head;
		head->next = first;

		last->next = at;
		at->prev = last;
	}
}

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_read(v)  ((v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_set(v,i) ((v)->counter = (i))


/*
 * The MIPS I implementation is only atomic with respect to
 * interrupts.  R3000 based multiprocessor machines are rare anyway ...
 *
 * atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v.  Note that the guaranteed useful range
 * of an atomic_t is only 24 bits.
 */
extern __inline__ void atomic_add(int i, atomic_t * v)
{
	int flags;

	save_and_cli(flags);
	v->counter += i;
	restore_flags(flags);
}

/*
 * atomic_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
extern __inline__ void atomic_sub(int i, atomic_t * v)
{
	int flags;

	save_and_cli(flags);
	v->counter -= i;
	restore_flags(flags);
}

extern __inline__ int atomic_add_return(int i, atomic_t * v)
{
	int temp, flags;

	save_and_cli(flags);
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags(flags);

	return temp;
}

extern __inline__ int atomic_sub_return(int i, atomic_t * v)
{
	int temp, flags;

	save_and_cli(flags);
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags(flags);

	return temp;
}

/*
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_inc(v) atomic_add(1,(v))

#define atomic_inc_return(v) atomic_add_return(1,(v))

/*
 * atomic_dec - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_dec(v) atomic_sub(1,(v))

#define atomic_dec_return(v) atomic_sub_return(1,(v))

/*
 * atomic_dec_and_test - decrement by 1 and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_dec_and_test(v) (atomic_sub_return(1, (v)) == 0)

/* Not needed on 64bit architectures */
static __inline__ unsigned int __div64_32(__uint64_t *n, unsigned int base)
{
	__uint64_t rem = *n;
	__uint64_t b = base;
	__uint64_t res, d = 1;
	unsigned int high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (__uint64_t) high << 32;
		rem -= (__uint64_t) (high*base) << 32;
	}

	while ((__int64_t)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

#define do_div(n,base) ({					\
	unsigned int __base = (base);				\
	unsigned int __rem;					\
	(void)(((typeof((n)) *)0) == ((__uint64_t *)0));	\
	if (((n) >> 32) == 0) {					\
		__rem = (unsigned int)(n) % __base;		\
		(n) = (unsigned int)(n) / __base;		\
	} else 							\
		__rem = __div64_32(&(n), __base);		\
	__rem;							\
 })

#ifndef CYGPKG_NET
/* Copy from bsd tcpip codes */
static int
arc4random(void)
{
    cyg_uint32 res;
    static unsigned long seed = 0xDEADB00B;
    HAL_CLOCK_READ(&res);  // Not so bad... (but often 0..N where N is small)
    seed = ((seed & 0x007F00FF) << 7) ^
        ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
        (res << 13) ^ (res >> 9);    // using the clock too!
    return (int)seed;
}

static void 
get_random_bytes(void *buf, size_t len)
{
    unsigned long ranbuf, *lp;
    lp = (unsigned long *)buf;
    while (len > 0) {
        ranbuf = arc4random();
        *lp++ = ranbuf;
        len -= sizeof(ranbuf);
    }
}
#endif
#endif /* __SYS_SUPPORT_H__ */
