#ifndef JPEG_SNAPSHOT_H
#define JPEG_SNAPSHOT_H

#include "video_common_api.h"

void jpeg_snapshot_initial_with_instance(void* jpeg_ctx, uint32_t dest_addr, uint32_t dest_len, uint8_t snapshot_during_jpeg_streaming);
void jpeg_snapshot_initial(uint32_t width, uint32_t height, uint32_t fps, uint32_t level, uint32_t dest_addr, uint32_t dest_len);
int jpeg_snapshot_encode_cb(uint32_t y_addr,uint32_t uv_addr);
void jpeg_snapshot_cb(void);

int jpeg_snapshot_isp_config(int streamid);
int jpeg_snapshot_get_buffer(VIDEO_BUFFER* video_buf, uint32_t timeout_ms);
void jpeg_snapshot_set_processing(uint8_t snapshot_processing);
uint8_t jpeg_snapshot_get_processing();

// For AT command
void jpeg_snapshot_stream();
int jpeg_snapshot_isp();
int jpeg_snapshot_isp_change_resolution(int width,int height);
void jpeg_snapshot_isp_callback(int arg);

int yuv_snapshot_isp_config(int width,int height,int fps,int streamid);
int yuv_snapshot_isp(unsigned char **raw_y);
int yuv_snapshot_isp_deinit();

#endif