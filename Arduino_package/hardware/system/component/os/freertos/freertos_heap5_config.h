#ifndef __FREERTOS_HEAP5_CONFIG_H_
#define __FREERTOS_HEAP5_CONFIG_H_


/*No OS in secure world, for RTK modified FreeRTOS 10.0.1*/
#if defined(CONFIG_BUILD_SECURE) && (CONFIG_BUILD_SECURE == 1)
__WEAK void vTaskSuspendAll( void );
__WEAK BaseType_t xTaskResumeAll( void );

void vApplicationMallocFailedHook( void );
NS_ENTRY void secure_heap_init( void );
#endif	

void os_heap_init(void);

#endif /* __FREERTOS_HEAP5_CONFIG_H_ */
