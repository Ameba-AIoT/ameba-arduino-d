/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ipv6-udp/
 */

#include <WiFi.h>

#include "Arduino.h"

#define MAX_SEND_SIZE 256
#define MAX_RECV_SIZE 1500
#define UDP_SERVER_PORT 5002

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password
int status = WL_IDLE_STATUS;        // Indicator of Wifi status

//////////////////////////////////////////////////////////////////////////////////////
// Update Server's IPv6 address according to log printed by [IPv6UDPServer] example //
//////////////////////////////////////////////////////////////////////////////////////
char server[] = "fe80:0000:0000:0000:02e0:4cff:fe10:0076";
char data_recv[MAX_RECV_SIZE];
char data_send[MAX_SEND_SIZE] = "Hi Server!!";

WiFiUDP UDPclient;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    // enable ipv6 function
    if (UDPclient.enableIPv6()) {
        if ((UDPclient.getIPv6Status()) == 1) {
            Serial.println("\nEnable IPv6 functions");
        } else {
            Serial.println("\nFailed to enable IPv6 functions");
        }
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
    Serial.println("Connected to wifi");
    printWifiStatus();

    // if you get a connection, report back via serial:
    if (UDPclient.connect(server, UDP_SERVER_PORT)) {
        Serial.println("connected to server\n");
    }
    delay(100);
}

void loop() {
    // send data to server
    if (UDPclient.writeImmediately((const uint8_t *)data_send, MAX_SEND_SIZE, (uint32_t)server, UDP_SERVER_PORT) == -1) {
        Serial.println("\n\r[ERROR] Send data failed\n");
    } else {
        Serial.println("\n\r[INFO] Send data to server successfully\n");
    }

    // if there are incoming bytes available
    // from the server, read and print them:
    if (UDPclient.read((uint8_t *)data_recv, MAX_SEND_SIZE) <= 0) {
        Serial.println("[ERROR] Receive data failed\n");
    } else {
        Serial.print("[INFO] Receive from server: ");
        Serial.println(data_recv);
    }

    // send data to server every 1 second
    // delay(1000);
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.print(ip);

    // print link local IPv6 address:
    if ((UDPclient.getIPv6Status()) == 1) {
        WiFi.printLocalIPv6();
    }

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm\n");
}
