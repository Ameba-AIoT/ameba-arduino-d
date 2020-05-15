#ifndef MMF2_SIMO_H
#define MMF2_SIMO_H
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "dlist.h"
#include "mmf2_module.h"


typedef struct mm_simo_item_s
{
    // list node
    struct list_head node;
    
    // data
    uint32_t data;
    
    // ref count
    uint32_t ref_cnt;
    
    // log
    uint32_t use_log;
}mm_simo_item_t;

typedef struct mm_simo_queue_s{
    // list entry
    struct list_head head;
    
	// item counter
	int count;
	
    // protection
     SemaphoreHandle_t lock;
}mm_simo_queue_t;

typedef struct mm_simo_s{
    mm_context_t *input;
    
    int output_cnt;
    mm_context_t *output[4];
    
    // internal queue to handle reference count and usage log
    mm_simo_queue_t queue;
    
    uint32_t 	pause_mask;
    uint32_t    status[4];
    uint32_t	stack_size;
    uint32_t    task_priority;
    xTaskHandle task[4];
}mm_simo_t;

extern int simo_start(mm_simo_t* simo);
extern void simo_stop(mm_simo_t* simo);
extern void simo_pause(mm_simo_t* simo, uint32_t pause_mask);
extern void simo_resume(mm_simo_t* simo);
extern void simo_ctrl(mm_simo_t* simo, uint32_t cmd, uint32_t arg1, uint32_t arg2);
extern mm_simo_t* simo_create(void);
extern mm_simo_t* simo_delete(mm_simo_t* simo);

#endif