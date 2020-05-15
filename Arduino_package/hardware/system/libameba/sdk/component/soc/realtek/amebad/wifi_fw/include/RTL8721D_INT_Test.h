#ifndef __RTL8192F_INT_TEST_H__
#define __RTL8192F_INT_TEST_H__

/*--------------------------Define -------------------------------------------*/


//Tx page 
#define TXPKTBUF_PGBNDY_INTTEST               0x60//Reserved 4k bytes, 2k for inttest packet, 2k for wmac buffer
#define TXPKTBUF_BNDY_INTTEST                 (PAGESIZE_TX*TXPKTBUF_PGBNDY_INTTEST)


#define SIZE_WLAN_HEAD                      24



/*------------------------------Define Enum-----------------------------------*/

enum{
    TYPE_MORE_DATA=0,
    TYPE_NO_MORE_DATA=1,
    TYPE_BCM_MORE_DATA=2,
    TYPE_BCM_NO_MORE_DATA=3,
    TYPE_QOS_MORE_DATA=4,
    TYPE_QOS_NO_MORE_DATA=5,
    TYPE_QOS_EOSP=6,
    TYPE_TRIGGER=7,
    TYPE_NULL0=8,
    TYPE_NULL1=9,
    TYPE_CCX=10,
    TYPE_BEACON=11,
    TYPE_SYNC_BCN=12,
    TYPE_MAGIC=13,
    TYPE_TRY=14,
    TYPE_MACID=15,
    TYPE_FTMREQ=16,
    TYPE_FTM=17,
    TYPE_NDPA=18,
    TYPE_MAX,
};

enum{
    ITEM_PKT_INT=0,
    ITEM_P0_TX_BCN=1,
    ITEM_P1_TX_BCN=2,
    ITEM_P2_TX_BCN=3,
    ITEM_MBSSID_INT=4,
    ITEM_BCN_ERLY_ADJ=5,
    ITEM_BCN_DMA_ERR=6,
    ITEM_HWRPT_TEST=7,
    
    ITEM_NULL,
};
/*--------------------------Define MACRO--------------------------------------*/

//Basic ISR function
//#define UpdateISRSys            UpdateISRSys_8721D
#define UpdateISRWlan           UpdateISRWlan_8721D
#define UpdateISRFT             UpdateISRFT_8721D

#define ClrAllFWUsedIMR         ClrAllFWUsedIMR_8721D_ROM
#define ClrAllFWUsedISR         ClrAllFWUsedISR_8721D_ROM
#define MCUINTEnable            MCUINTEnable_8721D_ROM

/*------------------------------Define Struct---------------------------------*/

typedef struct _STR_TYPE_ {
    const   u8* str;
}STR_TYPE, *PSTR_TYPE;

/*------------------------Export global variable------------------------------*/
#ifndef __ASSEMBLY__

extern MEMTYPE_CODE u8  MAC_HEADER[];
extern MEMTYPE_CODE u8  INTTEST_TXDESC[];

/*------------------------------Function declaration--------------------------*/

extern void 
FwIntTest(
    void
) ;

extern void
FwIntTestExt(
    const char *argv[]
);

extern void
FwIntInit(
    void
);

extern void 
PrepareIntPkt(
    u8 pktType
) ;
extern void PrintISRBitMap(void);
extern void ClrISRBitMap(void);

extern void    
WLAN_INT_HDL_TEST(
        void
); 

extern void    
FT_INT_HDL_TEST(
    void
);

#if 0
extern void 
Init32K(
    void
) MEMMDL_LARGE;

extern void 
Init32kCAL(
    void
) MEMMDL_LARGE;
#endif

#endif  //#ifndef   __ASSEMBLY__


#endif  //__RTL8192F_INT_TEST_H__