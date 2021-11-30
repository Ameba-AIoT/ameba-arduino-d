/*
  This sketch shows how to use power save tickless mode
*/
#include <PowerSave.h>

//SET_TL_UART_WAKEUP
//SET_TL_RTC_WAKEUP
//For RTL8722DM only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA25       //D16
    //SET_AON_GPIO_WAKEUP_GPIOA26       //D17
    //SET_AON_GPIO_WAKEUP_GPIOA21       //D26
    //SET_AON_GPIO_WAKEUP_GPIOA20       //D27
    //SET_AON_GPIO_WAKEUP_GPIOA19       //D28
//For RTL8722DM_MINI only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA12       //D9
    //SET_AON_GPIO_WAKEUP_GPIOA13       //D10
    //SET_AON_GPIO_WAKEUP_GPIOA14       //D11
    //SET_AON_GPIO_WAKEUP_GPIOA15       //D12
    //SET_AON_GPIO_WAKEUP_GPIOA16       //D13
    //SET_AON_GPIO_WAKEUP_GPIOA18       //D15
    //SET_AON_GPIO_WAKEUP_GPIOA19       //D16
    //SET_AON_GPIO_WAKEUP_GPIOA21       //D18
//For RTL8720DN_BW16 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA25       //D7
    //SET_AON_GPIO_WAKEUP_GPIOA26       //D8
    //SET_AON_GPIO_WAKEUP_GPIOA15       //D9
    //SET_AON_GPIO_WAKEUP_GPIOA14       //D10
    //SET_AON_GPIO_WAKEUP_GPIOA13       //D11
    //SET_AON_GPIO_WAKEUP_GPIOA12       //D12

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
        case SET_AON_GPIO_WAKEUP_GPIOA12:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA12();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA13:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA13();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA14:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA14();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA15:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA15();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA16:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA16();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA17:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA17();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA18:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA18();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA19:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA19();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA20:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA20();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA21:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA21();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA25:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA25();
            break;
        case SET_AON_GPIO_WAKEUP_GPIOA26:
            PowerSave.AON_WAKEPIN_WAKEUP_GPIOA26();
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
