#include <WiFi.h>

char ssid[] = "yourNetwork";        //  your network SSID (name)
char pass[] = "secretPassword";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "www.google.com";    // name address for Google (using DNS)
//unsigned char test_client_key[] = "";   //For the usage of verifying client
//unsigned char test_client_cert[] = "";  //For the usage of verifying client
//unsigned char test_ca_cert[] = "";      //For the usage of verifying server

WiFiSSLClient client;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
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
        status = WiFi.begin(ssid,pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("\n\rConnected to wifi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, 443)) { //client.connect(server, 443, test_ca_cert, test_client_cert, test_client_key)
        Serial.println("connected to server");
        // Make a HTTP request:
        client.println("GET /search?q=realtek HTTP/1.0");
        client.println("Host: www.google.com");
        client.println("Connection: close");
        client.println();
    } else {
        Serial.println("connected to server failed");
    }
    delay(100);
}

void loop() {
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
        char c = client.read();
        Serial.write(c);
    }

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();

        // do nothing forevermore:
        while (true);
    }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    Serial.print(mac[0], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.println(mac[5], HEX);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
