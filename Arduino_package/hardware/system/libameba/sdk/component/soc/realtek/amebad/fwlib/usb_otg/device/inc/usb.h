#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include "osdep_service.h"
#include "usb_errno.h"
#include "usb_ch9.h"

enum usb_device_speed {
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH,
    USB_SPEED_VARIABL
};

enum usb_status_t {
    USB_STATUS_INIT = 0,  // Initial status
    USB_STATUS_DETACHED,  // Detached from USB host or charger
    USB_STATUS_ATTACHED   // Attached to USB host
};

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#define USBD_IRQ_THREAD_PRIORITY  (tskIDLE_PRIORITY + 4)

int usb_init(int speed);
void usb_deinit(void);
int usb_get_status(void);

#endif /* _USB_H_ */

