#ifndef MMF2_SISO_H
#define MMF2_SISO_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "mmf2_module.h"


typedef struct mm_siso_s{
    mm_context_t *input;
    mm_context_t *output;
    int input_port_idx;		// default is 0, can be set to 1 or 2 or 3 if source module support 2 or more output queue
    
    uint32_t    status;
	
    uint32_t	stack_size;
    uint32_t    task_priority; 
    xTaskHandle task;
}mm_siso_t;

extern int siso_start(mm_siso_t* siso);
extern void siso_stop(mm_siso_t* siso);
extern void siso_pause(mm_siso_t* siso);
extern void siso_resume(mm_siso_t* siso);
extern void siso_ctrl(mm_siso_t* siso, uint32_t cmd, uint32_t arg1, uint32_t arg2);
extern mm_siso_t* siso_create(void);
extern mm_siso_t* siso_delete(mm_siso_t* siso);

#endif