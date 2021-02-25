/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_config.h
  * @brief    Head file for mesh configuration.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_CONFIG_H
#define _MESH_CONFIG_H

/* Add Includes here */
#include "platform_types.h"
#include "platform_opts_bt.h"

BEGIN_DECLS

#if defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE 
#define MESH_PROVISIONER 1
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
#define MESH_DEVICE 1
#endif

/** setting */
#define MESH_USE_RANDOM_ADDR                    0 //!< random bt address
#define MESH_SUPPORT_TRANS_PING                 1 //!< transport layer ping
#define MESH_CENTRAL_ROLE_SUPPORT               1 //!< central role
#define MESH_CONFIGURATION_MODEL_USE_APP_KEY    1 //!< violate the spec to use app key, shall also set mesh_node.features.cfg_model_use_app_key.
#define MESH_PROXY_ADV_WITH_BT_ADDR             1 //!< adv
#define MESH_PARAM_CONFIGURABLE                 0 //!< configurable parameters

/** !!! For test purpose. If don't know well, please do not modify in case of wrong operation !!! */
#define MESH_MUTE_MODE                          0 //!< just scan to Receive all mesh data pkts even not sent to me playing a role as a monitor.
#define MESH_TRANSCEIVER_MODE                   0 //!< Disable duplicate pkts and src address check in network layer.
#define MESH_PROVISIONING_SAMPLE_DATA           0 //!< Provisioning Protocol
#define MESH_UNPROVISIONING_SUPPORT             0 //!< Support unprovisioning cmd in pb-gatt

/* mesh features */
#define MESH_DATA_UART_DEBUG                    1 //!< Data uart debug
#define MESH_FN                                 1 //!< friend relay
#define MESH_BEARER_ADV                         1
#define MESH_BEARER_GATT                        1

/* mesh profile 1.1 */
#define MESH_1_1_SUPPORT                        1
#define MESH_PRB                                (MESH_1_1_SUPPORT && 0)
#define MESH_BLOB                               (MESH_1_1_SUPPORT && 1)
#define MESH_DFU                                (MESH_1_1_SUPPORT && MESH_BLOB && 1)
#define MESH_RPR                                (MESH_1_1_SUPPORT && 1)

#if defined(MESH_DEVICE) && MESH_DEVICE
//#warning "current project is device lib"
#define MESH_LPN                                1 //!< low power node
#define MESH_GENERATE_ECC_KEY_PAIR_EACH_SESSION 1 //!< only mandatory in the device?
#define MESH_DEVICE_ONLY_ONE_DEV_KEY            0 //!< Device may send configuration client msgs using other device's DevKey
#define MESH_DEVICE_PROV_PROXY_SERVER_COEXIST   1 //!< Why not?
#endif

#if defined(MESH_PROVISIONER) && MESH_PROVISIONER
//#warning "current project is provisioner lib"
#define MESH_PROV_WO_AUTH_VALUE                 0
#endif

#if MESH_DATA_UART_DEBUG
#include "mesh_data_uart.h"
#else
#define data_uart_debug(...)
#define data_uart_dump(...)
#endif

#if ((defined(MESH_FN) && MESH_FN) && \
    (defined(MESH_LPN) && MESH_LPN))
//#error "Can't support both Friend Node and Low Power Node at the same time!"
#endif

/** Networking parameters */
#define MESH_UDB_PERIOD                         3000 //!< in units of millisecond
#define MESH_SNB_PERIOD                         10000 //!< in units of millisecond
#define MESH_PROV_ADV_PERIOD                    3000 //!< in units of millisecond
#define MESH_PROXY_ADV_PERIOD                   3000 //!< in units of millisecond
#define MESH_ID_ADV_PERIOD                      300 //!< in units of millisecond
#if MESH_PRB
#define MESH_PRB_PERIOD                         10000 //!< in units of millisecond
#define MESH_PRB_RANDOM_UPDATE_PERIOD           600 //!< in units of seconds 
#define MESH_PID_ADV_PERIOD                     300 //!< in units of millisecond
#endif

/** Mesh stored message parameters */
#define MESH_NET_MSG_CACHE_SIZE                 30 //!< The number of devices to check duplicate packets
#define MESH_INNER_MSG_NUM                      16 //!< The number of mesh inner msg

/** PB-ADV Generic Provisioning Layer */
#define MESH_PB_GENERIC_TIMEOUT_PERIOD          30000 //!< 30s transaction timeout
#define MESH_PB_ADV_LINK_IDLE_TIMEOUT           80000 //!< 80s link idle
#define MESH_PB_ADV_RETRY_PERIOD                200 //!< ms
//#define MESH_PB_GENERIC_RETRY_TIMES             15

/** Prov */
#define MESH_PROV_TIMEOUT_PERIOD                60000 //!< 60s protocol timeout

/** Proxy */
#define MESH_PROXY_SAR_TIMEOUT_PERIOD           20000
#define MESH_PROXY_SAR_BUFFER_LEN               65

/** Network layer */
#define MESH_NET_RETRANSMIT_TIMES               1
#define MESH_RELAY_RETRANSMIT_TIMES             1

/** Transport layer */
#define MESH_TRANS_TX_WAIT_ACK_PERIOD           500
#define MESH_TRANS_RX_ACK_PERIOD                400
#define MESH_TRANS_RETRANSMIT_TIMES             2
#define MESH_TRANS_TX_QUEUE_SIZE                3
#define MESH_TRANS_RX_CTX_COUNT                 2

/** FN parameters */
#define MESH_FRIENDSHIP_SUB_LIST_SIZE           10
#define MESH_FRIENDSHIP_RX_WINDOW               (1 * GAP_SCHED_SCAN_INTERVAL * 625 / 1000) //!< ms, set according to scan interval
#define MESH_FN_TX_AHEAD                        15 //!< in units of millisecond
#define MESH_FRIENDSHIP_TIMEOUT_PERIOD          1000 //!< in units of millisecond
#define MESH_FRND_QUEUE_MIN_TTL                 2

/** LPN parameters */
#define MESH_LPN_RX_DELAY                       10
#define MESH_LPN_RX_WIDEN                       0
#define MESH_LPN_FRND_OFFER_RX_DELAY            90 //!< 100ms defined by spec
#define MESH_LPN_FRND_WAIT_PERIOD               1100 //!< offer range
#define MESH_LPN_FRND_OFFER_CHOOSE_DELAY        400 //!< 6 times, total 1s
#define MESH_LPN_FRND_POLL_RETRY_INTERVAL       300
#define MESH_LPN_FRND_POLL_RETRY_TIMES          8
#define MESH_LPN_FRND_POLL_FAILED_TIMES         1
#define MESH_LPN_FRND_SCAN_DELAY_TIME           0

/** Remote Provisioning server parameters */
#define MESH_RMT_PRO_SERVER_UNPRO_DEVICE_NUM    3 //!<
#define MESH_RMT_PRO_SERVER_SCAN_TIMEOUT_PERIOD 10000 //!<
#define MESH_RMT_PRO_SERVER_SCAN_REPORT_PERIOD  3000 //!<
#define MESH_RMT_PRO_SERVER_LINK_OPEN_INTERVAL  2000 //!<
#define MESH_RMT_PRO_SERVER_LINK_OPEN_RETRY_TIMES 3 //!<

/** device uuid */
#define MESH_DEVICE_UUID                        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}

/** application key default value */
#define MESH_APP_KEY                            {0x63, 0x96, 0x47, 0x71, 0x73, 0x4f, 0xbd, 0x76, 0xe3, 0xb4, 0x05, 0x19, 0xd1, 0xd9, 0x4a, 0x48}
#define MESH_APP_KEY1                           {0xf1, 0xa2, 0x4a, 0xbe, 0xa9, 0xb8, 0x6c, 0xd3, 0x33, 0x80, 0xa2, 0x4c, 0x4d, 0xfb, 0xe7, 0x43}

/** net key default value */
#define MESH_NET_KEY                            {0x7d, 0xd7, 0x36, 0x4c, 0xd8, 0x42, 0xad, 0x18, 0xc1, 0x7c, 0x2b, 0x82, 0x0c, 0x84, 0xc3, 0xd6}
#define MESH_NET_KEY1                           {0xef, 0xb2, 0x25, 0x5e, 0x64, 0x22, 0xd3, 0x30, 0x08, 0x8e, 0x09, 0xbb, 0x01, 0x5e, 0xd7, 0x07}

#define MESH_IDENTITY_CREDIT                    2 //!< shall be >= 1 && <= mesh_node.net_key_num

/** network source address default value */
#if MESH_MUTE_MODE
#define MESH_SRC                                0x0000
#else
#define MESH_SRC                                0x0001
#endif

/** network sequence number default value */
#define MESH_SEQ                                0 //0x010203
//#define MESH_SEQ_NUM_OVERFLOW_ERROR             (0x00ffffff - 100) //!< Sequence number error range in case of overflow
#define MESH_SEQ_NUM_MAX                        0xffffff //!< 16777216
#define MESH_SEQ_START_IV_UPDATE                (MESH_SEQ_NUM_MAX - 144 * 3600 / 5) //!< 144 * 3600 / 5 = 103680
#define MESH_SEQ_NUM_STEP                       100 // greater than or equal to 1, less than MESH_SEQ_NUM_MAX
#if MESH_SEQ_NUM_STEP == 0 || MESH_SEQ_NUM_STEP > MESH_SEQ_NUM_MAX
#error "Wrong seq step setting!"
#endif

/** network iv index */
#define MESH_IV_INDEX                           0
#define MESH_IV_INDEX_TIMER_PERIOD              600 //!< second
#define MESH_IV_INDEX_96H                       (96 * 3600 / MESH_IV_INDEX_TIMER_PERIOD)
#define MESH_IV_INDEX_144H                      (144 * 3600 / MESH_IV_INDEX_TIMER_PERIOD)
#define MESH_IV_INDEX_192H                      (2 * MESH_IV_INDEX_96H)
#define MESH_IV_INDEX_48W                       (42 * MESH_IV_INDEX_192H)

/** network TTL default value */
#define MESH_TTL_MAX                            0x7f
#define MESH_TTL_DEFAULT                        7

/** provisioning parameters */
#if MESH_PROVISIONING_SAMPLE_DATA
#define MESH_PROVISIONER_PUBLIC_KEY             {0x2c, 0x31, 0xa4, 0x7b, 0x57, 0x79, 0x80, 0x9e, 0xf4, 0x4c, 0xb5, 0xea, 0xaf, 0x5c, 0x3e, 0x43, 0xd5, 0xf8, 0xfa, 0xad, 0x4a, 0x87, 0x94, 0xcb, 0x98, 0x7e, 0x9b, 0x03, 0x74, 0x5c, 0x78, 0xdd, 0x91, 0x95, 0x12, 0x18, 0x38, 0x98, 0xdf, 0xbe, 0xcd, 0x52, 0xe2, 0x40, 0x8e, 0x43, 0x87, 0x1f, 0xd0, 0x21, 0x10, 0x91, 0x17, 0xbd, 0x3e, 0xd4, 0xea, 0xf8, 0x43, 0x77, 0x43, 0x71, 0x5d, 0x4f}
#define MESH_PROVISIONER_PRIVATE_KEY            {0x06, 0xa5, 0x16, 0x69, 0x3c, 0x9a, 0xa3, 0x1a, 0x60, 0x84, 0x54, 0x5d, 0x0c, 0x5d, 0xb6, 0x41, 0xb4, 0x85, 0x72, 0xb9, 0x72, 0x03, 0xdd, 0xff, 0xb7, 0xac, 0x73, 0xf7, 0xd0, 0x45, 0x76, 0x63}
#define MESH_PROVISIONER_RANDOM                 {0x8b, 0x19, 0xac, 0x31, 0xd5, 0x8b, 0x12, 0x4c, 0x94, 0x62, 0x09, 0xb5, 0xdb, 0x10, 0x21, 0xb9}
#define MESH_DEVICE_PUBLIC_KEY                  {0xf4, 0x65, 0xe4, 0x3f, 0xf2, 0x3d, 0x3f, 0x1b, 0x9d, 0xc7, 0xdf, 0xc0, 0x4d, 0xa8, 0x75, 0x81, 0x84, 0xdb, 0xc9, 0x66, 0x20, 0x47, 0x96, 0xec, 0xcf, 0x0d, 0x6c, 0xf5, 0xe1, 0x65, 0x00, 0xcc, 0x02, 0x01, 0xd0, 0x48, 0xbc, 0xbb, 0xd8, 0x99, 0xee, 0xef, 0xc4, 0x24, 0x16, 0x4e, 0x33, 0xc2, 0x01, 0xc2, 0xb0, 0x10, 0xca, 0x6b, 0x4d, 0x43, 0xa8, 0xa1, 0x55, 0xca, 0xd8, 0xec, 0xb2, 0x79}
#define MESH_DEVICE_PRIVATE_KEY                 {0x52, 0x9a, 0xa0, 0x67, 0x0d, 0x72, 0xcd, 0x64, 0x97, 0x50, 0x2e, 0xd4, 0x73, 0x50, 0x2b, 0x03, 0x7e, 0x88, 0x03, 0xb5, 0xc6, 0x08, 0x29, 0xa5, 0xa3, 0xca, 0xa2, 0x19, 0x50, 0x55, 0x30, 0xba}
#define MESH_DEVICE_RANDOM                      {0x55, 0xa2, 0xa2, 0xbc, 0xa0, 0x4c, 0xd3, 0x2f, 0xf6, 0xf3, 0x46, 0xbd, 0x0a, 0x0c, 0x1a, 0x3a}
#define MESH_PROVISIONING_NET_KEY               MESH_NET_KEY1
#define MESH_PROVISIONING_NET_KEY_INDEX         0x0567
#define MESH_PROVISIONING_IV_INDEX              0x01020304
#define MESH_PROVISIONING_UNICAST_ADDRESS       0x0b0c
#else
#define MESH_PROVISIONER_PUBLIC_KEY             {0}
#define MESH_PROVISIONER_PRIVATE_KEY            {0}
#define MESH_PROVISIONER_RANDOM                 {0}
#define MESH_DEVICE_PUBLIC_KEY                  {0}
#define MESH_DEVICE_PRIVATE_KEY                 {0}
#define MESH_DEVICE_RANDOM                      {0}
#define MESH_PROVISIONING_NET_KEY               MESH_NET_KEY
#define MESH_PROVISIONING_NET_KEY_INDEX         0
#define MESH_PROVISIONING_IV_INDEX              MESH_IV_INDEX
#define MESH_PROVISIONING_UNICAST_ADDRESS       0x0100//MESH_SRC
#endif

/* model configuration */
#define MESH_MODEL_ENABLE_DEINIT                1

#define MESH_SUPPORT_CHECK                      1

END_DECLS

#endif /* _MESH_CONFIG_H */

