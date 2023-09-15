#ifndef _AMB_ARD_PIN_CHECK_
#define _AMB_ARD_PIN_CHECK_

#include "Arduino.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include "PinNames.h"

// ameba - arduino pin mapping name check
void __attribute__((weak)) amb_ard_pin_check_name(int pin);

// ameba - arduino pin mapping type check
void __attribute__((weak)) amb_ard_pin_check_type(int pin, uint32_t pin_type);

// ameba - arduino pin mapping function check
void __attribute__((weak)) amb_ard_pin_check_fun(int pin, uint32_t pin_fun);

#ifdef __cplusplus
}
#endif

#endif
