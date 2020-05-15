/*
 Basic MQTT example with Authentication

  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

char ssid[] = "yourNetwork";     // your network SSID (name)
char pass[] = "secretPassword";  // your network password

int status  = WL_IDLE_STATUS;    // the Wifi radio's status

char mqttServer[] = "cloud.amebaiot.com";

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
PubSubClient client(mqttServer, 1883, callback, wifiClient);

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

    // Note - the default maximum packet size is 512 bytes. If the
    // combined length of clientId, username and password exceed this,
    // you will need to increase the value of MQTT_MAX_PACKET_SIZE in
    // PubSubClient.h

    if (client.connect(clientId, clientUser, clientPass)) {
        client.publish(publishTopic, publishPayload);
        client.subscribe(subscribeTopic);
    }
}

void loop() {
    client.loop();
}
