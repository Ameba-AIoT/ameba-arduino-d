#ifndef START_MENU_H
#define START_MENU_H

#include "rt_gui_demo_includes.h"

enum MENU_STATE {

	MENULOGO_RT,
	MENULOGO_RT_SCAN,
	MENU_2,
	MENU_2_LAYER_1,
	MENU_2_LAYER_2,
};

enum MENU_TOUCH_STATE {

	MENU_TOUCH_NULL = 0,
	MENU_LOGO_TRG,
	MENU_LOGO_RLEASE,
};

typedef struct MENU_RANE_DEF{
		int x0;
		int y0;
		int x1;
		int y1;
} menu_range;

typedef struct TOUCH_POS_DEF{
		int x;
		int y;
} touch_pos_type;

enum MENU_TOUCH_RANGE_STATE {

	TOUCH_POS_OUT = 1,
	TOUCH_POS_IN,
};
void ShowMainMenuRT(void);
void ShowMainMenuSTR(void);
void RtMenuLogoScan(void);


extern void RtCheckTouchPos(touch_pos_type * pos);
extern void StartMenuFunc(RT_CUS_DEMO_STS_TYPE * Sts) ;

void ShowStartMenuStr(void);
void ShowMainMenuStr(void);
void ShowClockMenuStr(void);
void ShowLightMenuStr(void);
void ShowGradMenuStr(void);
void ShowWaveMenuStr(void);

#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
