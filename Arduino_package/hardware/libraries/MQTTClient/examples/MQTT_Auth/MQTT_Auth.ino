/*
 MQTT Authentication example
  - connects to an MQTT server, providing username and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-mqtt-upload-listen/
 */

#include <WiFi.h>
#include <PubSubClient.h>

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password
int status = WL_IDLE_STATUS;        // Indicater of Wifi status

char mqttServer[]     = "cloud.amebaiot.com";
char clientId[]       = "amebaClient";
char clientUser[]     = "testuser";
char clientPass[]     = "testpass";
char publishTopic[]   = "outTopic";
char publishPayload[] = "hello world";
char subscribeTopic[] = "inTopic";

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
    }
    Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(mqttServer, 1883, callback, wifiClient);

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    // wait for serial port to connect.
    while (!Serial) {
        ;
    }

    //Attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
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
