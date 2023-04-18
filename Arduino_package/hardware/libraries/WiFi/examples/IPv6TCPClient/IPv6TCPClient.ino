/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ipv6-tcp/
 */

#include <WiFi.h>

#define MAX_SEND_SIZE 256
#define MAX_RECV_SIZE 1500
#define TCP_SERVER_PORT 5003

char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

//////////////////////////////////////////////////////////////////////////////////////
// Update Server's IPv6 address according to log printed by [IPv6TCPServer] example //
//////////////////////////////////////////////////////////////////////////////////////
char server[] = "fe80:0000:0000:0000:02e0:4cff:fe10:0076";  // name address of server board (no DNS)
char data_recv[MAX_RECV_SIZE];
char data_send[MAX_SEND_SIZE] = "Hi Server!!";

WiFiClient client;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
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

    // enable ipv6 functions:
    if (client.enableIPv6()) {
        if ((client.getIPv6Status()) == 1) {
            Serial.println("\nEnable IPv6 functions");
        } else {
            Serial.println("\nFailed to enable IPv6 functions");
        }
    }
    Serial.println("Connected to wifi");
    printWifiStatus();

    // if you get a connection, report back via serial:
    if (client.connect(server, TCP_SERVER_PORT)) {
        Serial.println("connected to server");
    }
    delay(100);
}

void loop() {
    // continously send data to server:
    if (client.write(data_send, MAX_SEND_SIZE)) {
        Serial.println("\n[CLIENT] Send data to server successfully");
    } else {
        Serial.println("\n[ERROR] Send data failed");
    }

    // if there are incoming bytes available
    // from the server, read and print them:
    if (client.read(data_recv, MAX_RECV_SIZE) < 0) {
        Serial.println("\n[ERROR] Receive data failed");
    } else {
        Serial.print("\n[CLIENT] Receive data from server: ");
    }
    Serial.println(data_recv);
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
    if ((client.getIPv6Status()) == 1) {
        WiFi.printLocalIPv6();
    }

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.println("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm\n");
}
