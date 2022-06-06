#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
extern void _tone(uint32_t ulPin, unsigned int frequency, unsigned long duration);
}
#endif

// a wrapper that support default value of duration
void tone(uint32_t ulPin, unsigned int frequency, unsigned long duration)
{
    _tone(ulPin, frequency, duration);
}
