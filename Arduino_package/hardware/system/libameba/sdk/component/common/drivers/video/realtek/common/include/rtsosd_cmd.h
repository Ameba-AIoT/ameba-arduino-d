/*
 * Realtek Semiconductor Corp.
 *
 * rtsosd/rtsosd_cmd.h
 *
 * Copyright (C) 2016      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _RTSOSD_RTSOSD_CMD_H
#define _RTSOSD_RTSOSD_CMD_H
#include <stdint.h>
//#include <rtsisp_hwver.h>
#include "rtsosd.h"

#if 0
#define __OSD_PRE_CHECK()	do {\
	struct rts_isp_hw_id hw_id = rts_get_hw_id();\
	if (RTS_HW_ID_VER_RTS3901 != hw_id.hw_ver &&\
			RTS_HW_ID_VER_RLE0745 != hw_id.hw_ver) {\
		RTS_ERR("only 0745/3901 support rtsosd, pls use rtosd2\n");\
		RTS_ASSERT(0);\
	}\
} while(0)
#endif

#define RTS_ISP_CMD_SET_OSD_LIB_CHAR_ATTR	((0x08 << 8) | 0x01)
#define RTS_ISP_CMD_GET_OSD_LIB_CHAR_ATTR	((0x08 << 8) | 0x81)
#define RTS_ISP_CMD_SET_OSD_CHINESE_ADDR	((0x08 << 8) | 0x02)
#define RTS_ISP_CMD_GET_OSD_CHINESE_ADDR	((0x08 << 8) | 0x82)
#define RTS_ISP_CMD_SET_OSD_TIME_DATE_EN	((0x08 << 8) | 0x03)
#define RTS_ISP_CMD_GET_OSD_TIME_DATE_EN	((0x08 << 8) | 0x83)
#define RTS_ISP_CMD_SET_OSD_TIME_DATE_DISP_CFG	((0x08 << 8) | 0x04)
#define RTS_ISP_CMD_GET_OSD_TIME_DATE_DISP_CFG	((0x08 << 8) | 0x84)
#define RTS_ISP_CMD_SET_OSD_TIME_FMT		((0x08 << 8) | 0x05)
#define RTS_ISP_CMD_GET_OSD_TIME_FMT		((0x08 << 8) | 0x85)
#define RTS_ISP_CMD_SET_OSD_CUR_TIME		((0x08 << 8) | 0x06)
#define RTS_ISP_CMD_SET_OSD_CUR_DATE		((0x08 << 8) | 0x07)
#define RTS_ISP_CMD_SET_OSD_NEXT_DATE		((0x08 << 8) | 0x08)
#define RTS_ISP_CMD_SET_OSD_BLK_EN		((0x08 << 8) | 0x09)
#define RTS_ISP_CMD_GET_OSD_BLK_EN		((0x08 << 8) | 0x89)
#define RTS_ISP_CMD_SET_OSD_BLK_ATTR		((0x08 << 8) | 0x0a)
#define RTS_ISP_CMD_GET_OSD_BLK_ATTR		((0x08 << 8) | 0x8a)
#define RTS_ISP_CMD_WRITE_OSD_MEM		((0x08 << 8) | 0x0b)
#define RTS_ISP_CMD_READ_OSD_MEM		((0x08 << 8) | 0x8b)
#define RTS_ISP_CMD_GET_OSD_MEM_SIZE		((0x08 << 8) | 0x8c)

struct rts_isp_osd_lib_char_attr {
	uint8_t osd_char_width;
	uint8_t osd_char_height;
	enum rts_osd_char_rotation osd_char_rotation;
};

struct rts_isp_osd_timedate_enable {
	uint8_t timeenable : 1,
	        dateenable : 1;
};

struct rts_isp_osd_timedata_disp_cfg {
	uint8_t time_blkidx;
	uint8_t time_startpos;
	uint8_t time_length;
	uint8_t date_blkidx;
	uint8_t date_startpos;
	uint8_t date_length;
};

struct rts_isp_osd_time_fmt {
	enum {
		osd_hour_fmt_24 = 0,
		osd_hour_fmt_12 = 1
	} hour_fmt;
	enum {
		osd_ampm_fmt_none = 0,
		osd_ampm_fmt_n1 = 1,
		osd_ampm_fmt_n2 = 2,
		osd_ampm_fmt_n3 = 3,
		osd_ampm_fmt_p8 = 4,
		osd_ampm_fmt_p9 = 5,
		osd_ampm_fmt_p10 = 6,
		osd_ampm_fmt_p11 = 7
	} ampm_fmt;
};

struct rts_isp_osd_time_t {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

struct rts_isp_osd_date_t {
	uint16_t year;
	uint8_t month;
	uint8_t day;
};

struct rts_isp_osd_date_ex {
	uint8_t content[32];
	uint8_t length;
};

struct rts_isp_osd_blk_attr {
	struct rts_isp_rect_t rect;
	enum {
		addressing_direct = 0,
		addressing_indirect
	} addressing_method;
	uint16_t byte_addr;
	uint8_t char_width;
	uint8_t char_height;
	uint8_t h_gap : 4,
	        v_gap : 4;
	uint8_t char_num;
	uint8_t row_num;
	enum {
		bg_noshow = 0,
		bg_show = 1,
	} bg_enable;
	struct rts_isp_color_yuv bg_color;
	struct rts_isp_color_yuv char_color;
	uint8_t flick_enable : 1,
	        flick_freq : 3;
	uint8_t char_color_thd;
	uint8_t char_color_inc;
	uint8_t char_color_alpha;
	uint8_t char_stroke_enable: 1,
	        char_stroke_direct: 1;
	uint8_t char_stroke_inc;
};

int rts_isp_set_osd_lib_char_attr(int fd, uint8_t strmidx,
                                  struct rts_isp_osd_lib_char_attr *pattr);
int rts_isp_get_osd_lib_char_attr(int fd, uint8_t strmidx,
                                  struct rts_isp_osd_lib_char_attr *pattr);
int rts_isp_set_osd_chinese_addr(int fd, uint8_t strmidx, uint16_t byte_addr);
int rts_isp_get_osd_chinese_addr(int fd, uint8_t strmidx,
                                 uint16_t *pbyte_addr);
int rts_isp_set_osd_time_date_en(int fd, uint8_t strmidx,
                                 struct rts_isp_osd_timedate_enable *penable);
int rts_isp_get_osd_time_date_en(int fd, uint8_t strmidx,
                                 struct rts_isp_osd_timedate_enable *penable);
int rts_isp_set_osd_time_date_display_cfg(int fd, uint8_t strmidx,
        struct rts_isp_osd_timedata_disp_cfg *pcfg);
int rts_isp_get_osd_time_date_display_cfg(int fd, uint8_t strmidx,
        struct rts_isp_osd_timedata_disp_cfg *pcfg);
int rts_isp_set_osd_time_fmt(int fd, struct rts_isp_osd_time_fmt *pfmt);
int rts_isp_get_osd_time_fmt(int fd, struct rts_isp_osd_time_fmt *pfmt);
int rts_isp_set_osd_cur_time(int fd, struct rts_isp_osd_time_t *ptime);
int rts_isp_set_osd_cur_date_ex(int fd, struct rts_isp_osd_date_ex *pdate);
int rts_isp_set_osd_cur_date(int fd, struct rts_isp_osd_date_t *pdate);
int rts_isp_set_osd_next_date_ex(int fd, struct rts_isp_osd_date_ex *pdate);
int rts_isp_set_osd_next_date(int fd, struct rts_isp_osd_date_t *pdate);
int rts_isp_get_osd_count(int fd);
int rts_isp_set_osd_blk_enable(int fd, uint8_t strmidx, uint8_t blkidx,
                               int enable);
int rts_isp_get_osd_blk_enable(int fd, uint8_t strmidx, uint8_t blkidx,
                               int *penable);
int rts_isp_disable_osd(int fd, uint8_t strmidx);
int rts_isp_set_osd_blk_attr(int fd, uint8_t strmidx, uint8_t blkidx,
                             struct rts_isp_osd_blk_attr *pattr);
int rts_isp_get_osd_blk_attr(int fd, uint8_t strmidx, uint8_t blkidx,
                             struct rts_isp_osd_blk_attr *pattr);
int rts_isp_write_osd_mem(int fd, uint8_t strmidx,
                          struct rts_isp_mem_rw *pmem);
int rts_isp_read_osd_mem(int fd, uint8_t strmidx,
                         struct rts_isp_mem_rw *pmem);
int rts_isp_get_osd_mem_size(int fd, uint8_t strmidx, uint16_t *psize);


int rts_test();
#endif
