/**
  ******************************************************************************
  * @file    usbd_hal.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbd_hal.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_HAL_H
#define USBD_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "ameba_soc.h"
#include "usb_ch9.h"
#include "usb_os.h"
#include "usb_hal.h"
#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/* USB section defines */
#define USBD_DATA_SECTION
#define USBD_BSS_SECTION
#define USBD_TEXT_SECTION

/* USB Core SW Configurations */
#define USBD_DMA_ENABLE                        0U

/* USB Core HW Configurations */
#define USBD_IN_TOKEN_QUEUE_DEPTH              8U

/* USB Core ID */
#define USB_OTG_CORE_ID_300A                   0x4F54300AU
#define USB_OTG_CORE_ID_310A                   0x4F54310AU

/* USB Core Mode */
#define USB_OTG_MODE_DEVICE                    0U
#define USB_OTG_MODE_HOST                      1U
#define USB_OTG_MODE_DRD                       2U

/* USB Core Speed */
#define USB_OTG_SPEED_HIGH                     0U
#define USB_OTG_SPEED_HIGH_IN_FULL             1U
#define USB_OTG_SPEED_FULL                     3U

/* USB Core Turnaround Timeout Value */
#define USBD_HS_TRDT_VALUE                     5U
#define USBD_FS_TRDT_VALUE                     9U

/* USB Core MPS */
#define USB_OTG_HS_MAX_PACKET_SIZE             512U
#define USB_OTG_FS_MAX_PACKET_SIZE             64U
#define USB_OTG_MAX_EP0_SIZE                   64U

/* USB Core PHY Frequency */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0U << 1)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1U << 1)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2U << 1)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3U << 1)

/* USB Core Frame Interval */
#define DCFG_FRAME_INTERVAL_80                 0U
#define DCFG_FRAME_INTERVAL_85                 1U
#define DCFG_FRAME_INTERVAL_90                 2U
#define DCFG_FRAME_INTERVAL_95                 3U

/* USB Core EP0 MPS */
#define DEP0CTL_MPS_64                         0U
#define DEP0CTL_MPS_32                         1U
#define DEP0CTL_MPS_16                         2U
#define DEP0CTL_MPS_8                          3U

/* USB EP Speed */
#define EP_SPEED_LOW                           0U
#define EP_SPEED_FULL                          1U
#define EP_SPEED_HIGH                          2U

/* USB EP Address Mask */
#define EP_ADDR_MSK                            0xFU

/* USB STS Defines */
#define STS_GOUT_NAK                           1U
#define STS_DATA_UPDT                          2U
#define STS_XFER_COMP                          3U
#define STS_SETUP_COMP                         4U
#define STS_SETUP_UPDT                         6U

/* Exported types ------------------------------------------------------------*/

/* USB PCD endpoint structure */
typedef struct {
	u8   num;                  /*!< Endpoint number, 1~4 */
	u8   is_in;                /*!< Endpoint direction */
	u8   is_stall;             /*!< Endpoint stall condition */
	u8   type;                 /*!< Endpoint type, USB_EP_Type_XXX */
	u8   data_pid_start;       /*!< Initial data PID, 0~1 */
	u8   even_odd_frame;       /*!< IFrame parity, 0~1 */
	u16  tx_fifo_num;          /*!< Transmission FIFO number, 1~max in ep num */
	u32  max_packet_len;            /*!< Endpoint Max packet size, 0~64KB */
	u8  *xfer_buff;            /*!< Pointer to transfer buffer */
	u32  dma_addr;             /*!< 32 bits aligned transfer buffer address */
	u32  xfer_len;             /*!< Current transfer length */
	u32  xfer_count;           /*!< Partial transfer length in case of multi packet transfer */
	u8   is_zlp;               /*!< ZLP packet */
} usbd_pcd_ep_t;

/* USB device LPM suspend state */
typedef enum {
	LPM_L0 = 0x00, /* on */
	LPM_L1 = 0x01, /* LPM L1 sleep */
	LPM_L2 = 0x02, /* suspend */
	LPM_L3 = 0x03, /* off */
} usbd_lpm_state_t;

/* PCD state */
typedef enum {
	HAL_PCD_STATE_RESET   = 0x00,
	HAL_PCD_STATE_READY   = 0x01,
	HAL_PCD_STATE_ERROR   = 0x02,
	HAL_PCD_STATE_BUSY    = 0x03,
	HAL_PCD_STATE_TIMEOUT = 0x04,
	HAL_PCD_STATE_STOP    = 0x05
} usbd_pcd_state_t;

/* PCD handle structure */
typedef struct {
	/*!< PCD required parameters  */
	usbd_config_t config;
	/*!< USB Address */
	__IO u8 address;
	/*!< IN endpoint parameters */
	usbd_pcd_ep_t in_ep[USBD_MAX_ENDPOINTS];
	/*!< OUT endpoint parameters */
	usbd_pcd_ep_t out_ep[USBD_MAX_ENDPOINTS];
	/*!< PCD peripheral status */
	usb_spinlock_t *lock;
	/*!< PCD communication state */
	__IO usbd_pcd_state_t pcd_state;
	/*!< LPM State */
	usbd_lpm_state_t lpm_state;
	/*!< Setup packet buffer */
	u8 *setup;
	/*!< PHY initialized */
	u8 phy_initialized;
	/*!< ISR initialized */
	u8 isr_initialized;
#if USBD_DMA_ENABLE
	/*!< Start predict NextEP based on Learning Queue if equal 1, also used as counter of disabled NP IN EP's */
	u8 start_predict;
	/*!< NextEp sequence, including EP0: nextep_seq[] = EP if non-periodic and active, 0xff otherwise */
	u8 nextep_seq[USBD_MAX_ENDPOINTS];
	/*!< Index of fisrt EP in nextep_seq array which should be re-enabled */
	u8 first_in_nextep_seq;
#endif
	_sema isr_sema;
	struct task_struct isr_task;
	/*!< IN EP sequence from In token queue */
	u8 in_ep_sequence[USBD_MAX_ENDPOINTS];
	/*!< Pointer to upper stack Handler */
	usb_dev_t *dev;
} usbd_pcd_t;

/* Unaligned 32 bit type for DFIFO usage */
typedef struct {
	u32 data __PACKED;
} usbd_unaligned_u32_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_hal_device_init(usbd_pcd_t *pcd);
u8 usbd_hal_set_turnaround_time(usbd_pcd_t *pcd);
u8 usbd_hal_set_device_speed(usbd_pcd_t *pcd, u8 speed);
u8 usbd_hal_set_tx_fifo(usbd_pcd_t *pcd, u8 fifo, u16 size);
u8 usbd_hal_set_rx_fifo(usbd_pcd_t *pcd, u16 size);
u8 usbd_hal_ep_activate(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_deactivate(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_start_transfer(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep0_start_transfer(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_set_stall(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_clear_stall(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_set_device_address(usbd_pcd_t *pcd, u8 address);
u8 usbd_hal_connect(usbd_pcd_t *pcd);
u8 usbd_hal_disconnect(usbd_pcd_t *pcd);
u8 usbd_hal_device_stop(usbd_pcd_t *pcd);
u8 usbd_hal_ep0_setup_activate(usbd_pcd_t *pcd);
u8 usbd_hal_ep0_out_start(usbd_pcd_t *pcd);
u8 usbd_hal_reset_in_token_queue(usbd_pcd_t *pcd);
u8 usbd_hal_predict_next_ep(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_get_device_speed(usbd_pcd_t *pcd);
u32 usbd_hal_read_all_out_ep_interrupts(usbd_pcd_t *pcd);
u32 usbd_hal_read_out_ep_interrupts(usbd_pcd_t *pcd, u8 ep_num);
u32 usbd_hal_read_all_in_ep_interrupts(usbd_pcd_t *pcd);
u32 usbd_hal_read_in_ep_interrupts(usbd_pcd_t *pcd, u8 ep_num);

#endif /* USBD_HAL_H */

