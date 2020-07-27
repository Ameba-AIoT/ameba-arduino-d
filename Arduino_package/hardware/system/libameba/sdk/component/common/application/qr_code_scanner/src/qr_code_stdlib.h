/************************************************************************************************/
//qr_code_stdlib.h
/************************************************************************************************/

#ifndef __QR_CODE_STDLIB_H__
#define __QR_CODE_STDLIB_H__

#include <config.h>

#if USE_QR_CODE_STDLIB
#define malloc	qr_code_malloc
#define free	qr_code_free
#define memcpy	qr_code_memcpy
#define memcmp	qr_code_memcmp
#define memset	qr_code_memset
#define calloc	qr_code_calloc
#define realloc	qr_code_realloc
#define memmove	qr_code_memmove
#define memchr	qr_code_memchr
#else
#include <stdlib.h>
#endif

#ifndef NULL
#define NULL	0
#endif

#define assert(x)		do {} while(0)
#define fprintf(...)	do {} while(0)

#endif

