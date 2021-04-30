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
#ifndef _MISC_CMD_H
#define _MISC_CMD_H

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
 * @addtogroup MISC_CMD
 * @{
 */

/**
 * @defgroup Misc_Cmd_Exported_Macros Miscellaneous Command Exported Macros
 * @brief
 * @{
 */
#define MISC_CMD \
    {\
        "iocapset",\
        "iocapset [iocap]\n\r",\
        "choose io capability.\n\r",\
        user_cmd_io_capa_set\
    },\
    {\
     "sauth",\
     "sauth\n\r",\
     "bond manager authentication request.\n\r",\
     user_cmd_security_req\
    },\
    {\
     "conupdreq",\
     "conupdreq \n\r",\
     "LE connection update request\n\r",\
     user_cmd_con_update_req\
    },\
    {\
     "txpwrset",\
     "txpwrset [txpwridx]\n\r",\
     "Set the TX Power of BLE RF\n\r",\
     user_cmd_tx_power_set\
    },\
    {\
     "rssiread",\
     "rssiread\n\r",\
     "Read the RSSI value of this local MDL ID\n\r",\
     user_cmd_rssi_read\
    },\
    {\
     "patchver",\
     "patchver\n\r",\
     "Get the patch Version\n\r",\
     user_cmd_patch_version_get\
    },\
    {\
     "chanclassset",\
     "chanclassset [idx0] [idx1] [idx2] [idx3] [idx4]\n\r",\
     "Set Host Channel Classification\n\r",\
     user_cmd_channel_class_set\
    },\
    {\
     "srvdis",\
     "srvdis \n\r",\
     "Service discovery, discover all primary services.\n\r",\
     user_cmd_service_discover\
    },\
    {\
     "reldis",\
     "reldis [start] [end]\n\r",\
     "Relationship discovery\n\r",\
     user_cmd_relation_discover\
    },\
    {\
     "chardis",\
     "chardis [start] [end]\n\r",\
     "Characteristic discovery\n\r",\
     user_cmd_character_discover\
    },\
    {\
     "charddis",\
     "charddis [start] [end]\n\r",\
     "Characteristic descriptor discovery\n\r",\
     user_cmd_character_descriptor_discover\
    },\
    {\
     "read",\
     "read [handle] \n\r",\
     "Read attribute\n\r",\
     user_cmd_attribute_read\
    },\
    {\
     "readu",\
     "readu [start] [end] [UUID16]\n\r",\
     "Read attribute using UUID16\n\r",\
     user_cmd_attribute_read_by_uuid\
    }
/** @} */

/**
 * @defgroup Misc_Cmd_Exported_Functions Miscellaneous Command Exported Functions
 * @brief
 * @{
 */
user_cmd_parse_result_t user_cmd_io_capa_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_security_req(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_con_update_req(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_service_discover(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_relation_discover(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_character_discover(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_character_descriptor_discover(user_cmd_parse_value_t
                                                               *pparse_value);
user_cmd_parse_result_t user_cmd_attribute_read(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_attribute_read_by_uuid(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_tx_power_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_rssi_read(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_patch_version_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_channel_class_set(user_cmd_parse_value_t *pparse_value);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _MISC_CMD_H */
