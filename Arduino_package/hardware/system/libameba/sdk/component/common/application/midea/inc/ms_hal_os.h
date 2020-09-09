/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: os.h
 * Introduction	: os api
 *
 * Current Version	: v0.3
 * Author			: yechuhan
 * Create Time	: 2015/08/01
 * Change Log	:  (2015/08/26) 1.rename this file  2.added the MS_HAL_ to each enum and struct
 *
 * All software, firmware and related documentation herein ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 *
 */
 
#ifndef __MS_HAL_OS_H__
#define __MS_HAL_OS_H__

/**********  include **********/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
/**********  End of include **********/


/**********  Definition & Structure **********/
/**
* @about Type by which task is referenced
**/
typedef xTaskHandle ms_hal_os_thread_t;

/**
* @about Type by which queue is referenced
**/
typedef xQueueHandle ms_hal_os_queue_t;

/**
* @about Type by which mutex is referenced
**/
typedef xSemaphoreHandle ms_hal_os_mutex_t;

/**
* @about Type by which semaphore is referenced
**/
typedef xSemaphoreHandle ms_hal_os_semaphore_t;

/**
* @about Type by which timer is referenced
**/
typedef xTimerHandle ms_hal_os_timer_t;

/**
* @about Type by which timer_tick is referenced
**/
typedef portTickType ms_hal_os_timer_tick;

typedef enum
{
    /* universal successful return result.*/
	MS_HAL_OS_RESULT_SUCCESS            = 0x00,
    /* universal error return result.*/
	MS_HAL_OS_RESULT_ERROR              = 0x01,
}ms_hal_os_result_t;

//typedef enum os_timer_reload 
typedef enum  
{
	/* The timer will be a one-shot and enter the dormant state after it expires */
	MS_HAL_OS_TIMER_ONE_SHOT,
	/* The timer will expire repeatedly with a frequency set by the TimerPeriod(ticks) parameter */
	MS_HAL_OS_TIMER_PERIODIC,
}ms_hal_os_timer_reload_t;

//typedef enum os_timer_activate 
typedef enum 
{
	/* OS_TIMER_AUTO_ACTIVATE£ºStart the timer on creation */
	MS_HAL_OS_TIMER_AUTO_ACTIVATE,
	/* OS_TIMER_NO_ACTIVATE£ºDo not start the timer on creation */
	MS_HAL_OS_TIMER_NO_ACTIVATE,
}ms_hal_os_timer_activate_t;

#define  MS_HAL_OS_WAIT_FOREVER   portMAX_DELAY
#define  MS_HAL_OS_TIME_IMMEDIATELY  0
 /*  for thread sleep and wakeup */
#define THREAD_SEMA_NUM  10

typedef struct
{
   /* the number of bytes of free heap.*/
   	uint32_t free_heap_size; 
   /* the biggest block of bytes of free heap*/
    uint32_t biggest_free_block_size;
}ms_hal_mem_info_t;

/**********  End of Definition & Structure **********/

/**********  Thread **********/
/**
* @Function	Create a new task and add it to the list of tasks that are ready to run
* @Parameter	thandle: 		Used to pass back a handle by which the created task can be referenced
*			name: 		A descriptive name for the task
*			main_func: 	Pointer to the task entry function
*			arg: 			Pointer that will be used as the parameter for the task being created
*			stack_depth: 	The size of the task stack
*			priority: 		The priority at which the task should run
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_thread_create(ms_hal_os_thread_t *thandle, const char *name,
		     void (*main_func) (void *arg), void *arg,
		     uint32_t stack_depth, int32_t priority);


/**
* @Function	Remove a task from the RTOS kernels management
* @Parameter	thandle: The handle of the task to be deleted. Passing NULL will cause the calling task to be deleted.
* @return	OS_RESULT_SUCCESS: 	delete successfully
*			OS_RESULT_ERROR:   	delete failed
**/
ms_hal_os_result_t ms_hal_os_thread_delete(ms_hal_os_thread_t *thandle);


/**
* @Function	Delay a task for a given number of ticks
* @Parameter	msecs: The amount of time, in millisecond, that the calling task should block
* @return	none
**/
void ms_hal_os_thread_sleep(int32_t msecs);


/**
* @Function	Wake up the thread that is sleep
* @Parameter	thandle: The sleep thread to be waked up
* @return	OS_RESULT_SUCCESS: 	wake successfully
*			OS_RESULT_ERROR  : 	wake failed
**/
ms_hal_os_result_t ms_hal_os_thread_wake(ms_hal_os_thread_t *thandle);


/**
* @Function	Suspend the thread
* @Parameter	thandle: Handle to the task being suspended. Passing a NULL handle will cause the calling task to be suspended
* @return	none
**/
void ms_hal_os_thread_suspend(ms_hal_os_thread_t *thandle);


/**
* @Function	Resume the thread
* @Parameter	thandle: Handle to the task being readied
* @return	none
**/
void ms_hal_os_thread_resume(ms_hal_os_thread_t *thandle);


/**********  End of Thread **********/

/**********  Queue **********/
/**
* @Function	Create a new queue instance
* @Parameter	qhandle: 		A handle to the newly created queue
*			name: 		A descriptive name for the queue
*			msgsize: 		The number of bytes each item in the queue will require
*			queue_length: The maximum number of items that the queue can contain
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_queue_create(ms_hal_os_queue_t *qhandle, const char *name, 
int32_t msgsize, int32_t queue_length);


/**
* @Function	Post an item to the back of a queue
* @Parameter	qhandle: 		The handle to the queue on which the item is to be posted
*			msg:  		A pointer to the item that is to be placed on the queue
*			msecs:	The maximum amount of time the task should block waiting for space to become available on the queue
* @return	OS_RESULT_SUCCESS: 	send successfully
*			OS_RESULT_ERROR  : 	send failed
**/
ms_hal_os_result_t ms_hal_os_queue_send(ms_hal_os_queue_t *qhandle, const void *msg,
                            uint32_t msecs);


/**
* @Function	Receive an item from a queue
* @Parameter	qhandle: 		The handle to the queue from which the item is to be received
*			msg:  		Pointer to the buffer into which the received item will be copied
*			msecs:          The maximum amount of time the task should block waiting for an item to receive
* @return	OS_RESULT_SUCCESS: 	receive successfully
*			OS_RESULT_ERROR  : 	receive failed
**/
ms_hal_os_result_t ms_hal_os_queue_recv(ms_hal_os_queue_t *qhandle, void *msg, 
uint32_t msecs);


/**
* @Function	Get the number of messages stored in a queue
* @Parameter	qhandle: A handle to the queue being queried
* @return	The number of messages available in the queue
**/
int32_t ms_hal_os_queue_get_msgs_waiting(ms_hal_os_queue_t *qhandle);


/**
* @Function	Delete a queue - freeing all the memory allocated for storing of items placed on the queue
* @Parameter	A handle to the queue to be deleted
* @return	OS_RESULT_SUCCESS: 	delete successfully
*			OS_RESULT_ERROR  : 	delete failed
**/
ms_hal_os_result_t ms_hal_os_queue_delete(ms_hal_os_queue_t *qhandle);


/**********  End of Queue **********/

/**********  Mutex **********/
/**
* @Function	Create a new mutex
* @Parameter	mhandle: A handle to the newly created mutex
*			name: 	A descriptive name for the mutex
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_mutex_create(ms_hal_os_mutex_t *mhandle, const char *name);


/**
* @Function	Get the mutex that was previously created
* @Parameter	mhandle: 	A handle to the mutex being taken
*			msecs: The time in ticks to wait for the mutex to become available
* @return	OS_RESULT_SUCCESS: 	get successfully
*			OS_RESULT_ERROR  : 	get failed
**/
ms_hal_os_result_t ms_hal_os_mutex_get(ms_hal_os_mutex_t *mhandle, uint32_t msecs);


/**
* @Function	Put the mutex that was previously created
* @Parameter	mhandle: A handle to the mutex being released
* @return	OS_RESULT_SUCCESS: 	put successfully
*			OS_RESULT_ERROR  : 	put failed
**/
ms_hal_os_result_t ms_hal_os_mutex_put(ms_hal_os_mutex_t *mhandle);


/**
* @Function	Delete the mutex that was previously created
* @Parameter	mhandle: A handle to the mutex that will be deleted
* @return	OS_RESULT_SUCCESS: 	delete successfully
*			OS_RESULT_ERROR  : 	delete failed
**/
ms_hal_os_result_t ms_hal_os_mutex_delete(ms_hal_os_mutex_t *mhandle);


/**********  End of Mutex **********/

/**********  Semaphore **********/
/**
* @Function	Create a new binary semaphore
* @Parameter	mhandle: A handle to the newly created binary semaphore
*			name: 	A descriptive name for the binary semaphore
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_semaphore_create(ms_hal_os_semaphore_t *mhandle, 
const char *name);


/**
* @Function	Create a new counting semaphore
* @Parameter	mhandle:	 	A handle to the newly created counting semaphore
*			name: 		A descriptive name for the counting semaphore
*			maxcount: 	The maximum count value that can be reached
*			initcount: 	The count value assigned to the semaphore when it is created
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_semaphore_create_counting(ms_hal_os_semaphore_t *mhandle,
		               const char *name, uint32_t maxcount,
		               uint32_t initcount);


/**
* @Function	obtain a semaphore
* @Parameter	mhandle: 	A handle to the semaphore being taken
*			msecs:  The time in ticks to wait for the semaphore to become available
* @return	OS_RESULT_SUCCESS: 	obtain successfully
*			OS_RESULT_ERROR  : 	obtain failed
**/
ms_hal_os_result_t ms_hal_os_semaphore_get(ms_hal_os_semaphore_t *mhandle, 
uint32_t msecs);


/**
* @Function	release a semaphore
* @Parameter	mhandle: A handle to the semaphore being released
* @return	OS_RESULT_SUCCESS: 	release successfully
*			OS_RESULT_ERROR  : 	release failed
**/
ms_hal_os_result_t ms_hal_os_semaphore_put(ms_hal_os_semaphore_t *mhandle);


/**
* @Function	Get the number of semaphore
* @Parameter	mhandle: A handle to the semaphore being queried
* @return	The number of semaphore available
**/
uint32_t ms_hal_os_semaphore_count_get(ms_hal_os_semaphore_t *mhandle);


/**
* @Function	Delete the semaphore that was previously created
* @Parameter	mhandle: A handle to the semaphore that will be deleted
* @return	OS_RESULT_SUCCESS: 	delete successfully
*			OS_RESULT_ERROR  : 	delete failed
**/
ms_hal_os_result_t ms_hal_os_semaphore_delete(ms_hal_os_semaphore_t *mhandle);


/**********  End of Semaphore **********/

/**********  Timer **********/
/**
* @Function	Create a new software timer instance
* @Parameter	timer_t: 	A handle to the newly created timer, NULL when error
*			name: 	A text name that is assigned to the timer
*			ticks:	The timer period that is defined in tick period
*			call_back: The function to call when the timer expires
*			cb_arg: 	An identifier that is assigned to the timer being created
*			reload: Define whether to reload timer when expires
*			activate:Define whether to start timer when it is created
* @return	OS_RESULT_SUCCESS: 	create successfully
*			OS_RESULT_ERROR  : 	create failed
**/
ms_hal_os_result_t ms_hal_os_timer_create(ms_hal_os_timer_t *timer_t, const char *name, 
							ms_hal_os_timer_tick ticks, void (*call_back) (void *), 
							void *cb_arg, ms_hal_os_timer_reload_t reload, 
							ms_hal_os_timer_activate_t activate);


/**
* @Function	Start a timer that was previously created
* @Parameter	timer_t: A handle to the newly created timer
* @return	OS_RESULT_SUCCESS: 	start successfully
*			OS_RESULT_ERROR  : 	start failed
**/
ms_hal_os_result_t ms_hal_os_timer_activate(ms_hal_os_timer_t *timer_t);


/**
* @Function	Stop a timer that was previously created
* @Parameter	timer_t: A handle to the newly created timer
* @return	OS_RESULT_SUCCESS: 	stop successfully
*			OS_RESULT_ERROR  : 	stop failed
**/
ms_hal_os_result_t ms_hal_os_timer_deactivate(ms_hal_os_timer_t *timer_t);


/**
* @Function	Change the period of a timer that was previously created
* @Parameter	timer_t: 	A handle to the newly created timer
*			ntime: 	The new period for the timer
* @return	OS_RESULT_SUCCESS: 	change successfully
*			OS_RESULT_ERROR  : 	change failed
**/
ms_hal_os_result_t ms_hal_os_timer_change(ms_hal_os_timer_t *timer_t, ms_hal_os_timer_tick ntime);


/**
* @Function	Re-start a timer that was previously created
* @Parameter	timer_t: A handle to the newly created timer
* @return	OS_RESULT_SUCCESS: 	re-start successfully
*			OS_RESULT_ERROR  : 	re-start failed
**/
ms_hal_os_result_t ms_hal_os_timer_reset(ms_hal_os_timer_t *timer_t);


/**
* @Function	Queries a timer to see if it is active or dormant
* @Parameter	timer_t: The timer being queried
* @return	OS_RESULT_SUCCESS: 	the timer is active
*			OS_RESULT_ERROR  : 	the timer is dormant
**/
ms_hal_os_result_t ms_hal_os_timer_is_active(ms_hal_os_timer_t *timer_t);


/**
* @Function	Get the ID assigned to the timer
* @Parameter	timer_t: A handle to the newly created timer
* @return	The ID assigned to the timer being queried
**/
void *ms_hal_os_timer_get_context(ms_hal_os_timer_t *timer_t);


/**
* @Function	Delete a timer that was previously created
* @Parameter	timer_t: A handle to the newly created timer
* @return	OS_RESULT_SUCCESS: 	delete successfully
*			OS_RESULT_ERROR  : 	delete failed
**/
ms_hal_os_result_t ms_hal_os_timer_delete(ms_hal_os_timer_t *timer_t);


/**********  End of Timer **********/

/**********  Memory **********/
/**
* @Function	Allocate memory
* @Parameter	size: Size of the memory to be allocated
* @return	Pointer to the allocated memory, NULL if allocation fails
**/
void *ms_hal_os_memory_alloc(int32_t size);


/**
* @Function	Free memory
* @Parameter	pmem: Pointer to the memory to be freed
* @return	OS_RESULT_SUCCESS: 	free memory successfully
*			OS_RESULT_ERROR  : 	free memory failed
**/
ms_hal_os_result_t ms_hal_os_memory_free (void *pmem);


/**
* @Function	Query memory information
* @Parameter	pmem_info: Pointer to the memory informaiton
* @return	OS_RESULT_SUCCESS: 	query successfully
*			OS_RESULT_ERROR  : 	query failed
**/
ms_hal_os_result_t ms_hal_os_memory_info (ms_hal_mem_info_t *pmem_info);


/**********  End of Memory **********/

/**********  Time Switch **********/
/**
* @Function	Convert millisecond to the number of OS ticks
* @Parameter	msecs: millisecond
* @return	the number of OS ticks
**/
uint32_t ms_hal_os_msec_to_ticks(uint32_t msecs);


/**
* @Function	Convert the number of OS ticks to millisecond
* @Parameter	ticks: the number of OS ticks
* @return	millisecond
**/
uint32_t ms_hal_os_ticks_to_msec(uint32_t ticks);


/**
* @Function	Get the current OS ticks that will never overflow
* @Parameter	none
* @return	The current OS ticks
**/
uint32_t ms_hal_os_tickcount_get(void);


/**********  End of Time Switch **********/

/**********  Kernel Control **********/
/**
* @Function	Force a context switch
* @Parameter	none
* @return	none
**/
void ms_hal_os_context_force_switch(void);


/**
* @Function	Enter critical section
* @Parameter	none
* @return	none
**/
void ms_hal_os_enter_critical_section(void);


/**
* @Function	Exit critical section
* @Parameter	none
* @return	none
**/
void ms_hal_os_exit_critical_section(void);


/**
* @Function	Disable all maskable interrupts
* @Parameter	none
* @return	none
**/
void ms_hal_os_disable_all_interrupts(void);


/**
* @Function	Enable microcontroller interrupts
* @Parameter	none
* @return	none
**/
void ms_hal_os_enable_all_interrupts(void);


/**
* @Function	Start the RTOS scheduler
* @Parameter	none
* @return	none
**/
void ms_hal_os_scheduler_start(void);


/**
* @Function	Stop the RTOS scheduler
* @Parameter	none
* @return	none
**/
void ms_hal_os_scheduler_stop(void);


/**
* @Function	Suspends the scheduler without disabling interrupts
* @Parameter	none
* @return	none
**/
void ms_hal_os_scheduler_suspend(void);


/**
* @Function	Resume the RTOS scheduler
* @Parameter	none
* @return	none
**/
void ms_hal_os_scheduler_resume(void);

/**
* @Function	Get Realtek WIFI2.0 Version
* @Parameter sVersion ( Min Size 20)
* @return	none
**/
void ms_hal_os_version(char* sVersion);


/**********  End of Kernel Control **********/
#endif /* __MS_HAL_OS_H__ */

