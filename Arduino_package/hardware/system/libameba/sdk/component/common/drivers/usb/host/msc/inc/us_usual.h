#ifndef __US_USUAL_H
#define __US_USUAL_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

#include "usb.h"
#include "storage.h"

#define US_FL_SINGLE_LUN			0x00000001/* allow access to only LUN 0 */
//#define US_FL_NEED_OVERRIDE		0x00000002/* unusual_devs entry is necessary */
//#define US_FL_SCM_MULT_TARG		0x00000004/* supports multiple targets */
//#define US_FL_FIX_INQUIRY			0x00000008/* INQUIRY response needs faking */
//#define US_FL_FIX_CAPACITY			0x00000010/* READ CAPACITY response too big */
//#define US_FL_IGNORE_RESIDUE		0x00000020/* reported residue is wrong */
//#define US_FL_BULK32				0x00000040/* Uses 32-byte CBW length */
#define US_FL_NOT_LOCKABLE			0x00000080/* PREVENT/ALLOW not supported */
#define US_FL_GO_SLOW				0x00000100/* Need delay after Command phase */
#define US_FL_NO_WP_DETECT			0x00000200/* Don't check for write-protect */
#define US_FL_MAX_SECTORS_64		0x00000400/* Sets max_sectors to 64    */
#define US_FL_IGNORE_DEVICE			0x00000800/* Don't claim device */
#define US_FL_CAPACITY_HEURISTICS	0x00001000/* sometimes sizes is too big */
#define US_FL_MAX_SECTORS_MIN		0x00002000/* Sets max_sectors to arch min */
#define US_FL_BULK_IGNORE_TAG		0x00004000/* Ignore tag mismatch in bulk operations */
#define US_FL_SANE_SENSE		    0x00008000/* Sane Sense (> 18 bytes) */
#define US_FL_CAPACITY_OK			0x00010000/* READ CAPACITY response is correct */
#define US_FL_BAD_SENSE				0x00020000/* Bad Sense (never more than 18 bytes) */
#define US_FL_NO_READ_DISC_INFO 	0x00040000/* cannot handle READ_DISC_INFO */
#define US_FL_NO_READ_CAPACITY_16	0x00080000/* cannot handle READ_CAPACITY_16*/
#define US_FL_INITIAL_READ10		0x00100000/* Initial READ(10) (and others) must be retried */
#define US_FL_WRITE_CACHE			0x00200000/* Write Cache status is not available */
#define US_FL_NEEDS_CAP16			0x00400000/* cannot handle READ_CAPACITY_10 */
#define US_FL_IGNORE_UAS			0x00800000/* Device advertises UAS but it is broken */
#define US_FL_BROKEN_FUA			0x01000000/* Cannot handle FUA in WRITE or READ CDBs */

extern int usb_usual_ignore_device(struct usb_interface *intf);
extern struct usb_device_id usb_storage_usb_ids[];

#endif // CONFIG_USBH_MSC

#endif // __US_USUAL_H

