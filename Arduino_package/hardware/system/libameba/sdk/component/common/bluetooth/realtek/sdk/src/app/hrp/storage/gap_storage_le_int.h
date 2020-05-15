/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      flash_key_mgr_le_int.h
* @brief     key storage function.
* @details
* @author    jane
* @date      2016-02-18
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef     FLASH_KEY_MGR_LE_INT_H
#define     FLASH_KEY_MGR_LE_INT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <btif.h>
#include <gap_storage_flash_int.h>
#include <gap_storage_le.h>

void le_key_init(void);
uint8_t le_get_key(P_LE_KEY_ENTRY p_entry, T_BTIF_KEY_TYPE key_type, uint8_t *key);
bool le_save_key(P_LE_KEY_ENTRY p_entry, T_BTIF_KEY_TYPE key_type, uint8_t key_length,
                 uint8_t *key);
bool le_update_remote_bd(P_LE_KEY_ENTRY p_entry, uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE bd_type);
P_LE_KEY_ENTRY le_allocate_key_entry(uint8_t *bd_addr, T_BTIF_REMOTE_ADDR_TYPE bd_type);
void le_clear_all_keys(void);
void le_delete_bond(P_LE_KEY_ENTRY p_entry);

bool le_get_cccd_data(P_LE_KEY_ENTRY p_entry, P_LE_CCCD p_data);
bool le_save_cccd_data(P_LE_KEY_ENTRY p_entry, P_LE_CCCD p_data);
bool le_add_cccd_data(P_LE_KEY_ENTRY p_entry, uint8_t add_size, uint8_t offset, uint8_t *p_data);


P_LE_KEY_ENTRY le_find_entry_by_aes(uint8_t *unresolved_addr, uint8_t addr_type);
bool le_check_local_resolved_address(uint8_t *unresolved_addr, uint8_t *local_irk);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_KEY_MGR_LE_INT_H */
