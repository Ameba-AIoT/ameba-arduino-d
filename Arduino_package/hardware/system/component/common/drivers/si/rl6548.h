/**
  ******************************************************************************
  * @file    rl6548.h
  * @author
  * @version V1.0.0
  * @date    2018-12-12
  * @brief   This file contains all the functions prototypes for the audio codec firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
  
#ifndef  __RL6548_H__
#define __RL6548_H__

/** @addtogroup AmebaD_Periph_Driver
  * @{
  */

/** @defgroup audio
  * @brief audio driver modules
  * @{
  */

/** @addtogroup audio
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * codec:
  * 	- Support anti-pop function to reduce audible pop
  * 	- Support AMIC-in, DMIC-in, Line-in and Line-out
  * 	- Sample rate: 8/16/32/44.1/48/96 KHz
  * 	- Sample bit: 16 bit, 24 bit, 8bit
  * 	- Channel number: mono or stereo
  * 	- Data format: I2S, Left justified, PCM mode A, PCM mode B, PCM mode A-N, PCM mode B-N
  * 	- Gain Control in ADC and DAC Path
  *
  * 	audio SI:
  * 	- Base Address: AUDIO_SI_DEV
  * 	- can use to configure codec register
  *
  *****************************************************************************************
  * How to use audio SI & codec API
  *****************************************************************************************
  *      To use audio codec si, the following steps are mandatory:
  *
  *      1. Open audio codec clock and function using
  *			PLLx_Set(0, ENABLE); (x is 0 or 1)
  *			RCC_PeriphClockCmd(APBPeriph_AUDIOC, APBPeriph_AUDIOC_CLOCK, ENABLE);
  *			RCC_PeriphClockCmd(APBPeriph_SPORT, APBPeriph_SPORT_CLOCK, ENABLE);
  *
  *      2. AUDIO SI enable:
  *			AUDIO_SI_Cmd(ENABLE).
  *
  *      3. Write AUDIO Codec registers using:
  *			AUDIO_SI_WriteReg(address, data);
  *
  *      4. Read AUDIO Codec registers using:
  *			AUDIO_SI_ReadReg(address);
  *
  *      @note Turn on AUDIO Codec register bank clock using:
  *			AUDIO_SI_ClkCmd(ENABLE);
  *
  *      @note Turn off AUDIO Codec register bank clock using:
  *			AUDIO_SI_ClkCmd(DISABLE);
  *
  *      To use audio codec si, the following steps are mandatory:
  *
  *      1. Codec initialize using
  *			CODEC_Init(sample_rate, word_len, mono_stereo, application);
  *
  *      2. Codec set volume using
  *			 CODEC_SetVolume(vol_lch, vol_rch);
  *
  *      3. Codec get volume using
  *			CODEC_GetVolume(*vol);
  *
  *      4. Set codec ADC and DAC sample rate using
  *			CODEC_SetSr(sample_rate);
  *
  *      5. Set codec ADC gain using
  *			CODEC_SetAdcGain(ad_gain_left, ad_gain_right);
  *
  *		 6. Set codec MIC_BIAS output voltage
  *			CODEC_SetMicBias(mic_bias);
  *
  *		 7. Codec de-initialize using
  *			CODEC_DeInit(application);
  *
  *      @note    All other functions can be used separately to modify, if needed,
  *			a specific feature of the AUDIO
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup AUDIO_Exported_Constants AUDIO Exported Constants
  * @{
  */
  
/** @defgroup CODEC_sample_rate AUDIO Codec Sample Rate
  * @{
  */
#define SR_48K			((u32)0x00000000)
#define SR_96K			((u32)0x00000001)
#define SR_32K			((u32)0x00000003)
#define SR_16K			((u32)0x00000005)
#define SR_8K			((u32)0x00000007)
#define SR_44P1K		((u32)0x00000008)
#define SR_88P2K		((u32)0x00000009)

/**
  * @}
  */

/** @defgroup CODEC_word_len AUDIO Codec Word Len
  * @{
  */
#define WL_16			((u32)0x00000000)
#define WL_24			((u32)0x00000002)	
#define WL_8				((u32)0x00000003)

/**
  * @}
  */

/** @defgroup CODEC_channel_mode AUDIO Codec Channel Mode
  * @{
  */
#define CH_STEREO		((u32)0x00000000)
#define CH_MONO			((u32)0x00000001)

/**
  * @}
  */

/** @defgroup CODEC_application_mode AUDIO CODEC Application Mode
  * @{
  */
#define APP_AMIC_IN		((u32)0x00000001)
#define APP_DMIC_IN		((u32)0x00000002)
#define APP_LINE_IN		((u32)0x00000004)
#define APP_LINE_OUT	((u32)0x00000008)
#define APP_DAAD_LPBK	((u32)0x00000010)

/**
  * @}
  */

/** @defgroup CODEC_mute_action AUDIO CODEC mute action per channel
  * @{
  */
#define MUTE_DISABLE	((u32)0x00000000)
#define MUTE_ENABLE		((u32)0x00000001)
#define MUTE_NO_ACT		((u32)0x00000002)

/**
  * @}
  */


/** @defgroup Codec_rx_channel_selection AUDIO CODEC Rx Channel Selection
  * @{
  */
#define RX_CH_LR						((u32)0x00000000)
#define RX_CH_RL						((u32)0x00000001)
#define RX_CH_LL						((u32)0x00000002)
#define RX_CH_RR						((u32)0x00000003)

/**
  * @}
  */
  
/**
  * @}
  */

/** @defgroup AUDIO_Exported_Functions AUDIO Exported Functions
  * @{
  */
/** @defgroup AUDIO_SI_functions 
  * @{
  */
void AUDIO_SI_Cmd(u8  new_state);
void AUDIO_SI_WriteReg(u32 address, u32 data);
u16 AUDIO_SI_ReadReg(u32 address);
void AUDIO_SI_ClkCmd(u8  new_state);
/**
  * @}
  */

/** @defgroup AUDIO_codec_functions
  * @{
  */
void CODEC_Init(u32 sample_rate, u32 word_len, u32 mono_stereo, u32 application);
void CODEC_SetVolume(u8 vol_lch, u8 vol_rch);
void CODEC_GetVolume(u16 *vol);
void CODEC_SetSr(u32 sample_rate);
void CODEC_SetAdcGain(u32 ad_gain_left, u32 ad_gain_right);
void CODEC_SetAmicBst(u32 amic_bst_left, u32 amic_bst_right);
void CODEC_SetDmicBst(u32 dmic_bst_left, u32 dmic_bst_right);
void CODEC_SetMicBias(u8 mic_bias);
void CODEC_MuteRecord(u32 mute_lch, u32 mute_rch);
void CODEC_MutePlay(u32 mute_lch, u32 mute_rch);
void CODEC_DeInit(u32 application);
/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */

/**
  * @}
  */
#endif

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor *****END OF FILE****/
