#ifndef TEMPRE_MENU_H
#define TEMPRE_MENU_H

#include "rt_gui_demo_includes.h"

#define CUS_TEMPRE_MENU_DEFAULT_BK_COLOR     GUI_WHITE
#define CUS_TEMPRE_MENU_DEFAULT_FR_COLOR     GUI_RED

#define CUS_TEMPRE_CIRCLE_WORK_BTN_X	(50)
#define CUS_TEMPRE_CIRCLE_WORK_BTN_Y	(LCD_GET_YSIZE()-80)
#define CUS_TEMPRE_CIRCLE_WORK_IN_R		(28)
#define CUS_TEMPRE_CIRCLE_WORK_OUT_R	(31)
#define CUS_TEMPRE_CIRCLE_WORK_FNT_TYP	(&GUI_Font8x16)//(&GUI_FontComic18B_1) //(&GUI_Font8x16) //
#define CUS_TEMPRE_CIRCLE_WORK_CEN_ENCOLR	GUI_BLUE //(0xCADAA9)
#define CUS_TEMPRE_CIRCLE_WORK_CEN_DISCOLR	GUI_GRAY//(0xCADAA9)
#define CUS_TEMPRE_CIRCLE_WORK_EDG_COLR	GUI_GREEN //(0x98D0DE)
#define CUS_TEMPRE_CIRCLE_WORK_FNT_COLR	(GUI_BLACK)

#define CUS_TEMPRE_CIRCLE_TEST_BTN_X	(LCD_GET_XSIZE()-100)
#define CUS_TEMPRE_CIRCLE_TEST_BTN_Y	(LCD_GET_YSIZE()-80)
#define CUS_TEMPRE_CIRCLE_TEST_IN_R		(28)
#define CUS_TEMPRE_CIRCLE_TEST_OUT_R	(31)
#define CUS_TEMPRE_CIRCLE_TEST_FNT_TYP	(&GUI_Font8x16) //(&GUI_FontComic18B_1) //(&GUI_Font8x16) //
#define CUS_TEMPRE_CIRCLE_TEST_CEN_ENCOLR	GUI_BLUE //(0xCADAA9)
#define CUS_TEMPRE_CIRCLE_TEST_CEN_DISCOLR	GUI_GRAY //(0xCADAA9)
#define CUS_TEMPRE_CIRCLE_TEST_EDG_COLR	GUI_GREEN //(0x98D0DE)
#define CUS_TEMPRE_CIRCLE_TEST_FNT_COLR	(GUI_BLACK)

#define TEMPRE_MENU_CIRCLE_BTN_NUM   2
#define TEMPRE_MENU_RET_BTN_NUM   1

#define CUS_TEMPRE_CIRCLE_WORK_STR_Y_OFFSET		10

#define CUS_TEMPRE_RET_BTN_OFFSET		5

#define CUS_TEMPRE_NOT_WARNING	0
#define CUS_TEMPRE_WARNING		1

void TempreMenuCusPro(RT_CUS_DEMO_STS_TYPE * Sts);
s32 TempreMenuGetPressId(touch_pos_type * PosTemp);
void * TempreMenuGetBtnFromId(s32 Id);

#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
