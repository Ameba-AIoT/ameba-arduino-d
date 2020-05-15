#ifndef RTL8195B_ISP__H
#define RTL8195B_ISP__H
//RTS_CAMERA.H
#include "osdep_api.h"
#include "isp_wrap_osdep_api.h"

#define ISP_DEBUG

#ifdef ISP_DEBUG
#define rtsprintk printf
#else
#define rtsprintk 
#endif

//CAMERA_RTSOC.H
enum mcu_lock_status {
	MCU_LCK_STS_OK = 0,
	MCU_LCK_STS_ERROR = 1,
	MCU_LCK_STS_NO_MCU_DEV = 2,
	MCU_LCK_STS_RESERVED
};

#ifdef CONFIG_RTS_MCU_FW_IN_DDR
static inline int rtscam_soc_acquire_mcu_lock(unsigned int timeout)
{
	return MCU_LCK_STS_OK;
}

static inline int rtscam_soc_release_mcu_lock(void)
{
	return MCU_LCK_STS_OK;
}
#else
//int rtscam_soc_acquire_mcu_lock(unsigned int timeout);
//int rtscam_soc_release_mcu_lock(void);
#endif

#define RTSCAM_SOC_CAP_FLAG_TDNR	(1<<0)
#define RTSCAM_SOC_CAP_FLAG_LDC		(1<<1)
#define RTSCAM_SOC_CAP_FLAG_DEHAZE	(1<<2)

struct rtscam_soc_pdata {
	u32 capibility;
};
//RTS_CAMERA_SOC.H
#define RTSCAM_SOC_VIDEO_NR_B			51

#define RTSCAM_SOC_CMD_TIMEOUT_MIN		300
#define RTSCAM_SOC_CMD_TIMEOUT_MAX		5000
#define RTSCAM_SOC_CMD_TIMEOUT			RTSCAM_SOC_CMD_TIMEOUT_MIN

#define RTSCAM_SOC_CMD_DATA_REG_NUM		8
#define RTSCAM_SOC_CMD_DIRECTION_MASK		0x80

#define RTSCAM_SOC_CMD_STATUS_STATUS_BIT	31
#define RTSCAM_SOC_CMD_STATUS_RESULT_BIT	30
#define RTSCAM_SOC_CMD_STATUS_ERROR_BIT		16
#define RTSCAM_SOC_CMD_STATUS_ERROR_MASK	0xff0000

#define RTSCAM_SOC_MAX_STREAM_NUM		5
#define RTSCAM_SOC_MJPEG_STRM_IDX		(RTSCAM_SOC_MAX_STREAM_NUM - 1)
#define RTSCAM_SOC_TD_INDEX			5
#define RTSCAM_SOC_LDC_INDEX			6

#define RTSCAM_SOC_MAX_SLOT_NUM			4
#define RTSCAM_SOC_HW_SLOT_NUM			2

#define RTSCAM_SOC_FPS_UNIT			10000000

#define RTSCAM_SOC_MIN_W			320
#define RTSCAM_SOC_MIN_H			240
#define RTSCAM_SOC_STEP_W			16
#define RTSCAM_SOC_STEP_H			8

#define RTSCAM_SOC_MTD_NUM			5

#define RTSCAM_SOC_FRAME_HEADER_LENGTH		8

#define RTSCAM_SOC_ISP_FUNC_TD_BIT		0
#define RTSCAM_SOC_ISP_FUNC_LDC_BIT		1

enum rtscam_soc_cmd_data_dir {
	host_to_device = 0,
	device_to_host = 1
};

enum RTS_CPU_LOCK_MCU_MASK {
	CPU_LOCK_MCU_REQUEST = 0x1,
	CPU_LOCK_MCU_DONE = 0x2,
	CPU_LOCK_MCU_RELEASE = 0x4,
	CPU_LOCK_MCU_CACHE_MISS = 0x8,
};

enum hclk_src_type {
	HCLK_SRC_480M = 0x00,
	HCLK_SRC_288M = 0x01,
	HCLK_SRC_324M = 0x02,
	HCLK_SRC_297M = 0x03,
};

#define RTS_CMDCODE_GET_DEVICE_DES			((0x01 << 8) | 0x81)
#define RTS_CMDCODE_GET_FRAME_INTERVAL_DES		((0x01 << 8) | 0x82)
#define RTS_CMDCODE_GET_VS_FMT_DES			((0x01 << 8) | 0x83)
#define RTS_CMDCODE_GET_ISP_PROCESS_DES			((0x01 << 8) | 0x84)
#define RTS_CMDCODE_GET_CAMERA_DES			((0x01 << 8) | 0x85)
#define RTS_CMDCODE_GET_RTK_EXT_CTL_DES			((0x01 << 8) | 0x86)
#define RTS_CMDCODE_GET_HCLK_CFG_DES			((0x01 << 8) | 0x87)
#define RTS_CMDCODE_GET_FW_VERSION			((0x01 << 8) | 0x88)
#define RTS_CMDCODE_GET_SNR_POWER			((0x01 << 8) | 0x89)
#define RTS_CMDCODE_GET_API_VERSION			((0x01 << 8) | 0x8a)

#define RTS_CMDCODE_SET_FPS				((0x02 << 8) | 0x01)
#define RTS_CMDCODE_START_PREVIEW			((0x02 << 8) | 0x02)
#define RTS_CMDCODE_STOP_PREVIEW			((0x02 << 8) | 0x03)
#define RTS_CMDCODE_PAUSE_VIDEO				((0x02 << 8) | 0x05)
#define RTS_CMDCODE_RESUME_VIDEO			((0x02 << 8) | 0x06)
#define RTS_CMDCODE_SET_FORMAT				((0x02 << 8) | 0x07)

#define RTS_CMDCODE_GET_SNR_FMT				((0x06 << 8) | 0x81)
#define RTS_CMDCODE_SET_ISP_FUNC			((0x0a << 8) | 0x01)
#define RTS_CMDCODE_GET_ISP_FUNC			((0x0a << 8) | 0x81)

#define BSP_SPI_FLASH_PADDR 0
#define RTS_MTD_MCU_FW_OFFSET 0

struct rtscam_soc_device_descriptor {
	u8 length;
	u8 type;
	u16 hwversion;
	u16 fwversion;
	u8 streamnum;
	u8 frmivalnum;
};

struct rtscam_soc_frmival_descriptor {
	u8 frmivalnum;
	u32 *frmivals;
};

struct rtscam_soc_stream_format_descriptor {
	u8 streamid;
	u8 length;
	u8 type;
	u8 format;
	u16 width;
	u16 height;
};

struct rtscam_soc_unit_descriptor {
	u8 length;
	u8 type;
	u8 controlsize;
	u8 bmcontrols[32];

	unsigned int ncontrols;
	struct rtscam_video_ctrl *controls;
};

struct rtscam_soc_hclk_descriptor {
	u8 length;
	u8 type;
	u8 src_type;
	u8 ssc_flag;
};

struct rtscam_soc_fw_version_t {
	u8 header;
	u8 length;
	u32 magictag;
	u16 ic_name;
	u16 vid;
	u16 pid;
	u32 fw_ver;
	u32 cus_ver;
	u8 reserved[16];
};

struct rtscam_soc_snr_power {
	u8 power_io;
	u8 power_analog;
	u8 power_core;
};

struct rtscam_soc_api_version {
	u8 main;
	u8 sub;
};

/* A.1 Descriptor Types */
#define RTSCAM_SOC_UNDEFINED_TYPE		0x0
#define RTSCAM_SOC_DEVICE_TYPE			0x1
#define RTSCAM_SOC_VS_INTERFACE_TYPE		0x2
#define RTSCAM_SOC_ISP_PROCESSING_TYPE		0x3
#define RTSCAM_SOC_CAMERA_TYPE			0x4
#define RTSCAM_SOC_RTK_EXT_CTL_TYPE		0x5
#define RTSCAM_SOC_HCLK_CFG_TYPE		0x6

/* A.2 Video Stream Data Types */
#define RTSCAM_SOC_VS_UNDEFINED			0x0
#define RTSCAM_SOC_VS_FORMAT_UNCOMPRESSED	0x4
#define RTSCAM_SOC_VS_FRAME_UNCOMPRESSED	0x5
#define RTSCAM_SOC_VS_FORMAT_MJPEG		0x6
#define RTSCAM_SOC_VS_FRAME_MJPEG		0x7
#define RTSCAM_SOC_VS_FORMAT_H264		0x13
#define RTSCAM_SOC_VS_FRAME_H264		0x14

/* format type */
#define RTSCAM_SOC_FORMAT_TYPE_YUV420_SEMIPLANAR			0x01
#define RTSCAM_SOC_FORMAT_TYPE_YUV422_SEMIPLANAR			0x02
#define RTSCAM_SOC_FORMAT_TYPE_MJPG					0x04

/* A.4 ISP Processing Control Selectors */
#define RTSCAM_SOC_PU_CONTROL_UNDEFINED					0x00
#define RTSCAM_SOC_PU_BACKLIGHT_COMPENSATION_CONTROL			0x01
#define RTSCAM_SOC_PU_BRIGHTNESS_CONTROL				0x02
#define RTSCAM_SOC_PU_CONTRAST_CONTROL					0x03
#define RTSCAM_SOC_PU_GAIN_CONTROL					0x04
#define RTSCAM_SOC_PU_POWER_LINE_FREQUENCY_CONTROL			0x05
#define RTSCAM_SOC_PU_HUE_CONTROL					0x06
#define RTSCAM_SOC_PU_SATURATION_CONTROL				0x07
#define RTSCAM_SOC_PU_SHARPNESS_CONTROL					0x08
#define RTSCAM_SOC_PU_GAMMA_CONTROL					0x09
#define RTSCAM_SOC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL			0x0A
#define RTSCAM_SOC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL		0x0B
#define RTSCAM_SOC_PU_WHITE_BALANCE_COMPONENT_CONTROL			0x0C
#define RTSCAM_SOC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL		0x0D
#define RTSCAM_SOC_PU_DIGITAL_MULTIPLIER_CONTROL			0x0E
#define RTSCAM_SOC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL			0x0F
#define RTSCAM_SOC_PU_HUE_AUTO_CONTROL					0x10
#define RTSCAM_SOC_PU_ANALOG_VIDEO_STANDARD_CONTROL			0x11
#define RTSCAM_SOC_PU_ANALOG_LOCK_STATUS_CONTROL			0x12
#define RTSCAM_SOC_PU_CONTRAST_AUTO_CONTROL				0x13

/* A.4 Camera Control Selectors */
#define RTSCAM_SOC_CT_CONTROL_UNDEFINED					0x00
#define RTSCAM_SOC_CT_SCANNING_MODE_CONTROL				0x01
#define RTSCAM_SOC_CT_AE_MODE_CONTROL					0x02
#define RTSCAM_SOC_CT_AE_PRIORITY_CONTROL				0x03
#define RTSCAM_SOC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL			0x04
#define RTSCAM_SOC_CT_EXPOSURE_TIME_RELATIVE_CONTROL			0x05
#define RTSCAM_SOC_CT_FOCUS_ABSOLUTE_CONTROL				0x06
#define RTSCAM_SOC_CT_FOCUS_RELATIVE_CONTROL				0x07
#define RTSCAM_SOC_CT_FOCUS_AUTO_CONTROL				0x08
#define RTSCAM_SOC_CT_IRIS_ABSOLUTE_CONTROL				0x09
#define RTSCAM_SOC_CT_IRIS_RELATIVE_CONTROL				0x0A
#define RTSCAM_SOC_CT_ZOOM_ABSOLUTE_CONTROL				0x0B
#define RTSCAM_SOC_CT_ZOOM_RELATIVE_CONTROL				0x0C
#define RTSCAM_SOC_CT_PANTILT_ABSOLUTE_CONTROL				0x0D
#define RTSCAM_SOC_CT_PANTILT_RELATIVE_CONTROL				0x0E
#define RTSCAM_SOC_CT_ROLL_ABSOLUTE_CONTROL				0x0F
#define RTSCAM_SOC_CT_ROLL_RELATIVE_CONTROL				0x10
#define RTSCAM_SOC_CT_PRIVACY_CONTROL					0x11
#define RTSCAM_SOC_CT_FOCUS_SIMPLE_CONTROL				0x12
#define RTSCAM_SOC_CT_WINDOW_CONTROL					0x13
#define RTSCAM_SOC_CT_REGION_OF_INTEREST_CONTROL			0x14

/* A.6 Rtk Extended Control Selectors */
#define RTSCAM_SOC_RTK_EXT_ISP_SPECIAL_EFFECT_CTL			0x01
#define RTSCAM_SOC_RTK_EXT_EVCOM_CTL					0x02
#define RTSCAM_SOC_RTK_EXT_CTE_CTL					0x03
#define RTSCAM_SOC_RTK_EXT_AE_LOCK_CTL					0x04
#define RTSCAM_SOC_RTK_EXT_AWB_LOCK_CTL					0x05
#define RTSCAM_SOC_RTK_EXT_AF_LOCK_CTL					0x06
#define RTSCAM_SOC_RTK_EXT_LED_TORCH_CTL				0x07
#define RTSCAM_SOC_RTK_EXT_LED_FLASH_CTL				0x08
#define RTSCAM_SOC_RTK_EXT_ISO_CTL					0x09
#define RTSCAM_SOC_RTK_EXT_PHOTO_SCENEMODE_CTL				0x0A
#define RTSCAM_SOC_RTK_EXT_ROI_MODE_CTL					0x13
/* CT_REGION_OF_INTEREST_CONTROL defined in UVC1.5 */
#define RTSCAM_SOC_RTK_EXT_ROI_CTL					0x14
#define RTSCAM_SOC_RTK_EXT_3ASTS_CTL					0x15
#define RTSCAM_SOC_RTK_EXT_BURSTMODE_CTL				0x16
#define RTSCAM_SOC_RTK_EXT_STILLMODE_CTL				0x17
#define RTSCAM_SOC_RTK_EXT_STILLSETTING_CTL				0x18
#define RTSCAM_SOC_RTK_EXT_IDEAEYE_SENSITIVITY_CTL			0x19
#define RTSCAM_SOC_RTK_EXT_IDEAEYE_STATUS_CTL				0x1A
#define RTSCAM_SOC_RTK_EXT_IDEAEYE_MODE_CTL				0x1B

//RTS_CAERA_SOC_REG.H

/* register address */
#define		RTS_REG_DATA_HOST_TO_MCU_BASE			0x00000000
#define		RTS_REG_DATA0_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*0)
#define		RTS_REG_DATA1_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*1)
#define		RTS_REG_DATA2_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*2)
#define		RTS_REG_DATA3_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*3)
#define		RTS_REG_DATA4_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*4)
#define		RTS_REG_DATA5_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*5)
#define		RTS_REG_DATA6_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*6)
#define		RTS_REG_DATA7_HOST_TO_MCU			(RTS_REG_DATA_HOST_TO_MCU_BASE+4*7)
#define		RTS_REG_DATA_MCU_TO_HOST_BASE			0x00000020
#define		RTS_REG_DATA0_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*0)
#define		RTS_REG_DATA1_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*1)
#define		RTS_REG_DATA2_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*2)
#define		RTS_REG_DATA3_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*3)
#define		RTS_REG_DATA4_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*4)
#define		RTS_REG_DATA5_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*5)
#define		RTS_REG_DATA6_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*6)
#define		RTS_REG_DATA7_MCU_TO_HOST			(RTS_REG_DATA_MCU_TO_HOST_BASE+4*7)
#define		RTS_REG_CMD_HOST_TO_MCU_BASE			0x00000040
#define		RTS_REG_CMD0_HOST_TO_MCU			(RTS_REG_CMD_HOST_TO_MCU_BASE+4*0)
#define		RTS_REG_CMD1_HOST_TO_MCU			(RTS_REG_CMD_HOST_TO_MCU_BASE+4*1)
#define		RTS_REG_MCU_CMD_STATUS				0x00000048

#define		RTS_REG_ISP_IMAGE_BASE				0x00000100
#define		RTS_REG_YUV_FRAME_Y_START_ADDRESS_BASE		RTS_REG_ISP_IMAGE_BASE
#define		RTS_REG_YUV_FRAME_UV_START_ADDRESS_BASE		(RTS_REG_ISP_IMAGE_BASE+0x00000080)
#define		RTS_REG_MJPEG_FRAME_BUFFER_START_ADDRESS_BASE	(RTS_REG_ISP_IMAGE_BASE+0x00000110)
#define		RTS_REG_MJPEG_FRAME_BUFFER_LENGTH		(RTS_REG_ISP_IMAGE_BASE+0x00000130)
#define		RTS_REG_YUV_FRAME_OVERFLOW_COUNT		(RTS_REG_ISP_IMAGE_BASE+0x00000158)
#define		RTS_REG_MJPEG_FRAME_OVERFLOW_COUNT		(RTS_REG_ISP_IMAGE_BASE+0x0000015C)
#define		RTS_REG_FRAME_BUFFER_COUNT			(RTS_REG_ISP_IMAGE_BASE+0x00000160)
#define		RTS_REG_ISP_CONTROL				(RTS_REG_ISP_IMAGE_BASE+0x00000164)
#define		RTS_REG_YUV_ISP_BUF_CONFIG_BASE			(RTS_REG_ISP_IMAGE_BASE+0x0000016C)
#define		RTS_REG_YUV_ISP_Y_BUF_CONFIG_S0			(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*0)
#define		RTS_REG_YUV_ISP_UV_BUF_CONFIG_S0		(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*1)
#define		RTS_REG_YUV_ISP_Y_BUF_CONFIG_S1			(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*2)
#define		RTS_REG_YUV_ISP_UV_BUF_CONFIG_S1		(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*3)
#define		RTS_REG_YUV_ISP_Y_BUF_CONFIG_S2			(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*4)
#define		RTS_REG_YUV_ISP_UV_BUF_CONFIG_S2		(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*5)
#define		RTS_REG_YUV_ISP_Y_BUF_CONFIG_S3			(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*6)
#define		RTS_REG_YUV_ISP_UV_BUF_CONFIG_S3		(RTS_REG_YUV_ISP_BUF_CONFIG_BASE+4*7)
#define		RTS_REG_MJPEG_ISP_BUF_CONFIG			(RTS_REG_ISP_IMAGE_BASE+0x0000018C)
#define		RTS_REG_TD_ISP_BUF_CONFIG			(RTS_REG_ISP_IMAGE_BASE+0x00000190)
#define		RTS_REG_STREAM_FRAME_INTERVAL_CONFIG		(RTS_REG_ISP_IMAGE_BASE+0x00000198)
#define		RTS_REG_PTS_VALUE_CONFIG			(RTS_REG_ISP_IMAGE_BASE+0x000001A0)
#define		RTS_REG_YUV_FRAME_BUFFER_STATUS_BASE		(RTS_REG_ISP_IMAGE_BASE+0x000001A4)
#define		RTS_REG_MJPEG_STREAM_FRAME_BUFFER_STATUS	(RTS_REG_ISP_IMAGE_BASE+0x000001B4)
#define		RTS_REG_TD_BUFFER_START_ADDRESS			(RTS_REG_ISP_IMAGE_BASE+0x000001B8)
#define		RTS_REG_TD_BUFFER_LENGTH			(RTS_REG_ISP_IMAGE_BASE+0x000001BC)
#define		RTS_REG_YUV_Cur_Frame_Index_Offset		(RTS_REG_ISP_IMAGE_BASE+0x000001C0)
#define		RTS_REG_MJPEG_Cur_Frame_Index			(RTS_REG_ISP_IMAGE_BASE+0x000001D0)
#define		RTS_REG_YUV_ISP_Y_BUFFER_OVERFLOW_COUNT		(RTS_REG_ISP_IMAGE_BASE+0x000001D4)
#define		RTS_REG_YUV_ISP_UV_BUFFER_OVERFLOW_COUNT	(RTS_REG_ISP_IMAGE_BASE+0x000001D8)
#define		RTS_REG_MJPEG_TD_ISP_BUFFER_OVERFLOW_COUNT	(RTS_REG_ISP_IMAGE_BASE+0x000001DC)
#define		RTS_REG_MJPEG_FRAME_RECVED_LENGTH		(RTS_REG_ISP_IMAGE_BASE+0x000001E0)
#define 	RTS_REG_LDC_MAP_TABLE_START			(RTS_REG_ISP_IMAGE_BASE+0x000001E0)
#define 	RTS_REG_LDC_MAP_TABLE_SIZE			(RTS_REG_ISP_IMAGE_BASE+0x000001E4)
#define		RTS_REG_ISP_HEADER_ADDR				(RTS_REG_ISP_IMAGE_BASE+0x000001F0)
#define		RTS_REG_ISP_OCP_IF_DUMMY			(RTS_REG_ISP_IMAGE_BASE+0x00000200)

#define		RTS_REG_SPI_BASE				0x00000400
#define		RTS_REG_MCU_SPI_BASE_ADDR			RTS_REG_SPI_BASE
#define		RTS_REG_CPU_LOCK_MCU				(RTS_REG_SPI_BASE+0x00000004)

#define		RTS_REG_ISP_INT_BASE				0x00000500
#define		RTS_REG_INT_EN_MCU_TO_HOST			RTS_REG_ISP_INT_BASE
#define		RTS_REG_INT_EN_ISP_TO_HOST			(RTS_REG_ISP_INT_BASE+0x00000004)
#define		RTS_REG_INT_FLAG_MCU_TO_HOST			(RTS_REG_ISP_INT_BASE+0x00000008)
#define		RTS_REG_INT_FLAG_ISP_HOST			(RTS_REG_ISP_INT_BASE+0x0000000C)

//RTS_CAMERA.H

struct rtscam_video_format_xlate {
	__u8 index;
	bool is_yuv;
	__u8 rts_code;
        __u8 bpp;
};

struct rtscam_video_device {
	//struct v4l2_device v4l2_dev;
	//struct device *dev;		/*initialized by user*/

	//struct vb2_alloc_ctx    *alloc_ctx;	/*initialized by user*/
	//const struct vb2_mem_ops *mem_ops;	/*initialized by user*/
	unsigned streamnum;		/*initialized by user*/
	struct rtscam_video_stream *streams;	/*initialized by user*/
	//struct rtscam_video_ops *ops;	/*initialized by user*/
	//const char *drv_name;	/*initialized by user*/
	//const char *dev_name;	/*initialized by user*/
	int video_nr;		/*initialized by user*/
	u32 type;		/*initialized by user*/

	//struct mutex dev_lock;

	//struct mutex ctrl_lock;
	//struct list_head controls;

	int streaming_count;
	//atomic_t *memory_use; /*initialized by user*/

	//atomic_t use_count;

	unsigned long flags;
	void *priv;	/*initialized by user*/
	int initialized;	/*initialized by user*/
};

struct rtscam_video_stream {
	struct rtscam_video_device *icd;

	__u8 streamid;	/*initialized by user*/

	//struct video_device *vdev;
	//struct vb2_queue vb2_vidp;
	//struct v4l2_prio_state prio;		/* V4L2 priority state */
	//atomic_t active;

	//spinlock_t	lock;
	//struct mutex stream_lock;
	//struct mutex queue_lock;
	//struct list_head capture;

	//struct rtscam_video_format *user_formats;	/*initialized by user*/

	__u8 rts_code;
	__u32 user_format;
	__u32 user_width;
	__u32 user_height;
	__u32 user_numerator;
	__u32 user_denominator;

	__u32 bytesperline;
	__u32 sizeimage;

	unsigned long sequence;

	//atomic_t use_count;

	//struct file *memory_owner;
};

struct rtscam_soc_snr_fmt {
	u16 width;
	u16 height;
	u8 snr_fmt;
};

struct rtscam_soc_dev {
	unsigned int	base;

	unsigned long iostart;
	unsigned int iosize;

	int initialized;
	//atomic_t init_count;

	//atomic_t mcu_count;
	int mcu_state;

	//struct completion	cmd_completion;
	//struct mutex		cmd_lock;
	_Mutex cmd_lock;

	struct rtscam_soc_device_descriptor dev_desc;
	struct rtscam_soc_unit_descriptor entities[3];
	struct rtscam_soc_hclk_descriptor hclk_desc;

	struct rtscam_soc_api_version api_version;

	u32 sensor_frmival;

	struct rtscam_video_device rvdev;

	//struct rtscam_soc_slot_info slot_info[RTSCAM_SOC_MAX_STREAM_NUM];
	//struct rtscam_soc_skip_info skip_info[RTSCAM_SOC_MAX_STREAM_NUM];

	unsigned long mtd_status;

	struct rtscam_soc_fw_version_t fw_version;

	int td_enable;

	int ldc_enable;

	u32 isp_capibility;

	u32 mcu_timeout;
};

enum rtscam_size_type {
	RTSCAM_SIZE_DISCRETE = 0,
	RTSCAM_SIZE_STEPWISE = 1,
	RTSCAM_SIZE_CONTINUOUS = 2,
};

struct rtscam_frame_size {
	__u32 width;
	__u32 height;
};


enum rtscam_state {
	RTSCAM_STATE_PASSIVE		= 0,
	RTSCAM_STATE_ACTIVE		= 1
};

/* Ctrl flags */
#define RTS_CTRL_FLAG_SET_CUR			(1 << 0)
#define RTS_CTRL_FLAG_GET_CUR			(1 << 1)
#define RTS_CTRL_FLAG_GET_MIN			(1 << 2)
#define RTS_CTRL_FLAG_GET_MAX			(1 << 3)
#define RTS_CTRL_FLAG_GET_RES			(1 << 4)
#define RTS_CTRL_FLAG_GET_LEN			(1 << 5)
#define RTS_CTRL_FLAG_GET_INFO			(1 << 6)
#define RTS_CTRL_FLAG_GET_DEF			(1 << 7)
/* Control should be saved at suspend and restored at resume. */
#define RTS_CTRL_FLAG_RESTORE			(1 << 8)
#define RTS_CTRL_FLAG_AUTO_UPDATE		(1 << 9)

#define RTS_CTRL_FLAG_GET_RANGE		\
	(RTS_CTRL_FLAG_GET_CUR | RTS_CTRL_FLAG_GET_MIN | \
	 RTS_CTRL_FLAG_GET_MAX | RTS_CTRL_FLAG_GET_RES | \
	 RTS_CTRL_FLAG_GET_DEF)

#define RTS_CTRL_DATA_TYPE_RAW			0
#define RTS_CTRL_DATA_TYPE_SIGNED		1
#define RTS_CTRL_DATA_TYPE_UNSIGNED		2
#define RTS_CTRL_DATA_TYPE_BOOLEAN		3
#define RTS_CTRL_DATA_TYPE_ENUM			4
#define RTS_CTRL_DATA_TYPE_BITMASK		5

#define RTS_CTRL_DATA_CURRENT			0
#define RTS_CTRL_DATA_BACKUP			1
#define RTS_CTRL_DATA_MIN			2
#define RTS_CTRL_DATA_MAX			3
#define RTS_CTRL_DATA_RES			4
#define RTS_CTRL_DATA_DEF			5
#define RTS_CTRL_DATA_LAST			6

#define RTS_CTRL_SET_CUR			0x01
#define RTS_CTRL_GET_CUR			0x81
#define RTS_CTRL_GET_MIN			0x82
#define RTS_CTRL_GET_MAX			0x83
#define RTS_CTRL_GET_RES			0x84
#define RTS_CTRL_GET_LEN			0x85
#define RTS_CTRL_GET_INFO			0x86
#define RTS_CTRL_GET_DEF			0x87

struct rtscam_video_ctrl_info {
	__u8 index;
	__u8 unit;
	__u8 selector;
	__u8 size;
	__u32 flags;
};

struct rtscam_video_ctrl_menu {
	__u32 value;
	__u8 name[32];
};

struct rtscam_queryctrl{
  __u32 id;
  __u32 type;
  __s32 minimum;
  __s32 maximum;
  __s32 step;
  __s32 default_value;
  __u32 flags;
  __u32 info;
  __u32 len;
};

struct rtscam_control {
  __u32	id;
  __s32 value;
  __u32 type;
};

struct rtscam_soc_cmd_stru {
	__u16 cmdcode;
	__u8 index;
	__u8 length;
	__u16 param;
	__u16 addr;
	__u8 *buf;
	__u32 error_type;
};

struct rtscam_soc_stream_format{
        __u16 width;
        __u16 height;
        __u32 format;
};

int rtscam_soc_exec_command(struct rtscam_soc_dev *rsocdev,struct rtscam_soc_cmd_stru *cmd);

#define RTSCAM_S_CTRL           0
#define RTSCAM_G_CTRL           1
#define RTSCAM_QUERYCTRL        2

#define RTSCAM_L_BYTE(a)	((a) & 0xff)
#define RTSCAM_H_BYTE(a)	(((a) >> 8) & 0xff)
#define RTSCAM_L_WORD(a)	((a) & 0xffff)
#define RTSCAM_H_WORD(a)	(((a) >> 16) & 0xffff)

#endif