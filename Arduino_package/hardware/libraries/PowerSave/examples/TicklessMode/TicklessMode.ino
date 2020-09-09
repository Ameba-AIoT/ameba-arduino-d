/*
  This sketch shows how to use power save tickless mode
*/
#include "ameba_soc.h"
#include <PowerSave.h>

#define TICKLESS_MODE                   22
#define SET_TL_UART_WAKEUP              0
#define SET_TL_RTC_WAKEUP               1
#define SET_TL_AON_WAKEPIN_WAKEUP       16 // D16:16    D17:17    D26:26    D27:27

//SET_TL_UART_WAKEUP:0; SET_TL_RTC_WAKEUP:1; SET_TL_AON_WAKEPIN_WAKEUP:16 or 17 or 26 or 27;
#define TL_WAKEUP_SOURCE                SET_TL_UART_WAKEUP

#define TL_SYSACTIVE_TIME               5000
#define TL_RTC_ALARM_DAY                0
#define TL_RTC_ALARM_HOUR               0
#define TL_RTC_ALARM_MIN                0
#define TL_RTC_ALARM_SEC                10

#if (TL_WAKEUP_SOURCE == 0)
    char TL_UART_WAKEUP[] = "Set Tickless wakeup LOGUART";
#elif (TL_WAKEUP_SOURCE == 1)
    char TL_RTC_WAKEUP[] = "Set Tickless wakeup RTC";
#elif (TL_WAKEUP_SOURCE == 16)
    char TL_AON_WAKEPIN_WAKEUP_D16[] = "Set Tickless wakeup AON wake pin D16";
#elif (TL_WAKEUP_SOURCE == 17)
    char TL_AON_WAKEPIN_WAKEUP_D17[] = "Set Tickless wakeup AON wake pin D17";
#elif (TL_WAKEUP_SOURCE == 26)
    char TL_AON_WAKEPIN_WAKEUP_D26[] = "Set Tickless wakeup AON wake pin D26";
#elif (TL_WAKEUP_SOURCE == 27)
    char TL_AON_WAKEPIN_WAKEUP_D27[] = "Set Tickless wakeup AON wake pin D27";
#endif


uint32_t TL_Suspend_function(void) {
#if (TL_WAKEUP_SOURCE == 0)
    printf("%s.    \r\n", TL_UART_WAKEUP);
#elif (TL_WAKEUP_SOURCE == 1)
    printf("%s.    \r\n", TL_RTC_WAKEUP);
    PowerSave.RTCWakeSetup(TL_RTC_ALARM_DAY, TL_RTC_ALARM_HOUR, TL_RTC_ALARM_MIN, TL_RTC_ALARM_SEC);
#elif (TL_WAKEUP_SOURCE == 16)
    printf("%s.    \r\n", TL_AON_WAKEPIN_WAKEUP_D16);
#elif (TL_WAKEUP_SOURCE == 17)
    printf("%s.    \r\n", TL_AON_WAKEPIN_WAKEUP_D17);
#elif (TL_WAKEUP_SOURCE == 26)
    printf("%s.    \r\n", TL_AON_WAKEPIN_WAKEUP_D26);
#elif (TL_WAKEUP_SOURCE == 27)
    printf("%s.    \r\n", TL_AON_WAKEPIN_WAKEUP_D27);
#endif

    printf("\r\nSytem suspend. Tickless mode enabled.    \r\n");
    return TRUE;
}

uint32_t TL_Resume_fnction(void) {
    printf("\r\nTickless mode resume. Wait 5s enter tickless again.     \r\n");

    PowerSave.TL_sysactive_time(TL_SYSACTIVE_TIME);
    return TRUE;
}

void setup() {
    PowerSave.begin(TICKLESS_MODE);
    PowerSave.TL_wakelock(1);
    pmu_register_sleep_callback(PMU_LOGUART_DEVICE, (PSM_HOOK_FUN)TL_Suspend_function, NULL, (PSM_HOOK_FUN)TL_Resume_fnction, NULL);
    PowerSave.TL_wakelock(0);
}

void loop() {
    delay(1000);
}
