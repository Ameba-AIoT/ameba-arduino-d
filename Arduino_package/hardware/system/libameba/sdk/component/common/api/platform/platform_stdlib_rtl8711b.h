#ifndef PLATFORM_STDLIB_RTL8711B_H
#define PLATFORM_STDLIB_RTL8711B_H

#define USE_CLIB_PATCH		0

#if defined(CONFIG_BUILD_ROM) || defined(CONFIG_MBED_ENABLED)
	#define USE_RTL_ROM_CLIB	0
#else
	#define BUFFERED_PRINTF		0
	#define USE_RTL_ROM_CLIB	1
#endif

#if defined (__IARSTDLIB__)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdint.h>
	#include <stdarg.h> /* va_list */
	#include "diag.h"

	#define strsep(str, delim)      	_strsep(str, delim)
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h> /* va_list */
	#include "diag.h"
	#include "strproc.h"
	#include "memproc.h"
	#include "basic_types.h"
#if USE_RTL_ROM_CLIB
	#include "rtl_lib.h"
	#include "rom_libc_string.h"
#endif

	#undef printf
	#undef sprintf
	#undef snprintf
	#undef memchr
	#undef memcmp
	#undef memcpy
	#undef memset
	#undef memmove
	#undef strcmp
	#undef strcpy
	#undef strlen
	#undef strncmp
	#undef strncpy
	#undef strsep
	#undef strtok
	#undef strcat
	#undef strchr
	#undef strncat
	#undef strstr
	#undef atol
	#undef atoi
	#undef strpbrk
	
#if USE_RTL_ROM_CLIB
#if BUFFERED_PRINTF
        extern int buffered_printf(const char* fmt, ...);
        #define printf				buffered_printf
#else
	#define printf				rtl_printf
#endif
	#define sprintf				rtl_sprintf
	#define snprintf				rtl_snprintf
	#define vsnprintf				rtl_vsnprintf
#else
	#define printf					DiagPrintf
	#define sprintf(fmt, arg...)			DiagSPrintf((u8*)fmt, ##arg)
	#define snprintf					DiagSnPrintf			// NULL function
	#define vsnprintf(buf, size, fmt, ap)	VSprintf(buf, fmt, ap)
#endif
	#define memchr						__rtl_memchr_v1_00
	#define memcmp(dst, src, sz)		_memcmp(dst, src, sz)
	#define memcpy(dst, src, sz)		_memcpy(dst, src, sz)
	#define memmove						__rtl_memmove_v1_00
	#define memset(dst, val, sz)		_memset(dst, val, sz)
	
	#define strchr(s, c)				_strchr(s, c)			// for B-cut ROM
	#define strcmp(str1, str2)			prvStrCmp((const unsigned char *) str1, (const unsigned char *) str2)
	#define strcpy(dest, src)			_strcpy(dest, src)
	#define strlen(str)				prvStrLen((const unsigned char *) str)
	#define strsep(str, delim)			_strsep(str, delim)
	#define strstr(str1, str2)			prvStrStr(str1, str2)	// NULL function
	#define strtok(str, delim)			prvStrtok(str, delim)//_strsep(str, delim)
	#define strcat					__rtl_strcat_v1_00
	
	#define strncmp(str1, str2, cnt)	_strncmp(str1, str2, cnt)
	#define strncpy(dest, src, count)	_strncpy(dest, src, count)
	#define strncat					__rtl_strncat_v1_00
	
	#define atol(str)					strtol(str,NULL,10)
	#define atoi(str)					prvAtoi(str)	
	#define strpbrk(cs, ct)			_strpbrk(cs, ct)		// for B-cut ROM
#if defined (__GNUC__)
	#undef sscanf
	#define sscanf					_sscanf_patch
	#define rand					Rand
	#define srand				
#endif
	//extern int _sscanf_patch(const char *buf, const char *fmt, ...);
	//#define sscanf					_sscanf_patch
	

#endif	// defined (__IARSTDLIB__)

//
// memory management
//
extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );
#define malloc                  pvPortMalloc
#define free                    vPortFree

#endif // PLATFORM_STDLIB_RTL8711B_H
