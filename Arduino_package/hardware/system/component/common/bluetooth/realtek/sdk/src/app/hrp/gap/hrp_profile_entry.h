#ifndef  _HRP_PROFILE_ENTRY_H_
#define  _HRP_PROFILE_ENTRY_H_

#include <hrp_utils.h>
#include <hrp_application.h>
#include <btif.h>
//#include <hrp_btif_system_api.h>

#define MAX_PROFILE_CMD_INDEX  10
typedef enum
{
    HRP_PROFILE_CMD_GROUP_SYSTEM = 0x0f,
    HRP_PROFILE_CMD_GROUP_GAP = 1,
    HRP_PROFILE_CMD_GROUP_A2DP   = 2,
    HRP_PROFILE_CMD_GROUP_AVRCP  = 3,
    HRP_PROFILE_CMD_GROUP_HFP    = 4,
    HRP_PROFILE_CMD_GROUP_SPP    = 5,

    HRP_PROFILE_CMD_GROUP_GAP_LEGACY = 0x10,
    HRP_PROFILE_CMD_GROUP_EVENT_GAP_LEGACY = 0x11,

    HRP_PROFILE_CMD_GROUP_LE = 0x21,

    HRP_PROFILE_CMD_GROUP_EVENT_SYS = 0x4f,
    HRP_PROFILE_CMD_GROUP_EVENT_GAP = 0x41,
    HRP_PROFILE_CMD_GROUP_EVENT_A2DP = 0x42,
    HRP_PROFILE_CMD_GROUP_EVENT_AVRCP  = 0x43,
    HRP_PROFILE_CMD_GROUP_EVENT_HFP    = 0x44,
    HRP_PROFILE_CMD_GROUP_EVENT_SPP    = 0X45,

    HRP_PROFILE_CMD_GROUP_EVENT_LE    = 0X61,

    HRP_PROFILE_CMD_GROUP_EVENT_GENERAL_ID = 0x65,

    HRP_PROFILE_CMD_GROUP_SIMP_BLE    = 0X70,
    HRP_PROFILE_CMD_GROUP_GAPS_CLIENT    = 0X71,

    HRP_PROFILE_CMD_GROUP_EVENT_SIMP_BLE = 0X80,
    HRP_PROFILE_CMD_GROUP_EVENT_GAPS_CLIENT = 0X81,

} HRP_PROFILE_CMD_GROUP;

typedef enum
{
    HRP_PROFILE_SYSTEM_COMMAND_COMPLETE = 0,
    HRP_PROFILE_SYSTEM_COMMAND_DISALLOW = 1,
    HRP_PROFILE_SYSTEM_UNKNOWN_COMMAND = 2,
} HRP_PROFILE_SYSTEM_CMD_STATUS;

typedef enum
{
    PROFILE_SYSTEM_CMD_RESULT = 1,
    PROFILE_SYSTEM_RESET_COMPLETE = 2,
    PROFILE_SYSTEM_INIT_COMPLETE = 3,
    PROFILE_SYSTEM_SET_GAP_PARAM_RSP = 4,
    PROFILE_SYSTEM_SET_PAIRABLE_MODE_RSP = 5,
    PROFILE_SYSTEM_ENABLE_DLPS_RSP = 6,

    PROFILE_SYSTEM_EVENT_ACK = 7,
    PROFILE_SYSTEM_CMD_ACK = 8,

} PROFILE_SYSTEM_CMD_ENTRY;

void hrp_profile_handle_req(uint8_t cmd_group, uint16_t cmd_index, uint16_t param_list_len,
                            uint8_t *p_param_list);

extern void (*(hrp_profile_handle_system[]))(uint16_t len, uint8_t *p_param_list) ;
extern void (*(hrp_profile_handle_gap[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_a2dp[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_avrcp[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_hfp[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_spp[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_gap_legacy_cmd_table[]))(uint16_t len, uint8_t *p_param_list);

extern void (*(hrp_profile_handle_gap_le[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_simp_ble[]))(uint16_t len, uint8_t *p_param_list);
extern void (*(hrp_profile_handle_gaps_client[]))(uint16_t len, uint8_t *p_param_list);


/*--------------------------------------------------------------------------*/
//============utils=====================//
void hrp_profile_evet(uint8_t cmd_group, uint16_t cmd_index, uint16_t param_list_len,
                      uint8_t *p_param_list);
void hrp_profile_commit_buf(uint8_t *destbuf, int *pos, void *srcbuf, int length);
void hrp_profile_fetch_buf(void *destbuf, uint16_t *pos, void *srcbuf, int length);

#endif
