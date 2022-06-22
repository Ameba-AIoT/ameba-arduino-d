#ifndef USBH_CDC_ACM_H
#define USBH_CDC_ACM_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_CDC_ACM

#include "usb.h"
#include "usbh_cdc_acm_if.h"


#define USBH_CDC_ACM_TASK_PRIORITY				2

/* Debug options */
#define USBH_CDC_ACM_DEBUG                       0
#define USBH_CDC_ACM_STR                         "ACM"
#if USBH_CDC_ACM_DEBUG
#define USBH_CDC_ACM_INFO(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBH_CDC_ACM_STR, __FUNCTION__, ## args)
#define USBH_CDC_ACM_WARN(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBH_CDC_ACM_STR, __FUNCTION__, ## args)
#define USBH_CDC_ACM_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBH_CDC_ACM_STR, __FUNCTION__, ## args)
#define USBH_CDC_ACM_ENTER                       printf("\n\r[%s]%s: =>\n", USBH_CDC_ACM_STR, __FUNCTION__)
#define USBH_CDC_ACM_EXIT                        printf("\n\r[%s]%s: <=\n", USBH_CDC_ACM_STR, __FUNCTION__)
#define USBH_CDC_ACM_EXIT_ERR                    printf("\n\r[%s]%s: ERR <=", USBH_CDC_ACM_STR, __FUNCTION__)
#else
#define USBH_CDC_ACM_INFO(fmt, args...)
#define USBH_CDC_ACM_WARN(fmt, args...)
#define USBH_CDC_ACM_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBH_CDC_ACM_STR, __FUNCTION__, ## args)
#define USBH_CDC_ACM_ENTER
#define USBH_CDC_ACM_EXIT
#define USBH_CDC_ACM_EXIT_ERR
#endif


/*Class type*/
#define CDC_COMM_INTERFACE_TYPE			0x02
#define CDC_DATA_INTERFACE_TYPE			0x0a


/*Class specific descriptor type*/
#define USB_CDC_HEADER_TYPE				0x00	/* header_desc */
#define USB_CDC_CALL_MANAGEMENT_TYPE	0x01	/* call_mgmt_descriptor */
#define USB_CDC_ACM_TYPE				0x02	/* acm_descriptor */
#define USB_CDC_UNION_TYPE				0x06	/* union_desc */
#define USB_CDC_COUNTRY_TYPE			0x07  /*country decsriptor, not support*/


/*Class specific request type*/
#define CDC_SEND_ENCAPSULATED_COMMAND_REQUEST		0x00
#define CDC_GET_ENCAPSULATED_RESPONE_REQUEST		0X01
#define CDC_SET_COMM_FEATURE_REQUEST				0x02
#define CDC_GET_COMM_FEATURE_REQUEST				0x03
#define CDC_CLEAR_COMM_FEATURE_REQUEST				0x04
#define CDC_SET_LINE_CODING_RQUEST					0x20
#define CDC_GET_LINE_CODING_RQUEST					0x21
#define CDC_SET_CONTROL_LINE_STATE_REQUEST			0x22
#define CDC_SEND_BREAK_REQUEST						0x23


/*Class specific notification type*/
#define USB_CDC_NOTIFY_NETWORK_CONNECTION			0x00
#define USB_CDC_NOTIFY_SERIAL_STATE					0x20

/*write/read buffer(urb) number*/
#define ACM_NW		1
#define ACM_NR		1



struct acm_wb {
	_sema comp_sema;
	int use;
	struct urb *urb;
	struct cdc_acm *instance;
};

struct acm_rb {
	unsigned char *base;
	u32 len;
	struct urb *urb;
	struct cdc_acm *instance;
};

/*Union descriptor struct*/
struct usb_cdc_union_desc {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubType;
	u8 bMasterInterface0;
	u8 bSlaveInterface0;
};

/*Notification struct*/
struct usb_cdc_notification {
	u8 bmRequestType;
	u8 bNotificationType;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
};

/*Request support according to acm descriptor*/
#define USB_CDC_CONFIG_SUPPORT_0			0x1		/*device support notification Network_Connection*/
#define USB_CDC_CONFIG_SUPPORT_1			0x1<<1		/*Device supports the request Send_Break*/
#define USB_CDC_CONFIG_SUPPORT_2			0x1<<2		/*Device supports the request combination of Set_Line_Coding, 
														Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State*/
#define USB_CDC_CONFIG_SUPPORT_3			0x1<<3		/*Device supports the request combination of Set_Comm_Feature,
														Clear_Comm_Feature, and Get_Comm_Feature*/


/*cdc acm function main struct*/
struct cdc_acm {
	struct usb_device *dev;				/* the corresponding usb device */
	struct usb_interface *intf;
	struct usb_interface *control;			/* control interface */
	struct usb_interface *data;			/* data interface */
	struct urb *ctrlurb;				/* urbs */
	u8 *ctrl_buffer;				/* buffers of urbs */
	dma_addr_t ctrl_dma;				/* dma handles of buffers */
	struct acm_wb wb[ACM_NW];
	struct acm_rb rb[ACM_NR];
	_lock read_lock;
	_lock write_lock;
	CDC_LineCodingTypeDef line;		/* bits, stop, parity */
	u32 ctrlin;				/* input control lines (DCD, DSR, RI, break, overruns) */
	u32 ctrlout;				/* output control lines (DTR, RTS) */
	u32 readsize;			/*read urb transfer size*/
	u32 writesize;			/*write urb transfer size*/
	u32 ctrlsize;			/* buffer sizes for freeing */
	u32 combined_interfaces;		/**indicate if device is combined interface*/
	u32 acm_caps;				/* control capabilities from the class specific header */
	u8 comm_interface_protocol;			/*communication interface class protocol code*/

	usbh_cdc_acm_usr_cb_t *cb;  /*user callback*/
};


#endif // CONFIG_USBH_CDC_ACM

#endif // USBH_CDC_ACM_H

