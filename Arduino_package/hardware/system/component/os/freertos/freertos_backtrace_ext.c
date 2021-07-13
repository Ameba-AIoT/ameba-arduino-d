#include "FreeRTOS.h"
#include "task.h"

#if (tskKERNEL_VERSION_MAJOR>=10) && (tskKERNEL_VERSION_MINOR>=2)
/*
 * Minimal Task control block.
 */
typedef struct 								/* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
	volatile StackType_t	*pxTopOfStack;	/*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

	#if ( portUSING_MPU_WRAPPERS == 1 )
		xMPU_SETTINGS	xMPUSettings;		/*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
	#endif

	ListItem_t			xStateListItem;	/*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
	ListItem_t			xEventListItem;		/*< Used to reference a task from an event list. */
	UBaseType_t			uxPriority;			/*< The priority of the task.  0 is the lowest priority. */
	StackType_t			*pxStack;			/*< Points to the start of the stack. */
	char				pcTaskName[ configMAX_TASK_NAME_LEN ];/*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

	#if ( ( portSTACK_GROWTH > 0 ) || ( configRECORD_STACK_HIGH_ADDRESS == 1 ) )
		StackType_t		*pxEndOfStack;		/*< Points to the highest valid address for the stack. */
	#endif

} MinTCB_t;


uint32_t * vTaskStackAddr(void)
{
	TaskHandle_t pTask = xTaskGetCurrentTaskHandle();

    return ((MinTCB_t *)pTask)->pxStack;
}

volatile uint32_t * vTaskStackTOPAddr(void)
{
	TaskHandle_t pTask = xTaskGetCurrentTaskHandle();
	
    return ((MinTCB_t *)pTask)->pxTopOfStack;
}

uint32_t vTaskStackSize(void)
{
	TaskHandle_t pTask = xTaskGetCurrentTaskHandle();
	
    return (((MinTCB_t *)pTask)->pxEndOfStack - ((MinTCB_t *)pTask)->pxStack + 1);
}

char * vTaskName(void)
{
	TaskHandle_t pTask = xTaskGetCurrentTaskHandle();

    return ((MinTCB_t *)pTask)->pcTaskName;
}

#endif
   