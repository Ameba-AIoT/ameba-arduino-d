#ifndef _MMF_SOURCE_UVC_H_
#define _MMF_SOURCE_UVC_H_

#include "videodev2.h"
#include "uvcvideo.h"
#include "v4l2_driver.h"
#include "uvc_intf.h"

void* uvc_mod_open(void);
void uvc_mod_close(void* ctx);
int uvc_mod_set_param(void* ctx, int cmd, int arg);
int uvc_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_UVC_SET_HEIGHT              0x10
#define CMD_UVC_SET_WIDTH               0x11
#define CMD_UVC_SET_FRAMERATE           0x12
#define CMD_UVC_SET_CPZRATIO            0x13
#define CMD_UVC_SET_FRAMETYPE           0x14
#define CMD_UVC_SET_APPLY               0x15
#define CMD_UVC_GET_STREAM_READY        0x16
#define CMD_UVC_GET_STREAM_STATUS       0x17

#endif