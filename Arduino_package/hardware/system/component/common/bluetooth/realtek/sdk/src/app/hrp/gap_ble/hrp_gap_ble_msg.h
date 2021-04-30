
#ifndef  _HRP_GAP_BLE_MSG_H_
#define  _HRP_GAP_BLE_MSG_H_

#include <string.h>
//#include <hrp_module_id.h>


#include "hrp.h"
#include <trace_app.h>
#include "hrp_gap_ble_api.h"
//#include <hrp_module_id.h>
#include "hrp_gap_ble_cmd_table.h"

void hrp_gap_ble_handle_adv_0000_perf_config_rsp(uint16_t len, uint8_t *p_param_list);
void hrp_gap_ble_handle_adv_0000_perf_start_rsp(uint16_t len, uint8_t *p_param_list);
void hrp_gap_ble_handle_adv_0000_perf_cmpl_info(uint16_t len, uint8_t *p_param_list);



#endif
