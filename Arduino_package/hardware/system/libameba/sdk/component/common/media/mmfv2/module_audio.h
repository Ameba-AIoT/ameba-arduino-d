#ifndef _MODULE_AUDIO_H
#define _MODULE_AUDIO_H

#include "mmf2_module.h"
#if defined(CONFIG_PLATFORM_8721D)	//amebad 
#include "ameba_soc.h"
#include "rl6548.h"
#else	//amebapro use audio api
#include "audio_api.h"
#endif
#define ENABLE_SPEEX_AEC 1

#define CMD_AUDIO_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_AUDIO_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_AUDIO_SET_SAMPLERATE	MM_MODULE_CMD(0x02)
#define CMD_AUDIO_SET_WORDLENGTH	MM_MODULE_CMD(0x03)
#define CMD_AUDIO_SET_MICGAIN		MM_MODULE_CMD(0x04)
#define CMD_AUDIO_SET_AES			MM_MODULE_CMD(0x05)
#define CMD_AUDIO_SET_ADC_GAIN          MM_MODULE_CMD(0x06)
#define CMD_AUDIO_SET_DAC_GAIN          MM_MODULE_CMD(0x07)

#define CMD_AUDIO_APPLY				MM_MODULE_CMD(0x20)  // for hardware module


#if defined(CONFIG_PLATFORM_8721D)	//amebad
typedef struct {
	GDMA_InitTypeDef       	SpTxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef
	GDMA_InitTypeDef       	SpRxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef		
}SP_GDMA_STRUCT, *pSP_GDMA_STRUCT;

typedef struct audio_param_s{
	uint32_t sample_rate;
	uint32_t word_length;
	uint32_t mono_stereo;
	uint32_t direction;
	uint32_t enable_aec;		// 0
	uint32_t mix_mode;		// 0
}audio_params_t;
#else	//amebapro
typedef struct audio_param_s{
    audio_sr        sample_rate;	// ASR_8KHZ
	audio_wl        word_length;	// WL_16BIT
	audio_mic_gain  mic_gain;		// MIC_40DB
	
	int				channel;		// 1
	int				enable_aec;		// 0
	int				mix_mode;		// 0
    //...
}audio_params_t;
#endif

typedef struct audio_ctx_s{
	void*             parent;
#if !defined(CONFIG_PLATFORM_8721D)	//amebapro
	audio_t*          audio;
#endif	
	audio_params_t    params;
	
	uint32_t          sample_rate;
	uint8_t           word_length; // Byte
	// for AEC
	TaskHandle_t      aec_rx_task;
	xSemaphoreHandle  aec_rx_done_sema;
}audio_ctx_t;


extern mm_module_t audio_module;
#endif
