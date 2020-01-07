#ifndef _MMF_SOURCE_H264_FILE_H_
#define _MMF_SOURCE_H264_FILE_H_
#include "encoder_buffer_handler.h"
#include "isp_api.h"

#define CMD_ISP_STREAMID           0x10
#define CMD_ISP_HW_SLOT            0x11
#define CMD_ISP_SW_SLOT            0x12
#define CMD_ISP_HEIGHT             0x13
#define CMD_ISP_WIDTH              0x14
#define CMD_ISP_FPS                0x15
#define CMD_ISP_FORMAT             0x16
#define CMD_ISP_APPLY              0x17

struct uvc_format{
  int width;
  int height;
  int format;
  int fps;
};

struct h1v6_nv12_context {
	uint32_t width;
	uint32_t height;
	int fps;
	long index;
	int start;
        int format;
        int uvc_format;//0:yuy2 1:nv12 2:mjpg 3:jpeg
	struct isp_info isp_info_value;
};
#endif