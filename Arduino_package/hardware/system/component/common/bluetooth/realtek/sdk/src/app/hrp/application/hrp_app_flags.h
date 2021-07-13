#ifndef _HRP_APP_FLAGS_H_
#define _HRP_APP_FLAGS_H_

#include "bt_flags.h"

/** @brief  Config APP LE link number */
#define APP_MAX_LINKS  3

#define F_BT_DLPS_EN 0
#define F_BT_LE_BTIF_SUPPORT                (F_BT_LE_SUPPORT && 1)

#define F_BT_CONFIG_INTERNAL_API            1

//add for compile error
#define B_ADDR_LEN 6
#define REMOTE_FEATURES_LEN 8

#define F_BT_PATCH_CHANGE_BOND_INFO 1
#define F_BT_PATCH_SRV_CHANGE_IND   1
#define F_BT_PATCH_ADD_PRIVACY_API  1

#endif
