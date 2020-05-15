/*
 * Ethernet gadget driver -- with CDC and non-CDC options
 * Builds on hardware support for a full duplex link.
 *
 * CDC Ethernet is the standard USB solution for sending Ethernet frames
 * using USB.  Real hardware tends to use the same framing protocol but look
 * different for control features.  This driver strongly prefers to use
 * this USB-IF standard as its open-systems interoperability solution;
 * most host side USB stacks (except from Microsoft) support it.
 *
 * There's some hardware that can't talk CDC.  We make that hardware
 * implement a "minimalist" vendor-agnostic CDC core:  same framing, but
 * link-level setup only requires activating the configuration.
 * Linux supports it, but other host operating systems may not.
 * (This is a subset of CDC Ethernet.)
 *
 * A third option is also in use.  Rather than CDC Ethernet, or something
 * simpler, Microsoft pushes their own approach: RNDIS.  The published
 * RNDIS specs are ambiguous and appear to be incomplete, and are also
 * needlessly complex.
 */
#ifndef __USB_VERIFY_DEVICE_H
#define __USB_VERIFY_DEVICE_H

#include "usb.h"
#include "usb_gadget.h"
#include "core/inc/usb_composite.h"

/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.  For now we do either full CDC, or
 * our simple subset, with RNDIS as an optional second configuration.
 *
 * RNDIS includes some CDC ACM descriptors ... like CDC Ethernet.  But
 * the class descriptors match a modem (they're ignored; it's really just
 * Ethernet functionality), they don't need the NOP altsetting, and the
 * status transfer endpoint isn't optional.
 */

#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_SERIALNUMBER		3
#define STRING_DATA				4

/*
 * This device advertises one configuration, eth_config, unless RNDIS
 * is enabled (rndis_config) on hardware supporting at least two configs.
 *
 * NOTE:  Controllers like superh_udc should probably be able to use
 * an RNDIS-only configuration.
 *
 * FIXME define some higher-powered configurations to make it easier
 * to recharge batteries ...
 */
#define DEV_CONFIG_VALUE		1

struct verify_dev{
	struct usb_gadget	*gadget;
	struct usb_request	*req;		/* for control responses */
	/* when configured, we have one of two configs:
	* - source data (in to host) and sink it (out from host)
	* - or loop it back (out from host back in to host)
	*/
	u8 config;
	struct usb_ep	*in_ep;
	struct usb_ep	*out_ep;
	const struct usb_endpoint_descriptor *in, *out, *status;		
	struct usb_function	func;
	struct usb_request	*reqin; 	/* for bulkin responses */
	struct usb_request	*reqout;
	u8 running;
	unsigned char *req_buffer;
};
#endif