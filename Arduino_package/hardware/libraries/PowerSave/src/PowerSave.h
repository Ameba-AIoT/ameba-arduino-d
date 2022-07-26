#ifndef _POWERSAVE_H_
#define _POWERSAVE_H_

#include <inttypes.h>

#define SET_AON_GPIO_WAKEUP_PA12      1UL<<12
#define SET_AON_GPIO_WAKEUP_PA13      1UL<<13
#define SET_AON_GPIO_WAKEUP_PA14      1UL<<14
#define SET_AON_GPIO_WAKEUP_PA15      1UL<<15
#define SET_AON_GPIO_WAKEUP_PA16      1UL<<16
#define SET_AON_GPIO_WAKEUP_PA17      1UL<<17
#define SET_AON_GPIO_WAKEUP_PA18      1UL<<18
#define SET_AON_GPIO_WAKEUP_PA19      1UL<<19
#define SET_AON_GPIO_WAKEUP_PA20      1UL<<20
#define SET_AON_GPIO_WAKEUP_PA21      1UL<<21
#define SET_AON_GPIO_WAKEUP_PA25      1UL<<25
#define SET_AON_GPIO_WAKEUP_PA26      1UL<<26

#define DEEPSLEEP_MODE                  11
#define SET_DS_AON_TIMER_WAKEUP         0
#define SET_DS_RTC_WAKEUP               1

#define TICKLESS_MODE                   22
#define SET_TL_UART_WAKEUP              0
#define SET_TL_RTC_WAKEUP               1

#define AONWakeReason_AON_TIMER         1111
#define AONWakeReason_RTC               2222
#define AONWakeReason_AON_GPIO          3333
/**
 * @class PMUClass PowerSave.h
 * @brief PowerSave is a power management unit for system to apply low energy performance 
 *
 * @note 
 */
class PMUClass {

public:
    /**
     * @brief Initialize a powersave class
     *
     * @param[in] sleep_mode 
     */
    PMUClass(void);

    void begin(uint32_t sleep_mode);

    void AONTimerDuration(uint32_t duration_ms);

    void AONTimerCmd(void);

    void RTCWakeSetup(uint32_t duration_d, uint32_t duration_h, uint32_t duration_m, uint32_t duration_s);

    void enable(void);

    uint32_t AONWakeReason(void);

    int WakePinCheck(void);

    void AONWakeClear(void);

    bool DsleepWakeStatusGet(void);

    void TL_sysactive_time(uint32_t duration_ms);

    void TL_wakelock(uint32_t select_lock);

    void TL_sleep_callback(uint32_t suspend(void), uint32_t resume(void));

    void DS_AON_TIMER_WAKEUP(void);
    void DS_RTC_WAKEUP(void);

    void TL_UART_WAKEUP(void);
    void TL_RTC_WAKEUP(void);

    void AON_WAKEPIN_WAKEUP_PA12(void);
    void AON_WAKEPIN_WAKEUP_PA13(void);
    void AON_WAKEPIN_WAKEUP_PA14(void);
    void AON_WAKEPIN_WAKEUP_PA15(void);
    void AON_WAKEPIN_WAKEUP_PA16(void);
    void AON_WAKEPIN_WAKEUP_PA17(void);
    void AON_WAKEPIN_WAKEUP_PA18(void);
    void AON_WAKEPIN_WAKEUP_PA19(void);
    void AON_WAKEPIN_WAKEUP_PA20(void);
    void AON_WAKEPIN_WAKEUP_PA21(void);
    void AON_WAKEPIN_WAKEUP_PA25(void);
    void AON_WAKEPIN_WAKEUP_PA26(void);
};

extern PMUClass PowerSave;

#endif
