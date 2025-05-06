/*
 This sketch shows how to open/close file and perform read/write to it.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-fatfssdio/
 */

// Core RTL8720DN do not support SDIO. Please use SPI interface for SD card
#ifndef CORE_RTL8720DN

#include "FatFs_SD.h"

char filename[] = "test.txt";
char write_content[] = "hello world!";

FatFsSD fs;

void setup() {
    char buf[128];
    char absolute_filename[128];

    fs.begin();

    printf("write something to \"%s\"\r\n", filename);
    sprintf(absolute_filename, "%s%s", fs.getRootPath(), filename);
    SdFatFile file = fs.open(absolute_filename);

    file.println(write_content);

    file.close();
    printf("write finish\r\n\r\n");

    printf("read back from \"%s\"\r\n", filename);
    file = fs.open(absolute_filename);

    memset(buf, 0, sizeof(buf));
    file.read(buf, sizeof(buf));

    file.close();
    printf("==== content ====\r\n");
    printf("%s", buf);
    printf("====   end   ====\r\n");

    fs.end();
}

void loop() {
    delay(1000);
}

#else

void setup() {}

void loop() {}

#endif
