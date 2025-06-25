/**
  ******************************************************************************
  * The header file for usbd_composite.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_COMPOSITE_H
#define USBD_COMPOSITE_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"
#include "usbd_composite_config.h"
#include "usbd_composite_cdc_acm.h"
#include "usbd_composite_hid.h"

/* Exported defines ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_composite_init(u16 cdc_bulk_out_xfer_size, u16 cdc_bulk_in_xfer_size, usbd_composite_cdc_acm_usr_cb_t *cdc_cb,
					   u16 hid_intr_in_xfer_size, usbd_composite_hid_usr_cb_t *hid_cb, usbd_composite_cb_t *cb);

void usbd_composite_deinit(void);

#endif // USBD_COMPOSITE_H

