#ifndef _APP_FLAGS_H_
#define _APP_FLAGS_H_

#include "bt_flags.h"

/** @brief  Config APP LE link number */
#define APP_MAX_LINKS  3

#define F_BT_GAPS_CHAR_WRITEABLE            1
#define F_BT_AIRPLANE_MODE_SUPPORT          0
#define F_BT_ANCS_CLIENT_SUPPORT            0
#define F_BT_ANCS_APP_FILTER                (F_BT_ANCS_CLIENT_SUPPORT & 1)
#define F_BT_ANCS_GET_APP_ATTR              (F_BT_ANCS_CLIENT_SUPPORT & 0)
#define F_BT_ANCS_CLIENT_DEBUG              (F_BT_ANCS_CLIENT_SUPPORT & 0)
#define F_BT_GAP_KEY_MANAGER_SUPPORT 1
#define F_BT_TASK_SCHEDULE_DEBUG            0

#define APP_HID_TEST 1
#define APP_GENERAL_CLIENT_TEST             0
#endif
