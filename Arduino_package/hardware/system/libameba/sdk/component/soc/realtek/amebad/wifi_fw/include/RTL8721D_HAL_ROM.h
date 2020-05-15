#ifndef __RTL8721D_HAL_H__
#define __RTL8721D_HAL_H__

#include "RTL8721D_Reg_ROM.h"
#include "RTL8721D_debug_ROM.h"
#include "RTL8721D_Init_ROM.h"
#include "RTL8721D_INT_ROM.h"
#include "RTL8721D_Util_ROM.h"
//#include "RTL8721D_Coex.h"
#include "RTL8721D_cmd_ROM.h"
#include "RTL8721D_PhyReg_ROM.h"
#include "RTL8721D_PhyCfg_ROM.h"
//#include "RTL8721D_Efuse.h"
#include "RTL8721D_Packet_ROM.h"
#include "RTL8721D_PowerCtrl_ROM.h"
//#include "RTL8721D_Initial_Offload.h"
#include "RTL8721D_IOPath_ROM.h"
#include "RTL8721D_FWLBK_ROM.h"
#include "RTL8721D_TxReport_ROM.h"
//#include "RTL8721D_DLFW.h"
#include "RTL8721D_RateAdaptive_ROM.h"
#include "RTL8721D_BTConCurr_ROM.h"
//#include "RTL8721D_FastChannelSwitch.h"
#include "RTL8721D_DMA_ROM.h"

//1 Code Memory Map
//
// 0x0000 ~ 0x3FFF: IROM
// 0x4000 ~ 0xBFFF: code dwonload ram 32K
// 0xC000 ~ 0xFFFF: TX pkt buff high 16K


//1 Xdata Memory Map
//
// 0x0000 ~ 0x7FFF: MAC Register
// 0x0800 ~ 0x0FFF: BB Register
// 0x1000 ~ 0x1FFF: download ram space
// 0x2000 ~ 0x2FFF: RSVD
// 0x3000 ~ 0x3FFF: ext ptcl reg
// 0x4000 ~ 0x7FFF: RSVD
// 0x8000 ~ 0x9FFF: XData & TXRPT
// 0xEE00 ~ 0xEE47: USB descriptor RAM
// 0xEE48 ~ 0xFAFF: RSVD
// 0xF000 ~ 0xF0FF: usb3 register
// 0xFB00 ~ 0xFBFF: RxPktBuf
// 0xFC00 ~ 0xFCFF: TxPktBuf
// 0xFD00 ~ 0xFDFF: 8051 Internal
// 0xFE00 ~ 0xFEFF: USB Register
// 0xFF00 ~ 0xFFFF: RSVD


/*--------------------------Define -------------------------------------------*/
//3 TaskBitMap0
#define RATEADAPTIVE        BIT0
#define H2CEVENT            BIT1
#define C2HEVENT            BIT2
#define RLPSEVENT           BIT3
#define REMOTEWAKEEVENT     BIT4
#define APOFFLOADEVENT      BIT5
#define MAILBOXEVENT        BIT6
#define SWTIMEREVENT        BIT7

//3 TaskBitMap1
#define BBNHMEVENT          BIT0
#define DBGPKTEVENT         BIT1
#define SIDEBANDWoWLAN      BIT2


//3 MAC base address
#define RTL8721D_IOREG_AHB_BASE             0x40080000
//3 BB base address
#define RTL8721D_BBREG_AHB_BASE             0x40082800 //0x40083000-0x800
//3 RF base address
#define RTL8721D_RFREG_AHB_BASE             0x40083800 
//3 DDMA control base address
#define RTL8721D_DDMA_CTRL_AHB_BASE         0x40081100 //1100~115B  11E0~11FF
//3 BT mailbox base address
#define RTL8721D_BT_MAILBOX_AHB_BASE        0x40081160 //1160~1177

//#define RTL8721D_UART0_CTRL_AHB_BASE         0x18002000
//#define RTL8721D_CPU_CTRL_AHB_BASE             RTL8814A_IOREG_OCP_BASE
//#define RTL8721D_CPU_LOCAL_AHB_BASE           0x18003400
//#define RTL8721D_FLASH_CTRL_AHB_BASE          0X18009000

//#define RTL8721D_LLT_AHB_BASE                      0x18650000
//3 TxReport Buffer base address
#define RTL8721D_RPTBUF_AHB_BASE            0x400B0000
#define RTL8721D_SPERPT_BASE                (RTL8721D_RPTBUF_AHB_BASE)
#define RTL8721D_BCNRPT_BASE                (RTL8721D_RPTBUF_AHB_BASE+0x80)
#define RTL8721D_TXRPT_BASE                 (RTL8721D_RPTBUF_AHB_BASE+0x100)
#define RTL8721D_RAMASK_BASE                (RTL8721D_RPTBUF_AHB_BASE+0x200)
#define RTL8721D_NANRPT_BASE                (RTL8721D_RPTBUF_AHB_BASE+0x280)

//3 Rx packet buffer base address
#define RTL8721D_RXFF_AHB_BASE              0x400A0000

//3 Tx packet buffer base address
#define RTL8721D_TXFF_AHB_BASE              0x40090000

//3 CAM base address
#define RTL8721D_SECCAM_AHB_BASE            0x40081A00
#define RTL8721D_WOWCAM_AHB_BASE            0x18674000
//#define RTL8721D_RXFILTERCAM_AHB_BASE       0x18676000
#define RTL8721D_BACAM_AHB_BASE             0x40081800
//#define RTL8721D_ADDRCAM_AHB_BASE           0x18678000

//3 HPON base address
#define RTL8721D_HPON_AHB_BASE              0x40080000

/*CPU Sram*/
//#define RTL8721D_FLASH_MEM_AHB_BASE         0x10000000
//#define RTL8721D_IMEM_AHB_BASE              0x00000000
//#define RTL8721D_DMEM_AHB_BASE              0x00200000
//#define RTL8721D_ERAM_AHB_BASE              0x00100000

//offset:
#define rtl_inb(offset)             (*(volatile u8 *)((u32)offset))
#define rtl_inw(offset)             (*(volatile u16 *)((u32)offset))
#define rtl_inl(offset)             (*(volatile u32 *)((u32)offset))

#define rtl_outb(offset,val)        do { (*(volatile u8 *)((u32)offset) = (val));    } while(0)
#define rtl_outw(offset,val)        do { (*(volatile u16 *)((u32)offset) = (val));    } while(0)
#define rtl_outl(offset,val)        do { (*(volatile u32 *)((u32)offset) = (val));    } while(0)


//3 MACREG

//offset :
#define WritePONRegDWord(offset, value)     rtl_outl((offset+RTL8721D_HPON_AHB_BASE), value)
#define WritePONRegWord(offset, value)      rtl_outw((offset+RTL8721D_HPON_AHB_BASE), value)
#define WritePONRegByte(offset, value)      rtl_outb((offset+RTL8721D_HPON_AHB_BASE), value)

#define ReadPONRegDWord(offset)             (rtl_inl((offset+RTL8721D_HPON_AHB_BASE)))
#define ReadPONRegWord(offset)              (rtl_inw((offset+RTL8721D_HPON_AHB_BASE)))
#define ReadPONRegByte(offset)              (rtl_inb((offset+RTL8721D_HPON_AHB_BASE)))


//offset :
#define WriteMACRegDWord(offset, value)     rtl_outl((offset+RTL8721D_IOREG_AHB_BASE), value)
#define WriteMACRegWord(offset, value)      rtl_outw((offset+RTL8721D_IOREG_AHB_BASE), value)
#define WriteMACRegByte(offset, value)      rtl_outb((offset+RTL8721D_IOREG_AHB_BASE), value)

#define ReadMACRegDWord(offset)             (rtl_inl((offset+RTL8721D_IOREG_AHB_BASE)))
#define ReadMACRegWord(offset)              (rtl_inw((offset+RTL8721D_IOREG_AHB_BASE)))
#define ReadMACRegByte(offset)              (rtl_inb((offset+RTL8721D_IOREG_AHB_BASE)))

//3 BB Reg
#define WritePortBBUlong(offset, value)     rtl_outl((offset+RTL8721D_BBREG_AHB_BASE), value)
#define WritePortBBUshort(offset, value)    rtl_outw((offset+RTL8721D_BBREG_AHB_BASE), value)
#define WritePortBBUchar(offset, value)     rtl_outb((offset+RTL8721D_BBREG_AHB_BASE), value)

#define ReadPortBBUlong(offset)             (rtl_inl((offset+RTL8721D_BBREG_AHB_BASE)))
#define ReadPortBBUshort(offset)            (rtl_inw((offset+RTL8721D_BBREG_AHB_BASE)))
#define ReadPortBBUchar(offset)             (rtl_inb((offset+RTL8721D_BBREG_AHB_BASE)))

//3 RF Reg only support dword mode
#define WritePortRFUlong(offset, value)     rtl_outl((4*offset+RTL8721D_RFREG_AHB_BASE), value)
#define ReadPortRFUlong(offset)             (rtl_inl((4*offset+RTL8721D_RFREG_AHB_BASE)))

//3 SEC CAM
#define WriteSECCAMDWord(offset, value)  rtl_outl((offset+RTL8721D_SECCAM_AHB_BASE), value)
#define WriteSECCAMWord(offset, value)   rtl_outw((offset+RTL8721D_SECCAM_AHB_BASE), value)
#define WriteSECCAMByte(offset, value)   rtl_outb((offset+RTL8721D_SECCAM_AHB_BASE), value)

#define ReadSECCAMDWord(offset)          (rtl_inl((offset+RTL8721D_SECCAM_AHB_BASE)))
#define ReadSECCAMWord(offset)           (rtl_inw((offset+RTL8721D_SECCAM_AHB_BASE)))
#define ReadSECCAMByte(offset)           (rtl_inb((offset+RTL8721D_SECCAM_AHB_BASE)))

//3 WOW CAM
#define WriteWOWCAMDWord(offset, value)  rtl_outl((offset+RTL8721D_WOWCAM_AHB_BASE), value)
#define WriteWOWCAMWord(offset, value)   rtl_outw((offset+RTL8721D_WOWCAM_AHB_BASE), value)
#define WriteWOWCAMByte(offset, value)   rtl_outb((offset+RTL8721D_WOWCAM_AHB_BASE), value)

#define ReadWOWCAMDWord(offset)          (rtl_inl((offset+RTL8721D_WOWCAM_AHB_BASE)))
#define ReadWOWCAMWord(offset)           (rtl_inw((offset+RTL8721D_WOWCAM_AHB_BASE)))
#define ReadWOWCAMByte(offset)           (rtl_inb((offset+RTL8721D_WOWCAM_AHB_BASE)))

#if 0
//3 RX FILTER CAM
#define WriteRXFILTERCAMDWord(offset, value)  rtl_outl((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE), value)
#define WriteRXFILTERCAMWord(offset, value)   rtl_outw((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE), value)
#define WriteRXFILTERCAMByte(offset, value)   rtl_outb((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE), value)

#define ReadRXFILTERCAMDWord(offset)          (rtl_inl((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE)))
#define ReadRXFILTERCAMWord(offset)           (rtl_inw((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE)))
#define ReadRXFILTERCAMByte(offset)           (rtl_inb((offset+RTL8814B_RXFILTERCAM_VIRTUAL_BASE)))
#endif

//3 BA CAM
#define WriteBACAMDWord(offset, value)  rtl_outl((offset+RTL8721D_BACAM_AHB_BASE), value)
#define WriteBACAMWord(offset, value)   rtl_outw((offset+RTL8721D_BACAM_AHB_BASE), value)
#define WriteBACAMByte(offset, value)   rtl_outb((offset+RTL8721D_BACAM_AHB_BASE), value)

#define ReadBACAMDWord(offset)          (rtl_inl((offset+RTL8721D_BACAM_AHB_BASE)))
#define ReadBACAMWord(offset)           (rtl_inw((offset+RTL8721D_BACAM_AHB_BASE)))
#define ReadBACAMByte(offset)           (rtl_inb((offset+RTL8721D_BACAM_AHB_BASE)))

#if 0
//3 ADDR CAM
#define WriteADDRCAMDWord(offset, value)  rtl_outl((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE), value)
#define WriteADDRCAMWord(offset, value)   rtl_outw((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE), value)
#define WriteADDRCAMByte(offset, value)   rtl_outb((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE), value)

#define ReadADDRCAMDWord(offset)          (rtl_inl((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE)))
#define ReadADDRCAMWord(offset)           (rtl_inw((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE)))
#define ReadADDRCAMByte(offset)           (rtl_inb((offset+RTL8814B_ADDRCAM_VIRTUAL_BASE)))
#endif

//3 ReportBUFF
#define WriteRPTBUFDWord(offset, value)  rtl_outl((offset+RTL8721D_RPTBUF_AHB_BASE), value)
#define WriteRPTBUFWord(offset, value)   rtl_outw((offset+RTL8721D_RPTBUF_AHB_BASE), value)
#define WriteRPTBUFByte(offset, value)   rtl_outb((offset+RTL8721D_RPTBUF_AHB_BASE), value)

#define ReadRPTBUFDWord(offset)          (rtl_inl((offset+RTL8721D_RPTBUF_AHB_BASE)))
#define ReadRPTBUFWord(offset)           (rtl_inw((offset+RTL8721D_RPTBUF_AHB_BASE)))
#define ReadRPTBUFByte(offset)           (rtl_inb((offset+RTL8721D_RPTBUF_AHB_BASE)))

#if 0
//3 RXFF_FW
#define WriteRXFF2DWord(offset, value)  rtl_outl((offset+RTL8814A_FWFF_VIRTUAL_BASE), value)
#define WriteRXFF2Word(offset, value)   rtl_outw((offset+RTL8814A_FWFF_VIRTUAL_BASE), value)
#define WriteRXFF2Byte(offset, value)   rtl_outb((offset+RTL8814A_FWFF_VIRTUAL_BASE), value)

#define ReadRXFF2DWord(offset)          (rtl_inl((offset+RTL8814A_FWFF_VIRTUAL_BASE)))
#define ReadRXFF2Word(offset)           (rtl_inw((offset+RTL8814A_FWFF_VIRTUAL_BASE)))
#define ReadRXFF2Byte(offset)           (rtl_inb((offset+RTL8814A_FWFF_VIRTUAL_BASE)))
#endif

//3 RXFF
#define WriteRXFFDWord(offset, value)   rtl_outl((offset+RTL8721D_RXFF_AHB_BASE), value)
#define WriteRXFFWord(offset, value)    rtl_outw((offset+RTL8721D_RXFF_AHB_BASE), value)
#define WriteRXFFByte(offset, value)    rtl_outb((offset+RTL8721D_RXFF_AHB_BASE), value)

#define ReadRXFFDWord(offset)           (rtl_inl((offset+RTL8721D_RXFF_AHB_BASE)))
#define ReadRXFFWord(offset)            (rtl_inw((offset+RTL8721D_RXFF_AHB_BASE)))
#define ReadRXFFByte(offset)            (rtl_inb((offset+RTL8721D_RXFF_AHB_BASE)))

#if 0
//3 RXDFF
#define WriteRXDFFDWord(offset, value)   rtl_outl((offset+RTL8814B_RXDFF_VIRTUAL_BASE), value)
#define WriteRXDFFWord(offset, value)    rtl_outw((offset+RTL8814B_RXDFF_VIRTUAL_BASE), value)
#define WriteRXDFFByte(offset, value)    rtl_outb((offset+RTL8814B_RXDFF_VIRTUAL_BASE), value)

#define ReadRXDFFDWord(offset)           (rtl_inl((offset+RTL8814B_RXDFF_VIRTUAL_BASE)))
#define ReadRXDFFWord(offset)            (rtl_inw((offset+RTL8814B_RXDFF_VIRTUAL_BASE)))
#define ReadRXDFFByte(offset)            (rtl_inb((offset+RTL8814B_RXDFF_VIRTUAL_BASE)))
#endif 

//3 TXFF
#define WriteTXFFDWord(offset, value)     rtl_outl((offset+RTL8721D_TXFF_AHB_BASE), value)
#define WriteTXFFWord(offset, value)      rtl_outw((offset+RTL8721D_TXFF_AHB_BASE), value)
#define WriteTXFFByte(offset, value)      rtl_outb((offset+RTL8721D_TXFF_AHB_BASE), value)

#define ReadTXFFDWord(offset)             (rtl_inl((offset+RTL8721D_TXFF_AHB_BASE)))
#define ReadTXFFWord(offset)              (rtl_inw((offset+RTL8721D_TXFF_AHB_BASE)))
#define ReadTXFFByte(offset)              (rtl_inb((offset+RTL8721D_TXFF_AHB_BASE)))

//3 Retention RAM
#define WriteRRamDWord(offset, value)     rtl_outl((offset+RETENTION_RAM_BASE), value)
#define WriteRRamWord(offset, value)      rtl_outw((offset+RETENTION_RAM_BASE), value)
#define WriteRRamByte(offset, value)      rtl_outb((offset+RETENTION_RAM_BASE), value)

#define ReadRRamDWord(offset)             (rtl_inl((offset+RETENTION_RAM_BASE)))
#define ReadRRamWord(offset)              (rtl_inw((offset+RETENTION_RAM_BASE)))
#define ReadRRamByte(offset)              (rtl_inb((offset+RETENTION_RAM_BASE)))


//3 TX report buffer access
#define WriteTxrptByte(macid,offset,val)        rtl_outb(RTL8721D_TXRPT_BASE + (macid<<4) + offset, val) //macid range: 0~16; offset range: 0~15
#define WriteTxrptDWord(macid,offset,val)       rtl_outl(RTL8721D_TXRPT_BASE + (macid<<4) + offset, val)
#define ReadTxrptByte(macid,offset)             rtl_inb(RTL8721D_TXRPT_BASE + (macid<<4) + offset)
#define ReadTxrptDWord(macid,offset)            rtl_inl(RTL8721D_TXRPT_BASE + (macid<<4) + offset)

#define WriteBcnrptByte(macid,offset,val)       rtl_outb(RTL8721D_BCNRPT_BASE + (macid<<4) + offset, val) //macid range: 0~16; offset range: 0~15
#define WriteBcnrptDWord(macid,offset,val)      rtl_outl(RTL8721D_BCNRPT_BASE + (macid<<4) + offset, val)
#define ReadBcnrptByte(macid,offset)            rtl_inb(RTL8721D_BCNRPT_BASE + (macid<<4) + offset)
#define ReadBcnrptDWord(macid,offset)           rtl_inl(RTL8721D_BCNRPT_BASE + (macid<<4) + offset)

#define WriteSperptByte(macid,offset,val)       rtl_outb(RTL8721D_SPERPT_BASE + (macid<<3) + offset, val) //macid range: 0~16; offset range: 0~7
#define WriteSperptDWord(macid,offset,val)      rtl_outl(RTL8721D_SPERPT_BASE + (macid<<3) + offset, val)
#define ReadSperptByte(macid,offset)            rtl_inb(RTL8721D_SPERPT_BASE + (macid<<3) + offset)
#define ReadSperptDWord(macid,offset)           rtl_inl(RTL8721D_SPERPT_BASE + (macid<<3) + offset)

#define ReadRAMASKByte(macid, offset)           rtl_inb(RTL8721D_RAMASK_BASE + (macid<<3) + offset)     //macid range: 0~16; offset range: 0~7
#define ReadRAMASKDWORD(macid, offset)          rtl_inl(RTL8721D_RAMASK_BASE + (macid<<3) + offset)
#define WriteRAMASKByte(macid, offset, val)     rtl_outb(RTL8721D_RAMASK_BASE + (macid<<3) + offset, val)
#define WriteRAMASKDWORD(macid, offset, val)    rtl_outl(RTL8721D_RAMASK_BASE + (macid<<3) + offset, val)

#define ReadNANrptByte(offset)                  rtl_inb(RTL8721D_NANRPT_BASE  + offset)     
#define ReadNANrptDWORD(offset)                 rtl_inl(RTL8721D_NANRPT_BASE  + offset)
#define WriteNANrptByte(offset, val)            rtl_outb(RTL8721D_NANRPT_BASE + offset, val)
#define WriteNANrptDWORD(offset, val)           rtl_outl(RTL8721D_NANRPT_BASE + offset, val)


//3 BT mailbox
#define WriteBTMailboxDWord(offset, value)      rtl_outl((offset+RTL8721D_BT_MAILBOX_AHB_BASE), value)
#define WriteBTMailboxWord(offset, value)       rtl_outw((offset+RTL8721D_BT_MAILBOX_AHB_BASE), value)
#define WriteBTmailboxByte(offset, value)       rtl_outb((offset+RTL8721D_BT_MAILBOX_AHB_BASE), value)

#define ReadBTMailboxDWord(offset)             (rtl_inl((offset+RTL8721D_BT_MAILBOX_AHB_BASE)))
#define ReadBTMailboxWord(offset)              (rtl_inw((offset+RTL8721D_BT_MAILBOX_AHB_BASE)))
#define ReadBTMailboxByte(offset)              (rtl_inb((offset+RTL8721D_BT_MAILBOX_AHB_BASE)))

//3 DDMA
#define WriteDDMADWord(offset, value)           rtl_outl((offset+RTL8721D_DDMA_CTRL_AHB_BASE), value)
#define WriteDDMAWord(offset, value)            rtl_outw((offset+RTL8721D_DDMA_CTRL_AHB_BASE), value)
#define WriteDDMAByte(offset, value)            rtl_outb((offset+RTL8721D_DDMA_CTRL_AHB_BASE), value)

#define ReadDDMADWord(offset)                  (rtl_inl((offset+RTL8721D_DDMA_CTRL_AHB_BASE)))
#define ReadDDMAWord(offset)                   (rtl_inw((offset+RTL8721D_DDMA_CTRL_AHB_BASE)))
#define ReadDDMAByte(offset)                   (rtl_inb((offset+RTL8721D_DDMA_CTRL_AHB_BASE)))


//Interrupt List
#ifdef _MCU_8051_
#define INT_USB_8721D         0
#define INT_WLAN_8721D        2
#define INT_UART_8721D        4
#define INT_SYS_8721D         8
#define INT_FT_8721D          9
#define INT_MAILBOX_8721D    10
#else
#define INT_USB_8721D         
#define INT_WLAN_8721D        
#define INT_UART_8721D        
#define INT_SYS_8721D         
#define INT_FT_8721D          
#define INT_MAILBOX_8721D    
#endif

//function pointer, added by jackie;
#define OSSendSignalCommon      OSSendSignalCommon_Ptr
#define ISRSendSignalCommon     ISRSendSignalCommon_Ptr
#define Change_PS_State             Change_PS_State_Ptr
#define IssueC2HPKT                     IssueC2HPKT_Ptr
#define FillC2HContant                  FillC2HContant_Ptr
#define C2HPKTRxDESC                C2HPKTRxDESC_Ptr
#define BTC_Pnty_ShiftRAM8721D               BTC_Pnty_Shift_Ptr
#define IssueCTS2Self                   IssueCTS2Self_Ptr
#define ChkandChangePS              ChkandChangePS_Ptr
#define IssuePSPoll                         IssuePSPoll_Ptr
#define ReadRxBuff                      ReadRxBuff_Ptr
#define IssueQNull                          IssueQNull_Ptr
#define PowerBitSetting                 PowerBitSetting_Ptr
#define ChangeTransmiteRate         ChangeTransmiteRate_Ptr
#define IssueNullData                       IssueNullData_Ptr

//#define PktBufAccessCtrl            PktBufAccessCtrl_CallPtr
#define PktBufAccessCtrl            PktBufAccessCtrl_Ptr
#define SetTxbufAddrFromPageNum     SetTxbufAddrFromPageNum_8721D_ROM


//Interrupt List
#define INT_USB             INT_USB_8721D
#define INT_WLAN            INT_WLAN_8721D
#define INT_UART            INT_UART_8721D
#define INT_SYS             INT_SYS_8721D
#define INT_FT              INT_FT_8721D
#define INT_MAILBOX         INT_MAILBOX_8721D

// HAL Structure
//#define HAL_DATA_TYPE         HAL_DATA_COMMON
//#define HAL_DATA_TYPE       HAL_DATA_8721D

// Interrupt handle function
//#define USB_INT_HDL         USB_INT_HDL_8721D/
//#define WLAN_INT_HDL        WLAN_INT_HDL_8721D
//#define UART_INT_HDL        UART_INT_HDL_8721D
//#define FT_INT_HDL          FT_INT_HDL_8721D
//#define SYS_INT_HDL         SYS_INT_HDL_8721D
//#define MAILBOX_INT_HDL_RAM      MAILBOX_INT_HDL_RAM_8721D

// Initial Offload Decision
//#define CheckInitialOffload     CheckInitialOffload_8721D_ROM
//#define INIT_OFLD_TYPE          INIT_OFLD_8721D

// ROM/RAM Decision
//#define CheckJumpToRAM          CheckJumpToRAM_8721D_ROM

//Download FW through TxPKTBuf
//#define CheckROMDLFWEnable      CheckROMDLFWEnable_8721D_ROM

//FWLBK
#define CheckFWLBKEnable        CheckFWLBKEnable_8721D_ROM


#if 0
// Reg R/W
//byte
#define WriteMACRegByte(addr, value)         WriteXBYTE((u32)addr|WIFI_BASEADDR, value)
#define ReadMACRegByte(addr)                 ReadXBYTE((u32)addr|WIFI_BASEADDR)
#define WritePONRegByte(addr, value)         WriteXBYTE((u32)addr|WIFI_BASEADDR, value)
#define ReadPONRegByte(addr)                 ReadXBYTE((u32)addr|WIFI_BASEADDR)
#define RWritePONRegByte(addr, value)        WriteXBYTE((u32)addr|WIFI_BASEADDR, value)
#define RReadPONRegByte(addr)                ReadXBYTE((u32)addr|WIFI_BASEADDR)
#define WriteCPURegByte                      WriteXBYTE
#define ReadCPURegByte                       ReadXBYTE
#define WriteUSBRegByte                      WriteXBYTE
#define ReadUSBRegByte                       ReadXBYTE

//Word (2 Bytes)
#define ReadMACRegWord(addr)                 ReadXWORD((u32)addr|WIFI_BASEADDR)
#define WriteMACRegWord(addr, value)         WriteXWORD((u32)addr|WIFI_BASEADDR, value)

//DWord (4 Bytes)
#define ReadMACRegDWord(addr)                ReadXDWORD((u32)addr|WIFI_BASEADDR)
#define WriteMACRegDWord(addr, value)        WriteXDWORD((u32)addr|WIFI_BASEADDR, value)
#endif

// BB/RF R/W

#define PHY_InitBBRFRegDef              PHY_InitBBRFRegDef_8721D

#define CheckBBRWIsStable               CheckBBRWIsStable_8721D
#define PHY_QueryBBReg                  PHY_QueryBBReg_8721D
#define PHY_SetBBReg                    PHY_SetBBReg_8721D


#define PHY_RFSerialRead                PHY_RFSerialRead_8721D
#define PHY_RFSerialWrite               PHY_RFSerialWrite_8721D

#define PHY_QueryRFReg                  PHY_QueryRFReg_8721D
#define PHY_SetRFReg                    PHY_SetRFReg_8721D_RAM

// Efuse
//#define EfuseOneByteRead                EfuseOneByteRead_8721D_ROM

// Delay Function
//#define DelayUS                         DelayUS_8721D_ROM
//#define DelayMS                         DelayMS_8721D_ROM

//2  GTimer
//#define InitGTimer32us                  InitGTimer32us_CallPtr
//#define InitGTimer1ms                   InitGTimer1ms_CallPtr
#define InitGTimer32us              InitGTimer32us_Ptr
#define InitGTimer1ms               InitGTimer1ms_Ptr

//2 Basic IO
#define PollingReg                  PollingReg_8721D_Ptr

//#define ClrAllFWUsedIMR         ClrAllFWUsedIMR_8721D

//#define SetMediaStatus          SetMediaStatus_8721D
#define GetMediaStatus          GetMediaStatus_8721D_ROM
#define CheckMaxMacidNum        CheckMaxMacidNum_8721D_ROM

//2 4.) FS ISR

// Interrupt IMR / ISR
#define setwlanimr              setwlanimr_8721D_ROM
#define clrwlanimr              clrwlanimr_8721D_ROM
//#define setwlanimr2              setwlanimr2_8721D   
//#define clrwlanimr2              clrwlanimr2_8721D
//#define setsysimr               setsysimrCommon
//#define clrsysimr               clrsysimrCommon
#define setftimr                setftimr_8721D_ROM
#define clrftimr                clrftimr_8721D_ROM

//2 H2C
//#define CheckH2CMailbox         CheckH2CMailboxCommon
#define H2CCmdFunc              H2CCmdFunc_Ptr
//#define MediaConnection         MediaConnectionCommon
//#define MediaDisconnection      MediaDisconnectionCommon
#define H2CInQueue              H2CInQueue_Ptr
//#define H2CDeQueue              H2CDeQueue_8721D

//2 C2H
//#define InitC2H                 InitC2H_8721D_ROM
#define C2HInQueue              C2HInQueue_Ptr
//#define C2HDeQueue              C2HDeQueue_8721D
#define WaitC2HOk               WaitC2HOk_8721D_ROM

// RF ON/OFF
#define PsOpenRF                         PsOpenRF_Ptr
#define PsCloseRF                        PsCloseRF_Ptr

//2 Packet
//#define CheckCPUMGQEmpty                CheckCPUMGQEmpty_8721D
//#define IssueRsvdPagePacketSetting    IssueRsvdPagePacketSetting_CallPtr
#define IssueRsvdPagePacketSetting    IssueRsvdPagePacketSetting_Ptr
#define CheckTxPktBufOverFlow           CheckTxPktBufOverFlow_8721D_ROM
//#define CheckRxPktBufOverFlow           CheckRxPktBufOverFlow_8721D

//2 PowerCtrl
#define WaitTxStateMachineOk            WaitTxStateMachineOk_Ptr
//#define GetRPWMValue                       GetRPWMValueCommon
//#define GetRPWM2Value                     GetRPWMValue2Common
//#define CheckTSFIsStable                   CheckTSFIsStableCommon
//#define WaitHWStateReady                 WaitHWStateReadyCommon
#define WriteTxPause				     WriteTxPause_Ptr
#define WriteTxPauseWithMask            WriteTxPauseWithMask_Ptr

//3 5.) BTConcurr
#define Write778        Write778_Ptr
#define DacSwing	    DacSwing_Ptr
#define InitBTMailbox   InitBTMailbox_Ptr
#define BTMailboxRetry	BTMailboxRetry_Ptr
#define C2HBTInfo		C2HBTInfo_Ptr
#define C2HBTLoopback	C2HBTLoopback_Ptr
#define C2HBTMpRpt		C2HBTMpRpt_Ptr
#define IssueBTQNull	IssueBTQNull_Ptr
#define WlanActCtrl		WlanActCtrl_Ptr

#define ChangeCoexTable	ChangeCoexTable_Ptr

//#define InitBTypeTDMA	InitBTypeTDMA_Ptr

#define InitBTypeTDMA2	InitBTypeTDMA2_Ptr

#define BTypeTDMABCNEarly	BTypeTDMABCNEarly_Ptr

#define BTypeTDMATBTThdl	BTypeTDMATBTThdl_Ptr

#define BTypeTDMATimeOuthdl		BTypeTDMATimeOuthdl_Ptr
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
#define AntennaSwitchCtrl			AntennaSwitchCtrl_Ptr
#endif

#define MailboxINTHDL		        MailboxINTHDL_Ptr
#define MailboxINTHDL_Patch		MailboxINTHDL_Patch_Ptr

#define BTRoleChangeTBTTHDL		BTRoleChangeTBTTHDL_Ptr

#define BTRoleChangeTimerHDL	BTRoleChangeTimerHDL_Ptr


//#define C2HBTMailBoxStatus		C2HBTMailBoxStatus_Ptr
#define InitBTCoexTimer			InitBTCoexTimer_Ptr

//#define InitSCOTimer				InitSCOTimer_Ptr

//#define BTSCOeSCOIssueNullHDL	BTSCOeSCOIssueNullHDL_Ptr
//#define AOAC_ANT_SWT			AOAC_ANT_SWT_CallPtr
#define AOAC_ANT_SWT			AOAC_ANT_SWT_Ptr
//#define BTNullSetting				BTNullSetting_CallPtr
#define BTNullSetting				BTNullSetting_Ptr
#define BT_TDMA_Slot_Operation_0and3	BT_TDMA_Slot_Operation_0and3_Ptr
#define FlexibleExtensionDecision		FlexibleExtensionDecision_Ptr
#define BT_TDMA_Slot_Operation_1and4	BT_TDMA_Slot_Operation_1and4_Ptr
#define BT_TDMA_Slot_Operation_2and5	BT_TDMA_Slot_Operation_2and5_Ptr
#define BackupAntennaSetting		BackupAntennaSetting_Ptr
#define BTRoleChangeDefaultSetting	BTRoleChangeDefaultSetting_Ptr
//#define BTSCOeSCOAntSetup		BTSCOeSCOAntSetup_Ptr
//#define SCO_eSCO_IssueCtrlData	SCO_eSCO_IssueCtrlData_Ptr

#define FillMailbox		     FillMailbox_Ptr
#define WL2BTMailbox		 WL2BTMailbox_Ptr
//#define H2CHDL_BTPSTDMA		H2CHDL_BTPSTDMA_8721D
//#define	H2CHDL_BTInfo		H2CHDL_BTInfo_8721D
//#define	H2CHDL_ForceBTTxpwr		H2CHDL_ForceBTTxpwr_8721D
//#define H2CHDL_BTIgnoreWlanAct		H2CHDL_BTIgnoreWlanAct_8721D
//#define H2CHDL_DACSwingValue		H2CHDL_DACSwingValue_8721D
//#define H2CHDL_AntSelReverse		H2CHDL_AntSelReverse_8721D
//#define H2CHDL_WLOpmode			H2CHDL_WLOpmode_8721D
//#define H2CHDL_BTMpH2C			H2CHDL_BTMpH2C_8721D
//#define H2CHDL_BTControl		H2CHDL_BTControl_8721D
//#define H2CHDL_BTWifiCtrl		H2CHDL_BTWifiCtrl_8721D
//#define H2CHDL_BT_SCO_eSCO_Operation		H2CHDL_BT_SCO_eSCO_Operation_8721D
//#define H2CHDL_BT_Page_Scan_Interval		H2CHDL_BT_Page_Scan_Interval_8721D
//#define H2CHDL_WLCalibration		H2CHDL_WLCalibration_8721D
//#define H2CHDL_GNT_BT_CTRL		H2CHDL_GNT_BT_CTRL_8721D
//#define H2CHDL_BT_ONLY_TEST		H2CHDL_BT_ONLY_TEST_8721D

//3 6.)power save
#define SetSmartPSTimer			SetSmartPSTimer_Ptr
#define	SetPwrStateReg			SetPwrStateReg_Ptr
#define	ResetPSParm				ResetPSParm_Ptr
//#define	ChkTxQueueIsEmpty		ChkTxQueueIsEmpty_8721D
#define	Legacy_PS_Setting		Legacy_PS_Setting_Ptr
#define PSModeSetting			PSModeSetting_Ptr
#define	ConfigListenBeaconPeriod		ConfigListenBeaconPeriod_Ptr
//#define PSSetMode			PSSetMode_8721D
//#define PS_S2ToS3State		PS_S2ToS3State_8721D
#define PS_S2ToS0State		PS_S2ToS0State_Ptr
//#define	PS_S3ToS2orS0State	PS_S3ToS2orS0State_8721D
//#define	PS_S0ToS1State		PS_S0ToS1State_8721D
//#define	PS_S1ToS0orS2State	PS_S1ToS0orS2State_8721D
//#define	PS_S2ToS4State		PS_S2ToS4State_8721D
//#define PS_S2ToS5State		PS_S2ToS5State_8721D
//#define PS_S5ToS2State		PS_S5ToS2State_8721D
#define	PS_S4ToS2State		PS_S4ToS2State_8721D_ROM
//#define	SmartPS2InitTimerAndToGetRxPkt	SmartPS2InitTimerAndToGetRxPkt_8721D
//#define SetBcnEarlyAndTimeout	SetBcnEarlyAndTimeout_CallPtr
#define SetBcnEarlyAndTimeout	SetBcnEarlyAndTimeout_Ptr
//#define BcnEarlyAdjustByTSF		BcnEarlyAdjustByTSF_8721D
#define	EnlargeBcnEarlyAndTimeout	EnlargeBcnEarlyAndTimeout_Ptr
#define ResetBcnEarlyAdjustParm		ResetBcnEarlyAdjustParm_Ptr
//#define PS_S0ToS6State		PS_S0ToS6State_8721D
//#define PS_S6ToS0State		PS_S6ToS0State_8721D


//3 7 Rate Adaptive
#define InitRAInfo_rsvd                  InitRAInfo_rsvd_Ptr
//#define TryDone                             TryDone_Ptr
#define check_rate                      check_rate_Ptr
//#define Set_arfr                        Set_arfr_CallPtr
#define Set_arfr                        Set_arfr_Ptr
#define ArfrRefresh                     ArfrRefresh_Ptr
#define InitialRateByRssi               InitialRateByRssi_Ptr
#define ODM_H2C_RADebugMonitor          ODM_H2C_RADebugMonitor_Ptr
//#define InitialRateUpdate               InitialRateUpdate_CallPtr
#define InitialRateUpdate               InitialRateUpdate_Ptr
#define RateUp_search_RateMask          RateUp_search_RateMask_Ptr
#define RateDown_search_RateMask        RateDown_search_RateMask_8721D_ROM

#define PsuseTxrpt                      PsuseTxrpt_8721D_ROM
#define ResetTxrpt                      ResetTxrpt_8721D_ROM

//3 WoWlan

#define UpdateIV                UpdateIV_Ptr
#define GetMACHeaderLen         GetMACHeaderLen_Ptr
#define WakeUpHost              WakeUpHost_Ptr
#define GetIVLenAndSecurityType GetIVLenAndSecurityType_Ptr
#define updateCheckSumVer2      updateCheckSumVer2_Ptr

//3 AES GTK
//#define AesTkipIvFun            AesTkipIvFun_CallPtr
#define AesTkipIvFun            AesTkipIvFun_Ptr
#define WepIvFun                WepIvFun_Ptr
#define OnWpa_offload           OnWpa_offload_Ptr
#define wpa_supplicant_process_1_of_2   wpa_supplicant_process_1_of_2_Ptr
#define wpa_supplicant_decrypt_key_data wpa_supplicant_decrypt_key_data_Ptr
#define wpa_supplicant_send_2_of_2      wpa_supplicant_send_2_of_2_Ptr
#define write_cam_wow                   write_cam_wow_Ptr
#define WriteCAM                WriteCAM_Ptr
#define ReadCAM                 ReadCAM_Ptr
#define ReadGTKData          ReadGTKData_Ptr
#define AppendTkipMIC       AppendTkipMIC_Ptr
#define RTmemcmpBackward       RTmemcmpBackward_Ptr
#define AesTkipIvCheck       AesTkipIvCheck_Ptr
#define WepIvCheck       WepIvCheck_Ptr
#define AssignIvToKeyRsc       AssignIvToKeyRsc_Ptr
#define AssignKeyRscToIV       AssignKeyRscToIV_Ptr



//2 IPSec

#define getSumValue             getSumValue_8721D_PTR
#define getCheckSum             getCheckSum_8721D_PTR

#define IPSecDataIn             IPSecDataIn_8721D_PTR
#define IPSecDataOut            IPSecDataOut_8721D_PTR
#define IPSecFillDesrciptor     IPSecFillDesrciptor_8721D_PTR
#define IPSWriteDescriptorIO    IPSWriteDescriptorIO_8721D_PTR
#define IPSecDesrciptor         IPSecDesrciptor_8721D_PTR
#define IPSecWriteDataIOMode    IPSecWriteDataIOMode_8721D_PTR
#define IPSecGetDataIOMode      IPSecGetDataIOMode_8721D_PTR
#define IPSecSetOPMode          IPSecSetOPMode_8721D_PTR
#define IPSecEngine             IPSecEngine_8721D_PTR
#define IPSecSetExMemoryAddr    IPSecSetExMemoryAddr_8721D_PTR
#define IPSecSetDescAddr        IPSecSetDescAddr_8721D_PTR
#define AesUnwrapIpsec          AesUnwrapIpsec_8721D_PTR
#define AESUnwrapSwap           AESUnwrapSwap_8721D_PTR
#define HmacSha1Ipsec           HmacSha1Ipsec_8721D_PTR
#define HmcMd5Ipsec             HmcMd5Ipsec_8721D_PTR
#define RC4Ipsec                RC4Ipsec_8721D_PTR
#define TkipMicIpsec            TkipMicIpsec_8721D_PTR
#define Aes128EncIpsec          Aes128EncIpsec_8721D_PTR
#define Aes128DecIpsec          Aes128DecIpsec_8721D_PTR
#define FillParameterIpsec      FillParameterIpsec_8721D_PTR
#define FillMichaelPacketInfo   FillMichaelPacketInfo_8721D_PTR
/* AES operations */
#define AESEncrypt128               AESEncrypt128_8721D_PTR
#define AESDecrypt128               AESDecrypt128_8721D_PTR
#define AES_KeyExpansion_1W         AES_KeyExpansion_1W_8721D_PTR
#define AES_CMAC_GenerateSubKey_1W  AES_CMAC_GenerateSubKey_1W_8721D_PTR
#define AES_CMAC_1W                 AES_CMAC_1W_8721D_PTR
#define AES_Encrypt_1W              AES_Encrypt_1W_8721D_PTR


//2 FWLBK
#define FwlbkDbgPrtU16              FwlbkDbgPrtU16_8721D_PTR
//#define FwlbkDbgStepByStep          FwlbkDbgStepByStep_8721D_PTR
#define FwlbkLLTWrite               FwlbkLLTWrite_8721D_PTR
#define FwlbkHWInitialLLT           FwlbkHWInitialLLT_8721D_PTR
#define FwlbkMode                   FwlbkMode_8721D_PTR
#define FwlbkPreparePkt             FwlbkPreparePkt_8721D_PTR
#define FwlbkModeSel                FwlbkModeSel_8721D_PTR
#define FwlbkCheckRxPayload         FwlbkCheckRxPayload_8721D_PTR
#define FwlbkSetCAM                 FwlbkSetCAM_8721D_PTR
#define FwlbkUpdateParam            FwlbkUpdateParam_8721D_PTR
#define FwlbkMcuPollMgq             FwlbkMcuPollMgq_8721D_PTR
#define FwlbkModeTxNoCheck          FwlbkModeTxNoCheck_8721D_PTR
#define FwlbkModeTx                 FwlbkModeTx_8721D_PTR
#define FwlbkModeMaclbk             FwlbkModeMaclbk_8721D_PTR
#define FwlbkModeSecTest            FwlbkModeSecTest_8721D_PTR
#define FwlbkModeAFElbk             FwlbkModeAFElbk_8721D_PTR

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

#ifndef __ASSEMBLY__

typedef struct _HAL_DATA_COMMON {

    u8  HCISel;

}HAL_DATA_COMMON, *PHAL_DATA_COMMON;

typedef struct _HAL_DATA_8721D {

    //IMR & ISR
#if 0    
    u8  sysimr[4];
    u8  sysisr[4];
#endif

    u8  wlanimr[4];
    u8  wlanisr[4];
    
    u8  wlanimr2[4];
    u8  wlanisr2[4];
    
    u8  ftimr[4];
    u8  ftisr[4];
}HAL_DATA_8721D, *PHAL_DATA_8721D;

#endif //__ASSEMBLY__
/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


#endif  //__RTL8721D_HAL_H__

