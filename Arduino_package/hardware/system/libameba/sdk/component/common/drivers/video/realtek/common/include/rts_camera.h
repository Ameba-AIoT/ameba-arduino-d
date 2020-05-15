/*
 * Realtek Semiconductor Corp.
 *
 * rts_camera.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _LINUX_RTS_CAMERA_H
#define _LINUX_RTS_CAMERA_H

//#include <linux/ioctl.h>
//#include <linux/types.h>

#define RTSCAM_DEV_MAJOR		82

#define RTS_V4L2_CID_BASE		(V4L2_CID_USER_BASE | 0xf000)
#define RTS_V4L2_CID_ROLL_ABSOLUTE	(RTS_V4L2_CID_BASE + 0)
#define RTS_V4L2_CID_AUTO_WHITE_BALANCE_COMPONENT	(RTS_V4L2_CID_BASE + 1)
#define RTS_V4L2_CID_GREEN_BALANCE	(RTS_V4L2_CID_BASE + 2)
#define RTS_V4L2_CID_AE_GAIN		(RTS_V4L2_CID_BASE + 3)

struct rtscam_vcmd {
	unsigned int cmdcode;
	void *arg;
};

#define RTSCAMIOC_VENDOR_CMD		0x01
#define RTSCAMIOC_GET_PHYADDDR		0x02

#endif
