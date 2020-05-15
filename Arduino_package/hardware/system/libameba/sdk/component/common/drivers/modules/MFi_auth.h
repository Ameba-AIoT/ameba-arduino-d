//==========================================================================
//
//      tmp75.h
//
//      tmp75/175 temperature sensor driver using i2c for STM32 CortexM processors
//                                 
//==========================================================================
// #####DESCRIPTIONBEGIN####
//
// Author(s):    Cloud Tseng
// Contributors:
// Date:         2013-11-07
// Description:  Temperature sensor driver using i2c for STM32 CortexM processors
//              
// ####DESCRIPTIONEND####
//
//==========================================================================
#ifndef __MFI_I2C_H__
#define __MFI_I2C_H__

#include <stdint.h>

#define kMFiAuthReg_ProtocolMajorVersion		0x02
#define kMFiAuthReg_ErrorCode					0x05
#define kMFiAuthReg_AuthControlStatus			0x10
	#define kMFiAuthErr_Flag						0x80
	#define kMFiAuthControl_GenerateSignature		0x01
#define kMFiAuthReg_SignatureSize				0x11
#define kMFiAuthReg_SignatureData				0x12
#define kMFiAuthReg_ChallengeSize				0x20
#define kMFiAuthReg_ChallengeData				0x21
#define kMFiAuthReg_DeviceCertificateSize		0x30
#define kMFiAuthReg_DeviceCertificateData1		0x31 // Note: auto-increments so next read is Data2, Data3, etc.

uint16_t	MFi_i2c_tx(uint8_t reg, const uint8_t* buf, uint16_t count, uint8_t retry);
uint16_t	MFi_i2c_rx(uint8_t reg, uint8_t* buf, uint16_t count, uint8_t retry);
int			MFi_auth_io(uint8_t reg, const uint8_t *write_buf, uint16_t write_count, uint8_t *read_buf, uint16_t read_count, uint8_t retry_count);
void		MFi_auth_init(void);
void		MFi_auth_deinit(void);

#endif //__MFI_I2C_H__
