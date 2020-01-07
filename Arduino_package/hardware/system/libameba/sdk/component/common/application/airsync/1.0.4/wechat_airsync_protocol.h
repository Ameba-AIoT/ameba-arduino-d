/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#ifndef _WECHAT_AIRSYNC_PROTOCOL_H_
#define _WECHAT_AIRSYNC_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "epb_MmBp.h"

/*define encryption method, only open one method at the same time*/
#define AirSync_macNoEncrypt
//#define AirSync_md5AndNoEnrypt
//#define AirSync_md5AndAesEnrypt

#define PROTO_VERSION 0x010004
#define AUTH_PROTO 1

#define DEVICE_TYPE "gh_3d15fea52c97"
#define DEVICE_ID "WeChat08"
#define DEVICE_KEY {0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF,0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF};

#ifdef AirSync_macNoEncrypt
#define AUTH_METHOD EAM_macNoEncrypt
#define MD5_TYPE_AND_ID_LENGTH 0
#define CIPHER_TEXT_LENGTH 0
#endif



#define AIRSYNC_MTU_SIZE        20
#define MAC_ADDRESS_LENGTH      6


#define HEADER_LENGTH           sizeof(AirSync_Header)
#define HEADER_MAGIC_NUMBER     0xFE
#define HEADER_VER              0x01
#define REQUEST_BUFFER_SIZE     0xFFF
#define RECEIVE_BUFFER_SIZE     0xFF

#define WECHAT_RESP             0x6f6b
#define AIRSYNC_TIME_SYNC       0x02
#define AIRSYNC_PEDO_SYNC       0x11


typedef struct
{
    uint16_t length;
    uint8_t *content;
    bool SendingORReceiving;
} PacketContent;

typedef struct BpFixHead
{
    uint8_t  bMagicNumber;
    uint8_t  bVer;
    uint16_t nLength;
    uint16_t nCmdId;
    uint16_t nSeq;
} AirSync_Header;




typedef struct
{
    bool wechats_switch_state;
    bool is_indicate_enable;
    bool is_connected;
    bool is_auth_pass;
    bool is_auth_sent;
    bool is_init_pass;
    bool is_init_sent;
    bool is_send_packet_done;
    bool is_view_background_switch;
    uint16_t sequence_id;
    uint16_t push_data_seq;
} AirSyncLinkState;

typedef enum
{
    errorCodeUnpackAuthResp = 0x9990,
    errorCodeUnpackInitResp = 0x9991,
    errorCodeUnpackSendDataResp = 0x9992,
    errorCodeUnpackCtlCmdResp = 0x9993,
    errorCodeUnpackRecvDataPush = 0x9994,
    errorCodeUnpackSwitchViewPush = 0x9995,
    errorCodeUnpackSwitchBackgroundPush = 0x9996,
    errorCodeUnpackErrorDecode = 0x9997,
} AirSyncUnpackErrorCode;

typedef enum
{
    AIRSYNC_TICK_SYNC = 0,
    AIRSYNC_SERVER_REPS
} AirsyncKeyIDType;
typedef enum
{
    AIRSYNC_SENDDATA_TEST_EVENT = 0,
    AIRSYNC_AUTH_EVENT,
    AIRSYNC_INIT_EVENT,
} AirsyncSendType;


void airsync_send_stream(uint8_t *pBuffer, uint16_t length);
void set_data_complete_callback(uint16_t length, uint8_t *current_pointer);
void airsync_receive_data(uint8_t *pBuffer, uint8_t length);
void airsync_send_data_event(uint8_t Event);
void airsync_auth_request(void);
void airsync_init_request(void);


#endif

#ifdef __cplusplus
}
#endif
