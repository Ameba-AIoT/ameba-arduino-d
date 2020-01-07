//  epb_WristBand.h
//  MicroMessenger
//
//  Created by harlliu@tencent.com on 14-04-24.
//  Copyright 2014 Tencent. All rights reserved.
//

//  Version : 1.0.2

#ifndef __EPB_WRISTBAND_H__
#define __EPB_WRISTBAND_H__


#include "epb.h"

typedef enum
{
    MPT_wristband_proto = 1
} MpProtoType;

typedef struct
{
    uint32_t step;
    bool has_timestamp;
    uint32_t timestamp;
    bool has_rtc_year;
    uint32_t rtc_year;
    bool has_rtc_month;
    uint32_t rtc_month;
    bool has_rtc_day;
    uint32_t rtc_day;
    bool has_rtc_hour;
    uint32_t rtc_hour;
    bool has_rtc_minute;
    uint32_t rtc_minute;
    bool has_rtc_second;
    uint32_t rtc_second;
} MMOpen_StepDataItem;

typedef struct
{
    int step_data_count;
    MMOpen_StepDataItem *step_data;
    bool has_ext_data;
    Bytes ext_data;
} MMOpen_WristbandRequest;

typedef struct
{
    bool has_err_code;
    int err_code;
    bool has_err_msg;
    CString err_msg;
} MMOpen_BaseResponse;

typedef struct
{
    MMOpen_BaseResponse base_response;
} MMOpen_WristbandResponse;

int epb_mmopen_wristband_request_pack_size(MMOpen_WristbandRequest *request);
int epb_mmopen_pack_wristband_request(MMOpen_WristbandRequest *request, uint8_t *buf, int buf_len);

MMOpen_WristbandResponse *epb_mmopen_unpack_wristband_response(const uint8_t *buf, int buf_len);

#endif
