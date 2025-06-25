#ifndef __UNUSUAL_USBAT_H
#define __UNUSUAL_USBAT_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

#if defined(CONFIG_USB_STORAGE_USBAT) || \
		defined(CONFIG_USB_STORAGE_USBAT_MODULE)

UNUSUAL_DEV(  0x03f0, 0x0207, 0x0001, 0x0001,
		"HP",
		"CD-Writer+ 8200e",
		USB_SC_8070, USB_PR_USBAT, init_usbat_cd, 0),

UNUSUAL_DEV(  0x03f0, 0x0307, 0x0001, 0x0001,
		"HP",
		"CD-Writer+ CD-4e",
		USB_SC_8070, USB_PR_USBAT, init_usbat_cd, 0),

UNUSUAL_DEV(  0x04e6, 0x1010, 0x0000, 0x9999,
		"Shuttle/SCM",
		"USBAT-02",
		USB_SC_SCSI, USB_PR_USBAT, init_usbat_flash,
		US_FL_SINGLE_LUN),

UNUSUAL_DEV(  0x0781, 0x0005, 0x0005, 0x0005,
		"Sandisk",
		"ImageMate SDDR-05b",
		USB_SC_SCSI, USB_PR_USBAT, init_usbat_flash,
		US_FL_SINGLE_LUN),

#endif /* defined(CONFIG_USB_STORAGE_USBAT) || ... */

#endif // CONFIG_USBH_MSC

#endif // __UNUSUAL_USBAT_H

