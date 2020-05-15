#ifndef MMF2_MIMO_H
#define MMF2_MIMO_H
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "mmf2_module.h"

#include "dlist.h"

typedef struct mm_mimo_item_s
{
    // list node
    struct list_head node;
    
    // data
    uint32_t data;
    
    // ref count
    uint32_t ref_cnt;
    
    // log
    uint32_t use_log;
}mm_mimo_item_t;

typedef struct mm_mimo_queue_s{
    // list entry
    struct list_head head;
    
    // protection
     SemaphoreHandle_t lock;
}mm_mimo_queue_t;

typedef struct mm_mimo_s{
    int input_cnt;
    
    // depend on intput count
    mm_context_t *input[4];
    mm_mimo_queue_t queue[4];    
    
    int output_cnt;
    
    // depend on output count
    uint32_t	    pause_mask[4];
    mm_context_t    *output[4];     // output module context
    uint32_t        output_dep[4];  // output depend on which input, bit mask
    uint32_t	    input_mask[4];	// convert from output_dep, input referenced by which output, bit mask
    uint32_t        status[4];      
    uint32_t	    stack_size;
    uint32_t        task_priority;
    xTaskHandle     task[4];
}mm_mimo_t;

extern int mimo_start(mm_mimo_t* mimo);
extern void mimo_stop(mm_mimo_t* mimo);
extern void mimo_pause(mm_mimo_t* mimo, uint32_t pause_mask);
extern void mimo_resume(mm_mimo_t* mimo);
extern void mimo_ctrl(mm_mimo_t* mimo, uint32_t cmd, uint32_t arg1, uint32_t arg2);
extern mm_mimo_t* mimo_create(void);
extern mm_mimo_t* mimo_delete(mm_mimo_t* mimo);

#endif
