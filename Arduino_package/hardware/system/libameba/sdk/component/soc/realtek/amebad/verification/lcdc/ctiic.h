#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "ameba_soc.h"
#include "platform_stdlib.h"
#include "basic_types.h"
#include "diag.h"
#include "rand.h"
#include "section_config.h"
#include "ameba_soc.h"
#include "rtl8721d_lcdc_test.h"
#include "rtl8721d_lcdc.h"
#include "bsp_rgb_lcd.h"
#include "gt9147.h"
#include "touch.h"

#define delay_ms	DelayMs

#define delay_us	DelayUs

#define CT_SDA_IN   CT_SDA_IN_Func
#define CT_SDA_OUT CT_SDA_OUT_Func 
#define CT_IIC_SCL 	CT_IIC_SCL_PIN 
#define CT_IIC_SDA CT_IIC_SDA_PIN  
#define CT_READ_SDA   GPIO_ReadDataBit(GT9147_I2C_SDA_PIN)
 
void CT_SDA_IN_Func(void);

void CT_SDA_OUT_Func(void);

void CT_IIC_SCL_PIN(int level);

void CT_IIC_SDA_PIN(int level);

void CT_IIC_Init(void);			 
void CT_IIC_Start(void);
void CT_IIC_Stop(void);
void CT_IIC_Send_Byte(u8 txd);
u8 CT_IIC_Read_Byte(unsigned char ack);
u8 CT_IIC_Wait_Ack(void); 
void CT_IIC_Ack(void);	
void CT_IIC_NAck(void);

#endif







