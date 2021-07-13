#ifndef _FREERTOS_BACKTRACE_EXT_H_
#define _FREERTOS_BACKTRACE_EXT_H_



#if (tskKERNEL_VERSION_MAJOR>=10) && (tskKERNEL_VERSION_MINOR>=2)
/*
 * Minimal Task control block.
 */
uint32_t * vTaskStackAddr(void);
volatile uint32_t * vTaskStackTOPAddr(void);
uint32_t vTaskStackSize(void);
char * vTaskName(void);

#endif

#endif /* _FREERTOS_BACKTRACE_EXT_H_ */
