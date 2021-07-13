/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     privacy_mgnt.h
  * @brief    privacy managerment.
  * @details  privacy managerment.
  * @author   jane
  * @date     2016-02-18
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PRIVACY_MGNT_H_
#define _PRIVACY_MGNT_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#if F_BT_LE_PRIVACY_SUPPORT
#include <gap_bond_le.h>
#include <gap_privacy.h>

#define PRIVACY_ENTRY_SIZE 4
#define PRIVACY_RESOLVING_LIST_IDLE            0
#define PRIVACY_RESOLVING_LIST_ADD_PENDING     1
#define PRIVACY_RESOLVING_LIST_REMOVE_PENDING  2
typedef struct
{
    bool is_used;
    uint8_t  addr[6];
    T_GAP_IDENT_ADDR_TYPE  remote_bd_type;
    bool is_add_to_list;
    uint8_t pending;

} T_LE_PRIVACY_ENTRY, *P_LE_PRIVACY_ENTRY;

typedef enum
{
    PRIVACY_STATE_MSGTYPE,
    PRIVACY_RESOLUTION_STATUS_MSGTYPE
} T_PRI_CB_TYPE;

typedef enum
{
    PRIVACY_STATE_INIT,
    PRIVACY_STATE_IDLE, //!< Disconnected.
    PRIVACY_STATE_BUSY
} T_PRIVACY_STATE;
extern T_LE_PRIVACY_ENTRY privacy_table[PRIVACY_ENTRY_SIZE];
typedef void(*P_FUN_PRIVACY_STATE_CB)(T_PRI_CB_TYPE type, uint8_t status);

T_APP_RESULT App_PrivacyCallback(uint8_t msg_type, T_LE_PRIVACY_CB_DATA msg_data);
void privacy_init_resolving_list(P_FUN_PRIVACY_STATE_CB p_fun);
T_PRIVACY_STATE privacy_handle_pending_resolving_list(void);
void privacy_add_pending_resolving_list(T_GAP_RESOLV_LIST_OP op,
                                        T_GAP_IDENT_ADDR_TYPE addr_type, uint8_t *addr);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _PRIVACY_MGNT_H_ */
