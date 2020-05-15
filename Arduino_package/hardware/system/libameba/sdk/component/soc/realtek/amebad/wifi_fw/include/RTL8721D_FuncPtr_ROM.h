#ifndef __RTL8721D_FUNCPTR_ROM_H__
#define __RTL8721D_FUNCPTR_ROM_H__

/*--------------------Define -------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct-------------------------------------*/
//function pointer, added by jackie;
/*typedef u32 (*_PHY_QueryBBReg_Ptr)(IN  u16 RegAddr);

typedef void (*_PHY_SetBBReg_Ptr)(IN  u16 RegAddr, IN  u32 Data);

typedef u32 (*_PHY_QueryRFReg_Ptr)(IN  u8 eRFPath, IN  u8 RegAddr);

typedef void (*_PHY_SetRFReg_Ptr)(IN  u8 eRFPath,  IN  u8  RegAddr, IN  u32 Data);*/
//3 Task
typedef void (*_SendSignalCommon_Ptr)(u8  event);  //brian for function both called in isrhdl or not
typedef void (*_OSSendSignalCommon_Ptr)(u8  event);
typedef void (*_ISRSendSignalCommon_Ptr)(u8  event);

//3 RA
typedef void (*_InitialRateUpdate_Ptr)( IN u8 macid,  IN u8 rate, IN u8 trybit, IN u8 BW); 
//8051 can't take more than 3 parameters for function pointer
typedef void (*_InitRAInfo_rsvd_Ptr)(void);
//typedef void (*_TryDone_Ptr)(void);
typedef void (*_Set_arfr_Ptr)(IN u8 e0, IN u8 e1, IN u8 e2 \
    , IN u8 e3, IN u8 e4, IN u8 e5, IN u8 e6, IN u8 e7);
//8051 can't take more than 3 parameters for function pointer
typedef u8 (*_check_rate_Ptr)(IN u8 rate_idx, IN u8 macid);
typedef void (*_RateUp_search_RateMask_Ptr)(IN u8 init_rate_idx, IN u8 highest_rate,IN u8 macid);
typedef void (*_ArfrRefresh_Ptr)(IN u8 macid);
typedef void (*_InitialRateByRssi_Ptr )(IN u8 macid, IN u8 start_rssi);
typedef void (*_ODM_H2C_RADebugMonitor_Ptr)(IN u8 macid, IN u8 mode);

//3 H2C/C2H CMD
typedef void (*_H2CCmdFunc_Ptr)(u8  CmdID, u8  *content);
typedef void (*_C2HInQueue_Ptr)(PC2H_EVT_HDR pc2h_hdr);
typedef void (*_C2HDeQueue_Ptr)(void);
typedef void (*_IssueC2HPKT_Ptr)( IN  PC2H_EVT_HDR pc2h_hdr);
typedef void (*_FillC2HContant_Ptr)( IN u16  C2HPKTOffset, IN  PC2H_EVT_HDR pc2h_hdr);
typedef void (*_C2HPKTRxDESC_Ptr)(IN  u16 PktLength, IN  u16  C2HDescOffset);
typedef void (*_H2CInQueue_Ptr)( void);
typedef void (*_H2CDeQueue_Ptr)( void);

typedef BOOLEAN (*_PktBufAccessCtrl_Ptr)(IN u8 mode,IN u8 rw,\
    IN u16 offset,IN u8 wbit);
//8051 can't take more than 3 parameters for function pointer

//3 BT Coex
typedef void (*_Write778_Ptr)( IN u8 value) ;
typedef u8 (*_WL2BTMailbox_Ptr)( IN  u8 cmd,  IN  u8 length,  IN  u8 *pparm);
typedef void (*_FillMailbox_Ptr)(   IN  u8 cmd,    IN  u8 length,   IN  u8 *pparm);
typedef void (*_BT_TDMA_Slot_Operation_1and4_Ptr)( IN u8	slot_num);
typedef void (*_BT_TDMA_Slot_Operation_2and5_Ptr)( IN u8	slot_num); 
typedef void (*_AOAC_ANT_SWT_Ptr)(IN  u8  TwoAnt,  IN  u8  value, \
    IN  u8  AntSwitchOutside, IN  u8  AntSelReverse);
//8051 can't take more than 3 parameters for function pointer
typedef void (*_InitBTCoexTimer_Ptr)(IN  u8 duration);
typedef void (*_WlanActCtrl_Ptr)(void);
typedef void (*_ChangeCoexTable_Ptr)( IN  u8  op);
typedef void (*_C2HBTMpRpt_Ptr)(IN  u8  idx, IN  u8  status,  IN  u8  req_num);
//typedef void (*_BTSCOeSCOIssueNullHDL_Ptr)(void);
typedef void (*_BTRoleChangeTimerHDL_Ptr)(void);
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
typedef void (*_AntennaSwitchCtrl_Ptr)(u8 side, u8 ctrl);
#endif
typedef void (*_InitBTypeTDMA2_Ptr)(void);
typedef void (*_BTypeTDMABCNEarly_Ptr)(void);
typedef void (*_InitBTMailbox_Ptr)(void);
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
typedef void (*_InitSCOTimer_Ptr)(IN  u16 duration);
typedef void (*_BTSCOeSCOAntSetup_Ptr)( IN	u8	slot);
typedef void (*_SCO_eSCO_IssueCtrlData_Ptr)(IN u8 ctrl_pkt_type, IN u8 slot, IN u8 power_bit);
#endif
typedef void (*_BTMailboxRetry_Ptr)( void);
typedef void (*_C2HBTLoopback_Ptr)( IN  u8  *pbuf);
typedef void (*_BackupAntennaSetting_Ptr)(IN  u8  option);
typedef void (*_BTypeTDMATBTThdl_Ptr)( void);
//typedef void (*_C2HBTMailBoxStatus_Ptr)(IN  u8  status);
typedef void (*_BTRoleChangeTBTTHDL_Ptr)( void);
typedef void (*_IssueBTQNull_Ptr)(IN  u8  bPowerBit, IN  u8  lsb, IN  u8  msb);
typedef void (*_DacSwing_Ptr)( IN u8 action);
typedef void (*_BTypeTDMATimeOuthdl_Ptr)( void);
typedef void (*_MailboxINTHDL_Ptr)( void);
typedef void (*_MailboxINTHDL_Patch_Ptr)( u8 cmd );
typedef void (*_BTNullSetting_Ptr)( IN  u8  bPowerBit,  IN  u16 offset, \
    IN  u8  lsb,  IN  u8  msb);
//8051 can't take more than 3 parameters for function pointer
typedef void (*_BTRoleChangeDefaultSetting_Ptr)( void);
//typedef void (*_InitBTypeTDMA_Ptr)( void);
typedef void (*_FlexibleExtensionDecision_Ptr)( void);
typedef void (*_BT_TDMA_Slot_Operation_0and3_Ptr)( IN u8	slot_num );
typedef void (*_C2HBTInfo_Ptr)( IN  u8  option);

//3 Power save
typedef void (*_SetSmartPSTimer_Ptr)(void);
typedef void (*_SetPwrStateReg_Ptr)( IN u8 PwrStateType, IN u8 value);
typedef void (*_Change_PS_State_Ptr)( IN  u8  request_ps_state,  IN  u8  nulldata0Allow);
typedef void (*_ResetPSParm_Ptr)( void);
typedef void (*_Legacy_PS_Setting_Ptr)(void);
typedef void (*_PSModeSetting_Ptr)( IN u8 on);
typedef void (*_PsOpenRF_Ptr)( void);								        
typedef void (*_PsCloseRF_Ptr)( void);
typedef BOOLEAN (*_PS_S2_Condition_Match_Ptr)( void);
typedef u8 (*_PS_S4_Condition_Match_Ptr)( void);
typedef BOOLEAN (*_WaitTxStateMachineOk_Ptr)(void);
typedef void (*_WriteTxPause_Ptr)(IN u8 value, IN u8 rcode);
typedef void (*_WriteTxPauseWithMask_Ptr)(IN u8 mask, IN u8 value, IN u8 rcode);
//typedef u8 (*_BTC_Pnty_Shift_Ptr)(IN u8  rate_idx);
typedef void (*_IssueCTS2Self_Ptr)(IN  u8 location, IN u16 NAV);
typedef void (*_ChkandChangePS_Ptr)(IN  u8 bPowerBit);
typedef void (*_IssuePSPoll_Ptr)( void);
typedef u8 (*_ReadRxBuff_Ptr)(IN u16 headaddr, IN u8  offset);
typedef void (*_IssueQNull_Ptr)( IN  u8  bPowerBit);
typedef void (*_PowerBitSetting_Ptr)(IN  u8 bPowerBit, IN  u16 offset);
typedef void (*_ChangeTransmiteRate_Ptr)( IN u16 offset);
typedef BOOLEAN (*_IssueNullData_Ptr)(IN  u8 bPowerBit, IN  u8 RtyLmt);
typedef void (*_ConfigListenBeaconPeriod_Ptr)( IN u8  RLBM,  IN u8  AwakeInterval);
typedef void (*_PS_S2ToS0State_Ptr)(void);
typedef void (*_SetBcnEarlyAndTimeout_Ptr)(u8 BcnAheadShift,  u8 BcnEarlyShift,  \
    u8 BcnTimeout, u8 ReasonCode);
//8051 can't take more than 3 parameters for function pointer
typedef void (*_EnlargeBcnEarlyAndTimeout_Ptr)(void);
typedef void (*_ResetBcnEarlyAdjustParm_Ptr)(BOOLEAN OnJoin);
typedef u16 (*_IssueRsvdPagePacketSetting_Ptr)(IN  u8 PageNum, IN  BOOLEAN bHwSEQEn,\
    IN  u8 RtyLmt,   IN  u8 TxInd);
//8051 can't take more than 3 parameters for function pointer

//3  Gtimer
typedef void (*_InitGTimer32us_Ptr)(IN u16 timer_addr, IN u8  _mode,\
    IN u8 Unit2s, IN u8 Unit8ms, IN u8 Unit32us); 
//8051 can't take more than 3 parameters for function pointer
typedef void (*_InitGTimer1ms_Ptr)(IN  u16 timer_addr, IN  u8 _mode,\
    IN  u8 Unit65s,  IN  u8 Unit256ms, IN  u8 Unit1ms); 
//8051 can't take more than 3 parameters for function pointer

//3 Basic IO
typedef  u8 (*_PollingReg_8721D_PTR)(u16 RegOffset,u8 PollingBit,u8 PollingResult,u16 PollingLimit, u16 *PollingCount);


//3 WoWLAN
typedef void (*_WakeUpHost_Ptr)(IN u8 source) ;
typedef void (*_UpdateIV_Ptr)(IN u8* buf, IN u8 retry_bit) ;
typedef void (*_updateCheckSumVer2_Ptr)(IN u8 mode, IN u16 offset, IN u8 loc_rsvd) ;
typedef u8 (*_GetMACHeaderLen_Ptr)(IN u8  macheader, IN u8  FrameCtrl, IN u8  raaddr) ;
typedef u8 (*_GetIVLenAndSecurityType_Ptr)(IN u8 raaddr) ;

//3 Security entry
typedef void (*_AesTkipIvFun_Ptr)(IN u8* pattrib_iv, IN u8 RetryBit, IN u8 SecurityType) ;
//8051 can't take more than 3 parameters for function pointer
typedef void (*_WepIvFun_Ptr)(IN unsigned char* iv, IN unsigned char retry_bit) ;
typedef u8 (*_OnWpa_offload_Ptr)(IN u16 mac_header_offset, IN u8 MacHeaderLen) ;
typedef void (*_wpa_supplicant_process_1_of_2_Ptr)(IN u16 version, IN u8 retry_bit) ;
typedef void (*_wpa_supplicant_decrypt_key_data_Ptr)(IN u16 key_desc_version) ;
typedef void (*_wpa_supplicant_send_2_of_2_Ptr)(IN u16 verson, IN u8 retry_bit) ;
typedef void (*_write_cam_wow_Ptr)(IN u8  entry, IN u8 encAlg, IN u8 *key) ;
typedef void (*_WriteCAM_Ptr)(IN u16 Addr, IN u8* Data);
typedef void (*_ReadCAM_Ptr)(IN u16 Addr, IN u8* Data);
typedef void (*_ReadGTKData_Ptr)(IN u8 entry, IN u8* key);
typedef void (*_AppendTkipMIC_Ptr)(IN u8 pktLoc, IN u8  pyldOffset);
typedef s8 (*_RTmemcmpBackward_Ptr)(IN const void *Dest, IN const void *Src, IN u8 count);
typedef u8 (*_AesTkipIvCheck_Ptr)(IN u8 *rx_iv_ori, IN u8 *rx_iv, IN u8 SecurityType);
typedef u8 (*_WepIvCheck_Ptr)(IN u8* rx_iv_ori, IN u8* rx_iv);
typedef void (*_AssignIvToKeyRsc_Ptr)(IN u8 *pattrib_iv, IN u8 *KeyRsc, IN u8 *KeyID, IN u8 SecurityType);
typedef void (*_AssignKeyRscToIV_Ptr)(IN u8 *pattrib_iv, IN u8 *KeyRsc, IN u8 SecurityType);


//3 IPSEC
typedef u32 (*_getSumValue_8721D_PTR)(u32 offset,u16 pktLen) ;
typedef u16 (*_getCheckSum_8721D_PTR)(u8 protocol,u16 pktLen,u32 src_addr,u32 dest_addr,u32 input) ;

typedef void (*_IPSecDataIn_8721D_PTR)(u32 *DataIn,u8 DataSel);
typedef void (*_IPSecDataOut_8721D_PTR)(u32 *DataOut,u8 DataSel);
typedef void (*_IPSecFillDesrciptor_8721D_PTR)(IPSEC_FUNC *IpsecInfo,u16 MsgOffsetDesc,u8 MsgLengthDesc);
typedef void (*_IPSWriteDescriptorIO_8721D_PTR)(u32 *Desc);
typedef void (*_IPSecDesrciptor_8721D_PTR)(IPSEC_FUNC *IpsecInfo);
typedef void (*_IPSecWriteDataIOMode_8721D_PTR)(u8 *Data,u8 DataLength);
typedef void (*_IPSecGetDataIOMode_8721D_PTR)(u8 *Data,u8 DataLength);
typedef void (*_IPSecSetOPMode_8721D_PTR)(u8 OPMode,u8 PollingBit);
typedef void (*_IPSecEngine_8721D_PTR)(IPSEC_FUNC *IpsecInfo,u8 *Msg,u8 *Key) ;
typedef void (*_IPSecSetExMemoryAddr_8721D_PTR)(u16 ExMStradr);
typedef void (*_IPSecSetDescAddr_8721D_PTR)(u16 DesStradr);
typedef void (*_AesUnwrapIpsec_8721D_PTR)(u8 *Kek,u8 *KeyData,u8 *Gtk,u8 GtkLen);
typedef void (*_AESUnwrapSwap_8721D_PTR)(u8 *AESData,u8 DataLength);
typedef void (*_HmacSha1Ipsec_8721D_PTR)(u8 *Kck,u8 *Pframe,u8 *Hash,u8 HashLen);
typedef void (*_HmcMd5Ipsec_8721D_PTR)(u8 *Pframe,u8 *Kck,u8 *Mic,u8 MicLen);
typedef void (*_RC4Ipsec_8721D_PTR)(u8 *Ek,u8 *KeyData, u8 *GtkData,u8 GtkDataLen);
typedef void (*_TkipMicIpsec_8721D_PTR)(u8 *MicKey,u8 *Message,u8 *Mic,u8 MicLen);
typedef void (*_Aes128EncIpsec_8721D_PTR)(u8 *InitialKey,u8 *InputMessage,u8 *Gtk,u8 GtkLen);
typedef void (*_Aes128DecIpsec_8721D_PTR)(u8 *InitialKey,u8 *InputMessage,u8 *Gtk,u8 GtkLen);
typedef void (*_FillParameterIpsec_8721D_PTR)(u16 MsgOffset,u16 KeyOffset,u8 MsgLength,u8 KeyLength,u16 DesStradr);
typedef void (*_FillMichaelPacketInfo_8721D_PTR)(u16 DaOffset,u16 SaOffset,u16 QosOffset,u16 ContentOffset,u8 QosOrNot);
/* AES operations */
typedef void (*_AESEncrypt128_8721D_PTR)(u8 *PlainBlock,u32   PlainBlockSize,u8 *Key,u32 KeyLength,u8 *CipherBlock,u32 *CipherBlockSize);
typedef void (*_AESDecrypt128_8721D_PTR)(u8 *PlainBlock,u32 PlainBlockSize,u8 *Key,u32 KeyLength,u8   *CipherBlock,u32 *CipherBlockSize);
typedef void (*_AES_KeyExpansion_1W_8721D_PTR)(u8 *Key,u32 KeyLength,AES_CTX_STRUC *paes_ctx);
typedef void (*_AES_CMAC_GenerateSubKey_1W_8721D_PTR)(u8  *Key,u8 KeyLength,u8 *SubKey1,u8 *SubKey2);
typedef void (*_AES_CMAC_1W_8721D_PTR)(u8 *PlainText,u8 PlainTextLength,u8 *Key,u8 KeyLength,u8 *MACText,u32 *MACTextLength);
typedef void (*_AES_Encrypt_1W_8721D_PTR)(u8 *PlainBlock,u32 PlainBlockSize,u8 *Key,u32 KeyLength,u8 *CipherBlock,u32 *CipherBlockSize);

//3 FWLBK
typedef void (*_FwlbkDbgPrtU16_8721D_PTR)(IN u16 addr, IN u16 wdata);
typedef void (*_FwlbkLLTWrite_8721D_PTR)(IN u16 address, IN u16 value);
typedef u8 (*_FwlbkHWInitialLLT_8721D_PTR)(void);
typedef void (*_FwlbkMode_8721D_PTR)(void);
typedef void (*_FwlbkPreparePkt_8721D_PTR)(void);
typedef void (*_FwlbkModeSel_8721D_PTR)(void);
typedef void (*_FwlbkCheckRxPayload_8721D_PTR)(void);
typedef void (*_FwlbkSetCAM_8721D_PTR)(IN u8 sec_type);
typedef void (*_FwlbkUpdateParam_8721D_PTR)(IN u8 Datarate);
typedef u8 (*_FwlbkMcuPollMgq_8721D_PTR)(void);
typedef void (*_FwlbkModeTxNoCheck_8721D_PTR)(void);
typedef void (*_FwlbkModeTx_8721D_PTR)(void);
typedef void (*_FwlbkModeMaclbk_8721D_PTR)(void);
typedef void (*_FwlbkModeSecTest_8721D_PTR)(void);
typedef void (*_FwlbkModeAFElbk_8721D_PTR)(void);


/*------------------------Export global variable------------------------------*/
#ifdef  __RTL8721D_INIT_C__

//Direct Mapping HW Register

//function pointer, added by jackie;
//3 Task
//brian for function both called in isrhdl or not, add to rom if possible
//FW_ROM_BSS_SECTION  _SendSignalCommon_Ptr               SendSignalCommon_Ptr;   
FW_ROM_BSS_SECTION  _OSSendSignalCommon_Ptr               OSSendSignalCommon_Ptr;
FW_ROM_BSS_SECTION  _ISRSendSignalCommon_Ptr              ISRSendSignalCommon_Ptr;

//3 RA
FW_ROM_BSS_SECTION  _InitialRateUpdate_Ptr                InitialRateUpdate_Ptr;
FW_ROM_BSS_SECTION  _InitRAInfo_rsvd_Ptr                  InitRAInfo_rsvd_Ptr;
FW_ROM_BSS_SECTION  _InitialRateByRssi_Ptr                InitialRateByRssi_Ptr;
//FW_ROM_BSS_SECTION  _TryDone_Ptr                                        TryDone_Ptr;
FW_ROM_BSS_SECTION  _Set_arfr_Ptr                         Set_arfr_Ptr;
FW_ROM_BSS_SECTION  _check_rate_Ptr                       check_rate_Ptr;
FW_ROM_BSS_SECTION  _RateUp_search_RateMask_Ptr           RateUp_search_RateMask_Ptr; 
FW_ROM_BSS_SECTION  _ArfrRefresh_Ptr                      ArfrRefresh_Ptr;
FW_ROM_BSS_SECTION  _ODM_H2C_RADebugMonitor_Ptr           ODM_H2C_RADebugMonitor_Ptr;
//FW_ROM_BSS_SECTION  _H2CHDL_SetRssiSingle_Ptr                    H2CHDL_SetRssiSingle_Ptr;

//3 H2C/C2H Cmd
FW_ROM_BSS_SECTION  _H2CCmdFunc_Ptr                       H2CCmdFunc_Ptr;
FW_ROM_BSS_SECTION  _C2HInQueue_Ptr                       C2HInQueue_Ptr;
FW_ROM_BSS_SECTION  _C2HDeQueue_Ptr                       C2HDeQueue_Ptr;
FW_ROM_BSS_SECTION  _IssueC2HPKT_Ptr                      IssueC2HPKT_Ptr;
FW_ROM_BSS_SECTION  _FillC2HContant_Ptr                   FillC2HContant_Ptr;
FW_ROM_BSS_SECTION  _C2HPKTRxDESC_Ptr                     C2HPKTRxDESC_Ptr;
FW_ROM_BSS_SECTION  _H2CInQueue_Ptr                       H2CInQueue_Ptr;
FW_ROM_BSS_SECTION  _H2CDeQueue_Ptr                       H2CDeQueue_Ptr;
FW_ROM_BSS_SECTION  _PktBufAccessCtrl_Ptr                 PktBufAccessCtrl_Ptr;

//3 BT Coex
FW_ROM_BSS_SECTION  _Write778_Ptr                         Write778_Ptr;
FW_ROM_BSS_SECTION  _WL2BTMailbox_Ptr                     WL2BTMailbox_Ptr;
FW_ROM_BSS_SECTION  _FillMailbox_Ptr                      FillMailbox_Ptr;
FW_ROM_BSS_SECTION  _BT_TDMA_Slot_Operation_1and4_Ptr     BT_TDMA_Slot_Operation_1and4_Ptr;
FW_ROM_BSS_SECTION  _BT_TDMA_Slot_Operation_2and5_Ptr     BT_TDMA_Slot_Operation_2and5_Ptr; 
FW_ROM_BSS_SECTION  _AOAC_ANT_SWT_Ptr                     AOAC_ANT_SWT_Ptr;
FW_ROM_BSS_SECTION  _InitBTCoexTimer_Ptr                  InitBTCoexTimer_Ptr;
FW_ROM_BSS_SECTION  _WlanActCtrl_Ptr                      WlanActCtrl_Ptr;
FW_ROM_BSS_SECTION  _ChangeCoexTable_Ptr                  ChangeCoexTable_Ptr;
FW_ROM_BSS_SECTION  _C2HBTMpRpt_Ptr                       C2HBTMpRpt_Ptr;
//FW_ROM_BSS_SECTION  _BTSCOeSCOIssueNullHDL_Ptr                BTSCOeSCOIssueNullHDL_Ptr;
FW_ROM_BSS_SECTION  _BTRoleChangeTimerHDL_Ptr             BTRoleChangeTimerHDL_Ptr;
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
FW_ROM_BSS_SECTION  _AntennaSwitchCtrl_Ptr                AntennaSwitchCtrl_Ptr;
#endif
FW_ROM_BSS_SECTION  _InitBTypeTDMA2_Ptr                   InitBTypeTDMA2_Ptr;
FW_ROM_BSS_SECTION  _BTypeTDMABCNEarly_Ptr                BTypeTDMABCNEarly_Ptr;
FW_ROM_BSS_SECTION  _InitBTMailbox_Ptr                    InitBTMailbox_Ptr;
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
FW_ROM_BSS_SECTION  _InitSCOTimer_Ptr                     InitSCOTimer_Ptr;
FW_ROM_BSS_SECTION  _BTSCOeSCOAntSetup_Ptr                BTSCOeSCOAntSetup_Ptr;
FW_ROM_BSS_SECTION  _SCO_eSCO_IssueCtrlData_Ptr           SCO_eSCO_IssueCtrlData_Ptr;
#endif
FW_ROM_BSS_SECTION  _BTMailboxRetry_Ptr                   BTMailboxRetry_Ptr;
FW_ROM_BSS_SECTION  _C2HBTLoopback_Ptr                    C2HBTLoopback_Ptr;
FW_ROM_BSS_SECTION  _BackupAntennaSetting_Ptr             BackupAntennaSetting_Ptr;
FW_ROM_BSS_SECTION  _BTypeTDMATBTThdl_Ptr                 BTypeTDMATBTThdl_Ptr;
//FW_ROM_BSS_SECTION  _C2HBTMailBoxStatus_Ptr                       C2HBTMailBoxStatus_Ptr;
FW_ROM_BSS_SECTION  _BTRoleChangeTBTTHDL_Ptr              BTRoleChangeTBTTHDL_Ptr;
FW_ROM_BSS_SECTION  _IssueBTQNull_Ptr                     IssueBTQNull_Ptr;
FW_ROM_BSS_SECTION  _DacSwing_Ptr                         DacSwing_Ptr;
FW_ROM_BSS_SECTION  _BTypeTDMATimeOuthdl_Ptr              BTypeTDMATimeOuthdl_Ptr;
FW_ROM_BSS_SECTION  _MailboxINTHDL_Ptr                    MailboxINTHDL_Ptr;
FW_ROM_BSS_SECTION  _MailboxINTHDL_Patch_Ptr              MailboxINTHDL_Patch_Ptr;
FW_ROM_BSS_SECTION  _BTNullSetting_Ptr                    BTNullSetting_Ptr;
FW_ROM_BSS_SECTION  _BTRoleChangeDefaultSetting_Ptr       BTRoleChangeDefaultSetting_Ptr;
//FW_ROM_BSS_SECTION  _InitBTypeTDMA_Ptr                              InitBTypeTDMA_Ptr;
FW_ROM_BSS_SECTION  _FlexibleExtensionDecision_Ptr        FlexibleExtensionDecision_Ptr;
FW_ROM_BSS_SECTION  _BT_TDMA_Slot_Operation_0and3_Ptr     BT_TDMA_Slot_Operation_0and3_Ptr;
FW_ROM_BSS_SECTION  _C2HBTInfo_Ptr                        C2HBTInfo_Ptr;

//3 Power save
FW_ROM_BSS_SECTION  _SetSmartPSTimer_Ptr                  SetSmartPSTimer_Ptr;
FW_ROM_BSS_SECTION  _SetPwrStateReg_Ptr                   SetPwrStateReg_Ptr;
FW_ROM_BSS_SECTION  _Change_PS_State_Ptr                  Change_PS_State_Ptr;
FW_ROM_BSS_SECTION  _ResetPSParm_Ptr                      ResetPSParm_Ptr;
FW_ROM_BSS_SECTION  _Legacy_PS_Setting_Ptr                Legacy_PS_Setting_Ptr;
FW_ROM_BSS_SECTION  _PSModeSetting_Ptr                    PSModeSetting_Ptr;
FW_ROM_BSS_SECTION  _PsOpenRF_Ptr                         PsOpenRF_Ptr;								        
FW_ROM_BSS_SECTION  _PsCloseRF_Ptr                        PsCloseRF_Ptr;
FW_ROM_BSS_SECTION  _PS_S2_Condition_Match_Ptr            PS_S2_Condition_Match_Ptr;
FW_ROM_BSS_SECTION  _PS_S4_Condition_Match_Ptr            PS_S4_Condition_Match_Ptr;
FW_ROM_BSS_SECTION  _WaitTxStateMachineOk_Ptr             WaitTxStateMachineOk_Ptr;
FW_ROM_BSS_SECTION  _WriteTxPause_Ptr                     WriteTxPause_Ptr;
FW_ROM_BSS_SECTION  _WriteTxPauseWithMask_Ptr             WriteTxPauseWithMask_Ptr;
//MEMTYPE_XDATA  _BTC_Pnty_Shift_Ptr                                       BTC_Pnty_Shift_Ptr;
FW_ROM_BSS_SECTION  _IssueCTS2Self_Ptr                    IssueCTS2Self_Ptr;
FW_ROM_BSS_SECTION  _ChkandChangePS_Ptr                   ChkandChangePS_Ptr;
FW_ROM_BSS_SECTION  _IssuePSPoll_Ptr                      IssuePSPoll_Ptr;
FW_ROM_BSS_SECTION  _ReadRxBuff_Ptr                       ReadRxBuff_Ptr;
FW_ROM_BSS_SECTION  _IssueQNull_Ptr                       IssueQNull_Ptr;
FW_ROM_BSS_SECTION  _PowerBitSetting_Ptr                  PowerBitSetting_Ptr;
FW_ROM_BSS_SECTION  _ChangeTransmiteRate_Ptr              ChangeTransmiteRate_Ptr;
FW_ROM_BSS_SECTION  _IssueNullData_Ptr                    IssueNullData_Ptr;
FW_ROM_BSS_SECTION  _ConfigListenBeaconPeriod_Ptr         ConfigListenBeaconPeriod_Ptr;
FW_ROM_BSS_SECTION  _PS_S2ToS0State_Ptr                   PS_S2ToS0State_Ptr;
FW_ROM_BSS_SECTION  _SetBcnEarlyAndTimeout_Ptr            SetBcnEarlyAndTimeout_Ptr;
FW_ROM_BSS_SECTION  _EnlargeBcnEarlyAndTimeout_Ptr        EnlargeBcnEarlyAndTimeout_Ptr;
FW_ROM_BSS_SECTION  _ResetBcnEarlyAdjustParm_Ptr          ResetBcnEarlyAdjustParm_Ptr;
FW_ROM_BSS_SECTION  _IssueRsvdPagePacketSetting_Ptr       IssueRsvdPagePacketSetting_Ptr;

//3 Gtimer
FW_ROM_BSS_SECTION  _InitGTimer32us_Ptr                   InitGTimer32us_Ptr;
FW_ROM_BSS_SECTION  _InitGTimer1ms_Ptr                    InitGTimer1ms_Ptr;

//3 Basic IO
FW_ROM_BSS_SECTION  _PollingReg_8721D_PTR                 PollingReg_8721D_Ptr;


//3 WoWLAN
FW_ROM_BSS_SECTION  _WakeUpHost_Ptr                       WakeUpHost_Ptr;
FW_ROM_BSS_SECTION  _UpdateIV_Ptr                         UpdateIV_Ptr;
FW_ROM_BSS_SECTION  _updateCheckSumVer2_Ptr               updateCheckSumVer2_Ptr;
FW_ROM_BSS_SECTION  _GetMACHeaderLen_Ptr                  GetMACHeaderLen_Ptr;
FW_ROM_BSS_SECTION  _GetIVLenAndSecurityType_Ptr          GetIVLenAndSecurityType_Ptr;

//3 Security entry
FW_ROM_BSS_SECTION _AesTkipIvFun_Ptr                      AesTkipIvFun_Ptr;
FW_ROM_BSS_SECTION _WepIvFun_Ptr                          WepIvFun_Ptr;
FW_ROM_BSS_SECTION  _OnWpa_offload_Ptr                    OnWpa_offload_Ptr;
FW_ROM_BSS_SECTION  _wpa_supplicant_process_1_of_2_Ptr    wpa_supplicant_process_1_of_2_Ptr;
FW_ROM_BSS_SECTION  _wpa_supplicant_decrypt_key_data_Ptr  wpa_supplicant_decrypt_key_data_Ptr;
FW_ROM_BSS_SECTION  _wpa_supplicant_send_2_of_2_Ptr       wpa_supplicant_send_2_of_2_Ptr;
FW_ROM_BSS_SECTION  _write_cam_wow_Ptr                    write_cam_wow_Ptr;
FW_ROM_BSS_SECTION  _WriteCAM_Ptr                         WriteCAM_Ptr;
FW_ROM_BSS_SECTION  _ReadCAM_Ptr                          ReadCAM_Ptr;
FW_ROM_BSS_SECTION  _ReadGTKData_Ptr                          ReadGTKData_Ptr;
FW_ROM_BSS_SECTION  _AppendTkipMIC_Ptr                    AppendTkipMIC_Ptr;
FW_ROM_BSS_SECTION  _RTmemcmpBackward_Ptr                    RTmemcmpBackward_Ptr;
FW_ROM_BSS_SECTION  _AesTkipIvCheck_Ptr                    AesTkipIvCheck_Ptr;
FW_ROM_BSS_SECTION  _WepIvCheck_Ptr                    WepIvCheck_Ptr;
FW_ROM_BSS_SECTION  _AssignIvToKeyRsc_Ptr                    AssignIvToKeyRsc_Ptr;
FW_ROM_BSS_SECTION  _AssignKeyRscToIV_Ptr                    AssignKeyRscToIV_Ptr;



//3 IPSEC
FW_ROM_BSS_SECTION         _getSumValue_8721D_PTR                       getSumValue_8721D_PTR;
FW_ROM_BSS_SECTION         _getCheckSum_8721D_PTR                       getCheckSum_8721D_PTR;

FW_ROM_BSS_SECTION         _IPSecDataIn_8721D_PTR                       IPSecDataIn_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecDataOut_8721D_PTR                      IPSecDataOut_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecFillDesrciptor_8721D_PTR               IPSecFillDesrciptor_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSWriteDescriptorIO_8721D_PTR              IPSWriteDescriptorIO_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecDesrciptor_8721D_PTR                   IPSecDesrciptor_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecWriteDataIOMode_8721D_PTR              IPSecWriteDataIOMode_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecGetDataIOMode_8721D_PTR                IPSecGetDataIOMode_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecSetOPMode_8721D_PTR                    IPSecSetOPMode_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecEngine_8721D_PTR                       IPSecEngine_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecSetExMemoryAddr_8721D_PTR              IPSecSetExMemoryAddr_8721D_PTR;
FW_ROM_BSS_SECTION         _IPSecSetDescAddr_8721D_PTR                  IPSecSetDescAddr_8721D_PTR;
FW_ROM_BSS_SECTION         _AesUnwrapIpsec_8721D_PTR                    AesUnwrapIpsec_8721D_PTR;
FW_ROM_BSS_SECTION         _AESUnwrapSwap_8721D_PTR                     AESUnwrapSwap_8721D_PTR;
FW_ROM_BSS_SECTION         _HmacSha1Ipsec_8721D_PTR                     HmacSha1Ipsec_8721D_PTR;
FW_ROM_BSS_SECTION         _HmcMd5Ipsec_8721D_PTR                       HmcMd5Ipsec_8721D_PTR;
FW_ROM_BSS_SECTION         _RC4Ipsec_8721D_PTR                          RC4Ipsec_8721D_PTR;
FW_ROM_BSS_SECTION         _TkipMicIpsec_8721D_PTR                      TkipMicIpsec_8721D_PTR;
FW_ROM_BSS_SECTION         _Aes128EncIpsec_8721D_PTR                    Aes128EncIpsec_8721D_PTR;
FW_ROM_BSS_SECTION         _Aes128DecIpsec_8721D_PTR                    Aes128DecIpsec_8721D_PTR;
FW_ROM_BSS_SECTION         _FillParameterIpsec_8721D_PTR                FillParameterIpsec_8721D_PTR;
FW_ROM_BSS_SECTION         _FillMichaelPacketInfo_8721D_PTR             FillMichaelPacketInfo_8721D_PTR;
/* AES operations */
FW_ROM_BSS_SECTION         _AESEncrypt128_8721D_PTR                    AESEncrypt128_8721D_PTR;
FW_ROM_BSS_SECTION         _AESDecrypt128_8721D_PTR                    AESDecrypt128_8721D_PTR;
FW_ROM_BSS_SECTION         _AES_KeyExpansion_1W_8721D_PTR              AES_KeyExpansion_1W_8721D_PTR;
FW_ROM_BSS_SECTION         _AES_CMAC_GenerateSubKey_1W_8721D_PTR       AES_CMAC_GenerateSubKey_1W_8721D_PTR;
FW_ROM_BSS_SECTION         _AES_CMAC_1W_8721D_PTR                      AES_CMAC_1W_8721D_PTR;
FW_ROM_BSS_SECTION         _AES_Encrypt_1W_8721D_PTR                   AES_Encrypt_1W_8721D_PTR;

//3 FWLBK
FW_ROM_BSS_SECTION     _FwlbkDbgPrtU16_8721D_PTR              FwlbkDbgPrtU16_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkLLTWrite_8721D_PTR               FwlbkLLTWrite_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkHWInitialLLT_8721D_PTR           FwlbkHWInitialLLT_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkMode_8721D_PTR                   FwlbkMode_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkPreparePkt_8721D_PTR             FwlbkPreparePkt_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeSel_8721D_PTR                FwlbkModeSel_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkCheckRxPayload_8721D_PTR         FwlbkCheckRxPayload_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkSetCAM_8721D_PTR                 FwlbkSetCAM_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkUpdateParam_8721D_PTR            FwlbkUpdateParam_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkMcuPollMgq_8721D_PTR             FwlbkMcuPollMgq_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeTxNoCheck_8721D_PTR          FwlbkModeTxNoCheck_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeTx_8721D_PTR                 FwlbkModeTx_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeMaclbk_8721D_PTR             FwlbkModeMaclbk_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeSecTest_8721D_PTR            FwlbkModeSecTest_8721D_PTR;
FW_ROM_BSS_SECTION     _FwlbkModeAFElbk_8721D_PTR             FwlbkModeAFElbk_8721D_PTR;

#else

//3 Task
//brian for function both called in isrhdl or not, add to rom if possible
//extern  _SendSignalCommon_Ptr               SendSignalCommon_Ptr;   
extern  _OSSendSignalCommon_Ptr               OSSendSignalCommon_Ptr;
extern  _ISRSendSignalCommon_Ptr              ISRSendSignalCommon_Ptr;

//3 RA
extern  _InitialRateUpdate_Ptr                InitialRateUpdate_Ptr;
extern  _InitRAInfo_rsvd_Ptr                  InitRAInfo_rsvd_Ptr;
extern  _InitialRateByRssi_Ptr                InitialRateByRssi_Ptr;
//extern  _TryDone_Ptr                                        TryDone_Ptr;
extern  _Set_arfr_Ptr                         Set_arfr_Ptr;
extern  _check_rate_Ptr                       check_rate_Ptr;
extern  _RateUp_search_RateMask_Ptr           RateUp_search_RateMask_Ptr; 
extern  _ArfrRefresh_Ptr                      ArfrRefresh_Ptr;
extern  _ODM_H2C_RADebugMonitor_Ptr           ODM_H2C_RADebugMonitor_Ptr;
//extern  _H2CHDL_SetRssiSingle_Ptr                    H2CHDL_SetRssiSingle_Ptr;

//3 H2C/C2H Cmd
extern  _H2CCmdFunc_Ptr                       H2CCmdFunc_Ptr;
extern  _C2HInQueue_Ptr                       C2HInQueue_Ptr;
extern  _C2HDeQueue_Ptr                       C2HDeQueue_Ptr;
extern  _IssueC2HPKT_Ptr                      IssueC2HPKT_Ptr;
extern  _FillC2HContant_Ptr                   FillC2HContant_Ptr;
extern  _C2HPKTRxDESC_Ptr                     C2HPKTRxDESC_Ptr;
extern  _H2CInQueue_Ptr                       H2CInQueue_Ptr;
extern  _H2CDeQueue_Ptr                       H2CDeQueue_Ptr;
extern  _PktBufAccessCtrl_Ptr                 PktBufAccessCtrl_Ptr;

//3 BT Coex
extern  _Write778_Ptr                         Write778_Ptr;
extern  _WL2BTMailbox_Ptr                     WL2BTMailbox_Ptr;
extern  _FillMailbox_Ptr                      FillMailbox_Ptr;
extern  _BT_TDMA_Slot_Operation_1and4_Ptr     BT_TDMA_Slot_Operation_1and4_Ptr;
extern  _BT_TDMA_Slot_Operation_2and5_Ptr     BT_TDMA_Slot_Operation_2and5_Ptr; 
extern  _AOAC_ANT_SWT_Ptr                     AOAC_ANT_SWT_Ptr;
extern  _InitBTCoexTimer_Ptr                  InitBTCoexTimer_Ptr;
extern  _WlanActCtrl_Ptr                      WlanActCtrl_Ptr;
extern  _ChangeCoexTable_Ptr                  ChangeCoexTable_Ptr;
extern  _C2HBTMpRpt_Ptr                       C2HBTMpRpt_Ptr;
//extern  _BTSCOeSCOIssueNullHDL_Ptr                BTSCOeSCOIssueNullHDL_Ptr;
extern  _BTRoleChangeTimerHDL_Ptr             BTRoleChangeTimerHDL_Ptr;
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
extern  _AntennaSwitchCtrl_Ptr                AntennaSwitchCtrl_Ptr;
#endif
extern  _InitBTypeTDMA2_Ptr                   InitBTypeTDMA2_Ptr;
extern  _BTypeTDMABCNEarly_Ptr                BTypeTDMABCNEarly_Ptr;
extern  _InitBTMailbox_Ptr                    InitBTMailbox_Ptr;
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
extern  _InitSCOTimer_Ptr                     InitSCOTimer_Ptr;
extern  _BTSCOeSCOAntSetup_Ptr                BTSCOeSCOAntSetup_Ptr;
extern  _SCO_eSCO_IssueCtrlData_Ptr           SCO_eSCO_IssueCtrlData_Ptr;
#endif
extern  _BTMailboxRetry_Ptr                   BTMailboxRetry_Ptr;
extern  _C2HBTLoopback_Ptr                    C2HBTLoopback_Ptr;
extern  _BackupAntennaSetting_Ptr             BackupAntennaSetting_Ptr;
extern  _BTypeTDMATBTThdl_Ptr                 BTypeTDMATBTThdl_Ptr;
//extern  _C2HBTMailBoxStatus_Ptr                       C2HBTMailBoxStatus_Ptr;
extern  _BTRoleChangeTBTTHDL_Ptr              BTRoleChangeTBTTHDL_Ptr;
extern  _IssueBTQNull_Ptr                     IssueBTQNull_Ptr;
extern  _DacSwing_Ptr                         DacSwing_Ptr;
extern  _BTypeTDMATimeOuthdl_Ptr              BTypeTDMATimeOuthdl_Ptr;
extern  _MailboxINTHDL_Ptr                    MailboxINTHDL_Ptr;
extern  _MailboxINTHDL_Patch_Ptr              MailboxINTHDL_Patch_Ptr;
extern  _BTNullSetting_Ptr                    BTNullSetting_Ptr;
extern  _BTRoleChangeDefaultSetting_Ptr       BTRoleChangeDefaultSetting_Ptr;
//extern  _InitBTypeTDMA_Ptr                              InitBTypeTDMA_Ptr;
extern  _FlexibleExtensionDecision_Ptr        FlexibleExtensionDecision_Ptr;
extern  _BT_TDMA_Slot_Operation_0and3_Ptr     BT_TDMA_Slot_Operation_0and3_Ptr;
extern  _C2HBTInfo_Ptr                        C2HBTInfo_Ptr;

//3 Power save
extern  _SetSmartPSTimer_Ptr                  SetSmartPSTimer_Ptr;
extern  _SetPwrStateReg_Ptr                   SetPwrStateReg_Ptr;
extern  _Change_PS_State_Ptr                  Change_PS_State_Ptr;
extern  _ResetPSParm_Ptr                      ResetPSParm_Ptr;
extern  _Legacy_PS_Setting_Ptr                Legacy_PS_Setting_Ptr;
extern  _PSModeSetting_Ptr                    PSModeSetting_Ptr;
extern  _PsOpenRF_Ptr                         PsOpenRF_Ptr;								        
extern  _PsCloseRF_Ptr                        PsCloseRF_Ptr;
extern  _PS_S2_Condition_Match_Ptr            PS_S2_Condition_Match_Ptr;
extern  _PS_S4_Condition_Match_Ptr            PS_S4_Condition_Match_Ptr;
extern  _WaitTxStateMachineOk_Ptr             WaitTxStateMachineOk_Ptr;
extern  _WriteTxPause_Ptr                     WriteTxPause_Ptr;
extern  _WriteTxPauseWithMask_Ptr             WriteTxPauseWithMask_Ptr;
//MEMTYPE_XDATA  _BTC_Pnty_Shift_Ptr                                       BTC_Pnty_Shift_Ptr;
extern  _IssueCTS2Self_Ptr                    IssueCTS2Self_Ptr;
extern  _ChkandChangePS_Ptr                   ChkandChangePS_Ptr;
extern  _IssuePSPoll_Ptr                      IssuePSPoll_Ptr;
extern  _ReadRxBuff_Ptr                       ReadRxBuff_Ptr;
extern  _IssueQNull_Ptr                       IssueQNull_Ptr;
extern  _PowerBitSetting_Ptr                  PowerBitSetting_Ptr;
extern  _ChangeTransmiteRate_Ptr              ChangeTransmiteRate_Ptr;
extern  _IssueNullData_Ptr                    IssueNullData_Ptr;
extern  _ConfigListenBeaconPeriod_Ptr         ConfigListenBeaconPeriod_Ptr;
extern  _PS_S2ToS0State_Ptr                   PS_S2ToS0State_Ptr;
extern  _SetBcnEarlyAndTimeout_Ptr            SetBcnEarlyAndTimeout_Ptr;
extern  _EnlargeBcnEarlyAndTimeout_Ptr        EnlargeBcnEarlyAndTimeout_Ptr;
extern  _ResetBcnEarlyAdjustParm_Ptr          ResetBcnEarlyAdjustParm_Ptr;
extern  _IssueRsvdPagePacketSetting_Ptr       IssueRsvdPagePacketSetting_Ptr;

//3 Gtimer
extern  _InitGTimer32us_Ptr                   InitGTimer32us_Ptr;
extern  _InitGTimer1ms_Ptr                    InitGTimer1ms_Ptr;

//3 Basic IO
extern  _PollingReg_8721D_PTR                 PollingReg_8721D_Ptr;


//3 WoWLAN
extern  _WakeUpHost_Ptr                       WakeUpHost_Ptr;
extern  _UpdateIV_Ptr                         UpdateIV_Ptr;
extern  _updateCheckSumVer2_Ptr               updateCheckSumVer2_Ptr;
extern  _GetMACHeaderLen_Ptr                  GetMACHeaderLen_Ptr;
extern  _GetIVLenAndSecurityType_Ptr          GetIVLenAndSecurityType_Ptr;

//3 Security entry
extern _AesTkipIvFun_Ptr                      AesTkipIvFun_Ptr;
extern _WepIvFun_Ptr                          WepIvFun_Ptr;
extern  _OnWpa_offload_Ptr                    OnWpa_offload_Ptr;
extern  _wpa_supplicant_process_1_of_2_Ptr    wpa_supplicant_process_1_of_2_Ptr;
extern  _wpa_supplicant_decrypt_key_data_Ptr  wpa_supplicant_decrypt_key_data_Ptr;
extern  _wpa_supplicant_send_2_of_2_Ptr       wpa_supplicant_send_2_of_2_Ptr;
extern  _write_cam_wow_Ptr                    write_cam_wow_Ptr;
extern  _WriteCAM_Ptr                         WriteCAM_Ptr;
extern  _ReadCAM_Ptr                          ReadCAM_Ptr;
extern  _ReadGTKData_Ptr                          ReadGTKData_Ptr;
extern  _AppendTkipMIC_Ptr                    AppendTkipMIC_Ptr;
extern  _RTmemcmpBackward_Ptr             RTmemcmpBackward_Ptr;
extern _AesTkipIvCheck_Ptr                     AesTkipIvCheck_Ptr; 
extern _WepIvCheck_Ptr                     WepIvCheck_Ptr; 
extern _AssignIvToKeyRsc_Ptr                     AssignIvToKeyRsc_Ptr; 
extern _AssignKeyRscToIV_Ptr                     AssignKeyRscToIV_Ptr; 



//3 IPSEC
extern         _getSumValue_8721D_PTR                       getSumValue_8721D_PTR;
extern         _getCheckSum_8721D_PTR                       getCheckSum_8721D_PTR;

extern         _IPSecDataIn_8721D_PTR                       IPSecDataIn_8721D_PTR;
extern         _IPSecDataOut_8721D_PTR                      IPSecDataOut_8721D_PTR;
extern         _IPSecFillDesrciptor_8721D_PTR               IPSecFillDesrciptor_8721D_PTR;
extern         _IPSWriteDescriptorIO_8721D_PTR              IPSWriteDescriptorIO_8721D_PTR;
extern         _IPSecDesrciptor_8721D_PTR                   IPSecDesrciptor_8721D_PTR;
extern         _IPSecWriteDataIOMode_8721D_PTR              IPSecWriteDataIOMode_8721D_PTR;
extern         _IPSecGetDataIOMode_8721D_PTR                IPSecGetDataIOMode_8721D_PTR;
extern         _IPSecSetOPMode_8721D_PTR                    IPSecSetOPMode_8721D_PTR;
extern         _IPSecEngine_8721D_PTR                       IPSecEngine_8721D_PTR;
extern         _IPSecSetExMemoryAddr_8721D_PTR              IPSecSetExMemoryAddr_8721D_PTR;
extern         _IPSecSetDescAddr_8721D_PTR                  IPSecSetDescAddr_8721D_PTR;
extern         _AesUnwrapIpsec_8721D_PTR                    AesUnwrapIpsec_8721D_PTR;
extern         _AESUnwrapSwap_8721D_PTR                     AESUnwrapSwap_8721D_PTR;
extern         _HmacSha1Ipsec_8721D_PTR                     HmacSha1Ipsec_8721D_PTR;
extern         _HmcMd5Ipsec_8721D_PTR                       HmcMd5Ipsec_8721D_PTR;
extern         _RC4Ipsec_8721D_PTR                          RC4Ipsec_8721D_PTR;
extern         _TkipMicIpsec_8721D_PTR                      TkipMicIpsec_8721D_PTR;
extern         _Aes128EncIpsec_8721D_PTR                    Aes128EncIpsec_8721D_PTR;
extern         _Aes128DecIpsec_8721D_PTR                    Aes128DecIpsec_8721D_PTR;
extern         _FillParameterIpsec_8721D_PTR                FillParameterIpsec_8721D_PTR;
extern         _FillMichaelPacketInfo_8721D_PTR             FillMichaelPacketInfo_8721D_PTR;
/* AES operations */
extern         _AESEncrypt128_8721D_PTR                    AESEncrypt128_8721D_PTR;
extern         _AESDecrypt128_8721D_PTR                    AESDecrypt128_8721D_PTR;
extern         _AES_KeyExpansion_1W_8721D_PTR              AES_KeyExpansion_1W_8721D_PTR;
extern         _AES_CMAC_GenerateSubKey_1W_8721D_PTR       AES_CMAC_GenerateSubKey_1W_8721D_PTR;
extern         _AES_CMAC_1W_8721D_PTR                      AES_CMAC_1W_8721D_PTR;
extern         _AES_Encrypt_1W_8721D_PTR                   AES_Encrypt_1W_8721D_PTR;

//3 FWLBK
extern     _FwlbkDbgPrtU16_8721D_PTR              FwlbkDbgPrtU16_8721D_PTR;
extern     _FwlbkLLTWrite_8721D_PTR               FwlbkLLTWrite_8721D_PTR;
extern     _FwlbkHWInitialLLT_8721D_PTR           FwlbkHWInitialLLT_8721D_PTR;
extern     _FwlbkMode_8721D_PTR                   FwlbkMode_8721D_PTR;
extern     _FwlbkPreparePkt_8721D_PTR             FwlbkPreparePkt_8721D_PTR;
extern     _FwlbkModeSel_8721D_PTR                FwlbkModeSel_8721D_PTR;
extern     _FwlbkCheckRxPayload_8721D_PTR         FwlbkCheckRxPayload_8721D_PTR;
extern     _FwlbkSetCAM_8721D_PTR                 FwlbkSetCAM_8721D_PTR;
extern     _FwlbkUpdateParam_8721D_PTR            FwlbkUpdateParam_8721D_PTR;
extern     _FwlbkMcuPollMgq_8721D_PTR             FwlbkMcuPollMgq_8721D_PTR;
extern     _FwlbkModeTxNoCheck_8721D_PTR          FwlbkModeTxNoCheck_8721D_PTR;
extern     _FwlbkModeTx_8721D_PTR                 FwlbkModeTx_8721D_PTR;
extern     _FwlbkModeMaclbk_8721D_PTR             FwlbkModeMaclbk_8721D_PTR;
extern     _FwlbkModeSecTest_8721D_PTR            FwlbkModeSecTest_8721D_PTR;
extern     _FwlbkModeAFElbk_8721D_PTR             FwlbkModeAFElbk_8721D_PTR;

#endif  //#ifdef    __RTL8721D_INIT_C__

/*--------------------Function declaration------------------------------*/

#endif //__RTL8721D_FUNCPTR_ROM_H__
