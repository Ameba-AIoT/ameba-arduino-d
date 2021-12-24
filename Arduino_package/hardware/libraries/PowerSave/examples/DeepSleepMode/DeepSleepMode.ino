/*
  This sketch shows how to use power save deepsleep mode
*/
#include <PowerSave.h>

//SET_DS_AON_TIMER_WAKEUP
//SET_DS_RTC_WAKEUP
//For RTL8722DM only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA25    // D16
    //SET_AON_GPIO_WAKEUP_GPIOA26    // D17
    //SET_AON_GPIO_WAKEUP_GPIOA21    // D26
    //SET_AON_GPIO_WAKEUP_GPIOA20    // D27
    //SET_AON_GPIO_WAKEUP_GPIOA19    // D28
//For RTL8722DM_MINI only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA12    // D9
    //SET_AON_GPIO_WAKEUP_GPIOA13    // D10
    //SET_AON_GPIO_WAKEUP_GPIOA14    // D11
    //SET_AON_GPIO_WAKEUP_GPIOA15    // D12
    //SET_AON_GPIO_WAKEUP_GPIOA16    // D13
    //SET_AON_GPIO_WAKEUP_GPIOA18    // D15
    //SET_AON_GPIO_WAKEUP_GPIOA19    // D16
    //SET_AON_GPIO_WAKEUP_GPIOA21    // D18
//For RTL8720DN_BW16 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_GPIOA25    // D7
    //SET_AON_GPIO_WAKEUP_GPIOA26    // D8
    //SET_AON_GPIO_WAKEUP_GPIOA15    // D9
    //SET_AON_GPIO_WAKEUP_GPIOA14    // D10
    //SET_AON_GPIO_WAKEUP_GPIOA13    // D11
    //SET_AON_GPIO_WAKEUP_GPIOA12    // D12

#define DS_WAKEUP_SOURCE                SET_DS_AON_TIMER_WAKEUP

#define AON_TIMER_SLEEP_DURATION        5000
#define DS_RTC_ALARM_DAY                0
#define DS_RTC_ALARM_HOUR               0
#define DS_RTC_ALARM_MIN                0
#define DS_RTC_ALARM_SEC                10

void DeepSleep_wakeup(void) {
    printf("\r\nDeep sleep wakeuped! \r\n");
    uint32_t wakereason_number = PowerSave.AONWakeReason();

    if (wakereason_number == AONWakeReason_AON_GPIO) {
        printf("AonWakepin wakeup. Wait 5s sleep again.    \r\n");
        delay(5000);
    } else if (wakereason_number == AONWakeReason_AON_TIMER) {
        PowerSave.AONTimerCmd();
        printf("AonTimer wakeup. Wait 5s sleep again.    \r\n");
        delay(5000);
    } else if (wakereason_number == AONWakeReason_RTC) {
        printf("RTC wakeup. Wait 5s sleep again.    \r\n");
        delay(5000);
    }

    PowerSave.AONWakeClear();
}

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    PowerSave.begin(DEEPSLEEP_MODE);

    if (TRUE == (PowerSave.DsleepWakeStatusGet())) {
        DeepSleep_wakeup();
    }

    PowerSave.AONTimerDuration(0);

    switch (DS_WAKEUP_SOURCE) {
        case SET_DS_AON_TIMER_WAKEUP:
            PowerSave.DS_AON_TIMER_WAKEUP();
            PowerSave.AONTimerDuration(AON_TIMER_SLEEP_DURATION);
            break;
        case SET_DS_RTC_WAKEUP:
            PowerSave.DS_RTC_WAKEUP();
            PowerSave.RTCWakeSetup(DS_RTC_ALARM_DAY, DS_RTC_ALARM_HOUR, DS_RTC_ALARM_MIN, DS_RTC_ALARM_SEC);
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

    PowerSave.enable();
}

void loop() {
    delay(1000);
}
