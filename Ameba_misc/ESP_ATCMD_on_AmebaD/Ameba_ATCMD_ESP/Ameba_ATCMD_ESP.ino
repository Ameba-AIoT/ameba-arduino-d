#include "atcmd_core.h"

void setup() {
  atcmd_init();           // Start ATCMD service for command processing
  atcmd_wifi_init();
  atcmd_tcpip_init();
  atcmd_uart_init();      // Ready to accept commands from UART
}

void loop() {
  // Do nothing
  delay(60000);
}
