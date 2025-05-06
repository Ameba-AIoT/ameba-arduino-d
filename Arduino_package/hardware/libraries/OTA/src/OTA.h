#ifndef __OTA_H__
#define __OTA_H__

#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <cmsis_os.h>
#include <osdep_service.h>
#include <rtl8721d_ota.h>
#include <wifi_conf.h>
#include <wifi_constants.h>
#include <wiring_os.h>

class OTA {
public:
    OTA(void);
    ~OTA(void);

    // Starts to connect to OTA server and receive the new firmware
    // To begin OTA firmware update process via MDNS
    void beginOTA(int port);

    // To start OTA firmware update process via HTTP
    void start_OTA_threads(int port, char *server);

private:
    // Check which OTA index to be updated
    void syncOTAAddr(u32 ota_target_index);

    static void thread1_task(void *param);
    static void thread2_task(void *param);
    static void http_update_ota_task(void *param);
    static void ota_http(void);

    static uint16_t _port;
    static char *_server;

    static char *g_otaState;
    static char *_resource;

    static char *OtaState[5];

    static JsonDocument doc;
    static String jsonString;
    static char buffer[1024];
};
#endif
