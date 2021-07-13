#ifndef  _HRP_BTIF_ENTRY_H_
#define  _HRP_BTIF_ENTRY_H_

#include <hrp_utils.h>
#include <hrp_application.h>
#include <btif.h>

typedef enum
{
    HRP_BTIF_CMD_GROUP_STACK   = 0,
    HRP_BTIF_CMD_GROUP_SYSTEM = 1,
} HRP_BTIF_CMD_GROUP;

typedef struct _GattDevInfo
{
    uint16_t    le_ds_pool_id;
    uint16_t    le_ds_data_offset;
    uint8_t     le_ds_credits;
} T_GATT_DEV_INFO;


typedef struct
{
    uint8_t   *p_srv_handle;
    void *p_service;
    bool is_used;
    uint8_t self_idx;
    uint16_t nbr_of_attrib;
    uint16_t database_length;
    uint16_t receive_length;
    uint32_t   host_service;
} T_GATT_SRV_TABLE;

typedef void (* P_HRP_BTIF_HANDLER)(uint16_t len, uint8_t *p_param);

void hrp_btif_handle_req(uint8_t cmd_group, uint16_t cmd_index, uint16_t param_list_len,
                         uint8_t *p_param_list);
void hrp_btif_handle_msg(T_BTIF_UP_MSG *pMsg);

//const extern P_HRP_BTIF_HANDLER hrp_btif_handle_bredr[];
//const extern  P_HRP_BTIF_HANDLER hrp_btif_handle_ble[];


#endif

