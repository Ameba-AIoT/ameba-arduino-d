#ifndef _BT_EXAMPLE_ENTRY_H_
#define _BT_EXAMPLE_ENTRY_H_

void bt_example_init(void);
#if defined(CONFIG_PLATFORM_8710C) && defined(CONFIG_FTL_ENABLED)
void app_ftl_init(void);
#endif

#endif //_BT_EXAMPLE_ENTRY_H_