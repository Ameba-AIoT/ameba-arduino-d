/*
  This sketch shows how to register Ameba as a service that can be recognized by Arduino IDE.

  If your computer that run Arduino IDE stays in the same local network with Ameba, then you can find Ameba in "Tools" -> "Port" -> "Arduino at 192.168.1.238 (Ameba RTL8195A)".
  It means the Arduino IDE find Ameba via mDNS.

*/

#include <WiFi.h>
#include <AmebaMDNS.h>

char ssid[] = "yourNetwork";     //  your network SSID (name)
char pass[] = "secretPassword";  // your network password

MDNSService service("MyAmeba", "_arduino._tcp", "local", 5000);

void setup() {
    printf("Try to connect to %s\r\n", ssid);
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
        printf("Failed. Wait 1s and retry...\r\n");
        delay(1000);
    }
    printf("\n\rConnected to %s\r\n", ssid);

    service.addTxtRecord("board", strlen("ameba_rtl8721d"), "ameba_rtl8721d");
    service.addTxtRecord("auth_upload", strlen("no"), "no");
    service.addTxtRecord("tcp_check", strlen("no"), "no");
    service.addTxtRecord("ssh_upload", strlen("no"), "no");

    printf("Start mDNS service\r\n");
    MDNS.begin();

    printf("register mDNS service\r\n");
    MDNS.registerService(service);
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(1000);
}
