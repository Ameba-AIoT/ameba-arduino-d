/*
 * Realtek Semiconductor Corp.
 *
 * include/rtsisp.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INCLUDE_RTSISP_H
#define _INCLUDE_RTSISP_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <rtscamkit.h>
#include <rtsisp_def.h>

#define RTS_ISP_CMD_DATA_LIMIT		32

enum rts_isp_raw_fmt {
	rts_isp_raw_diable = 0,
	rts_isp_raw_blc_b = 0x1,
	rts_isp_raw_blc_a = 0x2,
	rts_isp_raw_lsc_a = 0x3,
	rts_isp_raw_dns_a = 0x4,
	rts_isp_raw_reserved
};

struct rts_isp_mem_rw {
	uint16_t addr;
	uint16_t length;
	uint8_t *pdata;
};

enum rts_isp_i2c_mode {
	rts_isp_i2c_mode_general = 0,
	rts_isp_i2c_mode_random = 1,
	rts_isp_i2c_mode_reserved
};

struct rts_isp_i2c_rw {
	enum rts_isp_i2c_mode mode;
	uint8_t slave_id;
	uint8_t len;
	uint8_t data[8];
};

struct rts_isp_sensor_reg {
	uint16_t addr;
	uint8_t len;
	uint8_t data[32];
};

struct rts_isp_snr_fmt {
	uint32_t width;
	uint32_t height;
	enum {
		snr_fmt_raw8 = 0,
		snr_fmt_raw10,
		snr_fmt_raw12,
		snr_fmt_yuv,
		snr_fmt_reserved
	} fmt;
};

struct rts_isp_scale_coeff {
	uint32_t scale_h;
	uint32_t scale_v;
};

struct rts_isp_zoom_start {
	uint32_t start_x;
	uint32_t start_y;
};

struct rts_isp_color_yuv {
	uint8_t y;
	uint8_t u;
	uint8_t v;
};

struct rts_isp_grid_t {
	int16_t start_x;
	int16_t start_y;
	uint16_t grid_width;
	uint16_t grid_height;
	uint8_t grid_rows;
	uint8_t grid_columns;
};

struct rts_isp_rect_t {
	int16_t start_x;
	int16_t start_y;
	int16_t end_x;
	int16_t end_y;
};

struct rts_isp_grid_bitmap {
	uint16_t number;
	uint8_t bitmap[RTS_GRID_BITMAP_SIZE];
};

struct rts_isp_md_data_fmt {
	enum {
		blc_output = 0, //raw data
		mlsc_output = 1, //raw data
		awb_output = 2, //raw data
		ccm_input = 4, //rgb data
		ccm_output = 5, //rgb data
		gamma_input = 6, //rgb data
		gamma_output = 7, //rgb data
		eeh_input = 8, //yuv422 data
		eeh_output = 9, //yuv422 data
		rgb2yuv_output = 0xc, //yuv444 data
		zoom_jpeg_output = 0x10, //yuv422 data
		zoom_full_hd_output = 0x11, //yuv422 data
		zoom_hd_output = 0x12, //yuv422 data
		zoom_vga_output = 0x13, //yuv422 data
		zoom_qvga_output = 0x14, //yuv422 data
	} data_format;

	union {
		enum {
			raw_gr = 0,
			raw_r = 1,
			raw_b = 2,
			raw_gb = 3
		} raw_data;
		enum {
			rgb_r = 0,
			rgb_g = 1,
			rgb_b = 2,
			rgb_reserved
		} rgb_data;
		enum {
			yuv_y = 0,
			yuv_u = 1,
			yuv_v = 2,
			yuv_reserved
		} yuv_data;
	} data_element;
};

struct rts_isp_device_desc {
	uint8_t length;
	uint8_t type;
	uint16_t hwversion;
	uint16_t fwversion;
	uint8_t streamnum;
	uint8_t frmivalnum;
};

struct rts_isp_fw_version_t {
	uint8_t header;
	uint8_t len;
	uint32_t magictag;
	uint16_t ic_name;
	uint16_t vid;
	uint16_t pid;
	uint32_t fw_ver;
	uint32_t cus_ver;
	uint8_t reserved[12];
};

struct rts_isp_dma_buffer {
	void *vm_addr;
	uint32_t length;
	uint32_t phy_addr;
};

struct rts_isp_iq_table_attr {
	uint8_t version;
	uint16_t addr;
	uint16_t size;
};

struct rts_isp_3a_version {
	uint16_t main;
	uint8_t sub;
};

struct rts_isp_3a_version_t {
	struct rts_isp_3a_version awb;
	struct rts_isp_3a_version ae;
	struct rts_isp_3a_version af;
};

enum rts_isp_wdr_mode {
	wdr_disable = 0,
	wdr_manual,
	wdr_auto_weak,
	wdr_auto_medium,
	wdr_auto_strong,
	wdr_reserved
};

struct rts_isp_api_version {
	uint8_t main;
	uint8_t sub;
};

typedef const void *RtsRegInst;

int rts_isp_read_xmem(int fd, struct rts_isp_mem_rw *pmem);
int rts_isp_write_xmem(int fd, struct rts_isp_mem_rw *pmem);
int rts_isp_read_i2c(int fd, struct rts_isp_i2c_rw *pi2c);
int rts_isp_write_i2c(int fd, struct rts_isp_i2c_rw *pi2c);
int rts_isp_read_sensor_reg(int fd, struct rts_isp_sensor_reg *reg);
int rts_isp_write_sensor_reg(int fd, struct rts_isp_sensor_reg *reg);

int rts_isp_get_md_count(int fd);
int rts_isp_get_md_type(int fd, uint8_t mdidx);
int rts_isp_set_md_enable(int fd, uint8_t mdidx, int enable);
int rts_isp_get_md_enable(int fd, uint8_t mdidx, int *penable);
int rts_isp_set_md_data_fmt(int fd, struct rts_isp_md_data_fmt *pfmt);
int rts_isp_get_md_data_fmt(int fd, struct rts_isp_md_data_fmt *pfmt);
int rts_isp_set_md_grid_bitmap(int fd, uint8_t mdidx, struct rts_isp_grid_bitmap *pbitmap);
int rts_isp_get_md_grid_bitmap(int fd, uint8_t mdidx, struct rts_isp_grid_bitmap *pbitmap);
int rts_isp_get_md_result(int fd, uint8_t mdidx, struct rts_isp_grid_bitmap *pbitmap);
int rts_isp_set_md_grid(int fd, unsigned int strmidx, uint8_t mdidx,
		struct rts_isp_grid_t *pgrid);
int rts_isp_get_md_grid(int fd, unsigned int strmidx, uint8_t mdidx,
		struct rts_isp_grid_t *pgrid);
int rts_isp_set_md_rect(int fd, uint8_t strmidx, uint8_t mdidx,
		struct rts_isp_rect_t *prect);
int rts_isp_get_md_rect(int fd, uint8_t strmidx, uint8_t mdidx,
		struct rts_isp_rect_t *prect);
int rts_isp_set_md_grid_absolute(int fd, uint8_t mdidx, struct rts_isp_grid_t *pgrid);
int rts_isp_get_md_grid_absolute(int fd, uint8_t mdidx, struct rts_isp_grid_t *pgrid);
int rts_isp_set_md_rect_absolute(int fd, uint8_t mdidx,
		struct rts_isp_rect_t *prect);
int rts_isp_get_md_rect_absolute(int fd, uint8_t mdidx,
		struct rts_isp_rect_t *prect);
int rts_isp_set_md_sensitivity(int fd, uint8_t mdidx, uint8_t val);
int rts_isp_get_md_sensitivity(int fd, uint8_t mdidx, uint8_t *pval);
int rts_isp_set_md_percentage(int fd, uint8_t mdidx, uint8_t val);
int rts_isp_get_md_percentage(int fd, uint8_t mdidx, uint8_t *pval);
int rts_isp_set_md_frame_interval(int fd, uint8_t mdidx, uint8_t val);
int rts_isp_get_md_frame_interval(int fd, uint8_t mdidx, uint8_t *pval);
int rts_isp_get_md_status(int fd, uint8_t mdidx);
int rts_isp_get_mask_count(int fd);
int rts_isp_set_md_grid_res(int fd, uint16_t res_width, uint16_t res_height,
		struct rts_isp_grid_t *pgrid);
int rts_isp_get_md_grid_res(int fd, uint16_t res_width, uint16_t res_height,
		struct rts_isp_grid_t *pgrid);
int rts_isp_set_md_rect_res(int fd, uint8_t mdidx, uint16_t res_width,
		uint16_t res_height, struct rts_isp_rect_t *prect);
int rts_isp_get_md_rect_res(int fd, uint8_t mdidx, uint16_t res_width,
		uint16_t res_height, struct rts_isp_rect_t *prect);
int rts_isp_get_mask_type(int fd, uint8_t maskidx);
int rts_isp_set_mask_enable(int fd, uint8_t maskidx, int enable);
int rts_isp_get_mask_enable(int fd, uint8_t maskidx, int *penable);
int rts_isp_set_mask_color(int fd, struct rts_isp_color_yuv *pcolor);
int rts_isp_get_mask_color(int fd, struct rts_isp_color_yuv *pcolor);
int rts_isp_set_mask_grid_bitmap(int fd, uint8_t maskidx, struct rts_isp_grid_bitmap *pbitmap);
int rts_isp_get_mask_grid_bitmap(int fd, uint8_t maskidx, struct rts_isp_grid_bitmap *pbitmap);
int rts_isp_set_mask_grid(int fd, unsigned int strmidx, uint8_t maskidx,
		struct rts_isp_grid_t *pgrid);
int rts_isp_get_mask_grid(int fd, unsigned int strmidx, uint8_t maskidx,
		struct rts_isp_grid_t *pgrid);
int rts_isp_set_mask_rect(int fd, uint8_t strmidx, uint8_t maskidx,
		struct rts_isp_rect_t *prect);
int rts_isp_get_mask_rect(int fd, uint8_t strmidx, uint8_t maskidx,
		struct rts_isp_rect_t *prect);
int rts_isp_set_mask_grid_absolute(int fd, uint8_t maskidx, struct rts_isp_grid_t *pgrid);
int rts_isp_get_mask_grid_absolute(int fd, uint8_t maskidx, struct rts_isp_grid_t *pgrid);
int rts_isp_set_mask_rect_absolute(int fd, uint8_t maskidx,
		struct rts_isp_rect_t *prect);
int rts_isp_get_mask_rect_absolute(int fd, uint8_t maskidx,
		struct rts_isp_rect_t *prect);
int rts_isp_set_mask_grid_res(int fd, uint16_t res_width, uint16_t res_height,
		struct rts_isp_grid_t *pgrid);
int rts_isp_get_mask_grid_res(int fd, uint16_t res_width, uint16_t res_height,
		struct rts_isp_grid_t *pgrid);
int rts_isp_set_mask_rect_res(int fd, uint8_t maskidx, uint16_t res_width,
		uint16_t res_height, struct rts_isp_rect_t *prect);
int rts_isp_get_mask_rect_res(int fd, uint8_t maskidx, uint16_t res_width,
		uint16_t res_height, struct rts_isp_rect_t *prect);

int rts_isp_get_device_des(int fd, struct rts_isp_device_desc *desc);
int rts_isp_get_fw_version(int fd, struct rts_isp_fw_version_t *pversion);

/*!
 * rts_isp_get_api_version
 *
 * @return 0 : success, minus : fail
 */
int rts_isp_get_api_version(int fd, struct rts_isp_api_version *pversion);

int rts_isp_enable_ldc(int fd, uint8_t *ptable, unsigned int length);
int rts_isp_disable_ldc(int fd);
int rts_isp_get_ldc_state(int fd);

int rts_isp_enable_temporal_denoise(int fd, int enable);
int rts_isp_get_temporal_denoise_state(int fd);

int rts_isp_set_dehaze(int fd, int enable);
int rts_isp_get_dehaze(int fd);

int rts_isp_get_snr_fmt(int fd, struct rts_isp_snr_fmt *pfmt);
int rts_isp_get_scale_coeff(int fd, uint8_t strmidx, struct rts_isp_scale_coeff *pcoeff);
int rts_isp_get_zoom_start(int fd, uint8_t strmidx, struct rts_isp_zoom_start *pzoom);


int rts_isp_set_raw(int fd,uint8_t streamid, enum rts_isp_raw_fmt raw_type);

int rts_isp_get_streamid(int fd);
int rts_isp_get_stream_count(void);

unsigned long rts_isp_get_video_phy_addr(int fd, unsigned long vm_addr);

int rts_isp_ctrl_open(void);
int rts_isp_ctrl_close(int fd);

int rts_isp_soc_attach(void);
int rts_isp_soc_detach(void);

int rts_isp_mem_open(void);
void rts_isp_mem_close(int memfd);
int rts_isp_mem_alloc(int memfd, struct rts_isp_dma_buffer *buffer);
int rts_isp_mem_free(int memfd, struct rts_isp_dma_buffer *buffer);
int rts_isp_alloc_dma(struct rts_isp_dma_buffer *buffer);
int rts_isp_free_dma(struct rts_isp_dma_buffer *buffer);

int rts_reg_close(RtsRegInst reginst);
int rts_reg_isp_open(RtsRegInst *reginst);
int rts_reg_isp_close(RtsRegInst ispreg);
int rts_reg_h264_open(RtsRegInst *reginst);
int rts_reg_h264_close(RtsRegInst h264reg);
int rts_reg_mjpg_open(RtsRegInst *reginst);
int rts_reg_mjpg_close(RtsRegInst mjpgreg);
int rts_reg_mjpg_reset(RtsRegInst mjpgreg);
uint32_t rts_reg_read_reg(RtsRegInst reginst, unsigned int offset);
void rts_reg_write_reg(RtsRegInst reginst,
		unsigned int offset, uint32_t val);
void rts_reg_set_reg_bit(RtsRegInst reginst,
		unsigned int offset, uint8_t bit_idx);
void rts_reg_clr_reg_bit(RtsRegInst reginst,
		unsigned int offset, uint8_t bit_idx);
int rts_reg_get_handle(RtsRegInst reginst);
unsigned int rts_reg_get_size(RtsRegInst reginst);

enum rts_isp_gray_mode {
	RTS_ISP_NON_GRAY_MODE = 0,
	RTS_ISP_GRAY_MODE = 1
};

#define RTS_ISP_GPIO_NUM	8
enum rts_isp_gpio_status {
	RTS_ISP_GPIO_IDLE = 0,
	RTS_ISP_GPIO_USED = 1
};

enum rts_isp_gpio_direction {
	RTS_ISP_GPIO_INPUT = 0,
	RTS_ISP_GPIO_OUTPUT = 1
};

enum rts_isp_gpio_value {
	RTS_ISP_GPIO_LOW = 0,
	RTS_ISP_GPIO_HIGH = 1
};

int rts_isp_get_gray_mode(int fd, enum rts_isp_gray_mode *gray_mode);
int rts_isp_set_gray_mode(int fd, enum rts_isp_gray_mode gray_mode);
int rts_isp_get_gpio_use_status(int fd, uint8_t gpio_idx,
		enum rts_isp_gpio_status *status);
int rts_isp_get_gpio_direction(int fd, uint8_t gpio_idx,
		enum rts_isp_gpio_direction *direction);
int rts_isp_set_gpio_direction(int fd, uint8_t gpio_idx,
		enum rts_isp_gpio_direction direction);
int rts_isp_get_gpio_value(int fd, uint8_t gpio_idx,
		enum rts_isp_gpio_value *value);
int rts_isp_set_gpio_value(int fd, uint8_t gpio_idx,
		enum rts_isp_gpio_value value);

int rts_isp_set_crop_window(int fd, int strmid, struct rts_isp_rect_t *window);
int rts_isp_get_iq_table_attr(int fd, struct rts_isp_iq_table_attr *attr);
int rts_isp_get_3a_version(int fd, struct rts_isp_3a_version_t *version);

/*!
 * rts_isp_set_wdr_mode
 *
 * mode : wdr mode
 *
 * @return 0 : success, minus : fail
 */
int rts_isp_set_wdr_mode(int fd, enum rts_isp_wdr_mode mode);

/*!
 * rts_isp_get_wdr_mode
 *
 * @return wdr mode, minus : fail
 */
int rts_isp_get_wdr_mode(int fd);

/*!
 * rts_isp_set_wdr_level
 *
 * level : wdr level, 0 ~ 100, only when wdr mode equals to manual
 *
 * @return 0 : success, minus : fail
 */
int rts_isp_set_wdr_level(int fd, int level);

/*!
 * rts_isp_get_wdr_level
 *
 * @return wdr level, minus : fail
 */
int rts_isp_get_wdr_level(int fd);

/*!
 * rts_isp_set_tpnr_level
 *
 * level : tpnr level, 0 ~ 100
 *
 * @return 0 : success, minus : fail
 */
int rts_isp_set_tpnr_level(int fd, int level);

/*!
 * rts_isp_get_tpnr_level
 *
 * @return tpnr level, minus : fail
 */
int rts_isp_get_tpnr_level(int fd);

/*!
 * rts_isp_set_ir_mode
 *
 * enable 1 : enable IR mode, 0 : disable IR mode
 *
 * @return 0 : success, minus : fail
 * */
int rts_isp_set_ir_mode(int fd, int enable);

/*!
 * rts_isp_get_ir_mode
 *
 * @return 0 : disable, 1 : enable, minus : fail
 * */
int rts_isp_get_ir_mode(int fd);

int rts_isp_get_ae_gain(int fd, struct rts_isp_ae_gain *gain);
int rts_isp_set_ae_gain(int fd, struct rts_isp_ae_gain *gain);
int rts_isp_get_ae_weight(int fd, struct rts_isp_ae_weight *weight);
int rts_isp_set_ae_weight(int fd, struct rts_isp_ae_weight *weight);
int rts_isp_get_ae_statis(int fd, struct rts_isp_ae_statis *statis);
int rts_isp_get_awb_statis(int fd, struct rts_isp_awb_statis *statis);
int rts_isp_get_awb_ct_gain(int fd, int ct, struct rts_isp_awb_ct_gain *gain);
int rts_isp_get_ae_target_delta(int fd, int8_t *delta);
int rts_isp_set_ae_target_delta(int fd, int8_t delta);

#ifdef __cplusplus
}
#endif
#endif
