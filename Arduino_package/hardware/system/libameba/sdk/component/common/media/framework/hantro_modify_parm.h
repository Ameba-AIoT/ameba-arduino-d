#ifndef HANTRO_MODIFY_PARM_H
#define HANTRO_MODIFY_PARM_H

#define CMD_VIDEO_H264_CHG_FPS	        0x01
#define CMD_VIDEO_H264_CHG_RESOLUTION	0x02
#define CMD_VIDEO_H264_CHG_BPS		0x03
#define CMD_VIDEO_H264_CHG_IFRAME       0x04

struct mmf_video_h264_parm{
        int cmd_code;
        int config_flag;
	int fps;
	int width;
	int height;
        int bps;
};

struct mmf_video_modify_parm_context{
	struct mmf_video_h264_parm h264_parm[3];
};

void mmf_video_modify_parm_init(void);

void mmf_video_isp_reset_buffer(int streamid);

void mmf_video_h264_change_parm_cb(void *ctx);

int mmf_video_h264_change_fps(int streamid,int fps);

int mmf_video_h264_change_resolution(int streamid,int width,int height);

int mmf_video_h264_change_bps(int streamid,int bps);

int mmf_video_h264_force_iframe(int streamid);

void mmf_video_nv12_change_parm_cb(void *ctx);

#endif
