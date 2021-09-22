/*
 This sketch shows how to connect to IBM Watson IoT Platform

 Go to this website and fill in device's mac.
   https://quickstart.internetofthings.ibmcloud.com/#/
 You can find device's mac in the log like this:
   clientId:d:quickstart:iotsample-ameba:00ffbbccde02
 Ex. In this log, the mac is 00ffbbccde02

 Or you can just type the site link with mac:
   https://quickstart.internetofthings.ibmcloud.com/#/device/00ffbbccde02/sensor/

*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

char ssid[] = "yourNetwork";     // your network SSID (name)
char pass[] = "secretPassword";  // your network password
int status  = WL_IDLE_STATUS;    // the Wifi radio's status

char mqttServer[]     = "quickstart.messaging.internetofthings.ibmcloud.com";
char clientId[44];
char publishTopic[]   = "iot-2/evt/status/fmt/json";
char publishPayload[128];

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    unsigned int i;
    for (i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
    }
    Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.println("Connecting to IoT Foundation for publishing Temperature");
        // Attempt to connect
        if (client.connect(clientId)) {
            Serial.println("Connected successfully\n");
            Serial.println("Device Event (JSON) \t Temperature(in C)");
            Serial.println("____________________________________________________________________________");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        if (status == WL_CONNECTED) {
            break;
        }
        // wait 1 seconds for retry
        delay(1000);
    }

    byte mac[6];
    WiFi.macAddress(mac);
    sprintf(clientId, "d:quickstart:iotsample-ameba:%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("\r\nclientId:%s\r\n", clientId);

    client.setServer(mqttServer, 1883);
    client.setCallback(callback);

    // Allow the hardware to sort itself out
    delay(1500);
}

void loop() {
    if (!(client.connected())) {
        reconnect();
    }

    memset(publishPayload, 0, sizeof(publishPayload));
    sprintf(publishPayload, "{\"d\":{\"myName\":\"Ameba\",\"temperature\":\"%d\"}}", getTemp());
    printf("%s\r\n", publishPayload);
    client.publish(publishTopic, publishPayload);
    delay(1000);
}

int fakeTemp = 20;
int getTemp(void) {
    fakeTemp += 1;
    if (fakeTemp > 30) {
        fakeTemp = 20;
    }
    return fakeTemp;
}