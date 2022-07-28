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

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Load PHY vendor control registers
  * @param  addr: PHY register address
  * @retval HAL status
  */
static u8 USB_LoadPhyVendorControlRegister(u8 addr)
{
	u8 ret = HAL_OK;
	u32 count = 0U;
	u32 reg = 0x0A300000U;

	reg |= (((u32)(USB_OTG_PHY_LOW_ADDR(addr))) << USB_OTG_GPVNDCTL_VCTRL_Pos);
	USB_GLOBAL->GPVNDCTL = reg;

	do {
		/* 1us timeout expected, 1ms for safe */
		DelayUs(1);
		if (++count > 1000U) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "USB_LoadPhyVendorControlRegister timeout\n");
			ret = HAL_TIMEOUT;
			break;
		}
	} while ((USB_GLOBAL->GPVNDCTL & USB_OTG_GPVNDCTL_VSTSDONE) == 0U);

	return ret;
}

/**
  * @brief  Write USB PHY register
  * @param  addr: USB PHY register address
  * @param  data: USB PHY register value
  * @retval HAL status
  */
static u8 USB_WritePhyRegister(u8 addr, u8 value)
{
	u8 ret;
	u32 reg;

	reg = HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_VND_STS_OUT);
	reg &= (~USB_OTG_PHY_DATA_MASK);
	reg |= value;
	HAL_WRITE32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_VND_STS_OUT, reg);

	/* Load low addr */
	ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_LOW_ADDR(addr));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_HIGH_ADDR(addr));
	}

	return ret;
}

/**
  * @brief  Read USB PHY register
  * @param  addr: USB PHY register address
  * @retval HAL status
  */
static u8 USB_ReadPhyRegister(u8 addr, u8 *value)
{
	u8 ret;
	u32 reg;
	u8 addr_read;

	if (addr >= 0xE0) {
		addr_read = addr - 0x20;
	} else {
		addr_read = addr;
	}

	/* Load low addr */
	ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_LOW_ADDR(addr_read));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = USB_LoadPhyVendorControlRegister(USB_OTG_PHY_HIGH_ADDR(addr_read));
		if (ret == HAL_OK) {
			reg = USB_GLOBAL->GPVNDCTL;
			*value = ((reg & USB_OTG_GPVNDCTL_REGDATA_Msk) >> USB_OTG_GPVNDCTL_REGDATA_Pos) & 0xFF;
		}
	}

	return ret;
}

/**
  * @brief  Select USB PHY page
  * @param  None
  * @retval HAL status
  */
static u8 USB_PhySelectPage(u8 page)
{
	u8 ret;
	u8 reg;

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_F4, &reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_F4: %d\n", ret);
		return ret;
	}

	reg &= (~USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK);
	reg |= (page << USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_POS) & USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK;
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_F4, reg);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_F4: %d\n", ret);
	}

	return ret;
}


/**
  * @brief  USB Device PHY calibration
  * @param  None
  * @retval HAL status
  */
static u8 USB_DeviceCalibrate(void)
{
	u8 ret = HAL_OK;
	u8 tmp;

	/* Calibrate page 0 registers */
	ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
		return ret;
	}

	/* E0 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x33U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E0: %d\n", ret);
		return ret;
	}

	/* E1 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, 0xAEU);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	/* E2 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E2, 0x7dU);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E2: %d\n", ret);
		return ret;
	}

	/* E5 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E5, 0x96U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E5: %d\n", ret);
		return ret;
	}

	/* E6 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E6, 0x96U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E6: %d\n", ret);
		return ret;
	}

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_E1, &tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	tmp = tmp & (~BIT(7));
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_E1, &tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	tmp = tmp & BIT(7);
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	return HAL_OK;
}

/**
  * @brief  USB Host PHY calibration
  * @param  None
  * @retval HAL status
  */
static u8 USB_HostCalibrate(void)
{
	u8 ret = HAL_OK;
	u8 tmp;

	/* Calibrate page 0 registers */
	ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE0);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 0: %d\n", ret);
		return ret;
	}

	/* E0 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E0, 0x33U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E0: %d\n", ret);
		return ret;
	}

	/* E1 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, 0x2eU);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	/* E2 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E2, 0x9dU);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E2: %d\n", ret);
		return ret;
	}

	/* E5 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E5, 0x92U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E5: %d\n", ret);
		return ret;
	}

	/* E6 / Page 0 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E6, 0x96U);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E6: %d\n", ret);
		return ret;
	}

	ret = USB_ReadPhyRegister(USB_OTG_PHY_REG_E1, &tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to read USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	tmp = tmp & (~BIT(7));
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_E1, tmp);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_P0_E1: %d\n", ret);
		return ret;
	}

	/* Calibrate page 1 registers */
	ret = USB_PhySelectPage(USB_OTG_PHY_REG_F4_BIT_PAGE1);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to select page 1: %d\n", ret);
		return ret;
	}

	/* f7 / Page 1 */
	ret = USB_WritePhyRegister(USB_OTG_PHY_REG_F7, 0x9aU);
	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to write USB_OTG_PHY_REG_F7: %d\n", ret);
		return ret;
	}

	return HAL_OK;
}


/* Exported functions --------------------------------------------------------*/

/**
  * @brief  USB PHY calibration
  *			Shall be called after soft disconnect
  * @param  None
  * @retval HAL status
  */
u8 USB_Calibrate(void)
{
	u8 ret = HAL_OK;

	/* 3ms + 2.5us from DD, 3ms already delayed after soft disconnect */
	DelayUs(3);

	/*Device/Host mode calibration*/
	if ((USB_GLOBAL->GINTSTS) & 0x1U) {
		ret = USB_HostCalibrate();
	} else {
		ret = USB_DeviceCalibrate();
	}

	if (ret != HAL_OK) {
		DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "Fail to Calibration\n");
		return ret;
	}

	return HAL_OK;
}

/**
  * @brief  USB HAL initialization
  * @param  void
  * @retval HAL status
  */
u8 USB_Init(void)
{
	u32 reg = 0;
	u32 count = 0;

	PAD_PullCtrl(_PA_25, GPIO_PuPd_NOPULL);
	PAD_PullCtrl(_PA_26, GPIO_PuPd_NOPULL);
	PAD_PullCtrl(_PA_28, GPIO_PuPd_NOPULL);

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
	reg = HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL);
	reg |= USB_OTG_ADDON_REG_CTRL_BIT_USBPHY_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL, reg);

	/* Wait UPLL_CKRDY */
	do {
		/* 1ms timeout expected, 10ms for safe */
		DelayUs(10);
		if (++count > 1000) {
			DBG_PRINTF(MODULE_USB_OTG, LEVEL_ERROR, "USB_Init timeout\n");
			return HAL_TIMEOUT;
		}
	} while (!(HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL) & USB_OTG_ADDON_REG_CTRL_BIT_UPLL_CKRDY));

	/* USBOTG_EN = 1 => enable USBOTG */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL);
	reg |= USB_OTG_ADDON_REG_CTRL_BIT_USBOTG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL, reg);

	return HAL_OK;
}

/**
  * @brief  USB HAL deinitialization
  * @param  void
  * @retval HAL status
  */
u8 USB_DeInit(void)
{
	u32 reg = 0;

	/* USBOTG_EN = 0 => disable USBOTG */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL);
	reg &= (~USB_OTG_ADDON_REG_CTRL_BIT_USBOTG_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL, reg);

	/* USBPHY_EN = 0 */
	reg = HAL_READ32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL);
	reg &= (~USB_OTG_ADDON_REG_CTRL_BIT_USBPHY_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE, USB_OTG_ADDON_REG_CTRL, reg);

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

	return HAL_OK;
}

/**
  * @brief  USB HAL enable interrupt
  * @param  void
  * @retval void
  */
void USB_EnableInterrupt(void)
{
	InterruptEn(USB_OTG_IRQ, 10);
}

/**
  * @brief  USB HAL disable interrupt
  * @param  void
  * @retval void
  */
void USB_DisableInterrupt(void)
{
	InterruptDis(USB_OTG_IRQ);
}

/**
  * @brief  USB HAL register IRQ handler
  * @param  handler: IRQ handler
  * @retval void
  */
void USB_RegisterIrqHandler(IRQ_FUN handler)
{
	if (handler != NULL) {
		InterruptRegister(handler, USB_OTG_IRQ, NULL, 10);
	}
}

/**
  * @brief  USB HAL unregister IRQ handler
  * @param  void
  * @retval void
  */
void USB_UnregisterIrqHandler(void)
{
	InterruptUnRegister(USB_OTG_IRQ);
}

