/*
 This sketch shows how to create a folder and open a file under it.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-fatfssdio/
 */

// Core RTL8720DN do not support SDIO. Please use SPI interface for SD card
#ifndef CORE_RTL8720DN

#include "FatFs_SD.h"

char dirname[] = "testdir";
char filename[] = "test.txt";
char write_content[] = "hello world!";

FatFsSD fs;

void setup() {
    char buf[128];
    char absolute_filename[128];

    fs.begin();

    sprintf(absolute_filename, "%s%s", fs.getRootPath(), dirname);
    fs.mkdir(absolute_filename);
    printf("create dir at \"%s\"\r\n", absolute_filename);

    sprintf(absolute_filename, "%s%s/%s", fs.getRootPath(), dirname, filename);
    SdFatFile file = fs.open(absolute_filename);
    file.println(write_content);
    file.close();
    printf("create file at \"%s\"\r\n", absolute_filename);

    printf("read back from \"%s\"\r\n", absolute_filename);
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
