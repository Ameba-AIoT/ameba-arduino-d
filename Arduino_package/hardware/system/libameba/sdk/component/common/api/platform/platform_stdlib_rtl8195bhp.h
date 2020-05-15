#ifndef PLATFORM_STDLIB_RTL8195BHP_H
#define PLATFORM_STDLIB_RTL8195BHP_H


#undef USE_RTL_ROM_CLIB
#define USE_RTL_ROM_CLIB	0

#if 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "basic_types.h"
  //#include <diag.h>

  //#undef rt_printf
  //#define rt_printf             stdio_printf_stubs.rt_printf
  //#define printf                rt_printf


//
// memory management
//
//extern void *pvPortMalloc( size_t xWantedSize );
//extern void vPortFree( void *pv );
//#define malloc                  pvPortMalloc
//#define free                    vPortFree
#endif

#endif //PLATFORM_STDLIB_RTL8195BHP_H
