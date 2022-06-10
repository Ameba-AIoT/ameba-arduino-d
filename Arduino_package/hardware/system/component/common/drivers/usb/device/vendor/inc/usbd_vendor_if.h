#ifndef USBD_VENDOR_IF_H
#define USBD_VENDOR_IF_H

#include <platform_opts.h>
#ifdef CONFIG_USBD_VENDOR

#include "usb.h"

int usbd_vendor_init(void);
void usbd_vendor_deinit(void);

#endif // CONFIG_USBD_VENDOR

#endif // USBD_VENDOR_IF_H