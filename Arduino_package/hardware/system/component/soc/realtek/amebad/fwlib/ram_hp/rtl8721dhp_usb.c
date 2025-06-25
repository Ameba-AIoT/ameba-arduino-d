/**
  ******************************************************************************
  * @file    rtl8721dhp_usb.c
  * @date    2021-06-08
  * @brief   This file provides the USB HAL functionalities
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "ameba_soc.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

#define USB_PIN_DM									_PA_25
#define USB_PIN_DP									_PA_26
#define USB_PIN_RREF								_PA_28

/* USB OTG addon control register */
#define USB_ADDON_REG_CTRL							(SYSTEM_CTRL_BASE + 0x30004UL)

#define USB_ADDON_REG_CTRL_BIT_UPLL_CKRDY			BIT(5)  /* 1: USB PHY clock ready */
#define USB_ADDON_REG_CTRL_BIT_USBOTG_EN			BIT(8)  /* 1: Enable USB OTG */
#define USB_ADDON_REG_CTRL_BIT_USBPHY_EN			BIT(9)  /* 1: Enable USB APHY & DPHY */

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static u32 usb_dm_pinmux_cfg;
static u32 usb_dp_pinmux_cfg;
static u32 usb_rref_pinmux_cfg;
static u32 usb_pinmux_initialized = 0;

/*
 * Calibration data
 * RL6886:  IC version >= IC_VERSION_4
 * RL6548C: IC version < IC_VERSION_4
 */

/* Calibration data for RL6886 host */
static const usb_cal_data_t usb_host_rl6886_cal_data[] = {
	{0x00, 0xE0, 0x33},
	{0x00, 0xE1, 0x2E},
	{0x00, 0xE2, 0x9D},
	{0x00, 0xE3, 0x81},
	{0x00, 0xE5, 0x92},
	{0x00, 0xE6, 0x96},
	{0x00, 0xF7, 0x12},

	{0xFF, 0x00, 0x00}
};

/* Calibration data for RL6548C host */
static const usb_cal_data_t usb_host_rl6548c_cal_data[] = {
	{0x00, 0xE0, 0x33},
	{0x00, 0xE1, 0x2E},
	{0x00, 0xE2, 0x9A},
	{0x00, 0xE5, 0x92},
	{0x00, 0xE6, 0x9A},
	{0x00, 0xF7, 0x12},

	{0xFF, 0x00, 0x00}
};

/* Calibration data for RL6886 device */
static const usb_cal_data_t usb_device_rl6886_cal_data[] = {
	{0x00, 0xE0, 0x33},
	{0x00, 0xE1, 0xAE},
	{0x00, 0xE2, 0x7D},
	{0x00, 0xE3, 0x81},
	{0x00, 0xE5, 0x96},
	{0x00, 0xE6, 0x96},
	{0x00, 0xE1, 0x2E},
	{0x00, 0xE1, 0xAE},

	{0xFF, 0x00, 0x00}
};

/* Calibration data for RL6548C device */
static const usb_cal_data_t usb_device_rl6548c_cal_data[] = {
	{0x00, 0xE0, 0x33},
	{0x00, 0xE1, 0xAE},
	{0x00, 0xE2, 0x7A},
	{0x00, 0xE3, 0x81},
	{0x00, 0xE5, 0x96},
	{0x00, 0xE6, 0x9A},
	{0x00, 0xE1, 0x2E},
	{0x00, 0xE1, 0xAE},

	{0xFF, 0x00, 0x00}
};

/* Private functions ---------------------------------------------------------*/

static u32 usb_get_pinmux_config(u8 pin)
{
	return PINMUX->PADCTR[pin];
}

static void usb_set_pinmux_config(u8 pin, u32 config)
{
	PINMUX->PADCTR[pin] = config;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Get USB chip specific calibration data
  * @param  mode: 0 - device; 1 - host
  * @retval Pointer to calibration data buffer
  */
usb_cal_data_t *usb_chip_get_cal_data(u8 mode)
{
	u32 ic_version;
	usb_cal_data_t *data = NULL;

	ic_version = SYSCFG_ICVersion();

	if (mode == 0U) {
		if (ic_version >= IC_VERSION_4) {
			data = (usb_cal_data_t *)usb_device_rl6886_cal_data;
		} else {
			data = (usb_cal_data_t *)usb_device_rl6548c_cal_data;
		}
	} else {
		if (ic_version >= IC_VERSION_4) {
			data = (usb_cal_data_t *)usb_host_rl6886_cal_data;
		} else {
			data = (usb_cal_data_t *)usb_host_rl6548c_cal_data;
		}
	}

	return data;
}

/**
  * @brief  USB HAL initialization
  * @param  void
  * @retval Status
  */
u8 usb_chip_init(void)
{
	u32 reg = 0;
	u32 count = 0;

	usb_dm_pinmux_cfg = usb_get_pinmux_config(USB_PIN_DM);
	usb_dp_pinmux_cfg = usb_get_pinmux_config(USB_PIN_DP);
	usb_rref_pinmux_cfg = usb_get_pinmux_config(USB_PIN_RREF);

	PAD_PullCtrl(USB_PIN_DM, GPIO_PuPd_NOPULL);
	PAD_PullCtrl(USB_PIN_DP, GPIO_PuPd_NOPULL);
	PAD_PullCtrl(USB_PIN_RREF, GPIO_PuPd_NOPULL);

	usb_pinmux_initialized = 1;

	RCC_PeriphClockCmd(APBPeriph_OTG, APBPeriph_OTG_CLOCK, ENABLE);

	/* USB Power Sequence */
	/* USB digital pad en,dp/dm sharing GPIO PAD */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_USB2_DIGPADEN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB Xtal to Uphy clock source Gated */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_OTG_CLK_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB phy digital en */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_USBD_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB PWC_UAHY EN */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_USB_PDN33);
	reg |= (BIT_HS_UAHV_EN | BIT_HS_UABG_EN | BIT_USB_IBX2USB_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB power ldo en*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_USBA_LDO_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB ISO_USBA_EN = 0 => disable isolation output signal from PD_USBA*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_ISO_USBA_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB ISO_USBD_EN = 0 => disable isolation output signal from PD_USBD*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_ISO_USBD_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB PWC_UALV_EN */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_UALV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB otg mode */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_USB_OTGMODE;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USBPHY_EN = 1 */
	reg = HAL_READ32(USB_ADDON_REG_CTRL, 0U);
	reg |= USB_ADDON_REG_CTRL_BIT_USBPHY_EN;
	HAL_WRITE32(USB_ADDON_REG_CTRL, 0U, reg);

	/* Wait UPLL_CKRDY */
	do {
		/* 1ms timeout expected, 10ms for safe */
		DelayUs(10);
		if (++count > 1000) {
			DiagPrintf("ERROR: USB init timeout\n");
			return HAL_TIMEOUT;
		}
	} while (!(HAL_READ32(USB_ADDON_REG_CTRL, 0U) & USB_ADDON_REG_CTRL_BIT_UPLL_CKRDY));

	/* USBOTG_EN = 1 => enable USBOTG */
	reg = HAL_READ32(USB_ADDON_REG_CTRL, 0U);
	reg |= USB_ADDON_REG_CTRL_BIT_USBOTG_EN;
	HAL_WRITE32(USB_ADDON_REG_CTRL, 0U, reg);

	return HAL_OK;
}

/**
  * @brief  USB chip specific deinitialization
  * @param  void
  * @retval Status
  */
u8 usb_chip_deinit(void)
{
	u32 reg = 0;

	/* USBOTG_EN = 0 => disable USBOTG */
	reg = HAL_READ32(USB_ADDON_REG_CTRL, 0U);
	reg &= (~USB_ADDON_REG_CTRL_BIT_USBOTG_EN);
	HAL_WRITE32(USB_ADDON_REG_CTRL, 0U, reg);

	/* USBPHY_EN = 0 */
	reg = HAL_READ32(USB_ADDON_REG_CTRL, 0U);
	reg &= (~USB_ADDON_REG_CTRL_BIT_USBPHY_EN);
	HAL_WRITE32(USB_ADDON_REG_CTRL, 0U, reg);

	/* disable USB otg mode */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_USB_OTGMODE);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB ISO_USBD_EN = 1 => enable isolation output signal from PD_USBD*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_ISO_USBD_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB ISO_USBA_EN = 1 => enable isolation output signal from PD_USBA*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_ISO_USBA_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* PWC_UPHV_EN  = 0 => disable USBPHY analog 3.3V power */
	/* PWC_UPLV_EN = 0 => disable USBPHY analog 1.2V power */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_UALV_EN);
	reg &= ~(BIT_HS_UAHV_EN | BIT_HS_UABG_EN | BIT_USB_IBX2USB_EN);
	reg |= BIT_USB_PDN33;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB power ldo disable*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_USBA_LDO_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* PWC_USBD_EN = 0 => disable USB digital power */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_USBD_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* XTAL_GATED_DIGI = 0 => disable XTAL to UPHY clock source gating */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg &= (~BIT_HS_OTG_CLK_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);

	/* USB digital pad disable*/
	reg = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL);
	reg |= BIT_HS_USB2_DIGPADEN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HS_OTG_CTRL, reg);
	RCC_PeriphClockCmd(APBPeriph_OTG, APBPeriph_OTG_CLOCK, DISABLE);

	if (usb_pinmux_initialized) {
		usb_pinmux_initialized = 0;
		usb_set_pinmux_config(USB_PIN_DM, usb_dm_pinmux_cfg);
		usb_set_pinmux_config(USB_PIN_DP, usb_dp_pinmux_cfg);
		usb_set_pinmux_config(USB_PIN_RREF, usb_rref_pinmux_cfg);
	}

	return HAL_OK;
}

