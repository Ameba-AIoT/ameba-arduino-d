#ifndef __RTL8721D_REG_H__
#define __RTL8721D_REG_H__

/*--------------------------Define -------------------------------------------*/

//-----------------------------------------------------
//
//  0x0000h ~ 0x00FFh   System Configuration
//
//-----------------------------------------------------
#if 0
#define REG_WL_CLK_CTRL             0x0002
#define REG_WL_MAC_EN               0x0004
#define REG_WL_PHY_EN               0x0006
#define REG_WL_RF_EN                0x0007
#define REG_WL_PMC_CTRL             0x0020
#define REG_WL_POFF_CTRL            0x0022
#define REG_WL_PIN_FUNC_CTRL0       0x0070
#define REG_WL_PIN_FUNC_CTRL1       0x0074
#define REG_WL_PMC_ISR              0x0084
#define REG_RPWM                    0x008d
#define REG_RPWM2                   0x008e
#define REG_8051FWDL                0x0090
#define REG_MCUTSTCFG               0x0094
#define REG_ROM_VERSION             0x0099
#define REG_SYS_CFG                 0x009a
#define REG_INIT_OFLD_CTRL          REG_DBG_BYTE_6

#else        //mask for no define, ben, 160308
#define REG_SYS_ISO_CTRL            0x0000
#define REG_WL_CLK_CTRL             0x0002
#define REG_WL_FUNC_EN              0x0004
#define REG_WL_PHY_EN               0x0006
#define REG_RF_CTRL                 0x0007
#define REG_CLKDIV                  0x0008
//#define REG_9346CR                  0x000A
//#define REG_EE_VPD                  0x000C
//#define REG_AFE_MISC                0x0010
//#define REG_SPS0_CTRL               0x0011
//#define REG_SPS1_CTRL               0x0018
#define REG_RSV_CTRL                0x001C
//#define REG_RF_CTRL                 0x001F
#define REG_WL_PMC_CTRL             0x0020 
//#define REG_LDOV12D_CTRL            0x0021
//#define REG_LDOHCI12_CTRL           0x0022 
//#define REG_LPLDO_CTRL              0x0023
#define REG_WL_SYSCONFIG            0x0024
//#define REG_AFE_PLL_CTRL            0x0028
//#define REG_APE_PLL_CTRL_EXT        0x002C
#define REG_AFC_CTRL0               0x0030
#define REG_AFE_CTRL1_V1              0x0034					

//#define REG_PWR_DATA                0x0038
//#define REG_CAL_TIMER               0x003C
//#define REG_ACLK_MON                0x003E

//#define REG_GPIO_MUXCFG             0x0040
//#define REG_GPIO_PIN_CTRL           0x0044
//#define REG_GPIO_INTM               0x0048
//#define REG_LEDCFG                  0x004C

//FSIMR
//#define REG_SYSIMR0                 0x0050
//#define REG_SYSIMR1                 0x0051
//#define REG_SYSIMR2                 0x0052
//#define REG_SYSIMR3                 0x0053

//FSISR
//#define REG_SYSISR0                 0x0054
//#define REG_SYSISR1                 0x0055
//#define REG_SYSISR2                 0x0056
//#define REG_SYSISR3                 0x0057


//#define REG_GPIO_EXT_CTRL           0x0060
//#define REG_PAD_CTRL1               0x0064
//#define REG_WL_BT_PWR_CTRL          0x0068
//#define REG_GSSR                    0x006C
#define REG_WL_PIN_FUNC_CTRL0       0x0070
#define REG_WL_PIN_FUNC_CTRL1       0x0074
//#define REG_AFE_XTAL_CTRL_EXT       0x0078
//#define REG_XCK_OUTPUT_CTRL         0x007C    //8188E
#define REG_8051FWDL                0x0080




//0x0088 ~ 0x008F: there are no control circuit. But these register can be R/W.
//**Note**: 
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.

#define REG_INIT_OFLD_CTRL          REG_DBG_BYTE_6

#define REG_WL_LPS_CTRL             0x0090
#define REG_WL_POFF_CTRL            0x0091

#define REG_BOOT_REASON             0x0098
#define REG_RPWM                    0x009C
#define REG_RPWM2                   0x009E
#define REG_SYSON_FSM_MON           0x00A4
#define REG_PMC_DBG_CTL1            0x00A8
#define REG_PMC_DBG_CTL2            0x00AC


//#define REG_EFUSE_DATA0             0x00CC
//#define REG_EFUSE_DATA1             0x00CD
//#define REG_DBG_EN                  0x00CE    //8188E
//#define REG_EPPR                    0x00CF
//#define REG_BIST_SCAN               0x00D0
//#define REG_BIST_RPT                0x00D4
#define REG_BIST_ROM_RPT            0x00D8
#define REG_MEM_CTRL0               0x00E0
#define REG_MEM_CTRL1               0x00E4
#define REG_SDM_IDR_CTRL0           0x00E8
#define REG_SDM_IDR_CTRL1           0x00EC
#define REG_SYS_CFG                 0x00F0
//#define REG_SYS_STATUS_RPT          0x00F4
//#define REG_QINFV                   0x00F8    //8188E
#define REG_SYS_CFG3                0x00FC
#define REG_ROM_VERSION             0x0099
//#define REG_USB_CFG                 0x00FF
#endif

//-----------------------------------------------------
//
//  0x0100h ~ 0x01FFh   MACTOP General Configuration
//
//-----------------------------------------------------
#define REG_FUNC_ENABLE             0x0100
//#define REG_PAGE_SIZE               0x0104    //8812 doesn't use this register to set trx pagesize!!
#define REG_PKT_BUFF_ACCESS_CTRL    0x0106
#define REG_PLAYFORM_CLOCK          0x0109
#define REG_TRX_DMA_CTRL            0x010C
#define REG_SMPS_ENABLE             0x0110
#define REG_TX_PGBNDY               0x0114
#define REG_RX_PGBNDY               0x0116
#define REG_PKTFF_STATUS            0x0118
#define REG_RXFF_PTR                0x011C
#define REG_CPWM                    0x012F

//FWEIMR
#define REG_WLANIMR20                0x0120
#define REG_WLANIMR21                0x0121
#define REG_WLANIMR22                0x0122
#define REG_WLANIMR23                0x0123

//FWEISR
#define REG_WLANISR20                0x0124
#define REG_WLANISR21                0x0125
#define REG_WLANISR22                0x0126
#define REG_WLANISR23                0x0127

//FTEIMR
#define REG_FTIMR20                  0x0128
#define REG_FTIMR21                  0x0129
#define REG_FTIMR22                  0x012A
#define REG_FTIMR23                  0x012B

//FTEISR
#define REG_FTISR20                  0x012C
#define REG_FTISR21                  0x012D
#define REG_FTISR22                  0x012E
#define REG_FTISR23                  0x012F


//FWIMR
#define REG_WLANIMR0                0x0130
#define REG_WLANIMR1                0x0131
#define REG_WLANIMR2                0x0132
#define REG_WLANIMR3                0x0133

//FWISR
#define REG_WLANISR0                0x0134
#define REG_WLANISR1                0x0135
#define REG_WLANISR2                0x0136
#define REG_WLANISR3                0x0137

//FTIMR
#define REG_FTIMR0                  0x0138
#define REG_FTIMR1                  0x0139
#define REG_FTIMR2                  0x013A
#define REG_FTIMR3                  0x013B

//FTISR
#define REG_FTISR0                  0x013C
#define REG_FTISR1                  0x013D
#define REG_FTISR2                  0x013E
#define REG_FTISR3                  0x013F

#define REG_PKTBUF_DBG_CTRL         0x0140
#define REG_PKTBUF_DBG_DATA_L       0x0144
#define REG_PKTBUF_DBG_DATA_H       0x0148
#define REG_CPWM2                   0x014C

//
//  General Purpose Timer (Offset: 0x0150 - 0x016Fh)
//
#define REG_TC0_CTRL                0x0150  //by BT Time Division // instead of PowerTranning
#define REG_TC1_CTRL                0x0154  //by PS or APOffload(AP mode)
#define REG_TC2_CTRL                0x0158  //by PS
#define REG_TC3_CTRL                0x015C  //by PS
#define REG_TC4_CTRL                0x0160  //by Selective Suspend ROF
#define REG_TCUNIT                  0x0164
#define REG_TC5_CTRL                0x0168  //
#define REG_TC6_CTRL                0x016C  //BT TDMA

//#define REG_MREG_HCIDBG             0x0168    //8188E
//#define REG_MREG_CPUDBG             0x016C    //8188E
//0x168 ~ 0x16F: there are no control circuit. But these register can be R/W.
//**Note**: 
//      1.If one of these register is used, we should register MACRO at RTLXXX_debug.h.
//      2.8723BA uses this register as GTimer!!

#define REG_DRF_MBIST_FAIL          0x0170
#define REG_MBIST_START             0x0174
#define REG_MBIST_DONE              0x0178
#define REG_MBIST_FAIL              0x017C
#define REG_AES_DECRPT_DATA         0x0180
#define REG_AES_DECRPT_CFG          0x0184
#define REG_SHA1_DATA               0x0188
#define REG_SHA1_CFG                0x018C

//32K control reg
#define REG_TMCR                    0x0190
#define REG_32KCTRL1                0x0194
#define REG_32KCTRL2                0x0198
#define REG_32KCTRL3                0x019C

//0x1A0 ~ 0x1AF are used for C2H
#define REG_C2H_HEADER              0x01A0
#define REG_C2H_LEN                 0x01AE
#define REG_C2H_TRIGGER             0x01AF

#define REG_HIOE_CTRL               0x01B0
//#define REG_EFUSE_HW_CTRL              0x01B4

//0x1B8 ~ 0x1BF: there are no control circuit. But these register can be R/W.
//**Note**: 
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.


//0x1C0 ~ 0x1C7: there are no control circuit. But these register can be R/W.
//**Note**: 
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.

#define REG_FMETHR                  0x01C8
#define REG_HOST_MESSAGE_INT        0x01CC
#define REG_HOST_RCV_MESSAGE        0x01CF


//0x1D0 ~0x1DF are H2C message box
#define REG_H2C_0                   0x01d0
#define REG_H2C_1                   0x01d4
#define REG_H2C_2                   0x01d8
#define REG_H2C_3                   0x01dc

//LLT
#define REG_LLT_INIT_HDATA          0x01E0
#define REG_LLT_INIT_ADDR           0x01E1
#define REG_LLT_INIT_PDATA          0x01E2
#define REG_LLTE_RWM                0x01E3

//RAM Info
#define REG_RAM_VERSION             0x01E4
#define REG_RAM_SUBVERSION          0x01E5

//BB Indirect Access
#define REG_BB_PHY_CTRL             0x01E8
#define REG_BB_PHY_DATA             0x01EC

//0x1F0~0x1FF are used for H2C extension
#define REG_H2CEXT_0                0x01F0
#define REG_H2CEXT_1                0x01F4
#define REG_H2CEXT_2                0x01F8
#define REG_H2CEXT_3                0x01FC

//-----------------------------------------------------
//
//  0x0200h ~ 0x027Fh   TXDMA Configuration
//
//-----------------------------------------------------
#define REG_RQPN                    0x0200
#define REG_FIFOPAGE                0x0204
#define REG_TDECTL                  0x0208
#define REG_BCN_HEAD                0x0209
#define REG_TXDMA_OFFSET_CHECK      0x020C
#define REG_TXDMA_STATUS            0x0210
#define REG_RQPN_NPQ                0x0214
//#define REG_TXDMA_DBG               0x0218    //8188E
#define REG_AUTO_LLT                0x224

#define REG_H2C_HEAD			    0x0244
#define REG_H2C_TAIL				0x0248
#define REG_H2C_READ_ADDR			0x024C
#define REG_H2C_WR_ADDR				0x0250
#define REG_H2C_INFO				0x0254




//-----------------------------------------------------
//
//  0x0280h ~ 0x02FFh   RXDMA Configuration
//
//-----------------------------------------------------
#define REG_RXDMA_AGG_PG_TH         0x0280
#define REG_WOW_RXPKT_NUM           0x0282
#define REG_RXPKT_NUM               0x0284
#define REG_RXDMA_STATUS            0x0288
#define REG_RXDMA_DBG               0x028C
#define REG_RXDMA_PRO               0x0290
#define REG_C2HPKT                  0x0294

//-----------------------------------------------------
//
// BT_COEX
//
//-----------------------------------------------------
#define REG_BT_COEX_GLB_CTRL        0x02C0
#define REG_BT_COEX_TBL             0x02C4
#define REG_BT_SSI                  0x02D4
#define REG_BT_CSR_ENH_INTF_CTRL    0x02D6
#define REG_BT_ACT_STATISTICS       0x02D8
#define REG_BT_CMD_WLAN_RPT         0x02E0
#define REG_BT_CMD_BT_RPT           0x02E1
#define REG_BT_CMD_LATCH            0x02E2

//0x02F0~ 0x02FF: there are no control circuit. But these register can be R/W.
//**Note**: 
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.


//-----------------------------------------------------
//
//  0x0300h ~ 0x03FFh   PCIe
//
//-----------------------------------------------------
#define REG_DBI_WDATA               0x0348
#define REG_DBI_RDATA               0x034C
#define REG_DBI_CTRL                0x0350
#define REG_DBI_RWCMD               0x0352

#define REG_MDIO_WDATA              0x0354
#define REG_MDIO_RDATA              0x0356
#define REG_MDIO_CTRL               0x0358


#define REG_HRPWM                   0x0361
#define REG_HCPWM                   0x0363

//-----------------------------------------------------
//
//  0x0400h ~ 0x047Fh   Protocol Configuration
//
//-----------------------------------------------------
#define REG_Q0_INFORMATION          0x0400
#define REG_Q1_INFORMATION          0x0404
#define REG_Q2_INFORMATION          0x0408
#define REG_Q3_INFORMATION          0x040C
#define REG_MGQ_INFORMATION         0x0410
#define REG_HGQ_INFORMATION         0x0414
#define REG_BCNQ_INFORMATION        0x0418
#define REG_TX_ACQ_EMPTY            0x041A
#define REG_TX_MGQ_EMPTY            0x041B
#define REG_CPU_MGQ_INFORMATION     0x041C
#define REG_FUNCTION_ENABLE         0x0420
#define REG_HWSEQ_CTRL              0x0423
#define REG_TXPKTBUF_BCNQ_BDNY      0x0424
#define REG_TXPKTBUF_MGQ_BDNY       0x0425
#define REG_LIFETIME_CTRL           0x0426
#define REG_FREE_TAIL               0x0427
#define REG_SPEC0_SIFS              0x0428
#define REG_RL                      0x042A
#define REG_TXBF_CTRL           0x042C
#define REG_DARFRC                  0x0430
#define REG_RARFRC                  0x0438
#define REG_RRSR                    0x0440
#define REG_ARFR0                   0x0444
#define REG_ARFR1                   0x044C
#define REG_CCK_CHECK               0x0454
#define REG_AMPDU_MAX_TIME          0x0456
#define REG_TXPKTBUF_BCNQ_BDNY1     0x0457
#define REG_AMPDU_MAX_LENGTH        0x0458
#define REG_ACQ_STOP                0x045C
#define REG_WMAC_LBK_BUF_HD         0x045D
#define REG_TX_HANG_CTRL            0x045E
#define REG_FAST_EDCA_CTRL      0x0460
#define REG_RD_RESP_PKT_TH      0x0463
#define REG_Q4_INFORMATION          0x0468
#define REG_Q5_INFORMATION          0x046C
#define REG_Q6_INFORMATION          0x0470
#define REG_Q7_INFORMATION          0x0474
#define REG_SPCRPT_CTRL             0x047C
#define REG_SPC_W_PTR               0x047E
#define REG_SPC_R_PTR               0x047F
#define REG_INI_BASIC_CFEND_SEL     0x0481
#define REG_INI_STBC_CFEND_SEL      0x0482
#define REG_DATA_SC                 0x0483
#define REG_MACID_PKT_SLEEP_3       0x0484
#define REG_MACID_PKT_SLEEP_1       0x0488
#define REG_ARFR2                   0x048C
#define REG_ARFR3                   0x0494
//#define REG_ARFR4                   0x049C	// use by FW debug
#define REG_ARFR5                   0x04A4
#define REG_TXRPT_Start_Offset      0x04AC
#define REG_TRYING_CNT_TH           0x04B0
#define REG_POWER_STAGE1            0x04B4
#define REG_POWER_STAGE2            0x04B8
#define REG_SW_AMPDU_BURST_MODE     0x04BC
#define REG_PKT_LIFE_TIME           0x04C0
#define REG_STBC_SETTING            0x04C4
#define REG_QUEUE_CTRL              0x04C6
#define REG_PROT_MODE_CTRL          0x04C8
#define REG_BAR_MODE_CTRL           0x04CC
#define REG_RA_TRY_RATE_AGG_LMT     0x04CF
#define REG_MACID_PKT_SLEEP_2       0x04D0
#define REG_MACID_PKT_SLEEP_0       0x04D4
#define REG_HW_SEQ0                 0x04D8
#define REG_HW_SEQ1                 0x04DA
#define REG_HW_SEQ2                 0x04DC
#define REG_HW_SEQ3                 0x04DE
#define REG_NEED_CPU_HANDLE         0x04E0
#define REG_PTCL_ERR_STATUS         0x04E2
#define REG_TX_PKT_NUM              0x04E3
#define REG_PAGE4_DUMMY             0x04FC

//-----------------------------------------------------
//
//  0x0500h ~ 0x05FFh   EDCA Configuration
//
//-----------------------------------------------------
#define REG_EDCA_VO_PARAM           0x0500
#define REG_EDCA_VI_PARAM           0x0504
#define REG_EDCA_BE_PARAM           0x0508
#define REG_EDCA_BK_PARAM           0x050C
#define REG_BCNTCFG                 0x0510
#define REG_PIFS                    0x0512
#define REG_RDG_PIFS                0x0513
#define REG_SIFS                    0x0514
#define REG_TSFTR_SYN_OFFSET        0x0518
#define REG_AGGR_BREAK_TIME         0x051A
#define REG_SLOT                    0x051B
#define REG_TX_PTCL_CTRL            0x0520
#define REG_TXPAUSE                 0x0522
#define REG_DIS_TXREQ_CLR           0x0523
#define REG_RD_CTRL                 0x0524
#define REG_TX_PTCL_CTRL1           0x0525
#define REG_MBSSID_CTRL             0x0526
#define REG_P2PPS_CTRL              0x0527
#define REG_PKT_LIFETIME_CTRL       0x0528
#define REG_P2PPS_SPEC_STATE        0x052B
#define REG_TBTT_PROHIBIT           0x0540
#define REG_P2PPS_STATE             0x0543  
#define REG_RDG_RD_NAV_NXT          0x0544
#define REG_NAV_PROT_LEN            0x0546
#define REG_BCN_CTRL                0x0550
#define REG_BCN_CTRL1               0x0551
#define REG_MBID_NUM                0x0552
#define REG_DUAL_TSF_RST            0x0553
#define REG_MBSSID_BCN_SPACE        0x0554
#define REG_DRVERLYINT              0x0558
#define REG_BCNDMATIM               0x0559
#define REG_ATIMWND                 0x055A
#define REG_USTIME_TSF              0x055C
#define REG_BCN_MAX_ERR             0x055D
#define REG_RXTSF_OFFSET_CCK        0x055E
#define REG_RXTSF_OFFSET_OFDM       0x055F

#define REG_TSFTR                   0x0560
#define REG_TSFTR1                  0x0568
#define REG_ATIMWND1                0x0570
#define REG_CTWND                   0x0572
#define REG_BCNIVLCUNT              0x0573
#define REG_BCNDROPCTRL             0x0574
#define REG_HGQ_TIMEOUT_PERIOD      0x0575
#define REG_TSFTR2                  0x578

#define REG_PS_TIMER                0x0580
#define REG_PS_TIMER0               0x0584
#define REG_PS_TIMER1               0x0588
#define REG_TBTT_CTN_AREA           0x058C
#define REG_FORCE_BCN_IFS           0x058E
//#define REG_PRE_TX_CTRL             0x0590    //8188E
//#define REG_TXOP_CTRL               0x0592    //8188E
#define REG_ACMHWCTRL               0x05C0
#define REG_ACMRSTCTRL              0x05C1
#define REG_ACMAVG                  0x05C2
#define REG_VO_ADMTIME              0x05C4
#define REG_VI_ADMTIME              0x05C6
#define REG_BE_ADMTIME              0x05C8
#define REG_EDCA_RANDOM_GEN         0x05CC
#define REG_TXCMD_SEL               0x05CF
//#define REG_NOA_PARAM               0x05E0
#define REG_NOA_DURATION            0x05E0
#define REG_NOA_INTERVAL            0x05E4
#define REG_NOA_START_TIME          0x05E8
#define REG_NOA_COUNT               0x05EC
#define REG_TSF_SYNC               0x05F0
#define REG_SCH_TXCMD               0x05F8
#define REG_PAGE5_DUMMY             0x05FC

//-----------------------------------------------------
//
//  0x0600h ~ 0x07FFh   WMAC Configuration
//
//-----------------------------------------------------
#define REG_WMAC_CR                 0x0600
#define REG_TCR                     0x0604
#define REG_RCR                     0x0608
#define REG_RX_PKT_LIMIT            0x060C
#define REG_RX_DLK_TIME             0x060D
#define REG_RX_DRVINFO_SZ       0x060F
#define REG_MACID                   0x0610
#define REG_BSSID                   0x0618
#define REG_MAR                     0x0620
#define REG_MBIDCAMCFG              0x0628

//0x630 ~ 0x637: there are no control circuit. But these register can be R/W.
//**Note**: 
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.
#define REG_AOAC_REASON             0x0630
#define REG_AOAC_DEBUG              0x0634


#define REG_USTIME_EDCA             0x0638
#define REG_MAC_SPEC_SIFS           0x063A
#define REG_RESP_SIFS_CCK       0x063C
#define REG_RESP_SIFS_OFDM      0x063E
#define REG_ACKTO                   0x0640
#define REG_CTS2TO                  0x0641
#define REG_EIFS                    0x0642

//WMA, BA, CCX
#define REG_NAV_CTRL                0x0650
#define REG_BACAMCMD                0x0654
#define REG_BACAMCONTENT            0x0658
#define REG_LBDLY                   0x0660
#define REG_RTR                     0x0662
#define REG_RXERR_RPT               0x0664
#define REG_WMAC_TRXPTCL_CTL        0x0668

// Security
#define REG_CAMCMD                  0x0670
#define REG_CAMWRITE                0x0674
#define REG_CAMREAD                 0x0678
#define REG_CAMDBG                  0x067C
#define REG_SECCFG                  0x0680

// Power
#define REG_WOW_CTRL                0x0690
#define REG_PS_RX_INFO              0x0692
#define REG_WMMPS_UAPSD_TID         0x0693
#define REG_WKFMCAM_NUM             0x0698
#define REG_RXFLTMAP0               0x06A0
#define REG_RXFLTMAP1               0x06A2
#define REG_RXFLTMAP2               0x06A4
#define REG_BCN_PSR_RPT             0x06A8
#define REG_CALB32K_CTRL            0x06AC //spec no description, check RTL!!
#define REG_PKT_MON_CTRL            0x06B4
#define REG_COEX_TABLE1             0x06C0
#define REG_COEX_TABLE2             0x06C4
#define REG_BREAK_TABLE1            0x06C8
#define REG_BREAK_TABLE2            0x06CA
#define REG_ANT_SEL                 0x06CC

#define REG_RX_CSI_RPT_INFO         0x071C
#define REG_BT_COEX                 0x0764
#define REG_WLAN_ACT_MSK_CTRL       0x0768
#define REG_REG_BT_STATISTICS_CTRL  0x076E
#define REG_BT_ACT_STATISTICS_RPT   0x0770
#define REG_BT_STATISTICS_OTH_CTRL  0x0778
#define REG_BT_ISR_CTRL             0x0783
#define REG_BT_ISR_STA              0x078F

#define REG_WL2LTECOEX_Indirect_Access_CTRL	0x7C0
#define REG_WL2LTECOEX_Indirect_Access_WRITE_DATA	0x7C4
#define REG_WL2LTECOEX_Indirect_Access_READ_DATA	0x7C8

//NAN related
#define     REG_NAN_DISC_BCNQ_BDNY      0x0457
#define     REG_NAN_SYNC_BCNQ_BDNY      0x0455
#define     REG_NAN_CAMR_HOP_CNT        0x05DC
#define     REG_NAN_ROLE                0x5DF
#define     REG_FA_FILTER1              0x7D4
#define     REG_FA_FILTER2              0x7D8
#define     REG_NAN_FUNC_CTRL            0x7DC
#define     REG_NAN_ATTRI_ID_VLD        0x7DD
#define     REG_NAN_ATTRI_ID0               0x7DE
#define     REG_NAN_ATTRI_ID1               0x7DF
#define     REG_NAN_ATTRI_ID2               0x7E0
#define     REG_NAN_ATTRI_ID3               0x7E1
#define     REG_NAN_ATTRI_ID4               0x7E2
#define     REG_NAN_ENTRY_FLAG          0x7E3
#define     REG_NAN_START_OFFSET        0x7E4
#define     REG_NAN_RXA2_BITMAP         0x7E8


#define     REG_NAN_INTERFACE_ADDR                  0x2480     // 6 bytes
#define     REG_NAN_RANDOM_FACTOR                          0x2486     // 1 byte
#define     REG_NAN_MASTER_PREF                         0x2487     // 1 byte

//Current Anchor Master Record
#define     REG_NAN_CAMR_L                                      0x2488     // 4 bytes
#define     REG_NAN_CAMR_H                                     0x248C    // 4 byte
#define     REG_NAN_CAMR_AMBTT                         0x2490     // 4 bytes

//Last Anchor Master Record
#define     REG_NAN_LAMR_L                             0x2494     // 4 bytes
#define     REG_NAN_LAMR_H                                     0x2498     // 4 byte
#define     REG_NAN_LAMR_AMBTT                          0x249C    // 4 bytes

//TSF Synced:1bit
#define REG_NAN_TSF_SYNCED                     0x24A0    //BIT0



//-----------------------------------------------------
//
//  0x0800h ~ 0x0FFFh   BB Control Register setting
//
//-----------------------------------------------------
#define REG_RFE_INV                 0x092C
#define REG_RFE_CTRL89              0x0930
#define REG_RFE_SWCTRL              0x0948
#define REG_R_RFE_SEL01             0x0CB0
#define REG_R_RFE_SEL89             0x0CB4

#if 0

//-----------------------------------------------------
//
//  0xFB00h ~ 0xFCFFh   TX/RX packet buffer affress
//
//-----------------------------------------------------
#define REG_RXPKTBUF_STARTADDR      0xFB00
#define REG_TXPKTBUF_STARTADDR      0xFC00

//-----------------------------------------------------
//
//  0xFD00h ~ 0xFDFFh   8051 CPU Local REG
//
//-----------------------------------------------------
#define REG_SYS_CTRL            0xFD00
#define REG_PONSTS_RPT1         0xFD01
#define REG_PONSTS_RPT2         0xFD02
#define REG_PONSTS_RPT3         0xFD03
#define REG_PONSTS_RPT4         0xFD04  //0x94
#define REG_PONSTS_RPT5         0xFD05  //0x95
#define REG_8051ERRFLAG         0xFD08  
#define REG_8051ERRFLAG_MASK    0xFD09
#define REG_TXADDRH             0xFD10  //Tx Packet High Address
#define REG_RXADDRH             0xFD11  //Rx Packet High Address
#define REG_TXADDRH_EXT         0xFD12  //0xFD12[0] : for 8051 access txpktbuf high64k as external register

#define REG_U3_STATE            0xFD48  //(Read only) [7:4] : usb3 changed last state. [3:0]  usb3 state
#endif

//for MAILBOX 0x1160~0x1177
#define REG_OUTDATA0            0x0
#define REG_OUTDATA1            0x4
#define REG_OUTRDY              0x8  //bit[0] : OutReady, bit[1] : OutEmptyIntEn ,bit[7] : SW write 1 to clear OutReady

#define REG_INDATA0             0xC
#define REG_INDATA1             0x10
#define REG_INRDY               0x14  //bit[0] : InReady, bit[1] : InRdyIntEn

#if 0
#define REG_USB_D2F_MSG         0xFD70
#define REG_USB_F2D_MSG         0xFD74
#define REG_PCIE_D2F_MSG        0xFD78
#define REG_PCIE_F2D_MSG        0xFD7C
#define REG_SDIO_D2F_MSG        0xFD80
#define REG_SDIO_F2D_MSG        0xFD84


//MCU ERROR debug REG
#define REG_MCUERR_PCLSB        0xFD90  //PC[7:0]
#define REG_MCUERR_PCMSB        0xFD91  //PC[15:8]
#define REG_MCUERR_ACC          0xFD92
#define REG_MCUERR_B            0xFD93
#define REG_MCUERR_DPTRLSB      0xFD94  //DPTR[7:0]
#define REG_MCUERR_DPTRMSB      0xFD95  //DPTR[15:8]
#define REG_MCUERR_SP           0xFD96  //SP[7:0]
#define REG_MCUERR_IE           0xFD97  //IE[7:0]
#define REG_MCUERR_EIE          0xFD98  //EIE[7:0]
#define REG_VERA_SIM            0xFD9F
//0xFD99~0xFD9F are reserved.. 
#endif

//****************************************************//
//  Do not access 0xFE00~0xFEFF, 0xF000~0xF0FF !!
//****************************************************//
//-----------------------------------------------------
//
//  0xFE00h ~ 0xFEFFh   USB2 Configuration
//
//-----------------------------------------------------
#if 0
#define REG_USB_CTRL            0xFE10

//#define REG_CPWM2_USB2          0xFE34
//#define REG_HRPWM2_USB2         0xFE36

//-----------------------------------------------------
//
//  0xF000h ~ 0xF0FFh   USB3 Configuration
//
//-----------------------------------------------------
//#define REG_HRPWM_USB3          0xF052
//#define REG_CPWM_USB3           0xF053
//#define REG_CPWM2_USB3          0xF05C
//#define REG_HRPWM2_USB3         0xF05E
#endif




//============================================================================
//       8723B Regsiter Bit and Content definition 
//============================================================================
//-----------------------------------------------------
//
//  0x0000h ~ 0x00FFh   System Configuration
//
//-----------------------------------------------------
//2 SPS0_CTRL
#define SW18_FPWM                   BIT3


//2 SYS_ISO_CTRL
#define ISO_MD2PP                   BIT0
#define ISO_UA2USB                  BIT1
#define ISO_UD2CORE                 BIT2
#define ISO_PA2PCIE                 BIT3
#define ISO_PD2CORE                 BIT4
#define ISO_IP2MAC                  BIT5
#define ISO_DIOP                    BIT6
#define ISO_DIOE                    BIT7
#define ISO_EB2CORE                 BIT8
#define ISO_DIOR                    BIT9
#define UA12PC_EN                   BIT10   // For RTL8723 only.
#define UA33PC_EN                   BIT11   // For RTL8723 only.
#define PA12PC_EN                   BIT12   // For RTL8723 only.
#define PA33PC_EN                   BIT13   // For RTL8723 only.
#define PWC_EV25V                   BIT14
#define PWC_EV12V                   BIT15


//2 SYS_FUNC_EN
#define FEN_BBRSTB                  BIT0
#define FEN_BB_GLB_RSTn             BIT1
#define FEN_USBA                    BIT2
#define FEN_UPLL                    BIT3
#define FEN_USBD                    BIT4
#define FEN_DIO_PCIE                BIT5
#define FEN_PCIEA                   BIT6
#define FEN_PPLL                    BIT7
#define FEN_PCIED                   BIT8
#define FEN_DIOE                    BIT9
#define FEN_CPUEN                   BIT10
#define FEN_DCORE                   BIT11
#define FEN_ELDR                    BIT12
#define FEN_DIO_RF                  BIT13
#define FEN_HWPDN                   BIT14
#define FEN_MREGEN                  BIT15

//2 APS_FSMCO
#define PFM_LDALL                   BIT0
#define PFM_ALDN                    BIT1
#define PFM_LDKP                    BIT2
#define PFM_WOWL                    BIT3
#define EnPDN                       BIT4
#define PDN_PL                      BIT5
#define APFM_ONMAC                  BIT8
#define APFM_OFF                    BIT9
#define APFM_RSM                    BIT10
#define AFSM_HSUS                   BIT11
#define AFSM_PCIE                   BIT12
#define APDM_MAC                    BIT13
#define APDM_HOST                   BIT14
#define APDM_HPDN                   BIT15
#define RDY_MACON                   BIT16
#define SUS_HOST                    BIT17
#define ROP_ALD                     BIT20
#define ROP_PWR                     BIT21
#define ROP_SPS                     BIT22
#define SOP_MRST                    BIT25
#define SOP_FUSE                    BIT26
#define SOP_ABG                     BIT27
#define SOP_AMB                     BIT28
#define SOP_RCK                     BIT29
#define SOP_A8M                     BIT30
#define XOP_BTCK                    BIT31

//2 SYS_CLKR
#define ANAD16V_EN                  BIT0
#define ANA8M                       BIT1
#define MACSLP                      BIT4
#define LOADER_CLK_EN               BIT5
#define _80M_SSC_DIS                BIT7
#define _80M_SSC_EN_HO              BIT8
#define PHY_SSC_RSTB                BIT9
#define SEC_CLK_EN                  BIT10
#define MAC_CLK_EN                  BIT11
#define SYS_CLK_EN                  BIT12
#define RING_CLK_EN                 BIT13
#define CPU_CLK_EN                  BIT14


//2 9346CR

#define     BOOT_FROM_EEPROM        BIT4
#define     EEPROM_EN               BIT5


//2 AFE_MISC
#define AFE_BGEN                    BIT0
#define AFE_MBEN                    BIT1
#define MAC_ID_EN                   BIT7


//2 SPS0_CTRL


//2 SPS_OCP_CFG


//2 RSV_CTRL
#define WLOCK_ALL                   BIT0
#define WLOCK_00                    BIT1
#define WLOCK_04                    BIT2
#define WLOCK_08                    BIT3
#define WLOCK_40                    BIT4
#define R_DIS_PRST_0                BIT5
#define R_DIS_PRST_1                BIT6
#define LOCK_ALL_EN                 BIT7

//2 RF_CTRL
#define RF_EN                       BIT0
#define RF_RSTB                     BIT1
#define RF_SDMRSTB                  BIT2



//2 LDOA15_CTRL
#define LDA15_EN                    BIT0
#define LDA15_STBY                  BIT1
#define LDA15_OBUF                  BIT2
#define LDA15_REG_VOS               BIT3
#define _LDA15_VOADJ(x)             (((x) & 0x7) << 4)



//2 LDOV12D_CTRL
#define LDV12_EN                    BIT0
#define LDV12_SDBY                  BIT1
#define LPLDO_HSM                   BIT2
#define LPLDO_LSM_DIS               BIT3
#define _LDV12_VADJ(x)              (((x) & 0xF) << 4)


//2 AFE_XTAL_CTRL
#define XTAL_EN                     BIT0
#define XTAL_BSEL                   BIT1
#define _XTAL_BOSC(x)               (((x) & 0x3) << 2)
#define _XTAL_CADJ(x)               (((x) & 0xF) << 4)
#define XTAL_GATE_USB               BIT8
#define _XTAL_USB_DRV(x)            (((x) & 0x3) << 9)
#define XTAL_GATE_AFE               BIT11
#define _XTAL_AFE_DRV(x)            (((x) & 0x3) << 12)
#define XTAL_RF_GATE                BIT14
#define _XTAL_RF_DRV(x)             (((x) & 0x3) << 15)
#define XTAL_GATE_DIG               BIT17
#define _XTAL_DIG_DRV(x)            (((x) & 0x3) << 18)
#define XTAL_BT_GATE                BIT20
#define _XTAL_BT_DRV(x)             (((x) & 0x3) << 21)
#define _XTAL_GPIO(x)               (((x) & 0x7) << 23)


#define CKDLY_AFE                   BIT26
#define CKDLY_USB                   BIT27
#define CKDLY_DIG                   BIT28
#define CKDLY_BT                    BIT29


//2 AFE_PLL_CTRL
#define APLL_EN                     BIT0
#define APLL_320_EN                 BIT1
#define APLL_FREF_SEL               BIT2
#define APLL_EDGE_SEL               BIT3
#define APLL_WDOGB                  BIT4
#define APLL_LPFEN                  BIT5

#define APLL_REF_CLK_13MHZ          0x1
#define APLL_REF_CLK_19_2MHZ        0x2
#define APLL_REF_CLK_20MHZ          0x3
#define APLL_REF_CLK_25MHZ          0x4
#define APLL_REF_CLK_26MHZ          0x5
#define APLL_REF_CLK_38_4MHZ        0x6
#define APLL_REF_CLK_40MHZ          0x7

#define APLL_320EN                  BIT14
#define APLL_80EN                   BIT15
#define APLL_1MEN                   BIT24


//2 EFUSE_CTRL
#define ALD_EN                      BIT18
#define EF_PD                       BIT19
#define EF_FLAG                     BIT31

//2 EFUSE_TEST (For RTL8723 partially)
#define EF_TRPT                     BIT7
#define EF_CELL_SEL                 (BIT8|BIT9) // 00: Wifi Efuse, 01: BT Efuse0, 10: BT Efuse1, 11: BT Efuse2
#define LDOE25_EN                   BIT31
#define EFUSE_SEL(x)                (((x) & 0x3) << 8)
#define EFUSE_SEL_MASK              0x300
#define EFUSE_WIFI_SEL_0            0x0
#define EFUSE_BT_SEL_0              0x1
#define EFUSE_BT_SEL_1              0x2
#define EFUSE_BT_SEL_2              0x3

#define EFUSE_ACCESS_ON             0x69    // For RTL8723 only.
#define EFUSE_ACCESS_OFF            0x00    // For RTL8723 only.

//2 PWR_DATA 

//2 CAL_TIMER

//2 ACLK_MON
#define RSM_EN                      BIT0
#define Timer_EN                    BIT4


//2 GPIO_MUXCFG
#define TRSW0EN                     BIT2
#define TRSW1EN                     BIT3
#define EROM_EN                     BIT4
#define EnBT                        BIT5
#define EnUart                      BIT8
#define Uart_910                    BIT9
#define EnPMAC                      BIT10
#define SIC_SWRST                   BIT11
#define EnSIC                       BIT12
#define SIC_23                      BIT13
#define EnHDP                       BIT14
#define SIC_LBK                     BIT15

//2 GPIO_PIN_CTRL



//2 GPIO_INTM

//2 LEDCFG
#define LED0PL                      BIT4  
#define LED1PL                      BIT12
#define LED0DIS                     BIT7


//2 FSIMR
//2 FSISR
#define SYSINT_GPIO0_SHT            4//12
#define SYSINT_GPIO1_SHT            5//13
#define SYSINT_GPIO2_SHT            6//14
#define SYSINT_GPIO3_SHT            7//15

#define SYSINT_GPIO4_SHT            0//16
#define SYSINT_GPIO5_SHT            1//17
#define SYSINT_GPIO6_SHT            2//18
#define SYSINT_GPIO7_SHT            3//19
#define SYSINT_GPIO8_SHT            4//20
#define SYSINT_GPIO9_SHT            5//21
#define SYSINT_GPIOA_SHT            6//22
#define SYSINT_GPIOB_SHT            7//23

#define SYSINT_HPON_WRE_SHT         4//28   
#define SYSINT_SPS_OCP_SHT          5//29
#define SYSINT_HWSYS_TIMER_SHT      6//30
#define SYSINT_PDNINT_SHT           7//31


#define SYSINT_GPIO0INT             BIT(SYSINT_GPIO0_SHT)
#define SYSINT_GPIO1INT             BIT(SYSINT_GPIO1_SHT)
#define SYSINT_GPIO2INT             BIT(SYSINT_GPIO2_SHT)
#define SYSINT_GPIO3INT             BIT(SYSINT_GPIO3_SHT)

#define SYSINT_GPIO4INT             BIT(SYSINT_GPIO4_SHT)
#define SYSINT_GPIO5INT             BIT(SYSINT_GPIO5_SHT)
#define SYSINT_GPIO6INT             BIT(SYSINT_GPIO6_SHT)
#define SYSINT_GPIO7INT             BIT(SYSINT_GPIO7_SHT)
#define SYSINT_GPIO8INT             BIT(SYSINT_GPIO8_SHT)
#define SYSINT_GPIO9INT             BIT(SYSINT_GPIO9_SHT)
#define SYSINT_GPIOAINT             BIT(SYSINT_GPIOA_SHT)

#define SYSINT_HPON_WRE             BIT(SYSINT_HPON_WRE_SHT)
#define SYSINT_SPS_OCP              BIT(SYSINT_SPS_OCP_SHT)
#define SYSINT_HWSYS_TIMER          BIT(SYSINT_HWSYS_TIMER_SHT)
#define SYSINT_PDNINT               BIT(SYSINT_PDNINT_SHT)

//2 WLAN CLK CTRL+1      0x0003
#define BIT_TSF_CKSLP_SEL       BIT0

//2 WLAN PHY EN        0x0006
#define BIT_FEN_BB_GLB_RST     BIT0
#define BIT_FEN_BB_RSTB              BIT1

//2WLAN RF EN       0x0007
#define BIT_RF_EN           BIT0
#define BIT_RF_RSTB         BIT1
#define BIT_RF_SDMRSTB      BIT2

//2 WLAN PMC CTRL       0x0020
#define BIT_WL_PMC_LPS_EN   BIT2

//2 MCUFWDL     0x0080
#define MCUFWDL_EN                  BIT0
#define MCUFWDL_RDY                 BIT1
#define MCUFWDL_CHK_RPT             BIT2
//#define MACINI_RDY                  BIT3
//#define BBINI_RDY                   BIT4
//#define RFINI_RDY                   BIT5
#define WINTINI_RDY                 BIT6
#define RAM_DL_SEL                  BIT7


#define MCU_ROM_INITIAL_READY       BIT2    //10

#define ROMDLFW_EN                  BIT3    //11    
#define FWDL2TXBUF_RDY              BIT4    //12
#define MCUFWDL_DMA_EN      BIT5    //13

#define ROM_DLEN                    BIT3    //19
#define CPRST                       BIT7    //23

//2 REG_USB_SIE_INTF        0x00E0
#define USB2_SIE_READ               0x01          
#define USB2_SIE_WRITE              0x03
#define USB3_SIE_READ               0x40
#define USB3_SIE_WRITE              0x42
#define USB_WRITE_END               BIT0
#define USB_SIE_INTF_WE             BIT1


//2 REG_PCIE_MIO_INTF       0x00E4
#define PCIE_MIO_WRITE              0x20
#define PCIE_MIO_READ               0x30


//2 REG_PCIE_MIO_INTD       0x00E8


//2 REG_HPON_FSM
#define BOND92CE_1T2R_CFG           BIT22


//2 REG_SYS_CFG
#define XCLK_VLD                    BIT0
#define ACLK_VLD                    BIT1
#define UCLK_VLD                    BIT2
#define PCLK_VLD                    BIT3
#define PCIRSTB                     BIT4
#define V15_VLD                     BIT5
//#define TRP_B15V_EN                 BIT7
#define INIT_OFFLOAD_ROM_EN         BIT7 
#define SIC_IDLE                    BIT8
#define BD_MAC2                     BIT9
#define BD_MAC1                     BIT10
#define IC_MACPHY_MODE              BIT11
#define CHIP_VER                    (BIT12|BIT13|BIT14|BIT15)
#define BT_FUNC                     BIT16
#define VENDOR_ID                   BIT19
#define PAD_HWPD_IDN                BIT22
#define TRP_VAUX_EN                 BIT23   // RTL ID
#define TRP_BT_EN                   BIT24
#define BD_PKG_SEL                  BIT25
#define BD_HCI_SEL                  BIT26
#define TYPE_ID                     BIT27

#define CHIP_VER_RTL_MASK           0xF000  //Bit 12 ~ 15
#define CHIP_VER_RTL_SHIFT          12

//2REG_GPIO_OUTSTS (For RTL8723 only)
//2 REG_SYS_STATUS_RPT
#define EFS_HCI_SEL                 (BIT0|BIT1)
#define PAD_HCI_SEL                 (BIT2|BIT3)
#define HCI_SEL                     (BIT4|BIT5) //8723BA
#define PKG_SEL_HCI                 BIT6
#define FEN_GPS                     BIT7
#define FEN_BT                      BIT8
#define FEN_WL                      BIT9
#define FEN_PCI                     BIT10
#define FEN_USB                     BIT11
#define BTRF_HWPDN_N                BIT12
#define WLRF_HWPDN_N                BIT13
#define PDN_BT_N                    BIT14
#define PDN_GPS_N                   BIT15
#define BT_CTL_HWPDN                BIT16
#define GPS_CTL_HWPDN               BIT17
#define PPHY_SUSB                   BIT20
#define UPHY_SUSB                   BIT21
#define PCI_SUSEN                   BIT22
#define USB_SUSEN                   BIT23

#define RF_RL_ID                    (BIT31|BIT30|BIT29|BIT28)
#define TRAP_PKG_SEL                BIT5
//#define   HCI_SEL                     (BIT6|BIT7) //8188E

//2 REG_USB_CFG
#define USB2_SEL                    BIT7
//-----------------------------------------------------
//
//  0x0100h ~ 0x01FFh   MACTOP General Configuration
//
//-----------------------------------------------------


//2 Function Enable Registers

#define REG_LBMODE                  (REG_FUNC_ENABLE + 3)


#define HCI_TXDMA_EN                BIT0
#define HCI_RXDMA_EN                BIT1
#define TXDMA_EN                    BIT2
#define RXDMA_EN                    BIT3
#define PROTOCOL_EN                 BIT4
#define SCHEDULE_EN                 BIT5
#define MACTXEN                     BIT6
#define MACRXEN                     BIT7

#define ENSWBCN                     BIT0
#define ENSEC                       BIT1

#define FUNC_EN_TX_PATH             (HCI_TXDMA_EN/*|TXDMA_EN*/|PROTOCOL_EN|SCHEDULE_EN|MACTXEN)
                                    //for dynamic LLT initialization, TXDMA_EN shall not be reset.
#define FUNC_EN_RX_PATH             (HCI_RXDMA_EN|RXDMA_EN|MACRXEN)


// Network type
#define _NETTYPE(x)                 (((x) & 0x3) << 16)
#define MASK_NETTYPE                0x30000
#define NT_NO_LINK                  0x0
#define NT_LINK_AD_HOC              0x1
#define NT_LINK_AP                  0x2
#define NT_AS_AP                    0x3

#define _LBMODE(x)                  (((x) & 0xF) << 24)
#define MASK_LBMODE                 0xF000000
#define LOOPBACK_NORMAL             0x0
#define LOOPBACK_IMMEDIATELY        0xB
#define LOOPBACK_MAC_DELAY          0x3
#define LOOPBACK_PHY                0x1
#define LOOPBACK_DMA                0x7


//2 PLAYFORM_CLOCK Register 0x0109
#define CLK_STABLE_32K              BIT7        // 1: 32K Stable, 0: 40M Stable

//2 PBP - Page Size Register
#define GET_RX_PAGE_SIZE(value)     ((value) & 0xF)
#define GET_TX_PAGE_SIZE(value)     (((value) & 0xF0) >> 4)
#define _PSRX_MASK                  0xF
#define _PSTX_MASK                  0xF0
#define _PSRX(x)                    (x)
#define _PSTX(x)                    ((x) << 4)

#define PBP_64                      0x0
#define PBP_128                     0x1
#define PBP_256                     0x2
#define PBP_512                     0x3
#define PBP_1024                    0x4


//2 TX/RXDMA
#define RXDMA_ARBBW_EN              BIT0
#define RXSHFT_EN                   BIT1
#define RXDMA_AGG_EN                BIT2
#define QS_VO_QUEUE                 BIT8
#define QS_VI_QUEUE                 BIT9
#define QS_BE_QUEUE                 BIT10
#define QS_BK_QUEUE                 BIT11
#define QS_MANAGER_QUEUE            BIT12
#define QS_HIGH_QUEUE               BIT13

#define HQSEL_VOQ                   BIT0
#define HQSEL_VIQ                   BIT1
#define HQSEL_BEQ                   BIT2
#define HQSEL_BKQ                   BIT3
#define HQSEL_MGTQ                  BIT4
#define HQSEL_HIQ                   BIT5

// For normal driver, 0x10C
#define _TXDMA_HIQ_MAP(x)           (((x)&0x3) << 14)
#define _TXDMA_MGQ_MAP(x)           (((x)&0x3) << 12)
#define _TXDMA_BKQ_MAP(x)           (((x)&0x3) << 10)       
#define _TXDMA_BEQ_MAP(x)           (((x)&0x3) << 8 )
#define _TXDMA_VIQ_MAP(x)           (((x)&0x3) << 6 )
#define _TXDMA_VOQ_MAP(x)           (((x)&0x3) << 4 )

#define QUEUE_LOW                   1
#define QUEUE_NORMAL                2
#define QUEUE_HIGH                  3



//2 TRXFF_BNDY


//2 FWIMR / FWISR

#define WLANINT_TXCLOSE_SHT        0
#define WLANINT_TXCCX_SHT          1
#define WLANINT_ERRORHDL_SHT       2
#define WLANINT_RXDONE_SHT         3
#define WLANINT_H2CCMD_SHT         4
#define WLANINT_HRCV_SHT           5
#define WLANINT_CTW_SHT            6
#define WLANINT_LPSTBY_SHT         7

#define WLANINT_BCNDMA0INT_SHT     0//8    
#define WLANINT_BCNDMA1INT_SHT     1//9
#define WLANINT_BCNDMA2INT_SHT     2//10
#define WLANINT_BCNDMA3INT_SHT     3//11
#define WLANINT_BCNDMA4INT_SHT     4//12
#define WLANINT_BCNDMA5INT_SHT     5//13
#define WLANINT_BCNDMA6INT_SHT     6//14
#define WLANINT_BCNDMA7INT_SHT     7//15

#define WLANINT_BCNERLY_SHT        0//16
#define WLANINT_TBTTINT_SHT        1//17
#define WLANINT_BCNRX_SHT          2//18
#define WLANINT_RXBMD1_SHT         3//19
#define WLANINT_RXBMD0_SHT         4//20
#define WLANINT_RXUMD1_SHT         5//21
#define WLANINT_RXUMD0_SHT         6//22
#define WLANINT_TXBCNERR_SHT       7//23

#define WLANINT_TXBCNOK_SHT        0//24
#define WLANINT_TXPKTIN_SHT        1//25
#define WLANINT_WLAN_ACT_ON_SHT    2//26
#define WLANINT_WLAN_ACT_OFF_SHT   3//27
#define WLANINT_TXRPT_CNT_FULL_SHT 4//28
#define WLANINT_TRYDONE_SHT        5//29
#define WLANINT_SOUND_DONE_SHT     6//30
//#define WLANINT_VOQINT_SHT         7//31



// 0~7
#define WLANINT_TXCLOSE             BIT(WLANINT_TXCLOSE_SHT)
#define WLANINT_TXCCX               BIT(WLANINT_TXCCX_SHT)
#define WLANINT_ERRORHDL            BIT(WLANINT_ERRORHDL_SHT)
#define WLANINT_RXDONE              BIT(WLANINT_RXDONE_SHT)
#define WLANINT_H2CCMD              BIT(WLANINT_H2CCMD_SHT)
#define WLANINT_HRCV                BIT(WLANINT_HRCV_SHT)
#define WLANINT_CTW                 BIT(WLANINT_CTW_SHT)        //In 88C/8723 test chip, it is shared by CTWindow and ATIMWindow. In 8723 Normal Chip, CTWindow and ATIMWindow are separated.
#define WLANINT_LPSTBY              BIT(WLANINT_LPSTBY_SHT)

// 8~15
#define WLANINT_BCNDMA0INT          BIT(WLANINT_BCNDMA0INT_SHT)
#define WLANINT_BCNDMA1INT          BIT(WLANINT_BCNDMA1INT_SHT)
#define WLANINT_BCNDMA2INT          BIT(WLANINT_BCNDMA2INT_SHT)
#define WLANINT_BCNDMA3INT          BIT(WLANINT_BCNDMA3INT_SHT)
#define WLANINT_BCNDMA4INT          BIT(WLANINT_BCNDMA4INT_SHT)
#define WLANINT_BCNDMA5INT          BIT(WLANINT_BCNDMA5INT_SHT)
#define WLANINT_BCNDMA6INT          BIT(WLANINT_BCNDMA6INT_SHT)
#define WLANINT_BCNDMA7INT          BIT(WLANINT_BCNDMA7INT_SHT)

//16~23
#define WLANINT_BCNERLY             BIT(WLANINT_BCNERLY_SHT)
#define WLANINT_TBTTINT             BIT(WLANINT_TBTTINT_SHT)
#define WLANINT_BCNRX               BIT(WLANINT_BCNRX_SHT)
#define WLANINT_RXBMD1              BIT(WLANINT_RXBMD1_SHT)
#define WLANINT_RXBMD0              BIT(WLANINT_RXBMD0_SHT)
#define WLANINT_RXUMD1              BIT(WLANINT_RXUMD1_SHT)
#define WLANINT_RXUMD0              BIT(WLANINT_RXUMD0_SHT  )
#define WLANINT_TXBCNERR            BIT(WLANINT_TXBCNERR_SHT)


//24~31
#define WLANINT_TXBCNOK             BIT(WLANINT_TXBCNOK_SHT)
#define WLANINT_TXPKTIN             BIT(WLANINT_TXPKTIN_SHT)
#define WLANINT_WLAN_ACT_ON         BIT(WLANINT_WLAN_ACT_ON_SHT)
#define WLANINT_WLAN_ACT_OFF        BIT(WLANINT_WLAN_ACT_OFF_SHT)
#define WLANINT_TXRPT_CNT_FULL      BIT(WLANINT_TXRPT_CNT_FULL_SHT)
#define WLANINT_TRYDONE             BIT(WLANINT_TRYDONE_SHT)
#define WLANINT_SOUND_DONE          BIT(WLANINT_SOUND_DONE_SHT)
//#define WLANINT_VOQINT              BIT(WLANINT_VOQINT_SHT)


//3 FWIMR2 / FWISR2

#define WLANINT2_SYNCBCN_RXOK_SHT     0//8
#define WLANINT2_TBTT1_SHT            5//13
#define WLANINT2_DW_EARLY_SHT         6//14
#define WLANINT2_DW_END_SHT           7//15

#define WLANINT2_SYNCBCN_TXERR_SHT    0//16
#define WLANINT2_SYNCBCN_TXOK_SHT     1//17
#define WLANINT2_EQ_EARLY_SHT         2//18
#define WLANINT2_EQ_TXOK_SHT          3//19
#define WLANINT2_DDMA_HP_DONE_SHT     4//20 DDMA channel 0 and 1
#define WLANINT2_DDMA_LP_DONE_SHT     5//21 DDMA channel 2,3,4,5
#define WLANINT2_HIOE_DONE_SHT        6//22
#define WLANINT2_GNT_BT_RISE_SHT      7//23

#define WLANINT2_GNT_BT_FALL_SHT      0//24
#define WLANINT2_SCORE_BOARD_SHT      1//25

#define WLANINT2_1_WIRED_MAILBOX_SHT  3//27






//0~7


//8~15
#define WLANINT2_SYNCBCN_RXOK           BIT(WLANINT2_SYNCBCN_RXOK_SHT)
#define WLANINT2_TBTT1                  BIT(WLANINT2_TBTT1_SHT)
#define WLANINT2_DW_EARLY               BIT(WLANINT2_DW_EARLY_SHT)
#define WLANINT2_DW_END                 BIT(WLANINT2_DW_END_SHT)

//16~23
#define WLANINT2_SYNCBCN_TXERR          BIT(WLANINT2_SYNCBCN_TXERR_SHT)
#define WLANINT2_SYNCBCN_TXOK           BIT(WLANINT2_SYNCBCN_TXOK_SHT)
#define WLANINT2_EQ_EARLY               BIT(WLANINT2_EQ_EARLY_SHT)
#define WLANINT2_EQ_TXOK                BIT(WLANINT2_EQ_TXOK_SHT)
#define WLANINT2_DDMA_HP_DONE           BIT(WLANINT2_DDMA_HP_DONE_SHT)
#define WLANINT2_DDMA_LP_DONE           BIT(WLANINT2_DDMA_LP_DONE_SHT)
#define WLANINT2_HIOE_DONE              BIT(WLANINT2_HIOE_DONE_SHT)
#define WLANINT2_GNT_BT_RISE            BIT(WLANINT2_GNT_BT_RISE_SHT)

//24~31
#define WLANINT2_GNT_BT_FALL            BIT(WLANINT2_GNT_BT_FALL_SHT)
#define WLANINT2_SCORE_BOARD            BIT(WLANINT2_SCORE_BOARD_SHT)

#define WLANINT2_1_WIRED_MAILBOX        BIT(WLANINT2_1_WIRED_MAILBOX_SHT)




//2 FTIMR / FTISR
#define FTINT_GTINT0_SHT            0
#define FTINT_GTINT1_SHT            1
#define FTINT_GTINT2_SHT            2
#define FTINT_GTINT3_SHT            3
#define FTINT_GTINT4_SHT            4
#define FTINT_TIMEOUT0_SHT          5
#define FTINT_TIMEOUT1_SHT          6
#define FTINT_PSTIMER_SHT           7

#define FTINT_RPWMINT_SHT           0//8
#define FTINT_BCNRX_E_SHT           1//9
#define FTINT_EOSP_SHT              2//10
#define FTINT_GTINT5_SHT            3//11
#define FTINT_BCNERLY1_SHT          4//12
#define FTINT_ATIM1_SHT             5//13


#define FTINT_P2P_RFON_SHT          0//16
#define FTINT_P2P_RFOFF_SHT         1//17
#define FTINT_BTCMD_SHT             2//18
#define FTINT_TRIGGER_PKT_SHT       3//19
#define FTINT_TSF_BIT32TGL_SHT      4//20
#define FTINT_RPWMINT2_SHT          5//21
#define FTINT_RETRIVE_BUFFERED_SHT  6//22
#define FTINT_CLR_PS_STATUS_SHT     7//23


#define FTINT_C2H_W_READY_SHT       1//25
#define FTINT_RX_WAKEUP_PKT_SHT     2//26
#define FTINT_ATIM_SHT              3//27
#define FTINT_BCN_ELY_AGG_SHT       4//28
#define FTINT_TXNULL0_SHT           5//29
#define FTINT_TXNULL1_SHT           6//30
#define FTINT_GTINT6_SHT            7//31

//0~7
#define FTINT_GTINT0            BIT(FTINT_GTINT0_SHT)
#define FTINT_GTINT1            BIT(FTINT_GTINT1_SHT)
#define FTINT_GTINT2            BIT(FTINT_GTINT2_SHT)
#define FTINT_GTINT3            BIT(FTINT_GTINT3_SHT)
#define FTINT_GTINT4            BIT(FTINT_GTINT4_SHT)
#define FTINT_TIMEOUT0          BIT(FTINT_TIMEOUT0_SHT)
#define FTINT_TIMEOUT1          BIT(FTINT_TIMEOUT1_SHT)
#define FTINT_PSTIMER           BIT(FTINT_PSTIMER_SHT)  

//8~15
#define FTINT_RPWMINT           BIT(FTINT_RPWMINT_SHT)
#define FTINT_BCNRX_E           BIT(FTINT_BCNRX_E_SHT)
#define FTINT_EOSP              BIT(FTINT_EOSP_SHT)
#define FTINT_GTINT5            BIT(FTINT_GTINT5_SHT)
#define FTINT_BCNERLY1          BIT(FTINT_BCNERLY1_SHT)
#define FTINT_ATIM1             BIT(FTINT_ATIM1_SHT)

//16~23
#define FTINT_P2P_RFON          BIT(FTINT_P2P_RFON_SHT)
#define FTINT_P2P_RFOFF         BIT(FTINT_P2P_RFOFF_SHT)
#define FTINT_BTCMD             BIT(FTINT_BTCMD_SHT)
#define FTINT_TRIGGER_PKT       BIT(FTINT_TRIGGER_PKT_SHT)
#define FTINT_TSF_BIT32TGL      BIT(FTINT_TSF_BIT32TGL_SHT)
#define FTINT_RPWMINT2          BIT(FTINT_RPWMINT2_SHT)
#define FTINT_RETRIVE_BUFFERED  BIT(FTINT_RETRIVE_BUFFERED_SHT)
#define FTINT_CLR_PS_STATUS     BIT(FTINT_CLR_PS_STATUS_SHT)

//24~31
#define FTINT_C2H_W_READY       BIT(FTINT_C2H_W_READY_SHT)
#define FTINT_RX_WAKEUP_PKT     BIT(FTINT_RX_WAKEUP_PKT_SHT)
#define FTINT_ATIM              BIT(FTINT_ATIM_SHT)     //Added in 8723 Normal Chip.
#define FTINT_BCN_ELY_AGG       BIT(FTINT_BCN_ELY_AGG_SHT)
#define FTINT_TXNULL0           BIT(FTINT_TXNULL0_SHT)  //Added in 8179 test chip
#define FTINT_TXNULL1           BIT(FTINT_TXNULL1_SHT)  //Added in 8179 test chip
#define FTINT_GTINT6            BIT(FTINT_GTINT6_SHT)

//3 FTIMR2 / FTISR2

#define FTINT2_BT_MAILBOX_SHT   0 //0


//0~7
#define FTINT2_BT_MAILBOX       BIT(FTINT2_BT_MAILBOX_SHT)

//8~15


//16~23


//2 General Purpose Timer (Offset: 0x0150 - 0x016Fh)
#define MODE_TIMER                  1
#define MODE_COUNTER                0

#define TC_ENABLE                   BIT0
#define TC_TIMER_MODE               BIT1        


//2 HMETFR
#define H2CINTBOX_BITMASK           0xF
#define H2CINTBOX_NUMMASK           0x3


//2 LLT_INIT
#define _LLT_NO_ACTIVE              0x00
#define _LLT_WRITE_ACCESS           0x40
#define _LLT_READ_ACCESS            0x80

#define _LLT_INIT_DATA(x)           ((x) & 0xFF)
#define _LLT_INIT_ADDR(x)           (((x) & 0xFF) << 8)
#define _LLT_OP(x)                  (((x) & 0x3) << 30)
#define _LLT_OP_VALUE(x)            (((x) >> 30) & 0x3)



//2 BB_ACCESS_CTRL
#define BB_WRITE_READ_MASK          (BIT31 | BIT30)
#define BB_WRITE_EN                 BIT30
#define BB_READ_EN                  BIT31
//#define BB_ADDR_MASK              0xFFF
//#define _BB_ADDR(x)                   ((x) & BB_ADDR_MASK)

//-----------------------------------------------------
//
//  0x0200h ~ 0x027Fh   TXDMA Configuration
//
//-----------------------------------------------------
//2 RQPN
#define _HPQ(x)                     ((x) & 0xFF)
#define _LPQ(x)                     (((x) & 0xFF) << 8)
#define _PUBQ(x)                    (((x) & 0xFF) << 16)
#define _NPQ(x)                     ((x) & 0xFF)            // NOTE: in RQPN_NPQ register


#define HPQ_PUBLIC_DIS              BIT24
#define LPQ_PUBLIC_DIS              BIT25
#define LD_RQPN                     BIT31


//2 TDECTRL
#define BCN_VALID                   BIT16
#define BCN_HEAD(x)                 (((x) & 0xFF) << 8)
#define BCN_HEAD_MASK               0xFF00

//2 TDECTL
#define BLK_DESC_NUM_SHIFT          4
#define BLK_DESC_NUM_MASK           0xF


//2 TXDMA_OFFSET_CHK
#define DROP_DATA_EN                BIT9


//2 RX pkt unm  0x286
#define RXDMA_IDLE                  BIT1
#define STOP_RXDMA                  BIT2



//-----------------------------------------------------
//
//  0x0300h ~ 0x03FFh   PCIE 
//
//-----------------------------------------------------
//2  REG_DBI_CTRL                   0x0350

//2 REG_DBI_RWCMD               0x0352
#define DBI_WRITE                   BIT0
#define DBI_READ                    BIT1


//2 REG_MDIO_CTRL               0x0358
#define MDIO_PCIE_READ              BIT6
#define MDIO_PCIE_WRITE             BIT5

//-----------------------------------------------------
//
//  0x0400h ~ 0x047Fh   Protocol Configuration
//
//-----------------------------------------------------

//2 CPU_MGT_INFORMATION
#define CPUMGT_POLL                 BIT5
#define P0BCN_POLL                  BIT4
#define P1BCN_POLL                  BIT6
#define NANBCN_POLL                 BIT7


//2 FWHW_TXQ_CTRL
#define EN_AMPDU_RTY_NEW            BIT7

//2 INIRTSMCS_SEL
#define _INIRTSMCS_SEL(x)           ((x) & 0x3F)


//2 SPEC SIFS
#define _SPEC_SIFS_CCK(x)           ((x) & 0xFF)
#define _SPEC_SIFS_OFDM(x)          (((x) & 0xFF) << 8)


//2 RRSR

#define RATE_REG_BITMAP_ALL         0xFFFFF

#define _RRSC_BITMAP(x)             ((x) & 0xFFFFF)

#define _RRSR_RSC(x)                (((x) & 0x3) << 21)
#define RRSR_RSC_RESERVED           0x0
#define RRSR_RSC_UPPER_SUBCHANNEL   0x1
#define RRSR_RSC_LOWER_SUBCHANNEL   0x2
#define RRSR_RSC_DUPLICATE_MODE     0x3


//2 ARFR
#define USE_SHORT_G1                BIT20

//2 AGGLEN_LMT_L
#define _AGGLMT_MCS0(x)             ((x) & 0xF)
#define _AGGLMT_MCS1(x)             (((x) & 0xF) << 4)
#define _AGGLMT_MCS2(x)             (((x) & 0xF) << 8)
#define _AGGLMT_MCS3(x)             (((x) & 0xF) << 12)
#define _AGGLMT_MCS4(x)             (((x) & 0xF) << 16)
#define _AGGLMT_MCS5(x)             (((x) & 0xF) << 20)
#define _AGGLMT_MCS6(x)             (((x) & 0xF) << 24)
#define _AGGLMT_MCS7(x)             (((x) & 0xF) << 28)

//3 LIFETIME_CTRL
#define BT_INT_MCU                  BIT7
#define BT_INT_PTA                  BIT6

//2 RL
#define RETRY_LIMIT_SHORT_SHIFT     8
#define RETRY_LIMIT_LONG_SHIFT      0


//2 DARFRC
#define _DARF_RC1(x)                ((x) & 0x1F)
#define _DARF_RC2(x)                (((x) & 0x1F) << 8)
#define _DARF_RC3(x)                (((x) & 0x1F) << 16)
#define _DARF_RC4(x)                (((x) & 0x1F) << 24)
// NOTE: shift starting from address (DARFRC + 4)
#define _DARF_RC5(x)                ((x) & 0x1F)
#define _DARF_RC6(x)                (((x) & 0x1F) << 8)
#define _DARF_RC7(x)                (((x) & 0x1F) << 16)
#define _DARF_RC8(x)                (((x) & 0x1F) << 24)


//2 RARFRC
#define _RARF_RC1(x)                ((x) & 0x1F)
#define _RARF_RC2(x)                (((x) & 0x1F) << 8)
#define _RARF_RC3(x)                (((x) & 0x1F) << 16)
#define _RARF_RC4(x)                (((x) & 0x1F) << 24)
// NOTE: shift starting from address (RARFRC + 4)
#define _RARF_RC5(x)                ((x) & 0x1F)
#define _RARF_RC6(x)                (((x) & 0x1F) << 8)
#define _RARF_RC7(x)                (((x) & 0x1F) << 16)
#define _RARF_RC8(x)                (((x) & 0x1F) << 24)




//-----------------------------------------------------
//
//  0x0500h ~ 0x05FFh   EDCA Configuration
//
//-----------------------------------------------------



//2 EDCA setting
#define AC_PARAM_TXOP_LIMIT_OFFSET  16
#define AC_PARAM_ECW_MAX_OFFSET     12
#define AC_PARAM_ECW_MIN_OFFSET     8
#define AC_PARAM_AIFS_OFFSET        0


//2 EDCA_VO_PARAM
#define _AIFS(x)                    (x)
#define _ECW_MAX_MIN(x)             ((x) << 8)
#define _TXOP_LIMIT(x)              ((x) << 16)


#define _BCNIFS(x)                  ((x) & 0xFF)
#define _BCNECW(x)                  (((x) & 0xF))<< 8)


#define _LRL(x)                     ((x) & 0x3F)
#define _SRL(x)                     (((x) & 0x3F) << 8)


//2 SIFS_CCK
#define _SIFS_CCK_CTX(x)            ((x) & 0xFF)
#define _SIFS_CCK_TRX(x)            (((x) & 0xFF) << 8);


//2 SIFS_OFDM
#define _SIFS_OFDM_CTX(x)           ((x) & 0xFF)
#define _SIFS_OFDM_TRX(x)           (((x) & 0xFF) << 8);


//2 TXPAUSE 0x522
#define MAC_STOPBK                  BIT0
#define MAC_STOPBE                  BIT1
#define MAC_STOPVI                  BIT2
#define MAC_STOPVO                  BIT3
#define MAC_STOPMGQ                 BIT4
#define MAC_STOPHIQ                 BIT5
#define MAC_STOPBCNQ                BIT6
#define MAC_STOP_BCN_HI_MGT         BIT7


//2 TBTT PROHIBIT
#define _TBTT_PROHIBIT_HOLD(x)      (((x) & 0xFF) << 8)


//2 REG_RD_CTRL 0x524
#define DIS_EDCA_CNT_DWN            BIT11

//2 MBSSID_CTRL 0x526

//2 P2PPS_CTRL 0x527
#define P2P_BCN_SEL                 BIT0
#define P2P_NOA0_EN                 BIT1
#define P2P_NOA1_EN                 BIT2
#define P2P_CTWND_EN                BIT3
#define P2P_BCN_AREA_EN             BIT4
#define P2P_PWR_MGT_EN              BIT5
#define P2P_OFF_DISTX_EN            BIT6
#define P2P_CTW_ALLSTASLEEP         BIT7


//2 P2PPS_SPEC_STATE 0x52B
#define P2P_SPEC_FORCE_DOZE0        BIT0
#define P2P_SPEC_NOA0_OFF_PERIOD    BIT1
#define P2P_SPEC_FORCE_DOZE1        BIT2
#define P2P_SPEC_NOA1_OFF_PERIOD    BIT3
#define P2P_CTWIN_EARLY_DISTX       BIT4
#define P2P_SPEC_BEACON_AREA_ON     BIT5
#define P2P_SPEC_CTW_ON             BIT6
#define P2P_SPEC_POWER_STATE        BIT7        // 1: on, 0: off


//2 P2PPS_STATE 0x543
#define P2P_FORCE_DOZE0             BIT0
#define P2P_NOA0_OFF_PERIOD         BIT1
#define P2P_FORCE_DOZE1             BIT2
#define P2P_NOA1_OFF_PERIOD         BIT3
#define P2P_NOA_POWEROFF            BIT4
#define P2P_BEACON_AREA_ON          BIT5
#define P2P_CTW_ON                  BIT6
#define P2P_POWER_STATE             BIT7        // 1: on, 0: off


//2 BCN_CTRL
#define EN_MBSSID                   BIT1
#define EN_TXBCN_RPT                BIT2
#define EN_BCN_FUNCTION             BIT3

//2 DUAL_TSF_RST
#define TSFTR_RST                   BIT0
#define TSFTR1_RST                  BIT1


#define STOP_BCNQ                   BIT6

// The same function but different bit field.
#define DIS_TSF_UDT0_NORMAL_CHIP    BIT4
#define DIS_TSF_UDT0_TEST_CHIP      BIT5

//2 ACMHWCTRL
#define AcmHw_HwEn                  BIT0
#define AcmHw_BeqEn                 BIT1
#define AcmHw_ViqEn                 BIT2
#define AcmHw_VoqEn                 BIT3
#define AcmHw_BeqStatus             BIT4
#define AcmHw_ViqStatus             BIT5
#define AcmHw_VoqStatus             BIT6



//-----------------------------------------------------
//
//  0x0600h ~ 0x07FFh   WMAC Configuration
//
//-----------------------------------------------------

//2 APSD_CTRL
#define APSDOFF                     BIT6
#define APSDOFF_STATUS              BIT7


//2 BWOPMODE
#define BW_20MHZ                    BIT2
//#define BW_OPMODE_20MHZ               BIT2  // For compability


#define RATE_BITMAP_ALL             0xFFFFF

// Only use CCK 1M rate for ACK
#define RATE_RRSR_CCK_ONLY_1M       0xFFFF1

//2 TCR
#define TSFRST                      BIT0
#define DIS_GCLK                    BIT1
#define PAD_SEL                     BIT2
#define PWR_ST                      BIT6
#define PWRBIT_OW_EN                BIT7
#define ACRC                        BIT8
#define CFENDFORM                   BIT9
#define ICV                         BIT10



//2 RCR
#define AAP                         BIT0
#define APM                         BIT1
#define AM                          BIT2
#define AB                          BIT3
#define ADD3                        BIT4
#define APWRMGT                     BIT5
#define CBSSID                      BIT6
#define CBSSID_DATA                 BIT6
#define CBSSID_BCN                  BIT7
#define ACRC32                      BIT8
#define AICV                        BIT9
#define ADF                         BIT11
#define ACF                         BIT12
#define AMF                         BIT13
#define HTC_LOC_CTRL                BIT14
#define UC_DATA_EN                  BIT16
#define BM_DATA_EN                  BIT17
#define MFBEN                       BIT22
#define LSIGEN                      BIT23
#define EnMBID                      BIT24
#define APP_BASSN                   BIT27
#define APP_PHYSTS                  BIT28
#define APP_ICV                     BIT29
#define APP_MIC                     BIT30
#define APP_FCS                     BIT31

//2 RX_PKT_LIMIT

//2 RX_DLK_TIME

//2 MBIDCAMCFG

//2 WOW_CTRL 0x690
#define FORCE_WAKE_UP               BIT0
#define PATTERN_MATCH_EN            BIT1
#define MAGIC_PKT_EN                BIT2
#define UNICAST_EN                  BIT3
#define WOWHCI_EN                   BIT5

//2 AMPDU_MIN_SPACE
#define _MIN_SPACE(x)               ((x) & 0x7)
#define _SHORT_GI_PADDING(x)        (((x) & 0x1F) << 3)


//2 RXERR_RPT
#define RXERR_TYPE_OFDM_PPDU            0
#define RXERR_TYPE_OFDM_FALSE_ALARM     1
#define RXERR_TYPE_OFDM_MPDU_OK         2
#define RXERR_TYPE_OFDM_MPDU_FAIL       3
#define RXERR_TYPE_CCK_PPDU             4
#define RXERR_TYPE_CCK_FALSE_ALARM      5
#define RXERR_TYPE_CCK_MPDU_OK          6
#define RXERR_TYPE_CCK_MPDU_FAIL        7
#define RXERR_TYPE_HT_PPDU              8
#define RXERR_TYPE_HT_FALSE_ALARM       9
#define RXERR_TYPE_HT_MPDU_TOTAL        10
#define RXERR_TYPE_HT_MPDU_OK           11
#define RXERR_TYPE_HT_MPDU_FAIL         12
#define RXERR_TYPE_RX_FULL_DROP         15

#define RXERR_COUNTER_MASK              0xFFFFF
#define RXERR_RPT_RST                   BIT27
#define _RXERR_RPT_SEL(type)            ((u32)((type) << 28))


//2 SECCFG
#define SCR_TxUseDK                     BIT0            //Force Tx Use Default Key
#define SCR_RxUseDK                     BIT1            //Force Rx Use Default Key
#define SCR_TxEncEnable                 BIT2            //Enable Tx Encryption
#define SCR_RxDecEnable                 BIT3            //Enable Rx Decryption
#define SCR_SKByA2                      BIT4            //Search kEY BY A2
#define SCR_NoSKMC                      BIT5            //No Key Search Multicast
#define SCR_TXBCUSEDK                   BIT6            // Force Tx Broadcast packets Use Default Key
#define SCR_RXBCUSEDK                   BIT7            // Force Rx Broadcast packets Use Default Key


//1 BCN_PSR_RPT [31:0]: Beacon Parser Report Register
//0~10      AID0
//11~13     RSVD
#define PS_TIM                          BIT6      //14
#define PS_DTIM                         BIT7      //15
//16~23     DTIM period
//24~31     DTIM COUNT


//1NAN CTRL
#define NAN_ENTRY_SIZE  64
#define NAN_ENTRY_NUM   1

#define NAN_PSR_EN      BIT0
#define _NAN_ENTRY_SIZE(size)   ((u8)(((((size)>>5)-2)&0x7)<<1))
#define _NAN_ENTRY_NUM(num)     ((u8)(((num)&0x7)<<4))
#define NAN_ATTRI_ID0_VLD   BIT0
#define NAN_ATTRI_ID1_VLD   BIT1
#define NAN_ATTRI_ID2_VLD   BIT2
#define NAN_ATTRI_ID3_VLD   BIT3
#define NAN_ATTRI_ID4_VLD   BIT4
#define NAN_RXA2_FILTER_EN  BIT5

//1 CPU DOMAIN
//-----------------------------------------------------
//
//  0xFD00h ~ 0xFDFFh   8051 CPU Local REG
//
//-----------------------------------------------------

//3 REG_SYS_CTRL(0xFD00)
//0xFD00[0]: 8051 sw reset for fpga use
//0xFD00[1:2] : rsvd
//0xFD00[3]: write reg 00[4] to trigger int pkt
#define WAKEUP_BUS                      BIT4    //dw8051 wakeup usb2/3
//0xFD00[5] : rsvd
#define SYS_CTRL_TXBUF_ROM_ENABLE       BIT6    //enable this bit when 8051 want to run instruntion and disable while leaving.       
//0xFD00[7]: auto clr 0xFD20 ~ 0xFD3F reg

//3 REG_PONSTS_RPT1(0xFD01)
#define PONSTS1_FW_DATA_RDY             BIT0
#define PONSTS1_HCI_SEL                 BIT1    //usb/pcie hci sel(0:pcie, 1:usb)    
#define PONSTS1_USB2_SEL                BIT2    //usb2 card emulation to usb2
#define PONSTS1_EEPROM_LOAD_OK          BIT3    //reg0x0A[5]
#define PONSTS1_MCUFWDL_RDY             BIT4    //reg0x80[1]
#define PONSTS1_MCUFWDL_EN              BIT5    //reg0x80[0]
#define PONSTS1_USB_BUS_ACTIVE          BIT6    //usb suspend
#define PONSTS1_RAM_SPACE_SEL           BIT7

//3 REG_PONSTS_RPT2(0xFD02)
//0xFD02[7:4]       : ICFG              (0xF0[31:28])
//0xFD02[3:1]       : INI_RDY           (0x80[5:3])
//0xFD02[0]         : MCU_RSTST         (0x80[23])

//3 REG_PONSTS_RPT3(0xFD03)
//0xFD03[7]         : POR_CORE          (0x00[27])
//0xFD03[6]         : MACREG_EN         (0x00[31])
//0xFD03[5]         : PCLK_VLD          (0xF0[3])
//0xFD03[4]         : UCLK_VLD          (0xF0[2])
//0xFD03[3]         : ACLK_VLD          (0xF0[1])
//0xFD03[2]         : HCI_SEL           (0xF0[26])
//0xFD03[1]         : SIC_IDLE          (0xF0[8])
//0xFD03[0]         : MAC_RDY           (0x04[16])


/*------------------------------Define Enum-----------------------------------*/


/*--------------------------Define MACRO--------------------------------------*/

/*------------------------------Define Struct---------------------------------*/

/*------------------------Export global variable------------------------------*/

/*------------------------------Function declaration--------------------------*/


#endif  //__RTL8721D_REG_H__

