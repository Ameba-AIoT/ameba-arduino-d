/**
  ******************************************************************************
  * The header file for USB composite device configurations
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_COMPOSITE_CONFIG_H
#define USBD_COMPOSITE_CONFIG_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/* CDC ACM Device parameters */
#define USBD_COMP_VID					USB_VID
#define USBD_COMP_PID					USB_PID
#define USBD_COMP_CDC_COM_ITF			0x00
#define USBD_COMP_CDC_DAT_ITF			0x01
#define USBD_COMP_HID_ITF				0x02
#define USBD_COMP_LANGID				0x0409U
#define USBD_COMP_SELF_POWERED			1U
#define USBD_COMP_REMOTE_WAKEUP_EN		1U

/* CDC ACM Endpoint parameters */
#define USBD_COMP_CDC_BULK_IN_EP		0x81U
#define USBD_COMP_CDC_BULK_OUT_EP		0x02U
#define USBD_COMP_CDC_INTR_IN_EP		0x83U
#define USBD_COMP_HID_INTR_OUT_EP		0x04U
#define USBD_COMP_HID_INTR_IN_EP		0x85U

#define USBD_COMP_CTRL_BUF_SIZE			512U

#define USBD_IDX_CDC_ITF_STR			(USBD_IDX_SERIAL_STR + 1)
#define USBD_IDX_HID_ITF_STR			(USBD_IDX_CDC_ITF_STR + 1)

#define USBD_COMP_MFG_STRING			"Realtek"
#define USBD_COMP_PROD_STRING			"Realtek Composite Device"
#define USBD_COMP_SN_STRING				"1234567890"
#define USBD_COMP_CDC_HS_ITF_STRING		"Realtek HS CDC ACM Device"
#define USBD_COMP_CDC_FS_ITF_STRING		"Realtek FS CDC ACM Device"
#define USBD_COMP_HID_HS_ITF_STRING		"Realtek HS HID Device"
#define USBD_COMP_HID_FS_ITF_STRING		"Realtek FS HID Device"

/* Exported types ------------------------------------------------------------*/

typedef struct {
	void (*status_changed)(u8 status);
} usbd_composite_cb_t;

typedef struct {
	u8 *ctrl_buf;
	usb_setup_req_t ctrl_req;
	usbd_class_driver_t *cdc;
	usbd_class_driver_t *hid;
	usbd_composite_cb_t *cb;
	usb_dev_t *dev;
} usbd_composite_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#endif // USBD_COMPOSITE_CONFIG_H

