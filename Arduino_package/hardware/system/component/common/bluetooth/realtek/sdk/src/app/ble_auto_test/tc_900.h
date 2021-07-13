
#ifndef _BLE_AUTO_TEST_TC_900_H_
#define _BLE_AUTO_TEST_TC_900_H_

#include <stdint.h>
#include <gap_le.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum
{
    PRIVACY_INIT_STATE_INIT,
    PRIVACY_INIT_STATE_REL_LIST, //!< Disconnected.
    PRIVACY_INIT_STATE_REL_ENABLE,
    PRIVACY_INIT_STATE_REL_IDLE
} T_PRIVACY_INIT_STATE;



extern T_PRIVACY_INIT_STATE g_privacy_init_state;

/**
    TC_0900_PRIVACY_TEST_SLAVE
*/
void tc_900_privacy_slave(void);

/**
    TC_0901_PRIVACY_TEST_MASTER
*/
void tc_901_privacy_master(void);
void tc_901_handle_bond_modify_msg(T_LE_BOND_MODIFY_TYPE type, T_LE_KEY_ENTRY *p_entry);

#ifdef __cplusplus
}
#endif

#endif

