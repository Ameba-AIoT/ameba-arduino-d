/*
 This sketch shows how to check whether a item is a directory or a file.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-fatfssdio/
 */

// Core RTL8720DN do not support SDIO. Please use SPI interface for SD card
#ifndf CORE_RTL8720DN

#include "FatFs_SD.h"

FatFsSD fs;

void setup() {
    char buf[512];
    char absolute_filename[512];
    char *p;

    fs.begin();

    fs.readDir(fs.getRootPath(), buf, sizeof(buf));

    p = buf + strlen(buf) + 1;
    while (strlen(p) > 0) {
        printf("\"%s\" is ", p);

        sprintf(absolute_filename, "%s%s", fs.getRootPath(), p);
        if (fs.isDir(absolute_filename)) {
            printf("dir\r\n");
        }

        if (fs.isFile(absolute_filename)) {
            printf("file\r\n");
        }

        p += strlen(p) + 1;
    }

    fs.end();
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(1000);
}

#else

void setup() {}

void loop() {}

#endif
