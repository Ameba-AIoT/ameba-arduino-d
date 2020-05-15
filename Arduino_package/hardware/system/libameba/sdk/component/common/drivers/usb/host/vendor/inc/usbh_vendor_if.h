#ifndef USBH_VENDOR_IF_H
#define USBH_VENDOR_IF_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_VENDOR

int usbh_vendor_init(void);
void usbh_vendor_deinit(void);

#endif // CONFIG_USBH_VENDOR

#endif // USBH_VENDOR_IF_H

