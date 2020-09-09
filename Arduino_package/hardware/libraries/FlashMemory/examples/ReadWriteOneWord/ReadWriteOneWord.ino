/*
 This sketch shows how to request flash memory larger than defaut 4K, and read/write one specific word.
 */

#include <FlashMemory.h>

void setup() {
    unsigned int value;

    /* request flash size 0x2000 from 0x00100000 */
    FlashMemory.begin(0x00100000, 0x2000);

    /* read one word (32-bit) from 0x00100000 plus offset 0x1E00 */
    value = FlashMemory.readWord(0x1E00);

    printf("value is 0x%08X\r\n", value);

    if (value == 0xFFFFFFFF) {
        value = 0;
    } else {
        value++;
    }

    /* write one word (32-bit) to 0x00100000 plus offset 0x3F00 */
    FlashMemory.writeWord(0x1E00, value);
}

void loop() {
    // put your main code here, to run repeatedly:
}
