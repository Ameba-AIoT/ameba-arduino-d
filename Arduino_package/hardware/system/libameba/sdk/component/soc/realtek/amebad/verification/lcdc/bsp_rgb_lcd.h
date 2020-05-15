#ifndef __BSP_RGB_LCD_H
#define __BSP_RGB_LCD_H		
#include "sys.h"	 
#include "stdlib.h"

#include "ameba_soc.h"
#include "rtl8721d_lcdc.h"
#include "bsp_mcu_lcd.h"
#include "bsp_lcdc_font.h"

#ifdef LCDC_FPGA_VERIFY

#define RGB_LCD_WIDTH    (60)   //480
#define RGB_LCD_HEIGHT   (32)   //(34)

#else

#define RGB_LCD_WIDTH    (480)
#define RGB_LCD_HEIGHT   (272)

#endif

#define HORIZONTAL_SCREEN    	(0)

#define VERTICAL_SCREEN    	(1)

#define RGB_LCD_CLOSE             (0)
#define RGB_LCD_OPEN               (1)

typedef struct RGB_DEV
{
	u32 pWidth;
	u32 pHeight;
	u32 Hsw;
	u32 Hbp;
	u32 Hfp;
	u32 Vsw;
	u32 Vbp;
	u32 Vfp;
	u8 Dir;
	u32 Width;
	u32 Height;
	u32 PixSize;
	u16 * LcdFrameBuf;
} _Rgb_Dev_Info;

extern _Rgb_Dev_Info RgbLcdDev;

void RgbLcdDisplaySwitch(u32 State);
void RgbLcdInit(void);
void RgbLcdClear(void);
void RgbLcdDrawPoint(u16 x, u16 y);
void RgbLcdDrawPointColor(u16 x, u16 y, u16 color);
u16 RgbLcdReadPoint(u16 x, u16 y);
void RgbLcdDrawCircle(u16 x0, u16 y0, u16 r);
void RgbLcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void RgbLcdDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void RgbLcdFill(u16 sx,u16 sy,u16 ex,u16 ey);
void RgbLcdColorFill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void RgbLcdShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void RgbLcdShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void RgbLcdShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
void RgbLcdShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void RgbLcdDisplayDir(u8 dir);
void RgbLcdSetColor(u16 color);
void RgbLcdSetBkColor(u16 color);
	
#endif













