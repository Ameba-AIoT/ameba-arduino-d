#ifndef BT_MESH_APP_USER_CMD_PARSE_H
#define BT_MESH_APP_USER_CMD_PARSE_H

enum {
    Configure_Test_Item_Request,
    Configure_Test_Item_Response,
    Start_Test,
    Report_Test_Result_Request,
    Report_Test_Result_Response,
    Test_Data_Packet,
    Test_Data_Packet_Response,
    Clear_Test_Result
};

void config_cmd_parsing(char ** argv);
void start_cmd_parsing(char ** argv);
void report_cmd_parsing(char ** argv);

#endif /* BT_MESH_APP_USER_CMD_PARSE_H */