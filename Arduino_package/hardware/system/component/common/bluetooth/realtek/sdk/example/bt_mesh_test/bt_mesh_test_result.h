#ifndef BT_MESH_TEST_RESULT_H
#define BT_MESH_TEST_RESULT_H

typedef struct Node { 
    uint16_t SrcAddr;
    uint32_t Test0_PacketNum;
    uint32_t Test1_PacketNum;
    uint32_t Test2_PacketNum;
    struct Node *next;
} Bt_Mesh_Test_Node, *Bt_Mesh_Test_Node_Point;

void link_list_init(void);

void insert_dst_link_list(uint16_t insert_num, uint16_t test_item_index);

void print_link_list(void);

int link_list_length(void);

void destroy_list(void);

void obtain_data_from_link_list (uint8_t * cmd_array, uint16_t test_item_index);

#endif /* BT_MESH_TEST_RESULT_H */