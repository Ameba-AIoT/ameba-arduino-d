#ifndef ISP_CMD_H
#define ISP_CMD_H

#include "FreeRTOS.h"
#include "task.h"
#include <platform_stdlib.h>
#include "freertos_service.h"
#include "osdep_service.h"
//#define ISP_DEBUG

#define _func_enter_
#define _func_exit_
#define _PASS 1

#define ISP_ENABLE  1
#define ISP_DISABLE 0

#define ISP_SUCCESSFUL 1
#define ISP_FAILED     0

#define ISP_CMD_BUF_SIZE 32
#define ISP_CMD_TIMEOUT  1000

typedef struct isp_ADAPTER _isp_adapter,*PISPADAPTER;

struct isp_cmd_data{
  	unsigned short cmdcode;
	unsigned char index;
	unsigned char length;
	unsigned short param;
	unsigned short addr;
	unsigned char *buf;
	unsigned int error_type;
};

struct isp_cmd_obj {
	_list   list;
	struct isp_cmd_data isp_cmd;
};

typedef struct _isp_cmd_context{
	struct list_head isp_cmd_list;
	struct isp_cmd_obj isp_cmd;
	_mutex cmd_mutex;
	_sema cmd_done_sema;
	_sema cmd_wakeup_sema;
        _lock	cmd_lock;
	xTaskHandle     isp_cmd_task;
	int	cmdthd_running;
	int callback_running;
	int blocked;
        int running;
}isp_cmd_context;

u8 isp_send_cmd(struct isp_cmd_data *isp_data);
//u32 isp_atcmd_deinital();
//u32 isp_atcmd_initial();
u32 isp_atcmd_deinital(void *parm);
u32 isp_atcmd_initial(void *parm);
#endif
