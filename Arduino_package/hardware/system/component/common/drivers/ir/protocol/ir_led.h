/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      ir_led.h
* @brief    This file provides driver of led encoding.
* @details
* @author    elliot chen
* @date      2016-12-08
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __IR_LED_H
#define __IR_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines ------------------------------------------------------------------*/

typedef uint32_t IR_DataType;

/* Carrier waveform data type bit select */
#define PACK_TYPE               ((uint32_t)31)
/* Carrier waveform data type select */
#define PULSE_HIGH              ((uint32_t)0x80000000)
/* No carrier waveform data type select */
#define PULSE_LOW               0x0
/* IR data mask */
#define IR_DATA_MSK             0x7fffffffUL

/*when receiving waveform is inverse, we should define INVERSE_DATA*/
//#define INVERSE_DATA

/* Configure data code Width */
#define DATA_CODE_WIDTH          8

/* Configure maxium length of data code */
#define MAX_CODE_SIZE           3
/* Configure maxium length of data sent by IR peripheral */
#define MAX_IR_BUF_SIZE         51
/* Configure maxium data length of logical 0 or logical 1 */
#define MAX_LOG_WAVFORM_SIZE    2

/**
  * @brief  IR data structure definition
  */
typedef struct
{
    /* Unit of carrierFreq is Hz */
    uint32_t         carrierFreq;
    uint8_t         code[MAX_CODE_SIZE];
    uint8_t         codeLen;
    IR_DataType     irBuf[MAX_IR_BUF_SIZE];
    uint16_t        bufLen;
} IR_DataTypeDef;

/**
  * @brief  IR protocol structure definition
  */
typedef struct
{
	uint16_t         carrierFreq;
    	IR_DataType     log0Buf[MAX_LOG_WAVFORM_SIZE];
    	IR_DataType     log1Buf[MAX_LOG_WAVFORM_SIZE];
    	uint32_t        stopBuf;
    	uint32_t        tolerancePrecent;
} LED_ProtocolTypeDef;

/**
  * @brief  IR return type of encoding function
  */
typedef enum
{
    	IR_SUCCEED              = 0,
    	IR_FREQENCY_ERROR       = 1,
    	IR_HEARDE_TYPE_ERROR    = 2,
    	IR_HEADER_DATA_ERROR    = 3,
    	IR_DATA_TYPE_ERROR      = 4,
    	IR_DATA_ERROR           = 5
} IR_RETURN_Type;

IR_RETURN_Type IR_LEDEncode(uint32_t freqency, uint8_t *data, IR_DataTypeDef *IR_DataStruct, int flag);

#ifdef __cplusplus
}
#endif

#endif /*__IR_LED_H*/

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

