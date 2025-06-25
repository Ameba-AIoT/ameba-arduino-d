/**
  ******************************************************************************
  * The header file for usbd_hid.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_HID_H
#define USBD_HID_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

// HID usage configuration
#define USBD_HID_MOUSE_DEVICE					1
#define USBD_HID_KEYBOARD_DEVICE				2

#define USBD_HID_DEVICE_TYPE					USBD_HID_MOUSE_DEVICE

// HID PID/VID
#define USBD_HID_VID							USB_VID
#define USBD_HID_PID							USB_PID

// HID strings
#define USBD_HID_LANGID_STRING					0x0409U
#define USBD_HID_MFG_STRING						"Realtek"
#define USBD_HID_PROD_HS_STRING					"Realtek HID (HS)"
#define USBD_HID_PROD_FS_STRING					"Realtek HID (FS)"
#define USBD_HID_SN_STRING						"1234567890"

// HID request
#define USBD_HID_GET_REPORT						0x01
#define USBD_HID_GET_IDLE						0x02
#define USBD_HID_GET_PROTOCOL					0x03
#define USBD_HID_SET_REPORT						0x09
#define USBD_HID_SET_IDLE						0x0a
#define USBD_HID_SET_PROTOCOL					0x0b

#define USBD_HID_CTRL_BUF_SIZE					512U
#define USBD_HID_INTR_IN_BUF_SIZE				512U
#define USBD_HID_INTR_OUT_BUF_SIZE				1024U   // High speed INTR OUT packet size

#define USBD_HID_SELF_POWERED					1U

// wValue of HID report request
#define USBD_HID_DESC							0x21
#define USBD_HID_REPORT_DESC					0x22

// String index
#define USBD_HID_STRING_MANUFACTURER			1
#define USBD_HID_STRING_PRODUCT					2
#define USBD_HID_STRING_SERIALNUMBER			3
#define USBD_HID_STRING_STRING_INTERFACE		4

// Endpoint address
#define USBD_HID_INTERRUPT_OUT_EP_ADDRESS		0x02
#define USBD_HID_INTERRUPT_IN_EP_ADDRESS		0x81
#define USBD_HID_DEFAULT_INT_IN_XFER_SIZE		512
#define USBD_HID_DEFAULT_INT_OUT_XFER_SIZE		1024

#define USBD_HID_DESC_SIZE						0x9

// Mouse button definition
#define USBD_HID_MOUSE_BUTTON_LEFT				0x01	// Left button. 0: release, 1: press
#define USBD_HID_MOUSE_BUTTON_RIGHT				0x02	// Right button. 0: release, 1: press
#define USBD_HID_MOUSE_BUTTON_MIDDLE			0x04	// Wheel button. 0: release, 1: press
#define USBD_HID_MOUSE_BUTTON_RESERVED			0xf8	// Reserved. all bit should set to 1.

/* Exported types ------------------------------------------------------------*/

typedef struct {
	void(* init)(void);
	void(* deinit)(void);
	void(* setup)(void);
	void(* transmitted)(u8 status);
#if USBD_HID_DEVICE_TYPE == USBD_HID_KEYBOARD_DEVICE
	void(* received)(u8 *buf, u32 len);
#endif
	void (*status_changed)(u8 status);
} usbd_hid_usr_cb_t;

typedef struct {
	u32 intr_in_state;
	u16 intr_in_buf_len;
	u8 *intr_in_buf;

#if USBD_HID_DEVICE_TYPE == USBD_HID_KEYBOARD_DEVICE
	u32 intr_out_buf_size;
	u8  *intr_out_buf;
#endif

	u8 protocol;
	u8 idle_rate;

	__IO u8 is_ready;
	__IO u8 is_tx_busy;

	u8 *ctrl_buf;
	usb_setup_req_t ctrl_req;

	usbd_hid_usr_cb_t *cb;
	usb_dev_t *dev;
} usbd_hid_t;

u8 usbd_hid_init(u16 tx_buf_len, usbd_hid_usr_cb_t *cb);
u8 usbd_hid_deinit(void);
u8 usbd_hid_send_data(u8 *data, u16 len);

/* Exported functions --------------------------------------------------------*/

#endif // USBD_HID_H
