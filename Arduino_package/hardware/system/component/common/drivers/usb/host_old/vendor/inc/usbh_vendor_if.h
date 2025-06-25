#ifndef USBH_VENDOR_IF_H
#define USBH_VENDOR_IF_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_VENDOR

typedef struct {
    int(* attach)(void);
    void(* detach)(void);
} usbh_vendor_usr_cb_t;

int usbh_vendor_init(usbh_vendor_usr_cb_t *cb);
void usbh_vendor_deinit(void);

#endif // CONFIG_USBH_VENDOR

#endif // USBH_VENDOR_IF_H

