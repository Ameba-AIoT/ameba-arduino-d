#ifndef __RTL8721D_RCU_TASK_H__
#define __RTL8721D_RCU_TASK_H__

#define UNKNOW_RESULT 0
#define TX_NULL1_FAIL 1
#define TX_NULL1_SUCCESS 2

#define RF_CLOSED_STATE 0
#define RF_OPENED_STATE 1

extern void
Task_RCU_LPS_8721D(
    void
);

extern void
Task_RCU_Common_8721D(
    void
);

extern void
Task_RCU_32K_8721D(
    void
);
extern BOOLEAN
IssueNullData_RCU(
    IN  u8          bPowerBit,
    IN  u8          RtyLmt
);
#endif  //__RTL8721D_RCU_TASK_H__