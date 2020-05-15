/*
 * Realtek Semiconductor Corp.
 *
 * rts_camera_soc.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _UAPI_RTS_CAMERA_SOC_H
#define _UAPI_RTS_CAMERA_SOC_H

enum rtscam_soc_cmd_error_type
{
	RTSCAM_CMD_STS_CMD_ERR = 0x01,
	RTSCAM_CMD_STS_LEN_ERR = 0x02,
	RTSCAM_CMD_STS_ADDR_ERR = 0x03
};

struct rtscam_soc_cmd_stru
{
	uint16_t cmdcode;
	uint8_t index;
	uint8_t length;
	uint16_t param;
	uint16_t addr;
	uint8_t *buf;
	uint32_t error_type;
};

struct rtscam_soc_mtd_state
{
	int index;
	int state;
};

struct rtscam_soc_ldc_stru
{
	unsigned int length;
	uint8_t *ptable;
};

struct rtscam_soc_fw
{
	unsigned int length;
	uint8_t *fw;
};

#define RTSOCIOC_ISPCMD			0x31
#define RTSOCIOC_G_HWOFFSET		0x34
#define RTSOCIOC_G_HWIOSIZE		0x35
#define RTSOCIOC_G_MTDSTATE		0x36
#define RTSOCIOC_G_STREAMID		0x37

#define RTSOCIOC_S_TD			0x3a
#define RTSOCIOC_G_TD			0x3b
#define RTSOCIOC_ENABLE_LDC		0x3c
#define RTSOCIOC_DISABLE_LDC		0x3d
#define RTSOCIOC_G_LDC			0x3e

#define RTSOCIOC_CAMERA_DETACH		0x40
#define RTSOCIOC_CAMERA_ATTACH		0x41
#define RTSOCIOC_CAMERA_LOAD_FW		0x42
#define RTSOCIOC_CAMERA_DUMP_FW		0x43
#define RTSOCIOC_CAMERA_G_FW_SIZE	0x44

#endif
