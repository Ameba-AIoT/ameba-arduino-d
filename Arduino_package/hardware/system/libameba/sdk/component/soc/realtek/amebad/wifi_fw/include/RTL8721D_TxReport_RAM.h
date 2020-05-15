#ifndef __RTL8721D_TXREPORT_RAM_H__
#define __RTL8721D_TXREPORT_RAM_H__


/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void
PRT_XDATA(
    void
);

extern void
ResetTxrpt(
    IN u8   macid,
    IN u8   reset_var
);

extern void
PsuseTxrpt(
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
GetTxrptStatistic(
    u8  reset_var
);

extern void 
InitBcnRpt(
    void
);

#if 0
extern u8
CheckMaxMacidNum(
    void
);
#endif

#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)

extern void
GetTxrptStatistic_8721D_RAM(
    u8  reset_var
);


#endif

#endif  //#ifndef   __RTL8721D_TXREPORT_RAM_H__


