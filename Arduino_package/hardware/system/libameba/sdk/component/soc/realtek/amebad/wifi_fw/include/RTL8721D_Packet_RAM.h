#ifndef __RTL8721D_PACKET_RAM_H__
#define __RTL8721D_PACKET_RAM_H__

/*--------------------Define --------------------------------------------*/
// retry limit
//#define RTY_LMT_NULLDATA            8
//#define RTY_LMT_QNULL               8
//#define RTY_LMT_NULL_DISCONNECTION  24
//#define RTY_LMT_BTQNULL             8
//#define RTY_LMT_FCSNULL             8
//#define RTY_LMT_NDPA                24
//#define RTY_LMT_ProbeRsq            24
//#define RTY_LMT_GTK                 24
//#define RTY_LMT_BEACON              24
//#define RTY_LMT_ARP                 24
//#define RTY_LMT_PSPOLL              24
//#define RTY_LMT_NDP                 24
//#define RTY_LMT_SCONULL             5
//#define RTY_LMT_CTS2SELF            0	//Alan ,use 0 is correct, suggest by Ausin-Lo
#define RTY_LMT_LTECOEXNULL         8       // alan 2014_0331
#define RTY_LMT_LTECOEXQNULL        3       // alan 2014_0331
#define RTY_LMT_DBGPKT              8

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

#define ISSUEPKT_TYPE_NULLDATA      1
#define ISSUEPKT_TYPE_FCSNULLDATA   2
#define ISSUEPKT_TYPE_BTQNULL       3


/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void
ChangeTransmiteRate_8721D_RAM(
    IN u16 offset
);

extern u8
GetOccupyTxIndCommon(
    void
) ;

extern BOOLEAN
CheckCPUMGQEmptyCommon(
    void
);

extern u8 
CheckTxPktBufOverFlowCommon(
    IN u16 offset,
    IN u8 page
) ;

extern u8 
CheckRxPktBufOverFlowCommon(
    IN u16 offset,
    IN u8 RxHighAddr
) ; 

#endif  //__RTL8721D_PACKET_RAM_H__

