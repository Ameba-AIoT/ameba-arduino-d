#ifndef __BSP_LED_BOARD_H
#define __BSP_LED_BOARD_H		
#include "sys.h"	 
#include "stdlib.h"


#include "ameba_soc.h"
#include "rtl8721d_lcdc.h"
#include "bsp_mcu_lcd.h"
#include "bsp_lcdc_font.h"

#define LED_FRM_BUF_WIDTH		32 //64 //(128+2)
#define LED_FRM_BUF_HEIGHT		32 //64

#define LED_DISP_1CO1CH_IMG_WIDTH        	64
#define LED_DISP_1CO1CH_IMG_HEIGHT       	16
#define LED_DISP_1CO1CH_IMG_WIDTH_BYTE        	(LED_DISP_1CO1CH_IMG_WIDTH/8)

#define LED_DISP_1CO2CH_IMG_WIDTH        64
#define LED_DISP_1CO2CH_IMG_HEIGHT       32
#define LED_DISP_1CO2CH_IMG_WIDTH_BYTE        	(LED_DISP_1CO2CH_IMG_WIDTH/8)

#define LED_HORIZONTAL_SCREEN    	(0)
#define LED_VERTICAL_SCREEN    	(1)

#define LED_DEV_DISABLE			(0)
#define LED_DEV_ENABLE			(1)

#define LED_DISP_ON             	(0)
#define LED_DISP_OFF               	(1)

#define LED_WHITE			0//(0)
#define LED_BLACK			1//(1)

#define LED_DEV_1CO1CH		(0)
#define LED_DEV_1CO2CH		(1)

typedef struct LED_DEV
{
	volatile u32 pWidth;
	volatile u32 pHeight;
	volatile u8 Dir;
	volatile u32 Width;
	volatile u32 Height;
	volatile u32 ScanMode;
	volatile u8 * LedFrameBuf;

	volatile u32 ScanFreq;
	volatile u32 OEActWd;
		
} _Led_Dev_Info;

extern _Led_Dev_Info LedBdDev;


void LedDevDisplaySwitch(u32 State);
void LedDevInit(void);
void LedDevClear(void);
void LedDevDrawPoint(s16 x, s16 y);
void LedDevDrawPointColor(s16 x, s16 y, u16 color);
u16 LedDevReadPoint(s16 x, s16 y);
void LedDevDrawCircle(s16 x0, s16 y0, u16 r);
void LedDevDrawLine(s16 x1, s16 y1, s16 x2, s16 y2);
void LedDevDrawRectangle(s16 x1, s16 y1, s16 x2, s16 y2);
void LedDevFill(s16 sx,s16 sy,s16 ex,s16 ey);
void LedDevColorFill(s16 sx,s16 sy,s16 ex,s16 ey,u8 *color);
void LedDevShowChar(s16 x,s16 y,u8 num,u8 size,u8 mode);
void LedDevShowNum(s16 x,s16 y,u32 num,u8 len,u8 size);
void LedDevShowxNum(s16 x,s16 y,u32 num,u8 len,u8 size,u8 mode);
void LedDevShowString(s16 x,s16 y,s16 width,s16 height,u8 size,u8 *p);
void LedDevDisplayDir(u8 dir);
void LedDevSetColor(u16 color);
void LedDevSetBkColor(u16 color);
void LedDevShowChsChar(s16 x,s16 y,u8 * pChar,u8 size,u8 mode);
void LedDevShowChsString(s16 x,s16 y,u8 **pStr, u16 len, u8 size,u8 mode);

#endif













