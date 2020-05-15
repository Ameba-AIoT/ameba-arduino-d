#ifndef __HALCOM_DEBUG_RAM_H__
#define __HALCOM_DEBUG_RAM_H__


/*--------------------------Define -------------------------------------------*/

//debug trace component(16bit)
#define DBG_TRACE_COMP_RA     BIT0
#define DBG_TRACE_COMP_IQK     BIT1
#define DBG_TRACE_COMP_INIT     BIT2


// 5.) Power saving debug
#define REG_PS_32K              REG_DBG_DWORD_0  //1B8

// 6.) WoWlan wakeup reason
#define REG_WoW_REASON          REG_DBG_BYTE_5   //1C7

#define Rx_Pairwisekey          0x01
#define Rx_GTK                  0x02
#define Rx_Fourway_Handshake    0x03
#define Rx_DisAssoc             0x04
#define Rx_DeAuth               0x08
#define Rx_ArpRequest           0x09
#define Rx_NS                       0x0a
#define RX_EAPREQ_IDENTIFY 0x0b
#define FWDecisionDisconnect    0x10
#define Rx_MagicPkt             0x21
#define Rx_UnicastPkt           0x22
#define Rx_PatternPkt           0x23
#define RTD3_SSID_Match         0x24
#define HW_RX_WAKEUP_PATTERN    0x25
#define RX_RealWoW_V2_WakeupPkt 0x30
#define RX_RealWoW_V2_AckLost   0x31
#define Enable_Fail_DMAIdle     0x40
#define Enable_Fail_DMAPause    0x41
#define RTime_Fail_DMAIdle      0x42
#define RTime_Fail_DMAPause     0x43
#define RxShift_Error       0x44    //brian_zhang,20160405
#define BT_Wakeup_Host          0x45
#define NLO_SSID_Match          0x55  //brian_zhang,20170815
#define AP_Offload_WakeUp       0x66    //alan,2013_11_28
#define CLK_32K_UNLOCK          0xFD
#define CLK_32K_LOCK            0xFE



// 7.) Print Debug Message
#define REG_8051_PRT_MSG0       REG_DBG_DWORD_4    //2F0
#define REG_8051_PRT_MSG1       REG_DBG_DWORD_5    //2F4
#define REG_8051_PRT_MSG2       REG_DBG_DWORD_6    //2F8
#define REG_8051_PRT_MSG3       REG_DBG_DWORD_7    //2FC

// 8.) Driver FW Interface
#define REG_DRV_FW_INTF_ADDR_L  REG_DBG_DWORD_1    //1BC
#define REG_DRV_FW_INTF_ADDR_H  REG_DBG_DWORD_1+1  //1BD
#define REG_DRV_FW_INTF_CTRL    REG_DBG_DWORD_1+3  //1BF
#define REG_DRV_FW_DATA0        REG_DBG_DWORD_4    //2F0
#define REG_DRV_FW_DATA1        REG_DBG_DWORD_5    //2F4
#define REG_DRV_FW_DATA2        REG_DBG_DWORD_6    //2F8
#define REG_DRV_FW_DATA3        REG_DBG_DWORD_7    //2FC

// 9.) DBG CTRL
#define REG_DBG_CTRL_MACID      REG_MEM_CTRL0    //8C
#define REG_DBG_NUMPRT          REG_MEM_CTRL0+1  //8D
#define REG_DBG_NUMCTRL         REG_MEM_CTRL0+2  //8E
#define REG_DBG_CTRL            REG_MEM_CTRL0+3  //8F


// 10.) WoWLAN mode, ant external or internla switch flag
    //Alan 2013_1212
    //use for record internal/external antenna and single/dual antenna
    //bit[0]:  record internal/external antenna 
    //bit[4]:  record single/dual antenna


#define REG_AntSwitchOutSide    REG_DBG_BYTE_22


//REG_DBG_NUMPRT
#define DBGCTRL_DISABLE         0x00
#define DBGCTRL_PRT_TXRPT       0x01
#define DBGCTRL_PRT_SPERPT      0x02
#define DBGCTRL_PRT_XDATA       0x03
#define DBGCTRL_PRT_RA_info     0x04
#define DBGCTRL_PRT_RA_CNT      0x05
#define DBGCTRL_PRT_RA_CNT1     0x06
#define DBGCTRL_PRT_RA_CNT2     0x07
#define DBGCTRL_PRT_PS          0x08
#define DBGCTRL_PRT_BT          0x09



//REG_DBG_NUMCTRL
#define DBGCTRL_CollectRPT      0x01
#define DBGCTRL_SetPause        0x02
#define DBGCTRL_ClrPause        0x03
#define DBGCTRL_ResetRPT        0x04
#define DBGCTRL_W_TXRPT         0x05


//REG_DBG_CTRL
#define DBGFUN_STEP_ENABLE      BIT7
#define DBGFUN_PRT_ENABLE       BIT6
#define DBGFUN_CTRL_ENABLE      BIT5
#define DBGFUN_PS_STATE_EN      BIT4 //report to 0x88[15:0]

#define DBGFUN_NOSTOP           BIT0

#define TXBUF_TEST_PKT_LEN      0xC2
#define PKT_TXDESC_OFFSET       0x0
#define PKT_HEADER_OFFSET       0x28
#define PKT_PAYLOAD_OFFSET      0x42

#define PKT_HAED_SEQ_OFFSET     PKT_HEADER_OFFSET+22
#define PKT_TXD_SEQ_OFFSET      37


#define IC_CHIPCUT_A    0x00
#define IC_CHIPCUT_B    0x01
#define IC_CHIPCUT_C    0x02
#define IC_CHIPCUT_D    0x03



//definition for RuntimePM reason
#define RUNTIME_PM_BEACON    BIT0    

//Debug packet
#define DBGPKT_PAGENUM              0XF8        //for 8723B only
#define LENGTH_DBGPKT_BUFF          64

/*------------------------------Define Enum-----------------------------------*/
typedef enum _DEBUG_SEL_
{
    DBG_SEL_C2HPKT                  = BIT0,
    DBG_SEL_UART                    = BIT1,
    DBG_SEL_SNIFFER                 = BIT2,
    
}DEBUG_SEL, *PDEBUG_SEL;

/*--------------------------Define MACRO--------------------------------------*/

//-----------------------------------------------------------------------------
//	Define the debug levels
//
//	1.	DBG_TRACE and DBG_LOUD are used for normal cases.
//	So that, they can help SW engineer to develope or trace states changed 
//	and also help HW enginner to trace every operation to and from HW, 
//	e.g IO, Tx, Rx. 
//
//	2.	DBG_WARNNING and DBG_SERIOUS are used for unusual or error cases, 
//	which help us to debug SW or HW.
//
//-----------------------------------------------------------------------------
//
//	Never used in a call to RT_TRACE()!
//
#define DBG_OFF					0

//
//	Fatal bug. 
//	For example, Tx/Rx/IO locked up, OS hangs, memory access violation, 
//	resource allocation failed, unexpected HW behavior, HW BUG and so on.
//
#define DBG_SERIOUS				2

//
//	Abnormal, rare, or unexpeted cases.
//	For example, IRP/Packet/OID canceled, device suprisely unremoved and so on.
//
#define DBG_WARNING				3

//
//	Normal case with useful information about current SW or HW state. 
//	For example, Tx/Rx descriptor to fill, Tx/Rx descriptor completed status, 
//	SW protocol state change, dynamic mechanism state change and so on.
//
#define DBG_LOUD				4

//
//	Normal case with detail execution flow or information.
//
#define DBG_TRACE				5



//Debug
#if CONFIG_DBGMODE && (CONFIG_C2H_DEBUG_COMP == 0)

    #define RT_TRACE(_Comp, level, Fmt) \
    if( _Comp && ( (level <= SysMib.GlobalDebugLevel) || (level == DBG_SERIOUS)) ) \
    { \
        do { \
            Print2DbgBuf Fmt; \
        }while(0); \
    }
    
    #define RT_TRACE_COMP(_Comp, Fmt)
    
#elif CONFIG_DBGMODE && (CONFIG_C2H_DEBUG_COMP == 1)
    #define RT_TRACE_COMP(_Comp, Fmt) \
    if( (_Comp & SysMib.DebugComponent)) \
    { \
            Print2DbgBuf Fmt; \
    }    

    #define RT_TRACE(_Comp, level, Fmt)
 
#else
    #define RT_TRACE(_Comp, level, Fmt)
    #define RT_TRACE_COMP(_Comp, Fmt)

#endif  //#if CONFIG_DBGMODE


#define DBGPKTPTR_REACH_BOTTOM(x)   (((x) == LENGTH_DBGPKT_BUFF) ? _TRUE : _FALSE)
#define DBGPKT_Q_EMPTY(x, y)        (((x) == (y)) ? _TRUE : _FALSE)
#define DBGPKT_Q_FULL(x, y)         (((((x) == 0) && ((y) == (LENGTH_DBGPKT_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)

/*------------------------------Define Struct---------------------------------*/
typedef struct _DBGPKT_EVT_MSG {
    u16   length;
    char  content[50];
}DBGPKT_EVT_MSG, *PDBGPKT_EVT_MSG;


typedef struct _DBGPKT_EVT_ {
    DBGPKT_EVT_MSG     dbgpkt_sfbuff[LENGTH_DBGPKT_BUFF];
    u8                 dbgpkt_r_ptr;
    u8                 dbgpkt_w_ptr;
}DBGPKT_EVT, *PDBGPKT_EVT;

/*------------------------Export global variable------------------------------*/

/*------------------------------Funciton declaration--------------------------*/
extern void
PrintMcuDbgMsg(
    IN u8 idx,
    IN u8 byte_offset,
    IN u8 val
);

extern void
PrintMcuDbgMsgU16(
    IN u8 *val
) ;

extern void
DbgMonitorSel(
    void
    );

//extern void
//DbgStepByStep(
//    u8  step_num
//    );

extern void
InitDebugMode(
    void
);

extern void 
C2HPrintk(
    const char *fmt
);

extern void 
C2HDbg(
    u8  idx
);

extern void 
Print2DbgBuf(
    const char *fmt, ...
) ;

extern int 
vsprintf1(
    char *buf, 
    const char *fmt, 
    const char *dp
) ;

extern void
InitDebugPkt(
    void
);

extern BOOLEAN
issueDebugPkt(
    IN PDBGPKT_EVT_MSG  dbgmsg
);

extern void
DbgPktDeQueue(
    void
);

extern void
DbgPktInQueue(
    IN const char *fmt
) ;

extern void 
DebugFunction(
    void
) ;


#endif  //__HALCOM_DEBUG_RAM_H__

