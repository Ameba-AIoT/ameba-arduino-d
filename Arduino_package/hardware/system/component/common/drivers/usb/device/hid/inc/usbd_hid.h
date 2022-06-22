#ifndef USBD_HID_H
#define USBD_HID_H

#include <platform_opts.h>
#ifdef CONFIG_USBD_HID

#include "usb.h"
#include "usb_composite.h"
#include "usbd_hid_if.h"


/* Debug options */
#define USBD_HID_DEBUG                       0
#define USBD_HID_STR                         "HID"
#if USBD_HID_DEBUG
#define USBD_HID_INFO(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBD_HID_STR, __FUNCTION__, ## args)
#define USBD_HID_WARN(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBD_HID_STR, __FUNCTION__, ## args)
#define USBD_HID_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBD_HID_STR, __FUNCTION__, ## args)
#define USBD_HID_ENTER                       printf("\n\r[%s]%s: =>", USBD_HID_STR, __FUNCTION__)
#define USBD_HID_EXIT                        printf("\n\r[%s]%s: <=", USBD_HID_STR, __FUNCTION__)
#define USBD_HID_EXIT_ERR                    printf("\n\r[%s]%s: ERR <=", USBD_HID_STR, __FUNCTION__)
#else
#define USBD_HID_INFO(fmt, args...)
#define USBD_HID_WARN(fmt, args...)
#define USBD_HID_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBD_HID_STR, __FUNCTION__, ## args)
#define USBD_HID_ENTER
#define USBD_HID_EXIT
#define USBD_HID_EXIT_ERR
#endif

/*hid request*/
#define USBD_HID_GET_REPORT			0x01
#define USBD_HID_GET_IDLE			0x02
#define USBD_HID_GET_PROTOCOL		0x03
#define USBD_HID_SET_REPORT			0x09
#define USBD_HID_SET_IDLE			0x0a
#define USBD_HID_SET_PROTOCOL		0x0b

/*wValue of HID report request*/
#define HID_REPORT_DESC 	0x22

// String index
#define USBD_HID_STRING_MANUFACTURER         1
#define USBD_HID_STRING_PRODUCT              2
#define USBD_HID_STRING_SERIALNUMBER         3
#define USBD_HID_STRING_STRING_INTERFACE     4

// Endpoint address
#define USBD_HID_INTERRUPT_IN_EP_ADDRESS		0x81
#define USBD_HID_DEFAULT_INT_IN_XFER_SIZE		4
#define HID_MOUSE_REPORT_DESC_SIZE				52
#define USBD_HID_DESC_SIZE						0x9

//mouse button define
#define MOUSE_BUTTON_LEFT			0x01	//left button. 0: release, 1: press
#define MOUSE_BUTTON_RIGHT			0x02	//right button. 0: release, 1: press
#define MOUSE_BUTTON_MIDDLE			0x04	//wheel button. 0: release, 1: press
#define MOUSE_BUTTON_RESERVED		0xf8	//reserved. all bit should set to 1.

struct mouse{
	u8 button;					//see mouse button define below.	
	char x_axis;				//x-axis pixels. relative value from -127 to 127, positive for right and negative for left 
	char y_axis;				//y-axis pixels. relative value from -127 to 127, positive for up and negative for down 
	char wheel;					//scrolling units. relative value from -127 to 127, positive for up and negative for down. 
};


struct usb_hid_device_t {
	struct usb_gadget   *gadget;
	struct usb_ep       *int_in_ep;
	struct usb_request  *int_in_req;
	struct usb_function func;
	struct mouse data;
	u32 pending_data;
	u32 plug_in;
	usbd_hid_usr_cb_t *cb;
};


struct usb_hid_descriptor {
	u8  bLength;
	u8  bDescriptorType;
	u16  bcdHID;
	u8 bCountryCode;
	u8 bNumDescriptors;
	u8 bDescriptorTypeSub;
	u16 wDescriptorLength;
}__attribute__((packed));

#endif // CONFIG_USBD_HID

#endif // USBD_HID_H
