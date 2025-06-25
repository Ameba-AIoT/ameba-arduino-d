/**
  ******************************************************************************
  * The header file for usbh_cdc_ecm.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_CDC_ECM_H
#define USBH_CDC_ECM_H

/* Includes ------------------------------------------------------------------*/

#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

#define CDC_ECM_HOST_PIPE_COUNT                         (16)
#define USBH_CDC_ECM_BULK_BUF_MAX_SIZE                  (512)

#define CDC_ECM_MAC_FILTER_MAX_LEN                      (20)

/* CDC Class Codes */
#define CDC_CLASS_CODE                                  0x02U
#define CDC_IF_CDC_CTRL_CODE                            0x02U
#define CDC_IF_CDC_CTRL_SUB_CLASS_ECM_CODE              0x06U
#define CDC_IF_CDC_DATA_CODE                            0x0AU
#define CDC_IF_CDC_DATA_SUB_CLASS_DATA_CODE             0x00U

/* CDC Communication sub class codes */
#define CDC_RESERVED                                    0x00U
#define CDC_ETHERNET_NETWORK_CONTROL_MODEL              0x06U

/* Table 6: Class-Specific Request Codes for Ethernet subclass */
#define CDC_ECM_SET_ETHERNET_MULTICAST_FILTERS                  0x40U
#define CDC_ECM_SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER    0x41U
#define CDC_ECM_GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER    0x42U
#define CDC_ECM_SET_ETHERNET_PACKET_FILTER                      0x43U
#define CDC_ECM_GET_ETHERNET_STATISTIC                          0x44U
#define CDC_ECM_SPECIAL_REQUEST_CODE_RESERVED                   0x45U

/* [CDC]Table 20: Class-Specific Notification Codes */
/* Table 11: Class-Specific Notification Codes for Ethernet subclass */
#define CDC_ECM_NOTIFY_NETWORK_CONNECTION                       0x00U
#define CDC_ECM_NOTIFY_RESPONSE_AVAILABLE                       0x01U
#define CDC_ECM_NOTIFY_CONNECTION_SPEED_CHANGE                  0x2AU

/* Communication Interface Class Control Protocol Codes */
#define CDC_CTRL_PROTOCOL_NO_CLASS_SPECIFIC                     0x00U

/*  bDescriptor SubType in Communications Class Functional Descriptors  */
#define CDC_ECM_NETWORK_FUNC_DESCRIPTOR                         0x0FU

#define USB_DT_CS_INTERFACE                                     0x24

/* Exported types ------------------------------------------------------------*/

#pragma pack(push)
#pragma pack(1)

/* USB Host Status */
typedef enum {
    USBH_CDC_ECM_IDLE = 0U,
    USBH_CDC_ECM_DETACHED,
    USBH_CDC_ECM_ATTACH,
    USBH_CDC_ECM_SETUP,
    USBH_CDC_ECM_MAX,
} usbh_ecm_state_t;

/* CDC ECM Ethernet Packet Filter Bitmap  */
typedef enum {
    CDC_ECM_ETH_PACKET_TYPE_PROMISCUOUS     = 0x01U, /* ALL frames received by the networking device are forwarded up to the host (required) */
    CDC_ECM_ETH_PACKET_TYPE_ALL_MULTICAST   = 0x02U, /* ALL multicast frames received by the networking device are forwarded up to the host, not just
                                                        the ones enumerated in the device's multicast address list (required) */
    CDC_ECM_ETH_PACKET_TYPE_DIRECTED        = 0x04U, /* Directed packets received containing a destination address equal to the MAC address of the
                                                        networking device are forwarded up to the host (required) */
    CDC_ECM_ETH_PACKET_TYPE_BROADCAST       = 0x08U, /* All broadcast packets received by the networking device are forwarded up to the host. */
    CDC_ECM_ETH_PACKET_TYPE_MULTICAST       = 0x10U, /* All multicast packets enumerated in the device's multicast address list are forwarded up to the host */
    CDC_ECM_ETH_PACKET_TYPE_MAX,
} usbh_cdc_ecm_eth_packet_filter_bitmap_t;

/* CDC ECM Ethernet Statistics Feature Selector Codes */
typedef enum {
    CDC_ECM_FEATURE_SELECTOR_RESERVED = 0U,
    CDC_ECM_FEATURE_SELECTOR_XMIT_OK,
    CDC_ECM_FEATURE_SELECTOR_RCV_OK,
    CDC_ECM_FEATURE_SELECTOR_XMIT_ERROR,
    CDC_ECM_FEATURE_SELECTOR_RCV_ERROR,
    CDC_ECM_FEATURE_SELECTOR_RCV_NO_BUFFER,
    CDC_ECM_FEATURE_SELECTOR_DIRECTED_BYTES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_DIRECTED_FRAMES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_MULTICAST_BYTES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_MULTICAST_FRAMES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_BROADCAST_BYTES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_BROADCAST_FRAMES_XMIT,
    CDC_ECM_FEATURE_SELECTOR_DIRECTED_BYTES_RCV,
    CDC_ECM_FEATURE_SELECTOR_DIRECTED_FRAMES_RCV,
    CDC_ECM_FEATURE_SELECTOR_MULTICAST_BYTES_RCV,
    CDC_ECM_FEATURE_SELECTOR_MULTICAST_FRAMES_RCV,
    CDC_ECM_FEATURE_SELECTOR_BROADCAST_BYTES_RCV,
    CDC_ECM_FEATURE_SELECTOR_BROADCAST_FRAMES_RCV,
    CDC_ECM_FEATURE_SELECTOR_RCV_CRC_ERROR,
    CDC_ECM_FEATURE_SELECTOR_TRANSMIT_QUEUE_LENGTH,
    CDC_ECM_FEATURE_SELECTOR_RCV_ERROR_ALIGNMENT,
    CDC_ECM_FEATURE_SELECTOR_XMIT_ONE_COLLISION,
    CDC_ECM_FEATURE_SELECTOR_XMIT_MORE_COLLISIONS,
    CDC_ECM_FEATURE_SELECTOR_XMIT_DEFERRED,
    CDC_ECM_FEATURE_SELECTOR_XMIT_MAX_COLLISIONS,
    CDC_ECM_FEATURE_SELECTOR_RCV_OVERRUN,
    CDC_ECM_FEATURE_SELECTOR_XMIT_UNDERRUN,
    CDC_ECM_FEATURE_SELECTOR_XMIT_HEARTBEAT_FAILURE,
    CDC_ECM_FEATURE_SELECTOR_XMIT_TIMES_CRS_LOST,
    CDC_ECM_FEATURE_SELECTOR_XMIT_LATE_COLLISIONS,
    CDC_ECM_FEATURE_SELECTOR_MAX,
} usbh_cdc_ecm_statistic_feature_selector_t;

/* ConnectionSpeedChange Data Structure */
typedef union {
    u32 DLBitRate;      /* Contains the downlink bit rate, in bits per second, as sent on the IN pipe */
    u32 ULBitRate;      /* Contains the uplink bit rate, in bits per second, as sent on the OUT pipe */
} usbh_cdc_ecm_speed_change_t;

/* Ethernet Networking Functional Descriptor */
typedef struct {
    u8  bLength;
    u8  bDescriptorType;
    u8  bDescriptorSubtype;                              /* Ethernet Networking functional descriptor subtype */
    u8  iMACAddress;                                     /* Index of string descriptor */
    u32 bmEthernetStatistics;                            /* Indicates which Ethernet statistics functions the device collects. */
    u8  wMaxSegmentSize;                                 /* The maximum segment size that the Ethernet device is capable of supporting */
    u16 wNumberMCFilters;                                /* Contains the number of multicast filters that can be configured by the host */
    u8  bNumberPowerFilters;                             /* Contains the number of pattern filters that are available for causing wake-up of the host. */
} usbh_cdc_ecm_network_func_t ;


/* States for CDC State Machine */
typedef enum {
    CDC_ECM_TRANSFER_STATE_IDLE = 0U,
    CDC_ECM_TRANSFER_STATE_XFER,
    CDC_ECM_TRANSFER_STATE_BUSY,
    CDC_ECM_TRANSFER_STATE_ERROR,
} usbh_cdc_ecm_transfer_state_t;


/* CDC ECM state */
typedef enum {
    CDC_ECM_STATE_CTRL_IDLE = 0U,
    CDC_ECM_STATE_CTRL_ALT_SETTING,
    CDC_ECM_STATE_CTRL_SET_ETHERNET_MULTICAST_FILTER,
    CDC_ECM_STATE_CTRL_SET_ETHERNET_PACKET_FILTER,
    CDC_ECM_STATE_CTRL_GET_ETHERNET_STATISTIC,
    CDC_ECM_STATE_CTRL_ERROR,
} usbh_cdc_ecm_ctrl_state_t;

typedef enum {
    CDC_ECM_CTRL_XFER_OWNER_IDLE = 0U,

    CDC_ECM_CTRL_XFER_OWNER_ECM,
    CDC_ECM_CTRL_XFER_OWNER_APPX,

    CDC_ECM_CTRL_XFER_OWNER_MAX,
} usbh_cdc_ecm_ctrl_transfer_owner_t;

typedef enum {
    CDC_ECM_STATE_IDLE = 0U,

    CDC_ECM_STATE_SWITCH_TO_TRANSFER,
    CDC_ECM_STATE_TRANSFER,  //data transfor

    CDC_ECM_STATE_ERROR,
} usbh_cdc_ecm_state_t;

typedef struct {
    u8              bLength;
    u8              bDescriptorType;
} usb_ecm_descriptor_header;

typedef struct {
    /* descriptor infor*/
    u8              ep_addr;
    u16             ep_mps;
    u32             ep_interval;

    u8              pipe_id;
    /* xfer data infor */
    u8             *xfer_buf;
    u16             xfer_len;

    volatile usbh_cdc_ecm_transfer_state_t    state;
} usbh_cdc_ecm_ep_t;


/* CDC ECM user callback interface */
typedef struct {
    u8(* init)(void);
    u8(* deinit)(void);
    u8(* attach)(void);
    u8(* detach)(void);
    u8(* setup)(void);
    u8(* bulk_received)(u8 *buf, u32 len);
    u8(* bulk_transmitted)(usbh_urb_state_t state);
    u8(* intr_received)(u8 *buf, u32 len);
} usbh_cdc_ecm_state_cb_t;

/* CDC ECM host */
typedef struct {
    /* CDC ECM communication interface */
    struct task_struct intr_task;
    struct task_struct bulk_task;

    usbh_cdc_ecm_ep_t                   report_ep;
    usbh_cdc_ecm_ep_t                   tx_ep;
    usbh_cdc_ecm_ep_t                   rx_ep;
    u8                                  intr_idle_state_cnt;

    u16                                 vid;
    u16                                 pid;

    u8                                  ecm_if;   /* Interface number */
    u8                                  ecm_alt;  /* Value used to select alternative setting */

    u16                                 feature_selector;
    u32                                 eth_statistic_count;

    u16                                 packet_filter;
    u8                                  muticast_filter[CDC_ECM_MAC_FILTER_MAX_LEN];
    u16                                 muticast_filter_len;

    usbh_cdc_ecm_speed_change_t         user_speed_info;

    u8                                  next_transfor;
    usbh_cdc_ecm_state_t                state;     /*usb process status*/
    usbh_cdc_ecm_ctrl_state_t           sub_state;  /*usb ctrl process status*/

    volatile u8                         ctrl_transfer_owner;    /*ctrl transfer owner:ecm or appx*/

    volatile usbh_ecm_state_t           usbh_state; /*usb hw attach status*/

    u8                                  rx_task_flag;
    u8                                  notify_task_flag;
    volatile u8                         host_pipe[CDC_ECM_HOST_PIPE_COUNT];
    usbh_cdc_ecm_state_cb_t             *cb;
    usb_host_t                          *host;
} usbh_cdc_ecm_host_t;

#pragma pack(pop)

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Apis for ecm hal */
u8  usbh_cdc_ecm_init(usbh_cdc_ecm_state_cb_t *cb);
u8  usbh_cdc_ecm_deinit(void);
u8  usbh_cdc_ecm_choose_config(usb_host_t *host);
u8  usbh_cdc_ecm_check_enum_status(void);
u8  usbh_cdc_ecm_get_ctrl_transfer_owner(void);
u8  usbh_cdc_ecm_change_ctrl_transfer_owner(void);
u8  usbh_cdc_ecm_task(void);
u8  usbh_cdc_ecm_bulk_send(u8 *buf, u32 len);
u8  usbh_cdc_ecm_set_alt_setting(void);
u8  usbh_cdc_ecm_trigger_transfer(usbh_cdc_ecm_ep_t  *ep);
u16 usbh_cdc_ecm_get_usbin_mps(void);

#endif  /* USBD_CDC_ECM_H */
