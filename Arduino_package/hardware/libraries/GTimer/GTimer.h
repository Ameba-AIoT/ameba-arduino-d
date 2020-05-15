#ifndef _GTIMER_H_
#define _GTIMER_H_

#include <inttypes.h>

/**
 * @class GTimerClass GTimer.h
 * @brief GTimer is a hardware timer and this class is to operate it
 *
 * @note The Gtimer occupy same resource as PWM. Please make sure the timer is not conflict with you pwm index.
 */
class GTimerClass {
public:
    /**
     * @brief Initialize a timer and start it immediately
     *
     * @param[in] timerid There are 5 valid GTimer with timer id 0~4.
     * @param[in] duration_us The duration of timer. The time unit is microsecond and the precision is 32768Hz
     * @param[in] handler As timer timeout, it would invoke this handler.
     * @param[in] periodical By default the timer would keep periodicall countdown and reload which meas the handler would periodicall invoked.
     * @param[in] userdata The user data brings to the handler
     */
    void begin(uint32_t timerid, uint32_t duration_us, void (*handler)(uint32_t), bool periodical = true, uint32_t userdata = 0);

    /**
     * @brief Stop a specific timer
     *
     * @param[in] timerid Stop the timer with this timer id
     */
    void stop(uint32_t timerid);

    /**
     * @brief Reload a specific timer
     *
     * The GTimer is a countdown timer. Reload it would make it discard the current countdown value and restart countdown based on the duration.
     *
     * @param[in] timerid The timer to be modified
     * @param[in] duration_us The updated duration in unit of microseconds.
     */
    void reload(uint32_t timerid, uint32_t duration_us);

    /**
     * @brief Read current countdown value
     *
     * @param[in] timerid The timer to be read
     * @return The current countdown value
     */
    uint64_t read_us(uint32_t timerid);
};

extern GTimerClass GTimer;

#endif
