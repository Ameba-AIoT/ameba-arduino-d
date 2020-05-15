/*
 * <linux/usb_gadget.h>
 *
 * We call the USB code inside a Linux-based peripheral device a "gadget"
 * driver, except for the hardware-specific bus glue.  One USB host can
 * master many USB gadgets, but the gadgets are only slaved to one host.
 *
 *
 * (C) Copyright 2002-2004 by David Brownell
 * All Rights Reserved.
 *
 * This software is licensed under the GNU GPL version 2.
 */

#ifndef __USB_GADGET_H
#define __USB_GADGET_H

#include "usb.h"
#include "ameba_otg.h"
#include "ameba_otg_zero.h"
#include "dwc_list.h"
#include "dwc_otg_pcd.h"
#include "dwc_otg_common.h"
#include "dwc_otg_pcd_if.h"

/*
 * USB function drivers should return USB_GADGET_DELAYED_STATUS if they
 * wish to delay the data/status stages of the control transfer till they
 * are ready.
 */
#define USB_GADGET_DELAYED_STATUS   0x7FFF

typedef unsigned int gfp_t;

// Pre-declaration, to fix compiling warnings
struct usb_request;
struct usb_ep_ops;

//struct usb_ep;
/**
 * struct usb_ep - device side representation of USB endpoint
 * @name:identifier for the endpoint, such as "ep-a" or "ep9in-bulk"
 * @ops: Function pointers used to access hardware-specific operations.
 * @ep_list:the gadget's ep_list holds all of its endpoints
 * @maxpacket:The maximum packet size used on this endpoint.  The initial
 *  value can sometimes be reduced (hardware allowing), according to
 *      the endpoint descriptor used to configure the endpoint.
 * @driver_data:for use by the gadget driver.  all other fields are
 *  read-only to gadget drivers.
 *
 * the bus controller driver lists all the general purpose endpoints in
 * gadget->ep_list.  the control endpoint (gadget->ep0) is not in that list,
 * and is accessed only in response to a driver setup() callback.
 */
struct usb_ep {
    void *driver_data;
    const char *name;
    const struct usb_ep_ops *ops;
    dwc_list_link_t ep_list;
    unsigned maxpacket: 16;
    const struct usb_endpoint_descriptor *desc;
};

typedef void (*usb_req_complete_t)(struct usb_ep *, struct usb_request *);

/**
 * struct usb_request - describes one i/o request
 * @buf: Buffer used for data.  Always provide this; some controllers
 *  only use PIO, or don't use DMA for some endpoints.
 * @dma: DMA address corresponding to 'buf'.  If you don't set this
 *  field, and the usb controller needs one, it is responsible
 *  for mapping and unmapping the buffer.
 * @length: Length of that data
 * @no_interrupt: If true, hints that no completion irq is needed.
 *  Helpful sometimes with deep request queues that are handled
 *  directly by DMA controllers.
 * @zero: If true, when writing data, makes the last packet be "short"
 *     by adding a zero length packet as needed;
 * @short_not_ok: When reading data, makes short packets be
 *     treated as errors (queue stops advancing till cleanup).
 * @complete: Function called when request completes, so this request and
 *  its buffer may be re-used.
 *  Reads terminate with a short packet, or when the buffer fills,
 *  whichever comes first.  When writes terminate, some data bytes
 *  will usually still be in flight (often in a hardware fifo).
 *  Errors (for reads or writes) stop the queue from advancing
 *  until the completion function returns, so that any transfers
 *  invalidated by the error may first be dequeued.
 * @context: For use by the completion callback
 * @list: For use by the gadget driver.
 * @status: Reports completion code, zero or a negative errno.
 *  Normally, faults block the transfer queue from advancing until
 *  the completion callback returns.
 *  Code "-ESHUTDOWN" indicates completion caused by device disconnect,
 *  or when the driver disabled the endpoint.
 * @actual: Reports bytes transferred to/from the buffer.  For reads (OUT
 *  transfers) this may be less than the requested length.  If the
 *  short_not_ok flag is set, short reads are treated as errors
 *  even when status otherwise indicates successful completion.
 *  Note that for writes (IN transfers) some data bytes may still
 *  reside in a device-side FIFO when the request is reported as
 *  complete.
 *
 * These are allocated/freed through the endpoint they're used with.  The
 * hardware's driver can add extra per-request data to the memory it returns,
 * which often avoids separate memory allocations (potential failures),
 * later when the request is queued.
 *
 * Request flags affect request handling, such as whether a zero length
 * packet is written (the "zero" flag), whether a short read should be
 * treated as an error (blocking request queue advance, the "short_not_ok"
 * flag), or hinting that an interrupt is not required (the "no_interrupt"
 * flag, for use with deep request queues).
 *
 * Bulk endpoints can use any size buffers, and can also be used for interrupt
 * transfers. interrupt-only endpoints can be much less functional.
 */
// NOTE this is analagous to 'struct urb' on the host side,
// except that it's thinner and promotes more pre-allocation.

struct usb_request {
    void *buf;
    unsigned length;
    dma_addr_t dma;

    usb_req_complete_t complete;
    void *context;
    _list list;
    int status;
    unsigned actual;
    unsigned direct;

    unsigned no_interrupt: 1; //not used
    unsigned zero: 1;
    unsigned short_not_ok: 1; // not used
};

// add for iso
struct usb_iso_request {
    void *buf0;
    void *buf1;
    dma_addr_t dma0;
    dma_addr_t dma1;
    /* Time Interval(number of frame) for data exchange, i.e, interval for calling process_buffer(==> a function). */
    /* This variable sould be divisible to bInterval (interms of number of frames, not power of 2) */
    uint32_t buf_proc_intrvl;
    unsigned no_interrupt: 1;
    unsigned zero: 1;
    unsigned short_not_ok: 1;
    uint32_t sync_frame;
    uint32_t data_per_frame;
    uint32_t data_per_frame1;
    uint32_t data_pattern_frame;
    uint32_t start_frame;
    uint32_t flags;
    void (*process_buffer)(struct usb_ep *, struct usb_iso_request *);
    void *context;
    int status;
    struct usb_gadget_iso_packet_descriptor *iso_packet_desc0;
    struct usb_gadget_iso_packet_descriptor *iso_packet_desc1;
    uint32_t proc_buf_num;
};


// add for iso
struct usb_gadget_iso_packet_descriptor {
    unsigned int offset;
    unsigned int length; /* expected length */
    unsigned int actual_length;
    unsigned int status;
};

/*-------------------------------------------------------------------------*/

/* endpoint-specific parts of the api to the usb controller hardware.
 * unlike the urb model, (de)multiplexing layers are not required.
 * (so this api could slash overhead if used on the host side...)
 *
 * note that device side usb controllers commonly differ in how many
 * endpoints they support, as well as their capabilities.
 */
struct usb_ep_ops {
    int (*enable)(struct usb_ep *ep, const struct usb_endpoint_descriptor *desc);
    int (*disable)(struct usb_ep *ep);

    struct usb_request *(*alloc_request)(struct usb_ep *ep, gfp_t gfp_flags);
    void (*free_request)(struct usb_ep *ep, struct usb_request *req);

    int (*queue)(struct usb_ep *ep, struct usb_request *req, gfp_t gfp_flags);
    int (*dequeue)(struct usb_ep *ep, struct usb_request *req);

    int (*set_halt)(struct usb_ep *ep, int value);
    int (*fifo_status)(struct usb_ep *ep);
    void (*fifo_flush)(struct usb_ep *ep);
    
#ifdef USBD_EN_ISOC
    int (*iso_ep_start)(struct usb_ep *, struct usb_iso_request *, gfp_t);
    int (*iso_ep_stop)(struct usb_ep *, struct usb_iso_request *);
    struct usb_iso_request *(*alloc_iso_request)(struct usb_ep *ep, int packets, gfp_t);
    void (*free_iso_request)(struct usb_ep *ep, struct usb_iso_request *req);
#endif
};

/*-------------------------------------------------------------------------*/
/**
 * struct usb_gadget - represents a usb slave device
 * @ops: Function pointers used to access hardware-specific operations.
 * @ep0: Endpoint zero, used when reading or writing responses to
 *  driver setup() requests
 * @ep_list: List of other endpoints supported by the device.
 * @speed: Speed of current connection to USB host.
 * @is_dualspeed: True if the controller supports both high and full speed
 *  operation.  If it does, the gadget driver must also support both.
 * @is_otg: True if the USB device port uses a Mini-AB jack, so that the
 *  gadget driver must provide a USB OTG descriptor.
 * @is_a_peripheral: False unless is_otg, the "A" end of a USB cable
 *  is in the Mini-AB jack, and HNP has been used to switch roles
 *  so that the "A" device currently acts as A-Peripheral, not A-Host.
 * @a_hnp_support: OTG device feature flag, indicating that the A-Host
 *  supports HNP at this port.
 * @a_alt_hnp_support: OTG device feature flag, indicating that the A-Host
 *  only supports HNP on a different root port.
 * @b_hnp_enable: OTG device feature flag, indicating that the A-Host
 *  enabled HNP support.
 * @name: Identifies the controller hardware type.  Used in diagnostics
 *  and sometimes configuration.
 * @dev: Driver model state for this abstract device.
 *
 * Gadgets have a mostly-portable "gadget driver" implementing device
 * functions, handling all usb configurations and interfaces.  Gadget
 * drivers talk to hardware-specific code indirectly, through ops vectors.
 * That insulates the gadget driver from hardware details, and packages
 * the hardware endpoints through generic i/o queues.  The "usb_gadget"
 * and "usb_ep" interfaces provide that insulation from the hardware.
 *
 * Except for the driver data, all fields in this structure are
 * read-only to the gadget driver.  That driver data is part of the
 * "driver model" infrastructure in 2.6 (and later) kernels, and for
 * earlier systems is grouped in a similar structure that's not known
 * to the rest of the kernel.
 *
 * Values of the three OTG device feature flags are updated before the
 * setup() call corresponding to USB_REQ_SET_CONFIGURATION, and before
 * driver suspend() calls.  They are valid only when is_otg, and when the
 * device is acting as a B-Peripheral (so is_a_peripheral is false).
 */
struct usb_gadget {
    struct usb_ep         *ep0;
    dwc_list_link_t        ep_list; // by jimmy

    enum usb_device_speed  speed;
    enum usb_device_speed  max_speed;
    unsigned               is_dualspeed: 1;
    unsigned               is_otg: 1;
    unsigned               is_a_peripheral: 1;
    unsigned               b_hnp_enable: 1;
    unsigned               a_hnp_support: 1;
    unsigned               a_alt_hnp_support: 1;
    struct zero_dev        dev;
    void                  *driver_data;
    void                  *device;
};

/*-------------------------------------------------------------------------*/

/**
 * struct usb_gadget_driver - driver for usb 'slave' devices
 * @function: String describing the gadget's function
 * @speed: Highest speed the driver handles.
 * @bind: Invoked when the driver is bound to a gadget, usually
 *  after registering the driver.
 *  At that point, ep0 is fully initialized, and ep_list holds
 *  the currently-available endpoints.
 *  Called in a context that permits sleeping.
 * @setup: Invoked for ep0 control requests that aren't handled by
 *  the hardware level driver. Most calls must be handled by
 *  the gadget driver, including descriptor and configuration
 *  management.  The 16 bit members of the setup data are in
 *  USB byte order. Called in_interrupt; this may not sleep.  Driver
 *  queues a response to ep0, or returns negative to stall.
 * @disconnect: Invoked after all transfers have been stopped,
 *  when the host is disconnected.  May be called in_interrupt; this
 *  may not sleep.  Some devices can't detect disconnect, so this might
 *  not be called except as part of controller shutdown.
 * @unbind: Invoked when the driver is unbound from a gadget,
 *  usually from rmmod (after a disconnect is reported).
 *  Called in a context that permits sleeping.
 * @suspend: Invoked on USB suspend.  May be called in_interrupt.
 * @resume: Invoked on USB resume.  May be called in_interrupt.
 * @driver: Driver model state for this driver.
 *
 * Devices are disabled till a gadget driver successfully bind()s, which
 * means the driver will handle setup() requests needed to enumerate (and
 * meet "chapter 9" requirements) then do some useful work.
 *
 * If gadget->is_otg is true, the gadget driver must provide an OTG
 * descriptor during enumeration, or else fail the bind() call.  In such
 * cases, no USB traffic may flow until both bind() returns without
 * having called usb_gadget_disconnect(), and the USB host stack has
 * initialized.
 *
 * Drivers use hardware-specific knowledge to configure the usb hardware.
 * endpoint addressing is only one of several hardware characteristics that
 * are in descriptors the ep0 implementation returns from setup() calls.
 *
 * Except for ep0 implementation, most driver code shouldn't need change to
 * run on top of different usb controllers.  It'll use endpoints set up by
 * that ep0 implementation.
 *
 * The usb controller driver handles a few standard usb requests.  Those
 * include set_address, and feature flags for devices, interfaces, and
 * endpoints (the get_status, set_feature, and clear_feature requests).
 *
 * Accordingly, the driver's setup() callback must always implement all
 * get_descriptor requests, returning at least a device descriptor and
 * a configuration descriptor.  Drivers must make sure the endpoint
 * descriptors match any hardware constraints. Some hardware also constrains
 * other descriptors. (The pxa250 allows only configurations 1, 2, or 3).
 *
 * The driver's setup() callback must also implement set_configuration,
 * and should also implement set_interface, get_configuration, and
 * get_interface.  Setting a configuration (or interface) is where
 * endpoints should be activated or (config 0) shut down.
 *
 * (Note that only the default control endpoint is supported.  Neither
 * hosts nor devices generally support control traffic except to ep0.)
 *
 * Most devices will ignore USB suspend/resume operations, and so will
 * not provide those callbacks.  However, some may need to change modes
 * when the host is not longer directing those activities.  For example,
 * local controls (buttons, dials, etc) may need to be re-enabled since
 * the (remote) host can't do that any longer; or an error state might
 * be cleared, to make the device behave identically whether or not
 * power is maintained.
 */
struct usb_gadget_driver {
    int (*bind)(struct usb_gadget *, struct usb_gadget_driver *);
    void (*unbind)(struct usb_gadget *);
    int (*setup)(struct usb_gadget *,   const struct usb_control_request *);
    void (*disconnect)(struct usb_gadget *);
    void (*suspend)(struct usb_gadget *);
    void (*resume)(struct usb_gadget *);

    void *driver;
};

struct gadget_wrapper {
    dwc_otg_pcd_t *pcd;

    struct usb_gadget gadget;
    struct usb_gadget_driver *driver;

    struct usb_ep ep0;
    struct usb_ep in_ep[16];
    struct usb_ep out_ep[16];
};

/*-------------------------------------------------------------------------*/

/* utility to simplify dealing with string descriptors */

/**
 * struct usb_string - wraps a C string and its USB id
 * @id:the (nonzero) ID for this string
 * @s:the string, in UTF-8 encoding
 *
 * If you're using usb_gadget_get_string(), use this to wrap a string
 * together with its ID.
 */
struct usb_string {
    u8 id;
    const char *s;
};

/**
 * struct usb_gadget_strings - a set of USB strings in a given language
 * @language:identifies the strings' language (0x0409 for en-us)
 * @strings:array of strings with their ids
 *
 * If you're using usb_gadget_get_string(), use this to wrap all the
 * strings for a given language.
 */
struct usb_gadget_strings {
    u16 language;   /* 0x0409 for en-us */
    struct usb_string *strings;
};

#endif  /* __USB_GADGET_H */
