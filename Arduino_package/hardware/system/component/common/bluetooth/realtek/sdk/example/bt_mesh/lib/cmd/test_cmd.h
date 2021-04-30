/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     misc_cmd.h
  * @brief    Head file for miscellaneous cmd.
  * @details  User command interfaces.
  * @author   bill
  * @date     2017-12-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _TEST_CMD_H
#define _TEST_CMD_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "mesh_data_uart.h"
#include "mesh_user_cmd_parse.h"

/****************************************************************************************************************
* exported variables other .c files may use all defined here.
****************************************************************************************************************/
/**
 * @addtogroup TEST_CMD
 * @{
 */

/**
 * @defgroup Test_Cmd_Exported_Macros Test Command Exported Macros
 * @brief
 * @{
 */
#define TEST_CMD \
    {\
        "nss",\
        "nss [node state]\n\r",\
        "node state set\n\r",\
        user_cmd_node_state_set\
    },\
    {\
     "nas",\
     "nas [unicast addr]\n\r",\
     "node addr set\n\r",\
     user_cmd_node_addr_set\
    },\
    {\
     "nks",\
     "nks [index] [index_g] [key]\n\r",\
     "net key set\n\r",\
     user_cmd_net_key_set\
    },\
    {\
     "nkr",\
     "nkr [index]\n\r",\
     "net key refresh\n\r",\
     user_cmd_net_key_refresh\
    },\
    {\
     "aks",\
     "aks [index] [index_n] [index_g] [key]\n\r",\
     "app key set\n\r",\
     user_cmd_app_key_set\
    },\
    {\
     "dks",\
     "dks [index] [address] [element_num] [key]\n\r",\
     "dev key set\n\r",\
     user_cmd_dev_key_set\
    },\
    {\
     "ss",\
     "ss [seq]\n\r",\
     "seq set\n\r",\
     user_cmd_seq_set\
    },\
    {\
     "iis",\
     "iis [iv index]\n\r",\
     "iv index set\n\r",\
     user_cmd_iv_index_set\
    },\
    {\
     "iit",\
     "iit [1: update, 0: normal]\n\r",\
     "iv index transit\n\r",\
     user_cmd_iv_index_transit\
    },\
    {\
     "iim",\
     "iim [1: on, 0: off]\n\r",\
     "iv index mode\n\r",\
     user_cmd_iv_index_mode\
    },\
    {\
     "nmcc",\
     "nmcc\n\r",\
     "net msg cache clear\n\r",\
     user_cmd_nmc_clear\
    },\
    {\
     "rplc",\
     "rplc\n\r",\
     "replay protect list clear\n\r",\
     user_cmd_rpl_clear\
    },\
    {\
     "subscribe",\
     "subscribe [element index] [model index] [addr]\n\r",\
     "subscribe\n\r",\
     user_cmd_subscribe\
    },\
    {\
     "ts",\
     "ts [dst] [ttl] [len] [app key index(0xff:DevKey)]\n\r",\
     "test send\n\r",\
     user_cmd_test_send\
    },\
    {\
     "testdata",\
     "testdata [test case]\n\r",\
     "test sample mesh pkt\n\r",\
     user_cmd_test_data\
    }
/** @} */

/**
 * @defgroup Test_Cmd_Exported_Functions Test Command Exported Functions
 * @brief
 * @{
 */
user_cmd_parse_result_t user_cmd_node_state_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_node_addr_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_net_key_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_net_key_refresh(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_app_key_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_dev_key_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_seq_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_iv_index_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_iv_index_transit(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_iv_index_mode(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_nmc_clear(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_rpl_clear(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_subscribe(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_test_send(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_test_data(user_cmd_parse_value_t *pparse_value);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _TEST_CMD_H */
