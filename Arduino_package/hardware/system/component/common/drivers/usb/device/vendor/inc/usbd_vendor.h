#ifndef USBD_VENDOR_H
#define USBD_VENDOR_H

#include <platform_opts.h>
#ifdef CONFIG_USBD_VENDOR

#include "usb.h"
#include "usb_composite.h"

// Test options
#define CONFIG_USBD_VENDOR_ISO_IN_TEST          0
#define CONFIG_USBD_VENDOR_ISO_OUT_TEST         0
#define CONFIG_USBD_VENDOR_BULK_INOUT_TEST      1
#define CONFIG_USBD_VENDOR_ISO_IN_REQ_ASAP      1
#define CONFIG_USBD_VENDOR_ISO_OUT_REQ_ASAP     0

#define USBD_VENDOR_ISO_REQ_NUM                 2
#define USBD_VENDOR_ISO_OUT_XFER_CNT		    100
#define USBD_VENDOR_ISO_IN_CNT                  100

/* Debug options */
#define USBD_VENDOR_DEBUG                       0
#define USBD_VENDOR_STR                         "VS"
#if USBD_VENDOR_DEBUG
#define USBD_VENDOR_INFO(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBD_VENDOR_STR, __FUNCTION__, ## args)
#define USBD_VENDOR_WARN(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBD_VENDOR_STR, __FUNCTION__, ## args)
#define USBD_VENDOR_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBD_VENDOR_STR, __FUNCTION__, ## args)
#define USBD_VENDOR_ENTER                       printf("\n\r[%s]%s: =>", USBD_VENDOR_STR, __FUNCTION__)
#define USBD_VENDOR_EXIT                        printf("\n\r[%s]%s: <=", USBD_VENDOR_STR, __FUNCTION__)
#define USBD_VENDOR_EXIT_ERR                    printf("\n\r[%s]%s: ERR <=", USBD_VENDOR_STR, __FUNCTION__)
#else
#define USBD_VENDOR_INFO(fmt, args...)
#define USBD_VENDOR_WARN(fmt, args...)
#define USBD_VENDOR_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBD_VENDOR_STR, __FUNCTION__, ## args)
#define USBD_VENDOR_ENTER
#define USBD_VENDOR_EXIT
#define USBD_VENDOR_EXIT_ERR
#endif

struct usb_vendor_common_t;
struct usb_vendor_device_t;

struct usb_vendor_common_t {
    struct usb_vendor_device_t  *vendev;
    struct usb_gadget   *gadget;
    struct usb_ep       *ep0;
    struct usb_request  *req0;
};

struct usb_vendor_device_t {
    struct usb_vendor_common_t   *common;
    struct usb_function func;
    struct usb_ep       *bulk_in_ep;
    struct usb_ep       *bulk_out_ep;
    struct usb_ep       *iso_in_ep;
    struct usb_ep       *iso_out_ep;

    struct usb_request *bulk_out_req;
    struct usb_request *bulk_in_req;

    struct usb_iso_request *iso_out_req;
    u8 *iso_out_req_buf[USBD_VENDOR_ISO_REQ_NUM];

    struct usb_iso_request *iso_in_req;
    u8 *iso_in_req_buf[USBD_VENDOR_ISO_REQ_NUM];
};

typedef struct {
    u8 pos;
    u8 data;
} usb_vendor_iso_out_record_t;

extern struct usb_gadget_strings *vendor_gadget_strings[];
extern struct usb_device_descriptor vendor_device_desc;
extern struct usb_config_descriptor vendor_config_desc;
extern struct usb_interface_descriptor vendor_intf0_desc;
extern struct usb_interface_descriptor vendor_intf1_desc;
extern struct usb_endpoint_descriptor vendor_bulk_source_desc_FS;
extern struct usb_endpoint_descriptor vendor_bulk_sink_desc_FS;
extern struct usb_endpoint_descriptor vendor_bulk_source_desc_HS;
extern struct usb_endpoint_descriptor vendor_bulk_sink_desc_HS;
extern struct usb_endpoint_descriptor vendor_iso_source_desc_FS;
extern struct usb_endpoint_descriptor vendor_iso_sink_desc_FS;
extern struct usb_endpoint_descriptor vendor_iso_source_desc_HS;
extern struct usb_endpoint_descriptor vendor_iso_sink_desc_HS;
extern struct usb_descriptor_header *vendor_descriptors_FS[];
extern struct usb_descriptor_header *vendor_descriptors_HS[];

#endif // CONFIG_USBD_VENDOR

#endif // USBD_VENDOR_H
