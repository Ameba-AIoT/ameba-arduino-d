/*
 * This sketch shows how to get current OS free heap size
 *
 * In FreeRTOS, there is a heap for memory allocale and creating thread.
 * If heap size is too low, then some function may behave abnormal from the failure of memory allocation.
 */

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    char *buf;
    Serial.print("Dynamic memory size: ");
    Serial.println(os_get_free_heap_size_arduino());
    Serial.println();

    buf = (char *)(malloc(1000));
    Serial.println("Allocate 1000 bytes");
    Serial.print("Dynamic memory size: ");
    Serial.println(os_get_free_heap_size_arduino());
    Serial.println();

    free(buf);
    Serial.println("Free 1000 bytes");
    Serial.print("Dynamic memory size: ");
    Serial.println(os_get_free_heap_size_arduino());
}

void loop() {
    delay(1000);
}
