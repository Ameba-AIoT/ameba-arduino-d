/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#ifndef	__ODM_PRECOMP_H__
#define __ODM_PRECOMP_H__

#include "phydm_types.h"

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
#include "Precomp.h"		// We need to include mp_precomp.h due to batch file setting.

#else

#define		TEST_FALG___		1

#endif

//2 Config Flags and Structs - defined by each ODM Type

#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
	#include "../8192cd_cfg.h"
	#include "../odm_inc.h"

	#include "../8192cd.h"
	#include "../8192cd_util.h"
	#ifdef _BIG_ENDIAN_
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_BIG
	#else
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_LITTLE
	#endif

	#ifdef AP_BUILD_WORKAROUND
	#include "../8192cd_headers.h"
	#include "../8192cd_debug.h"		
	#endif
	
#elif (DM_ODM_SUPPORT_TYPE == ODM_ADSL)
	// Flags
	#include "../8192cd_cfg.h"		// OUTSRC needs ADSL config flags.
	#include "../odm_inc.h"			// OUTSRC needs some extra flags.
	// Data Structure
	#include "../common_types.h"	// OUTSRC and rtl8192cd both needs basic type such as UINT8 and BIT0.
	#include "../8192cd.h"			// OUTSRC needs basic ADSL struct definition.
	#include "../8192cd_util.h"		// OUTSRC needs basic I/O function.
	#ifdef _BIG_ENDIAN_
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_BIG
	#else
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_LITTLE
	#endif

	#ifdef ADSL_AP_BUILD_WORKAROUND
	// NESTED_INC: Functions defined outside should not be included!! Marked by Annie, 2011-10-14.
	#include "../8192cd_headers.h"
	#include "../8192cd_debug.h"	
	#endif	
	
#elif (DM_ODM_SUPPORT_TYPE ==ODM_CE)
	#define BEAMFORMING_SUPPORT 0
#elif (DM_ODM_SUPPORT_TYPE ==ODM_IOT)
	#include "rom_rtw_message.h"
	#define BEAMFORMING_SUPPORT 0
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_LITTLE
#elif (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	#include "mp_precomp.h"
	#define	ODM_ENDIAN_TYPE				ODM_ENDIAN_LITTLE
#endif


//2 Hardware Parameter Files


#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
#if (RTL8192C_SUPPORT==1)
	#include "rtl8192c/Hal8192CEFWImg_AP.h"
	#include "rtl8192c/Hal8192CEPHYImg_AP.h"
	#include "rtl8192c/Hal8192CEMACImg_AP.h"
#endif
#elif (DM_ODM_SUPPORT_TYPE == ODM_ADSL)
	#include "rtl8192c/Hal8192CEFWImg_ADSL.h"
	#include "rtl8192c/Hal8192CEPHYImg_ADSL.h"
	#include "rtl8192c/Hal8192CEMACImg_ADSL.h"

#elif (DM_ODM_SUPPORT_TYPE == ODM_CE)
	#if(RTL8192CE_SUPPORT ==1)
	#include "rtl8192c/Hal8192CEFWImg_CE.h"
	#include "rtl8192c/Hal8192CEPHYImg_CE.h"
	#include "rtl8192c/Hal8192CEMACImg_CE.h"
	#endif
	
	#if(RTL8192CU_SUPPORT ==1)
	#include "rtl8192c/Hal8192CUFWImg_CE.h"
	#include "rtl8192c/Hal8192CUPHYImg_CE.h"
	#include "rtl8192c/Hal8192CUMACImg_CE.h"
	#endif
	
	#if(RTL8192DE_SUPPORT ==1)
	#include "rtl8192d/Hal8192DEFWImg_CE.h"
	#include "rtl8192d/Hal8192DEPHYImg_CE.h"
	#include "rtl8192d/Hal8192DEMACImg_CE.h"	
	#endif
	
	#if(RTL8192DU_SUPPORT ==1)
	#include "rtl8192d/Hal8192DUFWImg_CE.h"
	#include "rtl8192d/Hal8192DUPHYImg_CE.h"
	#include "rtl8192d/Hal8192DUMACImg_CE.h"
	#endif
	
	#if(RTL8723AS_SUPPORT==1)
	#include "rtl8723a/Hal8723SHWImg_CE.h"
	#endif
	
	#if(RTL8723AU_SUPPORT==1)
	#include "rtl8723a/Hal8723UHWImg_CE.h"
	#endif

#elif (DM_ODM_SUPPORT_TYPE == ODM_WIN)

#endif


//2 OutSrc Header Files
#if (RTL8188E_SUPPORT==1)
// Old ODM 
#include "rtl8188e\odm.h"
#include "rtl8188e\odm_HWConfig.h"
#include "rtl8188e\odm_debug.h"
#include "rtl8188e\odm_RegDefine11AC.h"
#include "rtl8188e\odm_RegDefine11N.h"
#include "rtl8188e\odm_interface.h"
#include "rtl8188e\odm_reg.h"
#include "rtl8188e\Hal8188EAdaptivity.h"
#else
// new ODM
#include "phydm.h"
#include "phydm_HWConfig.h"
#include "phydm_debug.h"
#include "phydm_RegDefine11AC.h"
#include "phydm_RegDefine11N.h"
#include "phydm_AntDiv.h"
#include "phydm_EdcaTurboCheck.h"
#include "phydm_DIG.h"
#include "PhyDM_Adaptivity.h"
#include "phydm_PathDiv.h"
#include "phydm_RaInfo.h"
#include "phydm_DynamicBBPowerSaving.h"
#include "phydm_DynamicTxPower.h"
#include "phydm_CfoTracking.h"
#include "phydm_NoiseMonitor.h"
#include "phydm_interface.h"
#include "phydm_reg.h"
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
#if (RTL8192C_SUPPORT==1)
	#include "rtl8192c/HalDMOutSrc8192C_AP.h"
#endif
#if (RTL8188E_SUPPORT==1)
		#include "rtl8188e/Hal8188ERateAdaptive.h"//for  RA,Power training
#endif

#elif (DM_ODM_SUPPORT_TYPE == ODM_ADSL)
	#include "rtl8192c/HalDMOutSrc8192C_ADSL.h"

#elif (DM_ODM_SUPPORT_TYPE == ODM_CE)
	//#include "hal_com.h"
	#include "HalPhyRf.h"
	#if (RTL8192C_SUPPORT==1) 
		#ifdef CONFIG_INTEL_PROXIM
		#include "../proxim/intel_proxim.h"	
		#endif	
		#include  "rtl8192c/HalDMOutSrc8192C_CE.h"
		#include <rtl8192c_hal.h>
	#endif
	
	#if (RTL8192D_SUPPORT==1)
		#include  "rtl8192d/HalDMOutSrc8192D_CE.h"
		#include  "rtl8192d_hal.h"
	#endif
	
	#if (RTL8723A_SUPPORT==1)
		#include "rtl8192c/HalDMOutSrc8192C_CE.h" //for IQK,LCK,Power-tracking
		#include "../rtl8723a/rtl8723a_hal.h"  
	#endif
	
	#if (RTL8188E_SUPPORT==1)
		#include "rtl8188e/HalPhyRf_8188e.h"//for IQK,LCK,Power-tracking
		#include "rtl8188e/Hal8188ERateAdaptive.h"//for  RA,Power training
		#include "../rtl8188e/rtl8188e_hal.h"  	
	#endif

	#if (RTL8192E_SUPPORT==1)
		#include "rtl8192e/HalPhyRf_8192e.h"//for IQK,LCK,Power-tracking		
		#include "rtl8192e_hal.h"  			
	#endif

	#if (RTL8812A_SUPPORT==1)
		#include "rtl8812a/HalPhyRf_8812A.h"//for IQK,LCK,Power-tracking
		#include "rtl8812a_hal.h"
	#endif

	#if (RTL8821A_SUPPORT==1)
		#include "rtl8821a/HalPhyRf_8821A.h"//for IQK,LCK,Power-tracking
		#include "rtl8812a/HalPhyRf_8812A.h"//for IQK,LCK,Power-tracking
		#include "rtl8812a_hal.h"		
	#endif

	#if (RTL8723B_SUPPORT==1)
		#include "rtl8723b/HalPhyRf_8723B.h"//for IQK,LCK,Power-tracking
		#include "rtl8723b_hal.h"
	#endif

#elif (DM_ODM_SUPPORT_TYPE == ODM_IOT)
	#include "HalPhyRf.h"
	#if (RTL8195A_SUPPORT==1)
		#include "rtl8195a/HalPhyRf_8195A.h"//for IQK,LCK,Power-tracking
		#include "rtl8195a_hal.h"
	#endif

	#if (RTL8711B_SUPPORT==1)
        #include "rtl8711b/HalPhyRf_8711b.h"//for IQK,LCK,Power-tracking
        #include "rtl8711b_hal.h"
   	 #endif
	 
	#if (RTL8721D_SUPPORT==1)
        #include "rtl8721d/HalPhyRf_8721d.h"//for IQK,LCK,Power-tracking
        #include "rtl8721d_hal.h"
   	 #endif
#endif


#if (RTL8192C_SUPPORT==1) 
#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
#include "rtl8192c/Hal8192CHWImg_MAC.h"
#include "rtl8192c/Hal8192CHWImg_RF.h"
#include "rtl8192c/Hal8192CHWImg_BB.h"
#include "rtl8192c/Hal8192CHWImg_FW.h"
#endif
#include "rtl8192c/phydm_RTL8192C.h"
#endif
#if (RTL8192D_SUPPORT==1) 
#include "rtl8192d/phydm_RTL8192D.h"
#endif

#if (RTL8723A_SUPPORT==1) 
#include "rtl8723a/HalHWImg8723A_MAC.h"
#include "rtl8723a/HalHWImg8723A_RF.h"
#include "rtl8723a/HalHWImg8723A_BB.h"
#include "rtl8723a/HalHWImg8723A_FW.h"
#include "rtl8723a/phydm_RegConfig8723A.h"
#endif

#if (RTL8188E_SUPPORT==1) 
#include "rtl8188e/HalHWImg8188E_MAC.h"
#include "rtl8188e/HalHWImg8188E_RF.h"
#include "rtl8188e/HalHWImg8188E_BB.h"
#include "rtl8188e/HalHWImg8188E_FW.h"
#include "rtl8188e/Hal8188EReg.h"

#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
#include "rtl8188e/HalPhyRf_8188e.h"
#endif

#if (TEST_CHIP_SUPPORT == 1) 
#include "rtl8188e/HalHWImg8188E_TestChip_MAC.h"
#include "rtl8188e/HalHWImg8188E_TestChip_RF.h"
#include "rtl8188e/HalHWImg8188E_TestChip_BB.h"
#endif


#include "rtl8188e/odm_RegConfig8188E.h"
#include "rtl8188e/odm_RTL8188E.h"
#endif

#if (RTL8192E_SUPPORT==1) 
#include "rtl8192e/HalHWImg8192E_MAC.h"
#include "rtl8192e/HalHWImg8192E_RF.h"
#include "rtl8192e/HalHWImg8192E_BB.h"
#include "rtl8192e/HalHWImg8192E_FW.h"
#include "rtl8192e/Hal8192EReg.h"
#include "rtl8192e/phydm_RegConfig8192E.h"
#include "rtl8192e/phydm_RTL8192E.h"
#endif

#if (RTL8723B_SUPPORT==1) 
#include "rtl8723b/HalHWImg8723B_MAC.h"
#include "rtl8723b/HalHWImg8723B_RF.h"
#include "rtl8723b/HalHWImg8723B_BB.h"
#include "rtl8723b/HalHWImg8723B_FW.h"
#include "rtl8723b/HalHWImg8723B_MP.h"
#include "rtl8723b/Hal8723BReg.h"
#include "rtl8723b/phydm_RTL8723B.h"
#include "rtl8723b/phydm_RegConfig8723B.h"
#endif

#if (RTL8812A_SUPPORT==1) 
#include "rtl8812a/HalHWImg8812A_MAC.h"
#include "rtl8812a/HalHWImg8812A_RF.h"
#include "rtl8812a/HalHWImg8812A_BB.h"
#include "rtl8812a/HalHWImg8812A_FW.h"
#include "rtl8812a/phydm_RegConfig8812A.h"
#include "rtl8812a/phydm_RTL8812A.h"
#endif


#if (RTL8821A_SUPPORT==1) 
#include "rtl8821a/HalHWImg8821A_MAC.h"
#include "rtl8821a/HalHWImg8821A_RF.h"
#include "rtl8821a/HalHWImg8821A_BB.h"
#include "rtl8821a/HalHWImg8821A_FW.h"
#include "rtl8821a/phydm_RegConfig8821A.h"
#include "rtl8821a/phydm_RTL8821A.h"
#endif

#if (RTL8195A_SUPPORT==1) 
#include "rtl8195a/halhwimg8195a_mac.h"
#include "rtl8195a/halhwimg8195a_rf.h"
#include "rtl8195a/halhwimg8195a_bb.h"
#include "rtl8195a/Hal8195AReg.h"
#include "rtl8195a/phydm_RTL8195A.h"
#include "rtl8195a/phydm_RegConfig8195A.h"
#include "rtl8195a/ROM_RTL8195A_PHYDM.h"
#include "rtl8195a/Hal8195ARateAdaptive.h"
#include "rtl8195a/version_rtl8195a.h"
#endif

#if (RTL8711B_SUPPORT==1) 
#include "rtl8711b/HalHWImg8711B_MAC.h"
#include "rtl8711b/HalHWImg8711B_RF.h"
#include "rtl8711b/HalHWImg8711B_BB.h"
#include "rtl8711b/HalHWImg8711B_FW.h"
#include "rtl8711b/HalHWImg8711B_MP.h"
#include "rtl8711b/Hal8711BReg.h"
#include "rtl8711b/phydm_RTL8711B.h"
#include "rtl8711b/phydm_RegConfig8711B.h"
#include "rtl8711b/ROM_RTL8711B_PHYDM.h"
#endif

#if (RTL8721D_SUPPORT==1) 
#include "rtl8721d/HalHWImg8721D_MAC.h"
#include "rtl8721d/HalHWImg8721D_RF.h"
#include "rtl8721d/HalHWImg8721D_BB.h"
#include "rtl8721d/HalHWImg8721D_FW.h"
#include "rtl8721d/HalHWImg8721D_MP.h"
#include "rtl8721d/Hal8721DReg.h"
//#include "rtl8721d/phydm_RTL8721D.h"
#include "rtl8721d/phydm_RegConfig8721D.h"
#include "rtl8721d/ROM_RTL8721D_PHYDM.h"
#endif

#if (RTL8821C_SUPPORT==1)
#include "rtl8821c/phydm_hal_api8821c.h"
#include "rtl8821c/halhwimg8821c_mac.h"
#include "rtl8821c/halhwimg8821c_rf.h"
#include "rtl8821c/halhwimg8821c_bb.h"
#include "rtl8821c/halhwimg8821c_fw.h"
#include "rtl8821c/phydm_regconfig8821c.h"
#include "rtl8821c/halphyrf_8821c.h"
#include "rtl8821c/rtl8821c_hal.h"
#endif

#endif	// __ODM_PRECOMP_H__

