/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.

*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
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

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient client(mqttServer, 1883, callback, wifiClient);

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
    // In order to republish this payload, a copy must be made
    // as the orignal payload buffer will be overwritten whilst
    // constructing the PUBLISH packet.

    // Allocate the correct amount of memory for the payload copy
    byte* p = (byte*)(malloc(length));
    // Copy the payload to the new buffer
    memcpy(p, payload, length);
    client.publish(publishTopic, p, length);
    // Free the memory
    free(p);
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

    if (client.connect(clientId, clientUser, clientPass)) {
        client.publish(publishTopic, publishPayload);
        client.subscribe(subscribeTopic);
    }
}

void loop() {
    client.loop();
}
