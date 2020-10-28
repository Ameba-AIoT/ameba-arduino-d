/*
  main.cpp - Main loop for Arduino sketches
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

#define ARDUINO_MAIN
#include "Arduino.h"
#include "cmsis_os.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void console_init(void);

osThreadId main_tid = 0;

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

/*
 * \brief handle sketch
 */

// zzw 
//void main_task(void const *arg)
void main_task(void)
{
    delay(1);

    setup();

    for (;;) {
        loop();
        if (serialEventRun) {
            serialEventRun();
        }
        osThreadYield();
    }
}

/*
 * \brief Main entry point of Arduino application
 */
int main(void)
{
    init();

    initVariant();

    osThreadDef(main_task, osPriorityRealtime, 1, MAIN_THREAD_STACK_SIZE);
    main_tid = osThreadCreate(osThread(main_task), NULL);

    osKernelStart();

    while(1);

    return 0;
}

#ifdef __cplusplus
}
#endif
