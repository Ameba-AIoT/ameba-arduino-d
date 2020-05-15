#ifndef __RTL8721D_FWLBK_H__
#define __RTL8721D_FWLBK_H__

/*--------------------------Define -------------------------------------------*/
#define REG_FWLBK_CTRL_EXT                  REG_DBG_BYTE_9
#define DBG_PC                              0x1C4
#define TX_RTY_CNT                          0x10
#define RX_RTY_CNT                          0x4000  
#define RTY_CNT                             0x4000 


//Tx page 
#define TXPKTBUF_PGBNDY_FWLBK               0x60//Reserved 4k bytes, 2k for fwlbk packet, 2k for wmac buffer
#define TXPKTBUF_BNDY_FWLBK                 (PAGESIZE_TX*TXPKTBUF_PGBNDY_FWLBK)
//#define   RXPKTBUF_PGBNDY_FWLBK               0x27FF
#define RXPKTBUF_PGBNDY_FWLBK               0x6FF
//#define   RXPKTBUF_PGBNDY_FWLBK               0x1BFF



//FWLBK MODE
#define FWLBK_MOD_IDLE                      0x00
#define FWLBK_MOD_TX_Without_CHECK          0x20
#define FWLBK_MOD_TX_With_CHECK             0x40
#define FWLBK_MOD_MACLBK_With_CHECK         0x60
#define FWLBK_MOD_CP_MAC_TEST               0x80
#define FWLBK_MOD_CP_BBLBK                  0xA0
#define FWLBK_MOD_CP_AFELBK                 0xC0
#define FWLBK_MOD_MODULE_TEST               0xE0

//FWLBK MASK
#define FWLBK_MASK_LEN                      (BIT2 | BIT3)
#define FWLBK_MASK_CONT_TX                  BIT1
#define FWLBK_MASK_BW_40M                   BIT0

#define FWLBK_MASK_MODE                     (BIT5 | BIT6 | BIT7)
#define FWLBK_MASK_SGI                      BIT4
#define FWLBK_MASK_SCANRATE                 BIT3
#define FWLBK_MASK_DATARATE                 (BIT0 | BIT1 | BIT2)

//FWLBK Extend MASK
#define FWLBK_MASK_UC                       BIT3
#define FWLBK_MASK_LEGACY_ONLY              BIT2
#define FWLBK_MASK_BW_EXT                   BIT1
#define FWLBK_MASK_VHT_EN                   BIT0

//FWLBK Debug

#define FWLBK_DBG_STOP_CNT                  0x01A0

#define FWLBK_DBG_PAYLD_OFSET               0x01D0//The n'th byte of payload which compare error
#define FWLBK_DBG_TXBUF_DATA                0x01D2//The value of TXPKTBUF compared error
#define FWLBK_DBG_RXBUF_DATA                0x01D3//The value of RXPKTBUF compared error
#define FWLBK_DBG_RX_LEN                    0x01D4//length field of RxDESC
#define FWLBK_DBG_RXFF_RP                   0x01D6//RXFF Read Pointer
#define FWLBK_DBG_PKTCNT                    0x01D8//Tx packet count or Rx Receive packet count
#define FWLBK_DBG_DATARATE                  0x01DA
#define FWLBK_DBG_RX_OK_CNT                 0x01DB

//
//0x067C[31] : DBG_SEL.  1:TX, 0:RX 
//0x067C[15] : KEY_FOUND
//0x067C[14:12] : Secrity Type
//0x067C[9:5] : MIC Key Index
//0x067C[4:0] : Security Key Index 
//
#define FWLBK_DBG_CAM_0                     0x01DC//0x67C
#define FWLBK_DBG_CAM_1                     0x01DD//0x67D
#define FWLBK_DBG_POLL_OK_CNT               0x01DE
#define FWLBK_DBG_CTL                       0x01DF


//FWLBK Status (0x01C3)
#define FWLBK_TEST_FINISH                   BIT7
#define FWLBK_TEST_FAIL                     BIT6
#define FWLBK_TEST_ERR_SETCAM               BIT5
#define FWLBK_TEST_ERR_RX_RWPTR             BIT4
#define FWLBK_TEST_ERR_RXFF_NOEMPTY         BIT3
#define FWLBK_TEST_ERR_CPUMGQ               BIT2
#define FWLBK_ERR_RXDMABUSY                 BIT1
#define FWLBK_TEST_ERR_CMD                  BIT0


//FWLBK DBG CTRL (0x01DF)
#define DBGCTRL_STEP                        BIT0
#define DBGCTRL_STEP_NEXT                   BIT1
#define DBGCTRL_ERR_CTX                     BIT2    //if Rx compare error, continue test.


//EXREG Status Report

//0xFD01
#define EXREG_STATUS_HCI_TYPE               (BIT1|BIT2)
#define EXREG_STATUS_HCI_SDIO               0
#define EXREG_STATUS_HCI_USB                BIT1
#define EXREG_STATUS_HCI_PCIE               (BIT1|BIT2)


//0x4
#define EXREG_STATUS_WLOFF_EN               BIT1

//FWLBK Start Length
#define FWLBK_STARTLEN_64B                  0x58
#define FWLBK_STARTLEN_1024B                0x418
#define FWLBK_STARTLEN_1514B                0x602
#define FWLBK_STARTLEN_INCREMENTAL          0x58


#define SIZE_WLAN_HEAD                      24

//Security
#define SEC_TYPE_WEP40                      1
#define SEC_TYPE_WEP104                     5
#define SEC_TYPE_TKIP                       2
#define SEC_TYPE_AES                        4
#define SEC_TYPE_WAPI                       6

#define SEC_IDX_WEP40                       0
#define SEC_IDX_WEP104                      1
#define SEC_IDX_TKIP                        2
#define SEC_IDX_AES                         3
#define SEC_IDX_WAPI                        4

#define LAST_PAGE_OF_TXBUF_RSVDPAGE 0xFF   
#define LAST_PAGE_OF_TXBUF_ACQ      0xFF
#define LLT_WRITE_COUNT_MAX         0xFFFF


/*------------------------------Define Enum-----------------------------------*/


/*--------------------------Define MACRO--------------------------------------*/

/*------------------------------Define Struct---------------------------------*/


/*------------------------Export global variable------------------------------*/
#ifndef __ASSEMBLY__

extern u8  FWLBK_HEADER[];
extern u8  FWLBK_TXDESC[];
extern u8  MIC[];
extern u16 FWLBK_LEN[];
extern u8  FWLBK_DATARATE[];

extern volatile u8 FWLBK_CTL_L, FWLBK_CTL_H;
extern volatile u8 TxDESC_len;

/*------------------------------Function declaration--------------------------*/
#if 0
extern void 
TxpktbufWriteByte(
    IN u16 address, 
    IN u8  value
) ;

extern u8 
TxpktbufReadByte(
    IN u16 address
);

extern u8 
RxpktbufReadByte(
    IN u16 address
) ;

extern void
DbgStepByStep(
    IN u16     func_addr,
    IN u8      pc_offset,
    IN u16     curr_cnt
);
#endif

extern void
FwlbkDbgPrtU16_8721D_ROM(
    IN u16     addr,
    IN u16     wdata
);

extern void 
FwlbkLLTWrite_8721D_ROM(
    IN u8 address, 
    IN u8 value
);

extern void
FwlbkHWInitialLLT_8721D_ROM(
    void
);


extern void 
FwlbkMode_8721D_ROM(
    void
) ;

extern void 
FwlbkPreparePkt_8721D_ROM(
    void
) ;

extern void 
FwlbkModeSel_8721D_ROM(
    void
) ;

extern void 
FwlbkCheckRxPayload_8721D_ROM(
    void
) ;

extern void 
FwlbkSetCAM_8721D_ROM(
    IN u8   sec_type
) ;

extern void 
FwlbkUpdateParam_8721D_ROM(
    IN u8 Datarate
) ;

extern u8   
FwlbkMcuPollMgq_8721D_ROM(
    void
) ;

extern void 
FwlbkModeTxNoCheck_8721D_ROM(
    void
) ;

extern void 
FwlbkModeTx_8721D_ROM(
    void
) ;

extern void 
FwlbkModeMaclbk_8721D_ROM(
    void
) ;

extern void 
FwlbkModeSecTest_8721D_ROM(
    void
) ;
#if 0
extern void 
Mode_CP_BBLBK(
    void
) ;
#endif
extern void 
FwlbkModeAFElbk_8721D_ROM(
    void
) ;

#if 0
extern void 
Mode_MODULE_TEST(
    void
) ;
#endif

#endif  //#ifndef   __ASSEMBLY__


#endif  //__RTL8721D_FWLBK_H__
