#ifndef __INC_GENERALDEF_H
#define __INC_GENERALDEF_H

/*
 *  Note:   1.Only pure value definition can be put here.
 *          2.The definition here should be hardware and platform independent.
 *
*/

#define IN
#define OUT

#define VOID void

#define SUCCESS     0
#define FAIL        (-1)

#define _SUCCESS    1
#define _FAIL       0

#if 0
#undef _TRUE
#define _TRUE       1

#undef _FALSE
#define _FALSE      0

#endif

//Filen:
//  true, false, TRUE, FALSE  are not used as possible.
#ifndef true
#define true        1
#endif

#ifndef false
#define false       0
#endif

#ifndef TRUE
#define TRUE        true
#endif

#ifndef FALSE
#define FALSE       false
#endif

#ifndef BIT
//Below's Macro is necessary to take care for 8051 Keil-C
//Ex. BIT(16) is eqaul to zero in 8051 Keil-C
#define BIT(x)  (1 << (x))
#endif

#if 0
#define BIT0        0x00000001
#define BIT1        0x00000002
#define BIT2        0x00000004
#define BIT3        0x00000008
#define BIT4        0x00000010
#define BIT5        0x00000020
#define BIT6        0x00000040
#define BIT7        0x00000080
#define BIT8        0x00000100
#define BIT9        0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000
#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000
#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000
#endif

#define ON          1
#define OFF         0

#define Alignment_64byte(len)   (((len + (((len&0x3F)==0)?0:64))>>6)<<6)
#define Alignment_128byte(len)  (((len + (((len&0x7F)==0)?0:128))>>7)<<7)
//  TODO: [Dangerous] It is necessary to be checked Correctness if below's Macros are used in 8051.
#define Alignment_256byte(len)  (((len + (((len&0xFF)==0)?0:256))>>8)<<8)
#define Alignment_512byte(len)  (((len + (((len&0x01FF)==0)?0:512))>>9)<<9)
#define Alignment_1024byte(len) (((len + (((len&0x03FF)==0)?0:1024))>>10)<<10)

#endif // #ifndef __INC_GENERALDEF_H
