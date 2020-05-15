/******************************************************************************
  *
  * Name: sys-support.h - System type support for Linux
  *       $Revision: 1.1.1.1 $
  *
  *****************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__
#include <pkgconf/system.h>
#include <ecos/wrapper_cdl.h>
#include <cyg/kernel/kapi.h>

#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
#ifdef CYGPKG_NET
extern cyg_uint32 timeout(timeout_fun *fun, void *arg, cyg_int32 delta);
extern void untimeout(timeout_fun *fun, void * arg);
#elif defined(CYGPKG_NET_LWIP)
#include <lwip/sys.h>
extern void sys_timeout(u32_t msecs, sys_timeout_handler h, void *arg);
extern void sys_untimeout(sys_timeout_handler h, void *arg);
#define timeout(fun, arg, delta)	sys_timeout(10 * delta, fun, arg)
#define untimeout(fun, arg)		sys_untimeout(fun, arg)
#endif
#endif

#define jiffies	cyg_current_time()

enum {
	TIMER_NO_INIT = 0,
	TIMER_INIT = 1,
	TIMER_START = 2,
	TIMER_DISABLE = 3
};

#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
struct timer_list {
	int		flag;
	unsigned long	data;
	void (*function)(void *);
};
#else
struct timer_list {
	cyg_handle_t	alarm_hdl;	
	cyg_alarm	alarm_obj;
	int		flag;
	unsigned long	data;
	void (*function)(cyg_handle_t, cyg_addrword_t);
};
#endif

static inline void init_timer(struct timer_list *timer)
{
	u32 data = timer->data;
#ifndef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
	cyg_handle_t sys_clk;
	cyg_handle_t counter_hdl;
	void (*function)(cyg_handle_t, cyg_addrword_t) = timer->function;
#else
	void (*function)(void *) = timer->function;
#endif
	int flags;
	save_and_cli(flags);

	if (timer->flag != TIMER_DISABLE) {
		if (timer->flag == TIMER_NO_INIT) {
#ifndef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
			sys_clk = cyg_real_time_clock();
			cyg_clock_to_counter(sys_clk, &counter_hdl);
			cyg_alarm_create(counter_hdl, function, data,
						&timer->alarm_hdl, &timer->alarm_obj);
#endif
			timer->flag = TIMER_INIT;
		}
		else if (timer->flag == TIMER_START) {
#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
			untimeout(timer->function, (void *)timer->data);
#else
			cyg_alarm_disable(timer->alarm_hdl);
#endif
			timer->flag = TIMER_DISABLE;
		}
	}
	restore_flags(flags);	
	
}

static inline void mod_timer(struct timer_list *timer, cyg_tick_count_t timeout_ticks)
{
#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
	cyg_uint32 stamp;
#endif
	int flags;
	save_and_cli(flags);

	if (timer->flag == TIMER_NO_INIT) {
		if (timer->function) {
#ifndef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
			cyg_handle_t sys_clk;
			cyg_handle_t counter_hdl;
		
			sys_clk = cyg_real_time_clock();
			cyg_clock_to_counter(sys_clk, &counter_hdl);
			cyg_alarm_create(counter_hdl, (cyg_alarm_t *)timer->function, timer->data, &timer->alarm_hdl, &timer->alarm_obj);
#endif
			timer->flag = TIMER_INIT;
		}
		else {
			restore_flags(flags);
			return;
		}
	}
	else if (timer->flag == TIMER_START) {
#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
		untimeout(timer->function, (void *)timer->data);
#else
		cyg_alarm_disable(timer->alarm_hdl);
#endif
	 	timer->flag = TIMER_DISABLE;
	}
	
#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
	timeout_ticks -= cyg_current_time();
	if (timeout_ticks < 0)
		timeout_ticks = 2;
#ifdef CYGPKG_NET
	stamp = timeout(timer->function, (void *)timer->data, (cyg_int32)timeout_ticks);
	if (stamp)
		timer->flag = TIMER_START;
	else
		diag_printf("###mod_timer() - no slots available###\n");
#elif defined(CYGPKG_NET_LWIP)
	timeout(timer->function, (void *)timer->data, (cyg_int32)timeout_ticks);
	timer->flag = TIMER_START;
#endif
#else
	if (timeout_ticks < cyg_current_time())
		timeout_ticks = cyg_current_time() + 2;
	
	cyg_alarm_initialize(timer->alarm_hdl, timeout_ticks, 0);
	
	if (timer->flag == TIMER_DISABLE)
		 cyg_alarm_enable(timer->alarm_hdl);
	timer->flag = TIMER_START;
#endif
	restore_flags(flags);	
}
  
static inline int timer_pending (const struct timer_list * timer)
{
#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
	if (timer->flag != TIMER_NO_INIT)
#else
	if (timer->alarm_hdl && timer->flag != TIMER_NO_INIT)
#endif
		return 1;
	else
		return 0;
}
  
static inline void  del_timer_sync(struct timer_list * timer)
{
	int flags;
	save_and_cli(flags);

#ifdef RTLPKG_DEVS_ETH_RLTK_WLAN_TIMER_USE_NET_TIMEOUT
	if (timer->flag != TIMER_INIT) {
		if (timer->flag == TIMER_START)
			untimeout(timer->function, (void *)timer->data);
		timer->flag = TIMER_NO_INIT;
	}
#else
	if (timer->alarm_hdl && timer->flag != TIMER_INIT) {
		if (timer->flag == TIMER_START)
			cyg_alarm_disable(timer->alarm_hdl);
		
		cyg_alarm_delete(timer->alarm_hdl);
		timer->flag = TIMER_NO_INIT;
	}
#endif
	restore_flags(flags);		
}

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
 #define time_after(a,b)		((long)(b) - (long)(a) < 0)
 #define time_before(a,b)		time_after(b,a)
  
 #define time_after_eq(a,b)		((long)(a) - (long)(b) >= 0)
 #define time_before_eq(a,b)		time_after_eq(b,a)
  


#endif /* __TIMER_H__ */
