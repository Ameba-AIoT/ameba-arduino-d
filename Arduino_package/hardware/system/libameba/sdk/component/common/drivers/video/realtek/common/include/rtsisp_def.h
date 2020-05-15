/*
 * Realtek Semiconductor Corp.
 *
 * include/rtsisp_def.h
 *
 * Copyright (C) 2016      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INCLUDE_RTSISP_DEF_H
#define _INCLUDE_RTSISP_DEF_H
#ifdef __cplusplus
extern "C"
{
#endif

#define RTS_ISP_GRID_MAX_NUM		(1200)
#define RTS_GRID_BITMAP_SIZE		((RTS_ISP_GRID_MAX_NUM + 7) / 8)

#define RTS_ISP_AE_WINDOW_NUM		25
#define RTS_ISP_AWB_WINDOW_NUM		25

#define RTS_ISP_AE_GAIN_UNIT		256

enum rts_isp_blk_type {
	RTS_ISP_BLK_TYPE_UNDEFINED = 0,
	RTS_ISP_BLK_TYPE_RECT = 1,
	RTS_ISP_BLK_TYPE_GRID,
};

enum {
	RTS_ISP_AE_AUTO = 0,
	RTS_ISP_AE_MANUAL = 1
};

enum {
	RTS_ISP_AWB_TEMPERATURE = 0,
	RTS_ISP_AWB_AUTO = 1,
	RTS_ISP_AWB_COMPONENT
};

struct rts_isp_ae_gain {
	uint16_t analog;
	uint16_t digital;
	uint16_t isp_digital;
};

struct rts_isp_ae_weight {
	uint8_t weights[RTS_ISP_AE_WINDOW_NUM];
};

struct rts_isp_ae_statis {
	uint8_t y_mean;
	uint8_t y_means[RTS_ISP_AE_WINDOW_NUM];
	uint16_t statis[64];
};

struct rts_isp_awb_statis {
	uint8_t r_means[RTS_ISP_AWB_WINDOW_NUM];
	uint8_t g_means[RTS_ISP_AWB_WINDOW_NUM];
	uint8_t b_means[RTS_ISP_AWB_WINDOW_NUM];
};

enum rts_isp_color_temperature {
	RTS_ISP_CT_2800K = 0, /*A*/
	RTS_ISP_CT_3000K, /*U30*/
	RTS_ISP_CT_4000K, /*CWF*/
	RTS_ISP_CT_5000K, /*D50*/
	RTS_ISP_CT_6500K, /*D65*/
	RTS_ISP_CT_7500K, /*D75*/
	RTS_ISP_CT_RESERVED
};

struct rts_isp_awb_ct_gain {
	uint8_t r_gain;
	uint8_t b_gain;
};

typedef enum RS_PIXEL_FORMAT_E {

	PIXEL_FORMAT_RGB_1BPP = 0,
	PIXEL_FORMAT_RGB_1555,
} PIXEL_FORMAT_E;

typedef struct RS_BITMAP_S {
	PIXEL_FORMAT_E pixel_fmt;
	uint32_t u32Width;
	uint32_t u32Height;
	void *pData;
} BITMAP_S;

enum {
	RTS_ISP_PWR_FREQUENCY_DISABLED = 0,
	RTS_ISP_PWR_FREQUENCY_50HZ = 1,
	RTS_ISP_PWR_FREQUENCY_60HZ = 2,
	RTS_ISP_PWR_FREQUENCY_AUTO = 3
};


#ifdef __cplusplus
}
#endif
#endif

