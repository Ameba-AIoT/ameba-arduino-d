/*
 This sketch shows how to use power save tickless mode

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ticklessmode/
 */

#include <PowerSave.h>

//SET_TL_UART_WAKEUP
//SET_TL_RTC_WAKEUP
//For AMB21/AMB22 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA25      // pin 16
    //SET_AON_GPIO_WAKEUP_PA26      // pin 17
    //SET_AON_GPIO_WAKEUP_PA21      // pin 26
    //SET_AON_GPIO_WAKEUP_PA20      // pin 27
    //SET_AON_GPIO_WAKEUP_PA19      // pin 28
//For AMB23 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA12      // pin 9
    //SET_AON_GPIO_WAKEUP_PA13      // pin 10
    //SET_AON_GPIO_WAKEUP_PA14      // pin 11
    //SET_AON_GPIO_WAKEUP_PA15      // pin 12
    //SET_AON_GPIO_WAKEUP_PA16      // pin 13
    //SET_AON_GPIO_WAKEUP_PA18      // pin 15
    //SET_AON_GPIO_WAKEUP_PA19      // pin 16
    //SET_AON_GPIO_WAKEUP_PA21      // pin 18
//For BW16/BW16-TypeC only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA25      // pin 7
    //SET_AON_GPIO_WAKEUP_PA26      // pin 8
    //SET_AON_GPIO_WAKEUP_PA15      // pin 9
    //SET_AON_GPIO_WAKEUP_PA14      // pin 10
    //SET_AON_GPIO_WAKEUP_PA13      // pin 11
    //SET_AON_GPIO_WAKEUP_PA12      // pin 12
//For board AW-CU488_ThingPlus
    //SET_AON_GPIO_WAKEUP_PA18      // pin 0
    //SET_AON_GPIO_WAKEUP_PA16      // pin 1
    //SET_AON_GPIO_WAKEUP_PA17      // pin 2
    //SET_AON_GPIO_WAKEUP_PA13      // pin 3
    //SET_AON_GPIO_WAKEUP_PA12      // pin 4
    //SET_AON_GPIO_WAKEUP_PA26      // pin 5
    //SET_AON_GPIO_WAKEUP_PA25      // pin 6
    //SET_AON_GPIO_WAKEUP_PA19      // pin 8
    //SET_AON_GPIO_WAKEUP_PA15      // pin 28
    //SET_AON_GPIO_WAKEUP_PA14      // pin 29
//For board AMB25/AMB26
    //SET_AON_GPIO_WAKEUP_PA12      // pin 3
    //SET_AON_GPIO_WAKEUP_PA13      // pin 2
    //SET_AON_GPIO_WAKEUP_PA14      // pin 1
    //SET_AON_GPIO_WAKEUP_PA15      // pin 0
    //SET_AON_GPIO_WAKEUP_PA25      // pin 7
    //SET_AON_GPIO_WAKEUP_PA26      // pin 6

#define TL_WAKEUP_SOURCE            SET_TL_UART_WAKEUP

#define TL_SYSACTIVE_TIME           5000

#define TL_RTC_ALARM_DAY            0
#define TL_RTC_ALARM_HOUR           0
#define TL_RTC_ALARM_MIN            0
#define TL_RTC_ALARM_SEC            10

uint32_t TL_Suspend_function(void) {
    switch (TL_WAKEUP_SOURCE) {
        case SET_TL_UART_WAKEUP:
            PowerSave.TL_UART_WAKEUP();
            break;
        case SET_TL_RTC_WAKEUP:
            PowerSave.TL_RTC_WAKEUP();
            PowerSave.RTCWakeSetup(TL_RTC_ALARM_DAY, TL_RTC_ALARM_HOUR, TL_RTC_ALARM_MIN, TL_RTC_ALARM_SEC);
            break;
        case SET_AON_GPIO_WAKEUP_PA12:
            PowerSave.AON_WAKEPIN_WAKEUP_PA12();
            break;
        case SET_AON_GPIO_WAKEUP_PA13:
            PowerSave.AON_WAKEPIN_WAKEUP_PA13();
            break;
        case SET_AON_GPIO_WAKEUP_PA14:
            PowerSave.AON_WAKEPIN_WAKEUP_PA14();
            break;
        case SET_AON_GPIO_WAKEUP_PA15:
            PowerSave.AON_WAKEPIN_WAKEUP_PA15();
            break;
        case SET_AON_GPIO_WAKEUP_PA16:
            PowerSave.AON_WAKEPIN_WAKEUP_PA16();
            break;
        case SET_AON_GPIO_WAKEUP_PA17:
            PowerSave.AON_WAKEPIN_WAKEUP_PA17();
            break;
        case SET_AON_GPIO_WAKEUP_PA18:
            PowerSave.AON_WAKEPIN_WAKEUP_PA18();
            break;
        case SET_AON_GPIO_WAKEUP_PA19:
            PowerSave.AON_WAKEPIN_WAKEUP_PA19();
            break;
        case SET_AON_GPIO_WAKEUP_PA20:
            PowerSave.AON_WAKEPIN_WAKEUP_PA20();
            break;
        case SET_AON_GPIO_WAKEUP_PA21:
            PowerSave.AON_WAKEPIN_WAKEUP_PA21();
            break;
        case SET_AON_GPIO_WAKEUP_PA25:
            PowerSave.AON_WAKEPIN_WAKEUP_PA25();
            break;
        case SET_AON_GPIO_WAKEUP_PA26:
            PowerSave.AON_WAKEPIN_WAKEUP_PA26();
            break;
        default:
            printf("Unknown wakeup source.    \r\n");
        break;
    }

    printf("\r\nSystem suspend. Tickless mode enabled.    \r\n");
    return TRUE;
}

uint32_t TL_Resume_fnction(void) {
    printf("\r\nTickless mode resumed. Wait %ds enter tickless again.\r\n", (TL_SYSACTIVE_TIME/1000));

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
