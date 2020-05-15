#ifndef MMF2_MISO_H
#define MMF2_MISO_H
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "mmf2_module.h"

typedef struct mm_miso_s{
	int input_cnt;
	mm_context_t *input[4];  // max 4 input

	mm_context_t *output;

	uint32_t	pause_mask;
	uint32_t   	status;
	uint32_t 	stack_size;
	uint32_t    	task_priority;
	xTaskHandle 	task;
}mm_miso_t;

extern int miso_start(mm_miso_t* miso);
extern void miso_stop(mm_miso_t* miso);
extern void miso_pause(mm_miso_t* miso, uint32_t pause_mask);
extern void miso_resume(mm_miso_t* miso);
extern void miso_ctrl(mm_miso_t* miso, uint32_t cmd, uint32_t arg1, uint32_t arg2);
extern mm_miso_t* miso_create(void);
extern mm_miso_t* miso_delete(mm_miso_t* miso);

#endif