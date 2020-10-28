#include <WiFi.h>

char ssid[] = "yourNetwork";      //  your network SSID (name)
char pass[] = "secretPassword";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(5000);

void setup() {
    Serial.begin(115200);      // initialize serial communication
    pinMode(9, OUTPUT);      // set the LED pin mode

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);       // don't continue
    }

    // attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);                   // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.begin();                           // start the tcp server on port 5000
    printWifiStatus();                        // you're connected now, so print out the status
}

char buffer[256];
void loop() {
    WiFiClient client = server.available();

    while (client.connected()) {
        memset(buffer, 0, 256);
        int n = client.read((uint8_t*)(&buffer[0]), sizeof(buffer));
        if (n > 0) {
            for (int i = 0; i < n; i++) {
                Serial.print(buffer[i]);
            }
            n = client.write(buffer, n);
            if (n <= 0) {
                break;
            }
        }
    }

    client.stop();
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
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
