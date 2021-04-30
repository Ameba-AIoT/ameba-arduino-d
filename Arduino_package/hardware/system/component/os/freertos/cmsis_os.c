

#include "cmsis_os.h"
#include "diag.h"

// zzw arduino
#if defined(ARDUINO_SDK)
#include "cmsis_gcc.h"
#endif

#define CMSIS_OS_ERR(fmt, args...)          _DbgDump("\n\r%s: " fmt, __FUNCTION__, ## args)

extern void *_memset( void *s, int c, SIZE_T n );
extern void * memset ( void * ptr, int value, size_t num );
#define os_memset           memset//_memset

#if configSignalManagementSupport  // the older FreeRTOS version didn't support Signal Management functions
#if 0
#define THREAD_SIGNAL_MAP_SIZE          32
typedef struct thread_signal_map  {
    uint8_t               is_in_use;
    osThreadId            thread_id;
    EventGroupHandle_t    signals;
} ThreadSignalEntity;


ThreadSignalEntity ThreadSignalMapTable[THREAD_SIGNAL_MAP_SIZE]={0};
#endif

typedef struct thread_signal_map  {
    osThreadId            thread_id;
    EventGroupHandle_t    signals;
    void                  *pnext;
} ThreadSignalRec, *pThreadSignalRec;

ThreadSignalRec *pThreadSignalMapHead;
ThreadSignalRec *pThreadSignalMapTail;
#endif

/* Convert from CMSIS type osPriority to FreeRTOS priority number */
static unsigned portBASE_TYPE makeFreeRtosPriority (osPriority priority)
{
    unsigned portBASE_TYPE fpriority = tskIDLE_PRIORITY;

    if (priority != osPriorityError) {
        fpriority += (priority - osPriorityIdle);
    }

    return fpriority;
}


/* Convert from FreeRTOS priority number to CMSIS type osPriority */
static osPriority makeCmsisPriority (unsigned portBASE_TYPE fpriority)
{
    osPriority priority = osPriorityError;

    if ((fpriority - tskIDLE_PRIORITY) <= (osPriorityRealtime - osPriorityIdle)) {
        priority = (osPriority)((int)osPriorityIdle + (int)(fpriority - tskIDLE_PRIORITY));
    }

    return priority;
}


/* Determine whether we are in thread mode or handler mode. */
static int inHandlerMode (void)
{
    return __get_IPSR() != 0;
}

#if configSignalManagementSupport  // the older FreeRTOS version didn't support Signal Management functions
static void add_thread_signal_map (osThreadId thread_id, EventGroupHandle_t signals)
{
    int dummy=0;
//    uint32_t i;
    ThreadSignalRec *prec_entity;

    if (inHandlerMode()) {
        dummy = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else {
        vPortEnterCritical();
    }

    prec_entity = (ThreadSignalRec*) malloc(sizeof(ThreadSignalRec));

    if (prec_entity != NULL) {
        prec_entity->thread_id = thread_id;
        prec_entity->signals = signals;
        prec_entity->pnext = NULL;
        if (pThreadSignalMapHead == NULL) {
            pThreadSignalMapHead = prec_entity;
            pThreadSignalMapTail = prec_entity;
        }
        else {
            pThreadSignalMapTail->pnext = prec_entity;
            pThreadSignalMapTail = prec_entity;
        }
    }
    else {
        CMSIS_OS_ERR("No Free Thread-Signal entity\n");
    }

#if 0    
    for (i=0;i<THREAD_SIGNAL_MAP_SIZE;i++)
    {
        if (!ThreadSignalMapTable[i].is_in_use) {
            ThreadSignalMapTable[i].is_in_use = 1;
            ThreadSignalMapTable[i].thread_id = thread_id;
            ThreadSignalMapTable[i].signals = signals;
            break;
        }
    }

    if (i >= THREAD_SIGNAL_MAP_SIZE) {
        // No free Thread-Signals map entity
        CMSIS_OS_ERR("No Free Thread-Signal entity\n");
    }

#endif

    if (inHandlerMode()) {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(dummy);
    }
    else {
        vPortExitCritical();
    }

}

static EventGroupHandle_t find_signal_by_thread (osThreadId thread_id)
{
    EventGroupHandle_t signals_hdl=NULL;
//    uint32_t i;
    int dummy=0;
    ThreadSignalRec *prec_entity;
    
    if (inHandlerMode()) {
        dummy = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else {
        vPortEnterCritical();
    }

    prec_entity = pThreadSignalMapHead;
    while (prec_entity != NULL) {
        if (prec_entity->thread_id == thread_id) {
            signals_hdl = prec_entity->signals;
            break;
        }
        else {
            prec_entity = prec_entity->pnext;
        }
    }

#if 0    
    for (i=0;i<THREAD_SIGNAL_MAP_SIZE;i++)
    {
        if ((ThreadSignalMapTable[i].is_in_use)  && 
            (ThreadSignalMapTable[i].thread_id == thread_id)) {
            signals_hdl = ThreadSignalMapTable[i].signals;
            break;
        }
    }
#endif

    if (inHandlerMode()) {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(dummy);
    }
    else {
        vPortExitCritical();
    }

    if (NULL == signals_hdl) {
        CMSIS_OS_ERR("Cannot find the EventGroup Handle by thread_id\n");
    }

    return signals_hdl;
}

static EventGroupHandle_t remove_thread_signal_map (osThreadId thread_id)
{
    EventGroupHandle_t signals_hdl=NULL;
//    uint32_t i;
    int dummy=0;
    ThreadSignalRec *prec_entity;
    ThreadSignalRec *pprev_entity;
    
    if (inHandlerMode()) {
        dummy = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else {
        vPortEnterCritical();
    }

    prec_entity = pThreadSignalMapHead;
    pprev_entity = NULL;
    while (prec_entity != NULL) {
        if (prec_entity->thread_id == thread_id) {
            signals_hdl = prec_entity->signals;
            if (prec_entity == pThreadSignalMapHead) {
                if (prec_entity->pnext != NULL) {
                    pThreadSignalMapHead = prec_entity->pnext;
                }
                else {
                    pThreadSignalMapHead = NULL;
                    pThreadSignalMapTail = NULL;
                }
            }
            else if (prec_entity == pThreadSignalMapTail) {
                pprev_entity->pnext = NULL;
                pThreadSignalMapTail = pprev_entity;
            }
            else {
                pprev_entity->pnext = prec_entity->pnext;
            }
            free((void *)prec_entity);
            break;
        }
        else {
            pprev_entity = prec_entity;
            prec_entity = prec_entity->pnext;
        }        
    }

#if 0
    for (i=0;i<THREAD_SIGNAL_MAP_SIZE;i++)
    {
        if ((ThreadSignalMapTable[i].is_in_use)  && 
            (ThreadSignalMapTable[i].thread_id == thread_id)) {
            signals_hdl = ThreadSignalMapTable[i].signals;
            ThreadSignalMapTable[i].thread_id = NULL;
            ThreadSignalMapTable[i].signals = NULL;
            ThreadSignalMapTable[i].is_in_use = 0;
            break;
        }
    }
#endif

    if (inHandlerMode()) {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(dummy);
    }
    else {
        vPortExitCritical();
    }

    return signals_hdl;

}
#endif  //end of #if configSignalManagementSupport

/* Convert timeout milli second to system ticks */
static portTickType millisec_to_ticks(uint32_t millisec)
{
    portTickType ticks=0;
    
    if (millisec == osWaitForever) {
        ticks = portMAX_DELAY;
    }
    else {
        if (millisec != 0) {
            ticks = millisec/portTICK_RATE_MS;
            if (ticks == 0) {
                ticks = 1;
            }
        }
    }

    return ticks;
}

//  ==== Kernel Control Functions ====
///Initialize the RTOS Kernel for creating objects. 
osStatus osKernelInitialize (void)
{
    // It seems not thing needs to do for FreeRTOS initialization
    return osOK;
}


/// Start the RTOS Kernel with executing the specified thread.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osKernelStart shall be consistent in every CMSIS-RTOS.
osStatus osKernelStart (void)
{
    vTaskStartScheduler();

    return osOK;
}


/// Get the RTOS kernel system timer counter 
/// \note MUST REMAIN UNCHANGED: \b osKernelSysTick shall be consistent in every CMSIS-RTOS.
/// \return RTOS kernel system timer as 32-bit value 
uint32_t osKernelSysTick (void)
{
    return ((uint32_t)xTaskGetTickCount());
}

/// Check if the RTOS kernel is already started.
/// \note MUST REMAIN UNCHANGED: \b osKernelRunning shall be consistent in every CMSIS-RTOS.
/// \return 0 RTOS is not started, 1 RTOS is started.
int32_t osKernelRunning(void);


//  ==== Thread Management ====


/// Create a thread and add it to Active Threads and set it to state READY.
/// \param[in]     thread_def    thread definition referenced with \ref osThread.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \return thread ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osThreadCreate shall be consistent in every CMSIS-RTOS.
osThreadId osThreadCreate (const osThreadDef_t *thread_def, void *argument)
{
//    (void) argument;
    xTaskHandle handle;
    BaseType_t  result;

    result = xTaskCreate((pdTASK_CODE)thread_def->pthread,
                (const portCHAR *)thread_def->name,
                (thread_def->stacksize)?(thread_def->stacksize/4):configMINIMAL_STACK_SIZE /* RTX default stack size: 256 */,
                argument,
                makeFreeRtosPriority(thread_def->tpriority),
                &handle);
    if (pdPASS == result) {
#if configSignalManagementSupport
        EventGroupHandle_t signals;
        
        signals = xEventGroupCreate();
        if (signals == NULL) {
            /* The event group was not created because there was insufficient
               FreeRTOS heap available. */            
            CMSIS_OS_ERR("Create a EventGroup for a new thread failed\n");
        }
        else
        {
            add_thread_signal_map(handle, signals);
        }
#endif        
    }
    else
    {
        CMSIS_OS_ERR("Create a new thread(%s) failed\r\n", thread_def->name);
        handle = NULL;
    }

    return handle;
}


/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osThreadGetId shall be consistent in every CMSIS-RTOS.
osThreadId osThreadGetId (void)
{
    return xTaskGetCurrentTaskHandle();
}


/// Terminate execution of a thread and remove it from Active Threads.
/// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osThreadTerminate shall be consistent in every CMSIS-RTOS.
osStatus osThreadTerminate (osThreadId thread_id)
{
#if configSignalManagementSupport
    EventGroupHandle_t EventHandle;
    
    EventHandle = remove_thread_signal_map (thread_id);
    if (EventHandle) {
        vEventGroupDelete (EventHandle);
    }
#endif

    vTaskDelete(thread_id);

    return osOK;
}


/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osThreadYield shall be consistent in every CMSIS-RTOS.
osStatus osThreadYield (void)
{
    taskYIELD();

    return osOK;
}


/// Change priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osThreadSetPriority shall be consistent in every CMSIS-RTOS.
osStatus osThreadSetPriority (osThreadId thread_id, osPriority priority)
{
    vTaskPrioritySet(thread_id, makeFreeRtosPriority(priority));

    return osOK;
}


/// Get current priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return current priority value of the thread function.
/// \note MUST REMAIN UNCHANGED: \b osThreadGetPriority shall be consistent in every CMSIS-RTOS.
osPriority osThreadGetPriority (osThreadId thread_id)
{
    return makeCmsisPriority(uxTaskPriorityGet(thread_id));
}



//  ==== Generic Wait Functions ====

/// Wait for Timeout (Time Delay)
/// \param[in]     millisec      time delay value
/// \return status code that indicates the execution status of the function.
osStatus osDelay (uint32_t millisec)
{
#if INCLUDE_vTaskDelay
    portTickType ticks = (millisec * configTICK_RATE_HZ) / 1000;

    vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */

    return osOK;
#else
    (void) millisec;

    return osErrorResource;
#endif
}


#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))     // Generic Wait available

/// Wait for Signal, Message, Mail, or Timeout
/// \param[in] millisec          timeout value or 0 in case of no time-out
/// \return event that contains signal, message, or mail information or error code.
/// \note MUST REMAIN UNCHANGED: \b osWait shall be consistent in every CMSIS-RTOS.
osEvent osWait (uint32_t millisec);

#endif  // Generic Wait available


//  ==== Timer Management Functions ====

static void _osTimerCallbackFreeRTOS (xTimerHandle handle)
{
    osTimerDef_t *timer = (osTimerDef_t *)(pvTimerGetTimerID(handle));

    timer->ptimer(timer->custom->argument);
}


/// Create a timer.
/// \param[in]     timer_def     timer object referenced with \ref osTimer.
/// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer call back function.
/// \return timer ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osTimerCreate shall be consistent in every CMSIS-RTOS.
osTimerId osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *argument)
{
    timer_def->custom->argument = argument;

    return xTimerCreate((const portCHAR *)"",
                        1,  //Set later when timer is started
                        (type == osTimerPeriodic) ? pdTRUE : pdFALSE,
                        (void *)timer_def,
                        _osTimerCallbackFreeRTOS
                        );
}



/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \param[in]     millisec      time delay value of the timer.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osTimerStart shall be consistent in every CMSIS-RTOS.
osStatus osTimerStart (osTimerId timer_id, uint32_t millisec)
{
    portBASE_TYPE taskWoken = pdFALSE;
    osStatus result = osOK;
    portTickType ticks = millisec / portTICK_RATE_MS;
    if (ticks == 0) {
        ticks = 1;
    }

    if (inHandlerMode()) {
        if (xTimerChangePeriodFromISR(timer_id, ticks, &taskWoken) == pdPASS) {
            xTimerStartFromISR(timer_id, &taskWoken);
            portEND_SWITCHING_ISR(taskWoken);
        }
    }
    else {
        //TODO: add timeout support
        if (xTimerChangePeriod(timer_id, ticks, 0) != pdPASS) {
            result = osErrorOS;
        }
        else {
            if (xTimerStart(timer_id, 0) != pdPASS) {
                result = osErrorOS;
            }
        }
    }

    return result;
}



/// Stop the timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osTimerStop shall be consistent in every CMSIS-RTOS.
osStatus osTimerStop (osTimerId timer_id)
{
    portBASE_TYPE taskWoken = pdFALSE;
    osStatus result = osOK;

    if (inHandlerMode()) {
        xTimerStopFromISR(timer_id, &taskWoken);
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        if (xTimerStop(timer_id, 0) != pdPASS) {    //TODO: add timeout support
            result = osErrorOS;
        }
    }

    return result;
}


/// Delete a timer that was created by \ref osTimerCreate.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osTimerDelete shall be consistent in every CMSIS-RTOS.
osStatus osTimerDelete (osTimerId timer_id)
{
    osStatus result = osOK;
    
    /* try to delete the soft timer and wait max RTL_TIMER_API_MAX_BLOCK_TICKS 
        to send the delete command to the timer command queue */
    if (xTimerDelete(timer_id, portMAX_DELAY ) != pdPASS) {
        result = osErrorOS;
    }

    return result;
}

//  ==== Signal Management ====
#if configSignalManagementSupport
/// Set the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that should be set.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
/// \note MUST REMAIN UNCHANGED: \b osSignalSet shall be consistent in every CMSIS-RTOS.
int32_t osSignalSet (osThreadId thread_id, int32_t signals)
{
    EventGroupHandle_t event_handle;
    portBASE_TYPE taskWoken = pdFALSE;
    portBASE_TYPE xResult;
    EventBits_t uxBits_ret=0x80000000;
#ifdef CHECK_VALUE_OF_EVENT_GROUP    
    EventBits_t uxBits;
#endif    

    if (signals & (0xFFFFFFFF << osFeature_Signals)) {
        return 0x80000000;
    }

    event_handle = find_signal_by_thread(thread_id);
    if (event_handle) {
        if (inHandlerMode()) {
            uxBits_ret = xEventGroupGetBitsFromISR(event_handle);
            xResult = xEventGroupSetBitsFromISR(
                            event_handle,    /* The event group being updated. */
                            signals,         /* The bits being set. */
                            &taskWoken );
            if( xResult != pdFAIL )
            {
                portYIELD_FROM_ISR(taskWoken);
            }
        }
        else {
            uxBits_ret = xEventGroupGetBits(event_handle);
#ifdef CHECK_VALUE_OF_EVENT_GROUP                
            uxBits = 
#endif              
                      xEventGroupSetBits(
                           event_handle,    /* The event group being updated. */
                           signals );/* The bits being set. */
        }
    }

    return uxBits_ret;
}

/// Clear the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that shall be cleared.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
/// \note MUST REMAIN UNCHANGED: \b osSignalClear shall be consistent in every CMSIS-RTOS.
int32_t osSignalClear (osThreadId thread_id, int32_t signals)
{
    EventGroupHandle_t event_handle;
    //portBASE_TYPE taskWoken = pdFALSE;
    EventBits_t uxBits_ret=0x80000000;
#ifdef CHECK_VALUE_OF_EVENT_GROUP     
    EventBits_t uxBits;
#endif    

    if (signals & (0xFFFFFFFF << osFeature_Signals)) {
        return 0x80000000;
    }

    event_handle = find_signal_by_thread(thread_id);
    if (event_handle) {
        if (inHandlerMode()) {
            uxBits_ret = xEventGroupGetBitsFromISR(event_handle);
#ifdef CHECK_VALUE_OF_EVENT_GROUP                
            uxBits = 
#endif              
                      xEventGroupClearBitsFromISR(
                         event_handle,    /* The event group being updated. */
                         signals);/* The bits being cleared. */
        }
        else {
            uxBits_ret = xEventGroupGetBits(event_handle);
#ifdef CHECK_VALUE_OF_EVENT_GROUP                
            uxBits = 
#endif              
                      xEventGroupClearBits(                                          
                         event_handle,    /* The event group being updated. */
                         signals);/* The bits being cleared. */
        }
    }

    return uxBits_ret;
}


/// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
/// \param[in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return event flag information or error code.
/// \note MUST REMAIN UNCHANGED: \b osSignalWait shall be consistent in every CMSIS-RTOS.
osEvent osSignalWait (int32_t signals, uint32_t millisec)
{
    TaskHandle_t thread_id;
    EventGroupHandle_t event_handle;
    EventBits_t uxBits=0x80000000;
    osEvent   ret;
    uint32_t wait_ticks;

    if (signals & (0xFFFFFFFF << osFeature_Signals)) {
        ret.status = osErrorValue;
        ret.value.signals = 0;
        return ret;
    }
    
    thread_id = xTaskGetCurrentTaskHandle();
    event_handle = find_signal_by_thread(thread_id);
    if (event_handle) {
        if (signals == 0) {
            // if signals is 0, then wait any signal
            signals = (1 << osFeature_Signals) - 1;
        }

        wait_ticks = millisec_to_ticks(millisec);
        uxBits = xEventGroupWaitBits(
                        event_handle,   /* The event group being tested. */
                        signals, /* The bits within the event group to wait for. */
                        pdTRUE,        /* the signals should be cleared before returning. */
                        pdFALSE,       /* Don't wait for both bits, either bit will do. */
                        wait_ticks );/* Wait for either bit to be set. */
        if (uxBits == 0) {
            ret.status = millisec ? osEventTimeout : osOK;
            ret.value.signals = 0;
        }
        else {
            ret.status = osEventSignal;
            ret.value.signals = uxBits;
        }
    }
    
    return ret;
}
#else
// The older FreeRTOS version didn't support Signal Management functions

int32_t osSignalSet (osThreadId thread_id, int32_t signals) 
{
    return 0;
}

int32_t osSignalClear (osThreadId thread_id, int32_t signals)
{
    return 0;
}

osEvent osSignalWait (int32_t signals, uint32_t millisec)
{
    osEvent   ret;

    ret.status = osErrorOS;
    return ret;
}

#endif  // end of "else of #if configSignalManagementSupport

//  ==== Mutex Management ====


/// Create and Initialize a Mutex object
/// \param[in]     mutex_def     mutex definition referenced with \ref osMutex.
/// \return mutex ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osMutexCreate shall be consistent in every CMSIS-RTOS.
osMutexId osMutexCreate (const osMutexDef_t *mutex_def)
{
    (void) mutex_def;

    return xSemaphoreCreateMutex();
}



/// Wait until a Mutex becomes available
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMutexWait shall be consistent in every CMSIS-RTOS.
osStatus osMutexWait (osMutexId mutex_id, uint32_t millisec)
{
    portTickType ticks;


    if (mutex_id == NULL) {
        return osErrorParameter;
    }

    if (inHandlerMode()) {
        return osErrorISR;
    }

    ticks = millisec_to_ticks(millisec);

    if (xSemaphoreTake(mutex_id, ticks) != pdTRUE) {
        return osErrorOS;
    }

    return osOK;
}



/// Release a Mutex that was obtained by \ref osMutexWait
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMutexRelease shall be consistent in every CMSIS-RTOS.
osStatus osMutexRelease (osMutexId mutex_id)
{
    osStatus result = osOK;
    portBASE_TYPE taskWoken = pdFALSE;


    if (inHandlerMode()) {
        if (xSemaphoreGiveFromISR(mutex_id, &taskWoken) != pdTRUE) {
            result = osErrorOS;
        }
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        if (xSemaphoreGive(mutex_id) != pdTRUE) {
            result = osErrorOS;
        }
    }

    return result;
}

/// Delete a Mutex that was created by \ref osMutexCreate.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMutexDelete shall be consistent in every CMSIS-RTOS.
osStatus osMutexDelete (osMutexId mutex_id)
{
    if (!mutex_id) {
        return osErrorValue;
    }
    
	vSemaphoreDelete(mutex_id);
    return osOK;
}


//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))     // Semaphore available

/// Create and Initialize a Semaphore object used for managing resources
/// \param[in]     semaphore_def semaphore definition referenced with \ref osSemaphore.
/// \param[in]     count         number of available resources.
/// \return semaphore ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osSemaphoreCreate shall be consistent in every CMSIS-RTOS.
osSemaphoreId osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count)
{
    (void) semaphore_def;
    osSemaphoreId sema;

    if (count == 1) {
        vSemaphoreCreateBinary(sema);
        return sema;
    }

    return xSemaphoreCreateCounting(count, count);
}



/// Wait until a Semaphore token becomes available
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphore.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return number of available tokens, or -1 in case of incorrect parameters.
/// \note MUST REMAIN UNCHANGED: \b osSemaphoreWait shall be consistent in every CMSIS-RTOS.
int32_t osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
    portTickType ticks;


    if (semaphore_id == NULL) {
        return osErrorParameter;
    }

    ticks = millisec_to_ticks(millisec);

    if (inHandlerMode()) {
        return osErrorISR;
    }

    if (xSemaphoreTake(semaphore_id, ticks) != pdTRUE) {
        return osErrorOS;
    }

    return osOK;
}


/// Release a Semaphore token
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphore.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osSemaphoreRelease shall be consistent in every CMSIS-RTOS.
osStatus osSemaphoreRelease (osSemaphoreId semaphore_id)
{
    osStatus result = osOK;
    portBASE_TYPE taskWoken = pdFALSE;


    if (inHandlerMode()) {
        if (xSemaphoreGiveFromISR(semaphore_id, &taskWoken) != pdTRUE) {
            result = osErrorOS;
        }
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        if (xSemaphoreGive(semaphore_id) != pdTRUE) {
            result = osErrorOS;
        }
    }

    return result;
}


/// Delete a Semaphore that was created by \ref osSemaphoreCreate.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osSemaphoreDelete shall be consistent in every CMSIS-RTOS.
osStatus osSemaphoreDelete (osSemaphoreId semaphore_id)
{
    if (!semaphore_id) {
        return osErrorValue;
    }
    
	vSemaphoreDelete(semaphore_id);
    return osOK;
}


#endif     // Semaphore available

//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))  // Memory Pool Management available

#if 0
/// \brief Define a Memory Pool.
/// \param         name          name of the memory pool.
/// \param         no            maximum number of objects (elements) in the memory pool.
/// \param         type          data type of a single object (element).
/// \note CAN BE CHANGED: The parameter to \b osPoolDef shall be consistent but the
///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osPoolDef(name, no, type)   \
extern osPoolDef_t os_pool_def_##name;
#else                            // define the object
#define osPoolDef(name, no, type)   \
osPoolDef_t os_pool_def_##name = \
{ (no), sizeof(type), NULL };
#endif
#endif

//TODO
//This is a primitive and inefficient wrapper around the existing FreeRTOS memory management.
//A better implementation will have to modify heap_x.c!


typedef struct os_pool_cb {
    void *pool;
    uint8_t *markers;
    uint32_t pool_sz;
    uint32_t item_sz;
    uint32_t currentIndex;
} os_pool_cb_t;

#if 0
/// \brief Access a Memory Pool definition.
/// \param         name          name of the memory pool
/// \note CAN BE CHANGED: The parameter to \b osPool shall be consistent but the
///       macro body is implementation specific in every CMSIS-RTOS.
#define osPool(name) \
&os_pool_def_##name
#endif

/// Create and Initialize a memory pool
/// \param[in]     pool_def      memory pool definition referenced with \ref osPool.
/// \return memory pool ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osPoolCreate shall be consistent in every CMSIS-RTOS.
osPoolId osPoolCreate (const osPoolDef_t *pool_def)
{
    osPoolId thePool;
    int itemSize = 4 * ((pool_def->item_sz + 3) / 4);
    uint32_t i;

    /* First have to allocate memory for the pool control block. */
    thePool = pvPortMalloc(sizeof(os_pool_cb_t));
    if (thePool) {
        thePool->pool_sz = pool_def->pool_sz;
        thePool->item_sz = itemSize;
        thePool->currentIndex = 0;

        /* Memory for markers */
        thePool->markers = pvPortMalloc(pool_def->pool_sz);
        if (thePool->markers) {
            /* Now allocate the pool itself. */
            thePool->pool = pvPortMalloc(pool_def->pool_sz * itemSize);

            if (thePool->pool) {
                for (i = 0; i < pool_def->pool_sz; i++) {
                    thePool->markers[i] = 0;
                }
            }
            else {
                vPortFree(thePool->markers);
                vPortFree(thePool);
                thePool = NULL;
            }
        }
        else {
            vPortFree(thePool);
            thePool = NULL;
        }
    }

    return thePool;
}


/// Allocate a memory block from a memory pool
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \return address of the allocated memory block or NULL in case of no memory available.
/// \note MUST REMAIN UNCHANGED: \b osPoolAlloc shall be consistent in every CMSIS-RTOS.
void *osPoolAlloc (osPoolId pool_id)
{
    int dummy=0;
    void *p = NULL;
    uint32_t i;
    uint32_t index;

    if (inHandlerMode()) {
        dummy = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else {
        vPortEnterCritical();
    }

    for (i = 0; i < pool_id->pool_sz; i++) {
        index = pool_id->currentIndex + i;
        if (index >= pool_id->pool_sz) {
            index = 0;
        }

        if (pool_id->markers[index] == 0) {
            pool_id->markers[index] = 1;
            p = (void *)((uint32_t)(pool_id->pool) + (index * pool_id->item_sz));
            pool_id->currentIndex = index;
            break;
        }
    }

    if (inHandlerMode()) {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(dummy);
    }
    else {
        vPortExitCritical();
    }

    return p;
}


/// Allocate a memory block from a memory pool and set memory block to zero
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \return address of the allocated memory block or NULL in case of no memory available.
/// \note MUST REMAIN UNCHANGED: \b osPoolCAlloc shall be consistent in every CMSIS-RTOS.
void *osPoolCAlloc (osPoolId pool_id)
{
    void *p = osPoolAlloc(pool_id);

    os_memset(p, 0, pool_id->item_sz);

    return p;
}


/// Return an allocated memory block back to a specific memory pool
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \param[in]     block         address of the allocated memory block that is returned to the memory pool.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osPoolFree shall be consistent in every CMSIS-RTOS.
osStatus osPoolFree (osPoolId pool_id, void *block)
{
    int dummy=0;
    uint32_t index;

    if (pool_id == NULL) {
        return osErrorParameter;
    }

    if (block == NULL) {
        return osErrorParameter;
    }

    if (block < pool_id->pool) {
        return osErrorParameter;
    }

    index = (uint32_t)block - (uint32_t)(pool_id->pool);
    if (index % pool_id->item_sz) {
        return osErrorParameter;
    }
    index = index / pool_id->item_sz;
    if (index >= pool_id->pool_sz) {
        return osErrorParameter;
    }

    if (inHandlerMode()) {
        dummy = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else {
        vPortEnterCritical();
    }

    pool_id->markers[index] = 0;

    if (inHandlerMode()) {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(dummy);
    }
    else {
        vPortExitCritical();
    }

    return osOK;
}


#endif   // Memory Pool Management available


//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))     // Message Queues available

/// Create and Initialize a Message Queue.
/// \param[in]     queue_def     queue definition referenced with \ref osMessageQ.
/// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
/// \return message queue ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osMessageCreate shall be consistent in every CMSIS-RTOS.
osMessageQId osMessageCreate (const osMessageQDef_t *queue_def, osThreadId thread_id)
{
    (void) thread_id;

    return xQueueCreate(queue_def->queue_sz, queue_def->item_sz);
}


/// Put a Message to a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     info          message information.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMessagePut shall be consistent in every CMSIS-RTOS.
osStatus osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
    portBASE_TYPE taskWoken = pdFALSE;
    portTickType ticks;

    if (inHandlerMode()) {
        if (xQueueSendFromISR(queue_id, (const void *)info, &taskWoken) != pdTRUE) {
            return osErrorOS;
        }
        portEND_SWITCHING_ISR(taskWoken);        
    }
    else {
        ticks = millisec_to_ticks(millisec);
        
        if (xQueueSend(queue_id, (const void *)info, ticks) != pdTRUE) {
            return osErrorOS;
        }
    }

    return osOK;
}



/// Get a Message or Wait for a Message from a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return event information that includes status code.
/// \note MUST REMAIN UNCHANGED: \b osMessageGet shall be consistent in every CMSIS-RTOS.
osEvent osMessageGet (osMessageQId queue_id, uint32_t millisec)
{
    portBASE_TYPE taskWoken = pdFALSE;
    portTickType ticks;
    osEvent   retEvent;

    retEvent.def.message_id = queue_id;
    if (inHandlerMode()) {
        if (xQueueReceiveFromISR(queue_id, (void *)retEvent.value.p, &taskWoken) != pdTRUE) {
            retEvent.status = osErrorOS;
            return retEvent;
        }
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        ticks = millisec_to_ticks(millisec);
        
        if (xQueueReceive(queue_id, (void *)retEvent.value.p, ticks) != pdTRUE) {
            retEvent.status = osErrorOS;
            return retEvent;
        }
    }

    retEvent.status = osOK;
    
    return retEvent;
}

#endif     // Message Queues available


//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))     // Mail Queues available


typedef struct os_mailQ_cb {
    osMailQDef_t *queue_def;
    xQueueHandle handle;
    osPoolId pool;
} os_mailQ_cb_t;


/// Create and Initialize mail queue
/// \param[in]     queue_def     reference to the mail queue definition obtain with \ref osMailQ
/// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
/// \return mail queue ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osMailCreate shall be consistent in every CMSIS-RTOS.
osMailQId osMailCreate (const osMailQDef_t *queue_def, osThreadId thread_id)
{
    (void) thread_id;

    osPoolDef_t pool_def = {queue_def->queue_sz, queue_def->item_sz, NULL};


    /* Create a mail queue control block */
    *(queue_def->cb) = pvPortMalloc(sizeof(struct os_mailQ_cb));
    if (*(queue_def->cb) == NULL) {
        return NULL;
    }
    (*(queue_def->cb))->queue_def = (osMailQDef_t *)queue_def;

    /* Create a queue in FreeRTOS */
    (*(queue_def->cb))->handle = xQueueCreate(queue_def->queue_sz, sizeof(void *));
    if ((*(queue_def->cb))->handle == NULL) {
        vPortFree(*(queue_def->cb));
        return NULL;
    }

    /* Create a mail pool */
    (*(queue_def->cb))->pool = osPoolCreate(&pool_def);
    if ((*(queue_def->cb))->pool == NULL) {
        vQueueDelete((*(queue_def->cb))->handle);
        vPortFree(*(queue_def->cb));
        return NULL;
    }

    return *(queue_def->cb);
}



/// Allocate a memory block from a mail
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out
/// \return pointer to memory block that can be filled with mail or NULL in case error.
/// \note MUST REMAIN UNCHANGED: \b osMailAlloc shall be consistent in every CMSIS-RTOS.
void *osMailAlloc (osMailQId queue_id, uint32_t millisec)
{
    (void) millisec;
    void *p;
    uint32_t retried=0;


    if (queue_id == NULL) {
        return NULL;
    }

    do {
        p = osPoolAlloc(queue_id->pool);
        if (NULL == p) {
            // sleep a while and then try again
            if (millisec == osWaitForever) {
                osDelay(2);
            }
            else {
                if (!retried) {
                    osDelay(millisec);
                    retried = 1;
                }
                else {
                    break;
                }
            }
        }
    } while (NULL == p);
    

    return p;
}



/// Allocate a memory block from a mail and set memory block to zero
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out
/// \return pointer to memory block that can shall filled with mail or NULL in case error.
/// \note MUST REMAIN UNCHANGED: \b osMailCAlloc shall be consistent in every CMSIS-RTOS.
void *osMailCAlloc (osMailQId queue_id, uint32_t millisec)
{
//    uint32_t i;
    void *p = osMailAlloc(queue_id, millisec);

    if (p) {
        os_memset(p, 0, queue_id->queue_def->item_sz);
    }

    return p;
}



/// Put a mail to a queue
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     mail          memory block previously allocated with \ref osMailAlloc or \ref osMailCAlloc.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMailPut shall be consistent in every CMSIS-RTOS.
osStatus osMailPut (osMailQId queue_id, void *mail)
{
    portBASE_TYPE taskWoken;
    portTickType ticks=1000/portTICK_RATE_MS;   // No timeout is defined for this function, so we just wait 1 sec


    if (queue_id == NULL) {
        return osErrorParameter;
    }

    taskWoken = pdFALSE;

    if (inHandlerMode()) {
        if (xQueueSendFromISR(queue_id->handle, &mail, &taskWoken) != pdTRUE) {
            return osErrorOS;
        }
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        if (xQueueSend(queue_id->handle, &mail, ticks) != pdTRUE) {  //TODO where to get timeout value?
            return osErrorOS;
        }
    }

    return osOK;
}



/// Get a mail from a queue
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out
/// \return event that contains mail information or error code.
/// \note MUST REMAIN UNCHANGED: \b osMailGet shall be consistent in every CMSIS-RTOS.
osEvent osMailGet (osMailQId queue_id, uint32_t millisec)
{
    portBASE_TYPE taskWoken;
    portTickType ticks;
    osEvent event;

    event.def.mail_id = queue_id;

    if (queue_id == NULL) {
        event.status = osErrorParameter;
        return event;
    }

    taskWoken = pdFALSE;

    ticks = millisec_to_ticks(millisec);

    if (inHandlerMode()) {
        if (xQueueReceiveFromISR(queue_id->handle, &event.value.p, &taskWoken) == pdTRUE) {
            /* We have mail */
            event.status = osEventMail;
        }
        else {
            event.status = osOK;
        }
        portEND_SWITCHING_ISR(taskWoken);
    }
    else {
        if (xQueueReceive(queue_id->handle, &event.value.p, ticks) == pdTRUE) {
            /* We have mail */
            event.status = osEventMail;
        }
        else {
            event.status = (ticks == 0) ? osOK : osEventTimeout;
        }
    }

    return event;
}



/// Free a memory block from a mail
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     mail          pointer to the memory block that was obtained with \ref osMailGet.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osMailFree shall be consistent in every CMSIS-RTOS.
osStatus osMailFree (osMailQId queue_id, void *mail)
{
    if (queue_id == NULL) {
        return osErrorParameter;
    }

    osPoolFree(queue_id->pool, mail);

    return osOK;
}


void *calloc_freertos(size_t nelements, size_t elementSize)
{
    void *pbuf;
    uint32_t size;
    
    size = nelements*elementSize;
    pbuf = pvPortMalloc(size);
    os_memset(pbuf, 0, size);

    return pbuf;    
}
#endif  // Mail Queues available


