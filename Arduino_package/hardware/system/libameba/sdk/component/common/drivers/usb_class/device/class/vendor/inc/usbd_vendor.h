#ifndef USBD_MSC_H
#define USBD_MSC_H

#include "usb.h"
#include "usb_gadget.h"
#include "core/inc/usb_composite.h"
#if defined (CONFIG_PLATFORM_8721D)
#include "ameba_otg.h"
#else
#include "rtl8195a_otg.h"
#endif

/* config usb msc device debug inforation */
#define USBD_VENDOR_DEBUG          1

#if	USBD_VENDOR_DEBUG
#define USBD_PRINTF(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_ERROR(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_WARN(fmt, args...)		            	DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define FUN_ENTER                                   DBG_8195A("\n\r%s ==>\n", __func__)
#define FUN_EXIT                                    DBG_8195A("\n\r%s <==\n", __func__)
#define FUN_TRACE                                   DBG_8195A("\n\r%s:%d \n", __func__, __LINE__)
#else
#define USBD_PRINTF(fmt, args...)
#define USBD_ERROR(fmt, args...)		            DBG_8195A("\n\r%s: " fmt, __FUNCTION__, ## args)
#define USBD_WARN(fmt, args...)
#define FUN_ENTER
#define FUN_EXIT
#define FUN_TRACE
#endif
struct vend_common;
struct vend_dev; 

#define VEN_NUM_REQUESTS 2

struct ven_common{
	struct ven_dev	*vendev;
	struct usb_gadget	*gadget;
	struct usb_ep		*ep0;		
	struct usb_request	*req0;		/* for control responses */
        unsigned int 		running;
};

struct ven_dev {
        struct ven_common 	*common;
	struct usb_function	func;
	struct usb_ep		*in_ep;
	struct usb_ep		*out_ep;
        struct usb_ep		*iso_in_ep;
	struct usb_ep		*iso_out_ep;
        
        /* Requests */
	unsigned int req_size;

        struct usb_iso_request *req;

	unsigned char *req_buffer[VEN_NUM_REQUESTS];


        struct usb_iso_request *req_in;

	unsigned char *req_buffer_in[VEN_NUM_REQUESTS];
};

/*
* N_bh : number of buffer header
* Size_bh: buffer size per buffer
* type:msc physical disk type
*/
int usbd_vendor_init(int N_bh, int Size_bh, int type);
void usbd_vendor_deinit(void);

#endif
