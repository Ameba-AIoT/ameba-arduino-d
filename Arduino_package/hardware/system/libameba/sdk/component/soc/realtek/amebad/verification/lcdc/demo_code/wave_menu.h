#ifndef WAVE_MENU_H
#define WAVE_MENU_H

#include "rt_gui_demo_includes.h"
#include "start_menu.h"
#include "realtek_demo.h"


#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define s32 signed int

#define CUS_WAVE_RET_BTN_OFFSET     CUS_GRAD_RET_BTN_OFFSET


extern _BUTTON_CUS_Info WaveMenuVar;

void * WaveMenuGetBtnFromId(s32 Id);
void WaveMenuCusPro(RT_CUS_DEMO_STS_TYPE * Sts);

#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
