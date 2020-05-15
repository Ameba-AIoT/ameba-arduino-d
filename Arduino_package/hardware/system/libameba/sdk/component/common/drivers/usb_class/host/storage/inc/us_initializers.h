#include "usb.h"
#include "transport.h"

/* This places the Shuttle/SCM USB<->SCSI bridge devices in multi-target
 * mode */
int usb_stor_euscsi_init(struct us_data *us);

/* This function is required to activate all four slots on the UCR-61S2B
 * flash reader */
int usb_stor_ucr61s2b_init(struct us_data *us);

/* This places the HUAWEI E220 devices in multi-port mode */
int usb_stor_huawei_e220_init(struct us_data *us);

