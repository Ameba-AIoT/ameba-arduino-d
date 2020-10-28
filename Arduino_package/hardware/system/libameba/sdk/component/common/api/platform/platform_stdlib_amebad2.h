#ifndef PLATFORM_STDLIB_AMEBAD2_H
#define PLATFORM_STDLIB_AMEBAD2_H

#define CONFIG_PLATFORM_AMEBA_X 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* va_list */	
#include "diag.h"

#if defined (__IARSTDLIB__)
	#include <stdint.h>
	#define strsep(str, delim)      	_strsep(str, delim)
#else
	#include "memproc.h"
	#include "basic_types.h"
	#include "rtl8721d.h"
	#include "strproc.h"

	#undef rand
	#define rand						_rand

#endif	// defined (__IARSTDLIB__)

extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );
extern void *pvPortReAlloc( void *pv,  size_t xWantedSize );
#define malloc                  pvPortMalloc
#define free                    vPortFree
#define realloc			pvPortReAlloc
#define calloc			rtw_calloc


#endif // PLATFORM_STDLIB_AMEBAD2_H
