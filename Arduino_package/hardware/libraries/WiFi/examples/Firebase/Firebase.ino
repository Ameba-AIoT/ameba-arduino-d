#include <WiFi.h>

#define ACCESS_TOKEN ""     //paste your instance ID from android app
#define SERVER_KEY  ""      //paste server key available on Firebase cloud messaging 
#define HOST_NAME "fcm.googleapis.com"

char ssid[] = "SSID";       //  your network SSID (name)
char pass[] = "PASSWORD";   // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = "fcm.googleapis.com";    // name address for Google (using DNS)

char const* payload = "{" \
    "\"to\": \"" ACCESS_TOKEN "\"," \
    "\"notification\": {" \
    "\"body\": \"Hello World!\"," \
    "\"title\" : \"From Realtek Ameba\" " \
    "} }" ;

char const* message_fmt = "POST /fcm/send HTTP/1.1\nContent-Type: application/json\nAuthorization: key=" SERVER_KEY "\nHost: " HOST_NAME "\nContent-Length: ";

WiFiSSLClient client; //Due to recent FCM update, HTTPS is needed instead of HTTP
//WiFiClient client;

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
        delay(3000);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();

    char message[512] ;

    Serial.println("\nStarting connection to server...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 443)) {  //HTTPS default port 443
        Serial.println("connected to server");
        // Make a HTTP request:
        sprintf(message, "%s%s%s%s%s%d%s%s%s", "POST /fcm/send HTTP/1.1\nContent-Type: application/json\nAuthorization: key=", SERVER_KEY, "\nHost: ", HOST_NAME, "\nContent-Length: ", strlen(payload), "\n\n", payload, "\n");
        printf("\nRequest:\n%s \n", message);
        client.println(message);
        client.println();
    }
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

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
