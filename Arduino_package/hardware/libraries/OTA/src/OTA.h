#ifndef __OTA_H__
#define __OTA_H__

#include "Arduino.h"
#include <WiFi.h>

#define KM0_OTA1_ADDR 0x00006000
#define OTA2_ADDR 0X00106000

class OTA
{
public:
    OTA(void);
    ~OTA(void);

    // Starts to connect to OTA server and receiv the new firmware
    void beginOTA(int port);

private:
    // Check which OTA index to be updated
    void syncOTAAddr(u32 ota_target_index);
};

#endif
