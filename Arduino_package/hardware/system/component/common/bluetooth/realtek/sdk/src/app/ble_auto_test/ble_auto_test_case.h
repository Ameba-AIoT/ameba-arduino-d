
#ifndef _BLE_AUTO_TEST_CASE_H_
#define _BLE_AUTO_TEST_CASE_H_
#include "app_msg.h"
#include <tc_common.h>
#include <gap_conn_le.h>
#include "user_cmd_parse.h"

#define TC_0001_SUPPORT      0
#define TC_0002_SUPPORT      0
#define TC_100_SUPPORT      0
#define TC_101_SUPPORT      0
#define TC_200_SUPPORT      1
#define TC_200_SUT_SUPPORT  1
#define TC_206_SUPPORT      1
#define TC_206_SUT_SUPPORT  1
#define TC_207_SUPPORT      1
#define TC_207_SUT_SUPPORT  1
#define TC_208_SUPPORT      1
#define TC_208_SUT_SUPPORT  1
#define TC_300_SUPPORT      0
#define TC_300_SUT_SUPPORT  0
#define TC_301_SUPPORT      0
#define TC_301_SUT_SUPPORT  0
#define TC_310_SUPPORT      0
#define TC_310_SUT_SUPPORT  0
#define TC_311_SUPPORT      0
#define TC_311_SUT_SUPPORT  0
#define TC_400_SUPPORT      0
#define TC_400_SUT_SUPPORT  0
#define TC_401_SUPPORT      0
#define TC_401_SUT_SUPPORT  0
#define TC_500_SUPPORT      1
#define TC_500_SUT_SUPPORT  1
#define TC_501_SUPPORT      1
#define TC_501_SUT_SUPPORT  1
#define TC_502_SUPPORT      1
#define TC_502_SUT_SUPPORT  0
#define TC_600_SUPPORT      1
#define TC_600_SUT_SUPPORT  1
#define TC_601_SUPPORT      1
#define TC_601_SUT_SUPPORT  1
#define TC_700_SUPPORT      0
#define TC_700_SUT_SUPPORT  0
#if F_BT_LE_5_0_SUPPORT
#define TC_800_SUPPORT      0
#define TC_800_SUT_SUPPORT  0
#define TC_801_SUPPORT      0
#define TC_801_SUT_SUPPORT  0
#endif
#if F_BT_LE_PRIVACY_SUPPORT
#define TC_900_SUPPORT      0
#define TC_900_SUT_SUPPORT  0
#define TC_901_SUPPORT      0
#define TC_901_SUT_SUPPORT  0
#endif
#if F_BT_UPPER_STACK_USE_VIRTUAL_HCI
#define TC_1000_SUPPORT     0
#endif
#if F_BT_LE_4_1_CBC_SUPPORT
#define TC_1100_SUPPORT     1
#define TC_1100_SUT_SUPPORT 1
#define TC_1101_SUPPORT     1
#define TC_1101_SUT_SUPPORT 1
#define TC_1102_SUPPORT     1
#define TC_1102_SUT_SUPPORT 1
#endif
#define TC_1200_SUPPORT      0
#define TC_1201_SUPPORT      0
#define TC_1202_SUPPORT      0
#define TC_1203_SUPPORT      0
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
#define TC_1204_SUPPORT      1
#define TC_1205_SUPPORT      1
#define TC_1206_SUPPORT      1
#endif
#define TC_1207_SUPPORT      0
#if F_BT_LE_5_0_AE_ADV_SUPPORT
#define TC_1204_SUT_SUPPORT      1
#define TC_1205_SUT_SUPPORT      1
#define TC_1206_SUT_SUPPORT      1
#endif

#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#include <tc_0001.h>
#endif

#include <tc_100.h>

#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#include <tc_200.h>
#include <tc_300.h>
#include <tc_310.h>
#include <tc_400.h>
#include <tc_500.h>
#include <tc_600.h>
#include <tc_700.h>
#if F_BT_LE_5_0_SUPPORT
#include <tc_800.h>
#endif
#include <tc_900.h>
#include <tc_1000.h>
#if F_BT_LE_4_1_CBC_SUPPORT
#include <tc_1100.h>
#endif
#endif

#if F_BT_LE_GAP_CENTRAL_SUPPORT
#include <tc_200_sut.h>
#include <tc_300_sut.h>
#include <tc_310_sut.h>
#include <tc_400_sut.h>
#include <tc_500_sut.h>
#include <tc_600_sut.h>
#if F_BT_LE_5_0_SUPPORT
#include <tc_800_sut.h>
#endif
#if F_BT_LE_4_1_CBC_SUPPORT
#include <tc_1100_sut.h>
#endif
#endif

#include <tc_1200.h>

#include <tc_1200_sut.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t TGATTDBdAddr[6];
extern TGATTDBdAddr g_cur_rembd;


typedef enum
{
    TC_IDLE = 0x0000,
    TC_0001_ADV_PERFORMANCE = 1,
    TC_0002_ADV_STRESS_START_STOP,

    TC_0100_SCAN_PERFORMANCE = 100,
    TC_0101_SCAN_STRESS_ENABLE_DISABLE,

    TC_0200_TP_NOTIFICATION_TX_01 = 200,
    TC_0201_TP_INDICATION_TX_01,

    TC_0202_TP_WRITE_COMMAND_TX_01,
    TC_0203_TP_NOTIFICATION_RX_01,
    TC_0204_TP_INDICATION_RX_01,
    TC_0205_TP_WRITE_COMMAND_RX_01,

    TC_0206_TP_NOTIFICATION_TX_02,
    TC_0207_TP_WRITE_COMMAND_RX_02,
    TC_0208_TP_NOTIF_WRITE_CMD_TRX_02,

    TC_0300_ADV_ONLY = 300,
    TC_0301_ADV_DISC,

    TC_0310_2M_LONGRANGE_1 = 310,
    TC_0311_2M_LONGRANGE_2,

    TC_0400_CONN_PARAM_UPDATE_SLAVE = 400,//10
    TC_0401_CONN_PARAM_UPDATE_SLAVE_01,

    TC_0500_SLAVE_AUTO_ADV = 500,
    TC_0501_SLAVE_AUTO_ADV_WITH_SEC_REQ,
    TC_0502_SLAVE_MULTIPLE_LINK_AUTO_ADV,

    TC_0600_IOP_PAIR_LEGACL = 600,
    TC_0601_IOP_PAIR_SC,

    TC_0700_STABLE_NOTIFICATION_TX_01 = 700,

    TC_0800_IOP_PAIR_LEGACL = 800,
    TC_0801_IOP_PAIR_SC,

    TC_0900_PRIVACY_TEST_SLAVE = 900,
    TC_0901_PRIVACY_TEST_MASTER,

    TC_1000_CP_TEST = 1000,
    TC_1001_CP_TEST = 1001,
    TC_1002_CP_TEST = 1002,

    TC_1100_BT41_CONN_TEST = 1100,
    TC_1101_BT41_TRX_STRESS_TEST = 1101,
    TC_1102_BT41_TP_TEST = 1102,

    TC_1200_MULTI_LINK_4_MASTER = 1200,
    TC_1201_MULTI_LINK_4_MASTER,
    TC_1202_MULTI_LINK_4_MASTER,
    TC_1203_MULTI_LINK_4_MASTER,
    TC_1204_MULTI_LINK_4_MASTER,
    TC_1205_MULTI_LINK_4_MASTER,
    TC_1206_MULTI_LINK_4_MASTER,
    TC_1207_MULTI_LINK_4_MASTER,


    TC_MAX = 65535
} T_CUR_TEST_CASE;


typedef enum  _T_CUR_DEVICE_ROLE
{
    TC_ROLE_UNDEFINED,
    TC_ROLE_DUT,
    TC_ROLE_SUT
} T_CUR_DEVICE_ROLE;

T_CUR_DEVICE_ROLE app_get_cur_tc_role(void);
void app_set_cur_tc_role(T_CUR_DEVICE_ROLE role);

T_CUR_TEST_CASE app_get_cur_test_case(void);
void app_set_dlps_mode(T_USER_CMD_PARSED_VALUE *p_parse_value);
void app_select_cur_test_case(T_USER_CMD_PARSED_VALUE *p_parse_value);
void app_dump_cur_test_case(T_USER_CMD_PARSED_VALUE *p_parse_value);
bool app_set_rembd(T_USER_CMD_PARSED_VALUE *p_parse_value);
void app_set_cur_case(T_CUR_TEST_CASE test_case_id);


/**
    common api
*/
uint32_t os_time_get_elapsed(uint32_t begin, uint32_t end);


#ifdef __cplusplus
}
#endif

#endif

