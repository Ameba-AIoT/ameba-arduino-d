// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TLSIO_RTL8195A_H
#define TLSIO_RTL8195A_H

#ifdef __cplusplus
extern "C" {
#include <cstddef>
#else
#include <stddef.h>
#endif /* __cplusplus */

#include "azure_c_shared_utility/xio.h"
#include "azure_c_shared_utility/umock_c_prod.h"

/** @brief	Return the tlsio table of functions.
*
* @param	void.
*
* @return	The tlsio interface (IO_INTERFACE_DESCRIPTION).
*/
MOCKABLE_FUNCTION(, const IO_INTERFACE_DESCRIPTION*, tlsio_rtl8195a_get_interface_description);


/** Expose tlsio state for test proposes.
*/
#define TLSIO_RTL8195A_STATE_VALUES  \
    TLSIO_RTL8195A_STATE_CLOSED,     \
    TLSIO_RTL8195A_STATE_OPENING,    \
    TLSIO_RTL8195A_STATE_OPEN,       \
    TLSIO_RTL8195A_STATE_CLOSING,    \
    TLSIO_RTL8195A_STATE_ERROR,      \
    TLSIO_RTL8195A_STATE_NULL
DEFINE_ENUM(TLSIO_RTL8195A_STATE, TLSIO_RTL8195A_STATE_VALUES);


/** @brief	Return the tlsio state for test proposes.
*
* @param	Unique handle that identifies the tlsio instance.
*
* @return	The tlsio state (TLSIO_ARDUINO_STATE).
*/
TLSIO_RTL8195A_STATE tlsio_rtl8195a_get_state(CONCRETE_IO_HANDLE tlsio_handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TLSIO_ARDUINO_H */