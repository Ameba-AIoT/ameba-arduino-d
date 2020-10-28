#ifndef USBD_UVC_DESC_H
#define USBD_UVC_DESC_H

#include "core/inc/usb_config.h"
#include "core/inc/usb_composite.h"
#include "osdep_service.h"

#include "uvc_os_wrap_via_osdep_api.h"
#include "usb.h"
#include "basic_types.h"
#include "video.h"

#define DBG_8195A printf

#define VALUE_FPS(fps) (10000000/fps)

#define FORMAT_TYPE_YUY2        0
#define FORMAT_TYPE_NV12        1
#define FORMAT_TYPE_MJPEG       2
#define FORMAT_TYPE_H264        3


#define UVC_INTF_CONTROL        0
#define UVC_INTF_STREAMING      1

#define UVC_EVENT_FIRST			0//(V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_CONNECT		1//(V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_DISCONNECT		2//(V4L2_EVENT_PRIVATE_START + 1)
#define UVC_EVENT_STREAMON		3//(//V4L2_EVENT_PRIVATE_START + 2)
#define UVC_EVENT_STREAMOFF		4//(V4L2_EVENT_PRIVATE_START + 3)
#define UVC_EVENT_SETUP			5//(V4L2_EVENT_PRIVATE_START + 4)
#define UVC_EVENT_DATA			6//(V4L2_EVENT_PRIVATE_START + 5)
#define UVC_EVENT_LAST			7//(V4L2_EVENT_PRIVATE_START + 5)

#define USBD_UVC_DEBUG          0

#if	USBD_UVC_DEBUG
#define USBD_PRINTF(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_ERROR(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_WARN(fmt, args...)		            	DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
//#define FUN_ENTER                                   DBG_8195A("\n\r%s ==>\n", __func__)
//#define FUN_EXIT                                    DBG_8195A("\n\r%s <==\n", __func__)
//#define FUN_TRACE                                   DBG_8195A("\n\r%s:%d \n", __func__, __LINE__)
#else
#define USBD_PRINTF(fmt, args...)
#define USBD_ERROR(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_WARN(fmt, args...)
#define FUN_ENTER
#define FUN_EXIT
#define FUN_TRACE
#endif

//structure predefine
struct uvc_dev;
struct uvc_video;

#define UVC_CMD_THREAD_PRIORITY	2

//#define UVC_NUM_REQUESTS 4

#define UVC_NUM_REQUESTS			2
#define UVC_MAX_REQUEST_SIZE			64
#define UVC_MAX_EVENTS				4

//Tuning tool
#define VendorCommand   0X0A
#define VendorData      0X0B
#define VendorLength    0X08



//#define UVC_VC_PROCESS_UNIT     0X02
//#define UVC_VC_EXTENSION_UNIT   0X03

//Tuning tool structure

struct uvc_frame_info
{
	unsigned int width;
	unsigned int height;
	unsigned int intervals[4];
};

struct uvc_format_info
{
	unsigned int fcc;
	//const struct uvc_frame_info *frames;
        struct uvc_frame_info *frames;
};


#define MAKE_SUBCMD(data,dir,subcmd) ((u8)(data)<<7|(u8)(dir)<<6|subcmd)

typedef struct _UvcVendorCmd{
	unsigned char cmd;
	unsigned char subcmd;
	unsigned short address;
	unsigned short length;
	unsigned short reserved;
	unsigned char buffer[8];
}UvcVendorCmd;

typedef struct _process_unit{
	int cur;//0X81
	int min;//0X82
	int max;//0X83
	int res;//0X84
	int len;//0X85
	int info;//0X86
	int def;//0X87
}process_unit;

typedef struct _isp_usbd_cmd_data{
  	u16 cmdcode;
	u8 index;
	u8 length;
	u16 param;
	u16 addr;
	u8 buf[0x40];//The maximum buffer size
        u32 offset;//to recode the index for read and write
	//u32 error_type;
}isp_usbd_cmd_data;

struct usbd_uvc_buffer {
        struct list_head buffer_list;
	unsigned char *mem;
	unsigned int bytesused;
        void *exbuf;
};

struct uvc_video
{
	struct usb_ep *ep;
        struct usbd_uvc_buffer uvc_buffer;

	/* Frame parameters */
	u8 bpp;
	u32 fcc;
	unsigned int width;
	unsigned int height;
	unsigned int imagesize;
        unsigned int frm_type;
        

        void (*encode)(struct usb_iso_request *req, struct uvc_video *video,struct usbd_uvc_buffer *buf);
        

	/* Requests */
	unsigned int req_size;

	struct usb_iso_request *req;

	__u8 *req_buffer[UVC_NUM_REQUESTS];
	//struct list_head req_free;
	//spinlock_t req_lock;


	/* Context data used by the completion handler */
	__u32 payload_size;
	__u32 max_payload_size;

	//struct uvc_video_queue queue;
	unsigned int    fid;
        unsigned int    format;
        unsigned int    buf_used;
        
        // payload list
        struct list_head input_queue;
        struct list_head output_queue;
        _mutex input_lock;
        _mutex output_lock;
        _sema output_queue_sema;
};

struct uvc_request_data
{
	__s32 length;
	__u8 data[60];
};

struct uvc_req_data {
	__u32				type;
	union {
		struct usb_ctrlrequest req;
                struct uvc_request_data uvc_data;
		unsigned char buf[64];
	};
	_list	list;
};



struct uvc_common{
	struct msc_dev	*mscdev;

	struct usb_gadget	*gadget;
	struct usb_ep		*ep0;		
	struct usb_request	*req0;		/* for control responses */
	unsigned int 		running;
        /* bolk cmd data*/
	_mutex  bod_mutex;
	_list	bod_list;
};

//typedef void*	_lock;

struct uvc_dev{
	struct uvc_common 	*common;
        struct uvc_video        video;
        
        struct uvc_streaming_control probe;
	struct uvc_streaming_control commit;
	
	u16 		interface_number;
	u8			config;

	struct usb_ep	*in_ep;
	struct usb_ep	*out_ep;
	//unsigned int	bulk_in_enabled:1;
	//unsigned int	bulk_out_enabled:1;
        struct usb_request *control_req;
	void *control_buf;

        //const struct usb_endpoint_descriptor *in, *out, *status;
        // lock is held when accessing usb
        struct usb_function	func;
        _lock lock;
        struct task_struct uvc_CmdTask;
        struct usb_ctrlrequest req_ctrl;
        struct uvc_req_data    req_data;
        /* Events */
	unsigned int event_length;
	unsigned int event_setup_out : 1;
        int control;//control selector
        int command_interface;//Interface and entiny 0x00 control 0x01 streaming &0xff
        int command_entity;//0x02 process unit 0x03 extension unit >>8&0xff
        unsigned char result[64];
        struct uvc_req_data  *exten_temp_data;
        //UvcVendorCmd tuning_data;
        isp_usbd_cmd_data isp_data;
        void (*change_parm_cb)(void*);
};

/*-------------------------------------------------------------------------*/

/* module parameters specific to the Video streaming endpoint */
static unsigned int streaming_interval = 1;

static unsigned int streaming_maxpacket = 1000;//1024;

static unsigned int streaming_maxburst;
/* --------------------------------------------------------------------------
 * Function descriptors
 */

/* string IDs are assigned dynamically */

#define UVC_STRING_CONTROL_IDX			0
#define UVC_STRING_STREAMING_IDX		1

#define UVC_INTF_VIDEO_CONTROL			0
#define UVC_INTF_VIDEO_STREAMING		1

#define UVC_STATUS_MAX_PACKET_SIZE		16	/* 16 bytes status */

/////////////////////////////////////////////////////

#define WEBCAM_VENDOR_ID		0x1d6b	/* Linux Foundation */
#define WEBCAM_PRODUCT_ID		0x0102	/* Webcam A/V gadget */
#define WEBCAM_DEVICE_BCD		0x0010	/* 0.10 */

#define USB_CLASS_MISC                  0xef
#define CONFIG_USB_GADGET_VBUS_DRAW     100


#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_SERIALNUMBER		3
#define STRING_INTERFACE		4
#define STRING_UVC			5


#define DRIVER_DESC	"USB UVC CLASS"
#define DRIVER_VERSION	"Feb 2016"

#define MANUFACTURER 	"Realtek"

static char webcam_config_label[] = "Video";

static char	string_manufacturer [50] = MANUFACTURER;
static char	string_product [40] = DRIVER_DESC;
static char	string_serial [20] = "0123456789";

static struct usb_string		
webcam_strings [] = {
	{ STRING_MANUFACTURER,	string_manufacturer, },        
	{ STRING_PRODUCT,	string_product, },
	{ STRING_SERIALNUMBER,	string_serial, },
	{ STRING_INTERFACE,		"USB UVC Interface", },
	{ STRING_UVC,		"USB UVC", },
        NULL,
};

static struct usb_gadget_strings webcam_stringtab = {
	.language = 0x0409,	/* en-us */
	.strings = webcam_strings,
};

static struct usb_gadget_strings *webcam_device_strings[] = {
	&webcam_stringtab,
	NULL,
};

static struct usb_device_descriptor webcam_device_descriptor = {
	.bLength		= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB			= cpu_to_le16(0x0200),
	.bDeviceClass		= USB_CLASS_MISC,
	.bDeviceSubClass	= 0x02,
	.bDeviceProtocol	= 0x01,
	.bMaxPacketSize0	= 64, /* dynamic */
	.idVendor		= cpu_to_le16(WEBCAM_VENDOR_ID),
	.idProduct		= cpu_to_le16(WEBCAM_PRODUCT_ID),
	.bcdDevice		= cpu_to_le16(WEBCAM_DEVICE_BCD),
	.iManufacturer		= 0, /* dynamic */
	.iProduct		= 0, /* dynamic */
	.iSerialNumber		= 0, /* dynamic */
	.bNumConfigurations	= 0, /* dynamic */
};
////////////////////////
extern struct usb_endpoint_descriptor uvc_fs_streaming_ep;
extern struct usb_endpoint_descriptor uvc_hs_streaming_ep;
extern struct usb_endpoint_descriptor uvc_control_ep;

extern struct usb_interface_assoc_descriptor uvc_iad;
extern struct usb_interface_descriptor uvc_control_intf;
extern struct usb_interface_descriptor uvc_streaming_intf_alt0;
extern struct usb_interface_descriptor uvc_streaming_intf_alt1;
DECLARE_UVC_INPUT_HEADER_DESCRIPTOR(1, 4);
extern struct UVC_INPUT_HEADER_DESCRIPTOR(1, 4) uvc_input_header;
extern struct usb_descriptor_header *usbd_uvc_descriptors_FS[];
extern struct usb_descriptor_header *usbd_uvc_descriptors_HS[];
extern struct uvc_streaming_control probe;
extern struct uvc_descriptor_header *uvc_hs_streaming_cls[];
extern struct uvc_descriptor_header *uvc_fs_control_cls[];

DECLARE_UVC_HEADER_DESCRIPTOR(1);
extern struct UVC_HEADER_DESCRIPTOR(1) uvc_control_header;

extern struct uvc_streaming_control commit;

DECLARE_UVC_FRAME_UNCOMPRESSED(3);
DECLARE_UVC_FRAME_FRAMEBASED(3);
DECLARE_UVC_FRAME_MJPEG(3);
DECLARE_UVC_EXTENSION_UNIT_DESCRIPTOR(1, 3);

extern struct uvc_camera_terminal_descriptor uvc_camera_terminal;
extern struct uvc_processing_unit_descriptor uvc_processing;
extern struct UVC_EXTENSION_UNIT_DESCRIPTOR(1,3) uvc_extension_unit;
extern struct uvc_output_terminal_descriptor uvc_output_termina;
extern struct uvc_color_matching_descriptor uvc_color_matching;
extern struct uvc_control_endpoint_descriptor uvc_control_cs_ep;
extern struct uvc_output_terminal_descriptor uvc_output_terminal;

extern struct uvc_format_info uvc_formats[];
/////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#if 0
static struct usb_interface_assoc_descriptor uvc_iad  = {
	.bLength		= sizeof(uvc_iad),
	.bDescriptorType	= USB_DT_INTERFACE_ASSOCIATION,
	.bFirstInterface	= 0,
	.bInterfaceCount	= 2,
	.bFunctionClass		= USB_CLASS_VIDEO,
	.bFunctionSubClass	= UVC_SC_VIDEO_INTERFACE_COLLECTION,
	.bFunctionProtocol	= 0x00,
	.iFunction		= 0,
};

static struct usb_interface_descriptor uvc_control_intf  = {
	.bLength		= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= UVC_INTF_VIDEO_CONTROL,
	.bAlternateSetting	= 0,
	.bNumEndpoints		= 1,
	.bInterfaceClass	= USB_CLASS_VIDEO,
	.bInterfaceSubClass	= UVC_SC_VIDEOCONTROL,
	.bInterfaceProtocol	= 0x00,
	.iInterface		= 0,
};

static struct usb_endpoint_descriptor uvc_control_ep  = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize		= cpu_to_le16(UVC_STATUS_MAX_PACKET_SIZE),
	.bInterval		= 8,
};

static struct uvc_control_endpoint_descriptor uvc_control_cs_ep  = {
	.bLength		= UVC_DT_CONTROL_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_CS_ENDPOINT,
	.bDescriptorSubType	= UVC_EP_INTERRUPT,
	.wMaxTransferSize	= cpu_to_le16(UVC_STATUS_MAX_PACKET_SIZE),
};

static struct usb_interface_descriptor uvc_streaming_intf_alt0  = {
	.bLength		= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= UVC_INTF_VIDEO_STREAMING,
	.bAlternateSetting	= 0,
	.bNumEndpoints		= 0,
	.bInterfaceClass	= USB_CLASS_VIDEO,
	.bInterfaceSubClass	= UVC_SC_VIDEOSTREAMING,
	.bInterfaceProtocol	= 0x00,
	.iInterface		= 0,
};

static struct usb_interface_descriptor uvc_streaming_intf_alt1  = {
	.bLength		= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= UVC_INTF_VIDEO_STREAMING,
	.bAlternateSetting	= 1,
	.bNumEndpoints		= 1,
	.bInterfaceClass	= USB_CLASS_VIDEO,
	.bInterfaceSubClass	= UVC_SC_VIDEOSTREAMING,
	.bInterfaceProtocol	= 0x00,
	.iInterface		= 0,
};

static struct usb_endpoint_descriptor uvc_fs_streaming_ep  = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= 0x05,//USB_ENDPOINT_XFER_ISOC,//USB_ENDPOINT_SYNC_ASYNC
				//| USB_ENDPOINT_XFER_ISOC,
	/* The wMaxPacketSize and bInterval values will be initialized from
	 * module parameters.
	 */
};

static struct usb_endpoint_descriptor uvc_hs_streaming_ep  = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= 0x05,//USB_ENDPOINT_XFER_ISOC,//USB_ENDPOINT_SYNC_ASYNC// default 0x05
				//| USB_ENDPOINT_XFER_ISOC,
	/* The wMaxPacketSize and bInterval values will be initialized from
	 * module parameters.
	 */
};

static struct usb_descriptor_header *uvc_fs_streaming[] = {
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt1,
	(struct usb_descriptor_header *) &uvc_fs_streaming_ep,
	NULL,
};

static struct usb_descriptor_header *uvc_hs_streaming[] = {
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt1,
	(struct usb_descriptor_header *) &uvc_hs_streaming_ep,
	NULL,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* --------------------------------------------------------------------------
 * Device descriptor
 */

#define WEBCAM_VENDOR_ID		0x1d6b	/* Linux Foundation */
#define WEBCAM_PRODUCT_ID		0x0102	/* Webcam A/V gadget */
#define WEBCAM_DEVICE_BCD		0x0010	/* 0.10 */

#define USB_CLASS_MISC                  0xef
#define CONFIG_USB_GADGET_VBUS_DRAW     100


#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_SERIALNUMBER		3
#define STRING_INTERFACE		4
#define STRING_UVC			5


#define DRIVER_DESC	"USB UVC CLASS"
#define DRIVER_VERSION	"Feb 2016"

#define MANUFACTURER 	"Realtek"

static char webcam_config_label[] = "Video";

static char	string_manufacturer [50] = MANUFACTURER;
static char	string_product [40] = DRIVER_DESC;
static char	string_serial [20] = "0123456789";

static struct usb_string		
webcam_strings [] = {
	{ STRING_MANUFACTURER,	string_manufacturer, },        
	{ STRING_PRODUCT,	string_product, },
	{ STRING_SERIALNUMBER,	string_serial, },
	{ STRING_INTERFACE,		"USB UVC Interface", },
	{ STRING_UVC,		"USB UVC", },
        NULL,
};

static struct usb_gadget_strings webcam_stringtab = {
	.language = 0x0409,	/* en-us */
	.strings = webcam_strings,
};

static struct usb_gadget_strings *webcam_device_strings[] = {
	&webcam_stringtab,
	NULL,
};

static struct usb_device_descriptor webcam_device_descriptor = {
	.bLength		= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB			= cpu_to_le16(0x0200),
	.bDeviceClass		= USB_CLASS_MISC,
	.bDeviceSubClass	= 0x02,
	.bDeviceProtocol	= 0x01,
	.bMaxPacketSize0	= 64, /* dynamic */
	.idVendor		= cpu_to_le16(WEBCAM_VENDOR_ID),
	.idProduct		= cpu_to_le16(WEBCAM_PRODUCT_ID),
	.bcdDevice		= cpu_to_le16(WEBCAM_DEVICE_BCD),
	.iManufacturer		= 0, /* dynamic */
	.iProduct		= 0, /* dynamic */
	.iSerialNumber		= 0, /* dynamic */
	.bNumConfigurations	= 0, /* dynamic */
};

DECLARE_UVC_HEADER_DESCRIPTOR(1);

static struct UVC_HEADER_DESCRIPTOR(1) uvc_control_header = {
	.bLength		= UVC_DT_HEADER_SIZE(1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_HEADER,
	.bcdUVC			= cpu_to_le16(0x0100),
	.wTotalLength		= 0,//51, /* dynamic */
	.dwClockFrequency	= cpu_to_le32(48000000),
	.bInCollection		= 1, /* dynamic */
	.baInterfaceNr[0]       = 1, /* dynamic */
};


static struct uvc_camera_terminal_descriptor uvc_camera_terminal = {
	.bLength		= UVC_DT_CAMERA_TERMINAL_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_INPUT_TERMINAL,
	.bTerminalID		= 1,
	.wTerminalType		= cpu_to_le16(0x0201),
	.bAssocTerminal		= 0,
	.iTerminal		= 0,
	.wObjectiveFocalLengthMin	= cpu_to_le16(0),
	.wObjectiveFocalLengthMax	= cpu_to_le16(0),
	.wOcularFocalLength		= cpu_to_le16(0),
	.bControlSize		= 3,
	.bmControls[0]		= 0,
	.bmControls[1]		= 0,
	.bmControls[2]		= 0,
};

static struct uvc_processing_unit_descriptor uvc_processing = {
	.bLength		= UVC_DT_PROCESSING_UNIT_SIZE(2),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_PROCESSING_UNIT,
	.bUnitID		= 2,
	.bSourceID		= 1,
	.wMaxMultiplier		= cpu_to_le16(16*1024),
	.bControlSize		= 2,
	.bmControls[0]		= 1,
	.bmControls[1]		= 0,
	.iProcessing		= 0,
};

DECLARE_UVC_EXTENSION_UNIT_DESCRIPTOR(1, 3);

static struct UVC_EXTENSION_UNIT_DESCRIPTOR(1,3) uvc_extension_unit = {
        .bLength                = UVC_DT_EXTENSION_UNIT_SIZE(1,3),
        .bDescriptorType        = USB_DT_CS_INTERFACE,
        .bDescriptorSubType     = UVC_VC_EXTENSION_UNIT,
        .bUnitID                = 0x03,
        .guidExtensionCode      = {0xce, 0xb9, 0x55, 0x2b, 0xb0, 0x14, 0x49, 0xa8, 0xb2, 0x2e,
				   0xf6, 0xb0, 0xa0, 0x12, 0x82, 0x74},
        //.guidExtensionCode      = {0x99, 0x1D, 0x5A, 0x7D,0x32, 0x5B,0x8B, 0x42,0x8C, 0xAD,
          //                        0x67, 0x1B,0x8A, 0x65, 0xF3, 0x7A},
        //.guidExtensionCode      = {0x70,0x33,0xf0,0x28,0x11,0x63,0x2e,0x4a,0xba,0x2c,0x68,0x90,0xeb,0x33,0x40,0x16},
        .bNumControls           = 0x18,
        .bNrInPins              = 0x01,
        .baSourceID[0]          = 0x02,
        .bControlSize           = 0x03,
        .bmControls[0]          = 0x00,
        .bmControls[1]          = 0x06,
        .bmControls[2]          = 0x00,
        .iExtension             = 0x00,   
};

static struct uvc_output_terminal_descriptor uvc_output_terminal = {
	.bLength		= UVC_DT_OUTPUT_TERMINAL_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_OUTPUT_TERMINAL,
	.bTerminalID		= 4,
	.wTerminalType		= cpu_to_le16(0x0101),
	.bAssocTerminal		= 0,
	.bSourceID		= 3,
	.iTerminal		= 0,
};

DECLARE_UVC_INPUT_HEADER_DESCRIPTOR(1, 4);

static struct UVC_INPUT_HEADER_DESCRIPTOR(1, 4) uvc_input_header = {
	.bLength		= UVC_DT_INPUT_HEADER_SIZE(1, 4),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_INPUT_HEADER,
	.bNumFormats		= 4,
	.wTotalLength		= 0,//69, /* dynamic */
	.bEndpointAddress	= 0, /* dynamic */
	.bmInfo			= 0,
	.bTerminalLink		= 4,
	.bStillCaptureMethod	= 0,
	.bTriggerSupport	= 0,
	.bTriggerUsage		= 0,
	.bControlSize		= 1,
	.bmaControls[0][0]	= 0,
	.bmaControls[1][0]	= 0,
        .bmaControls[2][0]	= 0,
        .bmaControls[3][0]	= 0,
};

static  struct uvc_format_uncompressed uvc_format_yuy2 = {
	.bLength		= UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_UNCOMPRESSED,
	.bFormatIndex		= 1,
	.bNumFrameDescriptors	= 1,
	.guidFormat		=
		{ 'N',  'V',  '1',  '6', 0x00, 0x00, 0x10, 0x00,
		 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
	.bBitsPerPixel		= 16,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
};

static  struct uvc_format_uncompressed uvc_format_nv12 = {
	.bLength		= UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_UNCOMPRESSED,
	.bFormatIndex		= 2,
	.bNumFrameDescriptors	= 3,
	.guidFormat		=
		{ 'N',  'V',  '1',  '2', 0x00, 0x00, 0x10, 0x00,
                0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
	.bBitsPerPixel		= 12,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
};

static struct uvc_format_mjpeg uvc_format_mjpg = {
	.bLength		= UVC_DT_FORMAT_MJPEG_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_MJPEG,
	.bFormatIndex		= 3,
	.bNumFrameDescriptors	= 1,
	.bmFlags		= 1,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
};

static struct uvc_format_framebased uvc_format_h264 = {
        .bLength		= UVC_DT_FORMAT_FRAMEBASED_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_FRAME_BASED,
	.bFormatIndex		= 4,
	.bNumFrameDescriptors	= 1,
	.guidFormat		=
		{ 'H',  '2',  '6',  '4', 0x00, 0x00, 0x10, 0x00,
		 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
	.bBitsPerPixel		= 12,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
        .bVariableSize          = 1,
};

DECLARE_UVC_FRAME_UNCOMPRESSED(3);

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuv_160p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(128),
	.wHeight		= cpu_to_le16(160),
	.dwMinBitRate		= cpu_to_le32(81920),
	.dwMaxBitRate		= cpu_to_le32(614400),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(40960),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static const struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuv_360p = {
        .bLength                = UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
        .bDescriptorType        = USB_DT_CS_INTERFACE,
        .bDescriptorSubType     = UVC_VS_FRAME_UNCOMPRESSED,
        .bFrameIndex            = 1,
        .bmCapabilities         = 0,
        .wWidth                 = cpu_to_le16(640),
        .wHeight                = cpu_to_le16(360),
        .dwMinBitRate           = cpu_to_le32(18432000),
        .dwMaxBitRate           = cpu_to_le32(55296000),
        .dwMaxVideoFrameBufferSize      = cpu_to_le32(460800),
        .dwDefaultFrameInterval = cpu_to_le32(666666),
        .bFrameIntervalType     = 3,
        .dwFrameInterval[0]     = cpu_to_le32(666666),
        .dwFrameInterval[1]     = cpu_to_le32(1000000),
        .dwFrameInterval[2]     = cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuv_480p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(640),
	.wHeight		= cpu_to_le16(480),
	.dwMinBitRate		= cpu_to_le32(7372800),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuv_720p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1280),
	.wHeight		= cpu_to_le16(720),
	.dwMinBitRate		= cpu_to_le32(3686400),
	.dwMaxBitRate		= cpu_to_le32(27648000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(1843200),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuy2_1080p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1920),
	.wHeight		= cpu_to_le16(1080),
	.dwMinBitRate		= cpu_to_le32(1920*1080*16*2),
	.dwMaxBitRate		= cpu_to_le32(1920*1080*16*15),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(1920*1080*2),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_nv12_1080p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1920),
	.wHeight		= cpu_to_le16(1080),
	.dwMinBitRate		= cpu_to_le32(6220800),
	.dwMaxBitRate		= cpu_to_le32(46656000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(3110400),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_nv12_480p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 3,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(640),
	.wHeight		= cpu_to_le16(480),
	.dwMinBitRate		= cpu_to_le32(640*480*12*2),//cpu_to_le32(7372800),
	.dwMaxBitRate		= cpu_to_le32(640*480*12*15),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static  struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_nv12_720p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 2,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1280),
	.wHeight		= cpu_to_le16(720),
	.dwMinBitRate		= cpu_to_le32(1280*720*12*2),
	.dwMaxBitRate		= cpu_to_le32(1280*720*12*15),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(1280*720*1.5),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

DECLARE_UVC_FRAME_FRAMEBASED(3);

static struct UVC_FRAME_FRAMEBASED(3) uvc_frame_h264_480p = {
        .bLength		= UVC_DT_FRAME_FRAMEBASED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_FRAME_BASED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(854),
	.wHeight		= cpu_to_le16(480),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwDefaultFrameInterval	= cpu_to_le32(400000),
	.bFrameIntervalType	= 3,
        .dwBytesPerLine 	= cpu_to_le32(0),
	.dwFrameInterval[0]	= cpu_to_le32(400000),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct UVC_FRAME_FRAMEBASED(3) uvc_frame_h264_1080p = {
        .bLength		= UVC_DT_FRAME_FRAMEBASED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_FRAME_BASED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1920),
	.wHeight		= cpu_to_le16(1080),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwDefaultFrameInterval	= cpu_to_le32(400000),
	.bFrameIntervalType	= 3,
        .dwBytesPerLine 	= cpu_to_le32(0),
	.dwFrameInterval[0]	= cpu_to_le32(400000),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};
//666666  //400000


DECLARE_UVC_FRAME_MJPEG(3);

static struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_360p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(640),
	.wHeight		= cpu_to_le16(360),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_320p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(240),
	.wHeight		= cpu_to_le16(320),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_240p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(320),
	.wHeight		= cpu_to_le16(240),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_1080p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1920),
	.wHeight		= cpu_to_le16(1080),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_160p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(128),
	.wHeight		= cpu_to_le16(160),
	.dwMinBitRate		= cpu_to_le32(307200),
	.dwMaxBitRate		= cpu_to_le32(4915200),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(40960),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static struct uvc_color_matching_descriptor uvc_color_matching = {
	.bLength		= UVC_DT_COLOR_MATCHING_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_COLORFORMAT,
	.bColorPrimaries	= 1,
	.bTransferCharacteristics	= 1,
	.bMatrixCoefficients	= 4,
};

#define v_160p          40960
#define v_360p          460800
#define v_720p          1843200
#define v_1080p         3110400
#define v_1080p_422v         4147200

static struct uvc_streaming_control probe = {
        .bmHint = 0,
        .bFormatIndex = 1,
        .bFrameIndex = 1,
        .dwFrameInterval = cpu_to_le32(666666),
        .wKeyFrameRate = 0,
        .wPFrameRate = 0,
        .wCompQuality = 0,
        .wCompWindowSize =0 ,
        .wDelay = 0,
        .dwMaxVideoFrameSize = cpu_to_le32(v_1080p_422v),
        .dwMaxPayloadTransferSize = 0,
        .dwClockFrequency = cpu_to_le32(48000000),
        .bmFramingInfo = 0,
        .bPreferedVersion = 0,
        .bMinVersion    = 0,   
        .bMaxVersion    = 0,
        
};
static struct uvc_streaming_control commit = {
        .bmHint = 0,
        .bFormatIndex = 1,
        .bFrameIndex = 1,
        .dwFrameInterval = cpu_to_le32(666666),
        .wKeyFrameRate = 0,
        .wPFrameRate = 0,
        .wCompQuality = 0,
        .wCompWindowSize =0 ,
        .wDelay = 0,
        .dwMaxVideoFrameSize = cpu_to_le32(v_1080p),
        .dwMaxPayloadTransferSize = 0,
        .dwClockFrequency = cpu_to_le32(48000000),
        .bmFramingInfo = 0,
        .bPreferedVersion = 0,
        .bMinVersion    = 0,   
        .bMaxVersion    = 0,
};


static struct uvc_descriptor_header *uvc_fs_control_cls[] = {
	(struct uvc_descriptor_header *) &uvc_control_header,
	(struct uvc_descriptor_header *) &uvc_camera_terminal,
	(struct uvc_descriptor_header *) &uvc_processing,
        (struct uvc_descriptor_header *) &uvc_extension_unit,
	(struct uvc_descriptor_header *) &uvc_output_terminal,
	NULL,
};

static struct uvc_descriptor_header *uvc_fs_streaming_cls[] = {
	(struct uvc_descriptor_header *) &uvc_input_header,
        (struct uvc_descriptor_header *) &uvc_format_yuy2,
	(struct uvc_descriptor_header *) &uvc_frame_yuy2_1080p,
	(struct uvc_descriptor_header *) &uvc_format_nv12,
	(struct uvc_descriptor_header *) &uvc_frame_nv12_1080p,
        (struct uvc_descriptor_header *) &uvc_frame_nv12_720p,
        (struct uvc_descriptor_header *) &uvc_frame_nv12_480p,
	(struct uvc_descriptor_header *) &uvc_format_mjpg,
	(struct uvc_descriptor_header *) &uvc_frame_mjpg_240p,
	(struct uvc_descriptor_header *) &uvc_format_h264,
	(struct uvc_descriptor_header *) &uvc_frame_h264_1080p,
	(struct uvc_descriptor_header *) &uvc_color_matching,
	NULL,
};

static struct uvc_descriptor_header *uvc_hs_streaming_cls[] = {
	(struct uvc_descriptor_header *) &uvc_input_header,
        (struct uvc_descriptor_header *) &uvc_format_yuy2,
	(struct uvc_descriptor_header *) &uvc_frame_yuy2_1080p,
	(struct uvc_descriptor_header *) &uvc_format_nv12,
	(struct uvc_descriptor_header *) &uvc_frame_nv12_1080p,
        (struct uvc_descriptor_header *) &uvc_frame_nv12_720p,
        (struct uvc_descriptor_header *) &uvc_frame_nv12_480p,
	(struct uvc_descriptor_header *) &uvc_format_mjpg,
	(struct uvc_descriptor_header *) &uvc_frame_mjpg_1080p,
	(struct uvc_descriptor_header *) &uvc_format_h264,
	(struct uvc_descriptor_header *) &uvc_frame_h264_1080p,
	(struct uvc_descriptor_header *) &uvc_color_matching,
	NULL,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static struct usb_descriptor_header *usbd_uvc_descriptors_FS[] = {
	(struct usb_descriptor_header *) &uvc_iad,
	(struct usb_descriptor_header *) &uvc_control_intf,/////////
	(struct usb_descriptor_header *) &uvc_control_header,
	(struct usb_descriptor_header *) &uvc_camera_terminal,
	(struct usb_descriptor_header *) &uvc_processing,
        (struct usb_descriptor_header *) &uvc_extension_unit,
	(struct usb_descriptor_header *) &uvc_output_terminal,/////////
	(struct usb_descriptor_header *) &uvc_control_ep,
	(struct usb_descriptor_header *) &uvc_control_cs_ep,
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt0,/////////
	(struct usb_descriptor_header *) &uvc_input_header,
        (struct usb_descriptor_header *) &uvc_format_yuy2,
	(struct usb_descriptor_header *) &uvc_frame_yuy2_1080p,
	(struct usb_descriptor_header *) &uvc_format_nv12,
	(struct usb_descriptor_header *) &uvc_frame_nv12_1080p,
        (struct usb_descriptor_header *) &uvc_frame_nv12_720p,
        (struct usb_descriptor_header *) &uvc_frame_nv12_480p,
	(struct usb_descriptor_header *) &uvc_format_mjpg,
	(struct usb_descriptor_header *) &uvc_frame_mjpg_1080p,
	(struct usb_descriptor_header *) &uvc_format_h264,
	(struct usb_descriptor_header *) &uvc_frame_h264_1080p,
	(struct usb_descriptor_header *) &uvc_color_matching,
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt1,
	(struct usb_descriptor_header *) &uvc_fs_streaming_ep,
	NULL,
};

static struct usb_descriptor_header *usbd_uvc_descriptors_HS[] = {
	(struct usb_descriptor_header *) &uvc_iad,
	(struct usb_descriptor_header *) &uvc_control_intf,
	(struct usb_descriptor_header *) &uvc_control_header,///////
	(struct usb_descriptor_header *) &uvc_camera_terminal,
	(struct usb_descriptor_header *) &uvc_processing,
        (struct usb_descriptor_header *) &uvc_extension_unit,
	(struct usb_descriptor_header *) &uvc_output_terminal,/////////
	(struct usb_descriptor_header *) &uvc_control_ep,
	(struct usb_descriptor_header *) &uvc_control_cs_ep,
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt0,/////////
	(struct usb_descriptor_header *) &uvc_input_header,
        (struct usb_descriptor_header *) &uvc_format_yuy2,
	(struct usb_descriptor_header *) &uvc_frame_yuy2_1080p,
	(struct usb_descriptor_header *) &uvc_format_nv12,
	(struct usb_descriptor_header *) &uvc_frame_nv12_1080p,
        (struct usb_descriptor_header *) &uvc_frame_nv12_720p,
        (struct usb_descriptor_header *) &uvc_frame_nv12_480p,
	(struct usb_descriptor_header *) &uvc_format_mjpg,
	(struct usb_descriptor_header *) &uvc_frame_mjpg_1080p,
	(struct usb_descriptor_header *) &uvc_format_h264,
        (struct usb_descriptor_header *) &uvc_frame_h264_1080p,
	(struct usb_descriptor_header *) &uvc_color_matching,
	(struct usb_descriptor_header *) &uvc_streaming_intf_alt1,
	(struct usb_descriptor_header *) &uvc_hs_streaming_ep,
	NULL,
};
#endif
int usbd_uvc_init(void);
int get_frame_index(struct uvc_video *video);
struct uvc_dev *get_private_usbd_uvc(void);
struct usbd_uvc_buffer *uvc_video_out_stream_queue(struct uvc_dev *uvc_ctx);
void uvc_video_put_in_stream_queue(struct usbd_uvc_buffer *payload, struct uvc_dev *uvc_ctx);
void uvc_video_put_out_stream_queue(struct usbd_uvc_buffer *payload, struct uvc_dev *uvc_ctx);
void uvc_events_process(struct uvc_dev *dev,struct uvc_req_data *uvc_event);
void composite_setup_complete_1();
int set_uvc_string(char *product_name,char *serial_name,unsigned short bcdDevice);

#endif