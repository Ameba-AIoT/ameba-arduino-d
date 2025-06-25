/**
  ******************************************************************************
  * The header file for usbd_composite_hid.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_COMPOSITE_HID_H
#define USBD_COMPOSITE_HID_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"
#include "usbd_composite_config.h"

/* Exported defines ----------------------------------------------------------*/

/* HID requests */
#define COMP_HID_GET_REPORT							0x01
#define COMP_HID_GET_IDLE							0x02
#define COMP_HID_GET_PROTOCOL						0x03
#define COMP_HID_SET_REPORT							0x09
#define COMP_HID_SET_IDLE							0x0A
#define COMP_HID_SET_PROTOCOL						0x0B

#define COMP_HID_INTR_IN_PACKET_SIZE				64

/* wValue of HID report request */
#define COMP_HID_DESC								0x21
#define COMP_HID_REPORT_DESC						0x22

#define COMP_HID_REPORT_DESC_SIZE					52

/* Mouse buttons */
#define COMP_HID_MOUSE_BUTTON_LEFT					0x01	// left button. 0: release, 1: press
#define COMP_HID_MOUSE_BUTTON_RIGHT					0x02	// right button. 0: release, 1: press
#define COMP_HID_MOUSE_BUTTON_MIDDLE				0x04	// wheel button. 0: release, 1: press
#define COMP_HID_MOUSE_BUTTON_RESERVED				0xF8	// reserved. all bit should set to 1.

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8(* init)(void);
	void(* deinit)(void);
	u8(* setup)(usb_setup_req_t *req, u8 *buf);
	void(* transmitted)(u8 status);
} usbd_composite_hid_usr_cb_t;

typedef struct {
	u16 intr_in_buf_size;
	u8 *intr_in_buf;

	__IO u8 intr_in_state;
	__IO u8 is_intr_in_busy;
	__IO u8 is_ready;

	usbd_composite_hid_usr_cb_t *cb;
	usbd_composite_dev_t *cdev;
} usbd_composite_hid_device_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

extern usbd_class_driver_t usbd_composite_hid_driver;

/* Exported functions --------------------------------------------------------*/

u8 usbd_composite_hid_init(usbd_composite_dev_t *cdev, u16 tx_buf_len, usbd_composite_hid_usr_cb_t *cb);
u8 usbd_composite_hid_deinit(void);
u8 usbd_composite_hid_send_data(u8 *data, u16 len);

#endif // USBD_COMPOSITE_HID_H