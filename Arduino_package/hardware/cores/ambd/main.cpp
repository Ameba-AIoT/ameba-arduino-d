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
 * //app_mbedtls_rom_init 
 */
static void* app_mbedtls_calloc_func(size_t nelements, size_t elementSize)
{
    size_t size;
    void *ptr = NULL;

    size = nelements * elementSize;
    ptr = pvPortMalloc(size);

    if (ptr) {
        _memset(ptr, 0, size);
    }

    return ptr;
}

void app_mbedtls_rom_init(void)
{
    mbedtls_platform_set_calloc_free(app_mbedtls_calloc_func, vPortFree);
    //rom_ssl_ram_map.use_hw_crypto_func = 1;
    rtl_cryptoEngine_init();
}

/*
 * \brief handle sketch
 */
#if defined(Arduino_STD_PRINTF)
// Redirect regular printf output to UART
int _write(int file, char *ptr, unsigned int len){
    (void)file;
    unsigned int i;
    for(i = 0; i < len; i++){
        while (!UART_Writable((UART_TypeDef*)UART2_DEV));
        UART_CharPut((UART_TypeDef*)UART2_DEV, ptr[i]);
    }
    return i;
}
#endif

//void main_task(void const *arg)
//void main_task(void *arg)
void main_task(void)
{
//    (void)arg;

    delay(1);

    setup();

    for (;;) {
        loop();
        if (serialEventRun) {
            serialEventRun();
        }
        osThreadYield();
        //vTaskDelete(NULL);
    }
}

/*
 * \brief Main entry point of Arduino application
 */
int main(void)
{
    init();

    initVariant();

#ifdef CONFIG_MBED_TLS_ENABLED
    app_mbedtls_rom_init();
#endif

//For all amebad boards, Analog pin needs to pull none. GPIO_PuPd_NOPULL/GPIO_PuPd_DOWN/GPIO_PuPd_UP
    PAD_PullCtrl(_PB_1, GPIO_PuPd_NOPULL);
    PAD_PullCtrl(_PB_2, GPIO_PuPd_NOPULL);
    PAD_PullCtrl(_PB_3, GPIO_PuPd_NOPULL);

#if 1
    osThreadDef(main_task, osPriorityNormal, 1, MAIN_THREAD_STACK_SIZE);
    main_tid = osThreadCreate(osThread(main_task), NULL);

    osKernelStart();
#else
    if(xTaskCreate(main_task, ((const char*)"init"), MAIN_THREAD_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3 + PRIORITIE_OFFSET, NULL) != pdPASS) {
        printf("\n\r%s xTaskCreate(main_task) failed", __FUNCTION__);
    }

    vTaskStartScheduler();
#endif

    while(1);

    return 0;
}

#ifdef __cplusplus
}
#endif
