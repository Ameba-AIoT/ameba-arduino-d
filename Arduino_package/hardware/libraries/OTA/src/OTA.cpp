#include "OTA.h"

#ifdef __cplusplus
extern "C" {
#include "rtl8721d_ota.h"
#include <ard_socket.h>
#include <device_lock.h>
}
#endif

OTA::OTA(){};

OTA::~OTA(){};

void OTA::beginOTA(int port) {
    int server_socket = -1, client_socket = -1;
    unsigned char *alloc = NULL;
    int alloc_buf_size = BUF_SIZE;
    int ret = -1;
    u32 RevHdrLen = 0;
    update_ota_target_hdr OtaTargetHdr;
    uint32_t ota_target_index = OTA_INDEX_2;
    uint32_t file_info[3];
    u8 i = 0;

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


    /*--------------------step2: receive firmware information-----------------*/
    /* Receive file_info[] from server. */
    memset(file_info, 0, sizeof(file_info));
    if(!recv_file_info_from_server((u8 *)file_info, sizeof(file_info), client_socket)) {
        printf("[%s] [ERROR] Receive firmware file_info failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Receive file_info done\n", __FUNCTION__);
    
    /* Receive firmware header */
    if(!recv_ota_file_hdr(alloc, &RevHdrLen, &OtaTargetHdr, client_socket)) {
        printf("[%s] [ERROR] Receive firmware header failed", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Receive firmware header done\n", __FUNCTION__);


    /* -----step3: parse firmware header and get target OTA image header-----*/
    if(!get_ota_tartget_header(alloc, RevHdrLen, &OtaTargetHdr, ota_target_index)) {
        printf("[%s] [ERROR] Get OTA header failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Get OTA header done\n", __FUNCTION__);


    /*-----------------step4: erase flash space for new firmware-------------*/
    printf("\tErase is ongoing...\n");
    for(i = 0; i < OtaTargetHdr.ValidImgCnt; i++) {
        erase_ota_target_flash(OtaTargetHdr.FileImgHdr[i].FlashAddr, OtaTargetHdr.FileImgHdr[i].ImgLen);
    }
    printf("[%s] Erase flash for new firmware done\n", __FUNCTION__);


    /*---------step5: download new firmware from server and write it to flash--------*/
    if(download_new_fw_from_server(client_socket, &OtaTargetHdr, ota_target_index) == _FALSE){
        printf("[%s] [ERROR] Download new firmware failed\n", __FUNCTION__);
        goto update_ota_exit;
    }
    printf("[%s] Download new firmware done\n", __FUNCTION__);


     /*-------------step6: verify checksum and update signature-----------------*/
    if(verify_ota_checksum(&OtaTargetHdr)){
        if(!change_ota_signature(&OtaTargetHdr, ota_target_index)) {
            printf("[%s] [ERROR] Update OTA signature failed\n", __FUNCTION__);
            goto update_ota_exit;
        }
        printf("[%s] Update OTA signature done\n", __FUNCTION__);
        ret = 0;
    } 


    /*-------------step7: OTA upgrade successfully, restart device------------*/
    if(!ret){
        printf("[%s] Ready to reboot\n", __FUNCTION__);	
        ota_platform_reset();
    }

    update_ota_exit:
    if(alloc){
        ota_update_free(alloc);
    }
    if(server_socket >= 0 || client_socket >=0){
        close_socket(server_socket);
        close_socket(client_socket);
    }	
    printf("[%s] Update task exit\n", __FUNCTION__);

}

void OTA::syncOTAAddr(u32 ota_target_index){
    /* check OTA index we should update */
    if (ota_get_cur_index() == OTA_INDEX_1) {
        ota_target_index = OTA_INDEX_2;
    } else {
        ota_target_index = OTA_INDEX_1;
    }
    printf("[%s] OTA_INDEX_%d will be upgraded\n", __FUNCTION__, (int)ota_target_index);
}
