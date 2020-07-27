QC_TEST_EXAMPLE_VIDEO_SCENARIO_1

Description:
This example demostrates Streaming and SD card Storage + two-way audio (format:AAC) with AEC (echo cancellation

ISP   -> H264 -> MP4 (V1)  (SD card storage)
ISP   -> H264 -> RTSP (V2)
AUDIO -> AAC  -> RTSP and MP4 
RTP   -> AAD  -> AUDIO


Configuration:
[platform_opts.h]
	#define CONFIG_EXAMPLE_MEDIA_FRAMEWORK		1
[example_media_framework.c]
	(SDK 5.2d) Line 133:
		Remove comment to use function: mmf2_example_joint_test_rtsp_mp4_init();
[atcmd_media.c]
	Replace function void fATMt(void *arg) with contents in atcmd_media_QC.c
[mmf2_example_joint_test_rtsp_mp4_init.c]
	(SDK 5.2d) Line 399:
		Comment mm_module_ctrl(mp4_ctx, CMD_MP4_START, mp4_v1_params.record_file_num);
		(Otherwise, it will start recording MP4 automatically after reboot.)
[example_media_framework.h]
	Set video parameters, V1 for storage and V2 for streaming
[mmf2_example_param.c]
	Set MP4 parameters (mp4_v1_params)
	
Execution:
	(1) Connect to AP
	(2) The MP4 (SD card storage) long-run mode is default enabled
		If number of recording files exceeds V2_RECORD_FILENUM, it would overwrite the old files.
		(Reset file_index to 0 and remove the old file with same name)
	(3) Enable streaming by VLC player (AmebaPro V1+A -> VLC player)
		While the log message “RTSP stream enabled” is shown in the log console,
		please open the VLC media player,
		click "Media->Open Network Stream" option in menu bar,
		and type in: "rtsp://xxx.xxx.xxx.xxx:yyy/test.sdp" before clicking play button. 
		xxx.xxx.xxx.xxx: the Ameba IP address. 
		yyy: RTSP server port number (default is 554). 
		Also, please select ‘Show more options’ to adjust the network-cache (ex: 200ms)
	(4) Streaming another direction's audio by VLC player ( VLC player Audio File -> AmebaPro Audio Output)
		please open the VLC media player,
		click "Media->Stream" option in menu bar,
		click "Add" and select an .aac file (8K sample rate, MONO, AAC) and click "Next".
		set the "New destination" to be "RTP Audio/Video Profile" and click "Add".
		type in Address: "xxx.xxx.xxx.xxx" and Base port "16384" and click "Next"
		please UNCHECK "Active Transcoding" and click "Next"

[Supported List]
	Supported :
		Ameba-pro