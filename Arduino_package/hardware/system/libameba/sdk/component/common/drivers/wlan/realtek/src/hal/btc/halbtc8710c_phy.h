#ifndef __INC_HALBTC8710C_PHY_H
#define __INC_HALBTC8710C_PHY_H

#include "bt_intf.h"

uint32_t btc_set_gnt_bt_8710c(pta_type_t gnt_bt);
uint32_t btc_set_gnt_bt_with_clk_source_8710c(pta_type_t gnt_bt);
uint32_t btc_pta_init_8710c(void);

#if 0
void pta_register_read(void);
void pta_debug_register_write(int ver);
void pta_debug_register_read(int ver);
#endif

#endif // __INC_HALBTC8710C_PHY_H
