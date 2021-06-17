#ifndef _FREERTOS_SERVICE_H_
#define _FREERTOS_SERVICE_H_

//----- ------------------------------------------------------------------
// Include Files
//----- ------------------------------------------------------------------
//#include "wireless.h"
#include "dlist.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
// --------------------------------------------
//	Platform dependent include file
// --------------------------------------------
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
#include "platform/platform_stdlib.h"
extern VOID RtlUdelayOS(u32 us);
#elif defined(CONFIG_PLATFORM_8711B)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_PLATFORM_8721D)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8821C)
#include "basic_types.h"
#include "wlan_basic_types.h"
#elif defined(CONFIG_HARDWARE_8188F)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8192E)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8723D)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8188E)
#include "platform/platform_stdlib.h"
#else
// other MCU may use standard library 
#include <string.h>
#endif


#if (defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI) || defined(CONFIG_LX_HCI)
/* For SPI interface transfer and us delay implementation */
#if !defined(CONFIG_PLATFORM_8195A) && !defined(CONFIG_PLATFORM_8711B) && !defined(CONFIG_PLATFORM_8721D)  && !defined(CONFIG_PLATFORM_8195BHP) && !defined(CONFIG_PLATFORM_8710C)
#include <rtwlan_bsp.h>	
#endif
#endif


// --------------------------------------------
//	Platform dependent type define
// --------------------------------------------
#ifndef FIELD_OFFSET
#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
#endif

// os types
typedef char			    osdepCHAR;
typedef float			    osdepFLOAT;
typedef double			    osdepDOUBLE;
typedef long			    osdepLONG;
typedef short			    osdepSHORT;
typedef unsigned long	    osdepSTACK_TYPE;
typedef long			    osdepBASE_TYPE;
typedef unsigned long	    osdepTickType;

typedef void*	            _timerHandle;
typedef void*	            _sema;
typedef void*	            _mutex;
typedef void*	            _lock;
typedef void*	            _queueHandle;
typedef void*	            _xqueue;
typedef struct timer_list	_timer;

#define rtw_timer_list timer_list

typedef	struct sk_buff		_pkt;
typedef unsigned char		_buffer;
typedef unsigned int        systime;

#ifndef __LIST_H
#warning "DLIST_NOT_DEFINE!!!!!!"
struct list_head {
	struct list_head *next, *prev;
};
#endif

struct	__queue	{
	struct	list_head	queue;
	_lock			lock;
};

typedef struct	__queue		_queue;
typedef struct	list_head	_list;
typedef unsigned long		_irqL;

typedef void*			    _thread_hdl_;
typedef void			    thread_return;
typedef void*			    thread_context;

#if !defined(CONFIG_PLATFORM_8710C) 
typedef struct { volatile int counter; } atomic_t;
#endif

#define ATOMIC_T atomic_t
#define HZ configTICK_RATE_HZ

#define   KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
/* emulate a modern version */
#define LINUX_VERSION_CODE KERNEL_VERSION(2, 6, 17)

static __inline _list *get_next(_list	*list)
{
	return list->next;
}	

static __inline _list	*get_list_head(_queue	*queue)
{
	return (&(queue->queue));
}

#define LIST_CONTAINOR(ptr, type, member) \
	((type *)((char *)(ptr)-(SIZE_T)((char *)&((type *)ptr)->member - (char *)ptr)))
//#define container_of(p,t,n) (t*)((p)-&(((t*)0)->n))
#define container_of(ptr, type, member) \
			((type *)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#define TASK_PRORITY_LOW  				1
#define TASK_PRORITY_MIDDLE   			2
#define TASK_PRORITY_HIGH    			3
#define TASK_PRORITY_SUPER    			4

#define TIMER_MAX_DELAY    				0xFFFFFFFF

void save_and_cli(void);
void restore_flags(void);
void cli(void);

#ifndef mdelay
#define mdelay(t)					((t/portTICK_RATE_MS)>0)?(vTaskDelay(t/portTICK_RATE_MS)):(vTaskDelay(1))
#endif

#ifndef udelay
#define udelay(t)					((t/(portTICK_RATE_MS*1000))>0)?vTaskDelay(t/(portTICK_RATE_MS*1000)):(vTaskDelay(1))
#endif
//----- ------------------------------------------------------------------
// Common Definition
//----- ------------------------------------------------------------------

#define __init
#define __exit
#define __devinit
#define __devexit

#define KERN_ERR
#define KERN_INFO
#define KERN_NOTICE

#undef GFP_KERNEL
#define GFP_KERNEL			1
#define GFP_ATOMIC			1

#define SET_MODULE_OWNER(some_struct)	do { } while (0)
#define SET_NETDEV_DEV(dev, obj)	do { } while (0)
#define register_netdev(dev)		(0)
#define unregister_netdev(dev)		do { } while (0)
#define netif_queue_stopped(dev)	(0)
#define netif_wake_queue(dev)		do { } while (0)
#define printk				printf

#define DBG_ERR(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)

#if defined(WLAN_INTF_DBG) && WLAN_INTF_DBG
#define DBG_TRACE(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)
#define DBG_INFO(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)
#else
#define DBG_TRACE(fmt, args...)
#define DBG_INFO(fmt, args...)
#endif
#define HALT()				do { cli(); for(;;);} while(0)
#undef ASSERT
#define ASSERT(x)			do { \
						if((x) == 0){\
							printf("\n\rAssert(" #x ") failed on line %d in file %s", __LINE__, __FILE__); \
						HALT();}\
					} while(0)

#undef DBG_ASSERT
#define DBG_ASSERT(x, msg)		do { \
						if((x) == 0) \
							printf("\n\r%s, Assert(" #x ") failed on line %d in file %s", msg, __LINE__, __FILE__); \
					} while(0)

//----- ------------------------------------------------------------------
// Atomic Operation
//----- ------------------------------------------------------------------

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_read
#define atomic_read(v)  ((v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_set
#define atomic_set(v,i) ((v)->counter = (i))

 /*
  *      These inlines deal with timer wrapping correctly. You are 
  *      strongly encouraged to use them
  *      1. Because people otherwise forget
  *      2. Because if the timer wrap changes in future you wont have to
  *         alter your driver code.
  *
  * time_after(a,b) returns true if the time a is after time b.
  *
  * Do this with "<0" and ">=0" to only test the sign of the result. A
  * good compiler would generate better code (and a really good compiler
  * wouldn't care). Gcc is currently neither.
  */
 #define time_after(a,b)	((long)(b) - (long)(a) < 0)
 #define time_before(a,b)	time_after(b,a)
  
 #define time_after_eq(a,b)	((long)(a) - (long)(b) >= 0)
 #define time_before_eq(a,b)	time_after_eq(b,a)


extern void	rtw_init_listhead(_list *list);
extern u32	rtw_is_list_empty(_list *phead);
extern void	rtw_list_insert_head(_list *plist, _list *phead);
extern void	rtw_list_insert_tail(_list *plist, _list *phead);
extern void	rtw_list_delete(_list *plist);

extern int rtw_if_wifi_thread(char *name);

#if (defined CONFIG_PLATFORM_8711B) || (defined CONFIG_PLATFORM_8721D)
extern u32 random_seed;
#endif

#endif /* _FREERTOS_SERVICE_H_ */
