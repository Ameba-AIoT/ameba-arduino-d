/**
  ******************************************************************************
  * @file    rtl8721d_usb.h
  * @brief   This file contains the USB HAL definitions and function prototypes
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _RTL8721D_USB_H
#define _RTL8721D_USB_H

/* Includes ------------------------------------------------------------------*/

#include "basic_types.h"
#include "hal_platform.h"
#include "section_config.h"
#include "rtl8721d_vector.h"

/* Exported defines ----------------------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/* Enable USB OTG for internal test */
#define CONFIG_USB_OTG								0U

/* Enable USB phy */
#define CONFIG_USB_PHY								1U

/* USB ID */
#define USB_VID										0x0BDA
#define USB_PID										0x8721

/* USB base register */
#define USB_REG_BASE								USB_OTG_REG_BASE

/* USB IRQ */
#define USB_IRQ										USB_OTG_IRQ
#define USB_IRQ_PRI									5U

/* USB code sections */
#if defined(CONFIG_FLOADER_USBD_EN)
#define USB_DATA_SECTION							BOOT_RAM_DATA_SECTION
#define USB_BSS_SECTION								BOOT_RAM_BSS_SECTION
#define USB_TEXT_SECTION							BOOT_RAM_TEXT_SECTION
#else
#define USB_DATA_SECTION
#define USB_BSS_SECTION
#define USB_TEXT_SECTION
#endif

/* USB Core HW Configurations */
#define USB_MAX_ENDPOINTS							6U
#define USB_MAX_PIPES								8U
#define USB_IN_TOKEN_QUEUE_DEPTH					8U

/* USB OTG addon vendor status out register */
#define USB_ADDON_REG_VND_STS_OUT					(SYSTEM_CTRL_BASE + 0x3001CUL)

#define USB_ADDON_REG_VND_STS_OUT_DATA_POS			0U
#define USB_ADDON_REG_VND_STS_OUT_DATA_MASK			(0xFFU << USB_ADDON_REG_VND_STS_OUT_DATA_POS)
#define USB_ADDON_REG_VND_STS_OUT_DATA				(USB_ADDON_REG_VND_STS_OUT_DATA_MASK)

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8 page;
	u8 addr;
	u8 val;
} usb_cal_data_t;

typedef IRQ_FUN usb_irq_fun_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usb_chip_init(void);
u8 usb_chip_deinit(void);
usb_cal_data_t *usb_chip_get_cal_data(u8 mode);

#endif
