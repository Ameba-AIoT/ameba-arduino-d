#ifndef _USBD_CDC_ACM_IF_H_
#define _USBD_CDC_ACM_IF_H_

#include <platform_opts.h>

#ifdef CONFIG_USBD_CDC_ACM

// Use INT in endpoint
#define USBD_CDC_ACM_USE_INT_IN_ENDPOINT         1

#define USBD_CDC_ACM_USE_NOTIFY_SERIAL_STATE     0   //notification function is not neccessary normally
#if USBD_CDC_ACM_USE_NOTIFY_SERIAL_STATE
#if !USBD_CDC_ACM_USE_INT_IN_ENDPOINT
#error "You should enable interrupt in endpoint if you want to use notify serial state function"
#endif
#endif

typedef struct {
    int(* init)(void);
    int(* deinit)(void);
    int(* receive)(void *buf, u16 length);
    void(*transmit_complete)(int status);
} usbd_cdc_acm_usr_cb_t;


#if USBD_CDC_ACM_USE_NOTIFY_SERIAL_STATE
int usbd_cdc_acm_notify_serial_state(u16 serial_state);
#endif
int usbd_cdc_acm_sync_transmit_data(void *buf, u16 length);
int usbd_cdc_acm_async_transmit_data(void *buf, u32 length);

int usbd_cdc_acm_init(int speed, usbd_cdc_acm_usr_cb_t *cb);
void usbd_cdc_acm_deinit(void);

#endif // CONFIG_USBD_CDC_ACM

#endif // _USBD_CDC_ACM_IF_H_

