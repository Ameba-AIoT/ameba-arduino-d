#ifndef __GT9147_H
#define __GT9147_H	
#include "ameba_soc.h"
#include "touch.h"
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
#define TP_I2C_USING_GPIO


#define GT_RST_PIN    		_PA_16 
#define GT_INT_PIN    		_PA_17

#define RGB_43_SCREEN_RST		_PA_18

#define GT9147_I2C_SCL_PIN			_PB_24
#define GT9147_I2C_SDA_PIN			_PB_25

#define 	GT_RST	GT9147_RST_OUT 
#define GT_INT      GPIO_ReadDataBit(GT9147_I2C_SDA_PIN) 
 
#define GT_CMD_WR 		0X28
#define GT_CMD_RD 		0X29
  
#define GT_CTRL_REG 	0X8040   	
#define GT_CFGS_REG 	0X8047   	
#define GT_CHECK_REG 	0X80FF   	
#define GT_PID_REG 		0X8140   	

#define GT_GSTID_REG 	0X814E   	
#define GT_TP1_REG 		0X8150  	
#define GT_TP2_REG 		0X8158		
#define GT_TP3_REG 		0X8160		
#define GT_TP4_REG 		0X8168		
#define GT_TP5_REG 		0X8170		  

typedef struct  
{										    
	u16 width;
	u16 height;
	u16 id;
	u8  dir;
	u16	wramcmd;
	u16 setxcmd;
	u16  setycmd;
	volatile u32 McuLcdBitMode;
}_lcd_dev_TP;

extern _lcd_dev_TP TP_LCD_Dev;

void GT9147_RST_OUT(level);
void GT9147_INT_OUT(level);
void GT9147_RST_OUT(level);
u8 GT9147_Send_Cfg(u8 mode);
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len); 
u8 GT9147_Init(void);
u8 GT9147_Scan(u8 mode); 

#endif













