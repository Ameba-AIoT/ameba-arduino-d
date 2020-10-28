#ifndef BT_MESH_TIMER_HANDLER_H
#define BT_MESH_TIMER_HANDLER_H

#define CMD_SEND_NUM        5
#define CMD_SEND_INTERVAL   1000
#define ORIGINAL_DELAY_TIME 10000

void start_cmd_timer_handler(void *FunctionContext);
void start_test_timer_handler(void *FunctionContext);

#endif /* BT_MESH_TIMER_HANDLER_H */