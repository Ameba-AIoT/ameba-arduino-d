#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "wdt_api.h"

//extern u32 ConfigDebugErr;
extern u32 ConfigDebug[LEVEL_NUMs];


void wdt_reset(void) {
    watchdog_refresh();
}

void wdt_enable(uint32_t timeout_ms) {
    uint32_t backup_ConfigDebugErr;

    backup_ConfigDebugErr = ConfigDebug[0];
    ConfigDebug[0] = 0x00000000;

    if (timeout_ms > 8000) {
        timeout_ms = 8000;
    }
    watchdog_init(timeout_ms);
    watchdog_start();

    ConfigDebug[0] = backup_ConfigDebugErr;
}

void wdt_disable(void) {
    watchdog_stop();
}

#ifdef __cplusplus
} // extern "C"
#endif
