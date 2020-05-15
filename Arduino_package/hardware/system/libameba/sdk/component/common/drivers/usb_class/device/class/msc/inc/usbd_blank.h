#ifndef USBD_MSC_H
#define USBD_MSC_H

#include "usb.h"
#include "usb_gadget.h"
#include "core/inc/usb_composite.h"
#include "msc/inc/usbd_msc_config.h"

/* config usb msc device debug inforation */
#define USBD_MSC_DEBUG          0
#if defined(CONFIG_PLATFORM_8195BHP)
	#if	USBD_MSC_DEBUG
	#define USBD_PRINTF(fmt, args...)		            DBG_8195BL("\n\r%s: " fmt, __FUNCTION__, ## args)
	#define USBD_ERROR(fmt, args...)		            DBG_8195BL("\n\r%s: " fmt, __FUNCTION__, ## args)
	#define USBD_WARN(fmt, args...)		            	DBG_8195BL("\n\r%s: " fmt, __FUNCTION__, ## args)
	#define FUN_ENTER                                   DBG_8195BL("\n\r%s ==>\n", __func__)
	#define FUN_EXIT                                    DBG_8195BL("\n\r%s <==\n", __func__)
	#define FUN_TRACE                                   DBG_8195BL("\n\r%s:%d \n", __func__, __LINE__)
	#else
	#define USBD_PRINTF(fmt, args...)
	#define USBD_ERROR(fmt, args...)		            DBG_8195BL("\n\r%s: " fmt, __FUNCTION__, ## args)
	#define USBD_WARN(fmt, args...)
	#define FUN_ENTER
	#define FUN_EXIT
	#define FUN_TRACE
	#endif
#else
	#if	USBD_MSC_DEBUG
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
#endif

/* MSC Request Codes */
#define MSC_REQUEST_RESET               0xFF
#define MSC_REQUEST_GET_MAX_LUN         0xFE


struct blank_common{
	struct blank_dev	*blankdev;
	
	struct msc_lun	**luns;
	struct msc_lun	*curlun;

	struct usb_gadget	*gadget;
	struct usb_ep		*ep0;		
	struct usb_request	*req0;		/* for control responses */

	/* scsi cbw relevant */

	u32			data_size;
	u32			data_size_from_cmnd;
	u32			tag;
	u32			residue;
	u32			usb_amount_left;
	u8			scsi_cmnd[16]; // max command
	u8			cmnd_size;
	
	u8		lun;	/* current lun*/
	u8		nluns;

	u8	nbufhd;
	u8	nbufhd_a;
	_list	bufhd_pool;
	_mutex  bufhd_mutex;
	/* bulk out cmd*/
	_list	boc_list;
	_mutex  boc_mutex;

	/* bolk out data*/
	_mutex  bod_mutex;
	_list	bod_list;
	/**/
	
	unsigned int		can_stall:1;
	unsigned int		phase_error:1;
	unsigned int		short_packet_received:1;
	unsigned int		bad_lun_okay:1;
	unsigned int 		running:1;
};

typedef enum _bufhd_type{
	BUFHD_CBW = 0,
	BUFHD_CSW,
	BUFHD_DATA,
}bufhd_type;

struct msc_bufhd{
	u8* buf;
	int buf_size;
	bufhd_type type;
	_list  list;	
	struct usb_request	*reqin; 	/* for bulkin responses */
	struct usb_request	*reqout;
};

struct blank_dev{
	struct blank_common 	*common;
	
	u16 		interface_number;
	u8			config;

	struct usb_ep	*in_ep;
	struct usb_ep	*out_ep;
	unsigned int	bulk_in_enabled:1;
	unsigned int	bulk_out_enabled:1;

    const struct usb_endpoint_descriptor
				*in, *out, *status;
    struct usb_function	func;
};

static u32 min(u32 value1,u32 value2);



/*
* N_bh : number of buffer header
* Size_bh: buffer size per buffer
* type:msc physical disk type
*/
int usbd_blank_init(void);
void usbd_blank_deinit(void);

#endif
