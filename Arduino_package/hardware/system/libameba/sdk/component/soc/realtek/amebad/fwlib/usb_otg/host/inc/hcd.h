#ifndef _HCD_H_
#define _HCD_H_


struct hc_driver {
        const char      *description;   /* "ehci-hcd" etc */
const char      *product_desc;  /* product/vendor string */
size_t          hcd_priv_size;  /* size of private data */

/* irq handler */
//irqreturn_t     (*irq) (struct usb_hcd *hcd);

int     flags;
#define HCD_MEMORY      0x0001          /* HC regs use memory (else I/O) */
#define HCD_LOCAL_MEM   0x0002          /* HC needs local memory */
#define HCD_SHARED      0x0004          /* Two (or more) usb_hcds share HW */
#define HCD_USB11       0x0010          /* USB 1.1 */
#define HCD_USB2        0x0020          /* USB 2.0 */
#define HCD_USB3        0x0040          /* USB 3.0 */
#define HCD_MASK        0x0070

        /* called to init HCD and root hub */
        int     (*reset) (struct usb_hcd *hcd);
        int     (*start) (struct usb_hcd *hcd);

        /* NOTE:  these suspend/resume calls relate to the HC as
         * a whole, not just the root hub; they're for PCI bus glue.
         */
        /* called after suspending the hub, before entering D3 etc */
//        int     (*pci_suspend)(struct usb_hcd *hcd, bool do_wakeup);

        /* called after entering D0 (etc), before resuming the hub */
//        int     (*pci_resume)(struct usb_hcd *hcd, bool hibernated);

        /* cleanly make HCD stop writing memory and doing I/O */
        void    (*stop) (struct usb_hcd *hcd);

        /* shutdown HCD */
  //      void    (*shutdown) (struct usb_hcd *hcd);

        /* return current frame number */
        int     (*get_frame_number) (struct usb_hcd *hcd);

        /* manage i/o requests, device state */
        int     (*urb_enqueue)(struct usb_hcd *hcd,
                                struct urb *urb);//, gfp_t mem_flags);
        int     (*urb_dequeue)(struct usb_hcd *hcd,
                                struct urb *urb, int status);

        /*
         * (optional) these hooks allow an HCD to override the default DMA
         * mapping and unmapping routines.  In general, they shouldn't be
         * necessary unless the host controller has special DMA requirements,
         * such as alignment contraints.  If these are not specified, the
         * general usb_hcd_(un)?map_urb_for_dma functions will be used instead
         * (and it may be a good idea to call these functions in your HCD
         * implementation)
         */
        /* hw synch, freeing endpoint resources that urb_dequeue can't */
        void    (*endpoint_disable)(struct usb_hcd *hcd,
                        struct usb_host_endpoint *ep);

        /* (optional) reset any endpoint state such as sequence number
           and current window */
        void    (*endpoint_reset)(struct usb_hcd *hcd,
                        struct usb_host_endpoint *ep);

        /* root hub support */
        int     (*hub_status_data) (struct usb_hcd *hcd, char *buf);
        int     (*hub_control) (struct usb_hcd *hcd,
                                u16 typeReq, u16 wValue, u16 wIndex,
                                char *buf, u16 wLength);
};

#endif
