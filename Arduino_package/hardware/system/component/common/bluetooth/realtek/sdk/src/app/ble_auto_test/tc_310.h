
#ifndef _BLE_AUTO_TEST_TC_310_H_
#define _BLE_AUTO_TEST_TC_310_H_


#ifdef __cplusplus
extern "C" {
#endif


#if TC_301_SUPPORT

typedef struct _T_TC_310_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_310_IN_PARAM_DATA;


void tc_310_adv_only_start(uint32_t count);
void tc_310_adv_only_link_connected(uint8_t conn_id);
void tc_310_phy_update_evt(uint8_t conn_id, uint16_t cause, T_GAP_PHYS_TYPE tx_phy,
                           T_GAP_PHYS_TYPE rx_phy);
void tc_310_adv_only_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_310_dump_result(void);
void tc_310_add_case(uint32_t max_count);


/**
    TC_0301_ADV_DISC
*/

typedef struct _T_TC_311_IN_PARAM_DATA
{
    uint16_t id;
    uint32_t total_test_count;
} T_TC_311_IN_PARAM_DATA;

void tc_311_adv_disc_start(uint32_t count);
void tc_311_adv_disc_link_connected(uint8_t conn_id);
void tc_311_phy_update_evt(uint8_t conn_id, uint16_t cause, T_GAP_PHYS_TYPE tx_phy,
                           T_GAP_PHYS_TYPE rx_phy);

void tc_311_adv_disc_link_disconnected(uint8_t conn_id, uint16_t reason);
void tc_311_dump_result(void);
void tc_311_add_case(uint32_t max_count);
#endif

#ifdef __cplusplus
}
#endif

#endif

