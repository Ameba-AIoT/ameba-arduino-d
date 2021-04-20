#ifndef _BLE_PERIPHERAL_AT_CMD_H_
#define _BLE_PERIPHERAL_AT_CMD_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

int ble_peripheral_at_cmd_set_adv_int(int argc, char **argv);
int ble_peripheral_app_handle_at_cmd(uint16_t subtype, void *arg);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
