#ifndef __RTL8721D_NAN_RAM_H__
#define __RTL8721D_NAN_RAM_H__

#ifndef __ASSEMBLY__
/*--------------------Define -------------------------------------------*/

#define START_LOC_NAN_ATTRI         82  //attri offset+tx desc len
#define START_LOC_A3                56      //A3 offset + tx desc len

#define NAN_ATTRI_ID0               0
#define NAN_ATTRI_ID1               1

#define LEN_NAN_ATTRI_ID0       2
#define LEN_NAN_ATTRI_ID1       13

typedef struct _AMR_INFO_ {
    u8 AnchorMasterRank[8];
    u8 HopCount;
    u8 AMBTT[4];
}AMR_INFO;

typedef struct _NAN_PARM_ {
    u8      Enable:1;
    u8      CurrentRole:1;
    u8      FwTxSyncBcn:1;
    u8      FwTxDiscBcn:1;
    u8      Rsvd:4;
    u8      LocSyncBcn;
    u8      LocDiscBcn;
    AMR_INFO CurrentAMR;
    AMR_INFO LastAMR;
    AMR_INFO BcnAMR;
    AMR_INFO OwnAMR;
    
}NAN_PARM;

/*--------------------Define Enum---------------------------------------*/

#define NAN_DEVICE                  0
#define NAN_ANCHOR_MASTER       1

enum{
    NAN_EVENT_DW_EARLY  = 0,
    NAN_EVENT_DW_END  = 1,
    NAN_EVENT_EQ_EARLY  = 2,
    NAN_EVENT_EQ_OK  = 3,
    NAN_EVENT_RX_SYNC_BCN = 4,
};

enum{
    BCN_TYPE_DISC = 0,
    BCN_TYPE_SYNC = 1,
};    


/*--------------------Export global variable----------------------------*/



/*--------------------Function declaration------------------------------*/

extern void
C2H_NAN_Event(
    u8  event
) ;

extern void
TSFSync_Decision(
    void
);

extern BOOLEAN
IsAMRLarger(
    u8* pFirstAMR,
    u8* pSecondAMR
);

extern BOOLEAN
IsAMREqual(
    u8* pFirstAMR,
    u8* pSecondAMR
);

extern BOOLEAN
IsAMBTTLarger(
    u8* pFirstAMBTT,
    u8* pSecondAMBTT
);

extern BOOLEAN
IsAddrEqual(
    u8* pFirstAMR,
    u8* pSecondAMR
);

extern void
SyncTSF(
    BOOLEAN bSync
);

extern void
UpdateNANRole(
    void
);

extern void
UpdateCurrAMR(
    void
);

extern void
UpdateLastAMR(
    void
);

extern void
UpdateOwnAMR(
    void
);

extern void
UpdateRxBcnAMR(
    u8 entryID
);

extern void
NANRptSetting(
    BOOLEAN bNANPsr
);

extern void
UpdateNANBcnInfo(
    u8 bcnType
);

#endif      //#ifndef __ASSEMBLY__

#endif      //#ifndef __RTL8721D_NAN_RAM_H__
