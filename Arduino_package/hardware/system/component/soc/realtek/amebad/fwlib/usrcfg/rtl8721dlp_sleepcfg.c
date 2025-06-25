/**
  ******************************************************************************
  * @file    rtl8721dlp_sleepcfg.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of pin control:
  *           - sleep wakeup event
  *           - sleep power management
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#include "ameba_soc.h"

#ifdef CONFIG_SOC_PS_MODULE

/* if X can wakeup dsleep, it can wakeup dstandby & sleep */
/* if X can wakeup dstandby, it can wakeup sleep */
PWRCFG_TypeDef sleep_wevent_config[]=
{
//  	Module								Status
	{BIT_LP_WEVT_HS_MSK,				OFF},
	{BIT_LP_WEVT_AON_MSK,				ON},
	{BIT_LP_WEVT_SGPIO_MSK,			OFF},
	{BIT_LP_WEVT_COMP_MSK,				OFF},
	{BIT_LP_WEVT_ADC_MSK,				OFF},
	{BIT_LP_WEVT_I2C_MSK,				OFF},
	{BIT_LP_WEVT_UART_MSK,				ON},
	{BIT_LP_WEVT_BOD_MSK,				OFF},
	{BIT_LP_WEVT_WLAN_MSK,				ON},
	{BIT_LP_WEVT_GPIO_MSK,				OFF},
	{BIT_LP_WEVT_SWRD_OCP_MSK,		OFF},
	{BIT_LP_WEVT_TIMER_MSK,				OFF},

	{0xFFFFFFFF,							OFF},	/* Table end */
};

PWRCFG_TypeDef sleep_aon_wevent_config[]=
{
//  	Module									Status
	{BIT_CHIP_PDB_MSK,			ON},	/* [7]	R/W	0		1: Indicate chip power-down */
	{BIT_CAPTOUCH_WAKE_MSK,		ON},	/* [6]	R/W	0		1: Indicate captouch wake event */
	{BIT_KEYSCAN_WAKE_MSK,		ON},	/* [4]	R/W	0		1: Indicate keyscan wake */
	{BIT_DLPS_TSF_WAKE_MSK,		OFF},	/* [3]	R/W	0		1: Indicate tsf wake under deep-lps mode */
	{BIT_RTC_WAKE_MSK,			OFF},	/* [2]	R/W	0		1: Indicate RTC wake */
	{BIT_AON_WAKE_TIM0_MSK,	ON},	/* [1]	R/W	0		1: Indicate AON timer wake */
	{BIT_GPIO_WAKE_MSK,			ON},	/* [0]	R/W	0		1: Indicate GPIO wake, see aon_wakepin & dsleep_wakepin_config */
	
	{0xFFFFFFFF,								OFF},	/* Table end */
};

HSWAKEEVENT_TypeDef hs_wakeevent_config[]=
{
//  	Module								Event					Status
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_HS_STS,			ON},		/* [30] 1: Indicate HS Wakeup event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_AON_STS,		OFF},	/* [29] 1: Indicate AON Wakeup event (0x128) */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_SGPIO_STS,		OFF},	/* [28] 1: Indicate SGPIO Wakeup event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_COMP_STS,		OFF},	/* [27] 1: Indicate Comparetor Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_ADC_STS,		OFF},	/* [26] 1: Indicate  ADC Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_I2C_STS,			OFF},	/* [24] 1: Indicate I2C  Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_UART_STS,		OFF},	/* [20] 1: Indicate UART  Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_BOD_STS,		OFF},	/* [6]   1: Indicate BOD Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_WLAN_STS,		OFF},	/* [5]   1: Indicate WLAN Wakeup  event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_GPIO_STS,		OFF},	/* [4]   1: Indicate GPIO Wakeup event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_SWRD_OCP_STS,	OFF},	/* [2]   1: Indicate DCORE SWR OCP event */
	{REG_LP_SLP_WAKE_EVENT_STATUS0,	BIT_LP_WEVT_TIMER_STS,		OFF},	/* [1]   1: Indicate GTimer Wakeup system event; */

	{REG_AON_WAKE_OPT_STS,				BIT_CHIP_PDB_STS,			OFF},	/* [7] 1: Indicate chip power-down */
	{REG_AON_WAKE_OPT_STS,				BIT_CAPTOUCH_WAKE_STS,		ON},		/* [6] 1: Indicate captouch wake event */
	{REG_AON_WAKE_OPT_STS,				BIT_KEYSCAN_WAKE_STS,		ON},		/* [4] 1: Indicate keyscan wake */
	{REG_AON_WAKE_OPT_STS,				BIT_DLPS_TSF_WAKE_STS,		OFF},	/* [3] 1: Indicate tsf wake under deep-lps mode */
	{REG_AON_WAKE_OPT_STS,				BIT_RTC_WAKE_STS,			ON},		/* [2] 1: Indicate RTC wake */
	{REG_AON_WAKE_OPT_STS,				BIT_AON_WAKE_TIM0_STS,		OFF},	/* [1] 1: Indicate AON timer wake */
	{REG_AON_WAKE_OPT_STS,				BIT_GPIO_WAKE_STS,			ON},		/* [0] 1: Indicate GPIO wake, see aon_wakepin & dsleep_wakepin_config */
	
	{0xFFFFFFFF,		OFF,			0},	/* Table end */
};

/* if X can wakeup dsleep, it can wakeup dstandby & sleep */
/* if X can wakeup dstandby, it can wakeup sleep */
PWRCFG_TypeDef dsleep_aon_wevent_config[]=
{
//  	Module									Status
	{BIT_CAPTOUCH_WAKE_STS,	ON},	/* [6]	R/W	0		1: Indicate captouch wake event */
	{BIT_KEYSCAN_WAKE_STS,		ON},	/* [4]	R/W	0		1: Indicate keyscan wake */
	{BIT_DLPS_TSF_WAKE_STS,		ON},	/* [3]	R/W	0		1: Indicate tsf wake under deep-lps mode */
	{BIT_RTC_WAKE_STS,			OFF},	/* [2]	R/W	0		1: Indicate RTC wake */
	{BIT_AON_WAKE_TIM0_STS,	ON},	/* [1]	R/W	0		1: Indicate AON timer wake */
	{BIT_GPIO_WAKE_STS,			ON},	/* [0]	R/W	0		1: Indicate GPIO wake, see aon_wakepin & dsleep_wakepin_config */
	
	{0xFFFFFFFF,								OFF},	/* Table end */
};

u8 aon_wakepin[4][3] = {
	{	/* wakepin 0 */
		_PA_12, /* PINMUX_S0 */
		_PA_16, /* PINMUX_S1 */
		_PA_20, /* PINMUX_S2 */
	},
	{	/* wakepin 1 */
		_PA_13, /* PINMUX_S0 */
		_PA_17, /* PINMUX_S1 */
		_PA_21, /* PINMUX_S2 */
	},
	{	/* wakepin 2 */
		_PA_14, /* PINMUX_S0 */
		_PA_18, /* PINMUX_S1 */
		_PA_25, /* PINMUX_S2 */
	},
	{	/* wakepin 3 */
		_PA_15, /* PINMUX_S0 */
		_PA_19, /* PINMUX_S1 */
		_PA_26  /* PINMUX_S2 */
	},
};

/* you should set dsleep_wevent_config.BIT_GPIO_WAKE_STS ON if you use wakepin */
/* can be used by sleep mode & deep sleep mode */
WAKEPIN_TypeDef sleep_wakepin_config[]=
{
//  	Module			Status		Polarity
	{PINMUX_S0,		OFF,			1},	/* wakeup_0 config */
	{PINMUX_S0,		OFF,			1},	/* wakeup_1 config */
	{PINMUX_S0,		OFF,			1},	/* wakeup_2 config */
	{PINMUX_S0,		OFF,			1},	/* wakeup_3 config */
	
	{0xFFFFFFFF,		OFF,			0},	/* Table end */
};

PSCFG_TypeDef ps_config = {
	.km0_config_wifi_enable = TRUE,
	.km0_enable_key_touch = FALSE,//BIT_KEY_ENABLE | BIT_CAPTOUCH_ENABLE,
	.km0_tickles_debug = FALSE,/* if open WIFI FW, should close it, or beacon will lost in WOWLAN */
	.km0_osc2m_close = TRUE,
	.km0_pg_enable = FALSE,
	.km0_rtc_calibration = FALSE,
	.km0_audio_pad_enable = TRUE,
	.km0_disable_audio_ldo = TRUE,
};

#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
