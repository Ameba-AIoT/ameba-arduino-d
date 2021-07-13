/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      ir_led.c
* @brief    This file provides driver of nec protocol encoding.
* @details
* @author  elliot_chen
* @date     2016-12-08
* @version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "ir_led.h"
#include "rtl_lib.h"

/** @addtogroup  IO_DEMO_APP IO_DEMO APP
    * @brief
    * @{
    */

/** @defgroup  IR_NEC_PROTOCOL  IR NEC PROTOCOL
    * @brief  Ir nec protocol implementation demo code
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/

#define ABS_TIME(a,b) ((a>b) ? (a-b):(b-a))


/*============================================================================*
 *                              Constants
 *============================================================================*/

/*!
* @ brief:LED structure.
* @ note: Store parameters of led waveform.
* @ Carrier frequency = 10MHz
* @ duty factor = 1
* @ LSB is sent first !
*/
const LED_ProtocolTypeDef LED_PROTOCOL =
{
	10000,                                                 /* Carrier freqency KHz*/
	{PULSE_HIGH | 300, PULSE_LOW | 800},                 /* log0Buf unit: ns*/
	{PULSE_HIGH | 800, PULSE_LOW | 250},               /* log1Buf */
	PULSE_LOW | 300000,                                  /* stopBuf */
	30                                                  /* tolerance percentage is 10% */
};


/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
  * @brief    unit convert.
  * @param  time: time of waveform (ns).
  * @param  carrier_cycle: cycle of carrier.
  * @retval   vaule of data whose unit is cycle of carrier.
  */
static IR_DataType ConvertToCarrierCycle(uint32_t time, uint32_t freq)
{
	return ((time & PULSE_HIGH) | ((time & IR_DATA_MSK) * freq / 1000000));
}

/**
  * @brief   check specify bit value of data.
  * @param a: data which want to check.
  * @param  bit_pos: position of bit which want to check
  * @retval
  */
static uint8_t CheckBitStatus(uint32_t a, uint32_t bit_pos)
{
	return ((a >> bit_pos) & 0x1);
}

/**
  * @brief    Encode data to packet.
  * @param  IR_DataStruct: pointer to struct which store NEC code.
  * @param  IR_Protocol: pointer to specify IR protocol structure.
  * @retval None
  */
static IR_RETURN_Type LED_EncodePacket(IR_DataTypeDef *IR_DataStruct,
                                       LED_ProtocolTypeDef *IR_Protocol, int flag)
{
	uint8_t codeWidth = 0;
	uint16_t index = 0;
	uint16_t bitPos = 0;
	uint16_t bufLen = 0;
	IR_DataType Log1[MAX_LOG_WAVFORM_SIZE];
	IR_DataType Log0[MAX_LOG_WAVFORM_SIZE];

    /* Error handle */
    //if (IR_DataStruct->carrierFreq != IR_Protocol->carrierFreq)
   // {
   //     return IR_FREQENCY_ERROR;
   // }

    /* Encoding logical 1 and logical 0 */
    	for (index = 0; index < MAX_LOG_WAVFORM_SIZE; index++)
    	{
        	Log1[index] = ConvertToCarrierCycle(IR_Protocol->log1Buf[index] , IR_DataStruct->carrierFreq);
        	Log0[index] = ConvertToCarrierCycle(IR_Protocol->log0Buf[index], IR_DataStruct->carrierFreq);	
    	}
    /* Encoding command code */
    	for (index = 0; index < IR_DataStruct->codeLen; index++)
    	{
       	 /* Decide coding width */
            	codeWidth = DATA_CODE_WIDTH;

        /* Encoding handle */
	        for (bitPos = 0; bitPos < codeWidth; bitPos++)
	        {
	            	if (CheckBitStatus(IR_DataStruct->code[index], bitPos) == 0x01)
	            	{
	                	/* Logical 1 data */
	                	IR_DataStruct->irBuf[bufLen] = Log1[0];
	                	IR_DataStruct->irBuf[bufLen + 1] = Log1[1];
	            	}
	            	else
	            	{
	               	 /* Logical 0 data */
	                	IR_DataStruct->irBuf[bufLen] = Log0[0];
	                	IR_DataStruct->irBuf[bufLen + 1] = Log0[1];
	            	}
	            	bufLen += MAX_LOG_WAVFORM_SIZE;
	        }
    	}
    /* Encode stop code */
	if (IR_Protocol->stopBuf != 0 && flag == 1)
    	{
        	IR_DataStruct->irBuf[bufLen] = ConvertToCarrierCycle(IR_Protocol->stopBuf, IR_DataStruct->carrierFreq);
        	bufLen++;
    	}
    	IR_DataStruct->bufLen = bufLen;

    	return IR_SUCCEED;
}

/**
  * @brief    Encode data of LED waveform.
  * @param  data: user code.
  * @param   IR_DataStruct: pointer to struct which store LED waveform.
  * @retval None
  */
IR_RETURN_Type IR_LEDEncode(uint32_t freqency, uint8_t *data, IR_DataTypeDef * IR_DataStruct, int flag)
{
	IR_DataStruct->carrierFreq = freqency/1000;
    	IR_DataStruct->codeLen = 3;
    	IR_DataStruct->code[0] = data[0];
    	IR_DataStruct->code[1] = data[1];
    	IR_DataStruct->code[2] = data[2];

	return LED_EncodePacket(IR_DataStruct, (LED_ProtocolTypeDef *)(&LED_PROTOCOL), flag);	
}

/** @} */ /* End of group IR_LED */
/** @} */ /* End of group GPIO_OUTPUT_DEMO */

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

