#ifndef __HALCOM_SW_TIMER_H__
#define __HALCOM_SW_TIMER_H__


#if CONFIG_SW_TIMER

/*--------------------------Define -------------------------------------------*/



/*------------------------------Define Enum-----------------------------------*/

// we register sw timer in _RTL8723_SW_TIMER_LIST
// now maximun number is 10
typedef enum _SW_TIMER_NUM_ {
    ISSUE_RS_TIMER      = 0,
    CHECK_RA_TO_TIMER   = 1,
    DHCP_EXPIRE_TOTIMER = 2,
    SCAN_TIMER          = 3,
    SCAN_SWITCH_CHANNEL_TIMER = 4,
    test = 5,    
    ISSUE_KA_TIMER      = 6,
    CHECK_ACK_TO_TIMER  = 7,
    MAX_SW_TIMER
}SW_TIMER_NUM,*PSW_TIMER_NUM;

/*--------------------------Define MACRO--------------------------------------*/

#define SW_TIMER_UNIT                   0xA  // sw timer base unit : 10ms  

#define SWTIMER_AUTO_PERIODIC_MODE      1   //can be recount auto-matically till mode is disable
#define SWTIMER_NON_PERIODIC_MODE       0   //default mode, one time only, recounting requuires reload timerout value

#define SWTIMER_USED_GTIMER_CTRL        TIMER_TC0_CTRL  // sw timer used Gtimer3 
#define UNIT_PER_SECOND                 100



/*------------------------------Define Struct---------------------------------*/


typedef struct _TIMER_LIST_ {
    unsigned long       time_out;
    unsigned long       rst_value;
    unsigned char       mode;   //0: timer mode; 1: periodic auto-timer mode
}TIMER_LIST,*PTIMER_LIST;


typedef struct _SW_TIEMR_LIST {
    TIMER_LIST SWTimer;
    unsigned char bSWTimerValid;
}SW_TIEMR_LIST,PSW_TIEMR_LIST;


/*------------------------Export global variable------------------------------*/

//extern xdata SW_TIEMR_LIST SWTimerList[MAX_SW_TIMER];


/*------------------------------Function declaration--------------------------*/


// These are SW Timer APIs, called by user when use a SW timer
//extern void InitSWTimer(unsigned char ListID,unsigned char mode) ;

extern void 
InitSWTimer(
    void
) ; // Use HW timer to implement SW timer

extern void 
SWTimerFuncList(
    IN u8 ListID
) ;

extern void 
SWTimerCountDown(
    void
) ;

extern void 
StartSWTimer(
    IN u8 ListID,
    IN u32 timeout,
    IN u8 mode
);

extern void 
StopSWTimer(
    IN u8 ListID
);

extern void 
UpdateTimeout(
    IN u8 ListID,
    IN u32 timeout
);

extern void 
StopAllSWTimer(
    void
);

extern BOOLEAN 
CheckSWTimerIsIdle(
    void
);


#endif //#if __HALCOM_SW_TIMER_H__

#endif
