/*
  This sketch shows how to use power save tickless mode
*/
#include <PowerSave.h>

//SET_TL_UART_WAKEUP
//SET_TL_RTC_WAKEUP
//SET_TL_AON_GPIO_WAKEUP_D16
//SET_TL_AON_GPIO_WAKEUP_D17
//SET_TL_AON_GPIO_WAKEUP_D26
//SET_TL_AON_GPIO_WAKEUP_D27
#define TL_WAKEUP_SOURCE                SET_TL_UART_WAKEUP

#define TL_SYSACTIVE_TIME               5000

#define TL_RTC_ALARM_DAY                0
#define TL_RTC_ALARM_HOUR               0
#define TL_RTC_ALARM_MIN                0
#define TL_RTC_ALARM_SEC                10

uint32_t TL_Suspend_function(void) {
    switch (TL_WAKEUP_SOURCE) {
        case SET_TL_UART_WAKEUP:
            PowerSave.TL_UART_WAKEUP();
            break;
        case SET_TL_RTC_WAKEUP:
            PowerSave.TL_RTC_WAKEUP();
            PowerSave.RTCWakeSetup(TL_RTC_ALARM_DAY, TL_RTC_ALARM_HOUR, TL_RTC_ALARM_MIN, TL_RTC_ALARM_SEC);
            break;
        case SET_TL_AON_GPIO_WAKEUP_D16:
            PowerSave.TL_AON_WAKEPIN_WAKEUP_D16();
            break;
        case SET_TL_AON_GPIO_WAKEUP_D17:
            PowerSave.TL_AON_WAKEPIN_WAKEUP_D17();
            break;
        case SET_TL_AON_GPIO_WAKEUP_D26:
            PowerSave.TL_AON_WAKEPIN_WAKEUP_D26();
            break;
        case SET_TL_AON_GPIO_WAKEUP_D27:
            PowerSave.TL_AON_WAKEPIN_WAKEUP_D27();
            break;
        default:
            printf("Unknown wakeup source.    \r\n");
        break;
    }

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
    PowerSave.TL_sleep_callback(TL_Suspend_function, TL_Resume_fnction);
}

void loop() {
    delay(1000);
}
