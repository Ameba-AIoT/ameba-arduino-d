#ifndef __RTL8723D_INT_TEST_RAM_H__
#define __RTL8723D_INT_TEST_RAM_H__

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

/*------------------------------Define Struct---------------------------------*/


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
    void
);

extern void
FwIntInit(
    void
);

extern void 
PrepareIntPkt(
    u8 pktType
) ;


#endif  //#ifndef   __ASSEMBLY__


#endif  //__RTL8723D_INT_TEST_RAM_H__
