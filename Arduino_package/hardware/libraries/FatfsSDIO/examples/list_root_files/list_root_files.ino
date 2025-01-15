/*
 This sketch shows how to list files under root directory.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-fatfssdio/
 */

// Core RTL8720DN do not support SDIO. Please use SPI interface for SD card
#ifndf CORE_RTL8720DN

#include "FatFs_SD.h"

FatFsSD fs;

void setup() {
    char buf[512];
    char *p;

    fs.begin();

    /* list root directory and put results in buf */
    memset(buf, 0, sizeof(buf));
    fs.readDir(fs.getRootPath(), buf, sizeof(buf));

    printf("Files under \"%s\":\r\n", fs.getRootPath());

    /* the filenames are separated with '\0', so we scan one by one */
    p = buf + strlen(buf) + 1;
    while (strlen(p) > 0) {
        printf("%s\r\n", p);
        p += strlen(p) + 1;
    }

    fs.end();
}

void loop() {
    delay(1000);
}

#else

void setup() {}

void loop() {}

#endif
