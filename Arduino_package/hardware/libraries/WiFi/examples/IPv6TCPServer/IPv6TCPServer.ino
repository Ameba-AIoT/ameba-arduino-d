#include <WiFi.h>

#define MAX_SEND_SIZE 256
#define MAX_RECV_SIZE 1500
#define TCP_SERVER_PORT 5003

char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

char data_send[MAX_SEND_SIZE] = "Hi client!!";
char data_recv[MAX_RECV_SIZE];

int server_fd, client_fd;
WiFiServer server(TCP_SERVER_PORT);

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

    // Enable ipv6 function
    if (server.enableIPv6()) {
        if ((server.getIPv6Status()) == 1) {
            Serial.println("\nEnable IPv6 functions");
        } else {
            Serial.println("\nFailed to enable IPv6 functions");
        }
    }
    printWifiStatus();

    // conenct to server and listen for incoming sockets
    server.begin();
    client_fd = server.available(server_fd);
    delay(100);
}

void loop() {
    // continously read from to client:
    if (server.recv((uint8_t*)data_recv, MAX_RECV_SIZE) > 0) {
        Serial.print("\n[SERVER] Receive data: ");
        Serial.println(data_recv);

        // send response back to client:
        if (server.write((const uint8_t*)data_send, MAX_SEND_SIZE) > 0) {
            Serial.println("\n[SERVER] Send data successfully");
        } else {
            Serial.println("\n[ERROR] Send data failed");
        }
    }
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
    if ((server.getIPv6Status()) == 1) {
        WiFi.printLocalIPv6();
    }

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("\nsignal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm\n");
}
