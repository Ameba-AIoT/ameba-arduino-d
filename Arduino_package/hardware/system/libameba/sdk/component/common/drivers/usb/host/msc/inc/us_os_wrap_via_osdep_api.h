/*
 * umsc_os_wrap_via_osdep_api.h
 *
 *  Created on: Sep 5, 2014
 *      Author: jimmysqf
 */

#ifndef US_OS_WRAP_VIA_OSDEP_API_H_
#define US_OS_WRAP_VIA_OSDEP_API_H_

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

#include "basic_types.h"
#include "osdep_service.h"
#if !defined (CONFIG_PLATFORM_8721D)
#include "osdep_api.h"
#endif

#define GFP_KERNEL		1
#define GFP_ATOMIC		1

#if defined (CONFIG_PLATFORM_8721D)
#define RestoreFlags()		portEXIT_CRITICAL()
#define SaveAndCli()		portENTER_CRITICAL()
#endif

typedef unsigned int gfp_t;

/* misc items */
#ifndef ssize_t
#define ssize_t SSIZE_T
#endif
#ifndef size_t
#define size_t SIZE_T
#endif

#ifndef __user
#define __user
#endif

#ifndef loff_t
#define loff_t long
#endif
#ifndef __u8
#define __u8 	u8
#endif
#ifndef __u16
#define __u16	u16
#endif
#ifndef __u32
#define __u32	u32
#endif
#ifndef __u64
#define __u64	u64
#endif
#ifndef __s8
#define __s8	s8
#endif
#ifndef __s16
#define __s16	s16
#endif
#ifndef __s32
#define __s32	s32
#endif
#ifndef __s64
#define __s64	s64
#endif

typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;
typedef __u16 __sum16;
typedef __u32 __wsum;


#ifndef cpu_to_le32

#define cpu_to_le32(x)	rtk_cpu_to_le32(x)
#endif
#ifndef le32_to_cpu

#define le32_to_cpu(x)	rtk_le32_to_cpu(x)
#endif
#ifndef cpu_to_le16

#define cpu_to_le16(x)	rtk_cpu_to_le16(x)
#endif
#ifndef le16_to_cpu

#define le16_to_cpu(x)	rtk_le16_to_cpu(x)
#endif
#ifndef cpu_to_be32

#define cpu_to_be32(x)	rtk_cpu_to_be32(x)
#endif
#ifndef be32_to_cpu

#define be32_to_cpu(x)	rtk_be32_to_cpu(x)
#endif
#ifndef cpu_to_be16

#define cpu_to_be16(x)	rtk_cpu_to_be16(x)
#endif
#ifndef be16_to_cpu

#define be16_to_cpu(x)	rtk_be16_to_cpu(x)
#endif

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#endif
#ifndef BITS_PER_LONG
#define BITS_PER_LONG (32)
#endif
#ifndef BITS_PER_LONG_LONG
#define BITS_PER_LONG_LONG (32)
#endif
#ifndef BIT
#define BIT(nr)                 (1UL << (nr))
#endif
#ifndef BIT_ULL
#define BIT_ULL(nr)             (1ULL << (nr))
#endif
#ifndef BIT_MASK
#define BIT_MASK(nr)            (1UL << ((nr) % BITS_PER_LONG))
#endif
#ifndef BIT_WORD
#define BIT_WORD(nr)            ((nr) / BITS_PER_LONG)
#endif
#ifndef BIT_ULL_MASK
#define BIT_ULL_MASK(nr)        (1ULL << ((nr) % BITS_PER_LONG_LONG))
#endif
#ifndef BIT_ULL_WORD
#define BIT_ULL_WORD(nr)        ((nr) / BITS_PER_LONG_LONG)
#endif
#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE           (8)
#endif
#ifndef BITS_TO_LONGS
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#endif

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef min_t
#define min_t(type, x, y) ({                    \
         type __min1 = (x);                      \
         type __min2 = (y);                      \
         __min1 < __min2 ? __min1 : __min2; })
#endif
#ifndef max_t
#define max_t(type, x, y) ({                    \
         type __max1 = (x);                      \
         type __max2 = (y);                      \
         __max1 > __max2 ? __max1 : __max2; })
#endif

/**
 * container_of - cast a member of a structure out to the containing structure
 * @p(ptr):        the pointer to the member.
 * @t(type):       the type of the container struct this is embedded in.
 * @m(member):     the name of the member within the struct.
 *
 */
#ifndef container_of
	#define container_of(ptr, type, member) \
        ((type *)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#endif

/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
        return ((1UL << (nr & 31)) & (addr[nr >> 5])) != 0;
}
/**
* set_bit - Atomically set a bit in memory
* @nr: the bit to set
* @addr: the address to start counting from
*
* This function is atomic and may not be reordered.  See __set_bit()
* if you do not require the atomic guarantees.
*
* Note: there are no guarantees that this function will not be reordered
* on non x86 architectures, so if you are writing portable code,
* make sure not to rely on its reordering guarantees.
*
* Note that @nr may be almost arbitrarily large; this function is not
* restricted to acting on a single-word quantity.
*/
static inline void set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	SaveAndCli();
	*p  |= mask;
	RestoreFlags();
}

/**
* clear_bit - Clears a bit in memory
* @nr: Bit to clear
* @addr: Address to start counting from
*
* clear_bit() is atomic and may not be reordered.  However, it does
* not contain a memory barrier, so if it is used for locking purposes,
* you should call smp_mb__before_clear_bit() and/or smp_mb__after_clear_bit()
* in order to ensure changes are visible on other processors.
*/
static inline void clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	SaveAndCli();
	*p &= ~mask;
	RestoreFlags();
}

/**
* change_bit - Toggle a bit in memory
* @nr: Bit to change
* @addr: Address to start counting from
*
* change_bit() is atomic and may not be reordered. It may be
* reordered on other architectures than x86.
* Note that @nr may be almost arbitrarily large; this function is not
* restricted to acting on a single-word quantity.
*/
static inline void change_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	SaveAndCli();
	*p ^= mask;
	RestoreFlags();
}

/**
* test_and_set_bit - Set a bit and return its old value
* @nr: Bit to set
* @addr: Address to count from
*
* This operation is atomic and cannot be reordered.
* It may be reordered on other architectures than x86.
* It also implies a memory barrier.
*/
static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;
	
	SaveAndCli();
	old = *p;
	*p = old | mask;
	RestoreFlags();
	return (old & mask) != 0;
}

/**
* test_and_clear_bit - Clear a bit and return its old value
* @nr: Bit to clear
* @addr: Address to count from
*
* This operation is atomic and cannot be reordered.
* It can be reorderdered on other architectures other than x86.
* It also implies a memory barrier.
*/
static inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;
	
	SaveAndCli();
	old = *p;
	*p = old & ~mask;
	RestoreFlags();

	return (old & mask) != 0;
}
/**
* test_and_change_bit - Change a bit and return its old value
* @nr: Bit to change
* @addr: Address to count from
*
* This operation is atomic and cannot be reordered.
* It also implies a memory barrier.
*/
static inline int test_and_change_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;
	
	SaveAndCli();
	old = *p;
	*p = old ^ mask;
	RestoreFlags();

	return (old & mask) != 0;
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#endif // CONFIG_USBH_MSC

#endif // US_OS_WRAP_VIA_OSDEP_API_H_
