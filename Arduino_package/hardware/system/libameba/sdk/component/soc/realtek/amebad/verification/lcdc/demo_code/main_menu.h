#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "rt_gui_demo_includes.h"
#include "start_menu.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define s32 signed int

#define CUS_MAIN_MENU_DEFAULT_BK_COLOR     GUI_WHITE
#define CUS_MAIN_MENU_DEFAULT_FR_COLOR     GUI_RED
#define CUS_MAIN_MENU_SEL_ICON_COLOR     GUI_DARKMAGENTA

#define CUS_MAIN_MENU_ICON_NUM    (5)

#define CUS_MAIN_EDGE_NUM    15

#define CUS_BTN_SELECT_RANGE_X	6
#define CUS_BTN_SELECT_RANGE_Y	6

#define CUS_BTN_SELECTED_ALPHA_FACTOR    	0X2F/*0~255*/
#define CUS_BTN_SELECTED_ALPHA_BACKCOLOR    RT_DEFAULT_COLOR/*0~255*/

enum CUS_BUTTON_STS {
	RT_CUS_BTN_SEL,
	RT_CUS_BTN_UNSEL,
};

enum CUS_TOUCH_STS_FLAG {
	RT_CUS_TOUCHED_RELEASE,
	RT_CUS_TOUCHED_UNRELEASE,
	RT_CUS_NOT_TOUCHED,
};

enum CUS_MAIN_TRANS_STS {
	RT_CUS_STARTMENU_PRO,	
	RT_CUS_MAINMENU_PRO,	
	RT_CUS_CLOCK_PRO,
	RT_CUS_LIGHTCTRL_PRO,
	
	RT_CUS_TEMPRE_PRO,
	RT_CUS_TEMPRE_WORK_PRO, 
	RT_CUS_TEMPRE_TEST_PRO, 

	RT_CUS_GRAD_PRO,
	RT_CUS_WAVE_PRO,
};

typedef struct MAIN_MENU_ICON_MEMDEV_STR{
		GUI_MEMDEV_Handle Hdl;
		s32 Id;
} MAIN_MENU_ICON_MEMDEV_DEF;

void InitMainArray(void);
void DrawCusMainBkWin(void);
void CusDispAllMainIcon(void);
void CusMainMenuStoreAllIcon(void);
void CusMainMenuFreeAllIcon(void);
void CusMainMenuSelMenuDraw(_BUTTON_CUS_Info * pButton);
void CusMainMenuUnSelMenuDraw(_BUTTON_CUS_Info * pButton);
void CusMainMenuRestoreAllMenu(void);
void CusMainMenuDispTargetIcon(_BUTTON_CUS_Info * pButton);
void MainMenuCusPro(RT_CUS_DEMO_STS_TYPE * Sts);
s32 MainMenuGetPressId(touch_pos_type * PosTemp);
_BUTTON_CUS_Info * MainMenuGetBtnFromId(s32 Id);
void CusGetTouchPos(GUI_PID_STATE * pSts, touch_pos_type * Pos);
s32 CusCheckTouchPressSts(GUI_PID_STATE * pSts);
void MainMenuSingleClickPro(_BUTTON_CUS_Info * pBtn);
void MainMenuStateTransfer(RT_CUS_DEMO_STS_TYPE * Sts);
void CusResetTouchPressSts(void);

#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
