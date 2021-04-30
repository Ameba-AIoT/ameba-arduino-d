#ifndef  _HRP_PROFILE_SYSTEM_API_H_
#define  _HRP_PROFILE_SYSTEM_API_H_

#include <hrp.h>
#include <hrp_utils.h>
#include <hrp_profile_entry.h>

#define GAP_DATA_TYPE_LEN      2
typedef enum
{
    GAP_PARAM_TYPE_LE = 0,
    GAP_PARAM_TYPE_LE_BOND = 1,
    GAP_PARAM_TYPE_LE_SCAN = 2,
    GAP_PARAM_TYPE_LE_PRIVACY = 3,
    GAP_PARAM_TYPE_LE_ADV = 4,

    GAP_PARAM_TYPE_LEGACY = 0x10,
    GAP_PARAM_TYPE_LEGACY_BOND = 0x11,

    GAP_PARAM_TYPE_COMMON = 0x20,

} T_HRP_GAP_PARAM_TYPE;

typedef enum
{
    LE_SET_PARAM_TYPE_GAP = 0,
    LE_SET_PARAM_TYPE_BOND = 1,
    LE_SET_PARAM_TYPE_SCAN = 2,
    LE_SET_PARAM_TYPE_PRIVACY = 3,
    LE_SET_PARAM_TYPE_ADV = 4,
} T_HRP_LE_SET_PARAM_TYPE;
typedef enum
{
    LE_GET_PARAM_TYPE_GAP = 0,
    LE_GET_PARAM_TYPE_BOND = 1,
    LE_GET_PARAM_TYPE_SCAN = 2,
    LE_GET_PARAM_TYPE_PRIVACY = 3,
    LE_GET_PARAM_TYPE_ADV = 4,

} T_HRP_LE_GET_PARAM_TYPE;




typedef enum
{
    HRP_GAP_LEGACY = 1,
    HRP_GAP_LE = 2,
} T_HRP_GAP_SUPPORT_MODE;



typedef enum
{
    PROFILE_SYS_CMD_SUCESS  = 0X00,
    PROFILE_SYS_CMD_FAIL       = 0X01,
    PROFILE_SYS_CMD_UNSUPPORT = 0X02,
    PROFILE_SYS_CMD_TRYAGAIN  = 0X04,
    PROFILE_SYS_CMD_EXEFAIL    = 0X05,
    PROFILE_SYS_CMD_INVALID_PARAM = 0x06,
    PROFILE_SYS_CMD_INVALID_STATE = 0x07,
} PROFILE_SYS_CAUSE;



void hrp_profile_system_send_cmd_ack(uint8_t cmd_group, uint16_t cmd_index,
                                     HRP_PROFILE_SYSTEM_CMD_STATUS status);

#endif
