/**
  ******************************************************************************
  * The header file for usbd_composite_cdc_acm.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_COMPOSITE_CDC_ACM_H
#define USBD_COMPOSITE_CDC_ACM_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"
#include "usbd_composite_config.h"

/* Exported defines ----------------------------------------------------------*/

#define CONFIG_COMP_CDC_ACM_NOTIFY             0

/*  CDC definitions */
#define COMP_CDC_SEND_ENCAPSULATED_COMMAND               0x00U
#define COMP_CDC_GET_ENCAPSULATED_RESPONSE               0x01U
#define COMP_CDC_SET_COMM_FEATURE                        0x02U
#define COMP_CDC_GET_COMM_FEATURE                        0x03U
#define COMP_CDC_CLEAR_COMM_FEATURE                      0x04U
#define COMP_CDC_SET_LINE_CODING                         0x20U
#define COMP_CDC_GET_LINE_CODING                         0x21U
#define COMP_CDC_SET_CONTROL_LINE_STATE                  0x22U
#define COMP_CDC_SEND_BREAK                              0x23U

/* CDC ACM class-specific notification type */
#define COMP_CDC_NOTIFY_SERIAL_STATE                     0x20

#define COMP_CDC_ACM_CTRL_OVERRUN                        (1 << 6)
#define COMP_CDC_ACM_CTRL_PARITY                         (1 << 5)
#define COMP_CDC_ACM_CTRL_FRAMING                        (1 << 4)
#define COMP_CDC_ACM_CTRL_RI                             (1 << 3)
#define COMP_CDC_ACM_CTRL_BRK                            (1 << 2)
#define COMP_CDC_ACM_CTRL_DSR                            (1 << 1)
#define COMP_CDC_ACM_CTRL_DCD                            (1 << 0)

#define COMP_CDC_ACM_HS_BULK_MAX_PACKET_SIZE             512U   /* High speed BULK IN & OUT packet size */
#define COMP_CDC_ACM_FS_BULK_MAX_PACKET_SIZE             64U    /* Full speed BULK IN & OUT packet size */

#define COMP_CDC_ACM_HS_BULK_IN_PACKET_SIZE              COMP_CDC_ACM_HS_BULK_MAX_PACKET_SIZE
#define COMP_CDC_ACM_HS_BULK_OUT_PACKET_SIZE             COMP_CDC_ACM_HS_BULK_MAX_PACKET_SIZE

#define COMP_CDC_ACM_FS_BULK_IN_PACKET_SIZE              COMP_CDC_ACM_FS_BULK_MAX_PACKET_SIZE
#define COMP_CDC_ACM_FS_BULK_OUT_PACKET_SIZE             COMP_CDC_ACM_FS_BULK_MAX_PACKET_SIZE

#define COMP_CDC_ACM_INTR_IN_PACKET_SIZE                 10U    /* INTR IN packet size */
#define COMP_CDC_ACM_INTR_IN_REQUEST_SIZE                8U     /* INTR IN request size */
#define COMP_CDC_ACM_INTR_IN_DATA_SIZE                   2U     /* INTR IN data size */
#define COMP_CDC_ACM_HS_INTR_IN_INTERVAL                 8U     /* High speed INTR IN interval */
#define COMP_CDC_ACM_FS_INTR_IN_INTERVAL                 8U     /* Full speed INTR IN interval */

#define COMP_CDC_ACM_CTRL_BUF_SIZE                       512U   /* Control buffer size */

#define COMP_CDC_ACM_LINE_CODING_SIZE                    7U

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u32 bitrate;
	u8  format;
	u8  parity_type;
	u8  data_type;
} usbd_composite_cdc_acm_line_coding_t;

#if CONFIG_COMP_CDC_ACM_NOTIFY
typedef struct {
	u8 bmRequestType;
	u8 bNotificationType;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
	u8 buf[COMP_CDC_ACM_INTR_IN_DATA_SIZE];
} __attribute__((packed)) usbd_composite_cdc_acm_ntf_t;
#endif

typedef struct {
	u8(* init)(void);
	u8(* deinit)(void);
	u8(* setup)(usb_setup_req_t *req, u8 *buf);
	u8(* received)(u8 *buf, u32 len);
} usbd_composite_cdc_acm_usr_cb_t;

typedef struct {
	u8 *bulk_out_buf;
	u16 bulk_out_buf_size;
	u8 bulk_out_zlp;

	u8 *bulk_in_buf;
	u16 bulk_in_buf_size;
	__IO u8 bulk_in_state;
	__IO u8 is_bulk_in_busy;

#if CONFIG_COMP_CDC_ACM_NOTIFY
	usbd_composite_cdc_acm_ntf_t *intr_in_buf;
	__IO u8 intr_in_state;
	__IO u8 is_intr_in_busy;
#endif

	__IO u8 is_ready;

	usbd_composite_cdc_acm_usr_cb_t *cb;
	usbd_composite_dev_t *cdev;
} usbd_composite_cdc_acm_dev_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

extern usbd_class_driver_t usbd_composite_cdc_acm_driver;

/* Exported functions --------------------------------------------------------*/

u8 usbd_composite_cdc_acm_init(usbd_composite_dev_t *cdev, u16 bulk_out_xfer_size, u16 bulk_in_xfer_size, usbd_composite_cdc_acm_usr_cb_t *cb);
u8 usbd_composite_cdc_acm_deinit(void);
u8 usbd_composite_cdc_acm_transmit(u8 *buf, u16 len);
#if CONFIG_COMP_CDC_ACM_NOTIFY
u8 usbd_composite_cdc_acm_notify_serial_state(u16 serial_state);
#endif
#endif  /* USBD_COMPOSITE_CDC_ACM_H */

