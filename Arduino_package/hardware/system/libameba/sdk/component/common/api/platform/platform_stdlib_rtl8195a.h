#ifndef PLATFORM_STDLIB_RTL8195A_H
#define PLATFORM_STDLIB_RTL8195A_H

#define USE_CLIB_PATCH		0


#if defined (__GNUC__)
	/* build rom should set USE_RTL_ROM_CLIB=0 */
	#if !defined(CONFIG_MBED_ENABLED)
		#include <rt_lib_rom.h>
	#endif
#endif

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
	#include "diag.h"

	#define strsep(str, delim)      	_strsep(str, delim)
#elif defined (__CC_ARM)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdint.h>
	#include "diag.h"
	#define strsep(str, delim)      	_strsep(str, delim)
#elif defined (CONFIG_MBED_ENABLED)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdint.h>
	#include "diag.h"

	#define strsep(str, delim)      	_strsep(str, delim)
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "diag.h"
	#include "strproc.h"
	#include "basic_types.h"
	#include "hal_misc.h"
  #if USE_RTL_ROM_CLIB
	#include "rtl_lib.h"
  #endif

	#undef printf
	#undef sprintf
	#undef snprintf
	#undef atoi
	#undef memcmp
	#undef memcpy
	#undef memset
	#undef strcmp
	#undef strcpy
	#undef strlen
	#undef strncmp
	#undef strncpy
	#undef strsep
	#undef strtok
  #if USE_RTL_ROM_CLIB
	#undef memchr
	#undef memmove
	#undef strcat
	#undef strchr
	#undef strncat
	#undef strstr
  #endif

  #if USE_RTL_ROM_CLIB
		#if BUFFERED_PRINTF
			extern int buffered_printf(const char* fmt, ...);
			#define printf				buffered_printf
		#else
			#define printf                  	rtl_printf
		#endif

	#define sprintf						rtl_sprintf
	#define snprintf					rtl_snprintf
	#define memchr						rtl_memchr
	#define memcmp						rtl_memcmp
	#define memcpy						rtl_memcpy
	#define memmove						rtl_memmove
	#define memset						rtl_memset
	#define strcat						rtl_strcat
	#define strchr						rtl_strchr
	#define strcmp(s1, s2)				rtl_strcmp((const char *)s1, (const char *)s2)
	#define strcpy						rtl_strcpy
	#define strlen(str)					rtl_strlen((const char *)str)
	#define strncat						rtl_strncat
	#define strncmp(s1, s2, n)			rtl_strncmp((const char *)s1, (const char *)s2, n)
	#define strncpy						rtl_strncpy
	#define strstr						rtl_strstr
	#define strsep						rtl_strsep
	#define strtok						rtl_strtok
  #else
	#if USE_CLIB_PATCH
		extern int DiagSscanfPatch(const char *buf, const char *fmt, ...);
		extern char* DiagStrtokPatch(char *str, const char* delim);
		extern char* DiagStrstrPatch(char *string, char *substring);
		extern int DiagSnPrintfPatch(char *buf, size_t size, const char *fmt, ...);
		extern u32 DiagPrintfPatch(const char *fmt, ...);
		extern u32  DiagSPrintfPatch(u8 *buf, const char *fmt, ...);
		#define printf						DiagPrintfPatch
		#define sprintf						DiagSPrintfPatch
		#define snprintf					DiagSnPrintfPatch
		#define strstr(a, b)			DiagStrstrPatch((char *)(a), (char *)(b))
		#define strtok						DiagStrtokPatch
	#else
		#define printf                DiagPrintf
		#define sprintf(fmt, arg...)	DiagSPrintf((u8*)fmt, ##arg)
	  #if defined (__GNUC__)
		#define snprintf                DiagSnPrintf			// NULL function
		#define strstr(str1, str2)		prvStrStr(str1, str2)	// NULL function
	  #endif
		#define strtok(str, delim)      _strsep(str, delim)
	#endif
	#define memcmp(dst, src, sz)		_memcmp(dst, src, sz)
	#define memcpy(dst, src, sz)		_memcpy(dst, src, sz)
	#define memset(dst, val, sz)		_memset(dst, val, sz)
	#define strchr(s, c)						_strchr(s, c)			// for B-cut ROM
	#define strcmp(str1, str2)      	prvStrCmp((const unsigned char *) str1, (const unsigned char *) str2)
	#define strcpy(dest, src)				_strcpy(dest, src)
	#define strlen(str)             	prvStrLen((const unsigned char *) str)
	#define strncmp(str1, str2, cnt)	_strncmp(str1, str2, cnt)
	#define strncpy(dest, src, count)	_strncpy(dest, src, count)
	#define strsep(str, delim)      	_strsep(str, delim)
  #endif

	#define atoi(str)               	prvAtoi(str)	
	#define strpbrk(cs, ct)				_strpbrk(cs, ct)		// for B-cut ROM

  #if USE_CLIB_PATCH
	#undef sscanf
	#define sscanf						DiagSscanfPatch
  #else
    #if defined (__GNUC__)
			#undef sscanf 	//_sscanf
			//extern int DiagSscanfPatch(const char *buf, const char *fmt, ...);
			//#define sscanf						DiagSscanfPatch
			#define sscanf		sscanf	// use libc sscanf
    #endif
  #endif
#endif	// defined (__IARSTDLIB__)

//
// memory management
//
#if defined(CONFIG_MBED_ENABLED)
	//use libc memory functions
#else
	extern void *pvPortMalloc( size_t xWantedSize );
	extern void vPortFree( void *pv );
	#define malloc                  pvPortMalloc
	#define free                    vPortFree
#endif


#endif // PLATFORM_STDLIB_RTL8195A_H
