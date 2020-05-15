#ifndef _USBD_CDC_ACM_IF_H_
#define _USBD_CDC_ACM_IF_H_

#include <platform_opts.h>

#ifdef CONFIG_USBD_CDC_ACM

typedef struct {
    int(* init)(void);
    int(* deinit)(void);
    int(* receive)(void *buf, u16 length);
    void(*transmit_complete)(int status);
} usbd_cdc_acm_usr_cb_t;

int usbd_cdc_acm_sync_transmit_data(void *buf, u16 length);
int usbd_cdc_acm_async_transmit_data(void *buf, u32 length);

int usbd_cdc_acm_init(u16 bulk_in_buf_size, u16 bulk_out_buf_size, usbd_cdc_acm_usr_cb_t *cb);
void usbd_cdc_acm_deinit(void);

#endif // CONFIG_USBD_CDC_ACM

#endif // _USBD_CDC_ACM_IF_H_

