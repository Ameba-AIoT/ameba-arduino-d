/**
  ******************************************************************************
  * @file    usbh_hcd.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbh_hcd.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBH_HCD_H
#define USBH_HCD_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_hal.h"
#include "usbh_core.h"
#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

typedef enum {
	USBH_HCD_STATE_RESET    = 0x00,
	USBH_HCD_STATE_READY    = 0x01,
	USBH_HCD_STATE_ERROR    = 0x02,
	USBH_HCD_STATE_BUSY     = 0x03,
	USBH_HCD_STATE_TIMEOUT  = 0x04
} usbh_hcd_state_t;

typedef struct {
	usbh_hc_t             hc[16];          /*!< Host channels parameters */
	usb_spinlock_t        *lock;           /*!< HCD peripheral status    */
	__IO usbh_hcd_state_t hcd_state;       /*!< HCD communication state  */
	__IO u32              error_code;      /*!< HCD error code           */
	usbh_core_t           *core;           /*!< Core handler    */
	u8                    isr_initialized; /*!< ISR initialized */
	_sema                 isr_sema;
	struct task_struct    isr_task;
} usbh_hcd_t;

/* Exported macros -----------------------------------------------------------*/

#define USBH_HCD_MASK_INTERRUPT(__INTERRUPT__)            (USB_GLOBAL->GINTMSK &= ~(__INTERRUPT__))
#define USBH_HCD_UNMASK_INTERRUPT(__INTERRUPT__)          (USB_GLOBAL->GINTMSK |= (__INTERRUPT__))

#define USBH_HCD_GET_FLAG(__INTERRUPT__)                  ((usb_hal_read_interrupts() & (__INTERRUPT__)) == (__INTERRUPT__))
#define USBH_HCD_CLEAR_FLAG(__INTERRUPT__)                ((USB_GLOBAL->GINTSTS) = (__INTERRUPT__))

#define USBH_HCD_CLEAR_HC_INT(ch_num, __INTERRUPT__)      (USB_HC(ch_num)->HCINT = (__INTERRUPT__))
#define USBH_HCD_MASK_HALT_HC_INT(ch_num)                 (USB_HC(ch_num)->HCINTMSK &= ~USB_OTG_HCINTMSK_CHHM)
#define USBH_HCD_UNMASK_HALT_HC_INT(ch_num)               (USB_HC(ch_num)->HCINTMSK |= USB_OTG_HCINTMSK_CHHM)
#define USBH_HCD_MASK_ACK_HC_INT(ch_num)                  (USB_HC(ch_num)->HCINTMSK &= ~USB_OTG_HCINTMSK_ACKM)
#define USBH_HCD_UNMASK_ACK_HC_INT(ch_num)                (USB_HC(ch_num)->HCINTMSK |= USB_OTG_HCINTMSK_ACKM)

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_hcd_init(usbh_core_t *core);
u8 usbh_hcd_deinit(usbh_core_t *core);
u8 usbh_hcd_hc_init(usbh_hcd_t *hcd, u8 ch_num, u8 ep_num, u8 dev_addr, u8 speed, u8 ep_type, u16 mps);
u8 usbh_hcd_hc_halt(usbh_hcd_t *hcd, u8 ch_num);
u8 usbh_hcd_reset_port(usbh_hcd_t *hcd);
u8 usbh_hcd_start(usbh_hcd_t *hcd);
u8 usbh_hcd_stop(usbh_hcd_t *hcd);
u8 usbh_hcd_hc_reactivate(usbh_hcd_t *hcd, u8 ch_num);
u8 usbh_hcd_hc_submit_request(usbh_hcd_t *hcd, u8 ch_num, u8 direction, u8 ep_type, u8 token, u8 *buf, u16 length, u8 do_ping);
usbh_hcd_state_t usbh_hcd_get_state(usbh_hcd_t *hcd);
usbh_urb_state_t usbh_hcd_hc_get_urb_state(usbh_hcd_t *hcd, u8 ch_num);
u32 usbh_hcd_hc_get_transfer_size(usbh_hcd_t *hcd, u8 ch_num);
usbh_hc_state_t usbh_hcd_hc_get_state(usbh_hcd_t *hcd, u8 ch_num);
u32 usbh_hcd_get_current_frame(usbh_hcd_t *hcd);
u32 usbh_hcd_get_current_speed(usbh_hcd_t *hcd);
u8 usbh_hcd_set_toggle(usbh_hcd_t *hcd, u8 ch_num, u8 toggle);
u8 usbh_hcd_get_toggle(usbh_hcd_t *hcd, u8 ch_num);

void usbh_hcd_handle_sof_interrupt(usbh_hcd_t *hcd);
void usbh_hcd_handle_connect_interrupt(usbh_hcd_t *hcd);
void usbh_hcd_handle_disconnect_interrupt(usbh_hcd_t *hcd);
void usbh_hcd_handle_port_enable_interrupt(usbh_hcd_t *hcd);
void usbh_hcd_handle_port_disable_interrupt(usbh_hcd_t *hcd);
void usbh_hcd_hc_notify_urb_state_change(usbh_hcd_t *hcd, u8 ch_num, usbh_urb_state_t urb_state);

u8 usbh_hcd_enter_suspend(u8 flag);
u8 usbh_hcd_port_test_ctrl(u8 type);
#endif /* USBH_HCD_H */

