#ifndef __WLAN_REALWOW_V2_RAM_H__
#define __WLAN_REALWOW_V2_RAM_H__


/*--------------------Define --------------------------------------------*/
#define RWV2_ACK_PATTERN_SIZE       32
#define RWV2_WAKEUP_PATTERN_SIZE    32

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/


typedef struct _REALWOW_VER2_INFO_ {
    //download to rsvd page
    u16 Interval;   //unit: 1 ms
    u16 KAPktSize;
    u16 AckLostLimit;
    u16 AckPatternSize;
    u16 WakeupPatternSize;  

    //parameter
    u16 IPIdentifier;
    u8  NIC_IPv4[4];
    u16 NIC_UDPport;
    u16 AckLostCount;
    u8  AckPattern[RWV2_ACK_PATTERN_SIZE];
    u8  WakeupPattern[RWV2_WAKEUP_PATTERN_SIZE];
    u16 IntervalCounter;
    //u8  AckPatternNum;
    //u8  WakeupPatternNum;
    //u8  KAPktNum;    
    //u8  KARetryCount;
    //u8  KARetryLimit;
}REALWOW_VER2_INFO,*PREALWOW_VER2_INFO;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


void 
InitRealWoWLANVer2(
    void
) ;

void 
parsePatternInfo(
    void
) ;

void 
updateIPIDVer2(
    IN u16 offset
) ;

u8 
checkNICIPv4Ver2(
    IN u16 offset
) ;

u8 
checkNICUDPPortVer2(
    IN u16 offset
) ;



void 
getRealWoWVer2Info(
    void
) ;

extern u8 
OnAckPattern(
    IN u16 offset
)  ;

extern u8 
OnWakeUpPktVer2(
    IN u16 rx_offset
) ;


extern void 
issueKeepAlivePkt(
    void //IN u8 isRetry
) ;

extern void 
enableRealWowLanVer2(
    void
) ;

extern void 
disableRealWowLanVer2(
    void
) ;

#endif //#ifndef __WLAN_REALWOW_V2_RAM_H__

