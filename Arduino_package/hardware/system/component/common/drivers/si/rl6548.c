/**
  ******************************************************************************
  * @file    rl6548.c
  * @author
  * @version V1.0.0
  * @date    2018-12-12
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Audio codec peripheral:
  *           - Codec Initialization
  *           - Codec parameters configuration
  *           - SI read/write configuration
  *           - SI Clock configuration
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"
#include "rl6548.h"

/**
  * @brief  Enables or disables the specified AUDIO SI peripheral.
  * @param  new_state: new state of the SIx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SI_Cmd(u8  new_state)
{
	AUDIO_SI_TypeDef* SIx = AUDIO_SI_DEV;
	
	if (new_state == ENABLE) {
		SIx->SI_CTRLR &= ~ BIT_CTRLR_SI_DISABLE;
	} else {
		SIx->SI_CTRLR |= BIT_CTRLR_SI_DISABLE;
	}
}

/**
  * @brief	SI write codec register.
  * @param  address: codec register address.
  * @param  data: data which write to register.
  * @retval None
  */
void AUDIO_SI_WriteReg(u32 address, u32 data)
{
	u32 cnt = 0;
	AUDIO_SI_TypeDef* SIx = AUDIO_SI_DEV;
	
	SIx->SI_CTRLR = (address << 8) | (data << 16) | BIT_CTRLR_SI_WR_START;
	
	while((SIx->SI_CTRLR & BIT_CTRLR_SI_WR_START) && (++cnt) < 100000);
	
	if (cnt == 100000){
		DBG_8195A("write codec reg fail!!\n");
	}
}

/**
  * @brief	SI read codec register.
  * @param  address: codec register address.
  * @retval value of register.
  */
u16 AUDIO_SI_ReadReg(u32 address)
{
	u32 reg_value = 0;
	u32 cnt = 0;
	AUDIO_SI_TypeDef* SIx = AUDIO_SI_DEV;

	/* Configure Read command */
	SIx->SI_CTRLR = (address << 8) | BIT_CTRLR_SI_RD_START;

	do {
		reg_value = SIx->SI_CTRLR;
	/* waiting for read done */
	} while((reg_value & BIT_CTRLR_SI_RD_START) && (++cnt) < 100000);

	if (cnt == 100000){
		DBG_8195A("read codec reg fail!!\n");
		return 0;
	}

	return ((reg_value & BIT_CTRLR_SI_DATA) >> 16);
}

/**
  * @brief  Turn on or turn off the clock of register bank of audio codec.
  * @param  new_state: new state of the clock of register bank of audio codec.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SI_ClkCmd(u8  new_state)
{
	AUDIO_SI_TypeDef* SIx = AUDIO_SI_DEV;
	
	if (new_state == ENABLE) {
		SIx->SI_CLK_EN |= BIT_SI_CLK_EN;
	} else {
		SIx->SI_CLK_EN &= ~ BIT_SI_CLK_EN;
	}
}

/**
  * @brief  Initialize codec peripheral.
  * @param  sample_rate: codec ADC and DAC sample rate.
  *          This parameter can be one of the following values:
  *            @arg SR_8K: sample rate is 8kHz
  *            @arg SR_16K: sample rate is 16kHz
  *            @arg SR_32K: sample rate is 32kHz
  *            @arg SR_48K: sample rate is 48kHz
  *            @arg SR_96K: sample rate is 96kHz
  *            @arg SR_44P1K: sample rate is 44.1kHz
  *            @arg SR_88P2K: sample rate is 88.2kHz
  * @param  word_len: data sample bit
  *          This parameter can be one of the following values:
  *            @arg WL_16: sample bit is 16 bit
  *            @arg WL_24: sample bit is 24 bit
  *            @arg WL_8: sample bit is 8 bit
  * @param  mono_stereo: mono channel or stereo channel
  *          This parameter can be one of the following values:
  *            @arg CH_STEREO: stereo channel, channel number is 2
  *            @arg CH_MONO: mono channel, channel number is 1
  * @param  application: application mode
  *          This parameter can be one of the following values:
  *            @arg APP_AMIC_IN: analog microphone in
  *            @arg APP_DMIC_IN: digital microphone in
  *            @arg APP_LINE_IN: line in
  *            @arg APP_LINE_OUT: line out
  *            @arg APP_DAAD_LPBK: loopback
  * @return  None
  */
void CODEC_Init(u32 sample_rate, u32 word_len, u32 mono_stereo, u32 application)
{
    uint32_t reg_value = 0, reg_value2 = 0;

	reg_value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AUDIO_SHARE_PAD_CTRL);
	reg_value &= ~(BIT_LSYS_MASK_AC_LDO_REG << BIT_LSYS_SHIFT_AC_LDO_REG);
	//restore the pad power
	if (is_power_supply18()){
		reg_value |= ((u32)0x68) << BIT_LSYS_SHIFT_AC_LDO_REG;
	}
	else{
		reg_value |= ((u32)0x2A) << BIT_LSYS_SHIFT_AC_LDO_REG;
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AUDIO_SHARE_PAD_CTRL, reg_value);
	
    /* ================= CODEC initialize ======================== */

	if ((application&APP_DAAD_LPBK) == APP_DAAD_LPBK){

		AUDIO_SI_WriteReg(0x12, 0x0a10);
		AUDIO_SI_WriteReg(0x15, 0x0a10);

		reg_value = AUDIO_SI_ReadReg(0x1b);
		reg_value |= 0x7;
		AUDIO_SI_WriteReg(0x1b, reg_value);			

		reg_value = AUDIO_SI_ReadReg(0x1b);
		reg_value |= 0x10;
		AUDIO_SI_WriteReg(0x1b, reg_value);			

		AUDIO_SI_WriteReg(0x18,0x1fbf);		
		
		reg_value = AUDIO_SI_ReadReg(0xfb);
		reg_value |= 0x2;
		AUDIO_SI_WriteReg(0xfb, reg_value);	
		
		reg_value = AUDIO_SI_ReadReg(0x11);
		reg_value &= 0xdfff;
		reg_value |= 0x1000;
		AUDIO_SI_WriteReg(0x11, reg_value);		

		reg_value = AUDIO_SI_ReadReg(0x14);
		reg_value &= 0xdfff;
		reg_value |= 0x1000;
		AUDIO_SI_WriteReg(0x14, reg_value);	
		
		reg_value = AUDIO_SI_ReadReg(0x18);
		reg_value |= 0xe000;
		AUDIO_SI_WriteReg(0x18, reg_value);

		AUDIO_SI_WriteReg(0x1b, 0x0017);
		
		AUDIO_SI_WriteReg(0x17, (sample_rate << 4) | sample_rate);

    	AUDIO_SI_WriteReg(0x10, 0x80);	
			
		return;
	}

	reg_value = AUDIO_SI_ReadReg(0x03);		//VREF voltage selection
	reg_value &= 0xf3ff;
	reg_value |= 0x0800;
	AUDIO_SI_WriteReg(0x03, reg_value);


	if ((application&APP_LINE_OUT) == APP_LINE_OUT){

	
		reg_value = AUDIO_SI_ReadReg(0x18); 	//dac modulation/filter/fifo on
		reg_value |= 0x003f;
		AUDIO_SI_WriteReg(0x18, reg_value); 

		//step1
		reg_value = AUDIO_SI_ReadReg(0x00);
		reg_value &= 0x0c00;
		reg_value |= 0x031b;
		AUDIO_SI_WriteReg(0x00, reg_value);	
		
		AUDIO_SI_WriteReg(0x01, 0x07fc);

		reg_value = AUDIO_SI_ReadReg(0x02);
		reg_value &= 0x1c00;
		reg_value |= 0x81ea;
		AUDIO_SI_WriteReg(0x02, reg_value);

		AUDIO_SI_WriteReg(0x0c, 0x0010);
		
		//step2
		AUDIO_SI_WriteReg(0x01, 0x9ffc);

		reg_value = AUDIO_SI_ReadReg(0x02);
		reg_value &= 0x1c00;
		reg_value |= 0x81fa;
		AUDIO_SI_WriteReg(0x02, reg_value);
		
		//step3
		reg_value = AUDIO_SI_ReadReg(0x00);
		reg_value &= 0x0c00;		
		reg_value |= 0x039b;
		AUDIO_SI_WriteReg(0x00, reg_value);

		AUDIO_SI_WriteReg(0x0c, 0x0018);

	}

	if (((application&APP_AMIC_IN) == APP_AMIC_IN) || ((application&APP_LINE_IN) == APP_LINE_IN)){
		reg_value = AUDIO_SI_ReadReg(0x2);		//vref power on
		reg_value &= 0x9fff;
		reg_value |= 0x8100;
		AUDIO_SI_WriteReg(0x2, reg_value);
		DelayUs(5);

		if ((application&APP_AMIC_IN) == APP_AMIC_IN){
			reg_value = AUDIO_SI_ReadReg(0x0c);		//micbias chopper clock enable
			reg_value |= 0x0010;
			AUDIO_SI_WriteReg(0x0c, reg_value);	
			
			reg_value = AUDIO_SI_ReadReg(0x2);		//release micbias chopper clock gate and mic_bias on
			reg_value |= 0x0600;
			AUDIO_SI_WriteReg(0x2, reg_value);	
			DelayUs(100);

			reg_value = AUDIO_SI_ReadReg(0x3);		//PGA on and unmute mic
			reg_value &= 0xff0f;
			reg_value |= 0x03a0;
			AUDIO_SI_WriteReg(0x3, reg_value);	
		}
		else{
			reg_value = AUDIO_SI_ReadReg(0x3);		//PGA on and unmute line-in
			reg_value &= 0xff00;
			reg_value |= 0x0350;
			AUDIO_SI_WriteReg(0x3, reg_value);	

		}
	}

	if (((application&APP_AMIC_IN) == APP_AMIC_IN) || ((application&APP_LINE_IN) == APP_LINE_IN) || ((application&APP_LINE_OUT) == APP_LINE_OUT)) { 
		DelayMs(200); 
	} 

	if ((application&APP_LINE_OUT) == APP_LINE_OUT){
		//step5
		reg_value = AUDIO_SI_ReadReg(0x00);
		reg_value &= 0x0c00;		
		reg_value |= 0x021b;
		AUDIO_SI_WriteReg(0x00, reg_value);

		AUDIO_SI_WriteReg(0x01, 0x04fc);
		
		reg_value = AUDIO_SI_ReadReg(0x02);
		reg_value &= 0x1c00;
		reg_value |= 0x81ea;
		AUDIO_SI_WriteReg(0x02, reg_value);
		
		AUDIO_SI_WriteReg(0x0c, 0x0010);
	}
	
	if (((application&APP_AMIC_IN) == APP_AMIC_IN) || ((application&APP_LINE_IN) == APP_LINE_IN)){
		reg_value = AUDIO_SI_ReadReg(0x0);		//adc on
		reg_value |= 0x0e01;
		AUDIO_SI_WriteReg(0x0, reg_value);	

		DelayUs(50);
	}
	
	/* Configure ADC and DAC corresponding clock */
	AUDIO_SI_WriteReg(0x18,0x1fbf);
	/* Left channel: Enable High pass filter enable control (filter DC), analog ADC input path mute control Left Channel */
	/* Enable ADC SRC 1st LPF control and ADC SRC 2nd LPF control, DMIC SRC 1st LPF fc:46.92kHz */
	AUDIO_SI_WriteReg(0x11, 0x6320);
	/* Right channel: Enable High pass filter enable control (filter DC), analog ADC input path mute control Left Channel */
	/* Enable ADC SRC 1st LPF control and ADC SRC 2nd LPF control, DMIC SRC 1st LPF fc:46.92kHz */
	AUDIO_SI_WriteReg(0x14, 0x6320);

	if (((application&APP_AMIC_IN) == APP_AMIC_IN) || ((application&APP_LINE_IN) == APP_LINE_IN)){
		reg_value = AUDIO_SI_ReadReg(0x12);
		reg_value2 = AUDIO_SI_ReadReg(0x15);

		reg_value &= 0xf9c7;	//clear [10:9][5:3], immediate change and clear fc selection field.
		reg_value |= 0x4;		//set BIT2, enable adaptive 2nd HPF
		reg_value2 &= 0xf9c7;
		reg_value2 |= 0x4;
		
		switch(sample_rate){
			case SR_8K:
			case SR_16K:
				break;
			case SR_32K:
				reg_value |= 0x8;
				reg_value2 |= 0x8;
				break;
			case SR_44P1K:
			case SR_48K:
				reg_value |= 0x10;
				reg_value2 |= 0x10;
				break;				
			case SR_88P2K:
			case SR_96K:
				reg_value |= 0x18;
				reg_value2 |= 0x18;
				break;
			default:
				break;					
		}
		AUDIO_SI_WriteReg(0x12, reg_value);
		AUDIO_SI_WriteReg(0x15, reg_value2);
		DelayMs(50);		//maybe need fine tune per board
	}

	AUDIO_SI_WriteReg(0x1b, 0x0017);
	/* ADC and DAC sample rate 16K */
	AUDIO_SI_WriteReg(0x17, (sample_rate << 4) | sample_rate);

	/* Left channel: mon DAC Lch 128fs-domain mixer sidetone path mute enable, Digital DAC & ADC loop back control, mon DAC Lch dvol mute enable */
	reg_value = AUDIO_SI_ReadReg(0xfb);
	//AUDIO_SI_WriteReg(0xfb, 0x4);
	reg_value |= BIT3;
	AUDIO_SI_WriteReg(0xfb, reg_value & 0xfff9);
	/* Right channel: mon DAC Rch 128fs-domain mixer sidetone path mute enable, Digital DAC & ADC loop back control, mon DAC Rch dvol mute enable */
	//AUDIO_SI_WriteReg(0xfd, 0x4);
	reg_value = AUDIO_SI_ReadReg(0xfd);
	reg_value |= BIT3;
	AUDIO_SI_WriteReg(0xfd, reg_value & 0xfff9);

	/* Enable control for dac filter; ALC is stereo mode */
	reg_value = AUDIO_SI_ReadReg(0x27);
	reg_value &= 0xfeff;
	if (mono_stereo == CH_STEREO){
		reg_value |= BIT8;
	}
	AUDIO_SI_WriteReg(0x27, reg_value);

	reg_value = AUDIO_SI_ReadReg(0xfa);
	reg_value &= 0x5ff;
	reg_value |= 0x1000;
	AUDIO_SI_WriteReg(0xfa, reg_value);
	AUDIO_SI_WriteReg(0xfc, reg_value);

	if ((application&APP_DMIC_IN) == APP_DMIC_IN){
		reg_value = AUDIO_SI_ReadReg(0x11);
		reg_value &= 0xdffe;
		reg_value |= 0x5019;
		AUDIO_SI_WriteReg(0x11, reg_value);
		
		reg_value = AUDIO_SI_ReadReg(0x12);
		reg_value |= 0x04;
		AUDIO_SI_WriteReg(0x12, reg_value);

		reg_value = AUDIO_SI_ReadReg(0x14);
		reg_value &= 0xdffe;
		reg_value |= 0x5018;
		AUDIO_SI_WriteReg(0x14, reg_value);	//bit0: 0 means latching rising edge, whereas 1 means falling edge.
		
		reg_value = AUDIO_SI_ReadReg(0x15);
		reg_value |= 0x04;
		AUDIO_SI_WriteReg(0x15, reg_value);
		

		reg_value = AUDIO_SI_ReadReg(0x17);
		reg_value &= 0xf8ff;
		reg_value |= 0x0100;
		AUDIO_SI_WriteReg(0x17, reg_value);		

		reg_value = AUDIO_SI_ReadReg(0x18);
		reg_value |= 0xe000;
		AUDIO_SI_WriteReg(0x18, reg_value);
	}

	if ((sample_rate == SR_96K) || (sample_rate == SR_88P2K)){
		reg_value = AUDIO_SI_ReadReg(0x17);
		reg_value &= 0x87ff;
		reg_value |= 0x0800;
		AUDIO_SI_WriteReg(0x17, reg_value);

		reg_value = AUDIO_SI_ReadReg(0x1b);
		reg_value &= 0xfff3;
		AUDIO_SI_WriteReg(0x1b, reg_value);		
	}
	
	reg_value = ((word_len << 4) |mono_stereo | 0x0080);
	AUDIO_SI_WriteReg(0x10, reg_value);
}

/**
  * @brief  Set codec volume by controlling mon DAC channel dvol gain.
  * @param  vol_lch: mon DAC Lch dvol gain control(0.375dB/step)
  *          This parameter can be one of the following values:
  *            @arg 8'hAF: 0dB
  *            @arg ...
  *            @arg 8'h00: -65.625dB
  * @param  vol_rch: mon DAC Rch dvol gain control(0.375dB/step)
  *          This parameter can be one of the following values:
  *            @arg 8'hAF: 0dB
  *            @arg ...
  *            @arg 8'h00: -65.625dB
  * @return  None
  */
void CODEC_SetVolume(u8 vol_lch, u8 vol_rch)
{
	u32 reg_value = 0;
	
	reg_value = AUDIO_SI_ReadReg(0xfa);
	reg_value &= ~0xff;
	reg_value |= vol_lch;
	AUDIO_SI_WriteReg(0xfa, reg_value);

	reg_value = AUDIO_SI_ReadReg(0xfc);
	reg_value &= ~0xff;
	reg_value |= vol_rch;
	AUDIO_SI_WriteReg(0xfc, reg_value);
}

/**
  * @brief  Get codec mon DAC channel gain control.
  * @param  vol: mon DAC DAC channel dvol gain
  * @note vol value high 8 bits is rch gain, low 8 bits are lch gain
  * @return  None
  */
void CODEC_GetVolume(u16 *vol)
{
	u8 vol_lch = 0;
	u8 vol_rch = 0;
	
	 vol_lch = AUDIO_SI_ReadReg(0xfa) & 0xff;
	 vol_rch = AUDIO_SI_ReadReg(0xfc) & 0xff;
	*vol = (vol_rch << 8) | vol_lch; 
}

/**
  * @brief  Set codec ADC and DAC sample rate.
  * @param  sample_rate: codec ADC and DAC sample rate.
  *          This parameter can be one of the following values:
  *            @arg SR_8K: sample rate is 8kHz
  *            @arg SR_16K: sample rate is 16kHz
  *            @arg SR_32K: sample rate is 32kHz
  *            @arg SR_48K: sample rate is 48kHz
  *            @arg SR_96K: sample rate is 96kHz
  *            @arg SR_44P1K: sample rate is 44.1kHz
  *            @arg SR_88P2K: sample rate is 88.2kHz
  * @return  None
  */
void CODEC_SetSr(u32 sample_rate){
	u32 reg_value = 0;
	
	if ((sample_rate == SR_96K) || (sample_rate == SR_88P2K)){
		reg_value = AUDIO_SI_ReadReg(0x17);
		reg_value &= 0x8700;
		reg_value |= (0x0800|(sample_rate << 4) | sample_rate);
		AUDIO_SI_WriteReg(0x17, reg_value);

		reg_value = AUDIO_SI_ReadReg(0x1b);
		reg_value &= 0xfff3;
		AUDIO_SI_WriteReg(0x1b, reg_value);		
	}
	else{
		reg_value = AUDIO_SI_ReadReg(0x17);
		reg_value &= 0x8700;
		reg_value |= ((sample_rate << 4) | sample_rate);
		AUDIO_SI_WriteReg(0x17, reg_value);

		reg_value = AUDIO_SI_ReadReg(0x1b);
		reg_value &= 0xfff3;
		reg_value |= 0x01<<2;
		AUDIO_SI_WriteReg(0x1b, reg_value);				
	}

}

/**
  * @brief  Set codec ADC and DAC channel number.
  * @param  mono_stereo: codec ADC and DAC channel number.
  *          This parameter can be one of the following values:
  *            @arg CH_MONO: mono
  *            @arg CH_STEREO: stereo
  * @return  None
  */
void CODEC_SetCh(u32 mono_stereo){
	u32 reg_value = 0;

	reg_value = AUDIO_SI_ReadReg(0x10);
	reg_value &= 0xfffe;
	reg_value |= mono_stereo;
	AUDIO_SI_WriteReg(0x10, reg_value);	

	reg_value = AUDIO_SI_ReadReg(0x27);
	reg_value &= 0xfeff;
	if (mono_stereo == CH_STEREO){
		reg_value |= BIT8;
	}
	AUDIO_SI_WriteReg(0x27, reg_value);
}


/**
  * @brief  Set codec ADC gain.
  * @param  ad_gain_left: ADC left channel digital volume gain
  *          This parameter can be one of the following values:
  *            @arg 7'h00: -17.625dB
  *            @arg ...
  *            @arg 7'h2f: 0dB
  *            @arg 7'h30: 0.375dB
  *            @arg ...
  *            @arg 7'h7f: 30dB
  * @param  ad_gain_right: ADC right channel digital volume gain
  *          This parameter can be one of the following values:
  *            @arg 7'h00: -17.625dB
  *            @arg ...
  *            @arg 7'h2f: 0dB
  *            @arg 7'h30: 0.375dB
  *            @arg ...
  *            @arg 7'h7f: 30dB
  * @note ADC digital volume is -17.625dB~+30dB in 0.375 dB step.
  * @return  None
  */
void CODEC_SetAdcGain(u32 ad_gain_left, u32 ad_gain_right)
{
	u32 reg_value = 0;

	reg_value = AUDIO_SI_ReadReg(0x13);
	reg_value &= ~0x1fc0;
	reg_value |= (ad_gain_left << 6);
	AUDIO_SI_WriteReg(0x13, reg_value);
	
	reg_value = AUDIO_SI_ReadReg(0x16);
	reg_value &= ~0x1fc0;
	reg_value |= (ad_gain_right << 6);
	AUDIO_SI_WriteReg(0x16, reg_value);	
}

/**
  * @brief  Set codec amic boost.
  * @param  amic_bst_left: AMIC left channel boost gain
  *          This parameter can be one of the following values:
  *            @arg 2'b00: 0dB
  *            @arg 2'b01: 20dB
  *            @arg 2'b10: 30dB
  *            @arg 2'b11: 40dB
  * @param  amic_bst_right: AMIC right channel boost gain
  *          This parameter can be one of the following values:
  *            @arg 2'b00: 0dB
  *            @arg 2'b01: 20dB
  *            @arg 2'b10: 30dB
  *            @arg 2'b11: 40dB
  * @return  None
  */
void CODEC_SetAmicBst(u32 amic_bst_left, u32 amic_bst_right)
{
	u32 reg_value = 0;

	reg_value = AUDIO_SI_ReadReg(0x03);
	reg_value &= ~0x000f;
	reg_value |= ((amic_bst_left&0x03)|((amic_bst_right&0x03)<<2));
	AUDIO_SI_WriteReg(0x03, reg_value);
}

/**
  * @brief  Set codec dmic boost.
  * @param  dmic_bst_left: DMIC left channel boost gain
  *          This parameter can be one of the following values:
  *            @arg 2'b00: 0dB
  *            @arg 2'b01: 12dB
  *            @arg 2'b10: 24dB
  *            @arg 2'b11: 36dB
  * @param  dmic_bst_right: DMIC right channel boost gain
  *          This parameter can be one of the following values:
  *            @arg 2'b00: 0dB
  *            @arg 2'b01: 12dB
  *            @arg 2'b10: 24dB
  *            @arg 2'b11: 36dB
  * @return  None
  */
void CODEC_SetDmicBst(u32 dmic_bst_left, u32 dmic_bst_right)
{
	u32 reg_value = 0;

	reg_value = AUDIO_SI_ReadReg(0x12);
	reg_value &= ~0x00c0;
	reg_value |= (dmic_bst_left&0x03)<<6;
	AUDIO_SI_WriteReg(0x12, reg_value);
	
	reg_value = AUDIO_SI_ReadReg(0x15);
	reg_value &= ~0x00c0;
	reg_value |= (dmic_bst_right&0x03)<<6;
	AUDIO_SI_WriteReg(0x15, reg_value);
}

/**
  * @brief  Set MIC_BIAS output voltage.
  * @param  mic_bias: micphone bias voltage setting
  *          This parameter can be one of the following values:
  *            @arg 2'b00: 0.90*AVCC
  *            @arg 2'b01: 0.86*AVCC
  *            @arg 2'b10: 0.75*AVCC
  *            @arg 2'b11: reserved
  * @note AVCC is 2.8V@3.3V operation mode or 1.7V@1.8V operation mode
  * @return  None
  */
void CODEC_SetMicBias(u8 mic_bias)
{
	u32 reg_value = 0;

	reg_value = AUDIO_SI_ReadReg(0x02);
	reg_value &= ~0x1800;
	reg_value |= ((mic_bias&0x03) << 11);
	AUDIO_SI_WriteReg(0x02, reg_value);	
}

/**
  * @brief  mute or unmute per ad channel.
  * @param  mute_lch: mute option for left ad channel
  *			 This parameter can be one of the following values:
  *			   @arg MUTE_DISABLE: unmute
  *			   @arg MUTE_ENABLE: mute
  *			   @arg MUTE_NO_ACT: no modification
  * @param  mute_rch: mute option for right ad channel
  *			 This parameter can be one of the following values:
  *			   @arg MUTE_DISABLE: unmute
  *			   @arg MUTE_ENABLE: mute
  *			   @arg MUTE_NO_ACT: no modification 
  * @return  None
  */
void CODEC_MuteRecord(u32 mute_lch, u32 mute_rch)
{
	u32 reg_value = 0;

	if (mute_lch == MUTE_ENABLE){
		reg_value = AUDIO_SI_ReadReg(0x12);		//left ad mute
		reg_value |= 0x0100;
		AUDIO_SI_WriteReg(0x12, reg_value);						
	}
	else if (mute_lch == MUTE_DISABLE){
		reg_value = AUDIO_SI_ReadReg(0x12);		//left ad unmute
		reg_value &= 0xfeff;
		AUDIO_SI_WriteReg(0x12, reg_value);			

	}

	if (mute_rch == MUTE_ENABLE){
		reg_value = AUDIO_SI_ReadReg(0x15);		//right ad mute
		reg_value |= 0x0100;
		AUDIO_SI_WriteReg(0x15, reg_value);						
	}
	else if (mute_rch == MUTE_DISABLE){
		reg_value = AUDIO_SI_ReadReg(0x15);		//right ad unmute
		reg_value &= 0xfeff;
		AUDIO_SI_WriteReg(0x15, reg_value);			

	}
	return;
}

/**
  * @brief  mute or unmute per da channel.
  * @param  mute_lch: mute option for left da channel
  *			 This parameter can be one of the following values:
  *			   @arg MUTE_DISABLE: unmute
  *			   @arg MUTE_ENABLE: mute
  *			   @arg MUTE_NO_ACT: no modification
  * @param  mute_rch: mute option for right da channel
  *			 This parameter can be one of the following values:
  *			   @arg MUTE_DISABLE: unmute
  *			   @arg MUTE_ENABLE: mute
  *			   @arg MUTE_NO_ACT: no modification 
  * @return  None
  */
void CODEC_MutePlay(u32 mute_lch, u32 mute_rch)
{
	u32 reg_value = 0;

	if (mute_lch == MUTE_ENABLE){
		reg_value = AUDIO_SI_ReadReg(0xfb); 	//left da mute
		reg_value |= 0x0001;
		AUDIO_SI_WriteReg(0xfb, reg_value); 			
	}
	else if (mute_lch == MUTE_DISABLE){
		reg_value = AUDIO_SI_ReadReg(0xfb); 	//left da unmute
		reg_value &= 0xfffe;
		AUDIO_SI_WriteReg(0xfb, reg_value); 
	
	}
	
	if (mute_rch == MUTE_ENABLE){
		reg_value = AUDIO_SI_ReadReg(0xfd); 	//right da mute
		reg_value |= 0x0001;
		AUDIO_SI_WriteReg(0xfd, reg_value); 				
	}
	else if (mute_rch == MUTE_DISABLE){
		reg_value = AUDIO_SI_ReadReg(0xfd); 	//right da unmute
		reg_value &= 0xfffe;
		AUDIO_SI_WriteReg(0xfd, reg_value); 	
	
	}
	return;
}

/**
  * @brief  De-initialize codec peripheral.
  * @param  application: application mode
  *          This parameter can be one of the following values:
  *            @arg APP_AMIC_IN: analog microphone in
  *            @arg APP_DMIC_IN: digital microphone in
  *            @arg APP_LINE_IN: line in
  *            @arg APP_LINE_OUT: line out
  * @return  None
  */
void CODEC_DeInit(u32 application)
{
	u32 reg_value = 0;
	
	if (((application&APP_AMIC_IN) == APP_AMIC_IN) || ((application&APP_LINE_IN) == APP_LINE_IN)){
#if 0
		reg_value = AUDIO_SI_ReadReg(0x11);		//left ad filter off
		reg_value &= ~0x4000;
		AUDIO_SI_WriteReg(0x11, reg_value);	

		reg_value = AUDIO_SI_ReadReg(0x14);		//right ad filter off
		reg_value &= ~0x4000;
		AUDIO_SI_WriteReg(0x14, reg_value);	
#endif		
		reg_value = AUDIO_SI_ReadReg(0x0);		//adc off
		reg_value &= ~0x0c00;
		AUDIO_SI_WriteReg(0x0, reg_value);	

		reg_value = AUDIO_SI_ReadReg(0x3);		//PGA off and path mute
		reg_value &= ~0x0300;
		reg_value |= 0x00f0;
		AUDIO_SI_WriteReg(0x3, reg_value);	

		if ((application&APP_AMIC_IN) == APP_AMIC_IN){
			reg_value = AUDIO_SI_ReadReg(0x2);		//mic_bias off
			reg_value &= ~0x0400;
			AUDIO_SI_WriteReg(0x2, reg_value);				
		}
	}

	if ((application&APP_DMIC_IN) == APP_DMIC_IN){
		reg_value = AUDIO_SI_ReadReg(0x18);		//dmic clock off
		reg_value &= ~0xe000;
		AUDIO_SI_WriteReg(0x18, reg_value);	
		
	}

	if ((application&APP_LINE_OUT) == APP_LINE_OUT){

		reg_value = AUDIO_SI_ReadReg(0x1);		//amplifier off
		reg_value &= ~0x00c0;
		AUDIO_SI_WriteReg(0x1, reg_value);	
		
		reg_value = AUDIO_SI_ReadReg(0x2);		//path mute and right channel off
		reg_value &= ~0x0020;
		reg_value |= 0x000f;
		AUDIO_SI_WriteReg(0x2, reg_value);

		reg_value = AUDIO_SI_ReadReg(0x1);		//left channel off and amplifier off
		reg_value &= ~0x0400;
		AUDIO_SI_WriteReg(0x1, reg_value);		

		reg_value = AUDIO_SI_ReadReg(0x0);		//dac off
		reg_value &= ~0x0018;
		AUDIO_SI_WriteReg(0x0, reg_value);		

		reg_value = AUDIO_SI_ReadReg(0x18);		//dac modulation/filter/fifo reset
		reg_value &= ~0x003f;
		AUDIO_SI_WriteReg(0x18, reg_value);	
	}
}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor *****END OF FILE****/
