/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ipv6-udp/
 */

#include <WiFi.h>

#define MAX_SEND_SIZE 256
#define MAX_RECV_SIZE 1500
#define UDP_SERVER_PORT 5002

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password
int status = WL_IDLE_STATUS;        // Indicater of Wifi status

char data_recv[MAX_RECV_SIZE];
char data_send[MAX_SEND_SIZE] = "Hi client!!";

WiFiUDP UDPserver;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }

    // Enable ipv6 function
    if (UDPserver.enableIPv6()) {
        if ((UDPserver.getIPv6Status()) == 1) {
            Serial.println("\nEnable IPv6 functions");
        } else {
            Serial.println("\nFailed to enable IPv6 functions");
        }
    }
    Serial.println("Connected to wifi");
    printWifiStatus();

    // if you get a connection, report back via serial:
    UDPserver.UDPServerv6();
}

void loop() {}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.print(ip);

    // print link local IPv6 address:
    if ((UDPserver.getIPv6Status()) == 1) {
        WiFi.printLocalIPv6();
    }

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm\n");
}
