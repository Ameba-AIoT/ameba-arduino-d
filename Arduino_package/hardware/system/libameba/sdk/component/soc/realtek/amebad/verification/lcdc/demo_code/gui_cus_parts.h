#ifndef GUI_CUS_PARTS_H
#define GUI_CUS_PARTS_H

#include "rt_gui_demo_includes.h"

#define CUS_DEF_BUTTON_HANDLE   u32

#define CUS_DEF_CIRCLE_BUTTON_HANDLE   u32

#define CUS_DEF_TEMP_HANDLE   u32

#define CUS_RT_WIN_HANDLE	u32

#define RT_CUS_CLOCK	0
#define RT_CUS_LIGHT	1
#define RT_CUS_TEMPRE	2
#define RT_CUS_GRAD		3
#define RT_CUS_WAVE		4
#define RT_CUS_MAIN_INVALID	5


#define RT_CUS_CIR_TEMPRE_WORK	6
#define RT_CUS_CIR_TEMPRE_TEST	7
#define RT_CUS_CIR_TEMPRE_RET	8
#define RT_CUS_CIR_TEMPRE_ICON	9

#define RT_CUS_GRAD_PLATE_ICON	10
#define RT_CUS_GRAD_RET	11

#define RT_CUS_LIGHT_SWITCH	12
#define RT_CUS_LIGHT_PALET	13
#define RT_CUS_LIGHT_BULB	14
#define RT_CUS_LIGHT_RED_BAR	15
#define RT_CUS_LIGHT_GREEN_BAR	16
#define RT_CUS_LIGHT_BLUE_BAR	17
#define RT_CUS_LIGHT_RET	18

#define RT_CUS_WAVE_RET	19

#define RT_CUS_CLOCK_RET	20

typedef struct BUTTON_CUS
{
	s32 xPos;
	s32 yPos;
	const GUI_BITMAP *pBitmap;
	const GUI_BITMAP *pUnSelBitmap;
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;
	s32 ID;
	s32 SelState;
	s32 ProState;
} _BUTTON_CUS_Info;

typedef struct BUTTON_CIRCLE_CUS
{
	s32 OrgxPos;
	s32 OrgyPos;
	s32 InnerR;
	s32 OuterR;
	const GUI_FONT* Fontp;
	u32 CtrColor;
	u32 EdgeColor;
	u32 FntColor;
	const GUI_BITMAP *pBitmap;
	u8 * StrInBtn;
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;
	s32 ID;
	s32 SelState;
	s32 ProState;
} BUTTON_CIRCLE_CUS_Info;

typedef struct TEMPRE_CUS_ICON
{
	s32 OrgX;
	s32 OrgY;
	s32	L_Dx;
	s32 G_Dx;
	s32 G_Color;
	s32 F_Dx;
	s32 Lin_Dx;
	s32 Lin_S_Dx;
	s32 Lin_L_Dx;
	s32 L_Color;
	s32 C_X;
	s32 C_Y;
	s32 C_R;
	s32 C_Color;
	s32 E_Dx;
	s32 S_R_Dx;
	s32 S_L_Dx;
	s32 S_D_Dx;
	s32 S_Len_Dx;
	s32 S_La_Color;
	s32 L_D_Dx;
	s32 H_Dx;
	s32 TemprMin;
	s32 TemprMax;
	const GUI_FONT* Fontp;
	u32 FntColor;
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;
	s32 CurTempre;
	s32 CurSetTempre;
	s32 WarningSts;
	s32 ID;
	s32 SelState;
	s32 ProState;
} TEMPRE_CUS_ICON_Info;

typedef struct CUS_GRAD_PLATE
{
	s32 X0;
	s32 Y0;
	s32 R0;
	u32 Color0;

	s32 X1;
	s32 Y1;
	s32 R1;
	u32 Color1;

	s32 RangeXmin;
	s32 RangeYmin;
	s32 RangeXmax;
	s32 RangeYmax;

	s32 BkColor;
	
	s32 CIntvl;
	s32 ClCnt;

	s32 CombineClr;
	s32 Angle;
	s32 OrgDx;
	s32 OrgMaxDx;

	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;
	s32 ID;
	s32 SelState;
	s32 ProState;
} CUS_GRAD_PLATE_Info;

typedef struct LIGHT_PLATE_CUS
{
	s32 OrgxPos;
	s32 OrgyPos;
	s32 XRange;
	s32 YRange;

	s32 XColor;
	s32 YColor;

	s32 SelColor;

	s32 FixColor;
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;
	s32 ID;
	s32 SelState;
	s32 ProState;
} LIGHT_PLATE_CUS_Info;

typedef struct CUS_COLOR_BAR_DEF 
{
	s32 ColorRange;

	s32 BarOrgX;
	s32 BarOrgY;
	s32 BarXSize;
	s32 BarYSize;
	s32 BarFillColor;

	s32 BtnX;
	s32 BtnY;
	s32 BtnRange;
	s32 BtnR;
	s32 BtnFillColor;
	
	s32 CurColorVal;
	s32 CurShowStrX;
	s32 CurShowStrY;
	const GUI_FONT * pFnt;

	
	/*touch range*/
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;

	/*ID, state*/
	s32 ID;
	s32 SelState;
	s32 ProState;	
} CUS_COLOR_BAR_DEF_INFO;

typedef struct CUS_Light_BULB_DISP_DEF 
{
	s32 OrgX;
	s32 OrgY;
	s32 R_D_Y;
	s32 R_D_X;
	s32 R_Color;
	
	s32 Ex;
	s32 Ey;
	s32 Edx;
	s32 Edy;
	s32 BulbCurFillColor;
	s32 BulbOutColor;

	/*touch range*/
	s32 TouchXmin;
	s32 TouchXmax;
	s32 TouchYmin;
	s32 TouchYmax;

	/*ID, state*/
	s32 ID;
	s32 SelState;
	s32 ProState;	
} CUS_Light_BULB_DISP_DEF_INFO;

extern unsigned int CusCreateButton(_BUTTON_CUS_Info * pButton);
extern unsigned int CusCreateCircleButton(BUTTON_CIRCLE_CUS_Info * pBtn);
#endif /* LCDCONF_H */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
