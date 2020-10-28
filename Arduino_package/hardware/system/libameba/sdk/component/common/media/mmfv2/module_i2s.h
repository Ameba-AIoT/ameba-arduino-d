#ifndef _MODULE_I2S_H
#define _MODULE_I2S_H

#include "mmf2_module.h"
#include "audio_api.h"
#include "i2s_api.h"

#define ENABLE_SPEEX_AEC 1

#define CMD_I2S_SET_PARAMS			MM_MODULE_CMD(0x00)  // set parameter
#define CMD_I2S_GET_PARAMS			MM_MODULE_CMD(0x01)  // get parameter
#define CMD_I2S_SET_SAMPLERATE		MM_MODULE_CMD(0x02)
#define CMD_I2S_SET_WORDLENGTH		MM_MODULE_CMD(0x03)
#define CMD_I2S_SET_MICGAIN			MM_MODULE_CMD(0x04)
#define CMD_I2S_SET_AES				MM_MODULE_CMD(0x05)
#define CMD_I2S_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

typedef struct i2s_param_s{
	int		    sample_rate;	// SR_32KHZ
        int              out_sample_rate;	// SR_8KHZ
	int		    word_length;	// WL_24b
	int		    out_word_length;	// WL_16b
	audio_mic_gain	mic_gain;		// MIC_40DB
	int				channel;		// 1
    int             out_channel;
	int				enable_aec;		// 0
	int				mix_mode;		// 0
	//...
}i2s_params_t;

typedef struct i2s_ctx_s{
	void*				parent;
	i2s_t* 				i2s_obj;
	i2s_params_t		params;
	uint32_t			sample_rate;
	uint8_t				word_length; // Byte
    int                 sr_shrink;
    int                 wl_shrink;
    int                 ch_shrink;
	// for AEC
	TaskHandle_t		aec_rx_task;
	xSemaphoreHandle		aec_rx_done_sema;
}i2s_ctx_t;

extern mm_module_t i2s_module;
#endif
