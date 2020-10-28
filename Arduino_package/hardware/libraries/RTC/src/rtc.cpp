#include "rtc.h"

#include <time.h>

#include "rtl8721d_rtc.h"
#include "wait_api.h"

static struct tm rtc_timeinfo;
static int rtc_en = 0;
static alarm_irq_handler rtc_alarm_handler;


RTC::RTC(){};
RTC::~RTC(){};

/**
  * @brief  Initializes the RTC device, include clock, RTC registers and function.
  * @param  none
  * @retval  none
  */
void RTC::Init(void) {
    RTC_InitTypeDef RTC_InitStruct;
    //  1. Initialize the RTC Clock Source.
    RCC_PeriphClockSource_RTC(0);
    //  2. Fill the variable RTC_InitStruct with default parameters, or set the desired parameters manually:
    RTC_StructInit(&RTC_InitStruct);
    RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
    // 3. configure the RTC peripheral with the corresponding configurations
    RTC_Init(&RTC_InitStruct);
    RTC_BypassShadowCmd(ENABLE);
    rtc_en = 1;
}

/**
  * @brief  Deinitializes the RTC device.
  * @param  none
  * @retval  none
  */
void RTC::DeInit(void) {
    rtc_en = 0;
    rtc_alarm_handler = NULL;
}

/**
  * @brief  Set the specified timestamp in seconds to RTC.
  * @param  t: Seconds from 1970.1.1 00:00:00 to specified data and time
  *              which is to be set.
  * @retval  none
  */
void RTC::Write(long t) {
    /* Convert the time in to a tm*/
    struct tm *timeinfo = localtime(&t);

    RTC_TimeTypeDef RTC_TimeStruct;

    /*set time in RTC */
    RTC_TimeStruct.RTC_H12_PMAM = RTC_H12_AM;
    RTC_TimeStruct.RTC_Days = timeinfo->tm_yday;
    RTC_TimeStruct.RTC_Hours = timeinfo->tm_hour;
    RTC_TimeStruct.RTC_Minutes = timeinfo->tm_min;
    RTC_TimeStruct.RTC_Seconds = timeinfo->tm_sec;

    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

    /* Set rtc_timeinfo*/
    _memcpy((void *)&rtc_timeinfo, (void *)timeinfo, sizeof(struct tm));
}

/**
  * @brief  Get current timestamp in seconds from RTC.
  * @param  none
  * @retval value: The current timestamp in seconds which is calculated from 
  *              1970.1.1 00:00:00.
  */
int32_t RTC::Read(void) {
    return (rtc_read() - 508055296);
}

/**
  * @brief  Wait for 1000000 us / 1s.
  */
void RTC::Wait(float s) {
    DelayUs((int)(s * 1000000.0f));
}

/**
 * @brief: convert human readable time to epoch time
*/
int RTC::SetEpoch(int year, int month, int day, int hour, int min, int sec) {
    struct tm t;
    time_t t_of_day;

    t.tm_year = year - 1900;  // Year - 1970
    t.tm_mon = month - 1;     // Month, where 0 = jan
    t.tm_mday = day;          // Day of the month
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    t.tm_isdst = -1;  // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    // printf("seconds since the Epoch: %d\n", (long)t_of_day);
    return t_of_day;
}
