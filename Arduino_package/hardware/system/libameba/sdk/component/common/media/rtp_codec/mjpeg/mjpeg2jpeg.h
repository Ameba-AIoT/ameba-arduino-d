#ifndef MJPEG_2_JPEG_H
#define MJPEG_2_JPEG_H
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#endif
#include "osdep_api.h"
#include "basic_types.h"

typedef struct AVFrames
{
	char *FrameData;
	int FrameLength;
}AVFrame;

int mjpeg2jfif(AVFrame *in_frame);


#endif