#ifndef __RTL8721D_RCU_NORMAL_DBG_H__
#define __RTL8721D_RCU_NORMAL_DBG_H__

void
RegCmdDump(
    u16 stAddr,
    u16 endAddr,
    u8 type
);

void
CmdFW(
    IN u16 argc,
    IN u8  *argv[]
);

extern void 
CmdRegRW(
    IN  u16 argc,
    IN  u8  *argv[]
);

void
REG_CMD_Dump(const char *argv[]);

extern void
FwDbgCmdHelp(
    IN  u8 argc,
    IN  u8  *argv[]
);

VOID
NorFWdbgInit(void);

void
CmdBufRW(
    IN  u8 argc,
    IN  u8  *argv[]
);

void 
NorDbgBuf(const char *argv[]);

void
CmdTxrptRW(
    IN  u8 argc,
    IN  u8  *argv[]
);

void 
NorDbgTxrpt(const char *argv[]);
#endif
