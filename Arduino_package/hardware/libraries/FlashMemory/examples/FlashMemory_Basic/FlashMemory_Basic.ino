/*
 * This sketch demonstrates the use of flash memory
 * 
 * Ameba's flash memory can be edit in a unit of sector which has size of 
 * 4K bytes.
 *
 * To read data from flash memory, we can just read it.
 *
 * To write data to flash memory, each bit on flash memory can only change
 * from '1' to '0' and it cannot change from '0' to '1'. To make sure the
 * data are correctly written we do erase flash memory sector before write
 * data on it.
 * 
 * This sketch store boot count in flash. Each time device boot up, it read
 * boot count from flash, add one, and write back to flash.
 */

#include <FlashMemory.h>

void setup() {
    Serial.begin(115200);
    FlashMemory.read();

    if (FlashMemory.buf[0] == 0xFF) {
        FlashMemory.buf[0] = 0x00;
        FlashMemory.update();
        Serial.println("write count to 0");
    } else {
        FlashMemory.buf[0]++;
        FlashMemory.update();
        Serial.print("Boot count: ");
        Serial.println(FlashMemory.buf[0]);
    }
}

void loop() {
    delay(1000);
}
