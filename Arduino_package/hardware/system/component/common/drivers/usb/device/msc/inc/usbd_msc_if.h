#ifndef USBD_MSC_IF_H
#define USBD_MSC_IF_H

#include <platform_opts.h>

#ifdef CONFIG_USBD_MSC

#include <stdint.h>

//int usbd_msc_register(void);
int usbd_msc_init(void);
void usbd_msc_deinit(void);

#endif // CONFIG_USBD_MSC

#endif // USBD_MSC_H