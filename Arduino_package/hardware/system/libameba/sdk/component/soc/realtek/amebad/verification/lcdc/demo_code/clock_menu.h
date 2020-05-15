#ifndef CLOCK_MENU_H
#define CLOCK_MENU_H

#include "rt_gui_demo_includes.h"
#include "start_menu.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define s32 signed int

#define uint8_t unsigned char
#define uint16_t unsigned short
#define u8 unsigned char
#define u16 unsigned short

#define PI		3.14159

#define CLOCK_BK_COLOR		GUI_BLACK 
#define CLOCK_COLOR			GUI_YELLOW

#define CLOCK_HOUR_COLOR	GUI_RED
#define CLOCK_MIN_COLOR		GUI_GREEN
#define CLOCK_SEC_COLOR		GUI_YELLOW

#define CLOCK_CENTER_X		135
#define CLOCK_CENTER_Y		170 
#define CLOCK_RADIUS		 90
#define CLOCK_RADIUS_W		  4 
#define CLOCK_RADIUS_M		  3 

#define SCALL_OFFSET_B		  6 
#define SCALL_OFFSET_S		  4 
#define NUMBER_OFFSET		 14 

#define SEC_PTR_OFFSET       25 
#define MIN_PTR_OFFSET		 35 
#define HOUR_PTR_OFFSET		 50 

#define CUS_CLOCK_RET_BTN_OFFSET	CUS_GRAD_RET_BTN_OFFSET


typedef struct 
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;			
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_date;
	uint8_t  week;		 
} tm_cus;

extern tm_cus timer_rt;

void DrawClockPointer(uint8_t hour, uint8_t min, uint8_t sec); 
void ClearClockPointer(uint8_t hour, uint8_t min, uint8_t sec); 
void DrawClockBorder(void);
void ClockShowTime(uint16_t x, uint16_t y);


extern const GUI_BITMAP bmMicriumLogo;
extern const GUI_BITMAP bmMicriumLogo_1bpp;


#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
