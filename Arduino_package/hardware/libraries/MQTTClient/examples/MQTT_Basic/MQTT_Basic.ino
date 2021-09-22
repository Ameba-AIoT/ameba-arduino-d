/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

char ssid[] = "yourNetwork";     // your network SSID (name)
char pass[] = "secretPassword";  // your network password
int status  = WL_IDLE_STATUS;    // the Wifi radio's status

char mqttServer[]     = "test.mosquitto.org";
char clientId[]       = "amebaClient";
char publishTopic[]   = "outTopic";
char publishPayload[] = "hello world";
char subscribeTopic[] = "inTopic";

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
    }
    Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
    // Loop until we're reconnected
    while (!(client.connected())) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientId)) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(publishTopic, publishPayload);
            // ... and resubscribe
            client.subscribe(subscribeTopic);
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

    // Allow the hardware to sort itself out
    delay(1500);
}

void loop() {
    if (!(client.connected())) {
        reconnect();
    }
    client.loop();
}
