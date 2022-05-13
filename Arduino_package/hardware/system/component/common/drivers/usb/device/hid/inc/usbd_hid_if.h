#ifndef USBD_HID_IF_H
#define USBD_HID_IF_H

#include <platform_opts.h>
#ifdef CONFIG_USBD_HID

#include "usb.h"

typedef struct {
	void(* init)(void);
 	void(* deinit)(void);
	void(*transmit_complete)(int status);
} usbd_hid_usr_cb_t;

struct mouse_data{
	u8 left;			//left button. 0: release, 1: press
	u8 right;			//right button. 0: release, 1: press
	u8 middle;			//wheel button. 0: release, 1: press
	char x_axis;		//x-axis pixels. relative value from -127 to 127, positive for right and negative for left 
	char y_axis;		//y-axis pixels. relative value from -127 to 127, positive for up and negative for down 
	char wheel;			//scrolling units. relative value from -127 to 127, positive for up and negative for down. 
};


int usbd_hid_init(usbd_hid_usr_cb_t *cb);
void usbd_hid_deinit(void);
int usbd_hid_send_mouse(struct mouse_data *data);
int usbd_hid_status(void);

#endif // CONFIG_USBD_HID

#endif // USBD_HID_IF_H