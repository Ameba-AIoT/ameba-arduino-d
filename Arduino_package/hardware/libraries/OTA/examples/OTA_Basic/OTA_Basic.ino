/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ota/
 */

#include <OTA.h>
#include <WiFi.h>
#include <AmebaMDNS.h>

#define OTA_PORT 8082

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;        // Indicater of Wifi status

int ret = -1;

OTA ota;
MDNSService service("MyAmeba", "_arduino._tcp", "local", 5000);

void setup() {
    // Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ;  // wait for serial port to connect.
    }
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("[MAIN] Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP
        // network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    // you're connected now, so print out the status:
    printWifiStatus();

    // setup MDNS service to host OTA Server on the
    // Arduino Network Port
    beginMDNSService();

    // start connecting to OTA server and reboot 
    // with the new image
    ota.beginOTA(OTA_PORT);
}

void loop() {}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void beginMDNSService() {
    service.addTxtRecord("board", strlen("ameba_rtl8721d"), "ameba_rtl8721d");
    service.addTxtRecord("auth_upload", strlen("no"), "no");
    service.addTxtRecord("tcp_check", strlen("no"), "no");
    service.addTxtRecord("ssh_upload", strlen("no"), "no");

    printf("Start mDNS service\r\n");
    MDNS.begin();

    printf("register mDNS service\r\n");
    MDNS.registerService(service);
}
