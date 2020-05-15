#ifndef __RTL8721D_LTECOEX_RAM_H__
#define __RTL8721D_LTECOEX_RAM_H__

#ifndef __ASSEMBLY__
/*--------------------Define -------------------------------------------*/
#define REG_UART_DLL_CTRL                           0x0000
#define REG_UART_DLM_CTRL                           0x0004
#define REG_UART_Xfactor_CTRL                       0x0008
#define REG_UART_Xfactor_adj_CTRL                   0x000C

#define REG_UART_LINE_CTRL                          0x0010
#define REG_UART_MISC_CTRL                          0x0014
#define REG_BTTHR                                   0x0018  // for BT use
#define REG_BC_To_MWS_CTRL                          0x001C

#define REG_MWS_To_BC_CTRL                          0x0020
#define REG_UART_NRT_RT_LINE_Status                 0x0024

#define REG_MWS_To_BC_NRT_Message                   0x0030
#define REG_MWS_To_BC_RT_Message                    0x0034
#define REG_LTECOEX_CTRL                            0x0038

#define REG_CustomerMode_Realtime_Type_CTRL         0x0040
#define REG_CustomerMode_Realtime_Signal_CTRL       0x0044
#define REG_LTE_Interrupt_Pattern_1                 0x0048                                 
#define REG_LTE_Interrupt_Pattern_2                 0x004C

#define REG_FW_RX_RT_CMD_EN                         0x0050
#define REG_LTECOEX_STATUS                          0x0054
#define REG_LTECOEX_PATTERN_0_CTRL                  0x0058
#define REG_LTECOEX_PATTERN_1_CTRL                  0x005C

#define REG_LTECOEX_PATTERN_2_CTRL                  0x0060
#define REG_LTECOEX_PATTERN_3_CTRL                  0x0064
#define REG_LTECOEX_PATTERN_4_CTRL                  0x0068    
#define REG_LTECOEX_PATTERN_5_CTRL                  0x006C

#define REG_LTECOEX_PATTERN_6_CTRL                  0x0070
#define REG_LTE_SLOT_TIME                           0x0074   
#define REG_LTE_TX_TIMEOUT_CTRL                     0x0078
#define REG_LTE_RX_TIMEOUT_CTRL                     0x007C

#define REG_LTE_FrameSync_SetupTime                 0x0080
#define REG_LTE_RX_SetupTime                        0x0084
#define REG_LTE_TX_SetupTime                        0x0088
#define REG_LTE_Pattern_SetupTime                   0x008C

#define REG_IgnoreTRX_SetupTime                     0x0090

#define REG_WL_TRX_CTRL                             0x00A0
#define REG_BT_TRX_CTRL                             0x00A4
#define REG_WL_Break_LTE_CTRL                       0x00A8
#define REG_BT_Break_LTE_CTRL                       0x00AC

#define REG_LTE_Break_WLAN_CTRL                     0x00B0
#define REG_LTE_Break_BT_CTRL                       0x00B4
 
#define REG_T_LTE_Counter                           0x00C0

#define REG_FW_INT_STATUS_CRTL                      0x0200
#define REG_FW_INT_MASK_CRTL                        0x0204




/*--------------------Define Enum---------------------------------------*/





/*--------------------Export global variable----------------------------*/

/*--------------------Function declaration------------------------------*/

extern u32 
LTECOEX_ReadMACRegDWord(
    IN u16 Addr 
);


extern u16 
LTECOEX_ReadMACRegWord(
    IN u16 Addr    
);


extern u8 
LTECOEX_ReadMACRegByte(
    IN u16 Addr 
);


extern void 
LTECOEX_WriteMACRegDWord(
    IN u16 Addr,
    IN u32 Data 
);

extern void 
LTECOEX_WriteMACRegWord(
    IN u16 Addr,
    IN u16 Data 
);

extern void 
LTECOEX_WriteMACRegByte(
    IN u16 Addr,
    IN u8 Data 
);


extern BOOLEAN
LTECOEX_REGAccessTest(
    void
);

extern BOOLEAN
LTECOEX_UART_LB_Test(
    void
);

extern BOOLEAN
LTECOEX_Pattern_Mode_Test(
    void
);

extern void
LTECOEX_INT_Test(
    void
);

extern void
LTECOEX_Test_HDL(
    void
);

#endif

#endif //#ifndef __RTL8721D_LTECOEX_RAM_H__
