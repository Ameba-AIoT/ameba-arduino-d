/** mbed Microcontroller Library
  ******************************************************************************
  * @file    efuse_api.c
  * @author 
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for EFUSE.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */
#include "ameba_soc.h"
#include "rom_aes.h"
#include "osdep_service.h"
//#ifdef CONFIG_EFUSE_EN

/** @addtogroup AmebaD_Mbed_API 
  * @{
  */
  
/** @defgroup MBED_EFUSE
 *  @brief      MBED_EFUSE driver modules.
 *  @{
 */

/** @defgroup MBED_EFUSE_Exported_Functions MBED_EFUSE Exported Functions
  * @{
  */

/**
  * @brief  Get remaining efuse length 
  * @retval remaining efuse length 
  */
int  efuse_get_remaining_length(void)
{
	return EFUSE_RemainLength();
}


/**
  * @brief  Read efuse content of specified user 
  * @param  data: Specified the address to save the readback data.
  * @retval none  
  * @note read user MTP(0x160~0x17f).
  */
void  efuse_mtp_read(uint8_t * data)
{
	u8 EfuseBuf[1024];
	u8 ret;
	
	/*0xff will be efuse default value instead of 0x00. */
	_memset(data, 0xFF, 32);

	ret = EFUSE_LMAP_READ(EfuseBuf);
	
	if (ret == _FAIL) {
		DBG_8195A("EFUSE_LogicalMap_Read fail \n");
	}
	
	_memcpy(data, EfuseBuf+0x160, 32);

}

/**
  * @brief  Write user's content to efuse
  * @param  data: Specified the data to be programmed.
  * @param  len: Specifies the data length of programmed data.
  * @retval status value:
  *          - 1~32: Success
  *          - 0 or -1: Failure
  * @note read user MTP(0x160~0x17f).
  */
int  efuse_mtp_write(uint8_t *data, uint8_t len)
{
	u32 bResult;

	if(len > 32)	{
		DBG_8195A("string length should be smaller than 32\n");
		return -1;
	}

	
	bResult = EFUSE_LMAP_WRITE(USER_SECTION << 3, len, data);
	if(!bResult){
		DBG_8195A("write fail \n");
		return -1;
	}else{
   		return len;
	}
}


/**
  * @brief  Read efuse OTP contant 
  * @param  address: Specifies the offset of the OTP.
  * @param  len: Specifies the length of readback data.
  * @param  buf: Specified the address to save the readback data.
  * @retval status value:
  *          - 0: Success
  *          - -1: Failure   
  * @note read user OTP(0x130~0x14f).
  */
int efuse_otp_read(u8 address, u8 len, u8 *buf)
{	
	u8 content[32];	// the OTP max length is 32
	u8 index;
	u32 bResult;
	if((address+len) > 32) {
		return -1;
	}
	for (index = 0; index< 32; index++) {
		bResult = EFUSE_PMAP_READ8(0, 0x130 + index , content + index, L25EOUTVOLTAGE);
		if(!bResult){
			return -1;
		}
	}
	
	_memcpy(buf, content+address, len);
	return 0;

}


/**
  * @brief  Write user's contant to OTP efuse
  * @param  address: Specifies the offset of the programmed OTP.
  * @param  len: Specifies the data length of programmed data.
  * @param  buf: Specified the data to be programmed.
  * @retval status value:
  *          - 0: Success
  *          - -1: Failure   
  * @note read user OTP(0x130~0x14f).
  */
int efuse_otp_write(u8 address, u8 len, u8 *buf)
{
	u8 index;
	u32 bResult;
	
	if((address+len) > 32) {
		return -1;
	}
		
	for (index = 0; index < len; index++) {
		bResult = EFUSE_PMAP_WRITE8(0, address + 0x130 +index, buf[index], L25EOUTVOLTAGE);
		if(!bResult){
			return -1;
		}
	}
	return 0;
}

/**
  * @brief  check user's contant to OTP efuse
  * @param  *buf: Specified the data to be programmed.
  * @param len: Specifies the data length of programmed data.
  * @retval   status: Success:0 or Failure: -1.
  */
int efuse_otp_chk(u8 len, u8 *buf)
{
	/* To avoid gcc warnings */
	( void ) len;
	( void ) buf;
	DBG_8195A("Ameba-D not support efuse_otp_chk function!\n");
	return 0;
}

/** 
  * @brief  read logical efuse
  * @param  addr: Specified the address to be read.
  * @param  size: Specifies the data length to be read.
  * @param  *pbuf: Pointer to the read out data buffer.
  * @retval   status: Success:0 or Failure: -1.
  */
int efuse_logical_read(u16 addr, u16 size, u8 *pbuf)
{
    if(addr + size > EFUSE_MAP_LEN_8711B)
        return -1;

    int ret = -1;
    u8 *tmp_buf = NULL;

    tmp_buf = (u8 *)rtw_malloc(EFUSE_MAP_LEN_8711B);
    if(NULL == tmp_buf){
        DBG_8195A("memory error!\n");
    }
    else{
        _memset(tmp_buf, 0, EFUSE_MAP_LEN_8711B);
        
        if(1 == EFUSE_LogicalMap_Read(tmp_buf)){
            for(u16 i = 0; i < size; i++)
                pbuf[i] = tmp_buf[i+addr];
            ret =  0;
        }

        rtw_mfree(tmp_buf, EFUSE_MAP_LEN_8711B);
    }
    
    return ret;
}

/** 
  * @brief  write logical efuse
  * @param  addr: Specified the address to be write.
  * @param  size: Specifies the data length to be write.
  * @param  *pbuf: Pointer to the write data buffer.
  * @retval   status: Success:0 or Failure: -1.
  */

int efuse_logical_write(u16 addr, u16 size, u8 *data)
{
    if(addr + size > EFUSE_MAP_LEN_8711B)
        return -1;
    
    if(1 == EFUSE_LMAP_WRITE((u32)addr, (u32)size, data)){
        return 0;
    }

    return -1;
}

/** 
  * @}
  */
//#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
