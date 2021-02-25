/* FreeRTOS includes */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
//#include <autoconf.h>
#include <osdep_service.h>
#include <stdio.h>
#include <freertos_pmu.h>
//#include <tcm_heap.h>
#include <cmsis.h>

#if defined(CONFIG_PLATFORM_8710C)
#include "rtl8710c_freertos_pmu.h"
#endif

/********************* os depended utilities ********************/

#ifndef USE_MUTEX_FOR_SPINLOCK
#define USE_MUTEX_FOR_SPINLOCK 1
#endif
#if defined(CONFIG_PLATFORM_8710C)
u8 _freertos_get_scheduler_state(void);
#endif
//----- ------------------------------------------------------------------
// Misc Function
//----- ------------------------------------------------------------------

void save_and_cli()
{
	taskENTER_CRITICAL();
}

void restore_flags()
{
	taskEXIT_CRITICAL();
}

void cli()
{
	taskDISABLE_INTERRUPTS();
}

/* Not needed on 64bit architectures */
static unsigned int __div64_32(u64 *n, unsigned int base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	unsigned int high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high * base) << 32;
	}

	while ((u64)b > 0 && b < rem) {
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

/********************* os depended service ********************/

u8* _freertos_malloc(u32 sz)
{
	return pvPortMalloc(sz);
}

u8* _freertos_zmalloc(u32 sz)
{
	u8 *pbuf = _freertos_malloc(sz);

	if (pbuf != NULL)
		memset(pbuf, 0, sz);

	return pbuf;	
}

void _freertos_mfree(u8 *pbuf, u32 sz)
{
	/* To avoid gcc warnings */
	( void ) sz;
	
	vPortFree(pbuf);
}

static void _freertos_memcpy(void* dst, void* src, u32 sz)
{
	memcpy(dst, src, sz);
}

static int _freertos_memcmp(void *dst, void *src, u32 sz)
{
//under Linux/GNU/GLibc, the return value of memcmp for two same mem. chunk is 0
	if (!(memcmp(dst, src, sz)))
		return 1;

	return 0;
}

static void _freertos_memset(void *pbuf, int c, u32 sz)
{
	memset(pbuf, c, sz);
}

static void _freertos_init_sema(_sema *sema, int init_val)
{
	*sema = xSemaphoreCreateCounting(0xffffffff, init_val);	//Set max count 0xffffffff
}

static void _freertos_free_sema(_sema *sema)
{
	if(*sema != NULL)
		vSemaphoreDelete(*sema);

	*sema = NULL;
}

static void _freertos_up_sema(_sema *sema)
{
	xSemaphoreGive(*sema);
}

static void _freertos_up_sema_from_isr(_sema *sema)
{
	portBASE_TYPE taskWoken = pdFALSE;
	xSemaphoreGiveFromISR(*sema, &taskWoken);
	portEND_SWITCHING_ISR(taskWoken);
}

static u32 _freertos_down_sema(_sema *sema, u32 timeout)
{
	if(timeout == RTW_MAX_DELAY) {
		timeout = portMAX_DELAY;
	} else {
		timeout = rtw_ms_to_systime(timeout);
	}
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED && timeout != 0)
		return pdTRUE;
#endif
	if(xSemaphoreTake(*sema, timeout) != pdTRUE) {
		return pdFALSE;
	}

	return pdTRUE;
}

static void _freertos_mutex_init(_mutex *pmutex)
{
	*pmutex = xSemaphoreCreateMutex();
}

static void _freertos_mutex_free(_mutex *pmutex)
{
	if(*pmutex != NULL)
		vSemaphoreDelete(*pmutex);

	*pmutex = NULL;
}

static int __in_interrupt(void)
{
#ifdef ARM_CORE_CA7
	return __get_mode()!=CPSR_M_USR;
#else
#if defined(__ICCARM__)
	return (__get_PSR()&0x1FF)!=0;
#elif defined(__GNUC__)
	return (__get_xPSR()&0x1FF)!=0;
#endif
#endif
}

static void _freertos_mutex_get(_mutex *pmutex)
{
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED)
		return;
#endif
	if(__in_interrupt()){
		if(xSemaphoreTakeFromISR(*pmutex, NULL) != pdTRUE)
			printf("[%s] %s(%p) from ISR <<< FAIL >>> \n", pcTaskGetTaskName(NULL), __FUNCTION__, pmutex);
	}else{
		while(xSemaphoreTake(*pmutex, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
			printf("[%s] %s(%p) failed, retry\n", pcTaskGetTaskName(NULL), __FUNCTION__, pmutex);
	}
}

static int _freertos_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED && timeout_ms != 0)
		return 0;
#endif
	if(__in_interrupt()){
		if(xSemaphoreTakeFromISR(*pmutex, NULL) != pdTRUE){
			printf("[%s] %s(%p) from ISR <<< FAIL >>> \n", pcTaskGetTaskName(NULL), __FUNCTION__, pmutex);
			return -1;
		}
	}else{
		if(xSemaphoreTake(*pmutex, timeout_ms / portTICK_RATE_MS) != pdTRUE){
			printf("[%s] %s(%p) failed, retry\n", pcTaskGetTaskName(NULL), __FUNCTION__, pmutex);
			return -1;
		}
	}
	return 0;
}

static void _freertos_mutex_put(_mutex *pmutex)
{
	if(__in_interrupt()){
		xSemaphoreGiveFromISR(*pmutex, NULL);
	}else{
		xSemaphoreGive(*pmutex);
	}
}

static void _freertos_enter_critical_from_isr(_lock *plock, _irqL *pirqL);
static void _freertos_exit_critical_from_isr(_lock *plock, _irqL *pirqL);
static void _freertos_enter_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	( void ) plock;
#if defined(CONFIG_PLATFORM_8710C)
	if (__get_IPSR() != 0)
		_freertos_enter_critical_from_isr(plock,pirqL);
	else
#endif
	taskENTER_CRITICAL();
}

static void _freertos_exit_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	( void ) plock;
#if defined(CONFIG_PLATFORM_8710C)
	if (__get_IPSR() != 0)
		_freertos_exit_critical_from_isr(plock,pirqL);
	else
#endif
	taskEXIT_CRITICAL();
}

static u32 uxSavedInterruptStatus = 0;
static void _freertos_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	( void ) plock;
	
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

    uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
}

static void _freertos_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	( void ) plock;
	
	portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
}

static int _freertos_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	
	int ret = 0;
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED)
		return ret;
#endif
	while(xSemaphoreTake(*pmutex, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		printf("\n\r[%s] %s(%p) failed, retry\n", pcTaskGetTaskName(NULL), __FUNCTION__, pmutex);

	return ret;
}

static void _freertos_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	( void ) pirqL;
	
	xSemaphoreGive(*pmutex);
}

#if defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
#include "timer_api.h"
//static gtimer_t tmp_timer_obj;
#include "us_ticker_api.h"
static uint32_t lock_ticker;
#endif
static void _freertos_cpu_lock(void)
{
#if defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
	__disable_irq();
	icache_disable();
	dcache_disable();

	//gtimer_init(&tmp_timer_obj, 0xff);
	//gtimer_reload(&tmp_timer_obj, 400*1000 );	// 4s
	//gtimer_start(&tmp_timer_obj);
	us_ticker_init();
	lock_ticker = us_ticker_read();
#endif
}

static void _freertos_cpu_unlock(void)
{
#if defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
	//int duration = (int)gtimer_read_us(&tmp_timer_obj)/1000;
	//gtimer_deinit(&tmp_timer_obj);
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED){
		uint32_t duration = (us_ticker_read()-lock_ticker)/1000;
		// compensate rtos tick
		for(int i=0;i<duration;i++)
			xTaskIncrementTick();
	}
	
	dcache_enable();
	icache_enable();	
	icache_invalidate();
	__enable_irq();
#endif
}

static void _freertos_spinlock_init(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
	*plock = xSemaphoreCreateMutex();
#endif
}

static void _freertos_spinlock_free(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
	if(*plock != NULL)
		vSemaphoreDelete(*plock);

	*plock = NULL;
#endif
}

static void _freertos_spinlock(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED)
		return;
#endif
	while(xSemaphoreTake(*plock, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		printf("[%s] %s(%p) failed, retry\n", pcTaskGetTaskName(NULL), __FUNCTION__, plock);
#endif
}

static void _freertos_spinunlock(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
#if defined(CONFIG_PLATFORM_8710C)
	if(_freertos_get_scheduler_state() == OS_SCHEDULER_SUSPENDED)
		return;
#endif
	xSemaphoreGive(*plock);
#endif
}

static void _freertos_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
	/* To avoid gcc warnings */
	( void ) irqL;
	
	taskENTER_CRITICAL();
#if USE_MUTEX_FOR_SPINLOCK
	while(xSemaphoreTake(*plock, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		printf("[%s] %s(%p) failed, retry\n", pcTaskGetTaskName(NULL), __FUNCTION__, plock);
#endif
}

static void _freertos_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
	/* To avoid gcc warnings */
	( void ) irqL;
#if USE_MUTEX_FOR_SPINLOCK
	xSemaphoreGive(*plock);
#endif
	taskEXIT_CRITICAL();
}

static int _freertos_init_xqueue( _xqueue* queue, const char* name, u32 message_size, u32 number_of_messages )
{
	/* To avoid gcc warnings */
	( void ) name;
	
    if ( ( *queue = xQueueCreate( number_of_messages, message_size ) ) == NULL )
    {
        return -1;
    }

    return 0;
}

static int _freertos_push_to_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	if(timeout_ms == RTW_MAX_DELAY) {
	      timeout_ms = portMAX_DELAY;
	} else {
	      timeout_ms = rtw_ms_to_systime(timeout_ms);
	}

    if ( xQueueSendToBack( *queue, message, timeout_ms ) != pdPASS )
    {
        return -1;
    }

    return 0;
}

static int _freertos_pop_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	if(timeout_ms == RTW_WAIT_FOREVER) {
		timeout_ms = portMAX_DELAY;
	} else {
		timeout_ms = rtw_ms_to_systime(timeout_ms);
	}

    if ( xQueueReceive( *queue, message, timeout_ms ) != pdPASS )
    {
        return -1;
    }

    return 0;
}


static int _freertos_peek_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	if(timeout_ms == RTW_WAIT_FOREVER) {
		timeout_ms = portMAX_DELAY;
	} else {
		timeout_ms = rtw_ms_to_systime(timeout_ms);
	}

    if ( xQueuePeek( *queue, message, timeout_ms ) != pdPASS )
    {
        return -1;
    }

    return 0;
}
static int _freertos_deinit_xqueue( _xqueue* queue )
{
    int result = 0;

	  if( uxQueueMessagesWaiting( *queue ) )
	  {
	  	  result = -1;
	  }
    vQueueDelete( *queue );
    return result;
}

static u32 _freertos_get_current_time(void)
{
	return xTaskGetTickCount();	//The count of ticks since vTaskStartScheduler was called.
}

static u32 _freertos_systime_to_ms(u32 systime)
{
	return systime * portTICK_RATE_MS;
}

static u32 _freertos_systime_to_sec(u32 systime)
{
	return systime / configTICK_RATE_HZ;
}

static u32 _freertos_ms_to_systime(u32 ms)
{
	return ms / portTICK_RATE_MS;
}

static u32 _freertos_sec_to_systime(u32 sec)
{
	return sec * configTICK_RATE_HZ;
}

static void _freertos_msleep_os(int ms)
{
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8735B)
	vTaskDelay(ms / portTICK_RATE_MS);
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || (defined CONFIG_PLATFORM_AMEBAD2)
	if (pmu_yield_os_check()) {
		vTaskDelay(ms / portTICK_RATE_MS);
	} else {
		DelayMs(ms);
	}
#elif defined(CONFIG_PLATFORM_8710C)
	if (pmu_yield_os_check()) {
		vTaskDelay(ms / portTICK_RATE_MS);
	} else {
		hal_delay_ms(ms);
	}
#else
	vTaskDelay(ms / portTICK_RATE_MS);
#endif
}

static void _freertos_usleep_os(int us)
{
#if defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A)
	HalDelayUs(us);
#elif defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8735B)
	hal_delay_us(us);
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || (defined CONFIG_PLATFORM_AMEBAD2)
	DelayUs(us);
#else
	#error "Please implement hardware dependent micro second level sleep here"
#endif
}

static void _freertos_mdelay_os(int ms)
{
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8195BHP)
	vTaskDelay(ms / portTICK_RATE_MS);
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D)|| (defined CONFIG_PLATFORM_AMEBAD2)
	if (pmu_yield_os_check()) {
		vTaskDelay(ms / portTICK_RATE_MS);
	} else {
		DelayMs(ms);
	}
#elif defined(CONFIG_PLATFORM_8710C)
	if (pmu_yield_os_check()) {
		vTaskDelay(ms / portTICK_RATE_MS);
	} else {
		hal_delay_ms(ms);
	}
#else
	vTaskDelay(ms / portTICK_RATE_MS);
#endif
}

static void _freertos_udelay_os(int us)
{
#if defined(STM32F2XX)	|| defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A)
	HalDelayUs(us);
#elif defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8735B)
	hal_delay_us(us);
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D)|| (defined CONFIG_PLATFORM_AMEBAD2)
	DelayUs(us);
#else
	#error "Please implement hardware dependent micro second level sleep here"
#endif
}

static void _freertos_yield_os(void)
{
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8735B)
	taskYIELD();
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D)|| (defined CONFIG_PLATFORM_AMEBAD2)
	if (pmu_yield_os_check()) {
		taskYIELD();
	} else {
		DelayMs(1);
	}
#elif defined(CONFIG_PLATFORM_8710C)
	if (pmu_yield_os_check()) {
		taskYIELD();
	} else {
		hal_delay_ms(1);
	}
#else
	taskYIELD();
#endif
}

static void _freertos_ATOMIC_SET(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)	
	atomic_store(v, i);
#else
	atomic_set(v,i);
#endif
}

static int _freertos_ATOMIC_READ(ATOMIC_T *v)
{
#if defined(STDATOMIC)	
	return atomic_load(v);
#else	
	return atomic_read(v);
#endif
}

static void _freertos_ATOMIC_ADD(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)	
	atomic_fetch_add(v, i);
#else
	save_and_cli();
	v->counter += i;
	restore_flags();
#endif
}

static void _freertos_ATOMIC_SUB(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)	
	atomic_fetch_sub(v, i);
#else	
	save_and_cli();
	v->counter -= i;
	restore_flags();
#endif
}

static void _freertos_ATOMIC_INC(ATOMIC_T *v)
{
	_freertos_ATOMIC_ADD(v, 1);
}

static void _freertos_ATOMIC_DEC(ATOMIC_T *v)
{
	_freertos_ATOMIC_SUB(v, 1);
}

static int _freertos_ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)	
	atomic_fetch_add(v, i);
	return atomic_load(v);
#else	
	int temp;

	save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags();

	return temp;
#endif
}

static int _freertos_ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)	
	atomic_fetch_sub(v, i);
	return atomic_load(v);
#else	
	int temp;

	save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags();

	return temp;
#endif
}

static int _freertos_ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return _freertos_ATOMIC_ADD_RETURN(v, 1);
}

static int _freertos_ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return _freertos_ATOMIC_SUB_RETURN(v, 1);
}

static u64 _freertos_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;

	if (((n) >> 32) == 0) {
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	}
	else
		__rem = __div64_32(&(n), __base);
	
	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _freertos_arc4random(void)
{
#if defined(CONFIG_PLATFORM_8721D)|| (defined CONFIG_PLATFORM_AMEBAD2)

#if defined(CONFIG_PLATFORM_AMEBAD2)
	int value = (int)rand();
#else
	int value = (int)Rand();
#endif
	return value;
	
#else
	u32 res = xTaskGetTickCount();
	static unsigned long seed = 0xDEADB00B;

#if defined(CONFIG_PLATFORM_8711B)
	if(random_seed){
		seed = random_seed;
		random_seed = 0;
	}
#endif

	seed = ((seed & 0x007F00FF) << 7) ^
	    ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
	    (res << 13) ^ (res >> 9);    // using the clock too!
	return (int)seed;
#endif
}

static int _freertos_get_random_bytes(void *buf, u32 len)
{
#if 1 //becuase of 4-byte align, we use the follow code style.
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *) buf;

	for(i = 0; i < count; i ++) {
		lp[i] = _freertos_arc4random();  
		len -= sizeof(unsigned int);
	}

	if(len > 0) {
		ranbuf = _freertos_arc4random();
		_freertos_memcpy(&lp[i], &ranbuf, len);
	}
	return 0;
#else
	unsigned long ranbuf, *lp;
	lp = (unsigned long *)buf;
	while (len > 0) {
		ranbuf = _freertos_arc4random();
		*lp++ = ranbuf; //this op need the pointer is 4Byte-align!
		len -= sizeof(ranbuf);
	}
	return 0;
#endif
}

static u32 _freertos_GetFreeHeapSize(void)
{
	return (u32)xPortGetFreeHeapSize();
}
void *tcm_heap_malloc(int size);
static int _freertos_create_task(struct task_struct *ptask, const char *name,
	u32  stack_size, u32 priority, thread_func_t func, void *thctx)
{
	thread_func_t task_func = NULL;
	void *task_ctx = NULL;
	int ret = 0;

	ptask->task_name = name;

	if(func){
		task_func = func;
		task_ctx = thctx;
	}

	priority += tskIDLE_PRIORITY;

#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP && defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
	if(rtw_if_wifi_thread((char*)name) == 0){

		void *stack_addr = tcm_heap_malloc(stack_size*sizeof(int));
		//void *stack_addr = rtw_malloc(stack_size*sizeof(int));
		if(stack_addr == NULL){
			printf("Out of TCM heap in \"%s\" ", ptask->task_name);
		}
		ret = xTaskGenericCreate(
				task_func,
				(const char *)name,
				stack_size,
				task_ctx,
				priority,
				&ptask->task,
				stack_addr,
				NULL);
	}
	else
#endif
	{
		ret = xTaskCreate(
				task_func,
				(const char *)name,
				stack_size,
				task_ctx,
				priority,
				(TaskHandle_t*)&ptask->task);

	}
	if(ret != pdPASS){
		printf("Create Task \"%s\" Failed! ret=%d\n", ptask->task_name, ret);
	}
	
	DBG_TRACE("Create Task \"%s\"\n", ptask->task_name);
	return ret;
}
	
static void _freertos_delete_task(struct task_struct *ptask)
{
	if (!ptask->task){
		printf("_freertos_delete_task(): ptask is NULL!\n");
		return;
	}

	vTaskDelete(ptask->task);
	ptask->task = 0;
	
	DBG_TRACE("Delete Task \"%s\"\n", ptask->task_name);
}

static void _freertos_set_priority_task(void* task, u32 NewPriority)
{
	vTaskPrioritySet( (TaskHandle_t) task, (UBaseType_t) NewPriority );
}

static int _freertos_get_priority_task(void *task)
{
	return (int) uxTaskPriorityGet((TaskHandle_t) task );
}

static void _freertos_thread_enter(char *name)
{
	/* To avoid gcc warnings */
	( void ) name;
	
	DBG_INFO("\n\rRTKTHREAD %s\n", name);
}

static void _freertos_suspend_task(void *task)
{
	vTaskSuspend((TaskHandle_t) task );
}

static void _freertos_resume_task(void *task)
{
	vTaskResume((TaskHandle_t) task );
}

static void _freertos_thread_exit(void)
{
	DBG_INFO("\n\rRTKTHREAD exit %s\n", __FUNCTION__); 
	vTaskDelete(NULL); 
}

_timerHandle _freertos_timerCreate( const signed char *pcTimerName, 
							  osdepTickType xTimerPeriodInTicks, 
							  u32 uxAutoReload, 
							  void * pvTimerID, 
							  TIMER_FUN pxCallbackFunction )
{
	if(xTimerPeriodInTicks == TIMER_MAX_DELAY) {
		xTimerPeriodInTicks = portMAX_DELAY;
	}
	return xTimerCreate((const char *)pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, (TimerCallbackFunction_t)pxCallbackFunction);	
}

u32 _freertos_timerDelete( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return (u32)xTimerDelete(xTimer, xBlockTime);	
}

u32 _freertos_timerIsTimerActive( _timerHandle xTimer )
{
	return (u32)xTimerIsTimerActive(xTimer);	
}

u32  _freertos_timerStop( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return (u32)xTimerStop(xTimer, xBlockTime);	
}

u32  _freertos_timerChangePeriod( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepTickType xBlockTime )
{
	if(xNewPeriod == 0)
		xNewPeriod += 1;
	return (u32)xTimerChangePeriod(xTimer, xNewPeriod, xBlockTime);	
}
void *_freertos_timerGetID( _timerHandle xTimer ){

	return pvTimerGetTimerID(xTimer);
}

u32  _freertos_timerStart( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return (u32)xTimerStart(xTimer, xBlockTime);	
}

u32  _freertos_timerStartFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return (u32)xTimerStartFromISR(xTimer, pxHigherPriorityTaskWoken);	
}

u32  _freertos_timerStopFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return (u32)xTimerStopFromISR(xTimer, pxHigherPriorityTaskWoken);	
}

u32  _freertos_timerResetFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return (u32)xTimerResetFromISR(xTimer, pxHigherPriorityTaskWoken);	
}

u32  _freertos_timerChangePeriodFromISR( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	if(xNewPeriod == 0)
		xNewPeriod += 1;
	return (u32)xTimerChangePeriodFromISR(xTimer, xNewPeriod, pxHigherPriorityTaskWoken);
}

u32  _freertos_timerReset( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return (u32)xTimerReset(xTimer, xBlockTime);	
}

void _freertos_acquire_wakelock(void)
{
#if defined(CONFIG_PLATFORM_8195A)

#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
   	pmu_acquire_wakelock(PMU_WLAN_DEVICE);
#endif
	
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C)|| (defined CONFIG_PLATFORM_AMEBAD2)

#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
	if (pmu_yield_os_check()) 
   		 pmu_acquire_wakelock(PMU_WLAN_DEVICE);
#endif

#endif
}

void _freertos_release_wakelock(void)
{

#if defined(CONFIG_PLATFORM_8195A)

#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
    pmu_release_wakelock(PMU_WLAN_DEVICE);
#endif
	
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C)|| (defined CONFIG_PLATFORM_AMEBAD2)

#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
	if (pmu_yield_os_check()) 
		pmu_release_wakelock(PMU_WLAN_DEVICE);
#endif

#endif
}

void _freertos_wakelock_timeout(uint32_t timeout)
{
#if defined(CONFIG_PLATFORM_8195A)
	
#elif defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C)|| (defined CONFIG_PLATFORM_AMEBAD2)
	if (pmu_yield_os_check()) 
		pmu_set_sysactive_time(timeout);
	else {
		DBG_INFO("can't aquire wake during suspend flow!!\n");
	}
#endif
}

u8 _freertos_get_scheduler_state(void)
{
	u8 state = xTaskGetSchedulerState();
	switch(state){
		case taskSCHEDULER_NOT_STARTED:	state = OS_SCHEDULER_NOT_STARTED;	break;
		case taskSCHEDULER_RUNNING:		state = OS_SCHEDULER_RUNNING;		break;
		case taskSCHEDULER_SUSPENDED:	state = OS_SCHEDULER_SUSPENDED;		break;
	}
	return state;
}


void _freertos_create_secure_context(u32 secure_stack_size)
{
	(void)secure_stack_size;
	/* initial support on freertos 10.2.0 */
#if (tskKERNEL_VERSION_MAJOR>=10) && (tskKERNEL_VERSION_MINOR>=2)	
	/* This task calls secure side functions. So allocate a secure context for
	 * it. */
	portALLOCATE_SECURE_CONTEXT( secure_stack_size );
#endif	
}

void* _freertos_get_current_TaskHandle(void)
{
	return (void*)xTaskGetCurrentTaskHandle();
}

const struct osdep_service_ops osdep_service = {
	_freertos_malloc,			//rtw_vmalloc
	_freertos_zmalloc,			//rtw_zvmalloc
	_freertos_mfree,			//rtw_vmfree
	_freertos_malloc,			//rtw_malloc
	_freertos_zmalloc,			//rtw_zmalloc
	_freertos_mfree,			//rtw_mfree
	_freertos_memcpy,			//rtw_memcpy
	_freertos_memcmp,			//rtw_memcmp
	_freertos_memset,			//rtw_memset
	_freertos_init_sema,		//rtw_init_sema
	_freertos_free_sema,		//rtw_free_sema
	_freertos_up_sema,			//rtw_up_sema
	_freertos_up_sema_from_isr,	//rtw_up_sema_from_isr
	_freertos_down_sema,		//rtw_down_sema
	_freertos_mutex_init,		//rtw_mutex_init
	_freertos_mutex_free,		//rtw_mutex_free
	_freertos_mutex_get,		//rtw_mutex_get
	_freertos_mutex_get_timeout,//rtw_mutex_get_timeout
	_freertos_mutex_put,		//rtw_mutex_put
	_freertos_enter_critical,	//rtw_enter_critical
	_freertos_exit_critical,	//rtw_exit_critical
	_freertos_enter_critical_from_isr,	//rtw_enter_critical_from_isr
	_freertos_exit_critical_from_isr,	//rtw_exit_critical_from_isr
	NULL,		//rtw_enter_critical_bh
	NULL,		//rtw_exit_critical_bh
	_freertos_enter_critical_mutex,	//rtw_enter_critical_mutex
	_freertos_exit_critical_mutex,	//rtw_exit_critical_mutex
	_freertos_cpu_lock,
	_freertos_cpu_unlock,
	_freertos_spinlock_init,		//rtw_spinlock_init
	_freertos_spinlock_free,		//rtw_spinlock_free
	_freertos_spinlock,				//rtw_spin_lock
	_freertos_spinunlock,			//rtw_spin_unlock
	_freertos_spinlock_irqsave,		//rtw_spinlock_irqsave
	_freertos_spinunlock_irqsave,	//rtw_spinunlock_irqsave
	_freertos_init_xqueue,			//rtw_init_xqueue
	_freertos_push_to_xqueue,		//rtw_push_to_xqueue
	_freertos_pop_from_xqueue,		//rtw_pop_from_xqueue
	_freertos_peek_from_xqueue,		//rtw_peek_from_xqueue
	_freertos_deinit_xqueue,		//rtw_deinit_xqueue
	_freertos_get_current_time,		//rtw_get_current_time
	_freertos_systime_to_ms,		//rtw_systime_to_ms
	_freertos_systime_to_sec,		//rtw_systime_to_sec
	_freertos_ms_to_systime,		//rtw_ms_to_systime
	_freertos_sec_to_systime,		//rtw_sec_to_systime
	_freertos_msleep_os,	//rtw_msleep_os
	_freertos_usleep_os,	//rtw_usleep_os
	_freertos_mdelay_os,	//rtw_mdelay_os
	_freertos_udelay_os,	//rtw_udelay_os
	_freertos_yield_os,		//rtw_yield_os
	
	_freertos_ATOMIC_SET,	//ATOMIC_SET
	_freertos_ATOMIC_READ,	//ATOMIC_READ
	_freertos_ATOMIC_ADD,	//ATOMIC_ADD
	_freertos_ATOMIC_SUB,	//ATOMIC_SUB
	_freertos_ATOMIC_INC,	//ATOMIC_INC
	_freertos_ATOMIC_DEC,	//ATOMIC_DEC
	_freertos_ATOMIC_ADD_RETURN,	//ATOMIC_ADD_RETURN
	_freertos_ATOMIC_SUB_RETURN,	//ATOMIC_SUB_RETURN
	_freertos_ATOMIC_INC_RETURN,	//ATOMIC_INC_RETURN
	_freertos_ATOMIC_DEC_RETURN,	//ATOMIC_DEC_RETURN

	_freertos_modular64,			//rtw_modular64
	_freertos_get_random_bytes,		//rtw_get_random_bytes
	_freertos_GetFreeHeapSize,		//rtw_getFreeHeapSize

	_freertos_create_task,			//rtw_create_task
	_freertos_delete_task,			//rtw_delete_task
	NULL,							//rtw_wakeup_task
	_freertos_set_priority_task,	//rtw_set_priority_task
	_freertos_get_priority_task,	//rtw_get_priority_task
	_freertos_suspend_task,			//rtw_suspend_task
	_freertos_resume_task,			//rtw_resume_task

	_freertos_thread_enter,			//rtw_thread_enter
	_freertos_thread_exit,			//rtw_thread_exit

	_freertos_timerCreate,			//rtw_timerCreate,       
	_freertos_timerDelete,			//rtw_timerDelete,       
	_freertos_timerIsTimerActive,	//rtw_timerIsTimerActive,
	_freertos_timerStop,			//rtw_timerStop,         
	_freertos_timerChangePeriod,	//rtw_timerChangePeriod 
	_freertos_timerGetID,			//rtw_timerGetID
	_freertos_timerStart,			//rtw_timerStart
	_freertos_timerStartFromISR,	//rtw_timerStartFromISR
	_freertos_timerStopFromISR,		//rtw_timerStopFromISR
	_freertos_timerResetFromISR,	//rtw_timerResetFromISR
	_freertos_timerChangePeriodFromISR,	//rtw_timerChangePeriodFromISR
	_freertos_timerReset,			//rtw_timerReset

	_freertos_acquire_wakelock,		//rtw_acquire_wakelock
	_freertos_release_wakelock,		//rtw_release_wakelock
	_freertos_wakelock_timeout,		//rtw_wakelock_timeout
	_freertos_get_scheduler_state,	//rtw_get_scheduler_state
	_freertos_create_secure_context,	// rtw_create_secure_context	
	_freertos_get_current_TaskHandle,	//rtw_get_current_TaskHandle
};
