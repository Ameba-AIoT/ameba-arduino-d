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

#include "basic_types.h"
//#include "va_list.h"
#include <stdarg.h>

#include "diag.h"
#include "dwc_otg_dbg.h"
#include "freertos_service.h"

#ifdef __cplusplus
extern "C" {
#endif
extern
_LONG_CALL_ u32
DiagPrintf(
    IN  const char *fmt, ...
);

#define DBG_8195A_OTG(...)  do{\
	if (ConfigDebug[LEVEL_INFO] & BIT(MODULE_USB_OTG)) \
		DiagPrintf("\r" __VA_ARGS__);\
	}while(0)

#define DBG_8195A_OTG_INFO(...)  do{\
	if (ConfigDebug[LEVEL_INFO] & BIT(MODULE_USB_OTG)) \
		DiagPrintf("\r" __VA_ARGS__);\
	}while(0)

#define DBG_8195A_OTG_WARN(...)  do{\
	if (ConfigDebug[LEVEL_WARN] & BIT(MODULE_USB_OTG)) \
		DiagPrintf("\r" __VA_ARGS__);\
	}while(0)

#define DBG_8195A_OTG_ERR(...)  do{\
	if (ConfigDebug[LEVEL_ERROR] & BIT(MODULE_USB_OTG)) \
		DiagPrintf("\r" __VA_ARGS__);\
	}while(0)
            
#define DBG_8195A_OTG_LVL(LVL,...)  do{\
    if (ConfigDebug[LEVEL_INFO] & BIT(MODULE_USB_OTG)){ \
	DiagPrintf("\rRTL8195A[OTG_LVL_%2x]: ",LVL);\
	DiagPrintf(__VA_ARGS__);\
    }\
}while(0)

/** @file
 *
 * DWC portability library, low level os-wrapper functions
 *
 */

/* These basic types need to be defined by some OS header file or custom header
 * file for your specific target architecture.
 *
 * uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t
 *
 * Any custom or alternate header file must be added and enabled here.
 */

#include "usb_errno.h"

/** @name Primitive Types and Values */

/** We define a boolean type for consistency.  Can be either YES or NO */
typedef uint8_t dwc_bool_t;
#define YES  1
#define NO   0

/** Compile Flag Definition */
#define __OTG_LITTLE_ENDIAN

/** @name Error Codes */
#define DWC_E_INVALID		USB_EINVAL
#define DWC_E_NO_MEMORY		USB_ENOMEM
#define DWC_E_NO_DEVICE		USB_ENODEV
#define DWC_E_NOT_SUPPORTED	USB_EOPNOTSUPP
#define DWC_E_TIMEOUT		USB_ETIMEDOUT
#define DWC_E_BUSY			USB_EBUSY
#define DWC_E_AGAIN			USB_EAGAIN
#define DWC_E_RESTART		USB_ERESTART
#define DWC_E_ABORT			USB_ECONNABORTED
#define DWC_E_SHUTDOWN		USB_ESHUTDOWN
#define DWC_E_NO_DATA		USB_ENODATA
#define DWC_E_DISCONNECT	USB_ECONNRESET
#define DWC_E_UNKNOWN		USB_EINVAL
#define DWC_E_NO_STREAM_RES	USB_ENOSR
#define DWC_E_COMMUNICATION	USB_ECOMM
#define DWC_E_OVERFLOW		USB_EOVERFLOW
#define DWC_E_PROTOCOL		USB_EPROTO
#define DWC_E_IN_PROGRESS	USB_EINPROGRESS
#define DWC_E_PIPE			USB_EPIPE
#define DWC_E_IO			USB_EIO
#define DWC_E_NO_SPACE		USB_ENOSPC

/** @name Tracing/Logging Functions
 *
 * These function provide the capability to add tracing, debugging, and error
 * messages, as well exceptions as assertions.  The WUDEV uses these
 * extensively.  These could be logged to the main console, the serial port, an
 * internal buffer, etc.  These functions could also be no-op if they are too
 * expensive on your system.  By default undefining the DEBUG macro already
 * no-ops some of these functions. */

/** Returns non-zero if in interrupt context. */
extern _LONG_CALL_ dwc_bool_t DWC_IN_IRQ(void);
#define dwc_in_irq DWC_IN_IRQ

/** Returns "IRQ" if DWC_IN_IRQ is true. */
extern _LONG_CALL_ char *dwc_irq(void);

/** Returns non-zero if in bottom-half context. */
extern _LONG_CALL_ dwc_bool_t DWC_IN_BH(void);
#define dwc_in_bh DWC_IN_BH

/** Returns "BH" if DWC_IN_BH is true. */
extern _LONG_CALL_ char *dwc_bh(void);

/**
 * A vsnprintf() clone.  Just call vprintf if you've got it.
 */
extern _LONG_CALL_ int DWC_VSNPRINTF(char *str, int size, char *format, va_list args);
#define dwc_vsnprintf DWC_VSNPRINTF

/**
 * printf() clone.  Just call printf if you've go it.
 */
#ifdef CONFIG_USB_DEBUG
#define DWC_PRINTF(format...) do{ DBG_8195A_OTG(format); }while(0)
#else
#define DWC_PRINTF(format...)
#endif

//extern void DWC_PRINTF(char *format, ...)
/* This provides compiler level static checking of the parameters if you're
 * using GCC. */
#define dwc_printf DWC_PRINTF

/**
 * sprintf() clone.  Just call sprintf if you've got it.
 */
extern _LONG_CALL_ int DWC_SPRINTF(char *string, char *format, ...);
#define dwc_sprintf DWC_SPRINTF

/**
 * snprintf() clone.  Just call snprintf if you've got it.
 */
extern _LONG_CALL_ int DWC_SNPRINTF(char *string, int size, char *format, ...);
#define dwc_snprintf DWC_SNPRINTF

/**
 * Prints a WARNING message.  On systems that don't differentiate between
 * warnings and regular log messages, just print it.  Indicates that something
 * may be wrong with the driver.  Works like printf().
 *
 * Use the DWC_WARN macro to call this function.
 */
extern _LONG_CALL_ void __DWC_WARN(char *format, ...);

/**
 * Prints an error message.  On systems that don't differentiate between errors
 * and regular log messages, just print it.  Indicates that something went wrong
 * with the driver.  Works like printf().
 *
 * Use the DWC_ERROR macro to call this function.
 */
extern _LONG_CALL_ void __DWC_ERROR(char *format, ...);

/**
 * Prints an exception error message and takes some user-defined action such as
 * print out a backtrace or trigger a breakpoint.  Indicates that something went
 * abnormally wrong with the driver such as programmer error, or other
 * exceptional condition.  It should not be ignored so even on systems without
 * printing capability, some action should be taken to notify the developer of
 * it.  Works like printf().
 */
extern _LONG_CALL_ void DWC_EXCEPTION(char *format, ...)	;

#define dwc_exception DWC_EXCEPTION

#ifdef OTGDEBUG
/**
 * Prints out a debug message.  Used for logging/trace messages.
 *
 * Use the DWC_DEBUG macro to call this function
 */
extern _LONG_CALL_ void __DWC_DEBUG(char *format, ...);
#else
#define __DWC_DEBUG(...)
#endif

/**
 * Prints out a Debug message.
 */
#define DWC_DEBUG(fmt, args...)		            DBG_8195A_OTG("\n\rDWC_DEBUG:" fmt, ## args)
#define dwc_debug DWC_DEBUG
/**
 * Prints out an informative message.
 */
#define DWC_INFO(_format, _args...) DBG_8195A_OTG_INFO("INFO:%s: " _format "\n", \
					       dwc_irq(), ## _args)
#define dwc_info DWC_INFO
/**
 * Prints out a warning message.
 */
#define DWC_WARN(_format, _args...) DBG_8195A_OTG_WARN("WARN:%s: " _format "\n", \
					dwc_irq(), ## _args)
#define dwc_warn DWC_WARN
/**
 * Prints out an error message.
 */
#define DWC_ERROR(_format, _args...) DBG_8195A_OTG_ERR("ERROR:%s: " _format "\n", \
					dwc_irq(),  ## _args)

#define dwc_error DWC_ERROR

#define DWC_PROTO_ERROR(_format, _args...) DBG_8195A_OTG_ERR("ERROR:%s: " _format "\n", \
						dwc_irq(), ## _args)
#define dwc_proto_error DWC_PROTO_ERROR

#ifdef OTGDEBUG
/** Prints out a exception error message if the _expr expression fails.  Disabled
 * if DEBUG is not enabled. */
#define DWC_ASSERT(_expr, _format, _args...) do { \
	if (!(_expr)) { DBG_8195A_OTG("%s: " _format "\n", dwc_irq(), \
				       ## _args); } \
	} while (0)
#else
#define DWC_ASSERT(_x...)
#endif
#define dwc_assert DWC_ASSERT


/** @name Byte Ordering
 * The following functions are for conversions between processor's byte ordering
 * and specific ordering you want.
 */

/** Converts 32 bit data in CPU byte ordering to little endian. */
extern _LONG_CALL_ uint32_t DWC_CPU_TO_LE32(uint32_t *p);
#define dwc_cpu_to_le32 DWC_CPU_TO_LE32

/** Converts 32 bit data in CPU byte orderint to big endian. */
extern _LONG_CALL_ uint32_t DWC_CPU_TO_BE32(uint32_t *p);
#define dwc_cpu_to_be32 DWC_CPU_TO_BE32

/** Converts 32 bit little endian data to CPU byte ordering. */
extern _LONG_CALL_ uint32_t DWC_LE32_TO_CPU(uint32_t *p);
#define dwc_le32_to_cpu DWC_LE32_TO_CPU

/** Converts 32 bit big endian data to CPU byte ordering. */
extern _LONG_CALL_ uint32_t DWC_BE32_TO_CPU(uint32_t *p);
#define dwc_be32_to_cpu DWC_BE32_TO_CPU

/** Converts 16 bit data in CPU byte ordering to little endian. */
extern _LONG_CALL_ uint16_t DWC_CPU_TO_LE16(uint16_t *p);
#define dwc_cpu_to_le16 DWC_CPU_TO_LE16

/** Converts 16 bit data in CPU byte orderint to big endian. */
extern _LONG_CALL_ uint16_t DWC_CPU_TO_BE16(uint16_t *p);
#define dwc_cpu_to_be16 DWC_CPU_TO_BE16

/** Converts 16 bit little endian data to CPU byte ordering. */
extern _LONG_CALL_ uint16_t DWC_LE16_TO_CPU(uint16_t *p);
#define dwc_le16_to_cpu DWC_LE16_TO_CPU

/** Converts 16 bit bi endian data to CPU byte ordering. */
extern _LONG_CALL_ uint16_t DWC_BE16_TO_CPU(uint16_t *p);
#define dwc_be16_to_cpu DWC_BE16_TO_CPU


/** @name Register Read/Write
 *
 * The following six functions should be implemented to read/write registers of
 * 32-bit and 64-bit sizes.  All modules use this to read/write register values.
 * The reg value is a pointer to the register calculated from the void *base
 * variable passed into the driver when it is started.  */


/** @cond */

/** @name Some convenience MACROS used internally.  Define DWC_DEBUG_REGS to log the
 * register writes. */

#define dwc_define_read_write_reg_n(_reg,_container_type) \
static inline uint32_t dwc_read_##_reg##_n(_container_type *container, int num) { \
	return DWC_READ_REG32(&container->regs->_reg[num]); \
} \
static inline void dwc_write_##_reg##_n(_container_type *container, int num, uint32_t data) { \
	DWC_WRITE_REG32(&(((uint32_t*)container->regs->_reg)[num]), data); \
}

#define dwc_define_read_write_reg(_reg,_container_type) \
static inline uint32_t dwc_read_##_reg(_container_type *container) { \
	return DWC_READ_REG32(&container->regs->_reg); \
} \
static inline void dwc_write_##_reg(_container_type *container, uint32_t data) { \
	DWC_WRITE_REG32(&container->regs->_reg, data); \
}

/** @endcond */

/** @name Memory Allocation
 *
 * These function provide access to memory allocation.  There are only 2 DMA
 * functions and 3 Regular memory functions that need to be implemented.  None
 * of the memory debugging routines need to be implemented.  The allocation
 * routines all ZERO the contents of the memory.
 *
 * Defining DWC_DEBUG_MEMORY turns on memory debugging and statistic gathering.
 * This checks for memory leaks, keeping track of alloc/free pairs.  It also
 * keeps track of how much memory the driver is using at any given time. */

#define DWC_PAGE_SIZE 4096
#define DWC_PAGE_OFFSET(addr) (((uint32_t)addr) & 0xfff)
#define DWC_PAGE_ALIGNED(addr) ((((uint32_t)addr) & 0xfff) == 0)

#define DWC_INVALID_DMA_ADDR 0x0

/** Type for a DMA address */
typedef dma_addr_t dwc_dma_t;

/** Allocates a DMA capable buffer and zeroes its contents. */
extern _LONG_CALL_ void *__DWC_DMA_ALLOC(void *dma_ctx, uint32_t size, dwc_dma_t *dma_addr);

/** Allocates a DMA capable buffer and zeroes its contents in atomic contest */
extern _LONG_CALL_ void *__DWC_DMA_ALLOC_ATOMIC(void *dma_ctx, uint32_t size, dwc_dma_t *dma_addr);

/** Frees a previously allocated buffer. */
extern _LONG_CALL_ void __DWC_DMA_FREE(void *dma_ctx, uint32_t size, void *virt_addr, dwc_dma_t dma_addr);

/** Allocates a block of memory and zeroes its contents. */
extern _LONG_CALL_ void *__DWC_ALLOC(void *mem_ctx, uint32_t size);

/** Allocates a block of memory and zeroes its contents, in an atomic manner
 * which can be used inside interrupt context.  The size should be sufficiently
 * small, a few KB at most, such that failures are not likely to occur.  Can just call
 * __DWC_ALLOC if it is atomic. */
extern _LONG_CALL_ void *__DWC_ALLOC_ATOMIC(void *mem_ctx, uint32_t size);

/** Frees a previously allocated buffer. */
extern _LONG_CALL_ void __DWC_FREE(void *mem_ctx, void *addr);

#define DWC_ALLOC(_size_) __DWC_ALLOC(NULL, _size_)
#define DWC_ALLOC_ATOMIC(_size_) __DWC_ALLOC_ATOMIC(NULL, _size_)
#define DWC_FREE(_addr_) __DWC_FREE(NULL, _addr_)

#define DWC_DMA_ALLOC(_size_,_dma_) __DWC_DMA_ALLOC(NULL, _size_, _dma_)
#define DWC_DMA_ALLOC_ATOMIC(_size_,_dma_) __DWC_DMA_ALLOC_ATOMIC(NULL, _size_,_dma_)
#define DWC_DMA_FREE(_size_,_virt_,_dma_) __DWC_DMA_FREE(NULL, _size_, _virt_, _dma_)

#define dwc_alloc(_ctx_,_size_) DWC_ALLOC(_size_)
#define dwc_alloc_atomic(_ctx_,_size_) DWC_ALLOC_ATOMIC(_size_)
#define dwc_free(_ctx_,_addr_) DWC_FREE(_addr_)

/* Linux doesn't need any extra parameters for DMA buffer allocation, so we
 * just throw away the DMA context parameter.
 */
#define dwc_dma_alloc(_ctx_,_size_,_dma_) DWC_DMA_ALLOC(_size_, _dma_)
#define dwc_dma_alloc_atomic(_ctx_,_size_,_dma_) DWC_DMA_ALLOC_ATOMIC(_size_, _dma_)
#define dwc_dma_free(_ctx_,_size_,_virt_,_dma_) DWC_DMA_FREE(_size_, _virt_, _dma_)

/** @name Memory and String Processing */

/** memset() clone */
extern _LONG_CALL_ void *DWC_MEMSET(void *dest, uint8_t byte, uint32_t size);
#define dwc_memset DWC_MEMSET

/** memcpy() clone */
extern _LONG_CALL_ void *DWC_MEMCPY(void *dest, void const *src, uint32_t size);
#define dwc_memcpy DWC_MEMCPY

/** memmove() clone */
extern _LONG_CALL_ void *DWC_MEMMOVE(void *dest, void *src, uint32_t size);
#define dwc_memmove DWC_MEMMOVE

/** memcmp() clone */
extern _LONG_CALL_ int DWC_MEMCMP(void *m1, void *m2, uint32_t size);
#define dwc_memcmp DWC_MEMCMP

/** strcmp() clone */
extern _LONG_CALL_ int DWC_STRCMP(void *s1, void *s2);
#define dwc_strcmp DWC_STRCMP

/** strncmp() clone */
extern _LONG_CALL_ int DWC_STRNCMP(void *s1, void *s2, uint32_t size);
#define dwc_strncmp DWC_STRNCMP

/** strlen() clone, for NULL terminated ASCII strings */
extern _LONG_CALL_ int DWC_STRLEN(char const *str);
#define dwc_strlen DWC_STRLEN

/** strcpy() clone, for NULL terminated ASCII strings */
extern _LONG_CALL_ char *DWC_STRCPY(char *to, const char *from);
#define dwc_strcpy DWC_STRCPY

/** strdup() clone.  If you wish to use memory allocation debugging, this
 * implementation of strdup should use the DWC_* memory routines instead of
 * calling a predefined strdup.  Otherwise the memory allocated by this routine
 * will not be seen by the debugging routines. */
extern _LONG_CALL_ char *DWC_STRDUP(char const *str);
#define dwc_strdup(_ctx_,_str_) DWC_STRDUP(_str_)

/** NOT an atoi() clone.  Read the description carefully.  Returns an integer
 * converted from the string str in base 10 unless the string begins with a "0x"
 * in which case it is base 16.  String must be a NULL terminated sequence of
 * ASCII characters and may optionally begin with whitespace, a + or -, and a
 * "0x" prefix if base 16.  The remaining characters must be valid digits for
 * the number and end with a NULL character.  If any invalid characters are
 * encountered or it returns with a negative error code and the results of the
 * conversion are undefined.  On sucess it returns 0.  Overflow conditions are
 * undefined.  An example implementation using atoi() can be referenced from the
 * Linux implementation. */
extern _LONG_CALL_ int DWC_ATOI(const char *str, int32_t *value);
#define dwc_atoi DWC_ATOI

/** Same as above but for unsigned. */
extern _LONG_CALL_ int DWC_ATOUI(const char *str, uint32_t *value);
#define dwc_atoui DWC_ATOUI

/** Type for a workq */
typedef struct dwc_workq dwc_workq_t;

/** Type for a tasklet */
typedef struct dwc_tasklet dwc_tasklet_t;

/** @name Timer
 *
 * Callbacks must be small and atomic.
 */
struct dwc_timer;

/** Type for a timer */
typedef struct dwc_timer dwc_timer_t;

/** The type of the callback function to be called */
typedef void (*dwc_timer_callback_t)(void *data);

/** Allocates a timer */
extern _LONG_CALL_ dwc_timer_t *DWC_TIMER_ALLOC(char *name, dwc_timer_callback_t cb, void *data);
#define dwc_timer_alloc(_ctx_,_name_,_cb_,_data_) DWC_TIMER_ALLOC(_name_,_cb_,_data_)

/** Frees a timer */
extern _LONG_CALL_ void DWC_TIMER_FREE(dwc_timer_t *timer);
#define dwc_timer_free DWC_TIMER_FREE

/** Schedules the timer to run at time ms from now.  And will repeat at every
 * repeat_interval msec therafter
 *
 * Modifies a timer that is still awaiting execution to a new expiration time.
 * The mod_time is added to the old time.  */
extern _LONG_CALL_ void DWC_TIMER_SCHEDULE(dwc_timer_t *timer, uint32_t time);
#define dwc_timer_schedule DWC_TIMER_SCHEDULE

/** Disables the timer from execution. */
extern _LONG_CALL_ void DWC_TIMER_CANCEL(dwc_timer_t *timer);
#define dwc_timer_cancel DWC_TIMER_CANCEL


/** @name Spinlocks
 *
 * These locks are used when the work between the lock/unlock is atomic and
 * short.  Interrupts are also disabled during the lock/unlock and thus they are
 * suitable to lock between interrupt/non-interrupt context.  They also lock
 * between processes if you have multiple CPUs or Preemption.  If you don't have
 * multiple CPUS or Preemption, then the you can simply implement the
 * DWC_SPINLOCK and DWC_SPINUNLOCK to disable and enable interrupts.  Because
 * the work between the lock/unlock is atomic, the process context will never
 * change, and so you never have to lock between processes.  */

struct dwc_spinlock;

/** Type for a spinlock */
//typedef struct dwc_spinlock dwc_spinlock_t;
typedef _lock dwc_spinlock_t;

/** Type for the 'flags' argument to spinlock funtions */
typedef unsigned long dwc_irqflags_t;

/** Returns an initialized lock variable.  This function should allocate and
 * initialize the OS-specific data structure used for locking.  This data
 * structure is to be used for the DWC_LOCK and DWC_UNLOCK functions and should
 * be freed by the DWC_FREE_LOCK when it is no longer used. */
extern _LONG_CALL_ dwc_spinlock_t *DWC_SPINLOCK_ALLOC(void);
#define dwc_spinlock_alloc(_ctx_) DWC_SPINLOCK_ALLOC()

/** Frees an initialized lock variable. */
extern _LONG_CALL_ void DWC_SPINLOCK_FREE(dwc_spinlock_t *lock);
#define dwc_spinlock_free(_ctx_,_lock_) DWC_SPINLOCK_FREE(_lock_)

/** Disables interrupts and blocks until it acquires the lock.
 *
 * @param lock Pointer to the spinlock.
 * @param flags Unsigned long for irq flags storage.
 */
extern _LONG_CALL_ void DWC_SPINLOCK_IRQSAVE(dwc_spinlock_t *lock, dwc_irqflags_t *flags);
#define dwc_spinlock_irqsave DWC_SPINLOCK_IRQSAVE

/** Re-enables the interrupt and releases the lock.
 *
 * @param lock Pointer to the spinlock.
 * @param flags Unsigned long for irq flags storage.  Must be the same as was
 * passed into DWC_LOCK.
 */
extern _LONG_CALL_ void DWC_SPINUNLOCK_IRQRESTORE(dwc_spinlock_t *lock, dwc_irqflags_t flags);
#define dwc_spinunlock_irqrestore DWC_SPINUNLOCK_IRQRESTORE

/** Blocks until it acquires the lock.
 *
 * @param lock Pointer to the spinlock.
 */
extern _LONG_CALL_ void DWC_SPINLOCK(dwc_spinlock_t *lock);
#define dwc_spinlock DWC_SPINLOCK

/** Releases the lock.
 *
 * @param lock Pointer to the spinlock.
 */
extern _LONG_CALL_ void DWC_SPINUNLOCK(dwc_spinlock_t *lock);
#define dwc_spinunlock DWC_SPINUNLOCK


/** @name Mutexes
 *
 * Unlike spinlocks Mutexes lock only between processes and the work between the
 * lock/unlock CAN block, therefore it CANNOT be called from interrupt context.
 */

struct dwc_mutex;

/** Type for a mutex */
typedef struct dwc_mutex dwc_mutex_t;

/** Allocate a mutex */
extern _LONG_CALL_ dwc_mutex_t *DWC_MUTEX_ALLOC(void);
#define dwc_mutex_alloc(_ctx_) DWC_MUTEX_ALLOC()

/* For memory leak debugging when using Linux Mutex Debugging */
/** Free a mutex */
extern _LONG_CALL_ void DWC_MUTEX_FREE(dwc_mutex_t *mutex);
#define dwc_mutex_free(_ctx_,_mutex_) DWC_MUTEX_FREE(_mutex_)

/** Lock a mutex */
extern _LONG_CALL_ void DWC_MUTEX_LOCK(dwc_mutex_t *mutex);
#define dwc_mutex_lock DWC_MUTEX_LOCK

/** Non-blocking lock returns 1 on successful lock. */
extern _LONG_CALL_ int DWC_MUTEX_TRYLOCK(dwc_mutex_t *mutex);
#define dwc_mutex_trylock DWC_MUTEX_TRYLOCK

/** Unlock a mutex */
extern _LONG_CALL_ void DWC_MUTEX_UNLOCK(dwc_mutex_t *mutex);
#define dwc_mutex_unlock DWC_MUTEX_UNLOCK


/** @name Time */

/** Microsecond delay.
 *
 * @param usecs  Microseconds to delay.
 */
extern _LONG_CALL_ void DWC_UDELAY(uint32_t usecs);
#define dwc_udelay DWC_UDELAY

/** Millisecond delay.
 *
 * @param msecs  Milliseconds to delay.
 */
extern _LONG_CALL_ void DWC_MDELAY(uint32_t msecs);
#define dwc_mdelay DWC_MDELAY

/** Non-busy waiting.
 * Sleeps for specified number of milliseconds.
 *
 * @param msecs Milliseconds to sleep.
 */
extern _LONG_CALL_ void DWC_MSLEEP(uint32_t msecs);
#define dwc_msleep DWC_MSLEEP

extern _LONG_CALL_ void DWC_ENTER_CRITICAL(VOID);
extern _LONG_CALL_ void DWC_EXIT_CRITICAL(VOID);
extern _LONG_CALL_ uint8_t DWC_IN_CRITICAL(VOID);

void dwc_cache_clean(u32 addr, u32 bytes);
void dwc_cache_invalidate(u32 addr, u32 bytes);
void dwc_cache_clean_invalidate(u32 addr, u32 bytes);

#ifdef __cplusplus
}
#endif

#endif /* _DWC_OS_H_ */
