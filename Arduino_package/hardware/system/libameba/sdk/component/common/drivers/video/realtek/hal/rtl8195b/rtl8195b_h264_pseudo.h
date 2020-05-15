#ifndef _RTL8195B_H264_PSEUDO_H_
#define _RTL8195B_H264_PSEUDO_H_

#define H264_PSEUDO_FRAME_RATE 30

void h264_pseudo_init();
void h264_pseudo_set_isr(h264_enc_handle isr, void *data);

void h264_pseudo_get_frame(uint8_t **frame, uint32_t *frame_len);

#endif