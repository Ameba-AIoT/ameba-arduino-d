#ifndef _BT_OTA_CENTRAL_CLIENT_H_
#define _BT_OTA_CENTRAL_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif


#define SILENT_OTA    1

#if (SILENT_OTA == 1)
#define ENABLE_SILENT_OTA_PRESS_TEST       0
#else
#define ENABLE_NORMAL_OTA_PRESS_TEST       0
#endif

/*
0:      filter by device addr in adv data and device name in scan response data;
1:      filter by device addr in adv data;
others: filter by device name in scan response data.
*/
#define DFU_MODE_FILTER_SCAN_INFO_STRATEGY      1

//#define TEMP_OTA_IMG_ADDR  0x00844000   //ota img load address

#endif