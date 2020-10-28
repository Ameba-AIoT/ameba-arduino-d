#ifndef ISP_API_V2_H
#define ISP_API_V2_H

#define ISP_API_VER 2

#include <stdint.h>

#define HW_TIMESTAMP_BUF_SIZE  96//stream_num(3)*hw_slot_num(4)*buffer_zie(8)
//#define MAX_FPS 30 //Only support the follow fps [30 15 10 5 1]

#define	_MMF_DBG_ISP_			    0x00000004

#define ISP_PIN_SEL_S0 (0)
#define ISP_PIN_SEL_S1 (1)

#define ISP_MAX_BUFFER_COUNT 6
#define ISP_DISABLE 0
#define ISP_ENABLE  1

#define ISP_FORMAT_YUV420_SEMIPLANAR 0x01
#define ISP_FORMAT_YUV422_SEMIPLANAR 0x02
#define ISP_FORMAT_BAYER_PATTERN     0x03

//Sensor Clock setting
#define ISP_FREQ_74_25  (0)
#define ISP_FREQ_37_125 (1)
#define ISP_FREQ_54     (2)
#define ISP_FREQ_27     (3)
#define ISP_FREQ_24     (4)
#define ISP_FREQ_12     (5)

#define LDC_OFF  (0)
#define LDC_ON   (1)



extern uint32_t ConfigDebugMmfErr;
extern uint32_t ConfigDebugMmfInfo;
extern int InitExposureTimeAtNormalMode;

#define ISP_PRINTK      rt_printf
#define ISP_DBG_ERROR(...)													\
			do {															\
				if (ConfigDebugMmfErr&_MMF_DBG_ISP_)						\
					ISP_PRINTK("\n\r" ISP_ERR_PREFIX __VA_ARGS__);			\
			}while(0)
                          
#define ISP_DBG_INFO(...)													\
			do {															\
				if (ConfigDebugMmfInfo&_MMF_DBG_ISP_)						\
					ISP_PRINTK("\n\r" ISP_INFO_PREFIX __VA_ARGS__);			\
			}while(0)

// for h264 encoder (V1)
// TODO: Remove this after V2 stable
struct isp_info{
	uint32_t streamid;
	uint32_t hw_slot;
	uint32_t sw_slot;
};

#define MOTION_SIZE 5

typedef struct isp_init_cfg_s {
	int pin_idx;
	void (*motion_detection[MOTION_SIZE])(void*);
	void *motion_detection_arg[MOTION_SIZE];
	unsigned char *isp_addr;
	int clk;
	int ldc;
        int fps;
        int isp_fw_location;
        int isp_multi_sensor;
				int isp_InitExposureTime;
} isp_init_cfg_t;

typedef struct isp_cfg_s{
	int isp_id;
	int format;
	int width;
	int height;
	int fps;
	int hw_slot_num;
	int boot_mode;
	int bayer_type;
}isp_cfg_t;

typedef struct isp_info_s{
	int ddr_overflow_flag, isp_overflow_flag;
	int	ddr_overflow_cnt,uv_overflow_cnt,y_overflow_cnt;
	
	int is_stream_on;
}isp_info_t;

typedef struct isp_stream_s{
	
	void (*frame_complete)(void*);
	void *frame_complete_arg;
	void *priv;
	
	isp_cfg_t cfg;
	isp_info_t info;
}isp_stream_t;

typedef struct isp_buf_s{
	uint32_t slot_id;
	uint32_t y_addr;
	uint32_t uv_addr;
	
	uint32_t timestamp;
}isp_buf_t;

typedef struct isp_global_data_s{
	isp_stream_t* stream[3];
	uint32_t inited;		// 	0: not init , 1: inited
	
	unsigned char framerate[8];
	char* hw_ts_buf;
	
	int instance_cnt;
        
        int pin_idx;
	int clk;
	int ldc;
        int sensor_fps;
        int isp_fw_location;
        int isp_multi_sensor;
				int isp_InitExposureTime;
        
        int power_off_enable;//default sensor power off, after the streaming is all closed
}isp_global_data_t;
#define MAX_SENSOR_NUM 0X08
struct sensor_select_config{
        int     index;
        int     offset;
};
typedef struct isp_sensor_selection_s{
        struct sensor_select_config sen_sel_con[MAX_SENSOR_NUM];
}isp_sensor_selection_t;

extern int video_subsys_init(isp_init_cfg_t *ctx);

extern isp_stream_t* isp_stream_create(isp_cfg_t *cfg);
extern isp_stream_t* isp_stream_destroy(isp_stream_t* stream);
extern int isp_stream_poll(isp_stream_t* stream);
extern int isp_stream_get_status(int isp_id);
extern int isp_stream_control(isp_stream_t* stream, uint32_t cmd, uint32_t arg);
extern void isp_stream_apply(isp_stream_t* stream);
extern void isp_stream_cancel(isp_stream_t* stream);
extern void isp_stream_set_complete_callback(isp_stream_t* stream, void (*cb)(void*), void* arg);
extern void isp_stream_start(isp_stream_t* stream);
extern void isp_stream_stop(isp_stream_t* stream);

extern void isp_mcu_lock(void);
extern void isp_mcu_lock(void);
extern int video_subsys_deinit(void *parm);
//extern int video_sensor_check(int SensorName);
extern uint8_t isp_i2c_read_byte(int addr);
extern uint8_t isp_i2c_write_byte(int addr, uint8_t data);
extern uint8_t isp_power_sequence_write(int index, uint8_t data);//index: 0=reset, 1=power down, 2=power control, value:  0=low, 1=high
extern int isp_stream_power_off_config(unsigned char enable);
extern int isp_check_boot_status(void);
extern void isp_get_lost_frame_info(void *parm);



#define MODE_EXCHANGE	0		// exchange used frame and ready frame's buf.y_addr/uv_addr
#define MODE_SNAPSHOT	1		// only get ready frame from ISP
#define MODE_SKIP		2		// enqueue ready frame to ISP
#define MODE_SETUP		3		// enqueue buffer to ISP 
extern void isp_handle_buffer(isp_stream_t* stream, isp_buf_t* buf, int mode);

#endif