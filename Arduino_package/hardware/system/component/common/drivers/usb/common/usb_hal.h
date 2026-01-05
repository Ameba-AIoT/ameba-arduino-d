/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef USB_HAL_H
#define USB_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "usb_os.h"
#include "usb_ch9.h"
#include "basic_types.h"
#include "ameba_soc.h"

/* Exported defines ----------------------------------------------------------*/

/* USB speed */
#define USB_SPEED_HIGH                                 0
#define USB_SPEED_HIGH_IN_FULL                         1
#define USB_SPEED_LOW                                  2
#define USB_SPEED_FULL                                 3

#define USB_IN_TOKEN_QUEUE_DEPTH                       8U

/* Exported macros -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* USB device bus state */
typedef enum {
	USB_DEV_BUS_STATUS_DN       = BIT0,  // D-
	USB_DEV_BUS_STATUS_DP       = BIT1,  // D+
	USB_DEV_BUS_STATUS_SUSPEND  = BIT2,  // suspend indication
} usb_dev_bus_state_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

int usb_hal_core_init(u8 dma_enable);
void usb_hal_enable_interrupt(void);
void usb_hal_disable_interrupt(void);
void usb_hal_register_irq_handler(usb_irq_fun_t handler);
void usb_hal_unregister_irq_handler(void);
void usb_hal_enable_global_interrupt(void);
void usb_hal_disable_global_interrupt(void);
u32 usb_hal_read_clear_interrupts(void);
u32 usb_hal_read_interrupts(void);
void usb_hal_clear_interrupts(u32 interrupt);
int usb_hal_set_otg_mode(usb_otg_mode_t mode);
usb_otg_mode_t usb_hal_get_otg_mode(void);
int usb_hal_flush_tx_fifo(u32 num);
int usb_hal_flush_rx_fifo(void);
int usb_hal_set_rx_fifo(u16 size);
int usb_hal_set_np_tx_fifo(u16 size);
int usb_hal_set_ptx_fifo(u8 fifo, u16 size);
int usb_hal_write_packet(u8 *src, u8 ep_ch_num, u16 len);
int usb_hal_read_packet(u8 *dest, u8 ep_ch_num, u16 len);
int usb_hal_get_device_bus_status(u32 *bus_status);
void usb_hal_device_disconnect(u8 en);
void usb_hal_dump_registers(void);

#if CONFIG_USB_PHY
int usb_hal_read_phy_register(u8 addr, u8 *value);
int usb_hal_write_phy_register(u8 addr, u8 value);
int usb_hal_calibrate(u8 mode);
int usb_hal_enable_otg(void);
#endif

#endif /* USB_HAL_H */
