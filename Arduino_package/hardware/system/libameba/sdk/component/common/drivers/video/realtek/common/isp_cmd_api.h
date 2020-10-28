#ifndef _ISP_CMD_API_H_
#define _ISP_CMD_API_H_

#define ISP_CT_ROLL_SET ((0x04 << 8) | 0x01)
#define ISP_CT_ROLL_GET ((0x04 << 8) | 0x81)

extern void isp_set_flip(int a_dValue);
extern void isp_get_flip(int *a_pdValue);
extern void isp_set_brightness(int a_dValue);
extern void isp_get_brightness(int *a_pdValue);
extern void isp_set_contrast(int a_dValue);
extern void isp_get_contrast(int *a_pdValue);
extern void isp_set_hue(int a_dValue);
extern void isp_get_hue(int *a_pdValue);
extern void isp_set_saturation(int a_dValue);
extern void isp_get_saturation(int *a_pdValue);
extern void isp_set_sharpness(int a_dValue);
extern void isp_get_sharpness(int *a_pdValue);
extern void isp_set_gamma(int a_dValue);
extern void isp_get_gamma(int *a_pdValue);
extern void isp_set_gray_mode(int a_dValue);
extern void isp_get_gray_mode(int *a_pdValue);

extern void isp_set_exposure_mode(int a_dValue);
extern void isp_get_exposure_mode(int *a_pdValue);
extern void isp_set_exposure_time(int a_dValue);
extern void isp_get_exposure_time(int *a_pdValue);
extern void isp_set_zoom(int a_dValue);
extern void isp_get_zoom(int *a_pdValue);
extern void isp_set_pan_tilt(int a_dValuePan, int a_dValueTilt);
extern void isp_get_pan_tilt(int *a_pdValuePan, int *a_pdValueTilt);


extern void isp_set_AWB_ctrl(int a_dValue);
extern void isp_get_AWB_ctrl(int *a_pdValue);
extern void isp_set_power_line_freq(int a_dValue);
extern void isp_get_power_line_freq(int *a_pdValue);
extern void isp_set_AE_gain(int a_dValueAnalogGain, int a_dValueDigitalGain, int a_dValueISPDigitalGain);
extern void isp_get_AE_gain(int *a_pdValueAnalogGain, int *a_pdValueDigitalGain, int *a_pdValueISPDigitalGain);
extern void isp_set_WDR_mode(int a_dValue);
extern void isp_get_WDR_mode(int *a_pdValue);
extern void isp_set_WDR_level(int a_dValue);
extern void isp_get_WDR_level(int *a_pdValue);
extern void isp_set_IR_mode(int a_dValue);
extern void isp_get_IR_mode(int *a_pdValue);
extern void isp_set_AE_target_delta(int a_dValue);
extern void isp_get_AE_target_delta(int *a_pdValue);

extern void isp_cmd_set_all_stream_fps(int fps);
extern void isp_cmd_stop_preview(int streamid,char power_off);
extern void isp_cmd_set_start_preview(int streamid, u32 yuv_width, u32 yuv_height);
extern void isp_cmd_set_output_format(int streamid,int format);
extern void isp_cmd_set_format(int streamid,int format);
extern void isp_cmd_set_ldc(int enable);
extern void isp_cmd_get_ldc(unsigned char *buf);
extern void isp_cmd_set_stream_fps(unsigned char *buf);
extern void isp_cmd_set_power_line_freq(int a_dValue);
extern void isp_fixed_fps(unsigned char fps);

extern void isp_set_Init_ExposureTime(unsigned int Exp,unsigned int gain);
#endif
