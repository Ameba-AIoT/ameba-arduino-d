/* ==========================================================================
 * $File: //dwh/usb_iip/dev/software/otg/linux/drivers/dwc_otg_core_if.h $
 * $Revision: #15 $
 * $Date: 2012/12/10 $
 * $Change: 2123206 $
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
#if !defined(__DWC_CORE_IF_H__)
#define __DWC_CORE_IF_H__

#include "ameba_otg.h"
#include "dwc_os.h"

/** @file
 * This file defines DWC_OTG Core API
 */

struct dwc_otg_core_if;
typedef struct dwc_otg_core_if dwc_otg_core_if_t;

/** Maximum number of Periodic FIFOs */
#define MAX_PERIO_FIFOS 15
/** Maximum number of Periodic FIFOs */
#define MAX_TX_FIFOS 15

/** Maximum number of Endpoints/HostChannels */
#define MAX_EPS_CHANNELS 8

extern _LONG_CALL_ dwc_otg_core_if_t *dwc_otg_cil_init(const uint32_t * _reg_base_addr);
extern _LONG_CALL_ void dwc_otg_core_init(dwc_otg_core_if_t * _core_if);
extern _LONG_CALL_ void dwc_otg_cil_remove(dwc_otg_core_if_t * _core_if);

extern _LONG_CALL_ void dwc_otg_enable_global_interrupts(dwc_otg_core_if_t * _core_if);
extern _LONG_CALL_ void dwc_otg_disable_global_interrupts(dwc_otg_core_if_t * _core_if);

extern _LONG_CALL_ uint8_t dwc_otg_is_device_mode(dwc_otg_core_if_t * _core_if);
extern _LONG_CALL_ uint8_t dwc_otg_is_host_mode(dwc_otg_core_if_t * _core_if);

/** This function should be called on every hardware interrupt. */
extern _LONG_CALL_ int32_t dwc_otg_handle_common_intr(void *otg_dev);

/** @name OTG Core Parameters */
/** @{ */

/**
 * Specifies the OTG capabilities. The driver will automatically
 * detect the value for this parameter if none is specified.
 * 0 - HNP and SRP capable (default)
 * 1 - SRP Only capable
 * 2 - No HNP/SRP capable
 */
extern _LONG_CALL_ int dwc_otg_set_param_otg_cap(dwc_otg_core_if_t * core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_otg_cap(dwc_otg_core_if_t * core_if);
#define DWC_OTG_CAP_PARAM_HNP_SRP_CAPABLE 0
#define DWC_OTG_CAP_PARAM_SRP_ONLY_CAPABLE 1
#define DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE 2
#define dwc_param_otg_cap_default DWC_OTG_CAP_PARAM_HNP_SRP_CAPABLE

/**
 * Specifies the maximum speed of operation in host and device mode.
 * The actual speed depends on the speed of the attached device and
 * the value of phy_type. The actual speed depends on the speed of the
 * attached device.
 * 0 - High Speed (default)
 * 1 - Full Speed
 */
extern _LONG_CALL_ int dwc_otg_set_param_speed(dwc_otg_core_if_t * core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_speed(dwc_otg_core_if_t * core_if);
#if !defined(USB_DEV_FULL_SPEED)
#define dwc_param_speed_default 0
#else
#define dwc_param_speed_default 1
#endif

#define DWC_SPEED_PARAM_HIGH 0
#define DWC_SPEED_PARAM_FULL 1

/** Specifies whether low power mode is supported when attached
 *	to a Full Speed or Low Speed device in host mode.
 * 0 - Don't support low power mode (default)
 * 1 - Support low power mode
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_support_fs_ls_low_power(dwc_otg_core_if_t *
							  core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_support_fs_ls_low_power(dwc_otg_core_if_t
							      * core_if);
#define dwc_param_host_support_fs_ls_low_power_default 0

/** Specifies the PHY clock rate in low power mode when connected to a
 * Low Speed device in host mode. This parameter is applicable only if
 * HOST_SUPPORT_FS_LS_LOW_POWER is enabled. If PHY_TYPE is set to FS
 * then defaults to 6 MHZ otherwise 48 MHZ.
 *
 * 0 - 48 MHz
 * 1 - 6 MHz
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_ls_low_power_phy_clk(dwc_otg_core_if_t *
						       core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_ls_low_power_phy_clk(dwc_otg_core_if_t *
							   core_if);
#define dwc_param_host_ls_low_power_phy_clk_default 0
#define DWC_HOST_LS_LOW_POWER_PHY_CLK_PARAM_48MHZ 0
#define DWC_HOST_LS_LOW_POWER_PHY_CLK_PARAM_6MHZ 1

/**
 * 0 - Use cC FIFO size parameters
 * 1 - Allow dynamic FIFO sizing (default)
 */
extern _LONG_CALL_ int dwc_otg_set_param_enable_dynamic_fifo(dwc_otg_core_if_t * core_if,
						 int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_enable_dynamic_fifo(dwc_otg_core_if_t *
						     core_if);
#define dwc_param_enable_dynamic_fifo_default 1

/** Number of 4-byte words in the Rx FIFO in device mode when dynamic
 * FIFO sizing is enabled.
 * 16 to 32768 (default 1064)
 */
extern _LONG_CALL_ int dwc_otg_set_param_dev_rx_fifo_size(dwc_otg_core_if_t * core_if,
					      int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_dev_rx_fifo_size(dwc_otg_core_if_t * core_if);
#define dwc_param_dev_rx_fifo_size_default 1064

/** Number of 4-byte words in the non-periodic Tx FIFO in device mode
 * when dynamic FIFO sizing is enabled.
 * 16 to 32768 (default 1024)
 */
extern _LONG_CALL_ int dwc_otg_set_param_dev_nperio_tx_fifo_size(dwc_otg_core_if_t *
						     core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_dev_nperio_tx_fifo_size(dwc_otg_core_if_t *
							 core_if);
#define dwc_param_dev_nperio_tx_fifo_size_default 1024

/** Number of 4-byte words in each of the periodic Tx FIFOs in device
 * mode when dynamic FIFO sizing is enabled.
 * 4 to 768 (default 256)
 */
extern _LONG_CALL_ int dwc_otg_set_param_dev_perio_tx_fifo_size(dwc_otg_core_if_t * core_if,
						    int32_t val, int fifo_num);
extern _LONG_CALL_ int32_t dwc_otg_get_param_dev_perio_tx_fifo_size(dwc_otg_core_if_t *
							core_if, int fifo_num);
#define dwc_param_dev_perio_tx_fifo_size_default 256

/** Number of 4-byte words in the Rx FIFO in host mode when dynamic
 * FIFO sizing is enabled.
 * 16 to 32768 (default 1024)
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_rx_fifo_size(dwc_otg_core_if_t * core_if,
					       int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_rx_fifo_size(dwc_otg_core_if_t * core_if);
#define dwc_param_host_rx_fifo_size_default 512

/** Number of 4-byte words in the non-periodic Tx FIFO in host mode
 * when Dynamic FIFO sizing is enabled in the core.
 * 16 to 32768 (default 1024)
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_nperio_tx_fifo_size(dwc_otg_core_if_t *
						      core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_nperio_tx_fifo_size(dwc_otg_core_if_t *
							  core_if);
#define dwc_param_host_nperio_tx_fifo_size_default 1024

/** Number of 4-byte words in the host periodic Tx FIFO when dynamic
 * FIFO sizing is enabled.
 * 16 to 32768 (default 1024)
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_perio_tx_fifo_size(dwc_otg_core_if_t *
						     core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_perio_tx_fifo_size(dwc_otg_core_if_t *
							 core_if);
#define dwc_param_host_perio_tx_fifo_size_default 1024

/** The maximum transfer size supported in bytes.
 * 2047 to 65,535  (default 65,535)
 */
extern _LONG_CALL_ int dwc_otg_set_param_max_transfer_size(dwc_otg_core_if_t * core_if,
					       int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_max_transfer_size(dwc_otg_core_if_t * core_if);
#define dwc_param_max_transfer_size_default 65535

/** The maximum number of packets in a transfer.
 * 15 to 511  (default 511)
 */
extern _LONG_CALL_ int dwc_otg_set_param_max_packet_count(dwc_otg_core_if_t * core_if,
					      int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_max_packet_count(dwc_otg_core_if_t * core_if);
#define dwc_param_max_packet_count_default 511

/** The number of host channel registers to use.
 * 1 to 16 (default 12)
 * Note: The FPGA configuration supports a maximum of 12 host channels.
 */
extern _LONG_CALL_ int dwc_otg_set_param_host_channels(dwc_otg_core_if_t * core_if,
					   int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_host_channels(dwc_otg_core_if_t * core_if);
#define dwc_param_host_channels_default 7

/** The number of endpoints in addition to EP0 available for device
 * mode operations.
 * 1 to 15 (default 6 IN and OUT)
 * Note: The FPGA configuration supports a maximum of 6 IN and OUT
 * endpoints in addition to EP0.
 */
extern _LONG_CALL_ int dwc_otg_set_param_dev_endpoints(dwc_otg_core_if_t * core_if,
					   int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_dev_endpoints(dwc_otg_core_if_t * core_if);
#define dwc_param_dev_endpoints_default 5

extern _LONG_CALL_ int dwc_otg_set_param_ts_dline(dwc_otg_core_if_t * core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_ts_dline(dwc_otg_core_if_t * core_if);
#define dwc_param_ts_dline_default 0

/**
 * Specifies whether PTI enhancement is enabled
 */
extern _LONG_CALL_ int dwc_otg_set_param_pti_enable(dwc_otg_core_if_t * core_if,
					int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_pti_enable(dwc_otg_core_if_t * core_if);
#define dwc_param_pti_enable_default 0

extern _LONG_CALL_ int dwc_otg_set_param_power_down(dwc_otg_core_if_t * core_if,
					int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_power_down(dwc_otg_core_if_t * core_if);
#define dwc_param_power_down_default 0

extern _LONG_CALL_ int dwc_otg_set_param_reload_ctl(dwc_otg_core_if_t * core_if,
					int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_reload_ctl(dwc_otg_core_if_t * core_if);
#define dwc_param_reload_ctl_default 0

extern _LONG_CALL_ int dwc_otg_set_param_ahb_single(dwc_otg_core_if_t * core_if,
					int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_ahb_single(dwc_otg_core_if_t * core_if);
#define dwc_param_ahb_single_default 0

extern _LONG_CALL_ int dwc_otg_set_param_otg_ver(dwc_otg_core_if_t * core_if, int32_t val);
extern _LONG_CALL_ int32_t dwc_otg_get_param_otg_ver(dwc_otg_core_if_t * core_if);
#define dwc_param_otg_ver_default 0

/** @} */

/** @name Access to registers and bit-fields */

/**
 * Dump core registers and SPRAM
 */
extern _LONG_CALL_ void dwc_otg_dump_host_registers(dwc_otg_core_if_t * _core_if);
extern _LONG_CALL_ void dwc_otg_dump_global_registers(dwc_otg_core_if_t * _core_if);

/**
 * Get host negotiation status.
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_hnpstatus(dwc_otg_core_if_t * core_if);

/**
 * Get srp status
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_srpstatus(dwc_otg_core_if_t * core_if);

/**
 * Set hnpreq bit in the GOTGCTL register.
 */
extern _LONG_CALL_ void dwc_otg_set_hnpreq(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get Content of SNPSID register.
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_gsnpsid(dwc_otg_core_if_t * core_if);

/**
 * Get current mode.
 * Returns 0 if in device mode, and 1 if in host mode.
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_mode(dwc_otg_core_if_t * core_if);

/**
 * Get value of hnpcapable field in the GUSBCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_hnpcapable(dwc_otg_core_if_t * core_if);
/**
 * Set value of hnpcapable field in the GUSBCFG register
 */
extern _LONG_CALL_ void dwc_otg_set_hnpcapable(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of srpcapable field in the GUSBCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_srpcapable(dwc_otg_core_if_t * core_if);
/**
 * Set value of srpcapable field in the GUSBCFG register
 */
extern _LONG_CALL_ void dwc_otg_set_srpcapable(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of devspeed field in the DCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_devspeed(dwc_otg_core_if_t * core_if);
/**
 * Set value of devspeed field in the DCFG register
 */
extern void dwc_otg_set_devspeed(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get the value of busconnected field from the HPRT0 register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_busconnected(dwc_otg_core_if_t * core_if);

/**
 * Gets the device enumeration Speed.
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_enumspeed(dwc_otg_core_if_t * core_if);

/**
 * Get value of prtpwr field from the HPRT0 register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_prtpower(dwc_otg_core_if_t * core_if);

/**
 * Get value of flag indicating core state - hibernated or not
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_core_state(dwc_otg_core_if_t * core_if);

/**
 * Set value of prtpwr field from the HPRT0 register
 */
extern _LONG_CALL_ void dwc_otg_set_prtpower(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of prtsusp field from the HPRT0 regsiter
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_prtsuspend(dwc_otg_core_if_t * core_if);
/**
 * Set value of prtpwr field from the HPRT0 register
 */
extern _LONG_CALL_ void dwc_otg_set_prtsuspend(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of ModeChTimEn field from the HCFG regsiter
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_mode_ch_tim(dwc_otg_core_if_t * core_if);
/**
 * Set value of ModeChTimEn field from the HCFG regsiter
 */
extern _LONG_CALL_ void dwc_otg_set_mode_ch_tim(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of Fram Interval field from the HFIR regsiter
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_fr_interval(dwc_otg_core_if_t * core_if);
/**
 * Set value of Frame Interval field from the HFIR regsiter
 */
extern _LONG_CALL_ void dwc_otg_set_fr_interval(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Set value of prtres field from the HPRT0 register
 *FIXME Remove?
 */
extern _LONG_CALL_ void dwc_otg_set_prtresume(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of prt_sleep_sts field from the GLPMCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_lpm_portsleepstatus(dwc_otg_core_if_t * core_if);

/**
 * Get value of appl_resp field from the GLPMCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_lpmresponse(dwc_otg_core_if_t * core_if);
/**
 * Set value of appl_resp field from the GLPMCFG register
 */
extern _LONG_CALL_ void dwc_otg_set_lpmresponse(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of hsic_connect field from the GLPMCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_hsic_connect(dwc_otg_core_if_t * core_if);
/**
 * Set value of hsic_connect field from the GLPMCFG register
 */
extern _LONG_CALL_ void dwc_otg_set_hsic_connect(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * Get value of inv_sel_hsic field from the GLPMCFG register.
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_inv_sel_hsic(dwc_otg_core_if_t * core_if);
/**
 * Set value of inv_sel_hsic field from the GLPMFG register.
 */
extern _LONG_CALL_ void dwc_otg_set_inv_sel_hsic(dwc_otg_core_if_t * core_if, uint32_t val);

/*
 * Some functions for accessing registers
 */

/**
 *  GOTGCTL register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_gotgctl(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_gotgctl(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GUSBCFG register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_gusbcfg(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_gusbcfg(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GRXFSIZ register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_grxfsiz(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_grxfsiz(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GNPTXFSIZ register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_gnptxfsiz(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_gnptxfsiz(dwc_otg_core_if_t * core_if, uint32_t val);

extern _LONG_CALL_ uint32_t dwc_otg_get_gpvndctl(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_gpvndctl(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GGPIO register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_ggpio(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_ggpio(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GUID register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_guid(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_guid(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * HPRT0 register
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_hprt0(dwc_otg_core_if_t * core_if);
extern _LONG_CALL_ void dwc_otg_set_hprt0(dwc_otg_core_if_t * core_if, uint32_t val);

/**
 * GHPTXFSIZE
 */
extern _LONG_CALL_ uint32_t dwc_otg_get_hptxfsiz(dwc_otg_core_if_t * core_if);

/** @} */

#endif /* __DWC_CORE_IF_H__ */
