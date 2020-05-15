/*
 * Realtek Semiconductor Corp.
 *
 * include/rtsfw.h
 *
 * Copyright (C) 2016      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INCLUDE_RTSFW_H
#define _INCLUDE_RTSFW_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * rts_upload_fw
 *
 * length : fw length
 *
 * fw : point to fw data
 *
 * @return 0 : success, minus : fail
 * */
int rts_upload_fw(unsigned int length, uint8_t *fw);

/*!
 * rts_get_fw_size
 *
 * @return positive : size, minus : fail
 * */
int rts_get_fw_size();

/*!
 * rts_dump_fw
 *
 * length : fw length
 *
 * fw : point to fw data
 *
 * @return 0 : success, minus : fail
 * */
int rts_dump_fw(unsigned int length, uint8_t *fw);

/*!
 * rts_load_fw
 *
 * @return 0
 * */
int rts_load_fw();

#ifdef __cplusplus
}
#endif
#endif
