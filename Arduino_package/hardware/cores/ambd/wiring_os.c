#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "wiring_os.h"
#include "cmsis_os.h"

extern size_t xPortGetFreeHeapSize(void);

uint32_t os_thread_create_arduino(void(* task)(const void *argument), void *argument, int priority, uint32_t stack_size) {

    osThreadDef_t thread_def;

    thread_def.pthread = task;
    thread_def.tpriority = (osPriority)priority;
    thread_def.stacksize = stack_size;
    thread_def.name = "ARDUINO";

    return (uint32_t)osThreadCreate(&thread_def, argument);
}

uint32_t os_thread_get_id_arduino(void) {
    return (uint32_t)(osThreadGetId());
}

uint32_t os_thread_terminate_arduino(uint32_t thread_id) {
    return (uint32_t)(osThreadTerminate((osThreadId)thread_id));
}

uint32_t os_thread_yield_arduino(void) {
    return (uint32_t)osThreadYield();
}

uint32_t os_thread_set_priority_arduino(uint32_t thread_id, int priority) {
    return (uint32_t)osThreadSetPriority((osThreadId)thread_id, (osPriority)priority);
}

int os_thread_get_priority_arduino(uint32_t thread_id) {
    return (int)osThreadGetPriority((osThreadId)thread_id);
}

int32_t os_signal_set_arduino(uint32_t thread_id, int32_t signals) {
    return osSignalSet((osThreadId)thread_id, signals);
}

int32_t os_signal_clear_arduino(uint32_t thread_id, int32_t signals) {
    return osSignalClear((osThreadId)thread_id, signals);
}

os_event_t os_signal_wait_arduino(int32_t signals, uint32_t millisec) {

    osEvent evt;
    os_event_t ret;

    evt = osSignalWait(signals, millisec);
    ret.status = (uint32_t)evt.status;
    ret.value.signals = evt.value.signals;
    ret.def.message_id = evt.def.message_id;

    return ret;
}

typedef void (*os_ptimer) (void const *argument);

uint32_t os_timer_create_arduino(void(* callback)(void const *argument), uint8_t isPeriodic, void *argument) {

    osTimerDef_t *pTimerDef;

    pTimerDef = (osTimerDef_t *)(malloc(sizeof(osTimerDef_t)));
    pTimerDef->ptimer = callback;
    pTimerDef->custom = (struct os_timer_custom *)(malloc(sizeof(struct os_timer_custom)));

    return (uint32_t)(osTimerCreate(pTimerDef, (isPeriodic ? osTimerPeriodic : osTimerOnce), argument));
}

uint32_t os_timer_start_arduino(uint32_t timer_id, uint32_t millisec) {
    return osTimerStart((osTimerId)timer_id, millisec);
}

uint32_t os_timer_stop_arduino(uint32_t timer_id) {
    return osTimerStop((osTimerId)timer_id);
}

uint32_t os_timer_delete_arduino(uint32_t timer_id) {

    osTimerDef_t *pTimerDef;

    pTimerDef = (osTimerDef_t *)(pvTimerGetTimerID((const TimerHandle_t)timer_id));
    free (pTimerDef->custom);
    free (pTimerDef);

    return (uint32_t)(osTimerDelete((osTimerId)timer_id));
}

uint32_t os_semaphore_create_arduino(int32_t count) {
    return (uint32_t)osSemaphoreCreate(NULL, count);
}

int32_t os_semaphore_wait_arduino(uint32_t semaphore_id, uint32_t millisec) {
    if (osSemaphoreWait((osSemaphoreId)semaphore_id, millisec) == 0) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t os_semaphore_release_arduino(uint32_t semaphore_id) {
    return (uint32_t)osSemaphoreRelease((osSemaphoreId)semaphore_id);
}

uint32_t os_semaphore_delete_arduino(uint32_t semaphore_id) {
    return (uint32_t)osSemaphoreDelete((osSemaphoreId)semaphore_id);
}

size_t os_get_free_heap_size_arduino(void) {
    return xPortGetFreeHeapSize();
}

#ifdef __cplusplus
}
#endif
