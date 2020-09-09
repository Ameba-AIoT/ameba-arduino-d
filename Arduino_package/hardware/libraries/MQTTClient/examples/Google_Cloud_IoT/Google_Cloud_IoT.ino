#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include "src/jwt.h"

#define count   100

WiFiSSLClient wifiClient;
PubSubClient client(wifiClient);
WiFiUDP udpClient;
NTPClient timeClient(udpClient);

char ssid[] = "ssid";       // your network SSID (name)
char pass[] = "pass";       // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

char GOOGLE_MQTT_SERVER[] = "mqtt.googleapis.com";
int  GOOGLE_MQTT_PORT = 8883;

char project_id[] = "project-id";
char location[] = "us-central1";
char registry_id[] = "my-registry";
char device_id[] = "amebago-device";

char clientUser[] = "unused";
String clientPass;
char *mqtt_id;
char *pub_topic;
char payload[64];
NN_DIGIT priv_key[9];

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
char *private_key_str =
"57:e1:c0:3b:e5:cd:2f:42:fb:14:4f:a9:1e:3f:0a:"
"65:22:4c:f0:ac:0f:a0:82:e1:73:71:ae:76:70:48:"
"68:fe";

// To get the certificate for your region run:
//   openssl s_client -showcerts -connect mqtt.googleapis.com:8883
// for standard mqtt or for LTS:
//   openssl s_client -showcerts -connect mqtt.2030.ltsapis.goog:8883
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
char* rootCABuff  = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";

void fillPrivateKey(const char *private_key) {
  priv_key[8] = 0;
  for (int i = 7; i >= 0; i--) {
    priv_key[i] = 0;
    for (int byte_num = 0; byte_num < 4; byte_num++) {
      priv_key[i] = (priv_key[i] << 8) + strtoul(private_key, NULL, 16);
      private_key += 3;
    }
  }
}

void setup() {
  Serial.begin(115200);
  // attempt to connect to Wifi network:
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected to wifi");

  fillPrivateKey(private_key_str);
  timeClient.begin();
  wifiClient.setRootCA((unsigned char*)rootCABuff);
}

void loop() {
  boolean ret = 0;

  if (WiFi.status() != WL_CONNECTED) {
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      delay(1000);
    }
    Serial.println("Connected to wifi");
  }

  if(ret == 0) {
    Serial.println("Ready for Publishing");

    mqtt_id = (char *)malloc(strlen("projects/") + strlen(project_id) + strlen("/locations/") + strlen(location)
             + strlen("/registries/") + strlen(registry_id) + strlen("/devices/") + strlen(device_id) + 1);
    sprintf(mqtt_id, "projects/%s/locations/%s/registries/%s/devices/%s", project_id, location, registry_id, device_id);

    pub_topic = (char *)malloc(strlen("/devices/") + strlen(device_id) + strlen("/events") + 1);
    sprintf(pub_topic, "/devices/%s/events", device_id);

    clientPass = CreateJwt(project_id, timeClient.getEpochTime(), priv_key);

    client.setServer(GOOGLE_MQTT_SERVER, GOOGLE_MQTT_PORT);
    client.setPublishQos(MQTTQOS1);
    client.waitForAck(true);
  }

  Serial.println("client connecting...");
  if (client.connect(mqtt_id, clientUser, clientPass.c_str())) {
      for(int i = 0; i < count; i++) {
        memset(payload, 0x0, 64);
        sprintf(payload, "This is Ameba's %d message!!", i);
        printf("Publishing the payload \"%s\" with len: %d\r\n", payload, strlen(payload));
        ret = client.publish(pub_topic, payload);
        if(ret == 1)
          printf("client publish successfully!!! ret = %d\r\n",ret);
        else {
          printf("client publish failed!!! ret = %d\r\n",ret);
          break;
        }
        if (!client.connected()) {
          Serial.println("MQTT disconnected from server");
          break;
        }

        delay(1000);
      }
      client.disconnect();
  }

  free(mqtt_id);
  free(pub_topic);

  timeClient.update();
}

