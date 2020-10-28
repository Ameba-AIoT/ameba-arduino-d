#ifndef _MODULE_USBD_H
#define _MODULE_USBD_H

#include <stdint.h>
#include <osdep_service.h>
#include "mmf2_module.h"

#define FORMAT_TYPE_YUY2        0
#define FORMAT_TYPE_NV12        1
#define FORMAT_TYPE_MJPEG       2
#define FORMAT_TYPE_H264        3
#define FORMAT_TYPE_BAYER       4

#define BAYER_TYPE_BEFORE_BLC   1
#define BAYER_TYPE_AFTER_BLC    2
#define BAYER_TYPE_AFTER_LSC    3
#define BAYER_TYPE_AFTER_DNO    4

struct uvc_format{
	int width;
	int height;
	int format;
	int fps;
	int state;
        int isp_format;//1:YUV420 2:YUV422 3:Bayer
        int ldc;//0:Disable 1:Enable
	int bayer_type;
	_sema uvcd_change_sema;
        int init;//It only support whether the uvc is first init 0:Not initialized 1: Initialized
};


extern mm_module_t uvcd_module;

#endif