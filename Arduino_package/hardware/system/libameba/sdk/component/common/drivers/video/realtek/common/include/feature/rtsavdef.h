/*
 * Realtek Semiconductor Corp.
 *
 * inc/rtsavdef.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INC_RTSAVDEF_H
#define _INC_RTSAVDEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RTS_AV_FMT_AUDIO		0x80
enum RTS_AV_FMT {
	rts_av_fmt_undefined = 0x0,
	rts_v_fmt_yuyv = 0x1,
	rts_v_fmt_yuv420planar,
	rts_v_fmt_yuv420semiplanar, //V4L2_PIX_FMT_NV12
	rts_v_fmt_yuv422semiplanar,
	rts_v_fmt_mjpeg,
	rts_v_fmt_h264,

	rts_a_fmt_audio = RTS_AV_FMT_AUDIO | 0x1,
};

enum rts_av_rotation {
	rts_av_rotation_0 = 0,
	rts_av_rotation_90r = 1,
	rts_av_rotation_90l = 2,
	rts_av_rotation_180 = 3
};

/*!
 * convert v4l2 pixelformat to rts_av_fmt
 */
enum RTS_AV_FMT rts_get_av_fmt_from_v4l2_fmt(uint32_t pixelformat);

/*!
 * convert rts_av_fmt to v4l2 pixelformat
 */
uint32_t rts_get_v4l2_fmt_from_av_fmt(enum RTS_AV_FMT fmt);

#ifdef __cplusplus
}
#endif
#endif

