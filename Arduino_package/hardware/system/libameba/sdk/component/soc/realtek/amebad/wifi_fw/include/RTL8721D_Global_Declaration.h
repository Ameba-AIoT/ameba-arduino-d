#ifndef _RTL8721D_GLOBAL_DECLARATION_H
#define _RTL8721D_GLOBAL_DECLARATION_H

#ifndef __ASSEMBLY__
//Extern Variable
#ifdef __RTL8721D_INIT_C__
/*--------------------Define -------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct-------------------------------------*/

//FW_RAM_BSS_SECTION     struct mib_info_ram         SysMib;

FW_ROM_BSS_SECTION     u8                          TaskBitMap;

//2 INT HAL
FW_ROM_BSS_SECTION     PHAL_DATA_8721D             pIntHdl;

//2 Wlan
FW_ROM_BSS_SECTION     PRsvdPageLoc                pRPL;
FW_ROM_BSS_SECTION     PH2C_EVT                    pH2CEvt;
FW_ROM_BSS_SECTION     PC2H_EVT                    pC2HEvt;
FW_ROM_BSS_SECTION     u16                         RXFF0_BNDY; 

#if 0
FW_ROM_BSS_SECTION     PTASK_INFO_PARM             pTaskInfoParm;
FW_ROM_BSS_SECTION     PEFUSE_PARM                 pEfuseParm;
FW_ROM_BSS_SECTION     PRX_DESC_8822C              pRxDescPtr;
FW_ROM_BSS_SECTION     PRXPKT_INFO_FUNC            pRxPktInfo;
FW_ROM_BSS_SECTION     PLINK_PARM                  pLinkParm;
FW_ROM_BSS_SECTION     PDBGINFO_PARM               pDbgInfoParm;

//2 FW Offload
FW_ROM_BSS_SECTION     PFWOFFLOAD_FUNC             pFWOffloadFunc;
FW_ROM_BSS_SECTION     PDATAPACK_BUFF              pDataBuff;
FW_ROM_BSS_SECTION     PPACKET_BUFF                pPacketBuff;
FW_ROM_BSS_SECTION     PGENERAL_INFO               pGeneralInfo;
FW_ROM_BSS_SECTION     PCONFIG_RPT                 pConfigRpt;
FW_ROM_BSS_SECTION     PSCAN_PARA                  pScanParam;
#endif

//2 IPSEC
FW_ROM_BSS_SECTION     PIPSEC_FUNC                 pIpSec;
FW_ROM_BSS_SECTION     PDATA_CTRL_PARM             pDataCtrlParm;
FW_ROM_BSS_SECTION     POP_MODE_PARM               pOpModeParm;
FW_ROM_BSS_SECTION     PMSG_KEY_PARM               pMsgKeyParm;
FW_ROM_BSS_SECTION     PMICHAEL_PKT_PARM           pMichaelPktParm;
FW_ROM_BSS_SECTION     PHW_PARM                    pHwParm;

//2 PS
//P2PPS
FW_ROM_BSS_SECTION     PP2PPS_Info                 pP2PPSInfo;
FW_ROM_BSS_SECTION     PPS_PARM                    pPSParm;
FW_ROM_BSS_SECTION     PPS_TUNING_PPARM            pPSTParm;
FW_ROM_BSS_SECTION     PPS_BT_Parm                 pPSBTParm;
FW_ROM_BSS_SECTION     u8                          ScanEn;

//2 Media Status
FW_ROM_BSS_SECTION     PMEDIA_STATUS               pMediaStatus;


//2 BT Coex
FW_ROM_BSS_SECTION     PBT_B_TYPE_TDMA_Parm        pBtTdmaParm;
FW_ROM_BSS_SECTION     PBTTxStatus_Parm            pBTTxStatus;
FW_ROM_BSS_SECTION     PBT_MAILBOX_Parm            pBtParm;
FW_ROM_BSS_SECTION     PWLAN_Opmode_Parm           pWIFI_Info;
FW_ROM_BSS_SECTION     PBT_Page_Scan_Interval_Parm pBT_Page_Scan_Interval;
FW_ROM_BSS_SECTION     PBT_Init_Parm               pBT_Init_Info;
FW_ROM_BSS_SECTION     u8                          BTDebugEN;
FW_ROM_BSS_SECTION     u8                          BTcnt[40];

#if 0
//2 MCC
FW_ROM_BSS_SECTION     PH2C_MCCTIME_PARM           pMccTimeParm;
FW_ROM_BSS_SECTION     PMCC_CH_PARM                pChParm[MCC_MAX_CHIFO_NUM];
FW_ROM_BSS_SECTION     PMCC_GEN_PARM               pGenParm;
FW_ROM_BSS_SECTION     PMCC_UPDATE_PARM            pUpdateParm;
FW_ROM_BSS_SECTION     PMCC_POWER_IDX_PARM         pPwrIdxParm[2];
FW_ROM_BSS_SECTION     PCOMMON_PARM                pPDMOdmCommon;
#endif

//2 WoWLAN
//FW_ROM_BSS_SECTION     PH2C_REMOTE_WAKEUP_PARM     pRWakeupCtrlParm;
FW_ROM_BSS_SECTION     PAOAC_INFO                  pAOACInfo;
FW_ROM_BSS_SECTION     PARP_INFO                   pARPInfo;
FW_ROM_BSS_SECTION     PNDP_INFO                   pNDPInfo[2];
FW_ROM_BSS_SECTION     PIPV6_PSEUDO_HEADER_INFO    pIpv6PseudoHeaderInfo;
FW_ROM_BSS_SECTION     PSECURITY_INFO              pSecurityInfo;
FW_ROM_BSS_SECTION     PAOAC_REPORT                pAOACReport;
FW_ROM_BSS_SECTION     PTEREDO_INFO                pteredo_info;  
FW_ROM_BSS_SECTION     PREALWOW_VER2_INFO          pRWV2;
FW_ROM_BSS_SECTION     PNLO_OFFLOAD_NETWORK_LIST_INFO pNetworkListInfo;
FW_ROM_BSS_SECTION     PAOAC_GLOBAL_INFO           pAOACGlobalInfo;
FW_ROM_BSS_SECTION     PAOAC_RSVDPAGE1             pAOACPage1; 

//2 ODM
FW_ROM_BSS_SECTION     Podm                        pODM;
FW_ROM_BSS_SECTION     PSTAINFO_RA                 pstainfo_ra[MACID_NUM];
FW_ROM_BSS_SECTION     PPHYDM_DEBUG_T              pDEBUG;
FW_ROM_BSS_SECTION     PPHYDM_RA_T                 pRA;
FW_ROM_BSS_SECTION     u8                          (*pARFB_table)[ARFB_table_NUM];
FW_ROM_BSS_SECTION     u8                          *pRATE_UP_FAIL_WAITING;

/*--------------------Export global variable----------------------------*/

#else
//extern struct mib_info_ram                      SysMib;

extern     u8                          TaskBitMap;

//2 INT HAL
extern     PHAL_DATA_8721D                      pIntHdl;

//2 Wlan
extern     PRsvdPageLoc                         pRPL;
extern     PH2C_EVT                             pH2CEvt;
extern     PC2H_EVT                             pC2HEvt;
extern     u16                                  RXFF0_BNDY; 

#if 0
extern PTASK_INFO_PARM                          pTaskInfoParm;
extern PEFUSE_PARM                              pEfuseParm;
extern PRX_DESC_8822C                           pRxDescPtr;
extern PRXPKT_INFO_FUNC                         pRxPktInfo;
extern PLINK_PARM                               pLinkParm;
extern PDBGINFO_PARM                            pDbgInfoParm;

//2 FW Offload
extern PFWOFFLOAD_FUNC                          pFWOffloadFunc;
extern PDATAPACK_BUFF                           pDataBuff;
extern PPACKET_BUFF                             pPacketBuff;
extern PGENERAL_INFO                            pGeneralInfo;
extern PCONFIG_RPT                              pConfigRpt;
extern PSCAN_PARA                               pScanParam;
#endif

//2 IPSEC
extern PIPSEC_FUNC                              pIpSec;
extern PDATA_CTRL_PARM                          pDataCtrlParm;
extern POP_MODE_PARM                            pOpModeParm;
extern PMSG_KEY_PARM                            pMsgKeyParm;
extern PMICHAEL_PKT_PARM                        pMichaelPktParm;
extern PHW_PARM                                 pHwParm;

//2 PS
//P2PPS
extern PP2PPS_Info                              pP2PPSInfo;
extern PPS_PARM                                 pPSParm;
extern PPS_TUNING_PPARM                         pPSTParm;
extern PPS_BT_Parm                              pPSBTParm;
extern u8                                       ScanEn;

//2 Media Status
extern PMEDIA_STATUS                            pMediaStatus;


//2 BT Coex
extern PBT_B_TYPE_TDMA_Parm                     pBtTdmaParm;
extern PBTTxStatus_Parm                         pBTTxStatus;
extern PBT_MAILBOX_Parm                         pBtParm;
extern PWLAN_Opmode_Parm                        pWIFI_Info;
extern PBT_Page_Scan_Interval_Parm              pBT_Page_Scan_Interval;
extern PBT_Init_Parm                            pBT_Init_Info;
extern u8                                       BTDebugEN;
extern u8                                       BTcnt[40];

#if 0
//2 MCC
extern PH2C_MCCTIME_PARM                        pMccTimeParm;
extern PMCC_CH_PARM                             pChParm[MCC_MAX_CHIFO_NUM];
extern PMCC_GEN_PARM                            pGenParm;
extern PMCC_UPDATE_PARM                         pUpdateParm;
extern PCOMMON_PARM                             pPDMOdmCommon;
extern PMCC_POWER_IDX_PARM                      pPwrIdxParm[2];
#endif

//2 WoWLAN
//extern PH2C_REMOTE_WAKEUP_PARM                  pRWakeupCtrlParm;
extern PAOAC_INFO                               pAOACInfo;
extern PARP_INFO                                pARPInfo;
extern PNDP_INFO                                pNDPInfo[2];
extern PIPV6_PSEUDO_HEADER_INFO                 pIpv6PseudoHeaderInfo;
extern PSECURITY_INFO                           pSecurityInfo;
extern PAOAC_REPORT                             pAOACReport;
extern PTEREDO_INFO                             pteredo_info;  
extern PREALWOW_VER2_INFO                       pRWV2;
extern PNLO_OFFLOAD_NETWORK_LIST_INFO           pNetworkListInfo;
extern PAOAC_GLOBAL_INFO                        pAOACGlobalInfo;
extern PAOAC_RSVDPAGE1                          pAOACPage1; 

//2 ODM
extern Podm                                    pODM;
extern PSTAINFO_RA                             pstainfo_ra[MACID_NUM];
extern PPHYDM_DEBUG_T                          pDEBUG;
extern PPHYDM_RA_T                             pRA;
extern u8                                      (*pARFB_table)[ARFB_table_NUM];
extern u8                                      *pRATE_UP_FAIL_WAITING;

/*--------------------Function declaration------------------------------*/


#endif  //#ifdef __RTL8721D_HWINIT_ROM_C__
#endif //#ifndef __ASSEMBLY__

#endif //_RTL8822C_GLOBAL_DECLARATION_H
