#ifndef _VIDEO_COMMON_API_H_
#define _VIDEO_COMMON_API_H_

#include <stdint.h>

#define VIDEO_240P_WIDTH	320
#define VIDEO_240P_HEIGHT	240
#define VIDEO_480P_WIDTH	640
#define VIDEO_480P_HEIGHT	480
#define VIDEO_720P_WIDTH	1280
#define VIDEO_720P_HEIGHT	720
#define VIDEO_1080P_WIDTH	1920
#define VIDEO_1080P_HEIGHT	1080

typedef struct {
	uint8_t* output_buffer;
	uint32_t output_buffer_size;
	uint32_t output_size;
} VIDEO_BUFFER;

void gpio_ircut_switch(void);

#endif //#ifndef _VIDEO_COMMON_API_H_