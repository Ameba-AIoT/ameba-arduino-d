/**
  ******************************************************************************
  * The header file for usbh_cdc_ecm_hal.h
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_CDC_ECM_HAL_H
#define USBH_CDC_ECM_HAL_H

/* Includes ------------------------------------------------------------------*/

/*
        Example
      LWIP
        ECM HAL
    ECMClass ECMAPPx
       ECM Core
       USB CORE
*/

/* Exported defines ----------------------------------------------------------*/

//for different dongle product
#define USB_EF_DONGLE_VID        0x2C7C
#define EF_DONGLE_PID_EG915      0x0901
#define EF_DONGLE_PID_EG91       0x0191

#define USB_DEFAULT_VID       0x0BDA

typedef enum {
    USBH_ECM_RX_THREAD_PRIORITY           = 5,
    USBH_ECM_INIT_THREAD_PRIORITY         = 6,
    USBH_ECM_MAIN_THREAD_PRIORITY         = 6,  //usb process
    USBH_ECM_ISR_THREAD_PRIORITY          = 7,  //usb core isr
    USBH_ECM_HOTPLUG_THREAD_PRIORITY      = 8,  //hotplug
} usbh_cdc_ecm_task_priority_t;

typedef struct {
    u16 vid;               /*dongle vid*/
    u16 pid;               /*dongle pid*/
    u8 at_line_idx;        /*dongle at cmd interface idx*/
} usbh_cdc_ecm_appx_param_t;

/* Exported types ------------------------------------------------------------*/
/* CDC ECM data report interface */
typedef void (*usb_report_usbdata)(u8 *buf, u32 len);
/* CDC ECM appx data report interface */
typedef void (*usb_appx_report)(u8 *buf, u32 len,u8 type);

typedef struct {
	usb_appx_report                     appx_cb;       /* app callback api */
	u8                                  *mac_value;    /* mac value */
	u16                                 *led_array;    /* led array */
	u8                                  led_cnt;       /* led cnt */
} usbh_cdc_ecm_priv_data_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* USB Common Api */
/* ECM Init Api */
u8          usbh_cdc_ecm_do_init(usb_report_usbdata ecm_cb,usbh_cdc_ecm_priv_data_t *priv);
u8          usbh_cdc_ecm_do_deinit(void);
u8          usbh_cdc_ecm_usb_is_ready(void);

/* Lwip Api */
const u8*   usbh_cdc_ecm_process_mac_str(void);
u16         usbh_cdc_ecm_get_receive_mps(void);
u8          usbh_cdc_ecm_send_data(u8 *buf, u32 len);

/* Example Api */
u16         usbh_cdc_ecm_get_device_vid(void);
u16         usbh_cdc_ecm_get_device_pid(void);
u8          usbh_cdc_ecm_get_connect_status(void);
u8          usbh_cdc_ecm_appx_send_data(u8 *buf, u32 len);
u8          usbh_cdc_ecm_appx_set_param(usbh_cdc_ecm_appx_param_t *param);
u8          usbh_cdc_ecm_hex_to_char(u8 hex_num);

#endif  /* USBH_CDC_ECM_HAL_H */

