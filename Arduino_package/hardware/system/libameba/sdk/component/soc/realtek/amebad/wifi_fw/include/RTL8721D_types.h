
#ifndef _ARCH_TYPES_H
#define _ARCH_TYPES_H

//-------------------------------------------------------
//
//      Belows are Keil-C Related.
//
//-------------------------------------------------------
typedef unsigned char           BOOLEAN,*PBOOLEAN;

typedef unsigned char           u1Byte,*pu1Byte;
typedef unsigned short          u2Byte,*pu2Byte;
typedef unsigned long           u4Byte,*pu4Byte;

typedef char                    s1Byte,*ps1Byte;
typedef short                   s2Byte,*ps2Byte;
typedef long                    s4Byte,*ps4Byte;

#ifndef PVOID
typedef void                    * PVOID;
#endif

typedef signed char              s8;
typedef unsigned char           u8;

typedef short                   s16;
typedef unsigned short          u16;

#ifndef s32
typedef long                    s32;
#endif

#ifndef u32
typedef unsigned long           u32;
#endif

#ifdef _MCU_8051_
//8051 Variable Attribute
#define MEMTYPE_CODE            code
#define MEMTYPE_DATA            data
#define MEMTYPE_IDATA           idata
#define MEMTYPE_BDATA           bdata
#define MEMTYPE_XDATA           xdata
#define MEMTYPE_FAR             far
#define MEMTYPE_PDATA           pdata

//8051 Memory Model Selection
#define MEMMDL_SMALL            small
#define MEMMDL_COMPACT          compact
#define MEMMDL_LARGE            large
#else
#define MEMTYPE_CODE            
#define MEMTYPE_DATA            
#define MEMTYPE_IDATA           
#define MEMTYPE_BDATA           
#define MEMTYPE_XDATA           
#define MEMTYPE_FAR             
#define MEMTYPE_PDATA           

#define MEMMDL_SMALL            
#define MEMMDL_COMPACT          
#define MEMMDL_LARGE            
//#define NULL                    0
#define printf                  DBG_8195A
#define RTK_PRINT(_comp, _level, fmt,arg)    DBG_8195A(fmt,arg) 
#endif

//Keil-C Attribute
#ifdef _MCU_8051_
#define __TASK__                _task_
#define __INT__                 interrupt
#else
#define __TASK__
#define __INT__
#endif

//8051 Read/Write IO Macro
#ifdef _MCU_8051_
#define ReadXBYTE(addr)         XBYTE[addr]
#define WriteXBYTE(addr,value)  (XBYTE[addr] = (value))
#define ReadCBYTE(addr)         CBYTE[addr]
#define ReadXWORD(addr)         ((((u16)XBYTE[addr+1])<<8)+((u16)XBYTE[addr]))
#define ReadTXBUFWORD(offset) ((((u16)TxPktBuf[offset+1])<<8) +((u16)TxPktBuf[offset]))
#define ReadRXBUFWORD(offset) ((((u16)RxPktBuf[offset+1])<<8) +((u16)RxPktBuf[offset]))
#define ReadBUFWORD(buf,offset) ((((u16)buf[offset+1])<<8) +((u16)buf[offset]))
#define ReadMSBBUFWORD(buf,offset) ((((u16)buf[offset])<<8) +((u16)buf[offset+1]))
#define ReadTXBUFLONG(offset) ((((u32)TxPktBuf[offset+3])<<24)+(((u32)TxPktBuf[offset+2])<<16)+(((u32)TxPktBuf[offset+1])<<8)+((u32)TxPktBuf[offset]))
#define _U16_MSB(x)             (((u16)(x))>>8)
#define _U16_LSB(x)             ((u8)(x))
#define _B2W(LSB, MSB)          ((u16)(LSB) | (((u16)(MSB))<<8))
#define _B2D(x0,x1,x2,x3)       ((u32)(x0)|((u32)(x1)<<8)|((u32)(x2)<<16)|((u32)(x3)<<24))
#define _D2B(x, idx)            (u8)(((u32)(x))>>(8*(idx)))
#define _L2B(L)                 (((u32)(L&0xFF000000)>>24)|((u32)(L&0x00FF0000)>>8)|((u32)(L&0x0000FF00)<<8)|((u32)(L&0x000000FF)<<24))
#else
#if 0
#define ReadXBYTE(addr)         (*(u8 *)(addr))
#define WriteXBYTE(addr,value)  (*(u8 *)(addr) = (value))
#define ReadXWORD(addr)         (*(u16 *)(addr))
#define WriteXWORD(addr,value)  (*(u16 *)(addr) = (value))
#define ReadXDWORD(addr)         (*(u32 *)(addr))
#define WriteXDWORD(addr,value)  (*(u32 *)(addr) = (value))
#define ReadTXBUFWORD(offset) *((u16 *)((char *)TxPktBuf+offset))
#define ReadRXBUFWORD(offset) *((u16 *)((char *)RxPktBuf+offset))
#define ReadMSBBUFWORD(buf,offset)  *((u16 *)((char *)(buf)+offset))
#define ReadTXBUFLONG(offset) *((int *)((char *)TxPktBuf+offset))
#endif

#if !defined(__IARSTDLIB__)
#ifndef memcmp
#define memcmp(dst, src, sz)			_memcmp(dst, src, sz)
#endif
#ifndef memset
#define memset(dst, val, sz)			_memset(dst, val, sz)
#endif
#ifndef memcpy
#define memcpy(dst, src, sz)			_memcpy(dst, src, sz)
#endif
#endif

#define _U16_MSB(x)             (((u16)(x))>>8)
#define _U16_LSB(x)             ((u8)(x))
#define _B2W(LSB, MSB)          ((u16)(LSB) | (((u16)(MSB))<<8))
#define _B2D(x0,x1,x2,x3)       ((u32)(x0)|((u32)(x1)<<8)|((u32)(x2)<<16)|((u32)(x3)<<24))
#define _D2B(x, idx)            (u8)(((u32)(x))>>(8*(idx)))
#define _L2B(L)                 (((u32)(L&0xFF000000)>>24)|((u32)(L&0x00FF0000)>>8)|((u32)(L&0x0000FF00)<<8)|((u32)(L&0x000000FF)<<24))
#define _nop_()
#endif


#endif  //#ifndef _ARCH_TYPES_H
