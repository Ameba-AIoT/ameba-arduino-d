#ifndef _APP_FLAGS_H_
#define _APP_FLAGS_H_

#include "bt_flags.h"

#define F_BT_GAPS_CHAR_WRITEABLE            0
#define F_BT_AIRPLANE_MODE_SUPPORT          0
#define F_BT_ANCS_CLIENT_SUPPORT            0
#define F_BT_ANCS_APP_FILTER                (F_BT_ANCS_CLIENT_SUPPORT & 0)
#define F_BT_ANCS_GET_APP_ATTR              (F_BT_ANCS_CLIENT_SUPPORT & 0)
#define F_BT_ANCS_CLIENT_DEBUG              (F_BT_ANCS_CLIENT_SUPPORT & 0)

#define F_BT_TASK_SCHEDULE_DEBUG            0

#define APP_HID_TEST 0
#endif
