#ifndef __HALCOM_SERIAL_IO_H__
#define __HALCOM_SERIAL_IO_H__


#define TXBUF_SIZE  256      /* DO NOT CHANGE */
#define RXBUF_SIZE  15      /* DO NOT CHANGE */

#define UART_LOG_CMD_BUFLEN     63     
#define	MAX_ARGV				16
#define UART_LOG_HISTORY_ENTRY  8

#define KB_ASCII_NUL    0x00
#define KB_ASCII_BS     0x08
#define KB_ASCII_TAB    0x09
#define KB_ASCII_LF     0x0A
#define KB_ASCII_CR     0x0D
#define KB_ASCII_ESC    0x1B
#define KB_ASCII_SP     0x20
#define KB_ASCII_BS_7F  0x7F
#define KB_ASCII_LBRKT  0x5B         //[


#define KB_SPACENO_TAB  1

#if 0
#define _LEVEL_ALWAYS_		1
#define _LEVEL_ERR_			2
#define _LEVEL_WARNING_	3
#define _LEVEL_NOTICE_		4
#define _LEVEL_INFO_		5
#define _LEVEL_DUMP_		6
#define _LEVEL_DEBUG_		7

#define _FW_FUNC_PS_          BIT0
#define _FW_FUNC_BT_            BIT1
#define _FW_FUNC_WOW_       BIT2
#define _FW_FUNC_MCC_            BIT3
#define _FW_FUNC_CMD_           BIT4
#define _FW_FUNC_INIT_          BIT5
#define _FW_FUNC_TEST_          BIT6
#define _FW_FUNC_OTHERS_     BIT7
#endif


#define CONSOLE_8721D()   DBG_8195A("RTL8721D>", 0)
#define RTK_PRINT(_comp, _level, fmt, arg) \
    do { \
            DBG_8195A(fmt,(long)arg); \
    }while(0);

//typedef u8 (*ECHOFUNC)(IN u8*,...);    //UART LOG echo-function type.

typedef struct {
        u8  BufCount;                           //record the input cmd char number.
        u8  UARTLogBuf[UART_LOG_CMD_BUFLEN];   //record the input command.
} FW_UART_LOG_BUF, *PFW_UART_LOG_BUF;

typedef struct {
        u8 historyNum;
        u8 wrPtr;
        u8 rdPtr;
        u8 historyBuf[UART_LOG_HISTORY_ENTRY][UART_LOG_CMD_BUFLEN+1];
}FW_HISTORY_BUF, *PFW_HISTORY_BUF;

typedef struct _FW_COMMAND_TABLE_ {
    const    u8* cmd;
    u8       ArgvCnt;
    void     (*func)(u16 argc, u8* argv[]);
    const    u8* msg;
}FW_COMMAND_TABLE, *PFW_COMMAND_TABLE;

typedef struct {
        u8  NewIdx;
        u8  SeeIdx;
        u8  RevdNo;
        u8  EscSTS;
        u8  ExecuteCmd;
        u8  ExecuteEsc;
        u8  Resvd;
        PFW_UART_LOG_BUF   pTmpLogBuf;    
        PFW_HISTORY_BUF    pTmpHistoryBuf;
        PFW_COMMAND_TABLE  pCmdTbl;
        u32 CmdTblSz;
} FW_UART_LOG_CTL, *PFW_UART_LOG_CTL;

//extern void uart_isr (void) large;
#if 0
extern void uart_initialize (void)large;

extern void uart_baudrate (unsigned long baudrate)large;
#endif

#if 0
extern char uart_putone (unsigned char c)large;

extern char
uart_getone(
    char *pch, 
    u32 timeout
)MEMMDL_LARGE;

extern void 
uart_clear_buf(
    VOID
)MEMMDL_LARGE;

extern u8
uart_buf_empty(
    void
)MEMMDL_LARGE;   

extern void printf(const char *fmt, ...) large;

extern int   vsprintf(char *buf, const char *fmt, const char *dp)large;
#endif

extern int
_strcmp(
    const char *cs, 
    const char *ct
);

extern u32
Strtoul(
    IN  const u8 *nptr,
    IN  u8 **endptr,
    IN  u32 base
);

extern VOID
FwRtlConsolInit(
    IN  u16     TBLSz,
    IN  VOID    *pTBL    
);


#endif // #ifndef __HALCOM_SERIAL_IO_H__
