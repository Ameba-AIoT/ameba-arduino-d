/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define Arduino_STD_PRINTF

#ifdef Arduino_STD_PRINTF
#include <stdio.h>
#endif

#include "binary.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "wiring_constants.h"

extern uint32_t SystemCoreClock;

#define clockCyclesPerMicrosecond() (SystemCoreClock / 1000000L)
#define clockCyclesToMicroseconds(a) (((a) * 1000L) / (SystemCoreClock / 1000L))
#define microsecondsToClockCycles(a) ((a) * (SystemCoreClock / 1000000L))

void yield(void);

extern uint32_t                     DiagPrintf(const char *fmt, ...);
extern int                          _rtl_printf(const char *fmt, ...);
extern int                          _rtl_sprintf(char* str, const char* fmt, ...);

#ifndef Arduino_STD_PRINTF
#ifndef printf
#define printf                      _rtl_printf
#endif
#ifndef sprintf
#define sprintf                     _rtl_sprintf
#endif
#endif

extern void *pvPortMalloc(size_t xWantedSize);
extern void vPortFree(void *pv);
extern void *pvPortReAlloc(void *pv, size_t xWantedSize);
#ifndef malloc
#define malloc                      pvPortMalloc
#endif
#ifndef free
#define free                        vPortFree
#endif
#ifndef realloc
#define realloc                     pvPortReAlloc
#endif

/* sketch */
extern void setup( void );
extern void loop( void );

#define NOT_INITIAL     (1UL<<0)
#define PIO_GPIO        (1UL<<1)
#define PIO_PWM         (1UL<<2)
#define PIO_I2C         (1UL<<3)
#define PIO_ADC         (1UL<<4)
#define PIO_DAC         (1UL<<5)
#define PIO_GPIO_IRQ    (1UL<<6)

#define PWM_MODE_ENABLED  1
#define PWM_MODE_DISABLED 0

/* Types used for the tables below */
typedef struct _PinDescription
{
    // HW PinNames
    uint32_t    pinname;

    // Current Pin Type
    uint32_t    ulPinType;

    // Supported Pin Function
    uint32_t    ulPinAttribute;

    // Current Pin Mode
    uint32_t    ulPinMode;
} PinDescription;

/* Pins table to be instanciated into variant.cpp */
extern PinDescription g_APinDescription[];

#ifdef __cplusplus
} // extern "C"

#include "WCharacter.h"
#include "WString.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "wiring_pulse.h"

#endif // __cplusplus

// Include board variant
#include "variant.h"
#include "wiring.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "WInterrupts.h"
#include "wiring_os.h"
#include "wiring_watchdog.h"
#include "wiring_shift.h"

// C++ functions
#ifdef __cplusplus


// WMath prototypes
extern long random( long ) ;
extern long random( long, long ) ;
extern void randomSeed( uint32_t dwSeed ) ;
extern long map( long, long, long, long, long ) ;

void tone(uint32_t ulPin, unsigned int frequency, unsigned long duration = 0);

#endif

//
// custom defined
//

//#include "Thread.h"
//#include "DAC1.h"


#endif // Arduino_h
