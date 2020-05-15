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

/* Type declarations */
struct dwc_otg_pcd;

/**
 * This structure is a wrapper that encapsulates the driver components used to
 * manage a single DWC_otg controller.
 */
typedef struct dwc_otg_device {
    /** Base address returned from ioremap() */
    void *base;
    uint32_t reg_offset;
    /** Pointer to the core interface structure. */
    dwc_otg_core_if_t *core_if;

    /** Pointer to the PCD structure. */
    struct dwc_otg_pcd *pcd;

} dwc_otg_device_t;

typedef struct {
    dwc_otg_device_t *otg_dev;
    _sema otg_sema;
    struct task_struct otg_task;
} dwc_otg_adaptor_t, *dwc_otg_adaptor_ptr_t;

/* the module parameter settings */
struct dwc_otg_driver_module_params {
    int32_t otg_cap;
    int32_t speed;
    int32_t enable_dynamic_fifo;
    int32_t dev_rx_fifo_size;
    int32_t dev_nperio_tx_fifo_size;
    int32_t dev_perio_tx_fifo_size[MAX_PERIO_FIFOS];
    int32_t max_transfer_size;
    int32_t dev_endpoints;
    int32_t ts_dline;
    int32_t pti_enable;
    int32_t power_down;
    int32_t reload_ctl;
    int32_t ahb_single;
    int32_t otg_ver;
};

void dwc_otg_disable_irq(void);
void dwc_otg_enable_irq(void);

#endif
