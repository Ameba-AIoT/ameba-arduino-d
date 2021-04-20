#ifndef  _HRP_GAP_BLE_API_H_
#define  _HRP_GAP_BLE_API_H_

#include <string.h>
#include <stdint.h>
#include <hrp.h>

void hrp_gap_ble_handle_adv_0000_perf_config_req(uint16_t len, uint8_t *p_param_list);
void hrp_gap_ble_handle_adv_0000_perf_start_req(uint16_t len, uint8_t *p_param_list);


#endif
