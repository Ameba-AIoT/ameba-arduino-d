/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_cmd.h
  * @brief    Head file for mesh common cmd.
  * @details  User command interfaces.
  * @author   bill
  * @date     2017-3-31
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_CMD_H
#define _MESH_CMD_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "mesh_data_uart.h"
#include "mesh_user_cmd_parse.h"
#include "mesh_api.h"

/****************************************************************************************************************
* exported variables other .c files may use all defined here.
****************************************************************************************************************/
/**
 * @addtogroup MESH_CMD
 * @{
 */

/**
 * @defgroup Mesh_Cmd_Exported_Macros Mesh Command Exported Macros
 * @brief
 * @{
 */

#define MESH_COMMON_CMD \
    {\
        "reset",\
        "reset\n\r",\
        "reset\n\r",\
        user_cmd_reset\
    },\
    {\
     "ls",\
     "ls\n\r",\
     "list node state info\n\r",\
     user_cmd_list\
    },\
    {\
     "dis",\
     "dis [1 on/0 off]\n\r",\
     "device information show\n\r",\
     user_cmd_dev_info_show\
    },\
    {\
     "dus",\
     "dus [dev uuid]\n\r",\
     "device uuid set\n\r",\
     user_cmd_dev_uuid_set\
    },\
    {\
     "bas",\
     "bas [on off]\n\r",\
     "bearer adv set\n\r",\
     user_cmd_bearer_adv_set\
    },\
    {\
     "pav",\
     "pav [value]\n\r",\
     "prov authentication value\n\r",\
     user_cmd_prov_auth_value\
    },\
    {\
     "vas",\
     "vas [label uuid]\n\r",\
     "virtual addr set\n\r",\
     user_cmd_vir_addr_set\
    },\
    {\
     "tping",\
     "tping [dst] [init ttl] [net_key_index] [pong_max_delay/10ms] [period/10ms]\n\r",\
     "ping a remote device at the transport layer\n\r",\
     user_cmd_trans_ping\
    },\
    {\
     "ping",\
     "ping [dst] [init ttl] [app_key_index] [pong_max_delay/10ms] [period/10ms]\n\r",\
     "ping a remote device at the model layer\n\r",\
     user_cmd_ping\
    },\
    {\
     "bping",\
     "bping [dst] [init ttl] [app_key_index] [pong_max_delay/10ms] [period/10ms]\n\r",\
     "ping a remote device with a big seg msg at the model layer\n\r",\
     user_cmd_big_ping\
    },\
    {\
     "tp",\
     "tp [dst] [init ttl] [app_key_index] [msg]\n\r",\
     "tp test the transparent message\n\r",\
     user_cmd_tp_msg\
    },\
    {\
     "tps",\
     "tps [dst] [init ttl] [app_key_index] [count]\n\r",\
     "tp start to test the throughput of messages\n\r",\
     user_cmd_tp_start\
    },\
    {\
     "con",\
     "con [bt addr] [addr type]\n\r",\
     "connect to remote device\n\r",\
     user_cmd_connect\
    },\
    {\
     "disc",\
     "disc [conn id]\n\r",\
     "disconnect to remote device\n\r",\
     user_cmd_disconnect\
    },\
    {\
     "proxydis",\
     "proxydis [conn id]\n\r",\
     "Start discovery proxy service\n\r",\
     user_cmd_proxy_discover\
    },\
    {\
     "proxycmd",\
     "proxycmd [char CCCD] [command: enable/disable]\n\r",\
     "Proxy notify/ind switch command\n\r",\
     user_cmd_proxy_cccd_operate\
    },\
    {\
     "proxyls",\
     "proxyls\n\r",\
     "Proxy server handle list\n\r",\
     user_cmd_proxy_list\
    },\
    {\
     "pcsft",\
     "pcsft [type]\n\r",\
     "proxy cfg set filter type\n\r",\
     user_cmd_proxy_cfg_set_filter_type\
    },\
    {\
     "pcaa",\
     "pcaa [addr]\n\r",\
     "proxy cfg add addr\n\r",\
     user_cmd_proxy_cfg_add_addr\
    },\
    {\
     "pcra",\
     "pcra [addr]\n\r",\
     "proxy cfg remove addr\n\r",\
     user_cmd_proxy_cfg_remove_addr\
    },\
    {\
     "log",\
     "log [bit0: lower, bit1: upper, bit2: mesh]\n\r",\
     "log level setting\n\r",\
     user_cmd_log_set\
    },\
    {\
     "time",\
     "time\n\r",\
     "time since power-on\n\r",\
     user_cmd_time\
    },\
    {\
     "aps",\
     "aps [0:-20dBm, 1:0dBm, 2:4dBm, 3:8dBm]\n\r",\
     "adv power set\n\r",\
     user_cmd_adv_power_set\
    },\
    {\
     "fninit",\
     "fninit [lpn num] [queue size] [rx window(ms)]\n\r",\
     "friend node init\n\r",\
     user_cmd_fn_init\
    },\
    {\
     "fndeinit",\
     "fndeinit\n\r",\
     "friend node deinit\n\r",\
     user_cmd_fn_deinit\
    },\
    {\
     "hbpub",\
     "hbpub [dst] [count] [period]\n\r",\
     "Publish heartbeat\n\r",\
     user_cmd_hb_pub\
    },\
    {\
     "hbsub",\
     "hbsub [src] [dst] [period]\n\r",\
     "Subscribe heartbeat\n\r",\
     user_cmd_hb_sub\
    },\
    {\
     "md",\
     "md\n\r",\
     "mesh deinit\n\r",\
     user_cmd_mesh_deinit\
    }


#define PING_TIMEOUT_MSG           101
/** @} */


/**
 * @defgroup Mesh_Cmd_Exported_Types Mesh Command Exported Types
 * @brief
 * @{
 */
extern bool dev_info_show_flag;
/** @} */

/**
 * @defgroup Mesh_Cmd_Exported_Functions Mesh Command Exported Functions
 * @brief
 * @{
 */
user_cmd_parse_result_t user_cmd_reset(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_list(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_dev_info_show(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_dev_uuid_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_bearer_adv_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_prov_auth_value(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_vir_addr_set(user_cmd_parse_value_t *pparse_value);
mesh_msg_send_cause_t ping_handle_timeout(void);
user_cmd_parse_result_t user_cmd_trans_ping(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_ping(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_big_ping(user_cmd_parse_value_t *pparse_value);
int32_t tp_reveive(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs);
void pong_receive(uint16_t src, uint16_t dst, uint8_t hops_forward, ping_pong_type_t type,
                  uint8_t hops_reverse, uint16_t pong_delay);
user_cmd_parse_result_t user_cmd_tp_msg(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_tp_start(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_connect(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_disconnect(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_proxy_discover(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_proxy_cccd_operate(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_proxy_list(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_proxy_cfg_set_filter_type(user_cmd_parse_value_t
                                                           *pparse_value);
user_cmd_parse_result_t user_cmd_proxy_cfg_add_addr(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_proxy_cfg_remove_addr(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_log_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_adv_power_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_fn_init(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fn_deinit(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_hb_pub(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_hb_sub(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_mesh_deinit(user_cmd_parse_value_t *pparse_value);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _MESH_CMD_H */

