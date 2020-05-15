#ifndef __INC_HALBTC8721D_PHY_H
#define __INC_HALBTC8721D_PHY_H
#include <bt_intf.h>
/**
  * @brief Set gnt bt to bt or wifi.
  * @param gnt_bt, 1 is to bt, 0 is to wifi
  * @retval SUCESS or FAIL
  */
u32 btc_set_gnt_bt_8721d(pta_type_t gnt_bt);

/**
  * @brief Set gnt bt to bt or wifi with clock source
  * @param gnt_bt, 1 is to bt, 0 is to wifi
  * @retval SUCESS or FAIL
  */
u32 btc_set_gnt_bt_with_clk_source_8721d(pta_type_t gnt_bt);

/**
  * @brief Initial and enable PTA
  * @param pta_enable, 1 is enable, 0 is disable
  * @retval SUCESS or FAIL
  */
u32 btc_pta_init_8721d(void);

#endif
