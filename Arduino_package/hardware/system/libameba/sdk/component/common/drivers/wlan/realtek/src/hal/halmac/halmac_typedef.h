/******************************************************************************
 *
 * Copyright(c) 2017 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 ******************************************************************************/

#ifndef _HALMAC_TYPEDEF_H_
#define _HALMAC_TYPEDEF_H_

typedef enum halmac_network_type_select HALMAC_NETWORK_TYPE_SELECT, _HALMAC_NETWORK_TYPE_SELECT;
typedef enum halmac_acq_id HALMAC_ACQ_ID, _HALMAC_ACQ_ID;
typedef enum halmac_interface HALMAC_INTERFACE, _HALMAC_INTERFACE;
typedef enum halmac_mac_power HALMAC_MAC_POWER, _HALMAC_MAC_POWER;
typedef enum halmac_trx_mode HALMAC_TRX_MODE, _HALMAC_TRX_MODE;
typedef enum halmac_wireless_mode HALMAC_WIRELESS_MODE, _HALMAC_WIRELESS_MODE;
typedef enum halmac_bw HALMAC_BW, _HALMAC_BW;
typedef enum halmac_efuse_read_cfg HALMAC_EFUSE_READ_CFG, _HALMAC_EFUSE_READ_CFG;
typedef enum halmac_dlfw_mem HALMAC_DLFW_MEM, _HALMAC_DLFW_MEM;
typedef enum halmac_dma_mapping HALMAC_DMA_MAPPING, _HALMAC_DMA_MAPPING;
typedef enum hal_fifo_sel HAL_FIFO_SEL, _HAL_FIFO_SEL;
typedef enum halmac_drv_info HALMAC_DRV_INFO, _HALMAC_DRV_INFO;
typedef enum halmac_pri_ch_idx HALMAC_PRI_CH_IDX, _HALMAC_PRI_CH_IDX;
typedef enum halmac_data_rate HALMAC_DATA_RATE, _HALMAC_DATA_RATE;
typedef enum halmac_rf_path HALMAC_RF_PATH, _HALMAC_RF_PATH;
typedef enum hal_security_type HAL_SECURITY_TYPE, _HAL_SECURITY_TYPE;
typedef enum halmac_rf_type HALMAC_RF_TYPE, _HALMAC_RF_TYPE;
typedef enum halmac_cmd_process_status HALMAC_CMD_PROCESS_STATUS, _HALMAC_CMD_PROCESS_STATUS;
typedef enum halmac_feature_id HALMAC_FEATURE_ID, _HALMAC_FEATURE_ID;
typedef enum halmac_drv_rsvd_pg_num HALMAC_DRV_RSVD_PG_NUM, _HALMAC_DRV_RSVD_PG_NUM;
typedef enum halmac_pcie_cfg HALMAC_PCIE_CFG, _HALMAC_PCIE_CFG;
typedef enum halmac_snd_role HALMAC_SND_ROLE, _HALMAC_SND_ROLE;
typedef enum halmac_api_id HALMAC_API_ID, _HALMAC_API_ID;
typedef enum halmac_la_mode HALMAC_LA_MODE, _HALMAC_LA_MODE;
typedef enum halmac_rx_fifo_expanding_mode HALMAC_RX_FIFO_EXPANDING_MODE, _HALMAC_RX_FIFO_EXPANDING_MODE;
typedef enum halmac_sdio_cmd53_4byte_mode HALMAC_SDIO_CMD53_4BYTE_MODE, _HALMAC_SDIO_CMD53_4BYTE_MODE;
typedef enum halmac_hw_id HALMAC_HW_ID, _HALMAC_HW_ID;
typedef enum halmac_efuse_bank HALMAC_EFUSE_BANK, _HALMAC_EFUSE_BANK;
typedef enum halmac_sdio_tx_format HALMAC_SDIO_TX_FORMAT, _HALMAC_SDIO_TX_FORMAT;
typedef enum halmac_wlled_mode HALMAC_WLLED_MODE, _HALMAC_WLLED_MODE;
typedef enum halmac_usb_mode HALMAC_USB_MODE, _HALMAC_USB_MODE;
typedef enum halmac_ret_status HALMAC_RET_STATUS, _HALMAC_RET_STATUS;
typedef enum halmac_data_type HALMAC_DATA_TYPE, _HALMAC_DATA_TYPE;
typedef enum halmac_packet_id HALMAC_PACKET_ID, _HALMAC_PACKET_ID;
typedef enum halmac_cs_action_id HALMAC_CS_ACTION_ID, _HALMAC_CS_ACTION_ID;
typedef enum halmac_intf_phy_platform HALMAC_INTF_PHY_PLATFORM, _HALMAC_INTF_PHY_PLATFORM;
typedef enum halmac_parameter_cmd HALMAC_PARAMETER_CMD, _HALMAC_PARAMETER_CMD;

typedef struct halmac_rxagg_cfg HALMAC_RXAGG_CFG, _HALMAC_RXAGG_CFG, *PHALMAC_RXAGG_CFG;
typedef struct halmac_api_registry HALMAC_API_REGISTRY, _HALMAC_API_REGISTRY, *PHALMAC_API_REGISTRY;
typedef struct halmac_bcn_ie_info HALMAC_BCN_IE_INFO, _HALMAC_BCN_IE_INFO, *PHALMAC_BCN_IE_INFO;
typedef struct halmac_phy_parameter_info HALMAC_PHY_PARAMETER_INFO, _HALMAC_PHY_PARAMETER_INFO, *PHALMAC_PHY_PARAMETER_INFO;
typedef struct halmac_pg_efuse_info HALMAC_PG_EFUSE_INFO, _HALMAC_PG_EFUSE_INFO, *PHALMAC_PG_EFUSE_INFO;
typedef struct halmac_ch_info HALMAC_CH_INFO, _HALMAC_CH_INFO, *PHALMAC_CH_INFO;
typedef struct halmac_ch_extra_info HALMAC_CH_EXTRA_INFO, _HALMAC_CH_EXTRA_INFO, *PHALMAC_CH_EXTRA_INFO;
typedef struct halmac_ch_switch_option HALMAC_CH_SWITCH_OPTION, _HALMAC_CH_SWITCH_OPTION, *PHALMAC_CH_SWITCH_OPTION;
typedef struct halmac_p2pps HALMAC_P2PPS, _HALMAC_P2PPS, *PHALMAC_P2PPS;
typedef struct halmac_fw_version HALMAC_FW_VERSION, _HALMAC_FW_VERSION, *PHALMAC_FW_VERSION;
typedef struct halmac_general_info HALMAC_GENERAL_INFO, _HALMAC_GENERAL_INFO, *PHALMAC_GENERAL_INFO;
typedef struct halmac_pwr_tracking_option HALMAC_PWR_TRACKING_OPTION, _HALMAC_PWR_TRACKING_OPTION, *PHALMAC_PWR_TRACKING_OPTION;
typedef struct halmac_bcn_ctrl HALMAC_BCN_CTRL, _HALMAC_BCN_CTRL, *PHALMAC_BCN_CTRL;
typedef struct halmac_platform_api HALMAC_PLATFORM_API, _HALMAC_PLATFORM_API, *PHALMAC_PLATFORM_API;
typedef union halmac_wlan_addr HALMAC_WLAN_ADDR, _HALMAC_WLAN_ADDR, *PHALMAC_WLAN_ADDR;
typedef struct halmac_cfg_mumimo_para HALMAC_CFG_MUMIMO_PARA, _HALMAC_CFG_MUMIMO_PARA, *PHALMAC_CFG_MUMIMO_PARA;
typedef struct halmac_su_bfer_init_para HALMAC_SU_BFER_INIT_PARA, _HALMAC_SU_BFER_INIT_PARA, *PHALMAC_SU_BFER_INIT_PARA;
typedef struct halmac_mu_bfee_init_para HALMAC_MU_BFEE_INIT_PARA, _HALMAC_MU_BFEE_INIT_PARA, *PHALMAC_MU_BFEE_INIT_PARA;
typedef struct halmac_mu_bfer_init_para HALMAC_MU_BFER_INIT_PARA, _HALMAC_MU_BFER_INIT_PARA, *PHALMAC_MU_BFER_INIT_PARA;
typedef struct halmac_ver HALMAC_VER, _HALMAC_VER, *PHALMAC_VER;
typedef struct halmac_txff_allocation HALMAC_TXFF_ALLOCATION, _HALMAC_TXFF_ALLOCATION, *PHALMAC_TXFF_ALLOCATION;
typedef struct halmac_rqpn_map HALMAC_RQPN_MAP, _HALMAC_RQPN_MAP, *PHALMAC_RQPN_MAP;
typedef struct halmac_rqpn_ch_map HALMAC_RQPN_CH_MAP, _HALMAC_RQPN_CH_MAP, *PHALMAC_RQPN_CH_MAP;
typedef struct halmac_security_setting HALMAC_SECURITY_SETTING, _HALMAC_SECURITY_SETTING, *PHALMAC_SECURITY_SETTING;
typedef struct halmac_cam_entry_info HALMAC_CAM_ENTRY_INFO, _HALMAC_CAM_ENTRY_INFO, *PHALMAC_CAM_ENTRY_INFO;
typedef struct halmac_cam_entry_format HALMAC_CAM_ENTRY_FORMAT, _HALMAC_CAM_ENTRY_FORMAT, *PHALMAC_CAM_ENTRY_FORMAT;
typedef struct halmac_ampdu_config HALMAC_AMPDU_CONFIG, _HALMAC_AMPDU_CONFIG, *PHALMAC_AMPDU_CONFIG;
typedef struct halmac_rqpn HALMAC_RQPN, _HALMAC_RQPN, *PHALMAC_RQPN;
typedef struct halmac_sdio_hw_info HALMAC_SDIO_HW_INFO, _HALMAC_SDIO_HW_INFO, *PHALMAC_SDIO_HW_INFO;
typedef struct halmac_edca_para HALMAC_EDCA_PARA, _HALMAC_EDCA_PARA, *PHALMAC_EDCA_PARA;
typedef struct halmac_ofld_func_info HALMAC_OFLD_FUNC_INFO, _HALMAC_OFLD_FUNC_INFO, *PHALMAC_OFLD_FUNC_INFO;
typedef struct halmac_su_snding_info HALMAC_SU_SNDING_INFO, _HALMAC_SU_SNDING_INFO, *PHALMAC_SU_SNDING_INFO;
typedef struct halmac_iqk_para HALMAC_IQK_PARA, _HALMAC_IQK_PARA, *PHALMAC_IQK_PARA;
typedef struct halmac_adapter HALMAC_ADAPTER, _HALMAC_ADAPTER, *PHALMAC_ADAPTER;
typedef struct halmac_api HALMAC_API, _HALMAC_API, *PHALMAC_API;
typedef struct halmac_tlp_struct HALMAC_TLP_STRUCT, _HALMAC_TLP_STRUCT, *PHALMAC_TLP_STRUCT;
typedef struct halmac_obj HALMAC_OBJ, _HALMAC_OBJ, *PHALMAC_OBJ;

#endif
