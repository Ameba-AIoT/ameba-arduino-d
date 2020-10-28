#ifndef _US_USB_H_
#define _US_USB_H_

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

#include "usb.h"
#include "us_os_wrap_via_osdep_api.h"
#include "us_debug.h"
//#include "sg.h"

struct us_data;
struct scsi_cmnd;

#define CONFIG_SG	0
#define CONFIG_DMA	0

/*
 * Unusual device list definitions 
 */

struct us_unusual_dev {
	const char* vendorName;
	const char* productName;
	__u8  useProtocol;
	__u8  useTransport;
	int (*initFunction)(struct us_data *);
};

/* Flag definitions: these entries are static */
#define US_FL_SINGLE_LUN      0x00000001 /* allow access to only LUN 0	    */
//#define US_FL_MODE_XLATE      0          /* [no longer used]                */
#define US_FL_NEED_OVERRIDE   0x00000004 /* unusual_devs entry is necessary */
//#define US_FL_IGNORE_SER      0		 /* [no longer used]		    */
#define US_FL_SCM_MULT_TARG   0x00000020 /* supports multiple targets	    */
#define US_FL_FIX_INQUIRY     0x00000040 /* INQUIRY response needs faking   */
#define US_FL_FIX_CAPACITY    0x00000080 /* READ CAPACITY response too big  */
#define US_FL_IGNORE_RESIDUE  0x00000100 /* reported residue is wrong	    */
#define US_FL_BULK32          0x00000200 /* Uses 32-byte CBW length         */


/* Dynamic bitflag definitions (us->dflags): used in set_bit() etc. */
#define US_FLIDX_URB_ACTIVE	0	/* current_urb is in use    */
#define US_FLIDX_SG_ACTIVE	1	/* current_sg is in use     */
#define US_FLIDX_ABORTING	2	/* abort is in progress     */
#define US_FLIDX_DISCONNECTING	3	/* disconnect in progress   */
#define US_FLIDX_RESETTING	4	/* device reset in progress */
#define US_FLIDX_TIMED_OUT	5	/* SCSI midlayer timed out  */
#define US_FLIDX_SCAN_PENDING	6	/* scanning not yet done    */
#define US_FLIDX_REDO_READ10	7	/* redo READ(10) command    */
#define US_FLIDX_READ10_WORKED	8	/* previous READ(10) succeeded */

#define USB_STOR_STRING_LEN 32

/*
 * We provide a DMA-mapped I/O buffer for use with small USB transfers.
 * It turns out that CB[I] needs a 12-byte buffer and Bulk-only needs a
 * 31-byte buffer.  But Freecom needs a 64-byte buffer, so that's the
 * size we'll allocate.
 */

#define US_IOBUF_SIZE		64	/* Size of the DMA-mapped I/O buffer */
#define US_SENSE_SIZE		18	/* Size of the autosense data buffer */

typedef int (*trans_cmnd)(struct scsi_cmnd *, struct us_data*);
typedef int (*trans_reset)(struct us_data*);
typedef void (*proto_cmnd)(struct scsi_cmnd*, struct us_data*);
typedef void (*extra_data_destructor)(void *);	/* extra data destructor */
typedef void (*pm_hook)(struct us_data *, int);	/* power management hook */

#define US_SUSPEND	0
#define US_RESUME	1

/* we allocate one of these for every device that we remember */
struct us_data {
	/* The device we're working with
	 * It's important to note:
	 *    (o) you must hold dev_mutex to change pusb_dev
	 */
	_mutex		dev_mutex;
	struct usb_device	*pusb_dev;	 /* this usb_device */
	struct usb_interface	*pusb_intf;	 /* this interface */
	struct us_unusual_dev   *unusual_dev;	 /* device-filter entry     */
	unsigned long		fflags;		 /* fixed flags from filter */
	unsigned long		dflags;		 /* dynamic atomic bitflags */
	unsigned int		send_bulk_pipe;	 /* cached pipe values */
	unsigned int		recv_bulk_pipe;
	unsigned int		send_ctrl_pipe;
	unsigned int		recv_ctrl_pipe;
	unsigned int		recv_intr_pipe;

	/* information about the device */
	char			*transport_name;
	char			*protocol_name;
	__le32			bcs_signature;
	u8			subclass;
	u8			protocol;
	u8			max_lun;  // max number of logical unit (0,1,2,3...)

	u8			ifnum;		 /* interface number   */
	u8			ep_bInterval;	 /* interrupt interval */ 

	/* function pointers for this device */
	trans_cmnd		transport;	 /* transport function	   */
	trans_reset		transport_reset; /* transport device reset */
	proto_cmnd		proto_handler;	 /* protocol handler	   */

	/* SCSI interfaces */
	struct scsi_cmnd	*srb;		 /* current srb		*/
	unsigned int		tag;		 /* current dCBWTag	*/

	/* control and bulk communications data */
	struct urb		*current_urb;	 /* USB requests	 */
	struct usb_ctrlrequest	*cr;		 /* control requests	 */
//	struct usb_sg_request	current_sg;	 /* scatter-gather req.  */
	unsigned char		*iobuf;		 /* I/O buffer		 */
	dma_addr_t		iobuf_dma;	 /* buffer DMA addresses */

	xTaskHandle ctl_task; /*the control task handle*/

	/* mutual exclusion and synchronization structures */
	_sema cmnd_ready;	 /* to sleep thread on	    */
	_mutex	notify;		 /* thread begin/end	    */

	unsigned no_sg_constraint:1;	/* no sg constraint */
	unsigned sg_tablesize;		/* 0 or largest number of sg list entries */

	/* subdriver information */
	void			*extra;		 /* Any extra data          */
};

struct us_usr_cb {
	void(* attach)(void);
	void(* detach)(void);
};
/* Convert between us_data and the corresponding Scsi_Host */
//static inline struct Scsi_Host *us_to_host(struct us_data *us) {
//	return container_of((void *) us, struct Scsi_Host, hostdata);
//}
//static inline struct us_data *host_to_us(struct Scsi_Host *host) {
//	return (struct us_data *) host->hostdata;
//}

/* Function to fill an inquiry response. See usb.c for details */
extern void fill_inquiry_response(struct us_data *us,
	unsigned char *data, unsigned int data_len);

/* The scsi_lock() and scsi_unlock() macros protect the sm_state and the
 * single queue element srb for write access */
//#define scsi_unlock(host)	spin_unlock_irq(host->host_lock)
//#define scsi_lock(host)	spin_lock_irq(host->host_lock)
#define scsi_unlock(host)	spin_unlock(host->host_lock)
#define scsi_lock(host)		spin_lock(host->host_lock)

/* General routines provided by the usb-storage standard core */
#ifdef CONFIG_USBH_MSC_PM
extern int usb_stor_suspend(struct usb_interface *iface, pm_message_t message);
extern int usb_stor_resume(struct usb_interface *iface);
extern int usb_stor_reset_resume(struct usb_interface *iface);
#else
#define usb_stor_suspend	NULL
#define usb_stor_resume		NULL
#define usb_stor_reset_resume	NULL
#endif

extern int usb_stor_pre_reset(struct usb_interface *iface);
extern int usb_stor_post_reset(struct usb_interface *iface);

extern int usb_stor_probe1(struct us_data *us,
		struct usb_interface *intf,
		const struct usb_device_id *id,
		struct us_unusual_dev *unusual_dev);
extern int usb_stor_probe2(struct us_data *us);
extern void usb_stor_disconnect(struct usb_interface *intf);
extern void usb_msc_register_usr_cb(struct us_usr_cb *cb);

extern void usb_stor_adjust_quirks(struct usb_device *dev,
		unsigned long *fflags);

/* USB port reset for device reinitialization */
extern int usb_reset_device(struct usb_device *dev);
extern void usb_queue_reset_device(struct usb_interface *dev);

extern void *usb_alloc_coherent(struct usb_device *dev, size_t size,
	gfp_t mem_flags, dma_addr_t *dma);
extern void usb_free_coherent(struct usb_device *dev, size_t size,
	void *addr, dma_addr_t dma);


// copy from transport.h
extern int usb_stor_Bulk_transport(struct scsi_cmnd *, struct us_data*);
extern int usb_stor_Bulk_max_lun(struct us_data*);
extern int usb_stor_Bulk_reset(struct us_data*);

extern void usb_stor_invoke_transport(struct scsi_cmnd *, struct us_data*);
extern void usb_stor_stop_transport(struct us_data*);

// copy form protocol.h
extern void usb_stor_transparent_scsi_command(struct scsi_cmnd* srb, struct us_data* );

extern unsigned char usb_stor_sense_invalidCDB[18];

#endif // CONFIG_USBH_MSC

#endif // _US_USB_H_

