
#ifndef __RTL8721D_PACKET_H__
#define __RTL8721D_PACKET_H__

#ifndef __ASSEMBLY__
/*--------------------------Define -------------------------------------------*/

// retry limit
#define RTY_LMT_NULLDATA            8
#define RTY_LMT_QNULL               8
#define RTY_LMT_NULL_DISCONNECTION  24
#define RTY_LMT_BTQNULL             8
#define RTY_LMT_FCSNULL             8
#define RTY_LMT_NDPA                24
#define RTY_LMT_ProbeRsq            24
#define RTY_LMT_GTK                 24
#define RTY_LMT_BEACON              24
#define RTY_LMT_ARP                 24
#define RTY_LMT_PSPOLL              24
#define RTY_LMT_NDP                 24
#define RTY_LMT_SCONULL             5
#define RTY_LMT_CTS2SELF            0	//Alan ,use 0 is correct, suggest by Ausin-Lo

//issue rsvdpage marco
#define _ISSUE_BTQNULL              1
#define _ISSUE_DBGPKT_              2
#define _ISSUE_FCSNULL_             3
#define _ISSUE_LTEQNULL_            4
#define _ISSUE_NDPA_                5
#define _ISSUE_PROBERSP_            6
#define _ISSUE_PROBEREQ_            7
#define _ISSUE_GTKRSP_              8
#define _ISSUE_PSPOLL_              9
#define _ISSUE_NULL_                10
#define _ISSUE_QNULL_               11
#define _ISSUE_CTS2SELF_            12
#define _ISSUE_REALWOW_KA_          13
#define _ISSUE_RS_                  14
#define _ISSUE_BUBBLE_              15
#define _ISSUE_ARP_                 16
#define _ISSUE_NA_                  17

/*------------------------------Define Enum-----------------------------------*/


/*--------------------------Define MACRO--------------------------------------*/
#define RATE_CCK_1M         0x00 
#define RATE_CCK_2M         0x01
#define RATE_CCK_5M         0x02
#define RATE_CCK_11M        0x03
#define RATE_OFDM_6M        0x04
#define RATE_OFDM_9M        0x05
#define RATE_OFDM_12M       0x06 
#define RATE_OFDM_18M       0x07
#define RATE_OFDM_24M       0x08
#define RATE_OFDM_36M       0x09
#define RATE_OFDM_48M       0x0A
#define RATE_OFDM_54M       0x0B
#define RATE_HT_MCS0        0x0C 
#define RATE_HT_MCS1        0x0D
#define RATE_HT_MCS2        0x0E
#define RATE_HT_MCS3        0x0F
#define RATE_HT_MCS4        0x10
#define RATE_HT_MCS5        0x11    
#define RATE_HT_MCS6        0x12
#define RATE_HT_MCS7        0x13
#define RATE_HT_MCS8        0x14    
#define RATE_HT_MCS9        0x15
#define RATE_HT_MCS10       0x16
#define RATE_HT_MCS11       0x17
#define RATE_HT_MCS12       0x18
#define RATE_HT_MCS13       0x19
#define RATE_HT_MCS14       0x1A
#define RATE_HT_MCS15       0x1B
#define RATE_HT_MCS16       0x1C 
#define RATE_HT_MCS17       0x1D
#define RATE_HT_MCS18       0x1E
#define RATE_HT_MCS19       0x1F
#define RATE_HT_MCS20       0x20
#define RATE_HT_MCS21       0x21    
#define RATE_HT_MCS22       0x22
#define RATE_HT_MCS23       0x23
#define RATE_HT_MCS24       0x24    
#define RATE_HT_MCS25       0x25
#define RATE_HT_MCS26       0x26 
#define RATE_HT_MCS27       0x27
#define RATE_HT_MCS28       0x28
#define RATE_HT_MCS29       0x29
#define RATE_HT_MCS30       0x2A
#define RATE_HT_MCS31       0x2B
#define RATE_VHT1SS_MCS0    0x2C 
#define RATE_VHT1SS_MCS1    0x2D
#define RATE_VHT1SS_MCS2    0x2E
#define RATE_VHT1SS_MCS3    0x2F
#define RATE_VHT1SS_MCS4    0x30
#define RATE_VHT1SS_MCS5    0x31    
#define RATE_VHT1SS_MCS6    0x32
#define RATE_VHT1SS_MCS7    0x33
#define RATE_VHT1SS_MCS8    0x34    
#define RATE_VHT1SS_MCS9    0x35
#define RATE_VHT2SS_MCS0    0x36 
#define RATE_VHT2SS_MCS1    0x37
#define RATE_VHT2SS_MCS2    0x38
#define RATE_VHT2SS_MCS3    0x39
#define RATE_VHT2SS_MCS4    0x3A
#define RATE_VHT2SS_MCS5    0x3B    
#define RATE_VHT2SS_MCS6    0x3C
#define RATE_VHT2SS_MCS7    0x3D
#define RATE_VHT2SS_MCS8    0x3E    
#define RATE_VHT2SS_MCS9    0x3F
#define RATE_VHT3SS_MCS0    0x40 
#define RATE_VHT3SS_MCS1    0x41
#define RATE_VHT3SS_MCS2    0x42
#define RATE_VHT3SS_MCS3    0x43
#define RATE_VHT3SS_MCS4    0x44
#define RATE_VHT3SS_MCS5    0x45    
#define RATE_VHT3SS_MCS6    0x46
#define RATE_VHT3SS_MCS7    0x47
#define RATE_VHT3SS_MCS8    0x48    
#define RATE_VHT3SS_MCS9    0x49
#define RATE_VHT4SS_MCS0    0x4A 
#define RATE_VHT4SS_MCS1    0x4B
#define RATE_VHT4SS_MCS2    0x4C
#define RATE_VHT4SS_MCS3    0x4D
#define RATE_VHT4SS_MCS4    0x4E
#define RATE_VHT4SS_MCS5    0x4F    
#define RATE_VHT4SS_MCS6    0x50
#define RATE_VHT4SS_MCS7    0x51
#define RATE_VHT4SS_MCS8    0x52    
#define RATE_VHT4SS_MCS9    0x53

/*------------------------------Define Struct---------------------------------*/

typedef struct  _RsvdPageLoc_ {
    u8      loc_probersp;
    u8      loc_pspoll;
    u8      loc_nulldata;
    u8      loc_QosNull;
    u8      loc_BT_QoSNull;
    u8      loc_CTS2Self;		//alan 2013_10_08
    u8      loc_LTECOEX_QoSNull;        //alan 2014_0331
}RsvdPageLoc, *PRsvdPageLoc;

/*------------------------Export global variable------------------------------*/

/*------------------------------Function declaration--------------------------*/

extern BOOLEAN
CheckCPUMGQEmpty_8721D_ROM(
    void
);

#if 0
u16
IssueRsvdPagePacketSetting_CallPtr(
    IN  u8          PageNum,
    IN  BOOLEAN     bHwSEQEn,
    IN  u8          RtyLmt,
    IN  u8          TxInd
) ;
#endif

extern u16
IssueRsvdPagePacketSetting_8721D_ROM(
    IN  u8          PageNum,
    IN  BOOLEAN     bHwSEQEn,
    IN  u8          RtyLmt,
    IN  u8          TxInd    
) ;


extern u8 
CheckTxPktBufOverFlow_8721D_ROM(
    IN u16 offset,
    IN u8 page
) ;

extern u8 
CheckRxPktBufOverFlow_8721D_ROM(
    IN u16 offset,
    IN u8 RxHighAddr
) ; 


extern void
PowerBitSetting_8721D_ROM(
    IN  u8          bPowerBit,
    IN  u16         offset
);

extern void
IssuePSPoll_8721D_ROM(
    void
);

void 
CTS2SelfSetting_8721D_ROM(
    IN  u16 offset, 
    IN  u16 NAV
) ;

#define CTS2SelfSetting CTS2SelfSetting_8721D_ROM

extern void
IssueQNull_8721D_ROM(
    IN  u8  bPowerBit
);

extern void
IssueCTS2Self_8721D_ROM(
    IN  u8 location,
    IN	u16	NAV
);

extern u8
SetTxbufAddrFromPageNum_8721D_ROM(
    IN u8 PageNum
);

extern u8
ReadRxBuff_8721D_ROM(
    IN u16 headaddr,
    IN u8  offset
) ;

extern void
ChangeTransmiteRate_8721D_ROM(
    IN u16 offset
) ;

extern void 
ChkandChangePS_8721D_ROM(
    IN  u8     bPowerBit
);



#endif  //#ifndef __ASSEMBLY__

#endif  //__RTL8721DPACKET_H__

