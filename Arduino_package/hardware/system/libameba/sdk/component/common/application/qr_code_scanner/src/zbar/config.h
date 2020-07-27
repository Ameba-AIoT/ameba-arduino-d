#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

/* include/config.h.  Generated from config.h.in by configure.  */
/* include/config.h.in.  Generated from configure.ac by autoheader.  */

/* whether to build support for Code 128 symbology */
//#define ENABLE_CODE128 0

/* whether to build support for Code 39 symbology */
//#define ENABLE_CODE39 0

/* whether to build support for EAN symbologies */
//#define ENABLE_EAN 0

/* whether to build support for Interleaved 2 of 5 symbology */
//#define ENABLE_I25 0

/* whether to build support for PDF417 symbology */
//#define ENABLE_PDF417 0

/* whether to build support for QR Code */
#define ENABLE_QRCODE 1

/* Program major version (before the '.') as a number */
#define ZBAR_VERSION_MAJOR 0

/* Program minor version (after '.') as a number */
#define ZBAR_VERSION_MINOR 10

#define 	PRIx32   "lx"

#define OPTIMIZE_MEM_SIZE 0 //0: not optimize mem size in binarize; 1: binarize alloc mem size become 1/4; 2: binarize alloc mem size become 1/32;
#define OPTIMIZE_TIME 1
#define REMOVE_BK 1

#define UTF8_TO_GBK 0

#define USE_QR_CODE_STDLIB 0

#endif


