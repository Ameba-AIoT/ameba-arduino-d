#ifndef _BLE_CENTRAL_AT_CMD_H_
#define _BLE_CENTRAL_AT_CMD_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "platform_opts_bt.h"

int ble_central_app_handle_at_cmd(uint16_t subtype, void *arg);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

