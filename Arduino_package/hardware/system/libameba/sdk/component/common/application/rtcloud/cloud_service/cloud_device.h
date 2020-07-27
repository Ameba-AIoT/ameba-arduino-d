#ifndef CLOUD_DEVICE_H
#define CLOUD_DEVICE_H

#include "xml.h"

#define MAX_COMMAND_RETRY_COUNT 12

void cmd_cloud(int argc, char **argv);
int is_cloud_started();
void cloud_message_query_status_message_hook_callback(void (*callback)(struct xml_node_set *));
void cloud_retrieve_message_hook_callback(void (*callback)(char *, int, struct xml_node *, char *));
void message_forward_ext_buffer(char *response_buf, int buf_size, char *type, char *application_content, char *reply_key, char *status_rule);

#endif
