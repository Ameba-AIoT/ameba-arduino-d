#ifndef USBH_VENDOR_H
#define USBH_VENDOR_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_VENDOR

#include "usb.h"
#include "usbh_vendor_if.h"

#define CONFIG_USBH_VENDOR_CTRL_TEST		0
#define CONFIG_USBH_VENDOR_BULK_TEST		1
#define CONFIG_USBH_VENDOR_ISO_IN_TEST		0
#define CONFIG_USBH_VENDOR_ISO_OUT_TEST	0

#define USBH_VENDOR_ISO_IN_BUF_SIZE  128
#define USBH_VENDOR_ISO_OUT_BUF_SIZE 128
#define USBH_VENDOR_ISO_IN_CNT       100
#define USBH_VENDOR_ISO_OUT_CNT       100

#define USBH_VENDOR_TASK_PRIORITY    2

/* Debug options */
#define USBH_VENDOR_DEBUG                       0
#define USBH_VENDOR_STR                         "VS"
#if USBH_VENDOR_DEBUG
#define USBH_VENDOR_INFO(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBH_VENDOR_STR, __FUNCTION__, ## args)
#define USBH_VENDOR_WARN(fmt, args...)          printf("\n\r[%s]%s: " fmt, USBH_VENDOR_STR, __FUNCTION__, ## args)
#define USBH_VENDOR_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBH_VENDOR_STR, __FUNCTION__, ## args)
#define USBH_VENDOR_ENTER                       printf("\n\r[%s]%s: =>", USBH_VENDOR_STR, __FUNCTION__)
#define USBH_VENDOR_EXIT                        printf("\n\r[%s]%s: <=", USBH_VENDOR_STR, __FUNCTION__)
#define USBH_VENDOR_EXIT_ERR                    printf("\n\r[%s]%s: ERR <=", USBH_VENDOR_STR, __FUNCTION__)
#else
#define USBH_VENDOR_INFO(fmt, args...)
#define USBH_VENDOR_WARN(fmt, args...)
#define USBH_VENDOR_ERROR(fmt, args...)         printf("\n\r[%s]%s: " fmt, USBH_VENDOR_STR, __FUNCTION__, ## args)
#define USBH_VENDOR_ENTER
#define USBH_VENDOR_EXIT
#define USBH_VENDOR_EXIT_ERR
#endif

struct usbtest_info {
    const char *name;
    u8          ep_in;      /* bulk/intr source */
    u8          ep_out;     /* bulk/intr sink */
    unsigned    autoconf: 1;
    unsigned    ctrl_out: 1;
    unsigned    iso: 1;     /* try iso in/out */
    unsigned    intr: 1;    /* try interrupt in/out */
    int         alt;
    usbh_vendor_usr_cb_t *cb;
};

struct usbtest_dev {
    struct usb_interface    *intf;
    struct usbtest_info *info;
    int         in_pipe;
    int         out_pipe;
    int         in_pipe_addr;
    int         out_pipe_addr;
    int         in_iso_pipe;
    int         out_iso_pipe;
    int         in_iso_addr;
    int         out_iso_addr;
    int         in_int_pipe;
    int         out_int_pipe;
    struct usb_endpoint_descriptor  *iso_in, *iso_out;
    struct usb_endpoint_descriptor  *int_in, *int_out;
    u8          *buf;
};

#endif // CONFIG_USBH_VENDOR

#endif // USBH_VENDOR_H

