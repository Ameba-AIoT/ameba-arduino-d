## Ameba_ATCMD_ESP

### Introduction

Ameba_ATCMD_ESP is an Arduino sketch for the AmebaD RTL872xD development boards that receives and processes the ESP AT command set for ESP8266/ESP32.

### Usage

To use this sketch, open the "Ameba_ATCMD_ESP" sketch in Arduino, compile and upload to an AmebaD board.

The default UART port used for AT commands is TX pin PB_1 and RX pin PB_2.
Default UART configuration is 9600 baud, 8 data bits, 1 stop bit, no parity bit, no flow control.

Follow the example guide in the `ESP_ATCMD_on_AmebaD guide` folder on how to use this with an Arduino board.

### Supported ESP AT commands

| Basic AT commands | Wi-Fi AT commands | TCP/IP AT commands |
| ----------------- | ----------------- | ------------------ |
| AT                | AT+CWMODE         | AT+CIPSTATUS       |
| AT+RST            | AT+CWSTATE        | AT+CIPDOMAIN       |
| AT+GMR            | AT+CWJAP          | AT+CIPSTART        |
| ATE0              | AT+CWRECONNCFG    | AT+CIPSEND         |
| ATE1              | AT+CWLAP          | AT+CIPCLOSE        |
| AT+UART_CUR       | AT+CWQAP          | AT+CIFSR           |
| AT+SYSRAM         | AT+CWSAP          | AT+CIPMUX          |
|                   | AT+CWLIF          | AT+CIPMODE         |
|                   | AT+CWQIF          | AT+CIPDINFO        |
|                   | AT+CWDHCP         | AT+CIPSSLCSNI      |
|                   | AT+CWDHCPS        | AT+CIPRECONNINTV   |
|                   | AT+CIPSTAMAC      | AT+CIPRECVMODE     |
|                   | AT+CIPAPMAC       | AT+CIPRECVDATA     |
|                   | AT+CIPSTA         | AT+CIPRECVLEN      |
|                   | AT+CIPAP          | AT+PING            |
|                   | AT+CWHOSTNAME     | AT+CIPDNS          |

Note that only 2 simultaneous SSL connections can be made due to RAM limitations.

### AT commands syntax reference

Documentation and detailed instructions on how to use these AT commands can be found at Espressif's [documentation page](https://espressif-docs.readthedocs-hosted.com/projects/esp-at/en/release-v2.2.0.0_esp32/AT_Command_Set/index.html).
