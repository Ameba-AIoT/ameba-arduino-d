/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef DWC_OTG_COMMON_H
#define DWC_OTG_COMMON_H

#include <stdarg.h>

#include "basic_types.h"
#include "diag.h"
#include "dwc_os.h"

enum usb_power_state_t {
    USB_PDN = 0,
    USB_RTK_PWRON_SEQ_DONE,
    USB_SW_INIT,
    USB_ATTACHED,
    USB_ADDRESSED,
    USB_CONFIGURATED,
    USB_SUSPEND_STATE,
    USB_ACTIVE
};

enum usb_init_status_t {
	USB_INIT_NONE = -1,
	USB_INIT_OK = 0,
	USB_INIT_FAIL = 1
};

typedef struct {
    int usb_power_state;   // refer to usb_power_state_t
    int usb_init_status;   // refer to usb_init_status_t
    int usb_status;        // refer to usb_status_t
    int usb_speed;         // refer to usb_device_speed
} usb_info_t;

#define TIMER_MAX_DELAY    0xFFFFFFFF

#define GFP_KERNEL         1
#define GFP_ATOMIC         1

// big enough to hold our biggest descriptor
#define USB_COMP_EP0_BUFSIZ         (1024 + 24)

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr)-(size_t)(&((type *)0)->member)))
#endif

/*
 * The USB records contain some unaligned little-endian word
 * components.  The U[SG]ETW macros take care of both the alignment
 * and endian problem and should always be used to access non-byte
 * values.
 */
//#define UGETW(w) ((w)[0] | ((w)[1] << 8))
#define UGETW(w) (w)

extern usb_info_t usb_info;

#endif // DWC_OTG_COMMON_H

