#ifndef _VOE_API_H_
#define _VOE_API_H_
#include "osdep_service.h"
#if 0
typedef struct _h1v6_parm{
	//_sema sem_irq_h264;
        _sema sem_irq_cmd;
	_sema sem_irq_jpeg;
        _sema sem_irq_h264;
	//_sema sem_irq_cmd;
	int sem_status;
}h1v6_parm;
#endif
#if 1
typedef struct _h1v6_parm{
	_sema sem_irq_h264;
	_sema sem_irq_jpeg;
	_sema sem_irq_cmd;
        _sema sem_res_h264;
	_sema sem_res_jpeg;
	_sema sem_res_all;
}h1v6_parm;
#endif
u32 voe_run(void);
#endif