/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      ir_nec_protocol.c
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
#include "ir_nec_protocol.h"
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
* @ brief:NEC protocol structure.
* @ note: Store parameters of NEC protocol.
* @ Carrier frequency = 38000Hz
* @ duty factor = 1/2
* @ first pulse : 9.5ms 4.5ms
* @ Address (8 bits) is sent first, then ~Address
* @ Command (8 bits) follows, then ~Command
* @ LSB is sent first !
*/
const IR_ProtocolTypeDef NEC_PROTOCOL =
{
	38000,                                                 /* Carrier freqency */
	2,                                                  /* headerLen */
	{PULSE_HIGH | 9000, PULSE_LOW | (4500 - 26)},              /* headerBuf unit: us*/
	{PULSE_HIGH | 560, PULSE_LOW | (560 - 26)},                 /* log0Buf */
	{PULSE_HIGH | 560, PULSE_LOW | (1690 - 26)},               /* log1Buf */
	PULSE_HIGH | 560,                                  /* stopBuf */
	30                                                  /* tolerance percentage is 10% */
};


/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
  * @brief    unit convert.
  * @param  time: time of waveform (us).
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
  * @brief    Data type conversion.
  * @param  buf: address of a buffer.
  * @param  len: buffer len.
  * @retval   none.
  */
void InvsePulse(IR_DataType *pBuf, uint16_t len)
{
	uint16_t i = 0;

	for (i = 0; i < len; i++)
	{
       	if (pBuf[i] > PULSE_HIGH)
       	{
              	pBuf[i] &= (~PULSE_HIGH);
        	}
        	else
        	{
            		pBuf[i] |= PULSE_HIGH;
        	}
    	}
}

/**
  * @brief    Judge Data is active carrier or inactive carrier.
  * @param  a: data.
  * @retval   data type.
  */
static IR_DataType CheckCarrierType(uint32_t a)
{
    	return ((a & PULSE_HIGH) >> PACK_TYPE);
}

/**
  * @brief    Get the value of data.
  * @param  a: data.
  * @retval   data type.
  */
static IR_DataType GetData(uint32_t a)
{
   	return (a & IR_DATA_MSK);
}


/**
  * @brief    unit conversion.
  * @param  counter: data which read form IR RX FIFO.
  * @param  carrier_cycle: cycle of carrier.
  * @retval   vaule of data whose unit is us.
  */
static IR_DataType ConvertToTime(uint32_t counter, uint32_t freq)
{
    	return (((counter & IR_DATA_MSK) * 1000) / (freq / 1000));
}

/**
  * @brief    Encode data to packet.
  * @param  IR_DataStruct: pointer to struct which store NEC code.
  * @param  IR_Protocol: pointer to specify IR protocol structure.
  * @retval None
  */
static IR_RETURN_Type NEC_EncodePacket(IR_DataTypeDef *IR_DataStruct,
                                       IR_ProtocolTypeDef *IR_Protocol)
{
	uint8_t codeWidth = 0;
	uint16_t index = 0;
	uint16_t bitPos = 0;
	uint16_t bufLen = IR_Protocol->headerLen;
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
        	Log1[index] = ConvertToCarrierCycle(IR_Protocol->log1Buf[index], IR_DataStruct->carrierFreq);
        	Log0[index] = ConvertToCarrierCycle(IR_Protocol->log0Buf[index], IR_DataStruct->carrierFreq);
    	}

    /* Encoding header */
    	for (index = 0; index < IR_Protocol->headerLen; index++)
    	{
        	IR_DataStruct->irBuf[index] =  ConvertToCarrierCycle(IR_Protocol->headerBuf[index],
                                                             IR_DataStruct->carrierFreq);
    	}

    /* Encoding address and command code */
    	for (index = 0; index < IR_DataStruct->codeLen; index++)
    	{
       	 /* Decide coding width */
        	if ((index % 2) == 0)
        	{
            		codeWidth = USER_CODE_WDTH;
        	}
        	else
        	{
            		codeWidth = DATA_CODE_WDTH;
        	}

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
	if (IR_Protocol->stopBuf != 0)
    	{
        	IR_DataStruct->irBuf[bufLen] = ConvertToCarrierCycle(IR_Protocol->stopBuf,
                                                             IR_DataStruct->carrierFreq);
        	bufLen++;
    	}
    	IR_DataStruct->bufLen = bufLen;

    	return IR_SUCCEED;
}

/**
  * @brief    Encode data of NEC protocol.
  * @param  data: user code and data code.
  * @param   command: data code.
  * @param   IR_DataStruct: pointer to struct which store NEC code.
  * @retval None
  */
IR_RETURN_Type IR_NECEncode(uint32_t freqency, uint8_t *data, IR_DataTypeDef * IR_DataStruct)
{
	IR_DataStruct->carrierFreq = freqency;
    	IR_DataStruct->codeLen = 4;
    	IR_DataStruct->code[0] = data[0];
    	IR_DataStruct->code[1] = data[1];
    	IR_DataStruct->code[2] = data[2];
    	IR_DataStruct->code[3] = data[3];

	return NEC_EncodePacket(IR_DataStruct, (IR_ProtocolTypeDef *)(&NEC_PROTOCOL));	
}

static IR_RETURN_Type LogicalCheck(uint32_t carrierFreq, uint32_t tolerancePrecent,
                                   IR_DataType *pActualValue, IR_DataType *pTheoreticalValue)
{
	uint32_t i = 0;
	IR_DataType actualTime = 0;
	IR_DataType theoreticalTime = 0;
    	IR_DataType toleranceValue = 0;

    	for (i = 0; i < MAX_LOG_WAVFORM_SIZE; i++)
    	{
       	 /* Whether the data type match */
        	if (CheckCarrierType(pActualValue[i]) != CheckCarrierType(pTheoreticalValue[i]))
        	{
            		return IR_DATA_TYPE_ERROR;
        	}

        	/* Checking tolerant rate */
        	actualTime = ConvertToTime(pActualValue[i], carrierFreq);
        	theoreticalTime = GetData(pTheoreticalValue[i]);
        	toleranceValue = (theoreticalTime * tolerancePrecent) / 100;
        	if (ABS_TIME(actualTime, theoreticalTime) > toleranceValue)
        	{
            		return IR_DATA_ERROR;
        	}
    	}

   	return IR_SUCCEED;
}

/**
  * @brief    Decode IR packet.
  * @param  IR_DataStruct: pointer to struct which store NEC code.
  * @param  IR_Protocol: pointer to specify IR protocol structure.
  * @retval None
  */
static IR_RETURN_Type NEC_DecodePacket(IR_DataTypeDef *IR_DataStruct,
                                       IR_ProtocolTypeDef *IR_Protocol)
{
	uint8_t index = 0;
	uint8_t bitPos = 0;
	uint8_t data = 0;
    	uint8_t byteNum = 0;
    	uint8_t header_index = 0;
    	IR_DataType actualTime = 0;
    	IR_DataType theoreticalTime = 0;
    	IR_DataType toleranceValue = 0;

    /* Error handle */
   // if (IR_DataStruct->carrierFreq != IR_Protocol->carrierFreq)
    //{
   //     return IR_FREQENCY_ERROR;
   // }
   
#ifdef INVERSE_DATA
    	InvsePulse(IR_DataStruct->irBuf, IR_DataStruct->bufLen);
#endif
	
    /* Decoding header */
    	for (index = 0; header_index < IR_Protocol->headerLen; index++)
    	{
        	if (IR_DataStruct->irBuf[index] > 0)
        	{
            		/* Whether the data type match */
            		if (CheckCarrierType(IR_DataStruct->irBuf[index]) != CheckCarrierType(
                    		IR_Protocol->headerBuf[header_index]))
            		{
            			//DBG_8195A("header type: %d \n", (IR_DataStruct->irBuf[index]) & PULSE_HIGH);
				//DBG_8195A("header value: %d \n", (IR_DataStruct->irBuf[index]) & IR_DATA_MSK);
                		return IR_HEARDE_TYPE_ERROR;							
            		}

            		/* Checking tolerant rate */
            		actualTime = ConvertToTime(IR_DataStruct->irBuf[index], IR_DataStruct->carrierFreq);
            		theoreticalTime = GetData(IR_Protocol->headerBuf[header_index]);
            		toleranceValue = (theoreticalTime * (IR_Protocol->tolerancePrecent)) / 100;
            		if (ABS_TIME(actualTime, theoreticalTime) > toleranceValue)
            		{
                		return IR_HEADER_DATA_ERROR;
            		}
            		header_index++;
        	}
    	}

    	/* Decoding address and command code */
    	while (index < IR_DataStruct->bufLen)
    	{
        	/* Check log1 */
        	if (IR_SUCCEED == LogicalCheck(IR_DataStruct->carrierFreq, \
                                       IR_Protocol->tolerancePrecent, \
                                       & (IR_DataStruct->irBuf[index]), IR_Protocol->log1Buf))
        	{
            		data |= 1 << bitPos;
            		index = index + MAX_LOG_WAVFORM_SIZE;
        	}
        	/* Check log0 */
        	else if (IR_SUCCEED == LogicalCheck(IR_DataStruct->carrierFreq, \
                                            IR_Protocol->tolerancePrecent, \
                                            & (IR_DataStruct->irBuf[index]), IR_Protocol->log0Buf))
        	{
            		data |= 0 << bitPos;
            		index += MAX_LOG_WAVFORM_SIZE;
        	}
        	else
        	{
            		return IR_DATA_ERROR;
        	}

        	bitPos++;
       	if (bitPos >= 8)
        	{
            		IR_DataStruct->code[byteNum++] = data;
            		bitPos = 0;
            		IR_DataStruct->codeLen = byteNum;
            		data = 0;
        	}

        	/* Finish decoding */
        	if (IR_DataStruct->codeLen >= 4)
        	{
            		break;
        	}

    	}

    	return IR_SUCCEED;
}

/**
  * @brief    Decode data of NEC protocol.
  * @param  data: user code and data code.
  * @param   IR_DataStruct: pointer to struct which store NEC code.
  * @retval None
  */
IR_RETURN_Type IR_NECDecode(uint32_t freqency, uint8_t *pdata, IR_DataTypeDef *IR_DataStruct)
{
    	IR_RETURN_Type result = IR_SUCCEED;

    	IR_DataStruct->carrierFreq = freqency;
	
    	result = NEC_DecodePacket(IR_DataStruct, (IR_ProtocolTypeDef *)(&NEC_PROTOCOL));
    	*pdata = IR_DataStruct->code[0];
	*(pdata + 1) = IR_DataStruct->code[1];
	*(pdata + 2) = IR_DataStruct->code[2];
	*(pdata + 3) = IR_DataStruct->code[3];
	
    	return result;
}

/** @} */ /* End of group IR_NEC_PROTOCOL */
/** @} */ /* End of group GPIO_OUTPUT_DEMO */

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

