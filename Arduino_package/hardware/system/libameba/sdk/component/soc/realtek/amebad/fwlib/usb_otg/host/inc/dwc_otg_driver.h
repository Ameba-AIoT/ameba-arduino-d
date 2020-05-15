/* ==========================================================================
 * $File: //dwh/usb_iip/dev/software/otg/linux/drivers/dwc_otg_driver.h $
 * $Revision: #19 $
 * $Date: 2010/11/15 $
 * $Change: 1627671 $
 *
 * Synopsys HS OTG Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 * 
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 * 
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */

#ifndef __DWC_OTG_DRIVER_H__
#define __DWC_OTG_DRIVER_H__

/** @file
 * This file contains the interface to the Linux driver.
 */
#include "ameba_otg.h"
#include "dwc_otg_core_if.h"
#include "osdep_service.h"

/* Type declarations */
struct dwc_otg_hcd;

/**
 * This structure is a wrapper that encapsulates the driver components used to
 * manage a single DWC_otg controller.
 */
typedef struct dwc_otg_device {
	/** Structure containing OS-dependent stuff. KEEP THIS STRUCT AT THE
	 * VERY BEGINNING OF THE DEVICE STRUCT. OSes such as FreeBSD and NetBSD
	 * require this. */
	//struct os_dependent os_dep;
    /** Base address returned from ioremap() */
	void *base;
	uint32_t reg_offset;
	/** Pointer to the core interface structure. */
	dwc_otg_core_if_t *core_if;
    
	/** Pointer to the HCD structure. */
	struct dwc_otg_hcd *hcd;

	/** Flag to indicate whether the common IRQ handler is installed. */
	uint8_t common_irq_installed;

} dwc_otg_device_t;

typedef struct USB_OTG_DRV_ADP {
    dwc_otg_device_t    *otgdev;
#if !TASK_SCHEDULER_DISABLED
            _sema       Sema;
#else
            u32         Sema;
#endif
#if !TASK_SCHEDULER_DISABLED
            xTaskHandle OTGTask;
#else
        u32         OTGTask;
#endif

}USB_OTG_DRV_ADP,*PUSB_OTG_DRV_ADP;



typedef struct _DWC_OTG_ADAPTER_ {
    u32                 temp0;
    dwc_otg_device_t    *otgdev;
    u8                  TestItem;
}DWC_OTG_ADAPTER, *PDWC_OTG_ADAPTER;
void dwc_otg_disable_irq(void);
void dwc_otg_enable_irq(void);
int usb_hcd_post_init(void);
int dwc_otg_driver_remove_common(void);
int dwc_otg_boot_deinit(void);
int dwc_otg_driver_probe_sub(void);
int dwc_otg_boot_normal(void);

#endif
