#ifndef __RTL8721D_TXREPORT_H__
#define __RTL8721D_TXREPORT_H__

#ifndef __ASSEMBLY__


/*--------------------------Define -------------------------------------------*/
#if 1
//MACID Number = 128
//XDATA MAP
//0x8000: SPERPT:128byte
//0x8080: BCNRPT:128byte
//0x8100: TXRPT:2K
//0x8900: Rate Mask:1K
//0x8D00: SRAM Xdata
//0xAF00: ROM Xdata

//#define SPERPT_START_ADDR   0x8000
#define SPERPT_SIZE         8
#define SPERPT_NUM          16

//#define BCNRPT_START_ADDR   0x8080
#define BCNRPT_SIZE         16
#define BCNRPT_NUM          8

//#define TXRPT_START_ADDR    0x8100
#define TXRPT_SIZE          16
#define PAUSE_TXRPT_TIME    (12)

//#define RAMask_START_ADDR   0x8200
#define RAMASK_SIZE         8

//#define NANBCNRPT_START_ADDR    0x8280
#define NANBCNRPT_SIZE      0x80
#else
//MACID Number = 64
//XDATA MAP
//0x8000: SPERPT
//0x8080: BCNRPT
//0x8100: TXRPT
//0x8800: Rate Mask
//0x8A00: SRAM Xdata
//0x9F00: ROM Xdata

#define MACID_NUM           64

#define SPERPT_START_ADDR   0x8000
#define SPERPT_SIZE         8
#define SPERPT_NUM          8

#define BCNRPT_START_ADDR   0x8080
#define BCNRPT_SIZE         8
#define BCNRPT_NUM          8

#define TXRPT_START_ADDR    0x8100
#define TXRPT_SIZE          16
#define PAUSE_TXRPT_TIME    (12)

#define RAMask_START_ADDR   0x8800
#define RAMASK_SIZE         8
#endif

/*------------------------------Define Enum-----------------------------------*/


/*--------------------------Define MACRO--------------------------------------*/

//offset 0
#define TXRPT_DATARATE      (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6)
#define TXRPT_SGI           BIT7
//offset 1
#define TXRPT_PWRSTS        (BIT0|BIT1|BIT2)
#define TXRPT_TRYNESSCNT    (BIT3|BIT4|BIT5|BIT6)
#define TXRPT_TRYRATE       BIT7
//offset 2
#define TXRPT_TRYRESULT     BIT6
#define TXRPT_TRYFINISH     BIT7
//offset 3
#define TXRPT_PAUSERPT      BIT6
#define TXRPT_RESETRPT      BIT7
//offset 4

//offset 5
#define TXRPT_BW            (BIT0|BIT1)
#define TXRPT_PKTDROP       BIT2
/*------------------------------Define Struct---------------------------------*/

/*------------------------Export global variable------------------------------*/
extern MEMTYPE_XDATA u8  volatile CCXRPT[8];
extern MEMTYPE_XDATA u8  volatile CCX_IDX;

/*------------------------------Function declaration--------------------------*/

extern void
PRT_XDATA(
    void
);

extern void
ResetTxrpt_8721D_ROM(
    IN u8   macid,
    IN u8   reset_var
);

extern void
PsuseTxrpt_8721D_ROM(
    IN u8   macid
);

extern void
DbgWriteTxrptByte(
    void
);

extern void
DbgPrintTxrpt(
    IN u8   macid
);

extern void
DbgPrintSpeRpt(
    IN u8   macid
);

extern void 
DbgPrintBcnRpt(
    IN u8   idx
);

extern void
GetTxrptStatistic_8721D_ROM(
    u8  reset_var
);

extern void 
InitBcnRpt(
    void
);

#endif  //#ifndef __ASSEMBLY__

#endif  //#ifndef   __RTL8721D_TXREPORT_H__


