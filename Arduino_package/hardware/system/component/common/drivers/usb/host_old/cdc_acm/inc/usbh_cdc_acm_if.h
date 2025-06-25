#ifndef USBH_CDC_ACM_IF_H
#define USBH_CDC_ACM_IF_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_CDC_ACM

#define BULK_OUT_TRANSFER_SIZE 	1024
#define BULK_IN_TRANSFER_SIZE		1024 


/* wValue for SetControlLineState*/
#define CDC_ACTIVATE_CARRIER_SIGNAL_RTS                         0x0002
#define CDC_DEACTIVATE_CARRIER_SIGNAL_RTS                       0x0000
#define CDC_ACTIVATE_SIGNAL_DTR                                 0x0001
#define CDC_DEACTIVATE_SIGNAL_DTR                               0x0000

/*Value for bCharFormat*/
#define USB_CDC_1_STOP_BITS			0
#define USB_CDC_1_5_STOP_BITS			1
#define USB_CDC_2_STOP_BITS			2

/*Value for bParityType*/
#define USB_CDC_NO_PARITY			0
#define USB_CDC_ODD_PARITY			1
#define USB_CDC_EVEN_PARITY			2
#define USB_CDC_MARK_PARITY			3
#define USB_CDC_SPACE_PARITY			4

/*Line coding structure size*/
#define LINE_CODING_STRUCTURE_SIZE  0x07

/*Line coding structure*/
typedef union CDC_LineCodingStructure
{
	u8 Array[LINE_CODING_STRUCTURE_SIZE];
	struct{
		u32 dwDTERate; 		/*Data terminal rate, in bits per second*/
		u8 bCharFormat; 		/*Stop bits*/
		u8 bParityType;		/* Parity*/
		u8 bDataBits;		/* Data bits (5, 6, 7, 8 or 16). */
	}b;
}
CDC_LineCodingTypeDef;

/*user callback function*/
typedef struct {
	void(* init)(void);
	void(* deinit)(void);
	void(* attach)(void);
	void(* detach)(void);
	void(* receive)(u8 *buf, u32 length);
	void(*transmit_complete)(int status);
} usbh_cdc_acm_usr_cb_t;

/**
  * @brief  Initializes the CDC ACM host.
  * @param  bulk_in_transfer_size: bulk in transfer size. Max value is 65536 and 0 means default value 1024.
  * @param  bulk_out_transfer_size: bulk out transfer size. Max value is 65536 and 0 means default value 1024.
  * @param  cb: user call back function. please refer to struct  usbh_cdc_acm_usr_cb_t.
  * @retval status. 
  */
int usbh_cdc_acm_init(u32 bulk_in_transfer_size,u32 bulk_out_transfer_size, usbh_cdc_acm_usr_cb_t *cb);

void usbh_cdc_acm_deinit(void);
int usbh_cdc_acm_write(u8 * buf, int count);
int usbh_cdc_acm_read_open(void);
int usbh_cdc_acm_read_close(void);
int usbh_cdc_acm_set_line_coding(CDC_LineCodingTypeDef * LCStruct);
int usbh_cdc_acm_get_line_coding(CDC_LineCodingTypeDef * LCStruct);
int usbh_cdc_acm_send_encapsulated_command(u8* buf, u32 len);
int usbh_cdc_acm_get_encapsulated_command(u8* buf, u32 len);
int usbh_cdc_acm_set_ctrl_line_state(u32 ctrl);

#endif // CONFIG_USBH_CDC_ACM

#endif // USBH_CDC_ACM_IF_H

