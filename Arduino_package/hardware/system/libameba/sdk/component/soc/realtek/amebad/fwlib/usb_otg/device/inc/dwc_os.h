/* =========================================================================
 * $File: //dwh/usb_iip/dev/software/dwc_common_port_2/dwc_os.h $
 * $Revision: #14 $
 * $Date: 2010/11/04 $
 * $Change: 1621695 $
 *
 * Synopsys Portability Library Software and documentation
 * (hereinafter, "Software") is an Unsupported proprietary work of
 * Synopsys, Inc. unless otherwise expressly agreed to in writing
 * between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product
 * under any End User Software License Agreement or Agreement for
 * Licensed Product with Synopsys or any supplement thereto. You are
 * permitted to use and redistribute this Software in source and binary
 * forms, with or without modification, provided that redistributions
 * of source code must retain this notice. You may not view, use,
 * disclose, copy or distribute this file or any information contained
 * herein except pursuant to this license grant from Synopsys. If you
 * do not agree with this notice, including the disclaimer below, then
 * you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS"
 * BASIS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE HEREBY DISCLAIMED. IN NO EVENT SHALL
 * SYNOPSYS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================= */
#ifndef _DWC_OS_H_
#define _DWC_OS_H_

#include <stdarg.h>
#include "basic_types.h"
#include "diag.h"
#include "freertos_service.h"
#include "osdep_service.h"
#include "usb_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *
 * DWC portability library, low level os-wrapper functions
 *
 */

// Common interrupt mask
#define OTG_COMMON_INTR_MASK        0xF1010806UL /* BIT1 | BIT2 | BIT11 | BIT16 | BIT24 | BIT28 | BIT29 | BIT30 | BIT31 */

#define USB_PRINT_WARN0(...)
#define USB_PRINT_WARN1(...)
#define USB_PRINT_WARN2(...)
#define USB_PRINT_WARN3(...)
#define USB_PRINT_WARN4(...)
#define USB_PRINT_TRACE0(...)
#define USB_PRINT_TRACE1(...)
#define USB_PRINT_TRACE2(...)
#define USB_PRINT_TRACE3(...)
#define USB_PRINT_TRACE4(...)
#define USB_PRINT_TRACE5(...)
#define USB_PRINT_TRACE6(...)
#define USB_PRINT_TRACE7(...)
#define USB_PRINT_TRACE8(...)
#define USB_PRINT_INFO0(...)
#define USB_PRINT_INFO1(...)
#define USB_PRINT_INFO2(...)
#define USB_PRINT_INFO3(...)
#define USB_PRINT_INFO4(...)
#define USB_PRINT_INFO5(...)
#define USB_PRINT_ERROR0(...)
#define USB_PRINT_ERROR1(...)
#define USB_PRINT_ERROR2(...)
#define USB_PRINT_ERROR3(...)
#define USB_PRINT_ERROR4(...)
#define USB_PRINT_ERROR5(...)

#define DWC_OS_PORTING   1

#ifdef DWC_OS_PORTING

/** @name Error Codes */
#define DWC_E_INVALID			USB_EINVAL
#define DWC_E_NO_MEMORY			USB_ENOMEM
#define DWC_E_NO_DEVICE			USB_ENODEV
#define DWC_E_NOT_SUPPORTED		USB_EOPNOTSUPP
#define DWC_E_TIMEOUT			USB_ETIMEDOUT
#define DWC_E_BUSY				USB_EBUSY
#define DWC_E_AGAIN				USB_EAGAIN
#define DWC_E_RESTART			USB_ERESTART
#define DWC_E_ABORT				USB_ECONNABORTED
#define DWC_E_SHUTDOWN			USB_ESHUTDOWN
#define DWC_E_NO_DATA			USB_ENODATA
#define DWC_E_DISCONNECT		USB_ECONNRESET
#define DWC_E_UNKNOWN			USB_EINVAL
#define DWC_E_NO_STREAM_RES		USB_ENOSR
#define DWC_E_COMMUNICATION		USB_ECOMM
#define DWC_E_OVERFLOW			USB_EOVERFLOW
#define DWC_E_PROTOCOL			USB_EPROTO
#define DWC_E_IN_PROGRESS		USB_EINPROGRESS
#define DWC_E_PIPE				USB_EPIPE
#define DWC_E_IO				USB_EIO
#define DWC_E_NO_SPACE			USB_ENOSPC

#else

/** @name Error Codes */
#define DWC_E_INVALID       1001
#define DWC_E_NO_MEMORY     1002
#define DWC_E_NO_DEVICE     1003
#define DWC_E_NOT_SUPPORTED 1004
#define DWC_E_TIMEOUT       1005
#define DWC_E_BUSY          1006
#define DWC_E_AGAIN         1007
#define DWC_E_RESTART       1008
#define DWC_E_ABORT         1009
#define DWC_E_SHUTDOWN      1010
#define DWC_E_NO_DATA       1011
#define DWC_E_DISCONNECT    2000
#define DWC_E_UNKNOWN       3000
#define DWC_E_NO_STREAM_RES 4001
#define DWC_E_COMMUNICATION 4002
#define DWC_E_OVERFLOW      4003
#define DWC_E_PROTOCOL      4004
#define DWC_E_IN_PROGRESS   4005
#define DWC_E_PIPE          4006
#define DWC_E_IO            4007
#define DWC_E_NO_SPACE      4008

#endif

/** Type for a DMA address */
typedef dma_addr_t dwc_dma_t;

/** @name Memory and String Processing */
/** strdup() clone.  If you wish to use memory allocation debugging, this
 * implementation of strdup should use the DWC_* memory routines instead of
 * calling a predefined strdup.  Otherwise the memory allocated by this routine
 * will not be seen by the debugging routines. */
extern char *dwc_strdup(char const *str);

/** @name Spinlocks
 *
 * These locks are used when the work between the lock/unlock is atomic and
 * short.  Interrupts are also disabled during the lock/unlock and thus they are
 * suitable to lock between interrupt/non-interrupt context.  They also lock
 * between processes if you have multiple CPUs or Preemption.  If you don't have
 * multiple CPUS or Preemption, then the you can simply implement the
 * dwc_spinlock and dwc_spinunlock to disable and enable interrupts.  Because
 * the work between the lock/unlock is atomic, the process context will never
 * change, and so you never have to lock between processes.  */


/** Type for a spinlock */
typedef _lock dwc_spinlock_t;

/** Type for the 'flags' argument to spinlock funtions */
typedef unsigned long dwc_irqflags_t;

/** Returns an initialized lock variable.  This function should allocate and
 * initialize the OS-specific data structure used for locking.  This data
 * structure is to be used for the DWC_LOCK and DWC_UNLOCK functions and should
 * be freed by the os_mem_free_LOCK when it is no longer used. */
extern dwc_spinlock_t *dwc_spinlock_alloc(void);

/** Frees an initialized lock variable. */
extern void dwc_spinlock_free(dwc_spinlock_t *lock);

/** Disables interrupts and blocks until it acquires the lock.
 *
 * @param lock Pointer to the spinlock.
 * @param flags Unsigned long for irq flags storage.
 */
extern void dwc_spinlock_irqsave(dwc_spinlock_t *lock, dwc_irqflags_t *flags);

/** Re-enables the interrupt and releases the lock.
 *
 * @param lock Pointer to the spinlock.
 * @param flags Unsigned long for irq flags storage.  Must be the same as was
 * passed into DWC_LOCK.
 */
extern void dwc_spinlock_irqrestore(dwc_spinlock_t *lock, dwc_irqflags_t flags);

/** Blocks until it acquires the lock.
 *
 * @param lock Pointer to the spinlock.
 */
extern void dwc_spinlock(dwc_spinlock_t *lock);

/** Releases the lock.
 *
 * @param lock Pointer to the spinlock.
 */
extern void dwc_spinunlock(dwc_spinlock_t *lock);

/** @name Timer
 *
 * Callbacks must be small and atomic.
 */

/** The type of the callback function to be called */
typedef void (*dwc_timer_callback_t)(void *data);

/** Type for a timer */
typedef struct {
    _timerHandle  pRtlTimer;
    char *name;
    dwc_timer_callback_t cb;
    void *data;
    uint8_t scheduled;
    dwc_spinlock_t *lock;
} dwc_timer_t;

/** Allocates a timer */
extern dwc_timer_t *dwc_timer_alloc(char *name, dwc_timer_callback_t cb, void *data);

/** Frees a timer */
extern void dwc_timer_free(dwc_timer_t *timer);

/** Schedules the timer to run at time ms from now.  And will repeat at every
 * repeat_interval msec therafter
 *
 * Modifies a timer that is still awaiting execution to a new expiration time.
 * The mod_time is added to the old time.  */
extern void dwc_timer_schedule(dwc_timer_t *timer, uint32_t time);

/** Disables the timer from execution. */
extern void dwc_timer_cancel(dwc_timer_t *timer);

/** @name Time */

/** Microsecond delay.
 *
 * @param usecs  Microseconds to delay.
 */
extern void dwc_udelay(uint32_t usecs);

extern void dwc_memset(void *dest, uint8_t byte, uint32_t size);
/** Millisecond delay.
 *
 * @param msecs  Milliseconds to delay.
 */
extern void dwc_mdelay(uint32_t msecs);
extern void *dwc_malloc(uint32_t size);
extern void *dwc_malloc_atomic(uint32_t size);
extern void dwc_free(void *addr);
extern void dwc_memcpy(void *dest, void const *src, uint32_t size);

extern void dwc_cache_clean(u32 addr, u32 bytes);
extern void dwc_cache_invalidate(u32 addr, u32 bytes);
extern void dwc_cache_clean_invalidate(u32 addr, u32 bytes);

#endif /* _DWC_OS_H_ */

