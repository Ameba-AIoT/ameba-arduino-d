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
#ifndef __TMP75_H__
#define __TMP75_H__

#include <stdint.h>

#define SHUTDOWN_MODE_OFF  0x00
#define SHUTDOWN_MODE_ON   0x01
#define COMPARATOR_MODE    0x00
#define INTERRUPT_MODE     0x02
#define POLARITY_0         0x00
#define POLARITY_1         0x04
#define FAULT_QUEUE_1      0x00
#define FAULT_QUEUE_2      0x08
#define FAULT_QUEUE_4      0x10
#define FAULT_QUEUE_6      0x18
#define RESOLUTION_9       0x00
#define RESOLUTION_10      0x20
#define RESOLUTION_11      0x40
#define RESOLUTION_12      0x60
#define ONE_SHOT           0x80

void	tmp75_set_temperature_low(float value);
float	tmp75_get_temperature_low(void);
void	tmp75_set_temperature_high(float value);
float	tmp75_get_temperature_high(void);
float	tmp75_get_temperature(void);
void	tmp75_set_config(uint8_t config);
uint8_t	tmp75_get_config(void);
void	tmp75_start_single_conversion(void);
void	tmp75_init(void);

#endif //__TMP75_H__
