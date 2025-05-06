#include "OTA.h"

#ifdef __cplusplus
extern "C" {
#include "rtl8721d_ota.h"
#include <ard_socket.h>
#include <device_lock.h>
}
#endif

// DO NOT MODIFY
char *OTA::OtaState[5] = {"OTA_STATE_IDLE", "OTA_STATE_RECEIVED_START_SIGNAL",
                          "OTA_STATE_DOWNLOAD_FIRMWARE_IN_PROGRESS",
                          "OTA_STATE_DOWNLOAD_FIRMWARE_COMPLETED",
                          "OTA_STATE_REBOOT"};

uint16_t OTA::_port = 3000;
char *OTA::_server = "192.168.13.14";
char *OTA::g_otaState = OtaState[0];
char *OTA::_resource = "api/uploadfile";

JsonDocument OTA::doc;
String OTA::jsonString;
char OTA::buffer[1024];

OTA::OTA(){};

OTA::~OTA(){};

/***********************************************************************************
 *
 *
 * MDNS OTA Update
 *
 *
 ***********************************************************************************/

void OTA::beginOTA(int port)
{
    int server_socket = -1, client_socket = -1;
    unsigned char *alloc = NULL;
    int alloc_buf_size = BUF_SIZE;
    int ret = -1;
    u32 RevHdrLen = 0;
    update_ota_target_hdr OtaTargetHdr;
    uint32_t ota_target_index = OTA_INDEX_2;
    uint32_t file_info[3];

    printf("[%s] Alloc buffer Init\n", __FUNCTION__);
    alloc = (unsigned char *)ota_update_malloc(alloc_buf_size);
    if (!alloc) {
        printf("[%s] [ERROR] Alloc buffer failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Alloc buffer Succeed\n", __FUNCTION__);

    /*--------------------step1: Connect to OTA Server-------------------------*/
    server_socket = ota_bind_socket(server_socket, port);
    printf("Waiting for client ...\n");
    client_socket = ota_accept_socket(server_socket);
    if (server_socket == -1 || client_socket == -1) {
        goto update_ota_exit;
    }
    syncOTAAddr(ota_target_index);

    if (ota_get_cur_index() == OTA_INDEX_1) {
        ota_target_index = OTA_INDEX_2;
        printf("Current index = 1, Target index = 2\n");
    } else {
        ota_target_index = OTA_INDEX_1;
        printf("Current index = 2, Target index = 1\n");
    }

    /*--------------------step2: receive firmware information-----------------*/
    /* Receive file_info[] from server. */
    memset(file_info, 0, sizeof(file_info));
    if (!recv_file_info_from_server((u8 *)file_info, sizeof(file_info),
                                    client_socket)) {
        printf("[%s] [ERROR] Receive firmware file_info failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Receive file_info done\n", __FUNCTION__);

    /* Receive firmware header */
    if (!recv_ota_file_hdr(alloc, &RevHdrLen, &OtaTargetHdr, client_socket)) {
        printf("[%s] [ERROR] Receive firmware header failed", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Receive firmware header done\n", __FUNCTION__);

    /* -----step3: parse firmware header and get target OTA image header-----*/
    if (!get_ota_tartget_header(alloc, RevHdrLen, &OtaTargetHdr,
                                ota_target_index)) {
        printf("[%s] [ERROR] Get OTA header failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Get OTA header done\n", __FUNCTION__);

    /*-----------------step4: erase flash space for new firmware-------------*/
    printf("\tErase is ongoing...\n");
    // for(i = 0; i < OtaTargetHdr.ValidImgCnt; i++) {
    //     erase_ota_target_flash(OtaTargetHdr.FileImgHdr[i].FlashAddr,
    //     OtaTargetHdr.FileImgHdr[i].ImgLen);
    // }
    // printf("[%s] Erase flash for new firmware done\n", __FUNCTION__);

    if (ota_target_index == 1) {
        printf("\n\rErasing OTA target flash index 2...");
        erase_ota_target_flash(0x08106000, OtaTargetHdr.FileImgHdr[0].ImgLen);
    } else {
        printf("\n\rErasing OTA target flash index 1...");
        erase_ota_target_flash(0x08006000, OtaTargetHdr.FileImgHdr[0].ImgLen);
    }

    /*---------step5: download new firmware from server and write it to
     * flash--------*/
    if (download_new_fw_from_server(client_socket, &OtaTargetHdr,
                                    ota_target_index)
        == _FALSE) {
        printf("[%s] [ERROR] Download new firmware failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("\n\r[%s] Download new firmware done\n", __FUNCTION__);

    /*-------------step6: verify checksum and update signature-----------------*/
    if (verify_ota_checksum(&OtaTargetHdr)) {
        if (!change_ota_signature(&OtaTargetHdr, ota_target_index)) {
            printf("[%s] [ERROR] Update OTA signature failed\n", __FUNCTION__);
            goto update_ota_exit;
        }
        printf("[%s] Update OTA signature done\n", __FUNCTION__);
        ret = 0;
    }

    /*-------------step7: OTA upgrade successfully, restart device------------*/
    if (!ret) {
        printf("[%s] Ready to reboot\n", __FUNCTION__);
        ota_platform_reset();
    }

update_ota_exit:
    if (alloc) {
        ota_update_free(alloc);
    }
    if (server_socket >= 0 || client_socket >= 0) {
        close_socket(server_socket);
        close_socket(client_socket);
    }
    printf("[%s] Update task exit\n", __FUNCTION__);
}

void OTA::syncOTAAddr(u32 ota_target_index)
{
    /* check OTA index we should update */
    if (ota_get_cur_index() == OTA_INDEX_1) {
        ota_target_index = OTA_INDEX_2;
    } else {
        ota_target_index = OTA_INDEX_1;
    }
    printf("[%s] OTA_INDEX_%d will be upgraded\n", __FUNCTION__,
           (int)ota_target_index);
}

/***********************************************************************************
 *
 *
 * HTTP OTA Update
 *
 *
 ***********************************************************************************/

/**
 *  @brief   Start OTA Threads
 *
 *  @param   port: port number
 *  @param   server: server ip address
 *  @return  None
 *
 */
void OTA::start_OTA_threads(int port, char *server)
{
    _port = port;        // user defined port number
    _server = server;    // user defined server ip addr

    // thread 1 - to keep alive connectivity check (post request every 3.5
    // seconds)
    if (xTaskCreate(thread1_task, (const char *)"thread1_task", 1024, NULL,
                    tskIDLE_PRIORITY + 1, NULL)
        != pdPASS) {
        printf("\n\r[%s] Create task 1 failed", __FUNCTION__);
    }

    // thread 2 - to receive signal to start OTA process
    if (xTaskCreate(thread2_task, (const char *)"thread2_task", 2048, NULL,
                    tskIDLE_PRIORITY + 1, NULL)
        != pdPASS) {
        printf("\n\r[%s] Create task 2 failed", __FUNCTION__);
    }
}

/**
 *  @brief   OTA Thread 1 - to keep alive connectivity check
 *
 *  @param   param: void parameter
 *
 *  @return  None
 *
 */
void OTA::thread1_task(void *param)
{
    (void)param;
    while (1) {
        WiFiClient wifiClient;
        doc["OTA_state"] = g_otaState;
        serializeJson(doc, jsonString);
        if (wifiClient.connect(_server, _port)) {
            // Send POST request
            wifiClient.println("POST /api/connectedclients HTTP/1.1");
            wifiClient.println("Host: " + String(_server));
            wifiClient.println(
                "Content-Type: application/json");    // Use appropriate content type
            wifiClient.println(
                "Content-Length: " + String(jsonString.length()));    // Specify the length of the content
            wifiClient.println("Connection: keep-alive");
            wifiClient.println();    // Empty line indicates the end of headers
            wifiClient.print(jsonString);
        } else {
            Serial.println("Connection to server failed");
        }
        delay(500);
        wifiClient.stop();
        vTaskDelay(3000 / portTICK_PERIOD_MS);    // Sleep for 3 second
    }
}

/**
 *  @brief   OTA Thread 2 - to receive signal to start OTA process
 *
 *  @param   param: void parameter
 *
 *  @return  None
 *
 */
void OTA::thread2_task(void *param)
{
    (void)param;
    WiFiServer server(5000);
    server.begin();
    while (1) {
        WiFiClient client = server.available();
        while (client.connected()) {
            memset(buffer, 0, 1024);
            int n = client.read((uint8_t *)(&buffer[0]), sizeof(buffer));
            if (n > 0) {
                for (int i = 0; i < n; i++) {
                    // Serial.print(buffer[i]);
                }
                n = client.write(buffer, n);
                if (n <= 0) {
                    break;
                }
                if (strstr(buffer, "start_ota")) {
                    Serial.println("\n[OTA] Received start OTA signal from UI.");
                    if (g_otaState == OtaState[0]) {
                        ota_http();
                    }
                }
            }
            vTaskDelay(10000 / portTICK_PERIOD_MS);    // Sleep for 10 second
        }
    }
}

void OTA::http_update_ota_task(void *param)
{
    (void)param;

    int ret = -1;

    g_otaState = OtaState[2];

    ret = http_update_ota(_server, _port, _resource);

    g_otaState = OtaState[3];

    // printf("\n\r[%s] Update task exit", __FUNCTION__);
    if (!ret) {
        // printf("\n\r[%s] Ready to reboot", __FUNCTION__);
        g_otaState = OtaState[4];
        ota_platform_reset();
    }
    vTaskDelete(NULL);
}

/**
 *  @brief   OTA HTTP - to create a thread task using http_update_ota_task
 * [FreeRTOS SDK HTTP OTA Example]
 *
 *  @param   //None
 *
 *  @return  None
 *
 */
void OTA::ota_http(void)
{
    if (xTaskCreate(http_update_ota_task, (const char *)"http_update_ota_task",
                    1024, NULL, tskIDLE_PRIORITY + 1, NULL)
        != pdPASS) {
        printf("\n\r[%s] Create update task failed", __FUNCTION__);
    }
}
