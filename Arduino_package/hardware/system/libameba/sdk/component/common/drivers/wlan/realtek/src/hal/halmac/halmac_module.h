/******************************************************************************
 *
 * Copyright(c) 2016 - 2018 Realtek Corporation. All rights reserved.
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

#ifndef _HALMAC_MODULE_H_
#define _HALMAC_MODULE_H_

#define HALMAC_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
#define HALMAC_CURRENT_VERSION                                                 \
	HALMAC_VERSION(HALMAC_MAJOR_VER, HALMAC_PROTOTYPE_VER, HALMAC_MINOR_VER)

#define MAX_ARG_NUM 10

#define TLP_TYPE_NONE				0x00000000
#define TLP_TYPE_PU8				0x00000001
#define TLP_TYPE_PU16				0x00000002
#define TLP_TYPE_PU32				0x00000003
#define TLP_TYPE_PU64				0x00000004
#define TLP_TYPE_PS8				0x00000005
#define TLP_TYPE_PS16				0x00000006
#define TLP_TYPE_PS32				0x00000007
#define TLP_TYPE_PS64				0x00000008
#define TLP_TYPE_PVOID				0x00000009
#define TLP_TYPE_PU8_BUFFER			0x0000000A
#define TLP_TYPE_POS_API_ENTRY			0x0000000B
#define TLP_TYPE_PHALMAC_WLAN_ADDR		0x0000000C
#define TLP_TYPE_PHALMAC_PHY_PARAMETER_INFO	0x0000000D
#define TLP_TYPE_PHALMAC_CH_INFO		0x0000000E
#define TLP_TYPE_PHALMAC_CH_SWITCH_OPTION	0x0000000F
#define TLP_TYPE_PHALMAC_GENERAL_INFO		0x00000010
#define TLP_TYPE_PHALMAC_CMD_PROCESS_STATUS	0x00000011
#define TLP_TYPE_PHALMAC_INTERFACE		0x00000012
#define TLP_TYPE_PHALMAC_MAC_POWER		0x00000013
#define TLP_TYPE_PHALMAC_RXAGG_CFG		0x00000014
#define TLP_TYPE_PHALMAC_WIRELESS_MODE		0x00000015
#define TLP_TYPE_PHALMAC_BW			0x00000016
#define TLP_TYPE_PHALMAC_TRX_MODE		0x00000017
#define TLP_TYPE_PHALMAC_EFUSE_READ_CFG		0x00000018
#define TLP_TYPE_PHALMAC_PACKET_ID		0x00000019
#define TLP_TYPE_PHALMAC_DRV_INFO		0x0000001A
#define TLP_TYPE_PHALMAC_FEATURE_ID		0x0000001B
#define TLP_TYPE_PHALMAC_HW_ID			0x0000001C
#define TLP_TYPE_PHALMAC_DRV_RSVD_PG_NUM	0x0000001D
#define TLP_TYPE_PHALMAC_LA_MODE		0x0000001E
#define TLP_TYPE_PHALMAC_PWR_TRACKING		0x0000001F
#define TLP_TYPE_PHALMAC_EFUSE_BANK		0x00000020
#define TLP_TYPE_PHALMAC_IQK			0x00000021
#define TLP_TYPE_PHALMAC_MU_BFER_INIT_PARA	0x00000022
#define TLP_TYPE_PHALMAC_MU_BFEE_INIT_PARA	0x00000023
#define TLP_TYPE_PHALMAC_SU_BFER_INIT_PARA	0x00000024
#define TLP_TYPE_PHALMAC_SND_ROLE		0x00000025
#define TLP_TYPE_PHALMAC_DATA_RATE		0x00000026
#define TLP_TYPE_PHALMAC_CFG_MUMIMO_PARA	0x00000027
#define TLP_TYPE_PU8_1				0x00000028
#define TLP_TYPE_PHALMAC_PG_EFUSE_INFO		0x00000029
#define TLP_TYPE_PU8_2				0x0000002A
#define TLP_TYPE_PHALMAC_INTF_PHY_PLATFORM	0x0000002B
#define TLP_TYPE_PHALMAC_API_REGISTRY		0x0000002C
#define TLP_TYPE_PHALMAC_DLFW_MEM		0x0000002D
#define TLP_TYPE_PHALMAC_FW_VERSION		0x0000002E
#define TLP_TYPE_PHALMAC_SIDEBAND_INT_CFG	0x0000002F
#define TLP_TYPE_PHALMAC_FIFO_SEL		0x00000030
#define TLP_TYPE_PHALMAC_P2PPS			0x00000031
#define TLP_TYPE_PHALMAC_RX_EXPAND_MODE		0x00000032
#define TLP_TYPE_PHALMAC_PCIE_CFG		0x00000033
#define TLP_TYPE_PHALMAC_SDIO_CMD53_4BYTE_MODE	0x00000034
#define TLP_TYPE_PHALMAC_NETWORK_TYPE_SELECT	0x00000035
#define TLP_TYPE_PHALMAC_BCN_CTRL		0x00000036
#define TLP_TYPE_PHALMAC_SDIO_HW_INFO		0x00000037
#define TLP_TYPE_PHALMAC_INTF_INTGRA		0x00000038
#define TLP_TYPE_PHALMAC_GPIO_FUNC		0x00000039
#define TLP_TYPE_PHALMAC_WLLED_MODE		0x00000040
#define TLP_TYPE_PU32_1				0x00000041
#define TLP_TYPE_PHALMAC_OFLD_FUNC_INFO		0x00000042
#define TLP_TYPE_PHALMAC_CUT_AMSDU_CFG		0x00000043
#define TLP_TYPE_PSU_SNDING			0x00000044
#define TLP_TYPE_PMU_SNDING			0x00000045
#define TLP_TYPE_PWLCPU_MODE			0x00000046

struct halmac_tlp_struct {
	u32 type;
	u32 length;
	void *ptr;
};

struct halmac_api_arg {
	enum halmac_api_id api_id;
	u32 arg_count;
	u32 arg_type_seq[MAX_ARG_NUM];
};

struct halmac_obj {
	u8 init;
	u32 version;
	struct halmac_adapter *halmac_adapter;
	struct halmac_api *halmac_api_entry;
};

enum halmac_ret_status
halmac_initialize_obj(struct halmac_obj *halmac_obj,
		      struct halmac_tlp_struct *ptlp, u32 tlp_count);

enum halmac_ret_status
halmac_deinitialize_obj(struct halmac_obj *halmac_obj,
			struct halmac_tlp_struct *ptlp, u32 tlp_count);

enum halmac_ret_status
halmac_set_information(struct halmac_obj *halmac_obj, enum halmac_api_id api_id,
		       struct halmac_tlp_struct *ptlp, u32 tlp_count);

enum halmac_ret_status
halmac_get_information(struct halmac_obj *halmac_obj, enum halmac_api_id api_id,
		       struct halmac_tlp_struct *ptlp, u32 tlp_count);

#endif
