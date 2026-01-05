/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef USBH_HAL_H
#define USBH_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "usb_regs.h"
#include "usb_hal.h"
#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/
#define USBH_HCFG_30_60_MHZ                         0U
#define USBH_HCFG_48_MHZ                            1U
#define USBH_HCFG_6_MHZ                             2U

#define USBH_GRXSTS_PKTSTS_IN                       2U
#define USBH_GRXSTS_PKTSTS_IN_XFER_COMP             3U
#define USBH_GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5U
#define USBH_GRXSTS_PKTSTS_CH_HALTED                7U

/* Exported types ------------------------------------------------------------*/

/* Host channel states definition */
typedef enum {
	USBH_PID_DATA0 = 0U,
	USBH_PID_DATA2 = 1U,
	USBH_PID_DATA1 = 2U,
	USBH_PID_SETUP = 3U
} usbh_pid_t;

/* Host channel PID definition */
typedef enum {
	USBH_HC_IDLE = 0U,
	USBH_HC_XFRC,
	USBH_HC_HALTED,
	USBH_HC_NAK,
	USBH_HC_NYET,
	USBH_HC_STALL,
	USBH_HC_XACTERR,
	USBH_HC_BBLERR,
	USBH_HC_DATATGLERR
} usbh_hc_state_t;

typedef struct {
	u8  *xfer_buff;         /*!< Pointer to transfer buffer */
	u32 dma_addr;           /*!< 32 bits aligned transfer buffer address */
	u32 error_cnt;          /*!< Host channel error count.*/
	u32 xfer_len;           /*!< Current transfer total length */
	u32 xfer_count;         /*!< Partial transfer length in case of multi packet transfer */
	u16 max_packet;         /*!< Endpoint Max packet size, 0~65536 */
	u8  dev_addr;           /*!< USB device address, 1~255 */
	u8  ch_num : 4;             /*!< Host channel number, 1~15 */
	u8  ep_num : 4;             /*!< Endpoint number, 1~15 */
	__IO u8 hc_state : 4;       /*!< Host Channel state, defined in struct usbh_hc_state_t 0~8 */
	__IO u8 urb_state : 3;      /*!< URB state, defined in struct usbh_urb_state_t 0~4 */
	u8  speed : 2;              /*!< Host speed, 0~3 */
	u8  ep_type : 2;            /*!< Endpoint Type, defined in struct usb_ch_ep_type_t 0~3 */
	u8  data_pid : 2;           /*!< Initial data PID, defined in struct usbh_pid_t 0~3 */
	u8  toggle_in : 1;          /*!< IN transfer current toggle flag, 0~1 */
	u8  toggle_out : 1;         /*!< OUT transfer current toggle flag, 0~1 */
	u8  do_ping : 1;            /*!< Enable or disable the use of the PING protocol for HS mode 0~1*/
	u8  ep_is_in : 1;           /*!< Endpoint direction, 0~1 */
} usbh_hc_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

int usbh_hal_host_init(usbh_config_t *cfg, u8 ch_max);
int usbh_hal_init_clock(void);
int usbh_hal_set_frame_interval(u8 speed);
int usbh_hal_reset_port(void);
int usbh_hal_drive_vbus(u8 state);
u8 usbh_hal_get_host_speed(void);
u32 usbh_hal_get_current_frame(void);
int usbh_hal_hc_init(u8 ch_num, u8 ep_num, u8 dev_addr, u8 speed, u8 ep_type, u16 mps, u8 dma);
int usbh_hal_hc_start_transfer(usbh_hc_t *hc, u8 dma);
u32 usbh_hal_hc_read_interrupt(void);
int usbh_hal_hc_halt(u8 hc_num);
int usbh_hal_do_ping(u8 ch_num);
int usbh_hal_stop(void);

#endif /* USBD_HAL_H */

