/*
 * Ethernet gadget driver -- with CDC and non-CDC options
 * Builds on hardware support for a full duplex link.
 *
 * CDC Ethernet is the standard USB solution for sending Ethernet frames
 * using USB.  Real hardware tends to use the same framing protocol but look
 * different for control features.  This driver strongly prefers to use
 * this USB-IF standard as its open-systems interoperability solution;
 * most host side USB stacks (except from Microsoft) support it.
 *
 * There's some hardware that can't talk CDC.  We make that hardware
 * implement a "minimalist" vendor-agnostic CDC core:  same framing, but
 * link-level setup only requires activating the configuration.
 * Linux supports it, but other host operating systems may not.
 * (This is a subset of CDC Ethernet.)
 *
 * A third option is also in use.  Rather than CDC Ethernet, or something
 * simpler, Microsoft pushes their own approach: RNDIS.  The published
 * RNDIS specs are ambiguous and appear to be incomplete, and are also
 * needlessly complex.
 */
#ifndef __USB_ETHERNET_H
#define __USB_ETHERNET_H

#include "usb.h"
#include "usb_gadget.h"
#include "core/inc/usb_composite.h"
#include "usb_eth_opts.h"

//#define DRIVER_DESC		"Ethernet Gadget"
#define DRIVER_DESC		"USB Network Interface"
#define DRIVER_VERSION		"May Day 2015"
#define ETH_ADDR        "00E04C8196C8"
#define ETH_FRAME_LEN 0x5EA
#if 0
OTG_ROM_DATA_SECTION
static const char shortname [] = "ether";
OTG_ROM_DATA_SECTION
static const char driver_desc [] = DRIVER_DESC;
#endif
#define CONFIG_USB_ETH_RNDIS 1


#define RNDIS_VENDOR_NUM	ULINKER_ETHER_VID
#define RNDIS_PRODUCT_NUM	ULINKER_ETHER_PID


/* Thanks to NetChip Technologies for donating this product ID.
 * It's for devices with only CDC Ethernet configurations.
 */
#define CDC_VENDOR_NUM	0x0525		/* NetChip */
#define CDC_PRODUCT_NUM	0xa4a1		/* Linux-USB Ethernet Gadget */

/* USB DRIVER HOOKUP (to the hardware driver, below us), mostly
 * ep0 implementation:  descriptors, config management, setup().
 * also optional class-specific notification interrupt transfer.
 */

/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.  For now we do either full CDC, or
 * our simple subset, with RNDIS as an optional second configuration.
 *
 * RNDIS includes some CDC ACM descriptors ... like CDC Ethernet.  But
 * the class descriptors match a modem (they're ignored; it's really just
 * Ethernet functionality), they don't need the NOP altsetting, and the
 * status transfer endpoint isn't optional.
 */

#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_ETHADDR			3
#define STRING_DATA			4
#define STRING_CONTROL			5
#define STRING_RNDIS_CONTROL		6
#define STRING_CDC			7
#define STRING_SUBSET			8
#define STRING_RNDIS			9
#define STRING_SERIALNUMBER		10

/* holds our biggest descriptor (or RNDIS response) */
//#define USB_BUFSIZ	256

#define BUFSIZ_IN	512
#define BUFSIZ_OUT	512
/*
 * This device advertises one configuration, eth_config, unless RNDIS
 * is enabled (rndis_config) on hardware supporting at least two configs.
 *
 * NOTE:  Controllers like superh_udc should probably be able to use
 * an RNDIS-only configuration.
 *
 * FIXME define some higher-powered configurations to make it easier
 * to recharge batteries ...
 */

//#define DEV_CONFIG_VALUE	1	/* cdc or subset */
//#define DEV_RNDIS_CONFIG_VALUE	2	/* rndis; optional */
#define DEV_CONFIG_VALUE	2	/* cdc or subset */
#define DEV_RNDIS_CONFIG_VALUE	1	/* rndis; optional */


#define	DEVSPEED	USB_SPEED_HIGH


/* descriptors that are built on-demand */
#if 0
START_OTG_RAM_DATA_SECTION
static char				manufacturer [50];
START_OTG_RAM_DATA_SECTION
static char				product_desc [40] = DRIVER_DESC;
START_OTG_RAM_DATA_SECTION
static char				serial_number [20];
#endif

/* address that the host will use ... usually assigned at random */
//ModifiedByJD    static char				ethaddr [2 * ETH_ALEN + 1];
#if 0
START_OTG_RAM_DATA_SECTION
static char				ethaddr [2 * 6 + 1] = ETH_ADDR;
#endif
/* static strings, in UTF-8 */
#if 0
START_OTG_RAM_DATA_SECTION
static struct usb_string		strings [] = {
	{ STRING_MANUFACTURER,	manufacturer, },
	{ STRING_PRODUCT,	product_desc, },
	{ STRING_SERIALNUMBER,	serial_number, },
	{ STRING_DATA,		"Ethernet Data", },
#if 1//def	DEV_CONFIG_CDC//ModifiedByJD    
	{ STRING_CDC,		"CDC Ethernet", },
	{ STRING_ETHADDR,	ethaddr, },
	{ STRING_CONTROL,	"CDC Communications Control", },
#endif
#if 1//def	DEV_CONFIG_SUBSET//ModifiedByJD    
	{ STRING_SUBSET,	"CDC Ethernet Subset", },
#endif
#if 1//def	CONFIG_USB_ETH_RNDIS//ModifiedByJD    
	{ STRING_RNDIS,		"RNDIS", },
	{ STRING_RNDIS_CONTROL,	"RNDIS Communications Control", },
#endif		/* end of list */
};
#endif
#if 0
static struct usb_gadget_strings	stringtab = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings,
};

static struct usb_gadget_strings *dev_strings[] = {
	&stringtab,
	NULL,
};

START_OTG_RAM_DATA_SECTION
static struct usb_device_descriptor
device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		    (0x0200),

	.bDeviceClass =		USB_CLASS_COMM,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,

	.idVendor =		(CDC_VENDOR_NUM),
	.idProduct =		(CDC_PRODUCT_NUM),
	.iManufacturer =	STRING_MANUFACTURER,
	.iProduct =		STRING_PRODUCT,
	.bNumConfigurations =	1,
};

START_OTG_RAM_DATA_SECTION
static struct usb_config_descriptor
eth_config = {
	.bLength =		sizeof eth_config,
	.bDescriptorType =	USB_DT_CONFIG,

	/* compute wTotalLength on the fly */
	.bNumInterfaces =	1,
	.bConfigurationValue =	DEV_CONFIG_VALUE,
	.iConfiguration =	STRING_CDC,
	.bmAttributes =		USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower =		50,
};

START_OTG_RAM_DATA_SECTION
static struct usb_otg_descriptor
otg_descriptor = {
	.bLength =		sizeof otg_descriptor,
	.bDescriptorType =	USB_DT_OTG,

	.bmAttributes =		USB_OTG_SRP,
};
#endif

#if 0
#ifdef	CONFIG_USB_ETH_RNDIS

/* RNDIS doesn't activate by changing to the "real" altsetting */
START_OTG_RAM_DATA_SECTION
static struct usb_interface_descriptor
rndis_data_intf = {
	.bLength =		sizeof rndis_data_intf,
	.bDescriptorType =	USB_DT_INTERFACE,

	.bInterfaceNumber =	0,
	.bAlternateSetting =	0,
	.bNumEndpoints =	2,
	.bInterfaceClass =	USB_CLASS_CDC_DATA,
	.bInterfaceSubClass =	0,
	.bInterfaceProtocol =	0,
	.iInterface =		STRING_DATA,
};

#endif

START_OTG_RAM_DATA_SECTION
static struct usb_endpoint_descriptor
hs_source_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	(512),//ModifiedByJD    
};

START_OTG_RAM_DATA_SECTION
static struct usb_endpoint_descriptor
hs_sink_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	(512),//ModifiedByJD    
};

START_OTG_RAM_DATA_SECTION
static struct usb_endpoint_descriptor
fs_source_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

START_OTG_RAM_DATA_SECTION
static struct usb_endpoint_descriptor
fs_sink_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

OTG_ROM_DATA_SECTION
static const struct usb_descriptor_header *fs_rndis_function [] = {
	(struct usb_descriptor_header *) &otg_descriptor,
	/* control interface matches ACM, not Ethernet */
#if 0//ModifiedByJD        
	(struct usb_descriptor_header *) &rndis_control_intf,
	(struct usb_descriptor_header *) &header_desc,
	(struct usb_descriptor_header *) &call_mgmt_descriptor,
	(struct usb_descriptor_header *) &acm_descriptor,
	(struct usb_descriptor_header *) &union_desc,
	(struct usb_descriptor_header *) &fs_status_desc,
#endif	
	/* data interface has no altsetting */
	(struct usb_descriptor_header *) &rndis_data_intf,
	(struct usb_descriptor_header *) &fs_source_desc,
	(struct usb_descriptor_header *) &fs_sink_desc,
	NULL,
};



OTG_ROM_DATA_SECTION
static const struct usb_descriptor_header *fs_eth_function [11] = {
	(struct usb_descriptor_header *) &otg_descriptor,
#ifdef DEV_CONFIG_CDC
	/* "cdc" mode descriptors */
	(struct usb_descriptor_header *) &control_intf,
	(struct usb_descriptor_header *) &header_desc,
	(struct usb_descriptor_header *) &union_desc,
	(struct usb_descriptor_header *) &ether_desc,
	/* NOTE: status endpoint may need to be removed */
	(struct usb_descriptor_header *) &fs_status_desc,
	/* data interface, with altsetting */
	(struct usb_descriptor_header *) &data_nop_intf,
	(struct usb_descriptor_header *) &data_intf,
	(struct usb_descriptor_header *) &fs_source_desc,
	(struct usb_descriptor_header *) &fs_sink_desc,
	NULL,
#endif /* DEV_CONFIG_CDC */
};

#ifdef	CONFIG_USB_ETH_RNDIS
OTG_ROM_DATA_SECTION
static const struct usb_descriptor_header *hs_rndis_function [] = {
	(struct usb_descriptor_header *) &otg_descriptor,
	/* control interface matches ACM, not Ethernet */
#if 0//ModifiedByJD        
	(struct usb_descriptor_header *) &rndis_control_intf,
	(struct usb_descriptor_header *) &header_desc,
	(struct usb_descriptor_header *) &call_mgmt_descriptor,
	(struct usb_descriptor_header *) &acm_descriptor,
	(struct usb_descriptor_header *) &union_desc,
	(struct usb_descriptor_header *) &hs_status_desc,
#endif	
	/* data interface has no altsetting */
	(struct usb_descriptor_header *) &rndis_data_intf,
	(struct usb_descriptor_header *) &hs_source_desc,
	(struct usb_descriptor_header *) &hs_sink_desc,
	NULL,
};
#endif

START_OTG_RAM_DATA_SECTION
static struct usb_config_descriptor
rndis_config = {
	.bLength =              sizeof rndis_config,
	.bDescriptorType =      USB_DT_CONFIG,

	/* compute wTotalLength on the fly */
	.bNumInterfaces =       1,
	.bConfigurationValue =  DEV_RNDIS_CONFIG_VALUE,
	.iConfiguration =       STRING_RNDIS, 
	.bmAttributes =		USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower =            50,
};

#endif

struct rx_frame{
	dwc_list_link_t	list;
	struct sk_buff* skb;
	u8 		cmd;
	u16 	pkt_len;
	u8*		buf_addr;
	u8* 	pbuf;
	u8* 	priv;
	u8		status;
};
struct tx_frame{
	dwc_list_link_t	list;
	struct usb_request	*req;
	u16 	pkt_len;
	u8* 	priv;
	u8		status;
};

// indicate packages type
enum{
	PKTS_CMD = 0,
	PKTS_DATA
};

// inidicate frame status
enum{
	FRAME_INITED = 0,
	FRAME_QUEUED,
	FRAME_DONE,
	FRAME_FAIL
};
struct usb_queue{
	_Mutex	mutex;
	dwc_list_link_t 	list;		
};


#define MAX_PKT_LEN		1538 //
#define USB_DMA_ALIGN_4		4

#define BO_BUFF_SZ	NR_TX_SKB_PERBUF*MAX_PKT_LEN
#define BI_BUFF_SZ	NR_RX_SKB_PERBUF*MAX_PKT_LEN

#define VENDOR_WRITEN	0x00 //vendor memory wirte 
#define VENDOR_READN	0x01 //vendor memory read
#define VENDOR_DEV_ISREADY 0x02 // device is ready or not

enum {
	FW_READY_NOT = 0,
	FW_READY_YES = 1
};

#include "timer_api.h"
#include "dma_api.h"


struct eth_dev{
		struct usb_gadget	*gadget;
		struct usb_request	*req;		/* for control responses */
		/* when configured, we have one of two configs:
		 * - source data (in to host) and sink it (out from host)
		 * - or loop it back (out from host back in to host)
		 */
		u8			config;
		struct usb_ep	*in_ep;
		struct usb_ep	*out_ep;
	    const struct usb_endpoint_descriptor
					*in, *out, *status;
	    // lock is held when accessing usb
	    _Mutex	eth_mutex;
		
	    struct usb_function	func;

		u8 		usb_cmd; // this for usb common command
		
		/*send (depends on host)*/
		_Sema	xmit_sema;
		xTaskHandle	xmit_task;
		u8		tx_qlen;
		_Mutex 	xmit_mutex;
		dwc_list_link_t tx_reqs; 
		dwc_list_link_t	tx_act_reqs;
		u32		tx_dropped;
		struct usb_queue 	tx_frame_pending_que;
		/*receive (debuf_poolpends on host)*/
		_Sema	recv_sema;
		xTaskHandle	recv_task;
		_Mutex	recv_mutex;
		dwc_list_link_t	rx_reqs;
		dwc_list_link_t	rx_act_reqs;
		u32 	rx_dropped;
		u8		rx_qlen;
		struct usb_queue	rx_frame_pending_que;
#ifdef CONFIG_RX_AGGREGATION
#if CONFIG_RX_TIMER
	#if CONFIG_RX_HW_TIMER
		gtimer_t	rx_agg_timer;
	#elif CONFIG_RX_SW_TIMER
		PRTL_TIMER	rx_agg_timer;
	#endif
		u8		timer_scheduled;
		xTaskHandle timer_task;
		_Sema	timer_sema;
#endif
#if CONFIG_USE_GDMA
		gdma_t  gdma;
#endif 
#endif
	// define fucntion point to remove resource in ram code
	void (*remove_all)(void);
};


extern _LONG_CALL_ int usb_eth_init(void);
extern _LONG_CALL_ int usb_eth_deinit(void);
void _LONG_CALL_ usb_eth_rx(void *pPackage, u16 pPktsize, u8 CmdType);
#endif