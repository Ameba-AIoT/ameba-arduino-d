#ifndef _CLOUD_LED_H_
#define _CLOUD_LED_H_

#include <stm322xg_eval.h>

#define READ_WRITE_PATTERN		0
#define AP_MODE_PATTERN			1
#define NOT_SETUP_PATTERN		2

extern void set_led_pattern(uint8_t pattern, uint8_t pattern_continued);
extern void set_led_value(uint8_t value);

#endif
