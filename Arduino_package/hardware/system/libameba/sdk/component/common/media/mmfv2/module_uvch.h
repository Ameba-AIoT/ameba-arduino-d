#ifndef _MODULE_UVCH_H
#define _MODULE_UVCH_H

#include "platform_opts.h"
#if defined(CONFIG_PLATFORM_8721D) && (defined(CONFIG_USBH_UVC) || defined(REFER_USBH_UVC))

#include <stdint.h>
#include <osdep_service.h>
#include "timer_api.h"
#include "mmf2_module.h"
#include "uvc_intf.h"

#define MM_CMD_UVCH_SET_PARAMS         MM_MODULE_CMD(0x00)
#define MM_CMD_UVCH_SET_STREAMING      MM_MODULE_CMD(0x01)
#define MM_CMD_UVCH_APPLY              MM_MODULE_CMD(0x02)

#define MM_UVCH_PUMP_THREAD_STACK_SIZE 1024
#define MM_UVCH_PUMP_THREAD_PRIORITY  (tskIDLE_PRIORITY + 2)

typedef struct {
    int fmt_type;
    int width;
    int height;
    int frame_rate;
    int compression_ratio;
    int frame_buf_size;
    int ext_frame_buf;
#if UVC_BUF_DYNAMIC
    int req_buf_num;
    int req_buf_size;
#endif
} uvc_parameter_t;

typedef struct {
    void *parent;
    gtimer_t frame_timer;
    u32 frame_timer_period; // us
    u8 *frame_buffer;
    uvc_parameter_t params;
    struct uvc_context *ctx;
    _sema pump_sema;
    struct task_struct pump_task;
} uvc_ctx_t;

extern mm_module_t uvch_module;

#endif
#endif
