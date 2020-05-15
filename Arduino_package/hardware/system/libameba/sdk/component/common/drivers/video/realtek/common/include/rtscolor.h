/*
 * Realtek Semiconductor Corp.
 *
 * libs/include/rtscolor.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _RTSCOLOR_H
#define _RTSCOLOR_H

#define CONVERT_YUV_TO_RGB(y,u,v,r,g,b)		do {\
	r = (298*(y-16) + 409*(v-128) + 128) >> 8;\
	g = (298*(y-16) - 100*(u-128) - 208*(v-128) + 128) >> 8;\
	b = (298*(y-16) + 516*(u-128) + 128) >> 8;\
	r = (r > 255) ? 255 : ((r < 0) ? 0 : r);\
	g = (g > 255) ? 255 : ((g < 0) ? 0 : g);\
	b = (b > 255) ? 255 : ((b < 0) ? 0 : b);\
} while(0)

#define CONVERT_RGB_TO_YUV(r,g,b,y,u,v)		do {\
	y = ((66*r + 129*g + 25*b + 128) >> 8) + 16;\
	u = ((-38*r - 74*g + 112*b + 128) >> 8) + 128;\
	v = ((112*r - 94*g - 18*b + 128) >> 8) + 128;\
	y = (y > 236) ? 236 : ((y < 16) ? 16 : y);\
	u = (u > 240) ? 240 : ((u < 16) ? 16 : u);\
	v = (v > 240) ? 240 : ((v < 16) ? 16 : v);\
} while(0)

typedef union {
	struct {
		unsigned char b;
		unsigned char g;
		unsigned char r;
	} rgb;
	unsigned int color;
} rts_color;

enum {
	RTS_PIX_FMT_NV12 = 0,
	RTS_PIX_FMT_NV16,
	RTS_PIX_FMT_YUYV,

	RTS_PIX_FMT_1BPP = 0x100,
	RTS_PIX_FMT_GRAY_2,
	RTS_PIX_FMT_GRAY_4,
	RTS_PIX_FMT_GRAY_8,

	RTS_PIX_FMT_RGB888 = 0x200,
	RTS_PIX_FMT_ARGB8888,

	RTS_PIX_FMT_RGBA1111,
	RTS_PIX_FMT_RGBA2222,
	RTS_PIX_FMT_RGBA5551,
	RTS_PIX_FMT_RGBA4444,
	RTS_PIX_FMT_RGBA8888,

	RTS_PIX_FMT_MJPEG = 0x300,
	RTS_PIX_FMT_H264
};

int rts_get_bit_per_pixel(int pixelfmt);

#endif
