#ifndef __RTL8721D_CMD_RAM_H__
#define __RTL8721D_CMD_RAM_H__

/*--------------------Define --------------------------------------------*/

#if (!CONFIG_8BYTE_H2C)

//Register H2C Command ID here
typedef enum _RTL8188E_H2C_CMD_ {
    H2CID_RSVDPAGE              = 0,
    H2CID_JOININFO              = 1,
    H2CID_SCAN                  = 2,
    H2CID_P2PPS_OFFLOAD         = 3,
    H2CID_H2C2HLB               = 4,
    H2CID_SETPWRMODE            = 5,
    H2CID_COEX_MASK             = 6,
    H2CID_COEX_GPIO_MODE        = 7,
    H2CID_COEX_DAC_SWING        = 8,    
    H2CID_PSTURNINGPARM         = 9,
    H2CID_PSLPSPARM             = 10,
    H2CID_WoWLAN                = 11,
    H2CID_PSTURNINGPARM2        = 12,
    H2CID_KEEP_ALIVE            = 13,
    H2CID_DISCONNECT_DECISION   = 14,
    H2CID_REMOTE_WAKEUP_CTRL    = 15,
    H2CID_AP_OFFLOAD            = 16,   // Alan
    H2CID_BCN_RsvdPage          = 17,   //Alan
    H2CID_Probersp_RsvdPage     = 18    //Alan              
    
} RTL8188E_H2C_CMD, *PRTL8188E_H2C_CMD;
#else

//Register H2C Command ID here
typedef enum _H2C_CMD_ {
    //1 Class1: Common
    H2CID_RSVDPAGE              = 0x00,
    H2CID_JOININFO              = 0x01,
    H2CID_SCAN                  = 0x02,
    H2CID_KEEP_ALIVE            = 0x03,
    H2CID_DISCONNECT_DECISION   = 0x04,
    H2CID_PSD_OFFLOAD           = 0x05,
    H2CID_Customer_string_rpt   = 0x06,
    rsvd3                       = 0x07,    
    H2CID_AP_OFFLOAD            = 0x08,     // Alan     
    H2CID_BCN_RsvdPage          = 0x09,     //Alan
    H2CID_Probersp_RsvdPage     = 0x0A,     //Alan  
    H2CID_TXPower_Index_Offlaod = 0x0B,  //Alan
    H2CID_AP_PS_OFFLOAD         = 0x0C, //yllin
    H2CID_AP_PS_OFFLOAD_CTRL    = 0x0D, //yllin
    H2CID_ECSA                  = 0x0F,

    H2CID_FAST_CS_RSVDPAGE      = 0x10,
    H2CID_FAST_CHANNEL_SWITCH   = 0x11,
    H2CID_BB_GAIN_REPORT        = 0x12,	// Alan
    H2CID_GPIO_CTRL             = 0x13,
    H2CID_HW_INFO               = 0x14,		//Alan	
    H2CID_MCC_RQT_TSF      = 0x15,
    H2CID_MCC_MACID_BITMAP      = 0x16,
    H2CID_MCC_CTRL              = 0x18,
    H2CID_MCC_TIME_PARAM         = 0x19,
    H2CID_MCC_IQK_PARAM         = 0x1A,
    H2CID_NAN_CTRL              = 0x1B,
    H2CID_SINGLE_CHANNEL_SWITCH = 0x1C,
    H2CID_SINGLE_CHANNEL_SWITCH_V2 = 0x1D,
    H2CID_TX_PAUSE_DRV_INFO		= 0x1E,
    H2CID_MACID_PAUSE_DRV_INFO  = 0x1F,


    //1 Class2: Power Save
    H2CID_SETPWRMODE            = 0x20,
    H2CID_PSTURNINGPARM         = 0x21,
    H2CID_PSTURNINGPARM2        = 0x22,
    H2CID_PSLPSPARM             = 0x23,
    H2CID_P2PPS_OFFLOAD         = 0x24,
    H2CID_PS_SCAN               = 0x25,
    H2CID_SAPPS                 = 0x26,
    H2CID_INACTIVE_PS           = 0x27,
    H2CID_NOLINK_PS             = 0x28,
    H2CID_PARTIAL_OFF_CTRL   = 0x29,
    H2CID_PARTIAL_OFF_PARM  = 0x2A,
    H2CID_S1_NULL1_CTRL         =0x3F,
    
    //1 Class3: Dynamic Mechaism
    H2CID_MACID_CFG             = 0x40,
    H2CID_TxBF                  = 0x41,
    H2CID_RSSI_SETTING          = 0x42,
    H2CID_AP_REQ_TXRPT          = 0x43,
    H2CID_INIT_RATE_COLLECT     = 0x44,
    H2CID_IQK_OFFLOAD           = 0x45,
    H2CID_MACID_CFG_3SS         = 0x46,
    H2CID_RA_PARA_ADJUST        = 0x47,
    H2CID_DYNAMIC_TX_PATH       = 0x48,
    H2CID_FW_TRACE_EN           = 0x49,
    

    //1 Class4: BT Coex
    //H2CID_COEX_MASK             = 0x60,
    //H2CID_COEX_GPIO_MODE        = 0x61,
    //H2CID_COEX_DAC_SWING        = 0x62, 
    H2CID_B_TYPE_TDMA           = 0x60,
    H2CID_BT_INFO               = 0x61,
    H2CID_FORCE_BT_TXPWR        = 0x62,
    H2CID_BT_IGNORE_WLANACT     = 0x63,
    H2CID_DAC_SWING_VALUE       = 0x64,
    H2CID_ANT_SEL_REVERSE       = 0x65,
    H2CID_WL_OPMODE             = 0x66,
    H2CID_BT_MP_OPERATION       = 0x67,
    H2CID_BT_CONTROL            = 0x68,
    H2CID_BT_WIFICTRL           = 0x69,
    H2CID_BT_PATCH_DOWNLOAD     = 0x6A,
    H2CID_BT_SCO_eSCO_OPERATION	= 0x6B,  	
    H2CID_BT_Page_Scan_Interval = 0x6C,  	
    H2CID_WL_Calibraion         = 0x6D,
    H2CID_GNT_BT_CTRL           = 0x6E,
    H2CID_BT_ONLY_TEST          = 0x6F,
    H2CID_BT_INIT_PARAM           = 0x70,
    
    //H2CID_PSD_RESULTS           = 0x63,

    //1 Class5: WOWLAN
    H2CID_WoWLAN                = 0x80,
    H2CID_RemoteWakeCtrl        = 0x81,
    H2CID_AOAC_Global_info      = 0x82,
    H2CID_AOAC_Rsvdpage1        = 0x83,
    H2CID_AOAC_Rsvdpage2        = 0x84,
    H2CID_D0_Scan_offload_info  = 0x85,
    H2CID_D0_Scan_offload_ctrl  = 0x86,
    H2CID_Switch_channel        = 0x87,    
    H2CID_AOAC_Rsvdpage3        = 0x88,
    H2CID_GPIO_WF_Customize     = 0x89,
    H2CID_P2P_RsvdPage          = 0x8A,
    H2CID_P2P_Offload           = 0x8B,

    //1 Class6: LTECOEX
    H2CID_LTECOEX_EN            = 0xA0,
    H2CID_WLAN_High_Priority    = 0xA1,

    H2CID_FTM                   = 0xB0,    //Alan
    H2CID_FTMRPT                = 0xB1,        //Alan

    //1 Class7: Patch
    H2CID_TSF_RESET             = 0xC0,
    H2CID_BB_NHM                = 0xC1,
    H2CID_BCN_Ignore_EDCCA      = 0xC2,
    H2CID_Efuse_Hidden_Request  = 0xC3, 
    H2CID_APoffload_MultiWakeupPulse = 0xC4,
    H2CID_BCN_HWSEQ	        = 0xC5,   //8814A BCN HWSEQ issue for Seam
    H2CID_Customer_string1      = 0xC6,
    H2CID_Customer_string2      = 0xC7,
    H2CID_Customer_string3      = 0xC8,
    H2CID_IOTDRV_CloseRF      = 0xCF,
    //1 Class8: Testing
    H2CID_H2C2HLB               = 0xE0,
    H2CID_FW_Verification       = 0xE4,    
    
    //1 Class9:FW Offload
    CMD_ID_FW_OFFLOAD_H2C       = 0xFF
} H2C_CMD, *PH2C_CMD;

#endif  //#if (!CONFIG_8BYTE_H2C)

#if 0
//Register C2H Command ID here
typedef enum _C2H_CMD_ {
    C2HID_DEBUG                 = 0,
    C2HID_H2C2HLB               = 1,
    C2HID_TXBF                  = 2,
    C2HID_CCX_TXRPT             = 3,
    C2HID_AP_REQ_TXRPT          = 4,
    C2HID_INIT_RATE_COLLECT     = 5,
    C2HID_PSD_RPT               = 6,
    C2HID_SCAN_COMPLETE         = 7,
    C2HID_PSD_CONTROL           = 8,
    C2HID_BT_INFORMATION        = 9,
    C2HID_BT_LOOPBACK           = 0x0A,
    C2HID_BT_MP_REPORT          = 0x0B,
    C2HID_RA_RPT                = 0x0C,
    C2HID_SPE_STATIS            = 0x0D,
    C2HID_RA_PARA_RPT            = 0x0E,
    C2HID_DTP_RPT               = 0x0F,   
    C2HID_CURRENT_CHANNEL       = 0x10,
    C2HID_IQK_OFFLOAD           = 0x11,
    C2HID_BB_GAIN_REPORT        = 0x12,
    C2HID_RateAdaptive_RPT_88E  = 0x13,	//Ben, 130515, for 88e smic RA debug
    C2HID_GPIOWAKEUP            = 0x14,
    C2HID_MAILBOX_STATUS        = 0x15,		// Alan 2013_0720
    C2HID_H2C_PKT_Response      = 0x16, //mips
    C2HID_MCC_STATUS            = 0x17,
    C2HID_NAN_EVENT             = 0x18,
    C2HID_Efuse_Hidden_REPORT   = 0x19,
    C2HID_FTMC2H_RPT            = 0x1B,  //Alan 20141229
    C2HID_FTMSession_END        = 0x1C,  //Alan 20141229
    C2HID_DRVFTMC2H_RPT         = 0x1D,  //Alan 20150518

    C2HID_DEBUG_CODE            = 0xFE,
    C2HID_EXTEND_IND            = 0xFF
} C2H_CMD, *PC2H_CMD;
#endif 

typedef enum _C2H_CMD_SUBID_ {
    C2HEXTID_DEBUG_PRT          = 0,


} C2H_CMD_SUBID, *PC2H_CMD_SUBID;

/*--------------------------Define MACRO--------------------------------------*/
// H2C
#if CONFIG_8BYTE_H2C
#define LENGTH_H2C              8
#else
#define LENGTH_H2C              6
#endif
#define LENGTH_H2C_CMD_HDR      sizeof(H2C_CMD_HDR)
#define LENGTH_H2C_CONTENT      (LENGTH_H2C -LENGTH_H2C_CMD_HDR)
#define ADOPT_H2C_VALUE         BIT7
#define LENGTH_H2C_BUFF         10
#define H2C_Q_FULL(x, y)        (((((x) == 0) && ((y) == (LENGTH_H2C_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)
#define H2C_Q_EMPTY(x, y)       (((x) == (y)) ? _TRUE : _FALSE)
#define H2CPTR_REACH_BOTTOM(x)  (((x) == LENGTH_H2C_BUFF) ? _TRUE : _FALSE)

// C2H
/*
#define LENGTH_C2H              16
#define LENGTH_C2H_EVT_HDR      2
#define LENGTH_C2H_LEN_FIELD    1
#define LENGTH_C2H_SYNC         1
#define LENGTH_C2H_CONTENT      (LENGTH_C2H - LENGTH_C2H_EVT_HDR - LENGTH_C2H_LEN_FIELD - LENGTH_C2H_SYNC)
#define LENGTH_C2H_BUFF         10
#define WAIT_C2H_CHECK_CNT      (1000)
#define CHECK_C2H_ONCE_TIME     (10)
#define C2H_DATA_RDY            0xFF
#define C2HPTR_REACH_BOTTOM(x)  (((x) == LENGTH_C2H_BUFF) ? _TRUE : _FALSE)
#define C2H_Q_EMPTY(x, y)       (((x) == (y)) ? _TRUE : _FALSE)
#define C2H_Q_FULL(x, y)        (((((x) == 0) && ((y) == (LENGTH_C2H_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)
#define C2HPKT_POLL              BIT0
*/
#define LENGTH_C2HEXT_EVT_HDR   4
#if 1
#define LENGTH_C2HEXT_BUFF      32
#define LENGTH_C2HEXT_CONTENT   100  //128-24-4 
#else
#define LENGTH_C2HEXT_BUFF      16
#define LENGTH_C2HEXT_CONTENT   228 //256-24-4
#endif
#define C2HEXTPTR_REACH_BOTTOM(x)  (((x) == LENGTH_C2HEXT_BUFF) ? _TRUE : _FALSE)
#define C2HEXT_Q_EMPTY(x, y)       (((x) == (y)) ? _TRUE : _FALSE)
#define C2HEXT_Q_FULL(x, y)        (((((x) == 0) && ((y) == (LENGTH_C2HEXT_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)




// DRV_FW_INTF
#define DRV_FW_EN               BIT7
#define DRV_FW_RWM              BIT6 
#define BYTE_CNT                (BIT0|BIT1|BIT2)

#define DRV_FW_MODE            (BIT5|BIT4)

#define MODE_SRAM               0
#define MODE_TXBUF              0x10
#define MODE_RXBUF              0x20

#define FSM_MON_BT_DBG          0x5
#define BT_ACTIVE_MODE          0x4

#define SIGNAL_SEND_ISR         0
#define SIGNAL_SEND_OS          1

#define BT_ANT_TDMA_FUNC_EN         BIT0
#define BT_ANT_TDMA_SINGLE_ANT      BIT1
#define BT_ANT_TDMA_NAV_EN          BIT2
#define BT_ANT_TDMA_DAC_SWING_EN    BIT4

#define WLCALIBRATION           0x1e7
#if 0   //CONFIG_BB_GAIN_RPT
//BB Gain report
	#if (IS_CHIP_8188E(CONFIG_CHIP_SEL))
	#define efuse_BB_2G_TX	0xF6
	#define efuse_BB_5GLB_TX	0xF4		// don't care
	#define efuse_BB_5GMB_TX	0xF3		// don't care
	#define efuse_BB_5GHB_TX	0xF2		// don't care
	#define efuse_thermal		0xF5
	

	#elif (IS_CHIP_8195(CONFIG_CHIP_SEL))
	#define efuse_BB_2G_TX	0x3F6
	#define efuse_BB_5GLB_TX	0x3F4
	#define efuse_BB_5GMB_TX	0x3F3
	#define efuse_BB_5GHB_TX	0x3F2
	#define efuse_thermal		0x3F5
	
	
	#elif ((IS_CHIP_8821(CONFIG_CHIP_SEL))||(IS_CHIP_8881(CONFIG_CHIP_SEL))||(IS_CHIP_8192E(CONFIG_CHIP_SEL))||(IS_CHIP_8723B(CONFIG_CHIP_SEL))||(IS_CHIP_8703B(CONFIG_CHIP_SEL))||(IS_CHIP_8721D(CONFIG_CHIP_SEL)))
	#define efuse_BB_2G_TX	0x1F6
	#define efuse_BB_5GLB_TX	0x1F4
	#define efuse_BB_5GMB_TX	0x1F3
	#define efuse_BB_5GHB_TX	0x1F2
	#define efuse_thermal		0x1F5
	
	#endif
#endif	//#if CONFIG_BB_GAIN_RPT

#if 0   //CONFIG_Efuse_Hidden_Request
    #define efuse_UUID          0x1F4
    #define efuse_HCI_Package_Type    0x1F8        
    #define efuse_WL_Function_support 0x1F9
    #define efuse_BW_AntNum_Remark    0x1FA   
    #define efuse_BW_AntNum     0x1FB        
    #define efuse_Protocol_NIC_Router       0x1FD
#endif  //#if CONFIG_Efuse_Hidden_Request

//#define MACID_NUM_NEW           (MACID_NUM/8)


// FCS
#define FCS_REQ_END             0xF


//MACID Drop Action
#define MACID_DROP_DISABLE      0
#define MACID_DROP_ENABLE       1

/*------------------------------Define Struct---------------------------------*/
//================H2C command format====================
//
// Field     CONTENT      CMD_ID
// BITS      [63:8]           [7:0]

#if (!CONFIG_8BYTE_H2C)

// Field     CONTENT    CMD_EXT  CMD_ID
// BITS  [31:8]             7          [6:0]

typedef struct _H2C_CMD_HDR {
    u8 cmd_id:7;
    u8 cmd_ext:1;
}H2C_CMD_HDR, *PH2C_CMD_HDR;

#else
// Field     CONTENT    CMD_ID
// BITS  [31:8]             [7:0]          

typedef struct _H2C_CMD_HDR {
    u8 cmd_id;
}H2C_CMD_HDR, *PH2C_CMD_HDR;

#endif  //#if (!CONFIG_8BYTE_H2C)
#if 0
//==================C2H event format ====================
// Field     SYNC       CMD_LEN     CONTENT    CMD_SEQ      CMD_ID
// BITS  [127:120]  [119:112]      [111:16]          [15:8]        [7:0]
typedef struct _C2H_EVT_HDR {
    u8  cmd_id;
    u8  cmd_seq;
    u8  content[12];
    u8  cmd_len;
}C2H_EVT_HDR, *PC2H_EVT_HDR;
#endif 

//==================C2H Extend event format ====================
// Field     CONTENT    CMD_LEN      CMD_SEQ    CMD_SubID     CMD_ID
// BITS      [799:32]      [31:24]       [23:16]        [15:8]           [7:0]

typedef struct _C2HEXT_EVT_HDR {
    u8  cmd_id;
    u8  cmd_seq;
    u8  subid;
    u8  cmd_len;//content len
    u8  content[LENGTH_C2HEXT_CONTENT]; 
}C2HEXT_EVT_HDR, *PC2HEXT_EVT_HDR;

////////////////////////////////////////////////////////////////////


//3 Class1: Common

//H2C Index: 0x0
typedef struct _H2CParam_RsvdPage_ {
    RsvdPageLoc loc;
}H2CParam_RsvdPage, *PH2CParam_RsvdPage;


//H2C Index: 0x1
typedef struct _H2CParam_JoinInfo_ {
    BOOLEAN     bConnected:1;
    BOOLEAN     bMacid_ind:1;
    u8          Miracast:1;
    u8          Miracast_role:1;
    u8          Macid_Dest_Role:4;
    u8          macid;
    u8          macid_end;
}H2CParam_JoinInfo, *PH2CParam_JoinInfo;


//H2C Index: 0x2
typedef struct _H2CParam_Scan_ {
    BOOLEAN            bScan;
    BOOLEAN            bNIC;           // 1: NIC, 0: AP
}H2CParam_Scan, *PH2CParam_Scan;

//H2C Index: 0x3
typedef struct _keep_alive_parm_ {
    u8          Enable:1;
    u8          AdoptUserSetting:1;
    u8          PktType:1;
    u8          rsvd:4;
    u8          KeepAliveNullTx:1;
    //u8          KeepAliveNull0:1;
    //u8          KeepAliveNull1:1;
    u8          KeepAliveCountLimit;
    u8          KeepAliveCount;
    u8          SameTBTTRsvdPageCount;
}KeepAlive_Parm, *PKeepAlive_Parm;

//H2C Index: 0x4
typedef struct _disconnect_parm_ {
    u8          Enable:1;
    u8          AdoptUserSetting:1;
    u8          TryOkBcnFailCountEn:1;
    u8          DisconnectEN:1;
    u8          rsvd:4;
    u8          DisconnectCountLimit;
    u8          TryPKTCountLimit;
    u8          TryOkBcnFailCountLimit;
    //------------------------------------
    u8          DisconnectCount;
    u8          TryPKTCount;
    u8          BCNCount;
    u8          WHCKPatchEn;
}Disconnect_Parm, *PDisconnect_Parm;

//H2C Index: 0x8    // Alan
typedef struct  _H2CParam_ap_offload_ {
    ap_offload_parm ap_para;
}H2CParam_ap_offload,*PH2CParam_ap_offload;


//H2C Index: 0x9    // Alan
typedef struct  _H2CParam_BCN_rsvdpage_loc_ {
    BCN_rsvdpage_loc  BCN_loc;
}H2CParam_BCN_rsvdpage_loc,*PH2CParam_BCN_rsvdpage_loc;

//H2C Index: 0xA    // Alan
typedef struct  _H2CParam_Probersp_rsvdpage_loc_ {
    Probersp_rsvdpage_loc  Probersp_loc;
}H2CParam_Probersp_rsvdpage_loc,*PH2CParam_Probersp_rsvdpage_loc;


//H2C Index: 0x10
typedef struct _H2CParam_FCS_rsvdpage_loc_ {
    u8 loc_packet[FCS_MAX_CHIFO_NUM];
}H2CParam_FCS_rsvdpage_loc, *PH2CParam_FCS_rsvdpage_loc;


#if CONFIG_FCS_LOGO

//H2C Index: 0x11	
typedef struct _H2CParam_FCS_param_ {  
    u8 order:4;
    u8 totalnum:4;
    u8 chidx;
    u8 bw:2;
    u8 bw40sc:3;
    u8 bw80sc:3;
    u8 duration;  // UNIT:8ms
    u8 macid0;
    u8 macid1;
    u8 rfetype:4;
    u8 distxnull:1;
    u8 c2hrpt:2;
    u8 channelscan:1;
    //u8 queuesel;
} H2CParam_FCS_param, *PH2CParam_FCS_param;
#endif //#if CONFIG_FCS_LOGO

//H2C Index: 0x12		// Alan
typedef struct _H2CParam_BB_Gain_RPT_ {  
    u8 enable;
} H2CParam_BB_Gain_RPT, *PH2CParam_BB_Gain_RPT;

//H2C Index: 0x13
typedef struct _H2CParam_GPIO_CTRL_ {  
    u8 gpioidx:4;
    u8 c2hen:1;
    u8 pulse:1;
    u8 highactive:1;
    u8 en:1;
    u8 duration:7;
    u8 unitms:1;
    u8 duration_c2h;
    u8 hoststate:1;
    u8 RSVD:7;
} H2CParam_GPIO_CTRL, *PH2CParam_GPIO_CTRL;

//H2C Index: 0x14
typedef struct _H2CParam_HW_INFO_ {  
    u8 single_chip:1;

} H2CParam_HW_INFO, *PH2CParam_HW_INFO;


typedef struct _HW_Info_ {
    u8 single_chip:1;

} HW_Info, *PHW_Info;


//H2C Index: 0x15
typedef struct _H2CParam_FCS_Update_Param_ {  
    u8 order:4;
    u8 rsvd:4;
    
    u8 mask_duration:1; //if this bit mask = 1, fw will update parameter: duration
    u8 mask_InCurCh:1;  //if this bit mask = 1, fw will update parameter: InCurCh
    u8 mask_distxnull:1;//if this bit mask = 1, fw will update parameter: distxnull
    u8 mask_c2hrpt:1;   //if this bit mask = 1, fw will update parameter: c2hrpt
    u8 rsvd1:4;
    
    u8 duration;    // UNIT:1TU
    
    u8 InCurCh:1;   //1: if null pkt retry fail, keep in current channel,  0: if null pkt retry fail, switch to next channel
    u8 distxnull:1;        
    u8 c2hrpt:2;
    u8 rsvd2:4;
} H2CParam_FCS_Update_Param, *PH2CParam_FCS_Update_Param;

//H2C Index: 0x16
typedef struct _H2CParam_FCS_MacidBitMap_ {  
    u8 macid_bitmap0_low;
    u8 macid_bitmap0_high;
    u8 macid_bitmap1_low;
    u8 macid_bitmap1_high;
    u8 macid_bitmap2_low;
    u8 macid_bitmap2_high;
} H2CParam_FCS_MacidBitMap, *PH2CParam_FCS_MacidBitMap;

#if CONFIG_MCC

//H2C Index: 0x18
typedef struct _H2CParam_MCC_param_ {  
    u8 order:4;
    u8 totalnum:4;
    
    u8 chidx;
    
    u8 bw:2;
    u8 bw40sc:3;
    u8 bw80sc:3;
    
    u8 duration;    // UNIT:1TU
    
    u8 role:3;      //0: STA, 1: AP, 2: GC, 3: GO.	
    u8 InCurCh:1;   //1: if null pkt retry fail, keep in current channel,  0: if null pkt retry fail, switch to next channel
    u8 DisSwRetry:1;    // 0: means enable sw retry, 1: means disable sw retry
    u8 MaxSwRetryIdx:1;
    u8 rsvd:2;
    
    u8 rsvd1;
    
    u8 rfetype:4;
    u8 distxnull:1;
    u8 c2hrpt:2;
    u8 channelscan:1;
    //u8 queuesel;
} H2CParam_MCC_param, *PH2CParam_MCC_param;

//H2C Index: 0x19
typedef struct _H2CParam_MCC_Time_param_ { 
    u8 MCC_TSFSync_enable:1;
    u8 TSF_Sync_offset:7;
    u8 MCC_start_time;
    u8 MCC_interval;
    u8 MCC_EarlySwitch;
    u8 rsvd;
    u8 Update_En:1;
    u8 Update_Dur0:7;      
} _H2CParam_MCC_Time_param_, *_PH2CParam_MCC_Time_param_;

//H2C Index: 0x1A
typedef struct _H2CParam_MCC_IQK_param_ {  
    u8 IQK_Ready : 1;
    u8 IQK_index : 4;
    u8 IQK_rsv : 3;
    u8 IQK_RX_L;
    u8 IQK_RX_M;
    u8 IQK_RX_H;
    u8 IQK_TX_L;
    u8 IQK_TX_M;
    u8 IQK_TX_H;
} _H2CParam_MCC_IQK_param_, *_PH2CParam_MCC_IQK_param_;

#endif //#if CONFIG_MCC

#if CONFIG_NAN

//H2C Index: 0x1B
typedef struct _H2CParam_NAN_param_ {

    u8 Enable:1;
    u8  FwTxSyncBcn:1;
    u8  FwTxDiscBcn:1;
    u8 Rsvd:5;
}H2CParam_NAN_param, *PH2CParam_NAN_param;    

#endif

//H2C Index: 0x1C
typedef struct _H2CParam_Channel_Switch_ {
    u8 channel_num;

    u8 BW       :2;
    u8 BW40sc   :3;
    u8 BW80sc   :3;

    u8 RFEType  :4;
    u8 rsvd     :4;
} H2CParam_Channel_Switch, *PH2CParam_Channel_Switch;

//3 Class2: Power Save
//H2C Index: 0x20
typedef struct _H2CParam_SetPwrMode_parm_ {
    LEGACY_PS_PARM      PwrModeParm;
}H2CParam_PwrMode, *PH2CParam_PwrMode;

//H2C Index: 0x21
typedef struct _H2CParam_PSTuningParm_ {
    PS_TUNING_PPARM    PSTuningParm;
}H2CParam_PSTuningParm, *PH2CParam_PSTuningParm;

//H2C Index: 0x22
typedef struct _H2CParam_PSTuningParm2_ {
    PS_TUNING_PPARM2    PSTuningParm2;
}H2CParam_PSTuningParm2, *PH2CParam_PSTuningParm2;

//H2C Index: 0x23
typedef struct _H2CParam_PSLPSParm_ {
    u8  lpscontrol;
}H2CParam_PSLPSParm, *PH2CParam_PSLPSParm;

//H2C Index: 0x24
typedef struct _H2CParam_P2PPS_ {
    P2P_PS_Offload     P2PPSPara;
}H2CParam_P2PPS, *PH2CParam_P2PPS;

//H2C Index: 0x25
typedef struct _Scan_Parm_ {
    u8          Enable;
}Scan_Parm, *PScan_Parm;

//H2C Index: 0x26
typedef struct _H2CParam_SAPPSParm_ {
    u8  Enable:1;
    u8  EnPS:1;
    u8  EnLPRx:1;
    u8  Manual32k:1;    //revised by isaac on 20141202
    u8  rsvd:4;
    u8  Duration;
#if IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL)
    u8 SegNum;
#endif
}H2CParam_SAPPSParm, *PH2CParam_SAPPSParm;

//H2C Index: 0x27
typedef struct _H2CParam_INACTIVEPSParm_ {
    u8      Enable:1;
    u8      IgnorePsCondition:1;
    u8      rsvd:6;
    u8      Frequency;
    u8      Duration;
    u8      FrequencyCount;
}H2CParam_INACTIVEPSParm, *PH2CParam_INACTIVEPSParm;


//H2C Index: 0x28
typedef struct _H2CParam_NOLINKPSParm_ {
    u8      Enable:1;
    u8      NoConnect32k:1;
    u8      rsvd:6;
}H2CParam_NOLINKPSParm, *PH2CParam_NOLINKPSParm;

//H2C Index: 0x29
typedef struct  _H2CParam_PARTIALOFFCtrl_ {
    u8      Enable:1;
    u8      rsvd:7;
}H2CParam_PARTIALOFFCtrl, *PH2CParam_PARTIALOFFCtrl;

//H2C Index: 0x2A
typedef struct _H2CParam_PARTIALOFFParm_ {
    u8      ReadVld:1;
    u8      WriteVld:1;
    u8      rsvd:6;
    u8      StartAddrL;
    u8      StartAddrH;
    u8      EndAddrL;
    u8      EndAddrH;
}H2CParam_PARTIALOFFParm, *PH2CParam_PARTIALOFFParm;    


//3 Class3: Dynamic Mechaism




//3 Class4: BT Coex
#if 0
//H2C Index: 0x60
typedef struct _H2CParam_BTCoexMask_ {
    BTCOEX_CtrlInfo    Mask;
}H2CParam_BTCoexMask, *PH2CParam_BTCoexMask;

//H2C Index: 0x61
typedef struct _H2CParam_BTCoexGPIOMode_ {
    BTCOEX_CtrlInfo    GPIOMode;
}H2CParam_BTCoexGPIOMode, *PH2CParam_BTCoexGPIOMode;

//H2C Index: 0x62
typedef struct _H2CParam_DAC_ {
    u8                 bDACSwing;
}H2CParam_DAC, *PH2CParam_DAC;
#endif

#if 0
//H2C Index: 0x60
typedef struct _BT_MAILBOX_Parm_ {
    u8          signal_ant_en:1;
    u8          DAC_swing_en:1;
    u8          NAV_en:1;
    u8          TDMA_En:1;
    u8          rvsd:4;
    
    u8          bt_rssi_init_value;
    u8          bt_rssi_current_value;
    u32         DAC_swing_level;
    u32         DAC_swing_init_value;
    u8          init_value_0x2e;
    u8          init_value_0x2d;
    u8          OneAntSCOEnable;
    u8          BTNoResponseCount;
    u8          AntSelReverse;
    u8          SwitchOutside;
    u8          RAOffsetEn;
    u8          RAOffset;
    u8          RAOffsetMCS7;
    u8          RAOffsetMCS6;
    u8          RAOffsetMCS5;
    u8		 BTCalibration:1;
    u8           rsvd1:7;
    u8		 BTCalibrationPeriod;
    u8  	 Null1_FAIL_Counter;	
}BT_MAILBOX_Parm, *PBT_MAILBOX_Parm;


//H2C Index: 0x61
typedef struct _BT_Info_Parm_ {    
    u8          trigger;
}BT_Info_Parm, *PBT_Info_Parm;


//H2C Index: 0x62
typedef struct _Force_BT_Txpwr_Parm_ {
    u8          pwr_idx;
}Force_BT_Txpwr_Parm, *PForce_BT_Txpwr_Parm;

//H2C Index: 0x63
typedef struct _BT_Ignore_WlanAct_Parm_ {
    u8          enable;
}BT_Ignore_WlanAct_Parm, *PBT_Ignore_WlanAct_Parm;


//H2C Index: 0x64
typedef struct _DAC_Swing_Value_Parm_ {
    u8          value;
}DAC_Swing_Value_Parm, *PDAC_Swing_Value_Parm;


//H2C Index: 0x65
typedef struct _Ant_Sel_Reverse_Parm_ {
    u8          Reverse;
    u8          SwitchOutside;
}Ant_Sel_Reverse_Parm, *PAnt_Sel_Reverse_Parm;

//H2C Index: 0x66
typedef struct _WLAN_Opmode_Parm_ {
    u8          opmode; // RT_MEDIA_STATUS
    u8          chl_idx;
    u8          bw;     
}WLAN_Opmode_Parm, *PWLAN_Opmode_Parm;


//H2C Index:0x6B
typedef struct _BT_SCO_eSCO_operation_Parm_{

	//Byte0	
	u8    enable:1;
	u8	Control_Pkt_Type:2;
	u8	Tx_Pause_BT_slot:1;
	u8	Coex_Table_toggle:1;
	u8	Antenna_Toggle:1;
	u8	rsvd_1:2;

	//Byte1	
	u8	ValueOf778WiFi_slot:2;
	u8	Ant_position_wifi_slot:1;
	u8	rsvd_2:1;
	u8	ValueOf778BT_slot:2;
	u8	Ant_position_bt_slot:1;
	u8	rsvd_3:1;

	//Byte2
	u8	WiFi_slot;		
	//Byte3	
	u8	BT_slot;		
	//Byte4	
	u8	CTS2Self_NAV;	

	//not from H2C
	u8	stage;
	u8	INT_count;
	u8	issue_null_stage;
	u8	SCO_eSCO_TX_null_flag;
	u8	SCO_eSCO_period_end_flag;
	u16	WiFi_slot_real;
	u16	BT_slot_real;
	u16	CTS2Self_NAV_real;
}BT_SCO_eSCO_operation_Parm,*PBT_SCO_eSCO_operation_Parm;

//H2C Index:0x6C
typedef struct _H2CHDL_BT_Page_Scan_Interval_Parm_{
	u8	LSB_Interval;
	u8	MSB_Interval;
}BT_Page_Scan_Interval_Parm,*PBT_Page_Scan_Interval_Parm;

typedef struct _H2CHDL_BT_CSR_Mode_Parm_ {
    u8  Enhance3wireMode:1;
    u8  rsvd:7;
}BT_CSR_Mode_Parm, *PBT_CSR_Mode_Parm;

#endif

typedef struct _H2CParam_PSD_Result_ {
    u8          Data0;
    u8          Data1;
    u8          Data2;
    u8          Data3;
    u8          Data4;
}PSDResultParm, *PPSDResultParm;

//3 Class5: WOWLAN

// blow is WOWLAN define struct
//H2C Index: 0x80
typedef struct _wwlan_enable_ {
    u8          fun_En:1;
    u8          pattern_match_En:1;
    u8          magic_pkt_En:1;
    u8          unicast_En:1;
    u8          All_pkt_drop_En:1;
    u8          GPIO_Active_type_En:1;
    u8          Rekey_Wakeup_En:1;
    u8          Discon_Wakeup_En:1;
}WWlan_En, *PWWlan_En;
    
typedef struct _wwlan_parm_ {
    WWlan_En    enable;
    
    u8          gpio_num:7;
    u8          DataPinWakeUp:1;
    
    u8          gpio_duration;
    u8          gpio_pulse_en:1;
    u8          gpio_pulse_count:7;
    
    u8          usbphy_disable:1;
	u8			hst2dev_en:1;
	u8			gpio_duration_ms:1;
	u8			rsvd0:5;
    
    //not H2C from here
    u8          nullpkt:1;
    u8          DriverRdy:1;
    u8          rsvd:6;
}WWlan_Parm, *PWWlan_Parm;


//H2C Index: 0x81
typedef struct _remote_wakeup_enable_ {
    u8          RemoteWakeUpCtrlEn:1;
    u8          ArpEn:1;
    u8          NdpEn:1;
    u8          GtkEn:1;
    u8          NLOOffloadEn:1;
    u8          RealWoWLanEn:1;
    u8          RealWoWLanV2En:1;
    u8          MatchApplQQ:1; 

    u8          P2POffloadEn:1;
    u8          Runtime_PM_En:1;
    u8          NetbiosDropEn:1;
    u8          TkipOffloadEn:1;   
	u8          TcpSyncDropEn:1;        
    u8          rsvd:1;
#if CONFIG_NEGATIVE_PATTERN
    u8          NegativeFilterEn:1;
    u8          rsvd1:1;
#else
    u8          rsvd1:2;
#endif
    
    u8          ARP_ACTION:1;
    u8          rsvd2:3;
    u8          FWParsingUntilWakeup:1;
	u8          FWParsingAfterWakeup:1;
    u8          rsvd3:2;

#if CONFIG_NEGATIVE_PATTERN
    u8          NegativePatternNum:3;
    u8          Rsvd4:7;
#endif
}Remote_wakeup_enable, *PRemote_wakeup_enable;

typedef struct _remote_wakeup_ {
    Remote_wakeup_enable funcEn;
//#if CONFIG_WoWLAN
    WWlan_Parm      WWlanCtrl;
    u8              Wakeup:1;
#if (IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL) && CONFIG_DELAY_WAKEUP)
    u8              delay_wake_wait:1;
    u8              delay_wake_flag:1;
    u8              rsvd:5;
#else
    u8              rsvd:7;
#endif
    u8              MACAddr[6];
    u8              BSSIDAddr[6];
    u8              Debug[7];
    u8              AntSwitchOutside;
    u8              TwoAnt;
    u8              AntSelReverse;
    u8              RunTimePMFilter;
    u32             TotalRSSIByFw;
    //u8              Debug;
//#endif
}RemoteWakeup_Parm, *PRemoteWakeup_Parm;


#if 0
//H2C Index: 0x82
typedef struct _AOAC_GLOBAL_INFO_ {
    u8  PairwiseEncAlg;
    u8  GroupEncAlg;

}AOAC_GLOBAL_INFO,*PAOAC_GLOBAL_INFO;

//H2C Index: 0x83
typedef struct _AOAC_RSVDPAGE1_ {
    u8  loc_remoteCtrlInfo;
    u8  loc_arp;
    u8  loc_ndp;
    u8  loc_gtkRsp;
    u8  loc_gtkInfo;
    u8  loc_gtkExtMem;
    u8  loc_ndpInfo;
}AOAC_RSVDPAGE1,*PAOAC_RSVDPAGE1;
#endif

//H2C Index: 0x84
typedef struct _AOAC_RSVDPAGE2_ {
    u8  loc_routerSolicitation;
    u8  loc_bubblePacket;
    u8  loc_teredoInfo;
    u8  loc_RealwowInfo;
    u8  loc_KeepAlivePkt;
    u8  loc_AckPattern;
    u8  loc_WakeupPattern;
}AOAC_RSVDPAGE2,*PAOAC_RSVDPAGE2;


// H2C Index: 0x86
typedef struct _Scan_function_enable_ {
    u8  d0_fun_En:1;
    u8  RTD3_fun_En:1;
    u8  U3_SCAN_fun_En:1;    
    u8  NLO_fun_En:1;
    u8  ips_dependent:1;
    u8  rsvd:3;
}Scan_function_enable, *PScan_function_enable;

typedef struct _D0_Scan_offload_ctrl_ {
    Scan_function_enable fun_En;
    u8  loc_probe_packet;
    u8  loc_scan_info;
    u8  loc_SSID_info;
}D0_Scan_offload_ctrl,*PD0_Scan_offload_ctrl;

// H2C Index: 0x88
typedef struct _AOAC_RSVDPAGE3_ {
    u8  loc_NLOInfo;
	u8  loc_AOACReport;
#if CONFIG_NEGATIVE_PATTERN
    u8  loc_Negative_Pattern;
#endif
}AOAC_RSVDPAGE3,*PAOAC_RSVDPAGE3;



//3 Class6: Class no define


//3 Class7: Class no define


//3 Class8: Testing

//H2C Index: 0xE0
typedef struct _H2CParam_H2C2H_ {
    u8                 H2C2HPara;
}H2CParam_H2C2H, *PH2CParam_H2C2H;

#if 0
typedef struct _H2C_EVT_CONTENT {
    u8                 Content[LENGTH_H2C];
}H2C_EVT_CONTENT, *PH2C_EVT_CONTENT;

typedef struct _H2C_EVT_ {
    H2C_EVT_CONTENT    H2CFWBuff[LENGTH_H2C_BUFF];
    u8                 H2CRPtr;
    u8                 H2CWPtr;
}H2C_EVT, *PH2C_EVT;


typedef struct _C2H_EVT_ {
    C2H_EVT_HDR        c2h_sfbuff[LENGTH_C2H_BUFF];
    u8                 c2h_r_ptr;
    u8                 c2h_w_ptr;
}C2H_EVT, *PC2H_EVT;
#endif 

typedef struct _C2HEXT_EVT_ {
    C2HEXT_EVT_HDR      sfbuff[LENGTH_C2HEXT_BUFF];
    u8                  r_ptr;
    u8                  w_ptr;
    u8                  sfbuff_last_num;    // 寫cmd到 sfbuff時更新的 sfbuff number
    u8                  sfbuff_last_ofset;  // 寫cmd到 sfbuff時更新的 sfbuff offset               
}C2HEXT_EVT, *PC2HEXT_EVT;


#if 0
typedef struct _MEDIA_STATUS_ {
    BOOLEAN             bConnected[MACID_NUM_NEW];
    u8                  MaxConnectedMacid;
}MEDIA_STATUS, *PMEDIA_STATUS;
#endif //#if (!IS_CHIP_8703B(CONFIG_CHIP_SEL))&&(!IS_CHIP_8188F(CONFIG_CHIP_SEL))

/*------------------------Export global variable------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void
InitC2H_8721D_RAM(
    void
);

extern void
C2HDeQueue_8721D_RAM(
    void
);

extern void 
H2CDeQueue_8721D_RAM(
    void
); 

//brian for function both called in isrhdl or not
extern void
SendSignalCommon_8721D(
    u8  event
) ;

extern void
OSSendSignalCommon_8721D(
    u8  event
) ;

extern void
OSSendSignalCommon1(
    u8  event
) ;




extern void
CheckH2CMailboxCommon(
    void
);

extern void
H2CInQueueCommon(
    void
);

extern void 
H2CDeQueueCommon(
    void
) ;





extern void
C2HDeQueueCommon(
    void
) ;

extern void
C2HInQueueCommon(
    PC2H_EVT_HDR pc2h_hdr
) ;

extern void
C2HExtDeQueueCommon(
    void
) ;

extern void
C2HExtInQueueCommon(
    PC2HEXT_EVT_HDR pc2h_hdr
) ;



extern void
CfgTxBufAddrH(
    IN u8   h64,
    IN u8   offset_high
);

extern void
CfgRxBufAddrH(
    //IN u8   h64,
    IN u8   offset_high
);

extern void 
DrvFwIntfCtrl (
    void
);

extern void
MediaConnectionCommon(
    IN u8   macid
);


extern void
MediaDisconnectionCommon(
    IN u8   macid
);

#if 0
extern void
SetMediaStatusCommon(
    IN u8   macid,
    IN u8   status
);
#endif

extern BOOLEAN
GetMediaStatusCommon(
    IN u8   macid
);

extern void
C2HSpeRPT(
    void
);


extern void
Get_BB_Gain_Report(
	void
);	

extern void
C2H_BB_Gain_Report(
   IN 	u8 Efuse_Thermal,
   IN 	u8 Efuse_BB_Gain_2G_TX,
   IN	u8 Efuse_BB_Gain_5GLB_TX,
   IN	u8 Efuse_BB_Gain_5GMB_TX,
   IN	u8 Efuse_BB_Gain_5GHB_TX
);	

#if 0   //CONFIG_Efuse_Hidden_Request
extern void
Get_Efuse_Hidden_Report(
    BOOLEAN H2C_Request
);
#endif //#if CONFIG_Efuse_Hidden_Request

extern void 
H2CCmdFunc_8721D(
    u8  CmdID, 
    u8  *content
) ;

extern void 
H2CHDL_BTWifiCtrl_8721D(
    IN  u8  *pbuf
) ;

#if 0
void 
H2CHDL_WLOpmode (
    IN  u8   *pbuf
) ;

void 
H2CHDL_BTInfo(
    IN  u8  *pbuf
) ;

void 
H2CHDL_BTIgnoreWlanAct (
    IN  u8   *pbuf
) ;

void 
H2CHDL_BTMpH2C(
    IN  u8   *pbuf
);

void 
H2CHDL_BTControl(
    IN  u8   *pbuf
);

void 
H2CHDL_BT_SCO_eSCO_Operation(
    IN  u8   *pbuf
);



extern void 
H2CHDL_BTPSTDMA_8721D(
    IN  u8  *pbuf
) ;


#endif

extern void 
SetTxPauseDRVInfo(
    IN u8 BitMask, 
    IN u8 UserValue
);

extern void
H2CInQueue_8721D_RAM(
    void
);


#endif  //__RTL8721D_CMD_RAM_H__
