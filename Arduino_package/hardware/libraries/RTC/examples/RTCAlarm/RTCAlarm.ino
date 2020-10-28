/**
 * This example demonstrate how to use the RTC library methods 
 * in order to do something when an alarm is matched. 
 * In particular in this example, the RTC time is set at 16:00:00 and an alarm at 16:00:10. 
 * When the time match using the match type MATCH_HHMMSS is reached, 
 * the attached interrupt function will print on the serial monitor Alarm Match!
*/
#include <stdio.h>
#include <time.h>

#include "rtc.h"

/* Change these values to set the current initial date */
#define YEAR 2020
#define MONTH 1
#define DAY 1
/* Change these values to set the current initial time */
#define HOUR 16
#define MIN 0
#define SEC 0
/* Change these values to set the alarm time */
#define ALARM_HOUR 16
#define ALARM_MIN 0
#define ALARM_SEC 10  // set alarm as 10 seconds

/* Create an rtc object */
RTC rtc;
alarm_t alarm;
int32_t seconds;
struct tm *timeinfo;

void setup() {
    Serial.begin(115200);
    rtc.Init();
    int initTime = rtc.SetEpoch(YEAR, MONTH, DAY, HOUR, MIN, SEC);  // convert user time to seconds from 1970.1.1 00:00:00
    rtc.Write(initTime);
    EnableAlarm(ALARM_HOUR, ALARM_MIN, ALARM_SEC);
}

void loop() {
    seconds = rtc.Read();
    timeinfo = localtime(&seconds);
    printStringTime();
    rtc.Wait(1);
}

static void EnableAlarm(int hour, int min, int sec) {
    alarm.yday = 0;  //  deafault alarmDay to be zero
    alarm.hour = hour;
    alarm.min = min;
    alarm.sec = sec;
    rtc_set_alarm(&alarm, (alarm_irq_handler)rtc_handler);
}

void DisableAlarm(void) {
    rtc_disable_alarm();
    Serial.println("Alarm disabled");
}

void rtc_handler(void) {
    Serial.println("====   Alarm Matched   ====");
    while (1)
        ;
}

void printStringTime(void) {
    timeinfo = localtime(&seconds);
    Serial.print("Time:       ");
    Serial.print(timeinfo->tm_year + 1900);
    Serial.print("-");
    Serial.print(timeinfo->tm_mon + 1);
    Serial.print("-");
    Serial.print(timeinfo->tm_mday);
    Serial.print(" ");
    Serial.print(timeinfo->tm_hour);
    Serial.print(":");
    Serial.print(timeinfo->tm_min);
    Serial.print(":");
    Serial.println(timeinfo->tm_sec);
}
