#ifndef __RTC_H__
#define __RTC_H__

#include <Arduino.h>

#include "rtc_api.h"
#include "timer_api.h"

class RTC {
    public:
        RTC(void);
        ~RTC(void);
        void Init(void);
        void DeInit(void);
        void Write(long t);
        int32_t Read(void);
        void Wait(float s);
        int SetEpoch(int year, int month, int day, int hour, int min, int sec);

    private:

};
#endif
