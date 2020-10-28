/**
 * This example describes how to use watchdog api.
 * In this example, watchdog is setup to 5s timeout.
 * Watchdog won't bark if we refresh it before timeout in smallTask.
 * The timer is also reloaded after refresh.
 * Otherwise, while running bigTask, watchdog will restart system in default or call callback function if registered.
*/
#include "WDT.h"

#define RUN_CALLBACK_IF_WATCHDOG_BARKS (0)
WDT wdt;

void setup() {
    Serial.begin(115200);
    wdt.InitWatchdog(5000);  // setup 5s watchdog

#if RUN_CALLBACK_IF_WATCHDOG_BARKS
    wdt.InitWatchdogIRQ(my_watchdog_irq_handler, 0);
#else
    // system would restart in default when watchdog barks
#endif

    wdt.StartWatchdog();  // enable watchdog timer
    successfulTask();
    failedTask();
    while(1);
}

void loop() {
    delay(1000);
}

void successfulTask(void) {
    Serial.println("......doing small task......");
    for (int i = 0; i < 50000000; i++)  // dummy task
        asm(" nop");
    Serial.println("refresh watchdog");
    wdt.RefreshWatchdog();
}

/**
 * Doing this task will lead to failed refresh the 
 * watchdog timer within the time limits of 5 seconds
*/
void failedTask(void) {
    Serial.println("......doing big task......");
    for (int i = 0; i < 10; i++) {
        Serial.print("doing dummy task #");
        Serial.println(i, DEC);

        for (int j = 0; j < 50000000; j++) {  // dummy task
            asm(" nop");
        }
    }
    Serial.println("refresh watchdog");
    wdt.RefreshWatchdog();
}

void my_watchdog_irq_handler(uint32_t id) {
    printf("watchdog barks!!!\r\n");
    WDG_Cmd(DISABLE);
}
