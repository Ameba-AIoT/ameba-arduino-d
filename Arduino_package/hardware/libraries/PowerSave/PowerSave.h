#ifndef _POWERSAVE_H_
#define _POWERSAVE_H_

#include <inttypes.h>

#if defined(BOARD_RTL8720DN_BW16)
#error TBD for RTL8720DN/BW16
#elif defined(BOARD_RTL8722DM_MINI)
#error TBD for RTL8722DM_MINI
#endif

#define DEEPSLEEP_MODE                  11
#define SET_DS_AON_TIMER_WAKEUP         0
#define SET_DS_RTC_WAKEUP               2
#define SET_DS_AON_GPIO_WAKEUP_D16      16
#define SET_DS_AON_GPIO_WAKEUP_D17      17
#define SET_DS_AON_GPIO_WAKEUP_D26      26
#define SET_DS_AON_GPIO_WAKEUP_D27      27

#define TICKLESS_MODE                   22
#define SET_TL_UART_WAKEUP              0
#define SET_TL_RTC_WAKEUP               1
#define SET_TL_AON_GPIO_WAKEUP_D16      16
#define SET_TL_AON_GPIO_WAKEUP_D17      17
#define SET_TL_AON_GPIO_WAKEUP_D26      26
#define SET_TL_AON_GPIO_WAKEUP_D27      27

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
    void DS_AON_WAKEPIN_WAKEUP_D16(void);
    void DS_AON_WAKEPIN_WAKEUP_D17(void);
    void DS_AON_WAKEPIN_WAKEUP_D26(void);
    void DS_AON_WAKEPIN_WAKEUP_D27(void);

    void TL_UART_WAKEUP(void);
    void TL_RTC_WAKEUP(void);
    void TL_AON_WAKEPIN_WAKEUP_D16(void);
    void TL_AON_WAKEPIN_WAKEUP_D17(void);
    void TL_AON_WAKEPIN_WAKEUP_D26(void);
    void TL_AON_WAKEPIN_WAKEUP_D27(void);
};

extern PMUClass PowerSave;

#endif
