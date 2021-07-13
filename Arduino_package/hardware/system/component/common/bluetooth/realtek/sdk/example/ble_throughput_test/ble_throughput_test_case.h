
#ifndef _BLE_AUTO_TEST_CASE_H_
#define _BLE_AUTO_TEST_CASE_H_
#include "app_msg.h"
#include <gap_conn_le.h>
#include "user_cmd_parse.h"
 
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#include <ble_throughput_200.h>
#endif

#if F_BT_LE_GAP_CENTRAL_SUPPORT
#include <ble_throughput_200_sut.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t TGATTDBdAddr[6];
extern TGATTDBdAddr g_cur_rembd;


typedef enum
{
    TC_IDLE = 0x0000,

    TC_0206_TP_NOTIFICATION_TX_02 = 206,
    TC_0207_TP_WRITE_COMMAND_RX_02,

    TC_MAX = 65535
} T_CUR_TEST_CASE;


typedef enum  _T_CUR_DEVICE_ROLE
{
    TC_ROLE_UNDEFINED,
    TC_ROLE_DUT,
    TC_ROLE_SUT
} T_CUR_DEVICE_ROLE;

T_CUR_DEVICE_ROLE ble_throughput_app_get_cur_role(void);
void ble_throughput_app_set_cur_role(T_CUR_DEVICE_ROLE role);

T_CUR_TEST_CASE ble_throughput_app_get_cur_test_case(void);
void ble_throughput_app_select_cur_test_case(T_USER_CMD_PARSED_VALUE *p_parse_value);
bool ble_throughput_app_set_rembd(T_USER_CMD_PARSED_VALUE *p_parse_value);


/**
    common api
*/
uint32_t ble_throughput_os_time_get_elapsed(uint32_t begin, uint32_t end);


#ifdef __cplusplus
}
#endif

#endif

