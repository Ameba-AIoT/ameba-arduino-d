/*
 Reconnecting MQTT example - non-blocking

 This sketch demonstrates how to keep the client connected
 using a non-blocking reconnect function. If the client loses
 its connection, it attempts to reconnect every 5 seconds
 without blocking the main loop.

*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your hardware/network.
char ssid[] = "yourNetwork";     // your network SSID (name)
char pass[] = "secretPassword";  // your network password
int status  = WL_IDLE_STATUS;    // the Wifi radio's status

char mqttServer[]     = "cloud.amebaiot.com";
char clientId[]       = "amebaClient"; 
char clientUser[]     = "testuser";
char clientPass[]     = "testpass";
char publishTopic[]   = "outTopic";
char publishPayload[] = "hello world";
char subscribeTopic[] = "inTopic";

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

long lastReconnectAttempt = 0;

boolean reconnect() {
    if (client.connect(clientId, clientUser, clientPass)) {
        // Once connected, publish an announcement...
        client.publish(publishTopic, publishPayload);
        // ... and resubscribe
        client.subscribe(subscribeTopic);
    }
    return client.connected();
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

        // wait 10 seconds for connection:
        delay(10000);
    }

    client.setServer(mqttServer, 1883);
    client.setCallback(callback);
    client.connect(clientId, clientUser, clientPass);

    delay(1500);
    lastReconnectAttempt = 0;
}


void loop() {
    if (!(client.connected())) {
        long now = millis();
        if ((now - lastReconnectAttempt) > 5000) {
            lastReconnectAttempt = now;
            // Attempt to reconnect
            if (reconnect()) {
                lastReconnectAttempt = 0;
            }
        }
    } else {
        // Client connected
        client.loop();
    }
}
