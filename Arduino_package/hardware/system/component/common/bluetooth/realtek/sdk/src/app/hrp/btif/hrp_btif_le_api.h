#ifndef  _HRP_BTIF_LE_API_H_
#define  _HRP_BTIF_LE_API_H_

#include <hrp_utils.h>
#include <hrp.h>
#include <hrp_btif_entry.h>

typedef enum
{
    BTIF_ApplicationDefined = 0,   /**< application provides services */
    BTIF_ServiceDIS,                      /**< Device Information service    */
    BTIF_ServiceGLS,                      /**< Glucose service               */
    BTIF_ServiceBAS,                      /**< Battery service               */
    BTIF_ServiceCTS,                      /**< Current Time service          */
    BTIF_ServiceNDCS,                     /**< Next DST Change service       */
    BTIF_ServiceRTUS,                     /**< Reference Time Update service */
    BTIF_ServiceBLS,                      /**< Blood Pressure service        */
    BTIF_ServiceHRS,                      /**< Heart Rate service            */

    /* the following services are not (yet) available as built-in services:   */
    BTIF_ServiceCGM,                      /**< Continous Glucose Monitor service */
    BTIF_ServiceBMS,                      /**< Bond Management service       */
    BTIF_ServiceQUL,                      /**< Qualification test service    */
    BTIF_ServiceTST                       /**< Test service                  */
} GATT_SRV_ID;

void *hrp_gatt_srv_get(GATT_SRV_ID serviceID, uint16_t *pNbrOfAttrib);
typedef struct
{
    uint8_t                 bd_addr[6];
    uint8_t                 data_c[16];
    T_BTIF_CAUSE            cause;
    T_BTIF_REMOTE_ADDR_TYPE addr_type;
} T_BTIF_REMOTE_OOB_REQ_CFM;
typedef struct
{
    uint16_t    num_attr;
    void       *p_srv;
} T_BTIF_GATT_SRV_REG_REQ;

typedef struct
{
    void                   *p_srv_handle;
    void                   *p_req_handle;
    T_BTIF_GATT_PDU_TYPE    type;
    uint16_t                link_id;
    uint16_t                attr_index;
    uint16_t                attr_len;
    uint16_t                gap;
    uint8_t                 data[1];
} T_BTIF_GATT_ATTR_UPDATE_REQ;

typedef struct
{
    void          *p_srv_handle;
    void          *p_req_handle;
    uint16_t       link_id;
    uint16_t       attr_index;
} T_BTIF_GATT_ATTR_UPDATE_STATUS_CFM;

typedef struct
{
    void          *p_srv_handle;
    uint16_t       link_id;
    uint16_t       cause;
    uint16_t       attr_index;
    uint16_t       attr_len;
    uint16_t       gap;
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_READ_CFM;

typedef struct
{
    uint16_t       link_id;
    void          *p_srv_handle;
    uint16_t       cause;
    uint16_t       attr_index;
} T_BTIF_GATT_ATTR_WRITE_REQ_CFM;

typedef struct
{
    void          *p_srv_handle;
    uint16_t       link_id;
    uint16_t       cause;
    uint16_t       attr_index;
    uint16_t       attr_len;
    uint16_t       gap;
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_PREP_WRITE_CFM;

typedef struct
{
    uint16_t    link_id;
    uint16_t    cause;
    uint16_t    handle;
} T_BTIF_GATT_ATTR_EXEC_WRITE_CFM;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    start_handle;
    uint16_t                    end_handle;
    uint16_t                    uuid16;
    uint8_t                     uuid128[16];
} T_BTIF_GATT_DISCOVERY_REQ;

typedef struct
{
    uint16_t                    link_id;
    T_BTIF_GATT_DISCOVERY_TYPE  type;
    uint16_t                    start_handle;
    uint16_t                    end_handle;
} T_BTIF_GATT_DISCOVERY_CFM;

typedef struct
{
    uint16_t                link_id;
    T_BTIF_GATT_READ_TYPE   read_type;
    uint16_t                read_offset;
    uint16_t                start_handle;
    uint16_t                end_handle;
    uint16_t                uuid16;
    uint8_t                 uuid128[16];
} T_BTIF_GATT_ATTR_READ_REQ;

typedef struct
{
    uint16_t       link_id;
    uint16_t       num_handle;
    uint16_t       handles[11];
} T_BTIF_GATT_ATTR_READ_MULTI_REQ;

typedef struct
{
    uint16_t       link_id;
    uint8_t        write_type;
    uint16_t       attr_handle;
    uint16_t       attr_len;        /**< attrib. value length      */
    uint16_t       write_offset;    /**< write offset in attribute */
    uint16_t       gap;             /**< offset of attrib. value in data[] */
    uint8_t        data[1];
} T_BTIF_GATT_ATTR_WRITE_REQ;
typedef struct
{
    uint16_t    link_id;
    uint8_t     flags;
} T_BTIF_GATT_ATTR_EXEC_WRITE_REQ;

typedef struct
{
    uint16_t       link_id;
} T_BTIF_GATT_ATTR_CFM;

typedef struct
{
    uint16_t       link_id;
    uint16_t       requirements;
    uint8_t        min_key_size;
} T_BTIF_GATT_SECURITY_REQ;

typedef struct
{
    T_BTIF_GATT_STORE_OPCODE    op;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    uint8_t                     data_len;
    uint8_t                    *p_data;
    T_BTIF_CAUSE                cause;
} T_BTIF_GATT_SERVER_STORE_CFM;

typedef struct
{
    bool                        use_ext_conn;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_LOCAL_ADDR_TYPE      local_addr_type;
    uint8_t                     init_phys;
    uint16_t                    scan_timeout;
    T_BTIF_LE_CONN_REQ_PARAM    conn_req_param[BTIF_LE_MAX_INIT_PHYS_NUM];
} T_BTIF_LE_CONN_REQ;

typedef struct
{
    uint8_t         bd_addr[6];
    T_BTIF_CAUSE    cause;
}  T_BTIF_LE_CONN_CFM;

typedef struct
{
    uint16_t        link_id;
    T_BTIF_CAUSE    cause;
}  T_BTIF_LE_DISCONN_REQ;

typedef struct
{
    uint16_t        link_id;
}  T_BTIF_LE_DISCONN_CFM;

typedef struct
{
    T_BTIF_LE_ADV_MODE      adv_mode;
} T_BTIF_LE_ADV_REQ;

typedef struct
{
    T_BTIF_LE_ADV_TYPE          adv_type;
    T_BTIF_LE_ADV_FILTER_POLICY filter_policy;
    uint8_t                     chann_map;
    uint16_t                    min_interval;
    uint16_t                    max_interval;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_LOCAL_ADDR_TYPE      local_addr_type;
} T_BTIF_LE_ADV_PARAM_SET_REQ;

typedef struct
{
    T_BTIF_LE_ADV_DATA_TYPE data_type;
    uint8_t                 data_len;
    uint8_t                *p_data;
} T_BTIF_LE_ADV_DATA_SET_REQ;

typedef struct
{
    T_BTIF_LE_SCAN_MODE mode;
    T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates;
} T_BTIF_LE_SCAN_REQ;

typedef struct
{
    T_BTIF_LE_SCAN_TYPE          type;
    uint16_t                     interval;
    uint16_t                     window;
    T_BTIF_LE_SCAN_FILTER_POLICY filter_policy;
    T_BTIF_LOCAL_ADDR_TYPE       local_addr_type;
} T_BTIF_LE_SCAN_PARAM_SET_REQ;

typedef struct
{
    T_BTIF_LE_WHITE_LIST_OP     operation;
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
} T_BTIF_LE_MODIFY_WHITE_LIST_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
    uint16_t        min_ce_len;
    uint16_t        max_ce_len;
} T_BTIF_LE_CONN_UPDATE_REQ;

typedef struct
{
    uint16_t        link_id;
    T_BTIF_CAUSE    cause;
} T_BTIF_LE_CONN_UPDATE_CFM;

typedef struct
{
    uint16_t        link_id;
    uint16_t        min_interval;
    uint16_t        max_interval;
    uint16_t        latency;
    uint16_t        supv_tout;
    uint16_t        min_ce_len;
    uint16_t        max_ce_len;
} T_BTIF_LE_CONN_PARAM_REQ_REPLY_REQ;

typedef struct
{
    uint16_t        link_id;
    uint8_t         reason;
} T_BTIF_LE_CONN_PARAM_REQ_NEG_REPLY_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        le_psm;
    uint16_t        mtu;
    uint16_t        init_credits;
    uint16_t        credits_threshold;
} T_BTIF_LE_CREDIT_BASED_CONN_REQ;

typedef struct
{
    uint16_t                    link_id;
    uint16_t                    channel;
    uint16_t                    mtu;
    uint16_t                    init_credits;
    uint16_t                    credits_threshold;
    T_BTIF_L2C_LE_CONN_STATUS   cause;
} T_BTIF_LE_CREDIT_BASED_CONN_CFM;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_DISCONN_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
} T_BTIF_LE_CREDIT_BASED_DISCONN_CFM;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        credits;
} T_BTIF_LE_SEND_CREDIT_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    uint16_t        len;
    uint16_t        gap;
    uint8_t         data[1];
} T_BTIF_LE_CREDIT_BASED_DATA_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        channel;
    T_BTIF_CAUSE    cause;
} T_BTIF_LE_CREDIT_BASED_DATA_CFM;

typedef struct
{
    uint16_t                    le_psm;
    bool                        active;
    T_BTIF_LE_SECURITY_MODE     mode;
    uint8_t                     key_size;
} T_BTIF_LE_CREDIT_BASED_SECURITY_REG_REQ;

typedef struct
{
    uint16_t        le_psm;
    uint8_t         action;
} T_BTIF_LE_CREDIT_BASED_PSM_REG_REQ;

typedef struct
{
    uint8_t         channel_map[5];
} T_BTIF_LE_SET_CHANN_CLASSIF_REQ;

typedef struct
{
    uint16_t        link_id;
} T_BTIF_LE_READ_CHANN_MAP_REQ;

typedef struct
{
    uint8_t         rx_chann;
} T_BTIF_LE_RECEIVER_TEST_REQ;

typedef struct
{
    uint8_t         tx_chann;
    uint8_t         data_len;
    uint8_t         pkt_pl;
} T_BTIF_LE_TRANSMITTER_TEST_REQ;

typedef struct
{
    uint8_t          random_addr[6];
} T_BTIF_LE_SET_RAND_ADDR_REQ;

typedef struct
{
    uint16_t        tx_oct;
    uint16_t        tx_time;
} T_BTIF_LE_WRITE_DEFAULT_DATA_LEN_REQ;

typedef struct
{
    uint16_t        link_id;
    uint16_t        tx_oct;
    uint16_t        tx_time;
} T_BTIF_LE_SET_DATA_LEN_REQ;

typedef struct
{
    T_BTIF_LE_RESOLVING_LIST_OP operation;
    T_BTIF_IDENT_ADDR_TYPE      peer_ident_addr_type;
    uint8_t                     peer_ident_addr[6];
    uint8_t                     peer_irk[16];
    uint8_t                     local_irk[16];
} T_BTIF_LE_MODIFY_RESOLV_LIST_REQ;

typedef struct
{
    T_BTIF_IDENT_ADDR_TYPE  peer_ident_addr_type;
    uint8_t                 peer_ident_addr[6];
} T_BTIF_LE_READ_PEER_RESOLV_ADDR_REQ;

typedef struct
{
    T_BTIF_IDENT_ADDR_TYPE  peer_ident_addr_type;
    uint8_t                 peer_ident_addr[6];
} T_BTIF_LE_READ_LOCAL_RESOLV_ADDR_REQ;

typedef struct
{
    uint8_t     enable;
} T_BTIF_LE_SET_RESOLUTION_ENABLE_REQ;

typedef struct
{
    uint16_t    timeout;
} T_BTIF_LE_SET_RESOLV_PRIV_ADDR_TOUT_REQ;

typedef struct
{
    bool        generate;
    uint8_t     local_irk[16];
} T_BTIF_LE_CONFIG_LOCAL_IRK_REQ;

typedef struct
{
    T_BTIF_IDENT_ADDR_TYPE  peer_ident_addr_type;
    uint8_t                 peer_ident_addr[6];
    T_BTIF_LE_PRIVACY_MODE  privacy_mode;
} T_BTIF_LE_SET_PRIVACY_MODE_REQ;

typedef struct
{
    uint8_t          adv_handle;
    uint8_t          rand_addr[6];
} T_BTIF_LE_SET_ADV_SET_RAND_ADDR_REQ;

typedef struct
{
    uint8_t                         adv_handle;
    uint16_t                        adv_event_prop;
    uint32_t                        prim_adv_interval_min;
    uint32_t                        prim_adv_interval_max;
    uint8_t                         prim_adv_chann_map;
    T_BTIF_LOCAL_ADDR_TYPE          own_addr_type;
    T_BTIF_REMOTE_ADDR_TYPE         peer_addr_type;
    uint8_t                         peer_addr[6];
    T_BTIF_LE_ADV_FILTER_POLICY     filter_policy;
    uint8_t                         tx_power;
    T_BTIF_LE_PRIM_ADV_PHY_TYPE     prim_adv_phy;
    uint8_t                         secondary_adv_max_skip;
    T_BTIF_LE_PHY_TYPE              secondary_adv_phy;
    uint8_t                         adv_sid;
    T_BTIF_LE_SCAN_REQ_NOTIFY_TYPE  scan_req_notif_enable;
} T_BTIF_LE_EXT_ADV_PARAM_SET_REQ;

typedef struct
{
    T_BTIF_LE_EXT_ADV_DATA_TYPE         data_type;
    uint8_t                             adv_handle;
    T_BTIF_LE_ADV_FRAG_OP_TYPE          op;
    T_BTIF_LE_ADV_FRAG_PREFERENCE_TYPE  frag_preference;
    uint8_t                             len;
    uint8_t                            *p_data;
} T_BTIF_LE_EXT_ADV_DATA_SET_REQ;

typedef struct
{
    T_BTIF_LE_EXT_ADV_MODE          mode;
    uint8_t                         num_of_sets;
    T_BTIF_LE_EXT_ADV_SET_PARAM     adv_set_param[BTIF_LE_MAX_EXTENDED_ADV_SETS_NUM];
} T_BTIF_LE_EXT_ADV_ENABLE_REQ;

typedef struct
{
    T_BTIF_LOCAL_ADDR_TYPE          local_addr_type;
    T_BTIF_LE_SCAN_FILTER_POLICY    filter_policy;
    uint8_t                         scan_phys;
    T_BTIF_LE_EXT_SCAN_PARAM        scan_param[BTIF_LE_MAX_EXTENDED_SCAN_PHY_NUM];
} T_BTIF_LE_EXT_SCAN_PARAM_SET_REQ;

typedef struct
{
    T_BTIF_LE_SCAN_MODE                 mode;
    T_BTIF_LE_SCAN_FILTER_DUPLICATES    filter_duplicates;
    uint16_t                            duration;
    uint16_t                            period;
} T_BTIF_LE_EXT_SCAN_REQ;

typedef struct
{
    uint8_t         all_phys;
    uint8_t         tx_phys;
    uint8_t         rx_phys;
} T_BTIF_LE_SET_DEFAULT_PHY_REQ;

typedef struct
{
    uint16_t                link_id;
    uint8_t                 all_phys;
    uint8_t                 tx_phys;
    uint8_t                 rx_phys;
    T_BTIF_LE_PHY_OPTIONS   phy_options;
} T_BTIF_LE_SET_PHY_REQ;

typedef struct
{
    uint8_t         rx_chann;
    uint8_t         phy;
    uint8_t         modulation_index;
} T_BTIF_LE_ENHANCED_RECEIVER_TEST_REQ;

typedef struct
{
    uint8_t         tx_chann;
    uint8_t         data_len;
    uint8_t         pkt_pl;
    uint8_t         phy;
} T_BTIF_LE_ENHANCED_TRANSMITTER_TEST_REQ;

typedef struct
{
    T_BTIF_LE_PERIODIC_ADV_LIST_OP  op;
    uint8_t                         advertiser_addr_type;
    uint8_t                         advertiser_addr[6];
    uint8_t                         adv_sid;
} T_BTIF_LE_MODIFY_PERIODIC_ADV_LIST_REQ;

typedef struct
{
    int8_t      rf_tx_path_compensation;
    int8_t      rf_rx_path_compensation;
} T_BTIF_LE_WRITE_RF_PATH_COMPENSATION_REQ;

typedef struct
{
    T_BTIF_LE_ADV_SET_OP     operation;
    uint8_t                  adv_handle;
} T_BTIF_LE_MODIFY_ADV_SET_REQ;

typedef struct
{
    uint8_t     adv_handle;
    uint16_t    adv_interval_min;
    uint16_t    adv_interval_max;
    uint16_t    adv_prop;
} T_BTIF_LE_SET_PERIODIC_ADV_PARAM_REQ;

typedef struct
{
    uint8_t                     adv_handle;
    T_BTIF_LE_ADV_FRAG_OP_TYPE  op;
    uint8_t                     data_len;
    uint8_t                    *p_data;
} T_BTIF_LE_SET_PERIODIC_ADV_DATA_REQ;

typedef struct
{
    T_BTIF_LE_PERIODIC_ADV_MODE mode;
    uint8_t                     adv_handle;
} T_BTIF_LE_SET_PERIODIC_ADV_ENABLE_REQ;

typedef struct
{
    uint8_t    filter_policy;
    uint8_t    adv_sid;
    uint8_t    adv_addr_type;
    uint8_t    adv_add[6];
    uint16_t   skip;
    uint16_t   sync_timeout;
    uint16_t   unused;
} T_BTIF_LE_PERIODIC_ADV_CREATE_SYNC_REQ;

typedef struct
{
    uint16_t    sync_handle;
} T_BTIF_LE_PERIODIC_ADV_TERMINATE_SYNC_REQ;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_JUST_WORK_REQ_CFM;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_BTIF_REMOTE_ADDR_TYPE     remote_addr_type;
    T_BTIF_CAUSE                cause;
} T_BTIF_PASSKEY_NOTIF_CFM;

void hrp_remote_oob_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_srv_reg_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_update_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_update_status_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_read_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_write_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_prep_write_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_exec_write_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_discovery_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_discovery_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_read_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_read_multi_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_write_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_prep_write_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_exec_write_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_attr_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_security_req(uint16_t len, uint8_t *p_param_list);

void hrp_gatt_server_store_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_disconn_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_disconn_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_adv_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_adv_param_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_adv_data_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_scan_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_scan_param_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_modify_white_list_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_update_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_update_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_param_req_reply_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_conn_param_req_neg_reply_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_conn_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_conn_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_disconn_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_disconn_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_send_credit_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_data_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_data_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_security_reg_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_credit_based_psm_reg_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_chann_classif_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_chann_map_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_receiver_test_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_transmitter_test_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_test_end_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_adv_tx_power_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_rand_addr_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_max_data_len_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_default_data_len_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_write_default_data_len_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_data_len_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_modify_resolv_list_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_peer_resolv_addr_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_read_local_resolv_addr_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_resolution_enable_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_resolv_priv_addr_tout_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_config_local_irk_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_privacy_mode_req(uint16_t len, uint8_t *p_param_list);
#if F_BT_LE_5_0_AE_ADV_SUPPORT
void hrp_le_set_adv_set_rand_addr_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_ext_adv_param_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_ext_adv_data_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_ext_adv_enable_req(uint16_t len, uint8_t *p_param_list);
#endif
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
void hrp_le_ext_scan_param_set_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_ext_scan_req(uint16_t len, uint8_t *p_param_list);
#endif
void hrp_le_set_default_phy_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_phy_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_enhanced_receiver_test_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_enhanced_transmitter_test_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_modify_periodic_adv_list_req(uint16_t len, uint8_t *p_param_list);
#if F_BT_LE_5_0_RF_PATH_SUPPORT
void hrp_le_read_rf_path_compensation_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_write_rf_path_compensation_req(uint16_t len, uint8_t *p_param_list);
#endif
#if F_BT_LE_5_0_AE_ADV_SUPPORT
void hrp_le_modify_adv_set_req(uint16_t len, uint8_t *p_param_list);
#endif
void hrp_le_set_periodic_adv_param_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_periodic_adv_data_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_set_periodic_adv_enable_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_periodic_adv_create_sync_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_periodic_adv_create_sync_cancel_req(uint16_t len, uint8_t *p_param_list);

void hrp_le_periodic_adv_terminate_sync_req(uint16_t len, uint8_t *p_param_list);
#if F_BT_LE_5_0_AE_ADV_SUPPORT
void hrp_le_enable_ext_adv_mode_req(uint16_t len, uint8_t *p_param_list);
#endif
void hrp_just_work_req_cfm(uint16_t len, uint8_t *p_param_list);

void hrp_user_passkey_notif_cfm(uint16_t len, uint8_t *p_param_list);
#endif
