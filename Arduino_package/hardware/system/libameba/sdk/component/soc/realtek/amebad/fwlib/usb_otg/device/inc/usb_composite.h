#ifndef _USB_COMPOSITE_H_
#define _USB_COMPOSITE_H_

#include "usb_gadget.h"
#include "usb.h"

#define MAX_CONFIG_INTERFACES           16  /* arbitrary; max 255 */

// EP0 Data Direction
#define EP0_DATA_OUT 0
#define EP0_DATA_IN  1

// Default PID/VID
#define REALTEK_USB_VID             0x0BDA
#define REALTEK_USB_PID             0x4042

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

// Predefine Structure
struct usb_composite_dev;
struct usb_composite_driver;

/**
 * struct usb_configuration - represents one gadget configuration
 * @label: For diagnostics, describes the configuration.
 * @strings: Tables of strings, keyed by identifiers assigned during @bind()
 *  and by language IDs provided in control requests.
 * @descriptors: Table of descriptors preceding all function descriptors.
 *  Examples include OTG and vendor-specific descriptors.
 * @unbind: Reverses @bind; called as a side effect of unregistering the
 *  driver which added this configuration.
 * @setup: Used to delegate control requests that aren't handled by standard
 *  device infrastructure or directed at a specific interface.
 * @bConfigurationValue: Copied into configuration descriptor.
 * @iConfiguration: Copied into configuration descriptor.
 * @bmAttributes: Copied into configuration descriptor.
 * @MaxPower: Power consumtion in mA. Used to compute bMaxPower in the
 *  configuration descriptor after considering the bus speed.
 * @cdev: assigned by @usb_add_config() before calling @bind(); this is
 *  the device associated with this configuration.
 *
 * Configurations are building blocks for gadget drivers structured around
 * function drivers.  Simple USB gadgets require only one function and one
 * configuration, and handle dual-speed hardware by always providing the same
 * functionality.  Slightly more complex gadgets may have more than one
 * single-function configuration at a given speed; or have configurations
 * that only work at one speed.
 *
 * Composite devices are, by definition, ones with configurations which
 * include more than one function.
 *
 * The lifecycle of a usb_configuration includes allocation, initialization
 * of the fields described above, and calling @usb_add_config() to set up
 * internal data and bind it to a specific device.  The configuration's
 * @bind() method is then used to initialize all the functions and then
 * call @usb_add_function() for them.
 *
 * Those functions would normally be independent of each other, but that's
 * not mandatory.  CDC WMC devices are an example where functions often
 * depend on other functions, with some functions subsidiary to others.
 * Such interdependency may be managed in any way, so long as all of the
 * descriptors complete by the time the composite driver returns from
 * its bind() routine.
 */
struct usb_configuration {
    //const char          *label;
    struct usb_gadget_strings   **strings;
    const struct usb_descriptor_header **descriptors;

    /* REVISIT:  bind() functions can be marked __init, which
     * makes trouble for section mismatch analysis.  See if
     * we can't restructure things to avoid mismatching...
     */

    /* configuration management: unbind/setup */
    //void (*unbind)(struct usb_configuration *);
    //int (*setup)(struct usb_configuration *, const struct usb_control_request *);

#if 1
    struct usb_config_descriptor *config_descriptor;
#else
    /* fields in the config descriptor */
    uint8_t         bConfigurationValue;
    uint8_t         iConfiguration;
    uint8_t         bmAttributes;
    uint16_t        MaxPower;
#endif

    struct usb_composite_dev    *cdev;
    dwc_list_link_t list;
    dwc_list_link_t function_lists;

    uint8_t         next_interface_id;
    unsigned        superspeed: 1;
    unsigned        highspeed: 1;
    unsigned        fullspeed: 1;
    struct usb_function *interface[MAX_CONFIG_INTERFACES];
};

/**
 * struct usb_composite_driver - groups configurations into a gadget
 * @name: For diagnostics, identifies the driver.
 * @dev: Template descriptor for the device, including default device
 *  identifiers.
 * @strings: tables of strings, keyed by identifiers assigned during @bind
 *  and language IDs provided in control requests. Note: The first entries
 *  are predefined. The first entry that may be used is
 *  USB_GADGET_FIRST_AVAIL_IDX
 * @max_speed: Highest speed the driver supports.
 * @needs_serial: set to 1 if the gadget needs userspace to provide
 *  a serial number.  If one is not provided, warning will be printed.
 * @bind: (REQUIRED) Used to allocate resources that are shared across the
 *  whole device, such as string IDs, and add its configurations using
 *  @usb_add_config(). This may fail by returning a negative errno
 *  value; it should return zero on successful initialization.
 * @unbind: Reverses @bind; called as a side effect of unregistering
 *  this driver.
 * @disconnect: optional driver disconnect method
 * @suspend: Notifies when the host stops sending USB traffic,
 *  after function notifications
 * @resume: Notifies configuration when the host restarts USB traffic,
 *  before function notifications
 * @gadget_driver: Gadget driver controlling this driver
 *
 * Devices default to reporting self powered operation.  Devices which rely
 * on bus powered operation should report this in their @bind method.
 *
 * Before returning from @bind, various fields in the template descriptor
 * may be overridden.  These include the idVendor/idProduct/bcdDevice values
 * normally to bind the appropriate host side driver, and the three strings
 * (iManufacturer, iProduct, iSerialNumber) normally used to provide user
 * meaningful device identifiers.  (The strings will not be defined unless
 * they are defined in @dev and @strings.)  The correct ep0 maxpacket size
 * is also reported, as defined by the underlying controller driver.
 */

struct usb_composite_driver {
    //const char                     *name;
    //enum usb_device_speed           max_speed;
    struct usb_device_descriptor   *dev_desc;
    struct usb_config_descriptor   *config_desc;
    struct usb_gadget_strings     **strings;
    struct usb_function            *functions;

    int (*config)(struct usb_configuration *cfg);

    //int (*bind)(struct usb_composite_dev *cdev);
    //int (*unbind)(struct usb_composite_dev *cdev);

    //void (*disconnect)(struct usb_composite_dev *cdev);

    /* global suspend hooks */
    //void (*suspend)(struct usb_composite_dev *cdev);
    //void (*resume)(struct usb_composite_dev *cdev);
    struct usb_gadget_driver        gadget_driver;
    void                           *driver_data;
};
/**
 * struct usb_composite_device - represents one composite usb gadget
 * @gadget: read-only, abstracts the gadget's usb peripheral controller
 * @req: used for control responses; buffer is pre-allocated
 * @os_desc_req: used for OS descriptors responses; buffer is pre-allocated
 * @config: the currently active configuration
 * @qw_sign: qwSignature part of the OS string
 * @b_vendor_code: bMS_VendorCode part of the OS string
 * @use_os_string: false by default, interested gadgets set it
 * @os_desc_config: the configuration to be used with OS descriptors
 *
 * One of these devices is allocated and initialized before the
 * associated device driver's bind() is called.
 *
 * OPEN ISSUE:  it appears that some WUSB devices will need to be
 * built by combining a normal (wired) gadget with a wireless one.
 * This revision of the gadget framework should probably try to make
 * sure doing that won't hurt too much.
 *
 * One notion for how to handle Wireless USB devices involves:
 * (a) a second gadget here, discovery mechanism TBD, but likely
 *     needing separate "register/unregister WUSB gadget" calls;
 * (b) updates to usb_gadget to include flags "is it wireless",
 *     "is it wired", plus (presumably in a wrapper structure)
 *     bandgroup and PHY info;
 * (c) presumably a wireless_ep wrapping a usb_ep, and reporting
 *     wireless-specific parameters like maxburst and maxsequence;
 * (d) configurations that are specific to wireless links;
 * (e) function drivers that understand wireless configs and will
 *     support wireless for (additional) function instances;
 * (f) a function to support association setup (like CBAF), not
 *     necessarily requiring a wireless adapter;
 * (g) composite device setup that can create one or more wireless
 *     configs, including appropriate association setup support;
 * (h) more, TBD.
 */

struct usb_composite_dev {
    struct usb_gadget           *gadget;
    struct usb_request          *req;
    struct usb_configuration    *config;
    struct usb_device_descriptor dev_desc;

    dwc_list_link_t              config_list; // by jimmy
    dwc_list_link_t              gstring_list;// by jimmy

    struct usb_composite_driver *driver;
    unsigned int                 suspended: 1;
};

/**
 * struct usb_function - describes one function of a configuration
 * @name: For diagnostics, identifies the function.
 * @strings: tables of strings, keyed by identifiers assigned during bind()
 *  and by language IDs provided in control requests
 * @fs_descriptors: Table of full (or low) speed descriptors, using interface and
 *  string identifiers assigned during @bind().  If this pointer is null,
 *  the function will not be available at full speed (or at low speed).
 * @hs_descriptors: Table of high speed descriptors, using interface and
 *  string identifiers assigned during @bind().  If this pointer is null,
 *  the function will not be available at high speed.
 * @ss_descriptors: Table of super speed descriptors, using interface and
 *  string identifiers assigned during @bind(). If this
 *  pointer is null after initiation, the function will not
 *  be available at super speed.
 * @config: assigned when @usb_add_function() is called; this is the
 *  configuration with which this function is associated.
 * @os_desc_table: Table of (interface id, os descriptors) pairs. The function
 *  can expose more than one interface. If an interface is a member of
 *  an IAD, only the first interface of IAD has its entry in the table.
 * @os_desc_n: Number of entries in os_desc_table
 * @bind: Before the gadget can register, all of its functions bind() to the
 *  available resources including string and interface identifiers used
 *  in interface or class descriptors; endpoints; I/O buffers; and so on.
 * @unbind: Reverses @bind; called as a side effect of unregistering the
 *  driver which added this function.
 * @free_func: free the struct usb_function.
 * @set_alt: (REQUIRED) Reconfigures altsettings; function drivers may
 *  initialize usb_ep.driver data at this time (when it is used).
 *  Note that setting an interface to its current altsetting resets
 *  interface state, and that all interfaces have a disabled state.
 * @get_alt: Returns the active altsetting.  If this is not provided,
 *  then only altsetting zero is supported.
 * @disable: (REQUIRED) Indicates the function should be disabled.  Reasons
 *  include host resetting or reconfiguring the gadget, and disconnection.
 * @setup: Used for interface-specific control requests.
 * @suspend: Notifies functions when the host stops sending USB traffic.
 * @resume: Notifies functions when the host restarts USB traffic.
 * @get_status: Returns function status as a reply to
 *  GetStatus() request when the recipient is Interface.
 * @func_suspend: callback to be called when
 *  SetFeature(FUNCTION_SUSPEND) is reseived
 *
 * A single USB function uses one or more interfaces, and should in most
 * cases support operation at both full and high speeds.  Each function is
 * associated by @usb_add_function() with a one configuration; that function
 * causes @bind() to be called so resources can be allocated as part of
 * setting up a gadget driver.  Those resources include endpoints, which
 * should be allocated using @usb_ep_autoconfig().
 *
 * To support dual speed operation, a function driver provides descriptors
 * for both high and full speed operation.  Except in rare cases that don't
 * involve bulk endpoints, each speed needs different endpoint descriptors.
 *
 * Function drivers choose their own strategies for managing instance data.
 * The simplest strategy just declares it "static', which means the function
 * can only be activated once.  If the function needs to be exposed in more
 * than one configuration at a given speed, it needs to support multiple
 * usb_function structures (one for each configuration).
 *
 * A more complex strategy might encapsulate a @usb_function structure inside
 * a driver-specific instance structure to allows multiple activations.  An
 * example of multiple activations might be a CDC ACM function that supports
 * two or more distinct instances within the same configuration, providing
 * several independent logical data links to a USB host.
 */

struct usb_function {
    //const char                    *name;
    struct usb_gadget_strings    **strings;
    struct usb_descriptor_header **fs_descriptors;
    struct usb_descriptor_header **hs_descriptors;
    struct usb_configuration      *config;

    /* REVISIT:  bind() functions can be marked __init, which
     * makes trouble for section mismatch analysis.  See if
     * we can't restructure things to avoid mismatching.
     * Related:  unbind() may kfree() but bind() won't...
     */

    /* configuration management:  bind/unbind */
    int (*bind)(struct usb_configuration *, struct usb_function *);
    void (*unbind)(struct usb_configuration *, struct usb_function *);
    void (*free_func)(struct usb_function *f);

    /* runtime state management */
    int (*set_alt)(struct usb_function *, unsigned interface, unsigned alt);
    int (*get_alt)(struct usb_function *, unsigned interface);
    void (*disable)(struct usb_function *);
    int (*setup)(struct usb_function *, const struct usb_control_request *);
    void (*suspend)(struct usb_function *);
    void (*resume)(struct usb_function *);

    /* USB 3.0 additions */
    int (*get_status)(struct usb_function *);
    dwc_list_link_t list; // by jimmy
    uint32_t function_endp_bitmap;//bitmap// Rom2 add: store which endpoint belongs to this function
};

inline struct usb_endpoint_descriptor *usb_get_descriptor(struct usb_gadget *gadget,
    struct usb_endpoint_descriptor *hs, struct usb_endpoint_descriptor *fs)
{
    return (gadget->speed == USB_SPEED_HIGH) ? hs : fs;
}

/*-------------------------------------------------------------------------*/

/**
 * usb_ep_enable - configure endpoint, making it usable
 * @ep:the endpoint being configured.  may not be the endpoint named "ep0".
 *  drivers discover endpoints through the ep_list of a usb_gadget.
 * @desc:descriptor for desired behavior.  caller guarantees this pointer
 *  remains valid until the endpoint is disabled; the data byte order
 *  is little-endian (usb-standard).
 *
 * when configurations are set, or when interface settings change, the driver
 * will enable or disable the relevant endpoints.  while it is enabled, an
 * endpoint may be used for i/o until the driver receives a disconnect() from
 * the host or until the endpoint is disabled.
 *
 * the ep0 implementation (which calls this routine) must ensure that the
 * hardware capabilities of each endpoint match the descriptor provided
 * for it.  for example, an endpoint named "ep2in-bulk" would be usable
 * for interrupt transfers as well as bulk, but it likely couldn't be used
 * for iso transfers or for endpoint 14.  some endpoints are fully
 * configurable, with more generic names like "ep-a".  (remember that for
 * USB, "in" means "towards the USB master".)
 *
 * returns zero, or a negative error code.
 */
inline int usb_ep_enable(struct usb_ep *ep, const struct usb_endpoint_descriptor *desc)
{
    return ep->ops->enable(ep, desc);
}

/**
 * usb_ep_disable - endpoint is no longer usable
 * @ep:the endpoint being unconfigured.  may not be the endpoint named "ep0".
 *
 * no other task may be using this endpoint when this is called.
 * any pending and uncompleted requests will complete with status
 * indicating disconnect (-USB_ESHUTDOWN) before this call returns.
 * gadget drivers must call usb_ep_enable() again before queueing
 * requests to the endpoint.
 *
 * returns zero, or a negative error code.
 */
inline int usb_ep_disable(struct usb_ep *ep)
{
    return ep->ops->disable(ep);    // ep_disable()
}

/**
 * usb_ep_alloc_request - allocate a request object to use with this endpoint
 * @ep:the endpoint to be used with with the request
 * @gfp_flags:GFP_* flags to use
 *
 * Request objects must be allocated with this call, since they normally
 * need controller-specific setup and may even need endpoint-specific
 * resources such as allocation of DMA descriptors.
 * Requests may be submitted with usb_ep_queue(), and receive a single
 * completion callback.  Free requests with usb_ep_free_request(), when
 * they are no longer needed.
 *
 * Returns the request, or null if one could not be allocated.
 */
inline struct usb_request *usb_ep_alloc_request(struct usb_ep *ep, gfp_t gfp_flags)
{
    return ep->ops->alloc_request(ep, gfp_flags);  //dwc_otg_pcd_alloc_request()
}

/**
 * usb_ep_free_request - frees a request object
 * @ep:the endpoint associated with the request
 * @req:the request being freed
 *
 * Reverses the effect of usb_ep_alloc_request().
 * Caller guarantees the request is not queued, and that it will
 * no longer be requeued (or otherwise used).
 */
inline void usb_ep_free_request(struct usb_ep *ep, struct usb_request *req)
{
    ep->ops->free_request(ep, req); // dwc_otg_pcd_free_request()
}

/**
 * usb_ep_queue - queues (submits) an I/O request to an endpoint.
 * @ep:the endpoint associated with the request
 * @req:the request being submitted
 * @gfp_flags: GFP_* flags to use in case the lower level driver couldn't
 *  pre-allocate all necessary memory with the request.
 *
 * This tells the device controller to perform the specified request through
 * that endpoint (reading or writing a buffer).  When the request completes,
 * including being canceled by usb_ep_dequeue(), the request's completion
 * routine is called to return the request to the driver.  Any endpoint
 * (except control endpoints like ep0) may have more than one transfer
 * request queued; they complete in FIFO order.  Once a gadget driver
 * submits a request, that request may not be examined or modified until it
 * is given back to that driver through the completion callback.
 *
 * Each request is turned into one or more packets.  The controller driver
 * never merges adjacent requests into the same packet.  OUT transfers
 * will sometimes use data that's already buffered in the hardware.
 * Drivers can rely on the fact that the first byte of the request's buffer
 * always corresponds to the first byte of some USB packet, for both
 * IN and OUT transfers.
 *
 * Bulk endpoints can queue any amount of data; the transfer is packetized
 * automatically.  The last packet will be short if the request doesn't fill it
 * out completely.  Zero length packets (ZLPs) should be avoided in portable
 * protocols since not all usb hardware can successfully handle zero length
 * packets.  (ZLPs may be explicitly written, and may be implicitly written if
 * the request 'zero' flag is set.)  Bulk endpoints may also be used
 * for interrupt transfers; but the reverse is not true, and some endpoints
 * won't support every interrupt transfer.  (Such as 768 byte packets.)
 *
 * Interrupt-only endpoints are less functional than bulk endpoints, for
 * example by not supporting queueing or not handling buffers that are
 * larger than the endpoint's maxpacket size.  They may also treat data
 * toggle differently.
 *
 * Control endpoints ... after getting a setup() callback, the driver queues
 * one response (even if it would be zero length).  That enables the
 * status ack, after transfering data as specified in the response.  Setup
 * functions may return negative error codes to generate protocol stalls.
 * (Note that some USB device controllers disallow protocol stall responses
 * in some cases.)  When control responses are deferred (the response is
 * written after the setup callback returns), then usb_ep_set_halt() may be
 * used on ep0 to trigger protocol stalls.
 *
 * For periodic endpoints, like interrupt or isochronous ones, the usb host
 * arranges to poll once per interval, and the gadget driver usually will
 * have queued some data to transfer at that time.
 *
 * Returns zero, or a negative error code.  Endpoints that are not enabled
 * report errors; errors will also be
 * reported when the usb peripheral is disconnected.
 */
inline int usb_ep_queue(struct usb_ep *ep, void *req, gfp_t gfp_flags)
{
    return ep->ops->queue(ep, req, gfp_flags); // static int ep_queue(...)
}

/**
 * usb_ep_dequeue - dequeues (cancels, unlinks) an I/O request from an endpoint
 * @ep:the endpoint associated with the request
 * @req:the request being canceled
 *
 * if the request is still active on the endpoint, it is dequeued and its
 * completion routine is called (with status -USB_ECONNRESET); else a negative
 * error code is returned.
 *
 * note that some hardware can't clear out write fifos (to unlink the request
 * at the head of the queue) except as part of disconnecting from usb.  such
 * restrictions prevent drivers from supporting configuration changes,
 * even to configuration zero (a "chapter 9" requirement).
 */
inline int usb_ep_dequeue(struct usb_ep *ep, struct usb_request *req)
{
    return ep->ops->dequeue(ep, req);   // ep_dequeue
}

/**
 * usb_ep_set_halt - sets the endpoint halt feature.
 * @ep: the non-isochronous endpoint being stalled
 *
 * Use this to stall an endpoint, perhaps as an error report.
 * Except for control endpoints,
 * the endpoint stays halted (will not stream any data) until the host
 * clears this feature; drivers may need to empty the endpoint's request
 * queue first, to make sure no inappropriate transfers happen.
 *
 * Note that while an endpoint CLEAR_FEATURE will be invisible to the
 * gadget driver, a SET_INTERFACE will not be.  To reset endpoints for the
 * current altsetting, see usb_ep_clear_halt().  When switching altsettings,
 * it's simplest to use usb_ep_enable() or usb_ep_disable() for the endpoints.
 *
 * Returns zero, or a negative error code.  On success, this call sets
 * underlying hardware state that blocks data transfers.
 * Attempts to halt IN endpoints will fail (returning -USB_EAGAIN) if any
 * transfer requests are still queued, or if the controller hardware
 * (usually a FIFO) still holds bytes that the host hasn't collected.
 */
inline int usb_ep_set_halt(struct usb_ep *ep)
{
    return ep->ops->set_halt(ep, 1);
}

#ifdef USBD_EN_ISOC
/**
* Submit an ISOC transfer request to an EP.
*
*  - Every time a sync period completes the request's completion callback
*    is called to provide data to the gadget driver.
*  - Once submitted the request cannot be modified.
*  - Each request is turned into periodic data packets untill ISO
*    Transfer is stopped..
*/
int iso_ep_start(struct usb_ep *usb_ep, struct usb_iso_request *req, gfp_t gfp_flags);

/**
* Stops ISOC EP periodic data transfer.
*/
int iso_ep_stop(struct usb_ep *usb_ep, struct usb_iso_request *req);

/**
* Allocate ISOC transfer request.
*/
struct usb_iso_request *alloc_iso_request(struct usb_ep *ep, int packets, gfp_t gfp_flags);

/**
* Free ISOC transfer request.
*/
void free_iso_request(struct usb_ep *ep, struct usb_iso_request *req);

#endif // USBD_EN_ISOC

/**
* usb_interface_id() - allocate an unused interface ID
* @config: configuration associated with the interface
* @function: function handling the interface
* Context: single threaded during gadget setup
*
* usb_interface_id() is called from usb_function.bind() callbacks to
* allocate new interface IDs.  The function driver will then store that
* ID in interface, association, CDC union, and other descriptors.  It
* will also handle any control requests targeted at that interface,
* particularly changing its altsetting via set_alt().  There may
* also be class-specific or vendor-specific requests to handle.
*
* All interface identifier should be allocated using this routine, to
* ensure that for example different functions don't wrongly assign
* different meanings to the same identifier.  Note that since interface
* identifiers are configuration-specific, functions used in more than
* one configuration (or more than once in a given configuration) need
* multiple versions of the relevant descriptors.
*
* Returns the interface ID which was allocated; or -USB_ENODEV if no
* more interface IDs can be allocated.
*/
int usb_interface_id(struct usb_configuration *config, struct usb_function *function);
    
struct usb_ep *usb_ep_autoconfig(struct usb_gadget *, struct usb_endpoint_descriptor *);

int usb_assign_descriptors(struct usb_function *f,
    struct usb_descriptor_header **fs,
    struct usb_descriptor_header **hs,
    struct usb_descriptor_header **ss);

void usb_free_all_descriptors(struct usb_function *f);

int usb_composite_register(struct usb_composite_driver *driver);

void usb_composite_unregister(struct usb_composite_driver *driver);

#endif

