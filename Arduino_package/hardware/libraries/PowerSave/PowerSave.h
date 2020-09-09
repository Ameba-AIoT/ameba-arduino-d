#ifndef _POWERSAVE_H_
#define _POWERSAVE_H_

#include <inttypes.h>

extern "C" {
#include "ameba_soc.h"

}
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
};

extern PMUClass PowerSave;

#endif
