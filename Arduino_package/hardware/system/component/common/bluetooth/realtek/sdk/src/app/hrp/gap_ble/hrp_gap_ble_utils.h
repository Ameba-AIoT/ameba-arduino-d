#ifndef     _HRP_GAP_BLE_UTILS_H_
#define     _HRP_GAP_BLE_UTILS_H_

#include <stdint.h>
#include <stdbool.h>
#include <hrp_utils.h>


#ifdef __cplusplus
extern "C" {
#endif


bool hrp_gap_ble_send_adv_0000_perf_config_rsp(PLTPLib pLTPLib, uint8_t copmsk, uint8_t *pOpt,
                                               uint16_t cause);
bool hrp_gap_ble_send_adv_0000_perf_start_rsp(PLTPLib pLTPLib, uint8_t copmsk, uint8_t *pOpt,
                                              uint16_t cause);
bool hrp_gap_ble_send_adv_0000_perf_cmpl_info(PLTPLib pLTPLib, uint8_t copmsk, uint8_t *pOpt,
                                              uint16_t cause);

#ifdef __cplusplus
}
#endif

#endif
