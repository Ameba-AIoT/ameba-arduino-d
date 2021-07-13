
#ifndef _BLE_AUTO_TEST_TC_1200_SUT_H_
#define _BLE_AUTO_TEST_TC_1200_SUT_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
   dut is slave, dut init connection param update
*/



void tc_1204_sut_start(uint32_t count);
void tc_1204_sut_link_connected(uint8_t conn_id);
void tc_1204_sut_link_disconnected(uint8_t conn_id, uint16_t reason);

void tc_1205_sut_start(uint32_t count);
void tc_1205_sut_link_connected(uint8_t conn_id);
void tc_1205_sut_link_disconnected(uint8_t conn_id, uint16_t reason);

void tc_1206_sut_start(uint32_t count);
void tc_1206_sut_link_connected(uint8_t conn_id);
void tc_1206_sut_link_disconnected(uint8_t conn_id, uint16_t reason);


#ifdef __cplusplus
}
#endif

#endif

