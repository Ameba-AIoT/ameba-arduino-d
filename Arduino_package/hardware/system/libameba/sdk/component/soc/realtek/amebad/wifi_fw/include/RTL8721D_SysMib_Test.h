#ifndef __HALCOM_SYSMIB_TEST_H__
#define __HALCOM_SYSMIB_TEST_H__

//1.) Xdata Section
typedef struct _mib_info_test_ {

    //3 HAL
    HAL_DATA_8721D          IntHdl;

    //3 SECURITY
    IPSEC_FUNC              IpSec;

    u8              FEISRBitMap[4];
    u8              FWISRBitMap[4];
    u8              FTISRBitMap[4];
    union{
        u32             TSFOnBcnErly1;
        u8              TSFOnBcnErly1Arr[4];
    }TSFOnBcnErly1Union;
    union{
        u32             TSFOnBcnErly2;
        u8              TSFOnBcnErly2Arr[4];
    }TSFOnBcnErly2Union;
    union{
        u32             TSFOnTBTT1;
        u8              TSFOnTBTT1Arr[4];
    }TSFOnTBTT1Union;
    union{
        u32             TSFOnTBTT2;
        u8              TSFOnTBTT2Arr[4];
    }TSFOnTBTT2Union;    
    union{
        u32             TSFOnDWEarly;
        u8              TSFOnDWEarlyArr[4];
    }TSFOnDWEarlyUnion;
    union{
        u32             TSFOnDWEnd;
        u8              TSFOnDWEndArr[4];
    }TSFOnDWEndUnion;     
    union{
        u32             TSFOnP2POn;
        u8              TSFOnP2POnArr[4];
    }TSFOnP2POnUnion;
    union{
        u32             TSFOnP2POff;
        u8              TSFOnP2POffArr[4];
    }TSFOnP2POffUnion;    
    u32             DeltaTSF1;
    u32             DeltaTSF2;
    u8              MbssidCnt;
    u8              ErlyAdjCnt;
    u8              TestItem;

    u32             baseAddr;
}mib_info_test, *Pmib_info_test;


#ifdef __TEST_C__
MEMTYPE_XDATA mib_info_test SysMibTest;
#else
extern MEMTYPE_XDATA mib_info_test SysMibTest;    
#endif

#endif  //#ifndef __HALCOM_SYSMIB_TEST_H__