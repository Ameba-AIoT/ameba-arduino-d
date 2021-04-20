#ifndef BT_MESH_DEVICE_TEST_H
#define BT_MESH_DEVICE_TEST_H

#include "datatrans_model.h"
#include "mesh_node.h"

struct BT_MESH_TEST_PARAMETER {
    uint16_t DstAddr;        // Address of Device_DST
    uint16_t TestItemIndex;  // Index of test item 
    uint16_t SendPacketNum;  // Number of packets sent by Device_SRC
    uint16_t PacketInterval; // Interval of packets sent by Device_SRC
    uint16_t DelayTime;      // Delay time of sending packets for Device_SRC
};

void init_bt_mesh_test_parameter(void);

int32_t cmd_data_receive (const mesh_model_info_p pmodel_info, uint32_t type, void *pargs);

void config_test_item_request_post(uint8_t * data_array);

void send_config_test_item_response(uint16_t dst);

void report_test_result_request_post(uint16_t dst, uint8_t * data_array);

void start_test_post();

void test_data_packet_post(uint16_t dst, uint8_t * data_array);

#endif /* BT_MESH_DEVICE_TEST_H */