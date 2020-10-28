
#include "BLEDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_drv.h"
#include "trace_app.h"
#include "bte.h"
#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "os_task.h"
#include "os_msg.h"
#include "app_msg.h"
#include "rtk_coex.h"
#include "profile_server.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif

//---------------------------- Default handlers for core BLE functionality ----------------------------//

T_APP_RESULT BLEDevice::gapCallbackDefault(uint8_t cb_type, void *p_cb_data) {
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type) {
        case GAP_MSG_LE_DATA_LEN_CHANGE_INFO: {
            if (BTDEBUG) printf("GAP_MSG_LE_DATA_LEN_CHANGE_INFO: conn_id %d, tx octets 0x%x, max_tx_time 0x%x\r\n", p_data->p_le_data_len_change_info->conn_id, p_data->p_le_data_len_change_info->max_tx_octets, p_data->p_le_data_len_change_info->max_tx_time);
            break;
        }
        case GAP_MSG_LE_MODIFY_WHITE_LIST: {
            if (BTDEBUG) printf("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x\r\n", p_data->p_le_modify_white_list_rsp->operation, p_data->p_le_modify_white_list_rsp->cause);
            break;
        }
        case GAP_MSG_LE_CONN_UPDATE_IND: {
            if (BTDEBUG) {
                uint8_t conn_id = p_data->p_le_conn_update_ind->conn_id;
                uint16_t conn_int_max = p_data->p_le_conn_update_ind->conn_interval_max;
                uint16_t conn_int_min = p_data->p_le_conn_update_ind->conn_interval_min;
                uint16_t conn_latency = p_data->p_le_conn_update_ind->conn_latency;
                uint16_t supervision_timeout = p_data->p_le_conn_update_ind->supervision_timeout;
                printf("GAP_MSG_LE_CONN_UPDATE_IND: conn_id %d, conn_interval_max %d ms, conn_interval_min %d ms, conn_latency %d, supervision_timeout %d ms",
                    conn_id, (uint16_t)(conn_int_max * 1.25), (uint16_t)(conn_int_min * 1.25), conn_latency, (uint16_t)(supervision_timeout * 10));
            }
            /* if reject the proposed connection parameter from peer device, use APP_RESULT_REJECT. */
            result = APP_RESULT_ACCEPT;
            break;
        }
        case GAP_MSG_LE_PHY_UPDATE_INFO: {
            if (BTDEBUG) printf("GAP_MSG_LE_PHY_UPDATE_INFO:conn_id %d, cause 0x%x, rx_phy %d, tx_phy %d\r\n", p_data->p_le_phy_update_info->conn_id, p_data->p_le_phy_update_info->cause, p_data->p_le_phy_update_info->rx_phy, p_data->p_le_phy_update_info->tx_phy);
            break;
        }        
        case GAP_MSG_LE_REMOTE_FEATS_INFO: {
            uint8_t  remote_feats[8];
            //if (BTDEBUG) printf("GAP_MSG_LE_REMOTE_FEATS_INFO: conn id %d, cause 0x%x, remote_feats %b", p_data->p_le_remote_feats_info->conn_id, p_data->p_le_remote_feats_info->cause, TRACE_BINARY(8, p_data->p_le_remote_feats_info->remote_feats));
            if (p_data->p_le_remote_feats_info->cause == GAP_SUCCESS) {
                memcpy(remote_feats, p_data->p_le_remote_feats_info->remote_feats, 8);
                if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_2M_MASK_BIT) {
                    if (BTDEBUG) printf("GAP_MSG_LE_REMOTE_FEATS_INFO: support 2M\r\n");
                }
                if (remote_feats[LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1] & LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT) {
                    if (BTDEBUG) printf("GAP_MSG_LE_REMOTE_FEATS_INFO: support CODED\r\n");
                }
            }
            break;
        }
        case GAP_MSG_LE_SCAN_INFO: {
            /*if (BTDEBUG) printf("GAP_MSG_LE_SCAN_INFO:adv_type 0x%x, bd_addr %02x:%02x:%02x:%02x:%02x:%02x, remote_addr_type %d, rssi %d, data_len %d",
                            p_data->p_le_scan_info->adv_type,
                            (p_data->p_le_scan_info->bd_addr)[5],
                            (p_data->p_le_scan_info->bd_addr)[4],
                            (p_data->p_le_scan_info->bd_addr)[3],
                            (p_data->p_le_scan_info->bd_addr)[2],
                            (p_data->p_le_scan_info->bd_addr)[1],
                            (p_data->p_le_scan_info->bd_addr)[0],
                            p_data->p_le_scan_info->remote_addr_type,
                            p_data->p_le_scan_info->rssi,
                            p_data->p_le_scan_info->data_len);*/

            if (_pScanCB != nullptr) {
                _pScanCB(p_data);
            } else {
                _pBLEScan->printScanInfo(p_data);
            }
            break;
        }
        default:
            if (BTDEBUG) printf("gapCallbackDefault: unhandled cb_type 0x%x\r\n", cb_type);
            break;
    }
    return result;
}

void BLEDevice::ioMsgHandlerDefault(T_IO_MSG io_msg) {
    uint16_t msg_type = io_msg.type;

    switch (msg_type) {
        case IO_MSG_TYPE_BT_STATUS: {
            gapMsgHandlerDefault(&io_msg);
            break;
        }
        default:
            if (BTDEBUG) printf("ioMsgHandlerDefault: unhandled\r\n");
            break;
    }
}

void BLEDevice::gapMsgHandlerDefault(T_IO_MSG *p_gap_msg) {
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    if (BTDEBUG) printf("gapMsgHandlerDefault: subtype %d\r\n", p_gap_msg->subtype);
    switch (p_gap_msg->subtype) {
        case GAP_MSG_LE_DEV_STATE_CHANGE: {
            if (_bleState == 1) {
                devStateEvtHandlerPeriphDefault(gap_msg.msg_data.gap_dev_state_change.new_state, gap_msg.msg_data.gap_dev_state_change.cause);
            } else if (_bleState == 2) {
                devStateEvtHandlerCentralDefault(gap_msg.msg_data.gap_dev_state_change.new_state, gap_msg.msg_data.gap_dev_state_change.cause);
            }
            break;
        }
        case GAP_MSG_LE_CONN_STATE_CHANGE: {
            if (_bleState == 1) {
                connStateEvtHandlerPeriphDefault(gap_msg.msg_data.gap_conn_state_change.conn_id, (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state, gap_msg.msg_data.gap_conn_state_change.disc_cause);
            } else if (_bleState == 2) {
                connStateEvtHandlerCentralDefault(gap_msg.msg_data.gap_conn_state_change.conn_id, (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state, gap_msg.msg_data.gap_conn_state_change.disc_cause);
            }
            break;
        }
        case GAP_MSG_LE_CONN_PARAM_UPDATE: {
            connParamUpdateEvtHandlerDefault(gap_msg.msg_data.gap_conn_param_update.conn_id, gap_msg.msg_data.gap_conn_param_update.status, gap_msg.msg_data.gap_conn_param_update.cause);
            break;
        }
        case GAP_MSG_LE_CONN_MTU_INFO: {
            connMtuInfoEvtHandlerDefault(gap_msg.msg_data.gap_conn_mtu_info.conn_id, gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
            break;
        }
        case GAP_MSG_LE_AUTHEN_STATE_CHANGE: {
            authenStateEvtHandlerDefault(gap_msg.msg_data.gap_authen_state.conn_id, gap_msg.msg_data.gap_authen_state.new_state, gap_msg.msg_data.gap_authen_state.status);
            break;
        }
    // cases below relate to bonding requests from devices
    // defaults are set up to accept all bonding requests from all methods
    // add in user functions here to deal with bonding verification
        case GAP_MSG_LE_BOND_PASSKEY_DISPLAY: {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            if (BTDEBUG) printf("GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %ld\r\n", display_value);
            break;
        }
        case GAP_MSG_LE_BOND_PASSKEY_INPUT: {
            uint32_t passkey = 888888;
            conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
            le_bond_passkey_input_confirm(conn_id, passkey, GAP_CFM_CAUSE_ACCEPT);
            if (BTDEBUG) printf("GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d\r\n", conn_id);
            break;
        }
        case GAP_MSG_LE_BOND_OOB_INPUT: {
            uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
            if (BTDEBUG) printf("GAP_MSG_LE_BOND_OOB_INPUT\r\n");
            le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
            le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            break;
        }
        case GAP_MSG_LE_BOND_USER_CONFIRMATION: {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            if (BTDEBUG) printf("GAP_MSG_LE_BOND_USER_CONFIRMATION: passkey %ld\r\n", display_value);
            le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            break;
        }
        case GAP_MSG_LE_BOND_JUST_WORK: {
            conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
            le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            if (BTDEBUG) printf("GAP_MSG_LE_BOND_JUST_WORK\r\n");
            break;
        }
        default:
            if (BTDEBUG) printf("gapMsgHandlerDefault: unknown subtype %d\r\n", p_gap_msg->subtype);
            break;
    }
}

void BLEDevice::devStateEvtHandlerPeriphDefault(T_GAP_DEV_STATE new_state, uint16_t cause) {
    if (BTDEBUG) printf("devStateEvtHandlerPeriphDefault: init state %d, adv state %d, cause 0x%x\r\n", new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (_gapDevState.gap_init_state != new_state.gap_init_state) {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
            if (BTDEBUG) printf("GAP stack ready\n\r");
            // BLE stack is ready
            uint8_t bt_addr[6];
            gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
            printf("[BLE Device] Local BT addr: %2x:%2x:%2x:%2x:%2x:%2x\r\n", bt_addr[5], bt_addr[4], bt_addr[3], bt_addr[2], bt_addr[1], bt_addr[0]);
            le_adv_start();
        }
    }

    if (_gapDevState.gap_adv_state != new_state.gap_adv_state) {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE) {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN) {
                printf("[BLE Device] GAP adv stopped: because connection created\r\n");
            } else {
                printf("[BLE Device] GAP adv stopped\r\n"); 
            }
        } else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING) {
            printf("[BLE Device] GAP adv start\r\n");
        }
    }
    _gapDevState = new_state;
}

void BLEDevice::devStateEvtHandlerCentralDefault(T_GAP_DEV_STATE new_state, uint16_t cause) {
    if (BTDEBUG) printf("devStateEvtHandlerCentralDefault: init state %d, adv state %d, cause 0x%x\r\n", new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (_gapDevState.gap_init_state != new_state.gap_init_state) {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
            if (BTDEBUG) printf("GAP stack ready\n\r");
            // BLE stack is ready
            uint8_t bt_addr[6];
            gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
            printf("[BLE Device] Local BT addr: %2x:%2x:%2x:%2x:%2x:%2x\r\n", bt_addr[5], bt_addr[4], bt_addr[3], bt_addr[2], bt_addr[1], bt_addr[0]);
        }
    }

    if (_gapDevState.gap_scan_state != new_state.gap_scan_state) {
        if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE) {
            printf("[BLE Device] GAP scan stop\r\n");
        }
        else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING) {
            printf("[BLE Device] GAP scan start\r\n");
        }
    }
    _gapDevState = new_state;
}

void BLEDevice::connStateEvtHandlerPeriphDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause) {
    if (BTDEBUG) printf("connStateEvtHandlerPeriphDefault: conn_id %d old_state %d new_state %d, disc_cause 0x%x\r\n", conn_id, _gapConnState, new_state, disc_cause);
    switch (new_state) {
        case GAP_CONN_STATE_DISCONNECTED: {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE)) && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE))) {
                if (BTDEBUG) printf("connStateEvtHandlerPeriphDefault: connection lost cause 0x%x\r\n", disc_cause);
            }
            printf("[BLE Device] BT Disconnected, start ADV\n\r");
            le_adv_start();
            break;
        }
        case GAP_CONN_STATE_CONNECTED: {
            if (BTDEBUG) {
                uint16_t conn_interval;
                uint16_t conn_latency;
                uint16_t conn_supervision_timeout;
                uint8_t  remote_bd[6];
                T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
                le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
                le_get_conn_addr(conn_id, remote_bd, (uint8_t *)&remote_bd_type);
                printf("GAP_CONN_STATE_CONNECTED:remote_bd %x:%x:%x:%x:%x:%x, remote_addr_type %d, conn_interval %d ms, conn_latency %d, conn_supervision_timeout %d ms\r\n", remote_bd[0], remote_bd[1], remote_bd[2], remote_bd[3], remote_bd[4], remote_bd[5], remote_bd_type, (uint16_t)(conn_interval * 1.25), conn_latency, (uint16_t)(conn_supervision_timeout * 10));

                uint8_t tx_phy;
                uint8_t rx_phy;
                le_get_conn_param(GAP_PARAM_CONN_TX_PHY_TYPE, &tx_phy, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_RX_PHY_TYPE, &rx_phy, conn_id);
                printf("GAP_CONN_STATE_CONNECTED: tx_phy %d, rx_phy %d\r\n", tx_phy, rx_phy);
            }
            printf("[BLE Device] BT Connected\n\r");
            break;
        }
        default:
            break;
    }
    _gapConnState = new_state;
}

void BLEDevice::connStateEvtHandlerCentralDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause) {
    if (conn_id >= BLE_CENTRAL_APP_MAX_LINKS) {
        return;
    }
    if (BTDEBUG) printf("connStateEvtHandlerCentralDefault: conn_id %d, conn_state(%d -> %d), disc_cause 0x%x\r\n", conn_id, _bleCentralAppLinkTable[conn_id].conn_state, new_state, disc_cause);

    _bleCentralAppLinkTable[conn_id].conn_state = new_state;
    switch (new_state) {
        case GAP_CONN_STATE_DISCONNECTED: {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE)) && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE))) {
                if (BTDEBUG) printf("connStateEvtHandlerCentralDefault: connection lost, conn_id %d, cause 0x%x\r\n", conn_id, disc_cause);
            }
            printf("[BLE Device] Disconnected conn_id %d\r\n", conn_id);
            memset(&_bleCentralAppLinkTable[conn_id], 0, sizeof(T_APP_LINK));
            break;
        }
        case GAP_CONN_STATE_CONNECTED:{
            le_get_conn_addr(conn_id, _bleCentralAppLinkTable[conn_id].bd_addr, (uint8_t *)&_bleCentralAppLinkTable[conn_id].bd_type);
            printf("[BLE Device] Connected conn_id %d\r\n", conn_id);
            if (BTDEBUG) {
                uint16_t conn_interval;
                uint16_t conn_latency;
                uint16_t conn_supervision_timeout;
                uint8_t  remote_bd[6];
                T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
                le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
                le_get_conn_addr(conn_id, remote_bd, (uint8_t *)&remote_bd_type);
                printf("GAP_CONN_STATE_CONNECTED:remote_bd %x:%x:%x:%x:%x:%x, remote_addr_type %d, conn_interval %d ms, conn_latency %d, conn_supervision_timeout %d ms\r\n", remote_bd[0], remote_bd[1], remote_bd[2], remote_bd[3], remote_bd[4], remote_bd[5], remote_bd_type, (uint16_t)(conn_interval * 1.25), conn_latency, (uint16_t)(conn_supervision_timeout * 10));

                uint8_t tx_phy;
                uint8_t rx_phy;
                le_get_conn_param(GAP_PARAM_CONN_TX_PHY_TYPE, &tx_phy, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_RX_PHY_TYPE, &rx_phy, conn_id);
                printf("GAP_CONN_STATE_CONNECTED: tx_phy %d, rx_phy %d\r\n", tx_phy, rx_phy);
            }
            le_set_phy(conn_id, all_phys, tx_phys, rx_phys, phy_options);
            break;
        }
        default:
            break;
    }
    _gapConnState = new_state;
}

void BLEDevice::connMtuInfoEvtHandlerDefault(uint8_t conn_id, uint16_t mtu_size) {
    if (BTDEBUG) printf("connMtuInfoEvtHandlerDefault: conn_id %d, mtu_size %d\r\n", conn_id, mtu_size);
}

void BLEDevice::connParamUpdateEvtHandlerDefault(uint8_t conn_id, uint8_t status, uint16_t cause) {
    switch (status) {
        case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS: {
            uint16_t conn_interval;
            uint16_t conn_slave_latency;
            uint16_t conn_supervision_timeout;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            if (BTDEBUG) printf("connParamUpdateEvtHandlerDefault update success:conn_interval %d ms, conn_slave_latency %d, conn_supervision_timeout %d ms\r\n", (uint16_t)(conn_interval * 1.25), conn_slave_latency, (uint16_t)(conn_supervision_timeout * 10));
            break;
        }
        case GAP_CONN_PARAM_UPDATE_STATUS_FAIL: {
            if (BTDEBUG) printf("connParamUpdateEvtHandlerDefault update failed: cause 0x%x\r\n", cause);
            break;
        }
        case GAP_CONN_PARAM_UPDATE_STATUS_PENDING: {
            if (BTDEBUG) printf("connParamUpdateEvtHandlerDefault update pending: conn_id %d\r\n", conn_id);
            break;
        }
        default:
            break;
    }
}

void BLEDevice::authenStateEvtHandlerDefault(uint8_t conn_id, uint8_t new_state, uint16_t cause) {
    if (BTDEBUG) printf("authenStateEvtHandlerDefault:conn_id %d, cause 0x%x\r\n", conn_id, cause);
    switch (new_state) {
        case GAP_AUTHEN_STATE_STARTED: {
            if (BTDEBUG) printf("authenStateEvtHandlerDefault: GAP_AUTHEN_STATE_STARTED\r\n");
            break;
        }
        case GAP_AUTHEN_STATE_COMPLETE: {
            if (cause == GAP_SUCCESS) {
                if (BTDEBUG) printf("authenStateEvtHandlerDefault: Pair success conn id :%d\r\n", conn_id);
            } else {
                if (BTDEBUG) printf("authenStateEvtHandlerDefault: Pair failed: cause 0x%x\r\n", cause);
            }
            break;
        }
        default:
            if (BTDEBUG) printf("authenStateEvtHandlerDefault: unknown newstate %d\r\n", new_state);
            break;
    }
}

//--------------------------------------------------------- Default callbacks for services ---------------------------------------------------------//

T_APP_RESULT BLEDevice::appServiceCallbackDefault(T_SERVER_ID service_id, void *p_data) {
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID) {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId) {
            case PROFILE_EVT_SRV_REG_COMPLETE:// srv register result event.
                if (BTDEBUG) printf("PROFILE_EVT_SRV_REG_COMPLETE: result %d\r\n", p_param->event_data.service_reg_result);
                break;

            case PROFILE_EVT_SEND_DATA_COMPLETE:
                if (BTDEBUG) printf("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits %d\r\n",
                                p_param->event_data.send_data_result.conn_id,
                                p_param->event_data.send_data_result.cause,
                                p_param->event_data.send_data_result.service_id,
                                p_param->event_data.send_data_result.attrib_idx,
                                p_param->event_data.send_data_result.credits);
                if (p_param->event_data.send_data_result.cause == GAP_SUCCESS) {
                    if (BTDEBUG) printf("PROFILE_EVT_SEND_DATA_COMPLETE success\r\n");
                } else {
                    if (BTDEBUG) printf("PROFILE_EVT_SEND_DATA_COMPLETE failed\r\n");
                }
                break;

            default:
                break;
        }
    }
    return result;
}

T_APP_RESULT BLEDevice::serviceAttrReadCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                        uint16_t offset, uint16_t *p_length, uint8_t **pp_value) {
    T_APP_RESULT cause  = APP_RESULT_ATTR_NOT_FOUND;
    uint8_t i;
    for (i = 0; i < _serviceCount; i++) {
        if ((_servicePtrList[i]->getServiceID()) == service_id) {
            cause = _servicePtrList[i]->serviceAttrReadCallbackDefault(conn_id, service_id, attrib_index, offset, p_length, pp_value);
            break;
        }
    }
    return (cause);
}

T_APP_RESULT BLEDevice::serviceAttrWriteCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                        T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                                        P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc) {
    T_APP_RESULT cause = APP_RESULT_ATTR_NOT_FOUND;
    uint8_t i;
    for (i = 0; i < _serviceCount; i++) {
        if ((_servicePtrList[i]->getServiceID()) == service_id) {
            cause = _servicePtrList[i]->serviceAttrWriteCallbackDefault(conn_id, service_id, attrib_index, write_type, length, p_value, p_write_ind_post_proc);
            break;
        }
    }
    return cause;
}

void BLEDevice::serviceCccdUpdateCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t ccc_bits) {
    uint8_t i;
    for (i = 0; i < _serviceCount; i++) {
        if ((_servicePtrList[i]->getServiceID()) == service_id) {
            _servicePtrList[i]->serviceCccdUpdateCallbackDefault(conn_id, service_id, attrib_index, ccc_bits);
            break;
        }
    }
}

//--------------------------------------------------------- Default callbacks for clients ---------------------------------------------------------//

T_APP_RESULT BLEDevice::appClientCallbackDefault(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data) {
    T_APP_RESULT  result = APP_RESULT_SUCCESS;
    if (BTDEBUG) printf("app_client_callback: client_id %d, conn_id %d", client_id, conn_id);
    if (client_id == CLIENT_PROFILE_GENERAL_ID) {
        T_CLIENT_APP_CB_DATA *p_client_app_cb_data = (T_CLIENT_APP_CB_DATA *)p_data;
        switch (p_client_app_cb_data->cb_type) {
            case CLIENT_APP_CB_TYPE_DISC_STATE:
                if (p_client_app_cb_data->cb_content.disc_state_data.disc_state == DISC_STATE_SRV_DONE) {
                    if (BTDEBUG) printf("Discovery All Service Procedure Done.");
                } else {
                    if (BTDEBUG) printf("Discovery state send to application directly.");
                }
                break;

            case CLIENT_APP_CB_TYPE_DISC_RESULT:
                if (p_client_app_cb_data->cb_content.disc_result_data.result_type == DISC_RESULT_ALL_SRV_UUID16) {
                    if (BTDEBUG) printf("Discovery All Primary Service: UUID16 0x%x, start handle 0x%x, end handle 0x%x.",
                                    p_client_app_cb_data->cb_content.disc_result_data.result_data.p_srv_uuid16_disc_data->uuid16,
                                    p_client_app_cb_data->cb_content.disc_result_data.result_data.p_srv_uuid16_disc_data->att_handle,
                                    p_client_app_cb_data->cb_content.disc_result_data.result_data.p_srv_uuid16_disc_data->end_group_handle);
                } else {
                    if (BTDEBUG) printf("Discovery result send to application directly.");
                }
                break;

            default:
                break;
        }
    }
    return result;
}

void BLEDevice::clientDiscoverStateCallbackDefault(uint8_t conn_id, T_DISCOVERY_STATE discovery_state) {
    if (_clientPtrList[conn_id] == nullptr) {
        return;
    }
    _clientPtrList[conn_id]->clientDiscoverStateCallbackDefault(conn_id, discovery_state);
}

void BLEDevice::clientDiscoverResultCallbackDefault(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type, T_DISCOVERY_RESULT_DATA result_data) {
    if (_clientPtrList[conn_id] == nullptr) {
        return;
    }
    _clientPtrList[conn_id]->clientDiscoverResultCallbackDefault(conn_id, result_type, result_data);
}

void BLEDevice::clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    if (_clientPtrList[conn_id] == nullptr) {
        return;
    }
    _clientPtrList[conn_id]->clientReadResultCallbackDefault(conn_id, cause, handle, value_size, p_value);
}

void BLEDevice::clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits) {
    if (_clientPtrList[conn_id] == nullptr) {
        return;
    }
    _clientPtrList[conn_id]->clientWriteResultCallbackDefault(conn_id, type, handle, cause, credits);
}

T_APP_RESULT BLEDevice::clientNotifyIndicateCallbackDefault(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    T_APP_RESULT app_result = APP_RESULT_APP_ERR;
    if (_clientPtrList[conn_id] == nullptr) {
        return app_result;
    }
    return (_clientPtrList[conn_id]->clientNotifyIndicateCallbackDefault(conn_id, notify, handle, value_size, p_value));
}

void BLEDevice::clientDisconnectCallbackDefault(uint8_t conn_id) {
    if (_clientPtrList[conn_id] == nullptr) {
        return;
    }
    _clientPtrList[conn_id]->clientDisconnectCallbackDefault(conn_id);
    delete _clientPtrList[conn_id];
    _clientPtrList[conn_id] = nullptr;
}

