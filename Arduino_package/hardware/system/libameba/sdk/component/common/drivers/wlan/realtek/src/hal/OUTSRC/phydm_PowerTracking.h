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
 
#ifndef	__PHYDMPOWERTRACKING_H__
#define    __PHYDMPOWERTRACKING_H__

#define POWRTRACKING_VERSION	"1.0"

#define		DPK_DELTA_MAPPING_NUM	13
#define		index_mapping_HP_NUM	15	
#define	OFDM_TABLE_SIZE 	43
#define	CCK_TABLE_SIZE			33
#define TXSCALE_TABLE_SIZE 		37
#define TXPWR_TRACK_TABLE_SIZE 	30
#define DELTA_SWINGIDX_SIZE     30
#define BAND_NUM 				4
#define CCK_TABLE_SIZE_8711B	41

#define AVG_THERMAL_NUM		8
#define HP_THERMAL_NUM		8
#define IQK_MAC_REG_NUM		4
#define IQK_ADDA_REG_NUM		16
#define IQK_BB_REG_NUM_MAX	10
#if (RTL8192D_SUPPORT==1) 
#define IQK_BB_REG_NUM		10
#else
#define IQK_BB_REG_NUM		9
#endif


#define IQK_Matrix_REG_NUM	8
#if defined(CONFIG_RTL8188E) || defined(CONFIG_RTL8195A) || defined (CONFIG_RTL8711B) || defined (CONFIG_RTL8721D)
#define IQK_Matrix_Settings_NUM	14
#else
#define IQK_Matrix_Settings_NUM	14+24+21 // Channels_2_4G_NUM + Channels_5G_20M_NUM + Channels_5G
#endif

//extern	u4Byte OFDMSwingTable[OFDM_TABLE_SIZE];
//extern	u1Byte CCKSwingTable_Ch1_Ch13[CCK_TABLE_SIZE][8];
//extern	u1Byte CCKSwingTable_Ch14 [CCK_TABLE_SIZE][8];
#if (RTL8195A_SUPPORT == 1)
extern const u1Byte CCKSwingTable_Ch1_Ch13_8195A[CCK_TABLE_SIZE][9];
extern const u1Byte CCKSwingTable_Ch14_8195A[CCK_TABLE_SIZE][9];
extern const u1Byte CCKFCCTable_8195A[16];
extern const u1Byte CCKCETable_8195A[16];
extern const u1Byte CCKFCCTable_Ch14_8195A[16];
#endif

#if (RTL8711B_SUPPORT == 1)

extern const u1Byte CCKFCCTable_8711B[16];
extern const u1Byte CCKCETable_8711B[16];
extern const u1Byte CCKFCCTable_Ch14_8711B[16];
extern const u4Byte CCKSwingTable_Ch1_Ch14_8711B[CCK_TABLE_SIZE_8711B];
#endif

#if (RTL8721D_SUPPORT == 1)

extern const u1Byte CCKFCCTable_8721D[16];
extern const u1Byte CCKCETable_8721D[16];
extern const u1Byte CCKFCCTable_Ch14_8721D[16];
extern const u4Byte CCKSwingTable_Ch1_Ch14_8721D[CCK_TABLE_SIZE_8711B];
#endif

extern const u4Byte OFDMSwingTable_New[OFDM_TABLE_SIZE];
extern const u1Byte CCKSwingTable_Ch1_Ch13_New[CCK_TABLE_SIZE][8];
extern const u1Byte CCKSwingTable_Ch14_New [CCK_TABLE_SIZE][8];
//extern  u4Byte TxScalingTable_Jaguar[TXSCALE_TABLE_SIZE];

// <20121018, Kordan> In case fail to read TxPowerTrack.txt, we use the table of 88E as the default table.
//static u1Byte DeltaSwingTableIdx_2GA_P_8188E[] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4,  4,  4,  4,  4,  4,  5,  5,  7,  7,  8,  8,  8,  9,  9,  9,  9,  9};
//static u1Byte DeltaSwingTableIdx_2GA_N_8188E[] = {0, 0, 0, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5,  6,  6,  7,  7,  7,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11, 11, 11}; 

#define dm_CheckTXPowerTracking 	ODM_TXPowerTrackingCheck

typedef struct _IQK_MATRIX_REGS_SETTING{
	BOOLEAN 	bIQKDone;
#if defined(CONFIG_RTL8188E) || defined(CONFIG_RTL8195A) || defined(CONFIG_RTL8711B) || defined(CONFIG_RTL8721D)
	s4Byte		Value[1][IQK_Matrix_REG_NUM];
#else
	s4Byte		Value[3][IQK_Matrix_REG_NUM];
	BOOLEAN 	bBWIqkResultSaved[3];	
#endif
}IQK_MATRIX_REGS_SETTING,*PIQK_MATRIX_REGS_SETTING;

typedef struct ODM_RF_Calibration_Structure
{
	//for tx power tracking
	
	u4Byte	RegA24; // for TempCCK
	s4Byte	RegE94;
	s4Byte 	RegE9C;
	s4Byte	RegEB4;
	s4Byte	RegEBC;	

	u1Byte  	TXPowercount;
	BOOLEAN bTXPowerTrackingInit; 
	BOOLEAN bTXPowerTracking;
	u1Byte  	TxPowerTrackControl; //for mp mode, turn off txpwrtracking as default
	u1Byte  	TM_Trigger;
    	u1Byte  	InternalPA5G[2];	//pathA / pathB
	
	u1Byte  	ThermalMeter[2];    // ThermalMeter, index 0 for RFIC0, and 1 for RFIC1
	u1Byte  	ThermalValue;
	u1Byte  	ThermalValue_LCK;
	u1Byte  	ThermalValue_IQK;
	u1Byte	ThermalValue_DPK;		
	u1Byte	ThermalValue_AVG[AVG_THERMAL_NUM];
	u1Byte	ThermalValue_AVG_index;		
	u1Byte	ThermalValue_RxGain;
	u1Byte	ThermalValue_Crystal;
	u1Byte	ThermalValue_DPKstore;
	u1Byte	ThermalValue_DPKtrack;
	BOOLEAN	TxPowerTrackingInProgress;
	
	BOOLEAN	bReloadtxpowerindex;	
	u1Byte 	bRfPiEnable;
	u4Byte 	TXPowerTrackingCallbackCnt; //cosa add for debug


	//------------------------- Tx power Tracking -------------------------//
	u1Byte 	bCCKinCH14;
	u1Byte 	CCK_index[MAX_RF_PATH];
	s1Byte	PowerIndexOffset_CCK[MAX_RF_PATH];
	s1Byte	DeltaPowerIndex_CCK[MAX_RF_PATH];
	s1Byte	DeltaPowerIndexLast_CCK[MAX_RF_PATH];
	u1Byte 	OFDM_index[MAX_RF_PATH];
	s1Byte	PowerIndexOffset_OFDM[MAX_RF_PATH];
	s1Byte	DeltaPowerIndex_OFDM[MAX_RF_PATH];
	s1Byte	DeltaPowerIndexLast_OFDM[MAX_RF_PATH];	
	BOOLEAN bTxPowerChanged;
	s1Byte	XtalOffset;
	s1Byte	XtalOffsetLast;
		
	u1Byte 	ThermalValue_HP[HP_THERMAL_NUM];
	u1Byte 	ThermalValue_HP_index;
	IQK_MATRIX_REGS_SETTING IQKMatrixRegSetting[IQK_Matrix_Settings_NUM];
	u1Byte	Delta_LCK;
	s1Byte  BBSwingDiff2G, BBSwingDiff5G; // Unit: dB
	s1Byte  DeltaSwingTableIdx_2GCCKA_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKA_N[DELTA_SWINGIDX_SIZE];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s1Byte  DeltaSwingTableIdx_2GCCKB_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKB_N[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKC_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKC_N[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKD_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GCCKD_N[DELTA_SWINGIDX_SIZE];
#endif
	s1Byte  DeltaSwingTableIdx_2GA_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GA_N[DELTA_SWINGIDX_SIZE];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s1Byte  DeltaSwingTableIdx_2GB_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GB_N[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GC_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GC_N[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GD_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_2GD_N[DELTA_SWINGIDX_SIZE];
#endif
#if !defined(NOT_SUPPORT_5G)
	s1Byte  DeltaSwingTableIdx_5GA_P[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GA_N[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GB_P[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GB_N[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GC_P[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GC_N[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GD_P[BAND_NUM][DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableIdx_5GD_N[BAND_NUM][DELTA_SWINGIDX_SIZE];
#endif	
//	u1Byte  DeltaSwingTableIdx_2GA_P_8188E[DELTA_SWINGIDX_SIZE];
//	u1Byte  DeltaSwingTableIdx_2GA_N_8188E[DELTA_SWINGIDX_SIZE];

	s1Byte  DeltaSwingTableXtal_P[DELTA_SWINGIDX_SIZE];
	s1Byte  DeltaSwingTableXtal_N[DELTA_SWINGIDX_SIZE];

    
    
	//--------------------------------------------------------------------//	
	
	//for IQK	
	u4Byte 	RegC04;
	u4Byte 	Reg874;
	u4Byte 	RegC08;
	u4Byte 	RegB68;
	u4Byte 	RegB6C;
	u4Byte 	Reg870;
	u4Byte 	Reg860;
	u4Byte 	Reg864;
	
	BOOLEAN	bIQKInitialized;
	BOOLEAN bLCKInProgress;
	BOOLEAN	bAntennaDetected;
	BOOLEAN	bNeedIQK;
	BOOLEAN	bIQKInProgress;	
	u1Byte	Delta_IQK;
	u4Byte	ADDA_backup[IQK_ADDA_REG_NUM];
	u4Byte	IQK_MAC_backup[IQK_MAC_REG_NUM];
	u4Byte	IQK_BB_backup_recover[9];
	u4Byte	IQK_BB_backup[IQK_BB_REG_NUM];	
	u4Byte 	TxIQC_8723B[2][3][2]; // { {S1: 0xc94, 0xc80, 0xc4c} , {S0: 0xc9c, 0xc88, 0xc4c}}
	u4Byte 	RxIQC_8723B[2][2][2]; // { {S1: 0xc14, 0xca0} ,           {S0: 0xc14, 0xca0}}

	// <James> IQK time measurement 
	u8Byte	IQK_StartTime;
	u8Byte	IQK_ProgressingTime;
	u4Byte	LOK_Result;

	//for APK
	u4Byte 	APKoutput[2][2]; //path A/B; output1_1a/output1_2a
	u1Byte 	bAPKdone;
	u1Byte 	bAPKThermalMeterIgnore;
	
	// DPK
	BOOLEAN bDPKFail;	
	u1Byte 	bDPdone;
	u1Byte 	bDPPathAOK;
	u1Byte 	bDPPathBOK;

	u4Byte	TxLOK[2];
	u4Byte  DpkTxAGC;
	s4Byte  DpkGain;
	u4Byte  DpkThermal[4];
}ODM_RF_CAL_T,*PODM_RF_CAL_T;


VOID	
ODM_TXPowerTrackingCheck(
	IN 	PVOID	 	pDM_VOID
	);


VOID
odm_TXPowerTrackingInit(
	IN 	PVOID	 	pDM_VOID
	);

VOID
odm_TXPowerTrackingCheckAP(
	IN 	PVOID	 	pDM_VOID
	);

VOID
odm_TXPowerTrackingThermalMeterInit(
	IN 	PVOID	 	pDM_VOID
	);

VOID
odm_TXPowerTrackingInit(
	IN 	PVOID	 	pDM_VOID
	);

VOID
odm_TXPowerTrackingCheckMP(
	IN 	PVOID	 	pDM_VOID
	);


VOID
odm_TXPowerTrackingCheckCE(
	IN 	PVOID	 	pDM_VOID
	);

VOID
odm_TXPowerTrackingCheckIOT(
	IN 	PVOID	 	pDM_VOID
	);

#if(DM_ODM_SUPPORT_TYPE & (ODM_WIN)) 

VOID 
odm_TXPowerTrackingCallbackThermalMeter92C(
            IN PADAPTER	Adapter
            );

VOID
odm_TXPowerTrackingCallbackRXGainThermalMeter92D(
	IN PADAPTER 	Adapter
	);

VOID
odm_TXPowerTrackingCallbackThermalMeter92D(
            IN PADAPTER	Adapter
            );

VOID
odm_TXPowerTrackingDirectCall92C(
            IN	PADAPTER		Adapter
            );

VOID
odm_TXPowerTrackingThermalMeterCheck(
	IN	PADAPTER		Adapter
	);

#endif

#endif
