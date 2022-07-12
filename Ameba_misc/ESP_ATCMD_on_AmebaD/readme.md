## Ameba_ATCMD_ESP

### Introduction

Ameba_ATCMD_ESP is an Arduino sketch for the AmebaD RTL872xD development boards that receives and processes the ESP AT command set for ESP8266/ESP32.

### Usage

To use this sketch, open the "Ameba_ATCMD_ESP" sketch in Arduino, compile and upload to an AmebaD board.

The default UART port used for AT commands is TX pin PB_1 and RX pin PB_2.
Default UART configuration is 115200 baud, 8 data bits, 1 stop bit, no parity bit, no flow control.

Follow the example guide in the `ESP_ATCMD_on_AmebaD guide` folder on how to use this with an Arduino board.

### Supported ESP AT commands

- Basic AT commands
  - AT
  - AT+RST
  - AT+GMR
  - ATE0
  - ATE1
  - AT+UART_CUR
  - AT+SYSRAM
 
- Wi-Fi AT commands
  - AT+CWSTATE
  - AT+CWJAP
  - AT+CWLAP
  - AT+CWQAP
  - AT+CWRECONNCFG
 
- TCP/IP AT commands
  - AT+CIPSTATUS
  - AT+CIPDOMAIN
  - AT+CIPSTART
  - AT+CIPSEND
  - AT+CIPCLOSE
  - AT+CIFSR
  - AT+CIPMUX
  - AT+CIPMODE
  - AT+CIPDINFO
  - AT+CIPRECONNINTV
  - AT+PING

Note that for TCP/IP AT commands, only TCP connections are currently supported.

### AT commands syntax reference

Documentation and detailed instructions on how to use these AT commands can be found at Espressif's [documentation page](https://espressif-docs.readthedocs-hosted.com/projects/esp-at/en/release-v2.2.0.0_esp32/AT_Command_Set/index.html).
