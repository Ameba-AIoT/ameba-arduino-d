#ifndef _ISP_API_V1_H_
#define _ISP_API_V1_H_

#define ISP_API_VER	1

#include "hal_api.h"
#include "rtl8195bhp_isp.h"
//#include "rtl8195bhp_isp_reg_type.h"
#include "hal_isp.h"
#include "osdep_service.h"

#include "hal.h"
#include "rtl8195bhp_isp_reg_type.h"
#include "encoder_buffer_handler.h"

#define ISP_MAX_BUFFER_COUNT 6
#define ISP_DISABLE 0
#define ISP_ENABLE  1

#define MAX_STREAM_NUM 3
#define ISP_SIMULATE 1

#define STREAM_ZERO 0
#define STREAM_ONE  1
#define STREAM_TWO  2

#define ISP_DEBUG_SHOW 1

//Flip setting

#define ISP_CT_ROLL_SET ((0x04 << 8) | 0x01)
#define ISP_CT_ROLL_GET ((0x04 << 8) | 0x81)
#define stream_normal          0
#define stream_horizontal_flip 1
#define stream_vertical_flip   2
#define stream_horiVerti_flip  3

#define ISP_FORMAT_YUV420_SEMIPLANAR 0x01
#define ISP_FORMAT_YUV422_SEMIPLANAR 0x02
#define ISP_FORMAT_BAYER_PATTERN     0x03

#define MAX_FPS 30 //Only support the follow fps [30 15 10 5 1]

#define HW_TIMESTAMP_BUF_SIZE  96//stream_num(3)*hw_slot_num(4)*buffer_zie(8)

struct isp_info{
        uint32_t streamid;
        uint32_t hw_slot;
        uint32_t sw_slot;
};

typedef struct _isp_state{
	u32 timer_1;
	u32 timer_2;
	u32 drop_frame;
	u32 isp_frame;
	u32 drop_frame_total;
	u32 isp_frame_total;
}isp_state;

struct stream_handle{
    struct enc_buf_data enc_data[ISP_MAX_BUFFER_COUNT];
    struct enc_list_handle enc_list;
    isp_state isp_state_var;
    int value;
    int width;
    int height;
    int fps;
    int streamid;
    int slot_num;
    int buff_num;
    int isp_overflow_flag;
    int y_overflow_cnt;
    int uv_overflow_cnt;
    int ddr_overflow_cnt;
    int snapshot_flag;
    int IsStreamOn;
    int format;
};


struct stream_manage_handle{
    struct stream_handle *isp_stream[MAX_STREAM_NUM];
    _sema isp_cmd_done_sema;//For common semaphore
    int initial_status;
    int isp_simulate;
    char *hw_ts_buf;
};

//The buff_num must bigger than hw slot num
int mcu_init_wait_cmd_done();

void isp_open();
void isp_initial();
//void isp_config(int streamid,int hw_slot_num,int buff_num,int fps,int width ,int height);
void isp_config(int streamid,int hw_slot_num,int buff_num,int fps,int width ,int height, int format);
void isp_start(uint8_t streamid);
void video_init();
void isp_start_stream(uint8_t streamid);
void isp_stop_stream(uint8_t streamid);
void isp_snapshot_setting(int streamid);
void isp_snapshot_buf(int streamid);
void isp_reset_parm(uint8_t streamid,uint32_t fps,uint32_t width,uint32_t height,uint32_t format);
void isp_get_slot_id_info(u8 streamid, u8 slot_id, u32 *y_addr, u32 *uv_addr);//To get the slot id address

void isp_set_flip(int a_dValue);
void isp_get_flip(int *a_pdValue);
void isp_set_brightness(int a_dValue);
void isp_get_brightness(int *a_pdValue);
void isp_set_contrast(int a_dValue);
void isp_get_contrast(int *a_pdValue);
void isp_set_hue(int a_dValue);
void isp_get_hue(int *a_pdValue);
void isp_set_saturation(int a_dValue);
void isp_get_saturation(int *a_pdValue);
void isp_set_sharpness(int a_dValue);
void isp_get_sharpness(int *a_pdValue);
void isp_set_gamma(int a_dValue);
void isp_get_gamma(int *a_pdValue);
void isp_set_gray_mode(int a_dValue);
void isp_get_gray_mode(int *a_pdValue);

void isp_set_exposure_mode(int a_dValue);
void isp_get_exposure_mode(int *a_pdValue);
void isp_set_exposure_priority(int a_dValue);
void isp_get_exposure_priority(int *a_pdValue);
void isp_set_exposure_time(int a_dValue);
void isp_get_exposure_time(int *a_pdValue);
void isp_set_zoom(int a_dValue);
void isp_get_zoom(int *a_pdValue);
void isp_set_pan_tilt(int a_dValuePan, int a_dValueTilt);
void isp_get_pan_tilt(int *a_pdValuePan, int *a_pdValueTilt);


void isp_set_AWB_ctrl(int a_dValue);
void isp_get_AWB_ctrl(int *a_pdValue);
void isp_set_WB_temp(int a_dValue);
void isp_get_WB_temp(int *a_pdValue);
void isp_set_power_line_freq(int a_dValue);
void isp_get_power_line_freq(int *a_pdValue);
void isp_set_AE_gain(int a_dValueAnalogGain, int a_dValueDigitalGain, int a_dValueISPDigitalGain);
void isp_get_AE_gain(int *a_pdValueAnalogGain, int *a_pdValueDigitalGain, int *a_pdValueISPDigitalGain);
void isp_set_WDR_mode(int a_dValue);
void isp_get_WDR_mode(int *a_pdValue);
void isp_set_WDR_level(int a_dValue);
void isp_get_WDR_level(int *a_pdValue);

int isp_get_stream_status(uint8_t streamid);

int isp_poll(int streamid, int timout_ms);
void isp_get_buffer(int streamid, struct enc_buf_data* *p_enc_data);
void isp_put_buffer(int streamid, struct enc_buf_data* enc_data);
#endif